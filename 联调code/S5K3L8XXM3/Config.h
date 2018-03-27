/*********************************************************************
    *�ļ�˵��: ��ģ��֮����Ϣ���ݹ���ʵ��;
        ʹ��postmsgʱ���Զ������̡߳�
    *����: luojing
    *����: 2015/08/21
    *�޸���־:  2015/08/21 CW0793 ����
    ***********************************************************************/
#pragma once
#ifndef DLL_INCLUDE_COMM_CONFIG
#define DLL_INCLUDE_COMM_CONFIG
#include <string>
#include <hash_map>
using namespace std;
using stdext::hash_map;

typedef void* Handle;//gpz

#define CFG_INSTANCE CConfig::GetInstance()

size_t char_hash_value(const char* str);

class char_hash_compare: public stdext::hash_compare<char*>
{
public:
    size_t operator()(const char* _Key) const
    {
        return ((size_t)char_hash_value(_Key));
    }
    bool operator()(const char* _Keyval1, const char* _Keyval2) const
    {
        return (strcmp(_Keyval1, _Keyval2) ? true : false);
    }
};

typedef hash_map<char*, char*, char_hash_compare> char_hash_map;
typedef hash_map<string, FILE*> File_hash_map;

class CConfig
{
public:
    CConfig(void);
    virtual ~CConfig(void);

    static CConfig* GetInstance();
    static void ReleaseInstance();

    int ModifyCfg(const char* key, const char* name, const char* lpValue, int bSave);
    const char* GetCfg(const char* key, const char* name);

	///�����ӿ�
	int  get_cfg_int(const char* key, const char* name);
	double  get_cfg_double(const char* key, const char* name);
	const char*  get_cfg(const char* key, const char* name);

	int  modify_cfg_int(const char* key, const char* name, int iValue, int bSave= 0);
	int  modify_cfg_double(const char* key, const char* name, double iValue, int bSave = 0);
	int  modify_cfg(const char* key, const char* name, const char* lpValue, int bSave =0);

	//�����ӿ� ��дcsv�ļ�
	//д�����ݵ�ָ���ļ�
	 int  write_file(const char* file, const char *pszFormat, ...);

	int Write(const char* file, const char* pszContent, int len);
	int Close(const char* file);
	int  is_file_exist(const char* filepath);
	int  token_full_path(const char* fullPath, char* szPath, char* szFile);
	char*  comm_strncpy(char* src, const char* dest, unsigned int len, char ch=0);
	char*  comm_strcpy(char* src, const char* dest, char ch = 0);
	int  create_path(const char* path);

protected:

    void ReadConfig();
    int SaveConfig();

    //��ͼ���е��ֶν������飬 key+name / value   --> [key] / name = value
    void KeyToBuff(const char* src, const char* pvalue, char* pkey, char* pkeyVal);

    //��ȡһ�� xx=??? ����� xx�ַ��� �� ???�ַ��� , ������2����=�� ����Ϊ����
    bool GetKeyAndValue(const char* src, char* pKey, char* pValue);

    //��ȡһ��[xxx] ����� xxx�ַ�, ������2��[] ����Ϊ����
    bool GetKeyBuff(const char* src, char* pBuff);

    void InsertCfg(const char* key, const char* cfg);
    void ReleaseCfg();

	FILE* Open(const char* file);   //��һ���ļ�
	void ReleaseFile();//�ͷ��ļ�

private:
    static CConfig*             m_sInstance;

    char_hash_map               m_hash_cfg;     //������������������
    Handle                      m_hSection;     //�ٽ���

	File_hash_map       m_hash_file;    //�����ļ�
};

#endif  //��ֹ�ظ�����
