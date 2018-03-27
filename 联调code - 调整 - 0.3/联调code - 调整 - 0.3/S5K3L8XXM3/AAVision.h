/***********************************************************************
CAAvision�ࣺ��װ���Ӿ���
***********************************************************************/

#pragma once
#include <string>
#include <Windows.h>
#include "imagekit.h"

using namespace std;


/*enum VISIONSTATUS
{
	TESTOR_START =1,
	TESTOR_STOP

};*/
#define TESTOR_START				1
#define TESTOR_STOP					2

#define threadNUM					8

typedef  struct 
{
	int x;
	int y;
}AAVISIONPOINT,*PAAVISIONPOINT;

#define GRAPPITURENUM  10 //ץͼʱץ����
class CAAVision
{
	
public:
	CAAVision(string name,int defineid/*,string inifilePath*/);
	virtual ~CAAVision();
	static int EnumerateAAVision(char *DeviceName[],int iDeviceNumMax,int *pDeviceNum);
	int OpenVisionDevice();
	int CloseVisionDevice();

	BOOL TripleBufferFlipA(LPBYTE *pBuf, BOOL bPrimaryBuffer);


	int InitCamera();//����ƵҪ��ʼ�����,ץͼҲҪ����Ƶ

	int UinitCamera();//����ƵҪ��ʼ�����,ץͼҲҪ����Ƶ
	
	int OpenVisionVideo(HWND hWnd);
	int CloseVisionVideo();

	LPBYTE GrapOnePicture();
	int SaveOnePicture(); //��ʱ����ʵ��,��ͼ��ץȡ���ص�dlgȥ����

	int GetVisionDeviceSN(BYTE *pSN,int iBufferSize,int *pRetLen);

	int SetSensorTab(SensorTab& pCurrentSensor);

	static BOOL SetPaintFlag(BOOL flag);


	//static void PaintLine(BYTE* pBmp,FrameInfo& FrameInfo,int lineLength = 50,int lineWidth = 10);//��ˮƽ������ֱ��
	
	void PaintCrossCenter(BYTE* pBmp,FrameInfo& FrameInfo,int lineLength = 50,int lineWidth = 10);//�����Ļ�ʮ�ֽ�����
	 void PaintSpecialRetangle(BYTE* pBmp,FrameInfo& FrameInfo,int lineLength = 50,int lineWidth = 10);//
	 void PaintSpecialLine(BYTE* pBmp,FrameInfo& FrameInfo,int x,int y,int lineLength ,int lineWidth ,int color);//��ˮƽ������ֱ��,��������
	//1,blue;2,green,3,red

	//type =0 ˮƽ�ߣ�type = 1��ֱ�� color:1,2,3 blue green red
	void PaintLine(BYTE* pBmp,FrameInfo& FrameInfo,int x,int y,int Length ,int lineWidth ,int type,int color);//
	void PaintRetangle(BYTE* pBmp,FrameInfo& FrameInfo,int x,int y,int Length ,int width,int lineWidth ,int color);//
	void DecisionRectByTemplateType(int temPtype,int smallRectsize,int bigRectsize,int piclength,int picwidth,int &x,int &y,int &bigx,int &bigy);
	static int savepic;

	//�����߳��еļ���ȫ�ֱ���
	FrameInfo m_FrameInfoA;
	UINT AAFrameCnt;
	UINT AAPlayCnt;
	//Ϊ�����,��Щ�����ȶ�����Ϊpublic���������Ż�
	static BOOL        m_paintflag;   //��ͼ
	int			m_iDevIDA;		// �û�ָ����������豸��ID��

	int			m_iCurIDA; //�����id
	
	int			m_iWorkStatusA; //��ʶ�Ѿ�������Ƶ
	BYTE		pSN[32];
	SensorTab	CurrentSensorA;
	string		m_pSensorNameA;
	BOOL  m_isTV;

	LPBYTE		m_pTripleBufferA;  //buffer between display and camera 
	LPBYTE		m_pDisplayBufferA; //display buffer
	LPBYTE		m_pCameraBufferA;  //camera buffer.
	USHORT		m_PreviewWidthA;
	USHORT		m_PreviewHeightA;
	ULONG		m_GrabSizeA;
	float		m_fMclkA;
	float		m_fAvddA;
	float		m_fDvddA;
	float		m_fDovddA;
	float		m_fAfvccA;
	float       m_vppA;
	UINT		m_uFocusPosA;




	//int			m_iCurIDA;		// �������������豸��ID��
	
	BOOL		m_bEnableOpenA;
	

	string		m_sFwFileFolderA;
	string		m_strUserNameA;/* �û��Զ������� */
	
	USHORT*		m_pSensorParaA;
	USHORT*		m_pSensorSleepParaA;
	USHORT*		m_pAF_InitParaListA;
	USHORT*		m_pAF_AutoParaListA;
	USHORT*		m_pAF_FarParaListA;
	USHORT*		m_pAF_NearParaListA;
	USHORT*		m_pExposure_ParaListA;
	USHORT*		m_pGain_ParaListA;

	

	HANDLE		m_hCameraThreadEventDeadA; //thread for video 
	HANDLE		m_hDisplayThreadEventDeadA;//thread for display
	BOOL		m_bTripleBufferDirtyA;     //video update flag

	//����8���߳�
	HANDLE CameraHandleA[threadNUM] ;
	HANDLE DisplayHandleA[threadNUM] ;


	string		m_strIniPathName;
	string		m_sADInfoA;



	float		m_fFrameFpsA;
	int			m_iFrameErrA;
	int			m_iFrameCntA;

	CRITICAL_SECTION	m_csLock; 

	//Cstring m_strCurrentDeviceA;


	


	



private:
	void KillDataBuffer(BYTE byChannel = CHANNEL_A);
	int SetPinDefinition();//first set pin definition...
	BOOL SetVoltageMclk(int iDevID,float Mclk,float Avdd,float Dvdd,float Dovdd, float Afvcc, float vpp);

	void InitSomeParam();

	LPBYTE GrapOnePictureWhenVideo();

	void PaintColor(BYTE* pBmp,FrameInfo& FrameInfo,int x,int y,int width ,int height ,int type,int color);//

	//BOOL LoadLibIniFile(pSensorTab pCurrentSensor,BYTE byChannel);//�Ȳ���
};