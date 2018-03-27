
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
	int visionid;
	int robotid1;//֪ͨ���ݻ�����id
	int robotid2;//֪ͨ���ݻ�����id
};


enum notifyEvent
{
	STARTAA,
	RESETAA
};
typedef vector<int> (WINAPI *WorkNotifyFun)(int, AANotifyData, void*);


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