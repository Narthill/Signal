#include "AAvision.h"
#include "dtccm2.h" 

#include "AA_define_tool.h"
#include "AAvisionthreads.h"
#include <process.h>

//#include "HalconCpp.h"
//using namespace HalconCpp;

//#include "IniFileRW.h"//�����д�����ļ��Ƿ����,��������
#include "Config.h"
int CAAVision::savepic = 0;

extern  AA_Globle_Param g_AAParam;//����ȫ�ֱ���

BOOL   CAAVision::m_paintflag = FALSE;
CAAVision::~CAAVision()
{
	//
	EnterCriticalSection(&m_windcsLock);
	if (halconWinID>0)
	{
		CloseWindow(halconWinID);
		halconWinID = -1;
	}
	LeaveCriticalSection(&m_windcsLock);
	DeleteCriticalSection(&m_windcsLock);
	//���
	if (CurrentSensorA.ParaList)
	{
		delete CurrentSensorA.ParaList;
	}
	//�����Ĳ�������û���õ�,��������

	DeleteCriticalSection(&m_csLock);
	

	//�������������
	UinitSomeParam();

}

CAAVision::CAAVision(string name,int defineid/*,string inifilePath*/)
{
	AAFrameCnt = 0;
	AAPlayCnt =0;
	m_pSensorNameA = name;
	m_iWorkStatusA = TESTOR_STOP;
	m_iDevIDA =-1;
	m_iCurIDA = defineid;

	m_pTripleBufferA = NULL;
	m_pDisplayBufferA = NULL;
	m_pCameraBufferA = NULL;
	m_GrabSizeA = 0;

	m_isTV = FALSE ;//��ʾ����ץͼ��ͼ���Сһ��,ûʲô����,����
	m_firstopenvideo = true;
	halconWinID = -1;
	m_videowintype = false;
	savepic = CFG_INSTANCE->get_cfg_int(AACoreCfg,savepicture); //�Ƿ񱣴�ͼƬ
	InitializeCriticalSection(&m_csLock);
	InitializeCriticalSection(&m_windcsLock);
	InitSomeParam();
}
void CAAVision::UinitSomeParam()
{
	string temp = m_pSensorNameA.substr(m_pSensorNameA.length()-1,1);
	string str;
	int result = 0;
	/* str = Mclk;
	str.append(temp);
	CFG_INSTANCE->modify_cfg_double(AAVISION_MCLK,str.c_str(),m_fMclkA);

	str = Avdd;
	str.append(temp);
	CFG_INSTANCE->modify_cfg_double(AAVISION_POWER,str.c_str(),m_fAvddA*1000.0);
	str = Dvdd;
	str.append(temp);
	CFG_INSTANCE->modify_cfg_double(AAVISION_POWER,str.c_str(),m_fDvddA*1000.0);
	str = Dovdd;
	str.append(temp);
	CFG_INSTANCE->modify_cfg_double(AAVISION_POWER,str.c_str(),m_fDovddA*1000.0);
	str = Afvcc;
	str.append(temp);
	CFG_INSTANCE->modify_cfg_double(AAVISION_POWER,str.c_str(),m_fAfvccA*1000.0);
	str = Vpp;
	str.append(temp);
	CFG_INSTANCE->modify_cfg_double(AAVISION_POWER,str.c_str(),m_vppA*1000.0);
	str = FocusPos;
	str.append(temp);
	CFG_INSTANCE->modify_cfg_int(AAVISION_AF,str.c_str(),m_uFocusPosA);
	*/
	//������ع�
	str = AAGRAIN;
	str.append(temp);
	result = CFG_INSTANCE->modify_cfg_int(AAVISION_GRAIN,str.c_str(),m_grain);

	str = AAEXPOSURE;
	str.append(temp);
	result = CFG_INSTANCE->modify_cfg_int(AAVISION_EXPOSURE,str.c_str(),m_exposure);

}
void CAAVision::InitSomeParam()
{
	string temp = m_pSensorNameA.substr(m_pSensorNameA.length()-1,1);
	string str;
	str = Mclk;
	str.append(temp);
	m_fMclkA = CFG_INSTANCE->get_cfg_double(AAVISION_MCLK,str.c_str());

	str = Avdd;
	str.append(temp);
	m_fAvddA = CFG_INSTANCE->get_cfg_double(AAVISION_POWER,str.c_str())/1000;
	str = Dvdd;
	str.append(temp);
	m_fDvddA = CFG_INSTANCE->get_cfg_double(AAVISION_POWER,str.c_str())/1000;
	str = Dovdd;
	str.append(temp);
	m_fDovddA = CFG_INSTANCE->get_cfg_double(AAVISION_POWER,str.c_str())/1000;
	str = Afvcc;
	str.append(temp);
	m_fAfvccA = CFG_INSTANCE->get_cfg_double(AAVISION_POWER,str.c_str())/1000;
	str = Vpp;
	str.append(temp);
	m_vppA = CFG_INSTANCE->get_cfg_double(AAVISION_POWER,str.c_str())/1000;
	str = FocusPos;
	str.append(temp);
	m_uFocusPosA = CFG_INSTANCE->get_cfg_int(AAVISION_AF,str.c_str());

	//������ع�
	str = AAGRAIN;
	str.append(temp);
	m_grain = CFG_INSTANCE->get_cfg_int(AAVISION_GRAIN,str.c_str());

	str = AAEXPOSURE;
	str.append(temp);
	m_exposure = CFG_INSTANCE->get_cfg_int(AAVISION_EXPOSURE,str.c_str());


}

int CAAVision::EnumerateAAVision(char *DeviceName[],int iDeviceNumMax,int *pDeviceNum)
{

	return EnumerateDevice(DeviceName,iDeviceNumMax,pDeviceNum);

}

int CAAVision::OpenVisionDevice()
{
	int ret = DT_ERROR_OK;
	int iDevIDA = -1;

	if (m_iWorkStatusA == TESTOR_START) //�����Ѿ�����Ƶ��,�ǿ϶��Ǵ����豸
	{
		//ret = SetAWB(true,m_iDevIDA);
		return ret;
	}
	if (m_iDevIDA>=0)
	{
		//ret = SetAWB(true,m_iDevIDA);
		if (IsDevConnect(m_iDevIDA) == DT_ERROR_OK)
		{

			return ret;
		}	
	}
	if (m_iCurIDA<0)
	{
		return -1;
	}
	if ((ret = OpenDevice(m_pSensorNameA.c_str(),&iDevIDA,m_iCurIDA)) != DT_ERROR_OK)
	{
		PRINTF("Open %s failed.\r\n",m_pSensorNameA.c_str());
		return ret;
	}
	if (iDevIDA!=m_iCurIDA)
	{
		PRINTF("Open %s failed����ʱid��Ҫ���id��һ��.\r\n",m_pSensorNameA.c_str());
	}
	m_iDevIDA = iDevIDA;
	return ret;
}
int CAAVision::CloseVisionDevice()
{
	m_iWorkStatusA = TESTOR_STOP;

	m_pTripleBufferA = NULL;
	m_pDisplayBufferA = NULL;
	m_pCameraBufferA = NULL;

	return CloseDevice(m_iDevIDA);
}


int CAAVision::GetSensorRegValue(int type)
{
	int result = DT_ERROR_OK;
	USHORT value = 0;
	USHORT uReg = 0;

	switch(type)
	{
	case AA_EXP: //�ع�
		uReg = 0x0202;
		break;
	case AA_GRAIN:
		uReg = 0x0204;
		break;
	default:
		return -1;
	}
	//UCHAR uAddr = 0x20; //����21 дʫ20 ,���Ŷ�д����20
	//BYTE mode = 3;
	result = ReadSensorReg(CurrentSensorA.SlaveID,uReg,&value,CurrentSensorA.mode,m_iDevIDA);
	if (result == DT_ERROR_OK)
	{
		return value;
	}
	else
	{
		return -1;
	}

}
UINT CAAVision::GetInitRegValue(int type)
{
	UINT value = 0;
	switch(type)
	{
	case AA_EXP: //�ع�
		value = m_exposure;
	case AA_GRAIN:
		value = m_grain;
		break;
	default:
		value = -1;
	}
	return value;

}

int CAAVision::SetSensorRegValue(int type,USHORT value)
{
	int result = DT_ERROR_OK;
	USHORT uReg = 0;

	switch(type)
	{
	case AA_EXP: //�ع�
		uReg = 0x0202;
		break;
	case AA_GRAIN:
		uReg = 0x0204;
		break;
	default:
		return -1;
	}


	result = WriteSensorReg(CurrentSensorA.SlaveID,uReg,value,CurrentSensorA.mode,m_iDevIDA);
	if (result == DT_ERROR_OK)
	{
		switch(type)
		{
		case AA_GRAIN: //�ع�
			m_grain = value;
			break;
		case AA_EXP:
			m_exposure = value;
			break;
		default:
			break ;
		}
	}
	else
	{
		result =  -1;
	}



	return result;
}

BOOL CAAVision::TripleBufferFlipA(LPBYTE *pBuf, BOOL bPrimaryBuffer)
{
	BOOL bFlipped = FALSE;

	EnterCriticalSection(&m_csLock);
	{
		if (bPrimaryBuffer || m_bTripleBufferDirtyA)
		{
			LPBYTE pTmp = *pBuf;
			*pBuf = m_pTripleBufferA;
			m_pTripleBufferA = pTmp;
			bFlipped = TRUE;
		}
		m_bTripleBufferDirtyA = bPrimaryBuffer;
	}
	LeaveCriticalSection(&m_csLock);

	return bFlipped;	
}
void CAAVision::KillDataBuffer(BYTE byChannel)//ͨ��������û���û������,��ʱѯ��˵��ֻ��chanlA
{
	//delete the data buffer.
	//if (byChannel == CHANNEL_A)
	{
		if (m_pTripleBufferA)
		{
			free(m_pTripleBufferA);
		}
		if (m_pCameraBufferA)
		{
			free(m_pCameraBufferA);
		}
		if (m_pDisplayBufferA)
		{
			free(m_pDisplayBufferA);
		}

		m_pTripleBufferA = NULL;
		m_pCameraBufferA = NULL;
		m_pDisplayBufferA = NULL;
	}
}
BOOL CAAVision::SetVoltageMclk(int iDevID,float mclk,float avdd,float dvdd,float dovdd,float afvcc,float vpp)
{
	//int ret = DT_ERROR_OK;

	//int iDevID = m_iDevIDA;

	SENSOR_POWER Power[10] = {POWER_AVDD, POWER_DOVDD, POWER_DVDD, POWER_AFVCC, POWER_VPP};
	int Volt[10] = {0};
	int Current[10] = {300, 300, 300, 300, 100};//300mA
	BOOL OnOff[10] = {FALSE};
	CURRENT_RANGE range[5] = {CURRENT_RANGE_MA, CURRENT_RANGE_MA, CURRENT_RANGE_MA, CURRENT_RANGE_MA, CURRENT_RANGE_MA};
	//set power to 0V
	if (PmuSetVoltage(Power, Volt,5, iDevID) != DT_ERROR_OK)
	{
		CloseDevice(iDevID);
		PRINTF("Set Voltage Failed!");
		//return FALSE;
	}
	//wait for the power is all to zero....
	Sleep(50);
	//���õ�ѹ���� switch to on...
	if (PmuSetOnOff(Power, OnOff, 5, iDevID) != DT_ERROR_OK)
	{
		CloseDevice(iDevID);

		PRINTF("Open PowerOnOff Failed!");
		//return FALSE;
	}
	//20160103 power on the sensor...////		//first power the avdd.
	// 1. set power switch to on and power the avdd
	OnOff[POWER_AVDD]=TRUE;
	if (PmuSetOnOff(Power, OnOff, 5, iDevID) != DT_ERROR_OK)
	{
		CloseDevice(iDevID);
		PRINTF("Open PowerOnOff Failed!");
		//return FALSE;
	}
	Volt[POWER_AVDD] = (int)(avdd * 1000); // 2.8V
	if(PmuSetVoltage(Power, Volt, 5, iDevID)!=DT_ERROR_OK)
	{
		PRINTF("Open PowerOn Failed!");
		//return DT_ERROR_FAILED;
	}
	Sleep(1);
	// 2, set power switch to on and power the dvdd 
	OnOff[POWER_DVDD]=TRUE;
	if (PmuSetOnOff(Power, OnOff, 5, iDevID) != DT_ERROR_OK)
	{
		CloseDevice(iDevID);
		PRINTF("Open PowerOnOff Failed!");
		//return FALSE;
	}
	Volt[POWER_DVDD] = (int)(dvdd * 1000); // 2.8V
	if(PmuSetVoltage(Power, Volt, 5, iDevID)!=DT_ERROR_OK)
	{
		PRINTF("Open PowerOn Failed!");
		//return DT_ERROR_FAILED;
	}
	Sleep(2);

	// 3, set power switch to on and power the dovdd
	OnOff[POWER_DOVDD]=TRUE;
	if (PmuSetOnOff(Power, OnOff, 5, iDevID)!= DT_ERROR_OK)
	{
		CloseDevice(iDevID);
		//AfxMessageBox("Close Power Failed!");
		PRINTF("Open PowerOnOff Failed!");
		//return FALSE;
	}
	Volt[POWER_DOVDD] = (int)(dovdd * 1000); // 2.8V
	if(PmuSetVoltage(Power, Volt, 5, iDevID)!=DT_ERROR_OK)
	{
		PRINTF("Open PowerOn Failed!");
		//return DT_ERROR_FAILED;
	}
	Sleep(2);
	//4. set power switch to on and power the afvcc and vpp
	OnOff[POWER_AFVCC]=TRUE;
	OnOff[POWER_VPP]=TRUE;
	if (PmuSetOnOff(Power, OnOff, 5, iDevID) != DT_ERROR_OK)
	{
		CloseDevice(iDevID);
		//AfxMessageBox("Close Power Failed!");
		PRINTF("Open PowerOnOff Failed!");
		//return FALSE;
	}
	Volt[POWER_AFVCC] = (int)(afvcc * 1000); // 2.8V
	Volt[POWER_VPP] = (int)(vpp * 1000); // 2.8V 
	if(PmuSetVoltage(Power, Volt, 5, iDevID)!=DT_ERROR_OK)
	{
		PRINTF("Open PowerOn Failed!");
		return DT_ERROR_FAILED;
	}
	//should wait for 50ms to be ready...
	Sleep(50);
	//���òɼ�����
	int SampleSpeed[5]={100,100,100,100,100};
	PmuSetSampleSpeed(Power,SampleSpeed,5,iDevID);
	Sleep(10);
	//Ϊ�˼���HM�����������̹ʲ����жϷ���ֵ
	{
		// ��������
		PmuSetCurrentRange(Power,range,5,iDevID); 
		//���õ���
		PmuSetOcpCurrentLimit(Power,Current,5,iDevID);
	}
	if (SetSensorClock(TRUE,(USHORT)(mclk*10), iDevID) != DT_ERROR_OK)
	{
		//AfxMessageBox("Set Mclk Failed!");
		PRINTF("Set Mclk Failed!");
		return DT_ERROR_FAILED;
	}
	//����IO��������
	SetSoftPinPullUp(IO_PULLUP, iDevID);
	return TRUE;

}
int CAAVision::UinitCamera()
{
	int ret = DT_ERROR_OK;
	int iDevID = m_iDevIDA;
	//close
	{
		CloseVideo(m_iDevIDA);
		if(1)
		{
			ResetSensorI2cBus(m_iDevIDA);
			SensorEnable(CurrentSensorA.pin ^ 2, 1, m_iDevIDA);       //reset sensor....
			Sleep(20);
			SensorEnable(CurrentSensorA.pin ^ 1, 1, m_iDevIDA);       //power down sensor....
			Sleep(2);
			if(ReadSensorReg(CurrentSensorA.SlaveID, CurrentSensorA.FlagReg, &(CurrentSensorA.FlagData),  CurrentSensorA.mode))
			{
				//if pwdn not work...,just set to reset state...
				SensorEnable(CurrentSensorA.pin^2,1,m_iDevIDA);       //reset sensor....
			}
		}

		//close all power....
		{
			//close the clock
			SetSensorClock(0, 24*10, m_iDevIDA);
			//close the IO power...
			SetSoftPinPullUp(IO_NOPULL, m_iDevIDA);
			//close reset and power down pin...
			SensorEnable(0, 1, m_iDevIDA);
			//�رյ�ѹ
			SENSOR_POWER Power[10]={POWER_AVDD,POWER_DOVDD,POWER_DVDD,POWER_AFVCC,POWER_VPP};
			BOOL OnOff[10] = {0};	
			int Volt[10] = {0};
			//close the power switch...
			//if necessory, you can set the power voltage to 0
			PmuSetVoltage(Power, Volt, 5, m_iDevIDA);
			Sleep(50);
			PmuSetOnOff(Power,OnOff,5,m_iDevIDA);
		}
	}
	KillDataBuffer(CHANNEL_A);

	//����һЩ����

	return ret;

}
int CAAVision::InitCamera()
{
	int ret = DT_ERROR_OK;
	int iDevID = m_iDevIDA;
	//iDevID = m_iCurIDA;
	KillDataBuffer();//�������
	ret = SetSoftPinPullUp(IO_NOPULL, iDevID);//�ر�ʹ����������
	if ((ret = SetSensorClock(FALSE,(USHORT)(0*10), iDevID)) != DT_ERROR_OK)//�ر�����ʱ�� 10û����
	{
		CloseDevice(iDevID);
		PRINTF("Set Mclk Failed!");
		return FALSE;
	}
	Sleep(1);
	ret = SetPinDefinition();//		�������Խӿ�
	ret = EnableSoftPin(TRUE,iDevID);//ʹ�����Խӿں�gpio
	ret = EnableGpio(TRUE,iDevID);
	ret = SetVoltageMclk(iDevID,m_fMclkA, m_fAvddA,m_fDvddA, m_fDovddA, m_fAfvccA, m_vppA);
	Sleep(10);
	//i2C init....
	//����SENSOR I2C������Ϊ400Kbps,������豸ΪStreching mode�����豸�˿�������scl��sda��ƽ����ʾbusy��
	ret = SetSensorI2cRate(I2C_400K, iDevID);
	ret = SetSensorI2cRapid(TRUE, iDevID);

	//check sensor is on line or not ,if on line,init sensor to work....
	ret = SensorEnable(CurrentSensorA.pin ^ 0x02, 1, iDevID); //reset
	Sleep(20);
	ret = SensorEnable(CurrentSensorA.pin, 1, iDevID);
	Sleep(50);
	//check sensoris on line...
	if(SensorIsMe(&CurrentSensorA, CHANNEL_A, 0,iDevID) != DT_ERROR_OK)//Ϊʲô�������Լ�
	{
		PRINTF("Sensor is not ok!");
		//return DT_ERROR_FAILED;			
	}
	//init sensor....
	if(InitSensor(CurrentSensorA.SlaveID,
		CurrentSensorA.ParaList,
		CurrentSensorA.ParaListSize,
		CurrentSensorA.mode,iDevID) != DT_ERROR_OK)
	{
		PRINTF("Init Sensor Failed!");
		return DT_ERROR_FAILED;			
	}

	///2017-9-5���� 
	SetSensorRegValue(AA_GRAIN,m_grain);
	SetSensorRegValue(AA_EXP,m_exposure);

	if(CurrentSensorA.type == D_YUV || CurrentSensorA.type == D_YUV_SPI || CurrentSensorA.type == D_YUV_MTK_S)
		SetYUV422Format(CurrentSensorA.outformat, iDevID);
	else
		SetRawFormat(CurrentSensorA.outformat, iDevID);

	//if (byChannel == CHANNEL_A)
	{
		m_PreviewWidthA = CurrentSensorA.width;
		m_PreviewHeightA = CurrentSensorA.height;//& 0xfffe;
	}
	/*
	if (byChannel == CHANNEL_B)
	{
	m_PreviewWidthB = CurrentSensor.width;
	m_PreviewHeightB = CurrentSensor.height;//& 0xfffe;
	}*/

	USHORT roi_x0 = CurrentSensorA.width >> 2;
	USHORT roi_y0 = CurrentSensorA.height >> 2;
	USHORT roi_hb = 0;
	USHORT roi_vb = 0;
	USHORT roi_hnum = 1;
	USHORT roi_vnum = 1;

	USHORT fifo_div = 2;	
	roi_x0 = 0;
	roi_y0 = 0;
	roi_hb = 0;
	roi_vb = 0;
	roi_hnum = 1;
	roi_vnum = 1;
	//��ʼ���豸
	ret = InitRoi(0, 0, CurrentSensorA.width, CurrentSensorA.height , 0, 0, 1, 1, CurrentSensorA.type, TRUE, iDevID);	//������Ϊ��λ��ȫ�ֱ�����ʾ�ر�ROIʹ��
	ret = SetSensorPort(CurrentSensorA.port, CurrentSensorA.width, CurrentSensorA.height, iDevID);
	return DT_ERROR_OK;//�ȷ���ok
}
int CAAVision::ChangeVideoWind(HWND hWnd,bool windtype,int windwid, int windheight)
{
	int ret = DT_ERROR_OK;
	EnterCriticalSection(&m_windcsLock);
	m_hWnd = hWnd;//���ű��洰��,��hacon���󶨴���

	m_videowindwid = windwid;
	m_videowindheight = windheight;
	if (true == m_firstopenvideo || windtype !=m_videowintype )
	{
		HTuple HalWinId;
		if (halconWinID>0)
		{
			CloseWindow(halconWinID);
			halconWinID = -1;
		}
		OpenWindow(0,0,windwid,windheight, (long)hWnd, "visible", "", &HalWinId);
		halconWinID = (long)HalWinId.L();
		SetPart(halconWinID,0,0,m_PreviewHeightA,m_PreviewWidthA);
	}
	m_videowintype = windtype;
	m_firstopenvideo = false;
	LeaveCriticalSection(&m_windcsLock);

	return ret;
}
int CAAVision::OpenVisionVideo(HWND hWnd,bool windtype,int windwid, int windheight)
{
	int ret = DT_ERROR_OK;
	if (m_iWorkStatusA == TESTOR_START) //�����Ѿ�����Ƶ��
	{
		return ret;
	}
	if (m_iDevIDA <0) //�ȴ��豸
	{
		return -1;
	}

	//EnterCriticalSection(&m_windcsLock);
	//m_hWnd = hWnd;//���ű��洰��,��hacon���󶨴���

	//m_videowindwid = windwid;
	//m_videowindheight = windheight;
	//if (true == m_firstopenvideo || windtype !=m_videowintype )
	//{
	//	HTuple HalWinId;
	//	if (halconWinID>0)
	//	{
	//		CloseWindow(halconWinID);
	//		halconWinID = -1;
	//	}
	//	OpenWindow(0,0,windwid,windheight, (long)hWnd, "visible", "", &HalWinId);
	//	halconWinID = (long)HalWinId.L();
	//	SetPart(halconWinID,0,0,g_AAParam.pic_width,g_AAParam.pic_lenth);
	//}
	//m_videowintype = windtype;
	//m_firstopenvideo = false;
	//LeaveCriticalSection(&m_windcsLock);

	ret = InitCamera();
	ret = CalculateGrabSize(&m_GrabSizeA, m_iDevIDA);
	ret = OpenVideo(m_GrabSizeA,m_iDevIDA);

	ret = InitDisplay(hWnd,CurrentSensorA.width,CurrentSensorA.height,CurrentSensorA.type, CHANNEL_A, NULL, m_iDevIDA);
	ret = InitIsp(CurrentSensorA.width, CurrentSensorA.height, CurrentSensorA.type, CHANNEL_A , m_iDevIDA);

	//SetTimer(m_iDevIDA+1,500,NULL);��ʱ��Ҫ��Ҫ������������,��Ҫ����ʾ��Ϣ
	{
		m_iWorkStatusA = TESTOR_START;
		UINT nSize = CurrentSensorA.width * CurrentSensorA.height * 3 + 1024;
		m_pTripleBufferA = (LPBYTE)malloc(nSize);
		m_pCameraBufferA = (LPBYTE)malloc(nSize);
		m_pDisplayBufferA = (LPBYTE)malloc(nSize);

		if (m_pTripleBufferA  == NULL  || 
			m_pCameraBufferA == NULL  || 
			m_pDisplayBufferA == NULL 
			)
		{
			PRINTF("Memory error!");
			//return FALSE;
		}
		memset(m_pTripleBufferA, 0, nSize);
		memset(m_pCameraBufferA, 0, nSize);
		memset(m_pDisplayBufferA, 0, nSize);
	}

	if (m_iWorkStatusA ==  TESTOR_START)//����Ƶ
	{
		m_hCameraThreadEventDeadA = CreateEventA(NULL, TRUE, FALSE, NULL);
		m_hDisplayThreadEventDeadA = CreateEventA(NULL, TRUE, FALSE, NULL);
		//_beginthread( Camera_ThreadA, 0, (void*)this);
		//_beginthread( Display_ThreadA, 0, (void*)this);

		_beginthreadex( NULL,0,(unsigned int (__stdcall *)(void *))Camera_ThreadA, (void*)this,0 ,NULL);
		_beginthreadex( NULL,0,(unsigned int (__stdcall *)(void *))Display_ThreadA, (void*)this,0,NULL);

	}

	return ret;

}
int CAAVision::CloseVisionVideo()
{
	int ret = DT_ERROR_OK;

	if(m_iWorkStatusA == TESTOR_STOP || (DT_ERROR_OK != IsDevConnect(m_iDevIDA)))
		return ret;
	m_iWorkStatusA = TESTOR_STOP;
	//EnterCriticalSection(&m_windcsLock);
	//if (halconWinID>0)
	//{
	//	CloseWindow(halconWinID);
	//	halconWinID = -1;
	//}
	//LeaveCriticalSection(&m_windcsLock);



	if(m_hDisplayThreadEventDeadA)
	{
		WaitForSingleObject(m_hDisplayThreadEventDeadA, INFINITE);
		CloseHandle(m_hDisplayThreadEventDeadA);
		m_hDisplayThreadEventDeadA = NULL;
	}
	if(m_hCameraThreadEventDeadA)
	{
		WaitForSingleObject(m_hCameraThreadEventDeadA, INFINITE);
		CloseHandle(m_hCameraThreadEventDeadA);
		m_hCameraThreadEventDeadA = NULL;
	}
	ret = UinitCamera();
	//KillTimer(1);

	return ret;
}
LPBYTE CAAVision::GrapOnePictureWhenVideo()
{
	int ret = DT_ERROR_OK;
	//wait the thread to end
	BYTE mode = m_iWorkStatusA;;

	if(m_iWorkStatusA == TESTOR_STOP)
	{
		PRINTF("Camera %s,is closed!Please Open it.",m_pSensorNameA);
		return NULL;
	}
	else //stop the thread and stop capture image and display...
	{
		//m_iWorkStatusA = TESTOR_STOP;
	}
	//wait the thread to end
	/*if(mode == TESTOR_START)
	{
	WaitForSingleObject(m_hCameraThreadEventDeadA, INFINITE);
	WaitForSingleObject(m_hDisplayThreadEventDeadA, INFINITE);
	if(m_hCameraThreadEventDeadA)
	{
	CloseHandle(m_hCameraThreadEventDeadA);
	m_hCameraThreadEventDeadA = NULL;
	}
	if(m_hDisplayThreadEventDeadA)
	{
	CloseHandle(m_hDisplayThreadEventDeadA);
	m_hDisplayThreadEventDeadA = NULL;
	}

	}*/

	USHORT width  = m_PreviewWidthA;
	USHORT height = m_PreviewHeightA;
	BYTE   type   =CurrentSensorA.type;
	LPBYTE BmpBuffer = NULL;
	BmpBuffer = (LPBYTE)malloc(width*height*4);
	if((BmpBuffer == NULL) )
	{
		PRINTF("Memory allocate error!");
		msg("�ڴ����ʧ��");
		m_iWorkStatusA=mode;	
		/*	if(mode==TESTOR_START)
		{
		//SetDlgItemText(IDC_PLAY,"&PAUSE");
		m_hCameraThreadEventDeadA = CreateEvent(NULL, TRUE, FALSE, NULL);
		_beginthread( Camera_ThreadA, 0, (void*)this);
		m_hDisplayThreadEventDeadA = CreateEvent(NULL, TRUE, FALSE, NULL);
		_beginthread( Display_ThreadA, 0, (void*)this); 

		}*/
		return NULL;
	}
	//allocate the bmp buffer.
	UINT nSize = width*height*3 + 1024*1024;
	LPBYTE CameraBuffer = NULL;
	CameraBuffer = (LPBYTE)malloc(nSize);
	if((CameraBuffer == NULL) )
	{
		PRINTF("Memory allocate error!");
		free(BmpBuffer);
		m_iWorkStatusA=mode;	
		/*	if(mode==TESTOR_START)
		{
		//SetDlgItemText(IDC_PLAY,"&PAUSE");
		m_hCameraThreadEventDeadA = CreateEvent(NULL, TRUE, FALSE, NULL);
		_beginthread( Camera_ThreadA, 0, (void*)this);
		m_hDisplayThreadEventDeadA = CreateEvent(NULL, TRUE, FALSE, NULL);
		_beginthread( Display_ThreadA, 0, (void*)this); 

		}*/
		return NULL;
	}
	memset(CameraBuffer, 0, nSize);
	ULONG RetSize = 0;
	UINT i;
	FrameInfo Info;
	int bRet = 0;
	for( i=0; i<GRAPPITURENUM; i++)
	{
		if(m_isTV)//����ģʽ������ ����Ȥ������Ŀ�ȱ�Сһ��:m_isTV ? CurrentSensor.height >> 1 :CurrentSensor.height
		{
			bRet = GrabFrame(CameraBuffer, m_GrabSizeA,&RetSize,&Info,m_iDevIDA);
			if(bRet != DT_ERROR_OK)
			{
				msg("ץͼʧ��!11");
			}
			//else if(bRet == DT_ERROR_OK)
			//{
			//	bRet = GrabFrame(CameraBuffer+m_GrabSizeA, m_GrabSizeA,&RetSize,&Info,m_iDevIDA);
			//	if(bRet == DT_ERROR_OK)
			//	{
			//		break;
			//	}
			//	else
			//	{
			//		msg("ץͼʧ��!22");
			//	}
			//}
		}
		else
		{
			if (GrabFrame(CameraBuffer,m_GrabSizeA, &RetSize,&Info, m_iDevIDA) == DT_ERROR_OK)
			{
				break;
			}
		}
	}
	if(i == GRAPPITURENUM)
	{
		PRINTF("failed to save!");
	}
	else
	{
		ImageProcess(CameraBuffer,BmpBuffer,width,height,&Info,m_iDevIDA);
	}

	m_iWorkStatusA=mode;	
	/*	if(mode==TESTOR_START)
	{
	//SetDlgItemText(IDC_PLAY,"&PAUSE");
	m_hCameraThreadEventDeadA = CreateEvent(NULL, TRUE, FALSE, NULL);
	_beginthread( Camera_ThreadA, 0, (void*)this);
	m_hDisplayThreadEventDeadA = CreateEvent(NULL, TRUE, FALSE, NULL);
	_beginthread( Display_ThreadA, 0, (void*)this); 

	}*/


	if (CameraBuffer)
	{
		free(CameraBuffer);
		CameraBuffer = NULL;
	}
	return BmpBuffer ; //�������ͷ�
}
LPBYTE CAAVision::GrapOnePicture()
{
	//ץͼ������,һ�����ڿ�����Ƶ�������,һ������û�п���Ƶ�����
	int ret = DT_ERROR_OK;
	//��һ�֣�
	if (m_iWorkStatusA == TESTOR_START )
	{
		//ret = ResetFrameBuffer(m_iDevIDA);
		return GrapOnePictureWhenVideo();
	}

	//�ڶ��֣�
	ret =  OpenVisionDevice();
	ret = ResetFrameBuffer(m_iDevIDA);
	if (InitCamera() != DT_ERROR_OK)
	{
		PRINTF("InitCamera is Failed  !");
		return NULL;
	}

	ret = OpenVideo(m_GrabSizeA,m_iDevIDA);
	ret = InitIsp(CurrentSensorA.width, CurrentSensorA.height, CurrentSensorA.type, CHANNEL_A , m_iDevIDA);

	UINT nSize = CurrentSensorA.width*CurrentSensorA.height*3 + 1024*1024;
	LPBYTE CameraBuffer = NULL;
	CameraBuffer = (LPBYTE)malloc(nSize);
	if((CameraBuffer == NULL) )
	{
		PRINTF("Memory allocate error!");
		return NULL;
	}
	memset(CameraBuffer, 0, nSize);

	ULONG RetSize = 0;
	FrameInfo m_FrameInfoA;
	ret = CalculateGrabSize(&m_GrabSizeA, m_iDevIDA);
	//ret  = SetSnap(TRUE,1,m_iDevIDA); //ʹ��ץ��
	//ret = TriggerSnap(m_iDevIDA);
	for (int i =0;i<GRAPPITURENUM;i++)
	{
		ret = GrabFrame(CameraBuffer, m_GrabSizeA,&RetSize,&m_FrameInfoA,m_iDevIDA);
		if (DT_ERROR_OK == ret)
		{
			ret = GrabFrame(CameraBuffer, m_GrabSizeA,&RetSize,&m_FrameInfoA,m_iDevIDA);
			if (DT_ERROR_OK == ret)
			{
				break;
			}
		}
	}
	ret = CloseVideo(m_iDevIDA);
	LPBYTE BmpBuffer = NULL;
	//MallocBuffer(&BmpBuffer, width * height * 4);
	BmpBuffer = (LPBYTE)malloc(CurrentSensorA.width*CurrentSensorA.height*4);

	ret = ImageProcess(CameraBuffer,BmpBuffer,CurrentSensorA.width,CurrentSensorA.height,&m_FrameInfoA,m_iDevIDA);

	if (CameraBuffer!= NULL)
	{
		free(CameraBuffer);
	}
	ret  = CloseVisionDevice();
	return BmpBuffer;
}
int CAAVision::SaveOnePicture()
{
	int ret = DT_ERROR_OK;

	return ret;
}

int CAAVision::GetVisionDeviceSN(BYTE *pSN,int iBufferSize,int *pRetLen)
{
	return GetDeviceSN(pSN,iBufferSize,pRetLen,m_iDevIDA);
}
BOOL CAAVision::SetPaintFlag(BOOL flag)
{
	m_paintflag =  flag;

	return m_paintflag;
}

void CAAVision::PaintColor(BYTE* pBmp,FrameInfo& frameInfo,int x,int y,int width ,int height ,int type,int color)
{
	//width��ʶֱ�ߵ�ˮƽ�����ϵĳ��ȣ�height��ʶ��ֱ�����ϵĳ��ȣ�(x,y)��ʶֱ�ߵ����ϵ�
	for (int i= x;i<x+width;i++)
	{
		for (int j= y ;j<y +height;j++)
		{
			switch(color)
			{		
			case 1://blue
				pBmp[3*(i + j*frameInfo.uWidth) +0] = 255;
				pBmp[3*(i + j*frameInfo.uWidth) +1] = 0;
				pBmp[3*(i + j*frameInfo.uWidth) +2] = 0;	
				break;
			case 2://green
				pBmp[3*(i + j*frameInfo.uWidth) +0] = 0;
				pBmp[3*(i + j*frameInfo.uWidth) +1] = 255;
				pBmp[3*(i + j*frameInfo.uWidth) +2] = 0;	
				break;
			case 3://red
				pBmp[3*(i + j*frameInfo.uWidth) +0] = 0;
				pBmp[3*(i + j*frameInfo.uWidth) +1] = 0;
				pBmp[3*(i + j*frameInfo.uWidth) +2] = 255;	
				break;
			default:
				pBmp[3*(i + j*frameInfo.uWidth) +0] = 0;
				pBmp[3*(i + j*frameInfo.uWidth) +1] = 0;
				pBmp[3*(i + j*frameInfo.uWidth) +2] = 0;	
				break;

			}

		}
	}

}

void CAAVision::PaintLine(BYTE* pBmp,FrameInfo& frameInfo,int x,int y,int Length ,int lineWidth ,int type,int color)
{
	if (type == 1)//��ֱ��
	{
		PaintColor(pBmp,frameInfo,x,y,lineWidth,Length,type,color);
	}
	else if(type == 0)//ˮƽ��
	{
		PaintColor(pBmp,frameInfo,x,y,Length,lineWidth,type,color);
	}	
	else
	{
		//б��
	}
}

void CAAVision::PaintRetangle(BYTE* pBmp,FrameInfo& frameInfo,int x,int y,int Length ,int width,int lineWidth ,int color)

{
	PaintLine(pBmp,frameInfo,x,y,Length,lineWidth,0,color);//top
	PaintLine(pBmp,frameInfo,x,y,width,lineWidth,1,color);//left
	PaintLine(pBmp,frameInfo,x+Length,y,width,lineWidth,1,color);//right
	PaintLine(pBmp,frameInfo,x,y+width,Length,lineWidth,0,color);//bottom


}
void CAAVision::PaintCrossCenter(BYTE* pBmp,FrameInfo &frameInfo,int lineLength ,int lineWidth )
{

	if (m_iDevIDA< 0)
	{
		msg("m_iDevIDA С��0");
		return;
	}
	int centerx = g_AAParam.paintCenter[m_iDevIDA].X;//xΪ���
	int centery = g_AAParam.paintCenter[m_iDevIDA].Y;

	for (int i= centerx -lineWidth;i<centerx+lineWidth;i++)//��ʮ�ֲ������
	{
		for (int j= centery -lineLength;j<centery +lineLength;j++)
		{
			pBmp[3*(i + j*frameInfo.uWidth) +0] = 0;
			pBmp[3*(i + j*frameInfo.uWidth) +1] = 0;
			pBmp[3*(i + j*frameInfo.uWidth) +2] = 255;						
		}
	}
	for (int i= centerx-lineLength;i<centerx+lineLength;i++)	
	{
		for (int j= centery -lineWidth;j<centery+lineWidth;j++)//��ʮ�ֲ�ĺ���,ע�����lineWidth = 10  lineLength = 50
		{
			pBmp[3*(i + j*frameInfo.uWidth) +0] = 0;
			pBmp[3*(i + j*frameInfo.uWidth) +1] = 0;
			pBmp[3*(i + j*frameInfo.uWidth) +2] = 255;
		}
	}


}
void CAAVision::PaintSpecialLine(BYTE* pBmp,FrameInfo& frameInfo,int x,int y,int lineLength,int lineWidth ,int color)//��ˮƽ������ֱ��
{
	for (int i= x;i<x+lineWidth;i++)
	{
		for (int j= y ;j<y +lineLength;j++)
		{
			switch(color)
			{		
			case 1://blue
				pBmp[3*(i + j*frameInfo.uWidth) +0] = 255;
				pBmp[3*(i + j*frameInfo.uWidth) +1] = 0;
				pBmp[3*(i + j*frameInfo.uWidth) +2] = 0;	
				break;
			case 2://green
				pBmp[3*(i + j*frameInfo.uWidth) +0] = 0;
				pBmp[3*(i + j*frameInfo.uWidth) +1] = 255;
				pBmp[3*(i + j*frameInfo.uWidth) +2] = 0;	
				break;
			case 3://red
				pBmp[3*(i + j*frameInfo.uWidth) +0] = 0;
				pBmp[3*(i + j*frameInfo.uWidth) +1] = 0;
				pBmp[3*(i + j*frameInfo.uWidth) +2] = 255;	
				break;
			default:
				pBmp[3*(i + j*frameInfo.uWidth) +0] = 0;
				pBmp[3*(i + j*frameInfo.uWidth) +1] = 0;
				pBmp[3*(i + j*frameInfo.uWidth) +2] = 0;	
				break;

			}

		}
	}
}
void CAAVision::PaintSpecialRetangle(BYTE* pBmp,FrameInfo& frameInfo,int lineLength ,int lineWidth )
{
	int size = CFG_INSTANCE->get_cfg_int(AA_Globle_param,AA_size);
	int block = CFG_INSTANCE->get_cfg_int(AA_Globle_param,AA_block);
	double lenLarge = CFG_INSTANCE->get_cfg_double(AA_Globle_param,AA_t_mapdislength);
	double lenSmall = CFG_INSTANCE->get_cfg_double(AA_Globle_param,AA_t_mapdiswidth);
	double pixel_size = CFG_INSTANCE->get_cfg_double(AA_Globle_param,AA_pixel_size);
	double scale = CFG_INSTANCE->get_cfg_double(AA_Globle_param,AA_scale);

	//��ԭʼͼ����
	//pixel_size = pixel_size/scale;



	//���ĵ�����
	lineLength = size/scale; //��ֱ�����ϵĳ���
	lineWidth = 10;//ˮƽ�������ϵĳ���


	PaintSpecialLine(pBmp,frameInfo,frameInfo.uWidth/2-lineWidth/2,frameInfo.uHeight/2,lineLength,lineWidth,1);//�ұ���
	PaintSpecialLine(pBmp,frameInfo,frameInfo.uWidth/2-lineLength-lineWidth/2,frameInfo.uHeight/2,lineLength,lineWidth,1);//left
	PaintSpecialLine(pBmp,frameInfo,frameInfo.uWidth/2-lineLength,frameInfo.uHeight/2-lineWidth/2,lineWidth,lineLength,1);//top
	PaintSpecialLine(pBmp,frameInfo,frameInfo.uWidth/2-lineLength,frameInfo.uHeight/2+lineLength-lineWidth/2,lineWidth,lineLength,1);

}

void CAAVision::DecisionRectByTemplateType(int temPtype,int lengthS,int lengthB,int picwidth,int picheight,int &x,int &y,int &bigx,int &bigy)
{
	//temPtype  = 3 ;//����������Ϊ���㻭ͼ

	if (m_iDevIDA <0)
	{
		msg("m_iDevIDAС��0!");
		return;
	}
	picwidth = g_AAParam.paintCenter[m_iDevIDA].X *2; //�Ȱ���������Ϊ��ͼ������
	picheight = g_AAParam.paintCenter[m_iDevIDA].Y *2;

	switch(temPtype)
	{
	case 0:
		//��С�������ϵ�
		x = picwidth/2;
		y = picheight/2 ;
		//�Ҵ���ε����ϵ�
		bigx = picwidth/2 - lengthB + lengthS ;
		bigy = picheight/2 -lengthB + lengthS;
		break;
	case 2:
		//��С�������ϵ�
		x = picwidth/2 ;
		y = picheight/2 -lengthS;
		//�Ҵ���ε����ϵ�
		bigx = picwidth/2 - lengthB+ lengthS;
		bigy = picheight/2 -  lengthS;
		break;
	case 3:
	default:
		//��С�������ϵ�
		x = picwidth/2-lengthS;
		y = picheight/2 -lengthS;
		//�Ҵ���ε����ϵ�
		bigx = picwidth/2-lengthS;
		bigy = picheight/2 -lengthS;
		break;
	case 1:
		//��С�������ϵ�
		x = picwidth/2-lengthS;
		y = picheight/2 ;
		//�Ҵ���ε����ϵ�
		bigx = picwidth/2 -  lengthS;
		bigy = picheight/2 - lengthB+ lengthS;
		break;
	}

}
int CAAVision::SetSensorTab(SensorTab& pCurrentSensor)
{
	memcpy(&CurrentSensorA,&pCurrentSensor,sizeof(SensorTab));
	strcpy_s(CurrentSensorA.name,pCurrentSensor.name);
	CurrentSensorA.ParaList = new USHORT[pCurrentSensor.ParaListSize];
	memcpy(CurrentSensorA.ParaList,pCurrentSensor.ParaList,sizeof(USHORT)*pCurrentSensor.ParaListSize);

	//�����Ĳ�������û���õ�,��������


	return 0;

}
int CAAVision::SetPinDefinition()
{
	int ret = DT_ERROR_OK;
	int iDevID = m_iDevIDA;
	BYTE  pinDef[40] = {0};
	if(CurrentSensorA.port == PORT_MIPI || CurrentSensorA.port == PORT_HISPI)
	{
		pinDef[0] = 20;
		pinDef[1] = 0;
		pinDef[2] = 2;
		pinDef[3] = 1;
		pinDef[4] = 3;
		pinDef[5] = 4;
		pinDef[6] = 5;
		pinDef[7] = 6;
		pinDef[8] = 7;
		pinDef[9] = 8;
		pinDef[10] = 9;
		pinDef[11] = 20;
		pinDef[12] = 10;
		pinDef[13] = 11;
		pinDef[14] = 12;
		pinDef[15] = 20;
		pinDef[16] = 20;
		pinDef[17] = 13;
		pinDef[18] = 15;
		pinDef[19] = 14;
		pinDef[20] = 19;
		pinDef[21] = 18;
		pinDef[22] = 20;
		pinDef[23] = 16;
		pinDef[24] = 20;
		pinDef[25] = 20;
	}
	else  //standard parallel..
	{

		pinDef[0] = 16;
		pinDef[1] = 0;
		pinDef[2] = 2;
		pinDef[3] = 1;
		pinDef[4] = 3;
		pinDef[5] = 4;
		pinDef[6] = 5;
		pinDef[7] = 6;
		pinDef[8] = 7;
		pinDef[9] = 8;
		pinDef[10] = 9 ;
		pinDef[11] = 20;
		pinDef[12] = 10;
		pinDef[13] = 11;
		pinDef[14] = 12;
		pinDef[15] = 20;
		pinDef[16] = 20;
		pinDef[17] = 20;
		pinDef[18] = 20;
		pinDef[19] = 20;
		pinDef[20] = 13;
		pinDef[21] = 20;
		pinDef[22] = 14;
		pinDef[23] = 15;
		pinDef[24] = 18;
		pinDef[25] = 19;
	}
	//�������Խӿ�
	ret = SetSoftPin(pinDef,iDevID);

	return ret;
}


/* ��ʱ����
//�ڲ�˽�к���
BOOL CAAVision::LoadLibIniFile(pSensorTab pCurrentSensor,BYTE byChannel)
{

string sfilename ;  

if (byChannel ==0 )
{
//SetIniFileName(m_sFwFileFolderA);

sfilename = m_sFwFileFolderA;
}







pCurrentSensor->width    = ReadIniData("Sensor","width",0);
pCurrentSensor->height   = ReadIniData("Sensor","height",0);
pCurrentSensor->type     = ReadIniData("Sensor","type",2);

pCurrentSensor->port = ReadIniData("Sensor", "port", 0);
pCurrentSensor->pin = ReadIniData("Sensor", "pin", 0);

pCurrentSensor->SlaveID  = ReadIniData("Sensor", "SlaveID", 0);
pCurrentSensor->mode     = ReadIniData("Sensor", "mode", 0);
pCurrentSensor->FlagReg  = ReadIniData("Sensor", "FlagReg", 0);
pCurrentSensor->FlagMask = ReadIniData("Sensor", "FlagMask", 0xff);
pCurrentSensor->FlagData = ReadIniData("Sensor", "FlagData", 0);

pCurrentSensor->FlagReg1  = ReadIniData("Sensor", "FlagReg1", 0);
pCurrentSensor->FlagMask1 = ReadIniData("Sensor", "FlagMask1", 0x0);
pCurrentSensor->FlagData1 = ReadIniData("Sensor", "FlagData1", 0);

pCurrentSensor->outformat= ReadIniData("Sensor", "outformat", 0x00);
pCurrentSensor->mclk     = ReadIniData("Sensor", "mclk", 0x01);

pCurrentSensor->avdd     = ReadIniData("Sensor", "avdd", 0x00);
pCurrentSensor->dovdd     = ReadIniData("Sensor", "dovdd", 0x00);
pCurrentSensor->dvdd     = ReadIniData("Sensor", "dvdd", 0x00);

ClearIniFileName();

pCurrentSensor->ParaList = NULL;
pCurrentSensor->ParaListSize = 0;
pCurrentSensor->SleepParaList = NULL;
pCurrentSensor->SleepParaListSize = NULL;

GetI2CDataFromLibFile(sfilename, pCurrentSensor,byChannel);
if( (pCurrentSensor->width==0)       ||
(pCurrentSensor->height==0)      ||
(pCurrentSensor->ParaList==NULL) ||
(pCurrentSensor->ParaListSize==0)	  )
{
return FALSE;
}


return TRUE;
}
*/