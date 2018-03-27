#include <Windows.h>
#include <vector>
//#include "comm.h"
#include "Config.h"

using std::vector;
using std::pair;

#define USER_CFG_PATH   "AAconfig.dat"
#define KEY_LEN     64      //�����ֶγ���
#define VALUE_LEN   1024    //����ֵ����

#ifndef RETURN_CHK(p, v)

#define RETURN_CHK(p, v) if (!(p)) \
{\
	return (v); \
}
#endif

#ifndef ZERO_CHK(p)

#define ZERO_CHK(p) if (!(p)) \
{    \
	return; \
}
#endif
Handle WINAPI sys_InitializeCriticalSection()
{
    CRITICAL_SECTION* pCSection = new CRITICAL_SECTION;
    //handle_push(pCSection); //ѹ��ջ����
    InitializeCriticalSection(pCSection);
    return pCSection;
}

void WINAPI sys_EnterCriticalSection(Handle hSection)
{
    EnterCriticalSection((CRITICAL_SECTION*)hSection);
}

void WINAPI sys_LeaveCriticalSection(Handle hSection)
{
    LeaveCriticalSection((CRITICAL_SECTION*)hSection);
}

void WINAPI sys_DeleteCriticalSection(Handle hSection)
{
    /*if (handle_erase(hSection))
    {*/
        CRITICAL_SECTION* pCSection = (CRITICAL_SECTION*)hSection;
        DeleteCriticalSection(pCSection);
        delete pCSection;
    //}
}

 int WINAPI get_char_cnt(const char* src, char ch)
{
	const char* tmp = src;
	int nCnt = 0;

	RETURN_CHK(src, 0);

	while(0 != *tmp)
	{
		if (*tmp++ == ch)
		{
			nCnt++;
		}
	}
	return nCnt;
}
 void WINAPI trim_char(char* pbuff, const char* key)
 {
	 int index = 0;
	 const char* ptmp = key;

	 ZERO_CHK(pbuff && key);

	 index = strlen(pbuff) - 1;

	 ZERO_CHK(index >= 0);

	 //ɾ��β��
	 while (index >= 0)
	 {
		 if (!strchr(key, pbuff[index]))
		 {//�������ַ�
			 break;
		 }
		 pbuff[index--] = 0;
	 }

	 //ɾ��ͷ��
	 index = 0;
	 while (pbuff[index])
	 {
		 if (!strchr(key, pbuff[index]))
		 {
			 break;
		 }
		 index++;
	 }

	 ZERO_CHK(index);    //���������ַ����˳�

	 //��ǰ����
	 ptmp = &pbuff[index];
	 while (*ptmp)
	 {
		 *pbuff++ = *ptmp++;
	 }
	 *pbuff = 0;
 }

size_t char_hash_value(const char* str)
{
    size_t value = _HASH_SEED;
    size_t size = strlen(str);
    if (size > 0)
    {
        size_t temp = (size/16) + 1;
        size -= temp;
        for (size_t idx=0; idx<=size; idx+=temp)
        {
            value += (size_t)str[(int)idx];
        }
    }
    return value;
}

struct TSAVE_DATA
{
    char key[KEY_LEN];
    vector<char*> vecValue;
};

CConfig* CConfig::m_sInstance = 0;

bool g_bInitComm = false;
CConfig::CConfig(void)
{
	g_bInitComm = true;//new 
    m_hSection = sys_InitializeCriticalSection();
    ReadConfig();
}

CConfig::~CConfig(void)
{
    sys_EnterCriticalSection(m_hSection);
    SaveConfig();
    ReleaseCfg();
    sys_LeaveCriticalSection(m_hSection);
    sys_DeleteCriticalSection(m_hSection);
	g_bInitComm = false;//new

}

//��ȡʵ��
CConfig* CConfig::GetInstance()
{
    if(NULL == m_sInstance)
    {
        m_sInstance = new CConfig();
    }
    return m_sInstance;
}

//ɾ��ʵ��
void CConfig::ReleaseInstance()
{
    ////LOG_INF2("CConfig::ReleaseInstance()");
    ////FREE_ANY(m_sInstance);
	if (m_sInstance!=NULL)
	{
		delete m_sInstance;
		m_sInstance = NULL;
	}
}

//�ⲿ����
void CConfig::InsertCfg(const char* key, const char* cfg)
{
    size_t nLen = strlen(cfg) + 1;
    char* pBuff = new char[KEY_LEN];
    char* pVal = new char[nLen];

    strcpy_s(pBuff, KEY_LEN, key);
    strcpy_s(pVal, nLen, cfg);
    m_hash_cfg.insert(pair<char*, char*>(pBuff, pVal));
}

//�ⲿ����
void CConfig::ReleaseCfg()
{
    hash_map<char*, char*>::iterator it;
    for(it = m_hash_cfg.begin(); it != m_hash_cfg.end(); it++)
    {
        delete it->first;
        delete it->second;
    }
    m_hash_cfg.clear();
}

int CConfig::ModifyCfg(const char* key, const char* name, const char* lpValue, int bSave)
{
    int ret = 0;
    size_t nLen = 0;
    char buff[KEY_LEN] = {0};
    hash_map<char*, char*>::iterator it;

    RETURN_CHK(key && name && lpValue, -1);

    sprintf_s(buff, "%s/%s", key, name);
    sys_EnterCriticalSection(m_hSection);
    it = m_hash_cfg.find(buff);
    if (it != m_hash_cfg.end())
    {
        nLen = strlen(lpValue);
        if (0 == nLen)
        {
            m_hash_cfg.erase(it);
        }
        else
        {
            if (nLen > strlen(it->second))
            {
                delete it->second;
                it->second = new char[nLen + 1];
            }
            strcpy_s(it->second, nLen + 1, lpValue);
        }
    }
    else
    {
        InsertCfg(buff, lpValue);
    }

    if (bSave)
    {
        ret = SaveConfig();
    }
    sys_LeaveCriticalSection(m_hSection);

    return ret;
}

const char* CConfig::GetCfg(const char* key, const char* name)
{
    char buff[KEY_LEN] = {0};
    const char* ptmp = NULL;
    hash_map<char*, char*>::iterator it;

    RETURN_CHK(key && name, NULL);

    sprintf_s(buff, "%s/%s", key, name);
    sys_EnterCriticalSection(m_hSection);
    it = m_hash_cfg.find(buff);
    if (it != m_hash_cfg.end())
    {
        ptmp = it->second;
    }
    sys_LeaveCriticalSection(m_hSection);

    return ptmp;
}

void CConfig::ReadConfig()
{
    errno_t er = 0;
    FILE* fp = NULL;
    char prevBuff[VALUE_LEN] = {0};
    char buff[VALUE_LEN] = {0};
    char key[KEY_LEN] = {0};
    char pvalue[VALUE_LEN] = {0};
    hash_map<char*, char*>::iterator it;

    er = fopen_s(&fp, USER_CFG_PATH, "r");   //�������ļ������������򴴽�
    if (fp == NULL)
    {
       // LOG_ERR("open %s error..", USER_CFG_PATH);
        return;
    }

    sys_EnterCriticalSection(m_hSection);
    ReleaseCfg();   //���ͷ�
    while(!feof(fp))    //�ļ�ָ��δ��β��,����ѭ��
    {
        fgets(buff, VALUE_LEN, fp);
        if (*buff == '[')   //key
        {
            if (!GetKeyBuff(buff + 1, prevBuff))
            {
                continue;
            }
        }
        else if (*prevBuff && GetKeyAndValue(buff, key, pvalue))
        {//prevBuff ���벻Ϊ�գ����ܶ�ȡ
            sprintf_s(buff, "%s/%s", prevBuff, key);
            it = m_hash_cfg.find(buff);
            if (it == m_hash_cfg.end())
            {//�����ظ�
                InsertCfg(buff, pvalue);
            }
        }
    }
    sys_LeaveCriticalSection(m_hSection);
    fclose(fp);
}

//�ⲿ����
int CConfig::SaveConfig()
{
    errno_t er = 0;
    FILE* fp = NULL;
    bool bFind = false;
    char key[KEY_LEN] = {0};
    char pvalue[VALUE_LEN] = {0};
    char* pTmp = NULL;
    TSAVE_DATA* pdata = NULL;
    vector<TSAVE_DATA*> vecKey;
    hash_map<char*, char*>::iterator it;
    vector<TSAVE_DATA*>::iterator itData;
    vector<char*>::iterator itValue;

    if (0 == m_hash_cfg.size())
    {//����Ҫ����
        return 0;
    }

    er = fopen_s(&fp, USER_CFG_PATH, "w");   //�������ļ������������򴴽�
    if (fp == NULL)
    {
       // LOG_ERR("open %s for write error..", USER_CFG_PATH);
        return -2;
    }

    //�ȱ��浽vector
    for(it = m_hash_cfg.begin(); it != m_hash_cfg.end(); it++)
    {
        bFind = false;
        KeyToBuff(it->first, it->second, key, pvalue);
        for(itData = vecKey.begin(); itData != vecKey.end(); itData++)
        {
            if (0 == strcmp((*itData)->key, key))
            {
                pTmp = new char[VALUE_LEN];
                strcpy_s(pTmp, VALUE_LEN, pvalue);
                (*itData)->vecValue.push_back(pTmp);
                bFind = true;
                break;
            }
        }

        if (!bFind)
        {
            pdata = new TSAVE_DATA;
            strcpy_s(pdata->key, key);
            pTmp = new char[VALUE_LEN];
            strcpy_s(pTmp, VALUE_LEN, pvalue);
            pdata->vecValue.push_back(pTmp);
            vecKey.push_back(pdata);
        }
    }

    for(itData = vecKey.begin(); itData != vecKey.end(); itData++)
    {
        fwrite((*itData)->key, strlen((*itData)->key), 1, fp);
        for(itValue = (*itData)->vecValue.begin(); itValue != (*itData)->vecValue.end(); itValue++)
        {
            fwrite(*itValue, strlen(*itValue), 1, fp);
            delete *itValue;
        }
        delete *itData;
        fwrite("\n", 1, 1, fp);
    }

    fclose(fp);
    return 0;
}

//��ͼ���е��ֶν������飬
//key+name / value   --> [key] / name = value
void CConfig::KeyToBuff(const char* src, const char* pvalue, char* pkey, char* pkeyVal)
{
    *pkey++ = '[';
    while(*src != '/')
    {
        *pkey++ = *src++;
    }
    *pkey++ = ']';
    *pkey++ = '\n';
    *pkey = 0;
    src++;  //���� "/"
    sprintf_s(pkeyVal, VALUE_LEN, "%s = %s\n", src, pvalue);
}

//��ȡһ�� xx=??? ����� xx�ַ��� �� ???�ַ��� , ������2����=�� ����Ϊ����
bool CConfig::GetKeyAndValue(const char* src, char* pKey, char* pValue)
{
    char* tmp = pKey;
    if (1 != get_char_cnt(src, '=') ||
        0 != get_char_cnt(src, '/'))
    {
        return false;
    }

    while(*src != '=')
    {
        *tmp++ = *src++;
    }
    *tmp = 0;

    src++;  //����'='
    tmp = pValue;
    while(*src != 13 && *src != 10 && *src != 0)
    {
        *tmp++ = *src++;
    }
    *tmp = 0;

    trim_char(pKey," ");
    trim_char(pValue," ");
    return strlen(pKey) && strlen(pValue);
}

//��ȡһ��[xxx] ����� xxx�ַ�, ������2��[] ����Ϊ����
bool CConfig::GetKeyBuff(const char* src, char* pBuff)
{
    char* tmp = pBuff;
    if (0 != get_char_cnt(src, '[') ||
        1 != get_char_cnt(src, ']') ||
        0 != get_char_cnt(src, '/'))
    {
        return false;
    }

    while(*src != ']')
    {
        *tmp++ = *src++;
    }

    *tmp = 0;
    trim_char(pBuff," ");
    return strlen(pBuff) == 0 ? false : true;
}


 int  CConfig::modify_cfg_int(const char* key, const char* name, int iValue, int bSave)
{
	char buff[32] = {0};
	RETURN_CHK(g_bInitComm, -1);
	sprintf_s(buff, "%d", iValue);
	return CFG_INSTANCE->ModifyCfg(key, name, buff, bSave);
}

  const char* CConfig:: get_cfg(const char* key, const char* name)
 {
	 RETURN_CHK(g_bInitComm, NULL);
	 return CFG_INSTANCE->GetCfg(key, name);
 }
   int  CConfig::get_cfg_int(const char* key, const char* name)
  {
	  RETURN_CHK(g_bInitComm, 0);
	  const char* ptmp = CFG_INSTANCE->GetCfg(key, name);
	  return ptmp ? atoi(ptmp) : 0;
  }
    int  CConfig::modify_cfg(const char* key, const char* name, const char* lpValue, int bSave)
   {
	   RETURN_CHK(g_bInitComm, -1);
	   return CFG_INSTANCE->ModifyCfg(key, name, lpValue, bSave);
   }
   int  CConfig::modify_cfg_double(const char* key, const char* name, double iValue, int bSave/* = 0*/)
  {
	  char buff[32] = {0};
	  RETURN_CHK(g_bInitComm, -1);
	  sprintf_s(buff, "%.5lf", iValue);
	  return CFG_INSTANCE->ModifyCfg(key, name, buff, bSave);
  }

   double CConfig::get_cfg_double(const char* key, const char* name)
  {
	  RETURN_CHK(g_bInitComm, 0);
	  const char* ptmp = CFG_INSTANCE->GetCfg(key, name);
	  return ptmp ? atof(ptmp) : 0.0f;
  }