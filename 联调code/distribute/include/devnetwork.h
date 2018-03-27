
#pragma once
#ifndef DLL_INCLUDE_SDK_DEVNETWORK_H
#define DLL_INCLUDE_SDK_DEVNETWORK_H


#ifndef WINAPI
#define WINAPI __stdcall
#endif

#ifndef EXPORT_C
#define EXPORT_C extern "C"
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#include<vector>
using namespace std;

struct AANotifyData
{//�ϱ����ݣ�����NotifyEvent �������ݸ�ʽ
	int funCode;
	int robotid1;//֪ͨ���ݻ�����id
	int robotid2;//֪ͨ���ݻ�����id
	//double th[3];//���: u��v��w
};

struct AANotifyResult
{//�ϱ����ݣ�����NotifyEvent �������ݸ�ʽ
	int funID;  //���źб�ţ�����˵��Լ���Ĺ��ܺţ����ܺ��ǱȽ������ģ����������������Ӷ�Ӧһ����λ����ô�������Ĺ��ܺþͲ����������Ǹ�����
	int result; //�Ƿ��쳣
	int type;   //�������ͣ�Ϊ�˼����ǻ����˵����������������hsldlg������޸Ĵ�ֵ
	int isfinsh; //�Ƿ������
	double tha[3];//���: u��v��w
	double thb[3];//���: u��v��w
};


enum notifyEvent
{
	LIGHT, //����ģ��
	DAA,    //˫�����
	//����Ϊ��ָ��
	STARTAA,
	RESETAA
};

#define INVALID_VALUE -1
#define DATA_SEG_FLAG   ","            //��������
#define SPLIT_FLAG      "|"            //�ָ����
#define END_CHAR        '#'
#define REPLY_TIMEOUT   5000

#define SENDHEAD "C|"
#define RECVHEAD "S|"


//ָ���

#define LightA "Light"
#define DAAA "AA"
#define StartAAA "StartAA"
#define ResetAAA "ResetAA"





typedef vector<AANotifyResult> (WINAPI *WorkNotifyFun)(int, AANotifyData, void*);


EXPORT_C int WINAPI init_devnetwork();
EXPORT_C void WINAPI uninit_devnetwork();

EXPORT_C void WINAPI test();



/****************************************************
    *Function: ע�����������ϱ��ص�����
    *Intput: fun �ص�������ַ
             param ���Ӳ���
    *Output: NULL
    *Return: �ɹ�����0, ʧ�ܷ��ش�����
******************************************************/
EXPORT_C int WINAPI regist_work_notify(WorkNotifyFun fun, void* param);





#endif