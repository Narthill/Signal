/*********************************************************************
    *文件说明: 各模块之间消息传递功能实现;
        使用postmsg时会自动创建线程。
    *作者: luojing
    *日期: 2015/08/21
    *修改日志:  2015/08/21 CW0793 创建
    ***********************************************************************/
#pragma once
#ifndef DLL_INCLUDE_COMM_CONFIG
#define DLL_INCLUDE_COMM_CONFIG
//#include "sysapi.h"
#include <hash_map>

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

class CConfig
{
public:
    CConfig(void);
    virtual ~CConfig(void);

    static CConfig* GetInstance();
    static void ReleaseInstance();

    int ModifyCfg(const char* key, const char* name, const char* lpValue, int bSave);
    const char* GetCfg(const char* key, const char* name);

	///新增接口
	int  get_cfg_int(const char* key, const char* name);
	double  get_cfg_double(const char* key, const char* name);
	const char*  get_cfg(const char* key, const char* name);

	int  modify_cfg_int(const char* key, const char* name, int iValue, int bSave= 0);
	int  modify_cfg_double(const char* key, const char* name, double iValue, int bSave = 0);
	int  modify_cfg(const char* key, const char* name, const char* lpValue, int bSave =0);

protected:

    void ReadConfig();
    int SaveConfig();

    //把图表中的字段进行重组， key+name / value   --> [key] / name = value
    void KeyToBuff(const char* src, const char* pvalue, char* pkey, char* pkeyVal);

    //获取一个 xx=??? 里面的 xx字符串 和 ???字符串 , 若出现2个‘=’ 则认为错误
    bool GetKeyAndValue(const char* src, char* pKey, char* pValue);

    //获取一个[xxx] 里面的 xxx字符, 若出现2个[] 则认为错误
    bool GetKeyBuff(const char* src, char* pBuff);

    void InsertCfg(const char* key, const char* cfg);
    void ReleaseCfg();

private:
    static CConfig*             m_sInstance;

    char_hash_map               m_hash_cfg;     //用来缓存所有配置项
    Handle                      m_hSection;     //临界区
};

#endif  //防止重复包含
