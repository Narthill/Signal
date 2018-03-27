// AARobotLib.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#pragma once

#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�:
#include <windows.h>

#include "AARobotLib.h"

#include "mtGCS.h"

#include <iostream>
using namespace std;




// ������ID
static int gControllerID = -1;
// ���������ں�
static int gComPort = -1;
// API������
static int gErrorCode = -1;
// API���������Ϣ
static char gErrorMsg[256] = {0};


#define PI_AXIS_MAXNUM 6
#define EnumDevCharLen 10000
#define OnePIcharMaxLen         64
#define  MaxPINum               8

#define PRINTF(msg, ...) printf(msg, __VA_ARGS__)


static bool referenceIfNeeded(int ID, char* axis)
{
	BOOL bReferenced[6] = {false};
	BOOL bFlag;
	if(false  == PI_qFRF(ID, axis, bReferenced))
	{
		cout << "��ѯ����״̬ʧ��" << endl;
		gErrorCode = PI_GetError(ID);
		PI_TranslateError(gErrorCode, gErrorMsg, sizeof(gErrorMsg));
		cout << "PI_qFRF error" << gErrorMsg << endl;
		return false;
	}

	bFlag = true;
	for (int i = 0; i < 6; i++)
	{
		if (false  == bReferenced[i])
		{
			cout << "��" << i << "���ỹû������" << endl;
			bFlag = false;
		}
	}

	if (false == bFlag)
	{// if needed,
		// reference the axis using the refence switch
		printf("Referencing axis %s...\n",axis);
		if(!PI_FRF(ID, axis))
		{
			gErrorCode = PI_GetError(gControllerID);
			PI_TranslateError(gErrorCode, gErrorMsg, sizeof(gErrorMsg));
			cout << "PI_FRF error" << gErrorMsg << endl;
			return false;
		}
		// Wait until the reference move is done.
		bFlag = false;
		while(bFlag != TRUE)
		{
			if(!PI_IsControllerReady(ID, &bFlag))
			{
				gErrorCode = PI_GetError(gControllerID);
				PI_TranslateError(gErrorCode, gErrorMsg, sizeof(gErrorMsg));
				cout << "������û�о�����" << gErrorMsg << endl;
				return false;
			}
		}
	}

	cout << "���óɹ�" << endl;
	return true;
}

static int CountAxisInstrAxis(const char *axisString)
{

	int axisnum =0 ;
	if ((axisString[0] == 'X' || axisString[0] == 'Y'|| axisString[0] == 'Z'|| axisString[0] == 'U'|| axisString[0] == 'V'|| axisString[0] == 'W') && 1 == strlen(axisString))
	{
		axisnum = 1;
	}
	else
	{
		int pos = 0;
		string axisstr = axisString;
		while (pos!= string::npos)
		{
			pos = axisstr.find(" ",pos);
			if (pos!= string::npos)
			{
				axisnum++;
				pos+=1;
			}
		}   
		axisnum += 1;
	}

	return axisnum;

}

long ConnectFirstFoundHexapodViaTCPIP()
{
	char szFoundDevices[10000];
	printf("searching TCPIP devices...\n");
	if(PI_EnumerateTCPIPDevices(szFoundDevices,9999,"")==0)
	{
		return -1;
	}
	char* szAddressToConnect = NULL;
	int port = 0;
	char * pch = strtok (szFoundDevices,"\n");
	while (pch != NULL)
	{
		_strupr(pch);
		
		if(
			  ((strstr(pch,"F-HEX") != NULL) && (strstr(pch,"LISTENING") != NULL))
			||((strstr(pch,"HEXAPOD") != NULL) && (strstr(pch,"LISTENING") != NULL))
			||((strstr(pch,"F-206") != NULL) && (strstr(pch,"LISTENING") != NULL))
			||((strstr(pch,"M-8") != NULL) && (strstr(pch,"LISTENING") != NULL))
			||((strstr(pch,"C-887") != NULL) && (strstr(pch,"LISTENING") != NULL))
		  )
		{
			char* colon = strstr(pch,":");
			*colon = '\0';
			szAddressToConnect = new char[strlen(strstr(pch,"(")+1)+1];
			strcpy (szAddressToConnect,strstr(pch,"(")+1);

			*strstr(colon+1,")") = '\0';
			port = atoi(colon+1);
			break;
		}
		pch = strtok (NULL, "\n");
	}

	if(szAddressToConnect != NULL)
	{
		printf("trying to connect to %s, port %d\n",szAddressToConnect,port);
		int iD = PI_ConnectTCPIP(szAddressToConnect ,port);
		delete []szAddressToConnect;
		return iD;
	}
	return -1;

}

AAROBOTLIB_API int AARobotConnectTCPIPbyDescription (const char* szDescription) 
{
	 int contorolID = -1;
	 char axis[] = "X Y Z U V Z";

	 contorolID = PI_ConnectTCPIPByDescription(szDescription);

	 if(referenceIfNeeded(contorolID, axis) == false)
	 {
		 gErrorCode = PI_GetError(contorolID);
		 PI_TranslateError(gErrorCode, gErrorMsg, sizeof(gErrorMsg));
		 cout << "�޷���ȡ�����" << gErrorMsg << endl;
		 return gErrorCode;
	 }

	 return contorolID;
}

AAROBOTLIB_API int AARobotEnumerateTCPIPDevices (char* szBuffer, int iBufferSize, const char* szFilter) 
{
	return PI_EnumerateTCPIPDevices (szBuffer, iBufferSize, szFilter) ;
}

AAROBOTLIB_API int AARobotInit(int comPort)
{
	int result = 0;
	char axis[] = "X Y Z U V Z";

	gControllerID = PI_ConnectRS232(comPort,115200);
	if (gControllerID<0)
	{
		gErrorCode = PI_GetError(gControllerID);
		PI_TranslateError(gErrorCode, gErrorMsg, sizeof(gErrorMsg));
		cout << "�޷����ӿ�������" << gErrorMsg << endl;
		return -2;
	}

	cout << "�����������ɹ�" << endl;

	if(referenceIfNeeded(gControllerID, axis) == false)
	{
		gErrorCode = PI_GetError(gControllerID);
		PI_TranslateError(gErrorCode, gErrorMsg, sizeof(gErrorMsg));
		cout << "�޷���ȡ�����" << gErrorMsg << endl;
		return -3;
	}

	cout << "��ȡ�����ɹ�" << endl << "��ʼ���ɹ�" << endl;

	return 0;
}

/*.......................�����������ƫ�ƣ���Χ�������������ת..................................
 *   @Input:		axisString			��Ҫ�˶���������ɵ��ַ���
 *												����Ϊ  X��Y��Z��U��V��W����
 *												�����м䣬��Ҫ�ÿո�ָ�
 *												���磬��Ҫ�ƶ�X��Y��Zʱ��axisString Ϊ ��   "X  Y  Z"
 *												��Ҫѡ��U��Vʱ��axisStringΪ��      "U  V"
 *						posArray			��Ҫ�ߵ�ƫ���������ף���ƫ�ƽǶȣ��ȣ�
 *												��һ�����飬���м������ݾ���Ҫ��������Ԫ��
 *												���磬��ҪX��Y��Z���ƶ�0.1mm����posArray���ݳ���Ϊ3��3��Ԫ�ض�Ϊ0.1
 *   @Return�� >= 0 �ɹ��� < 0 ʧ��
 */

AAROBOTLIB_API int AARobotMoveRelative(int controlID,const char *axisString, const double *posArray)
{
	int result = 0;
	BOOL tmpArray[6] = {false};
	BOOL isMovingCallResult = 0;
	int isMovingFlag = 0;
	int axisnum =0;

	if (axisString == NULL || *axisString == '\0')
	{
		return -1;
	}
	//strupr(const_cast<char*>(axisString));
	if ((axisString[0] == 'X' || axisString[0] == 'Y'|| axisString[0] == 'Z'|| axisString[0] == 'U'|| axisString[0] == 'V'|| axisString[0] == 'W') && 1 == strlen(axisString))
	{
		axisnum = 1;
	}
	else
	{
		int pos = 0;
		string axisstr = axisString;
		while (pos!= string::npos)
		{
			pos = axisstr.find(" ",pos);
			if (pos!= string::npos)
			{
				axisnum++;
				pos+=1;
			}
		}   
		axisnum += 1;
	}

	if(false == PI_MRT(controlID, axisString, posArray))
	{
		gErrorCode = PI_GetError(controlID);
		PI_TranslateError(gErrorCode, gErrorMsg, sizeof(gErrorMsg));
		cout << "���й�������ϵ�ڵ�����˶�ʧ�ܣ�" << gErrorMsg << endl;
		return -1;
	}
	else // ��Ҫ���˶�����
	{
		while (1)
		{
			isMovingCallResult = PI_IsMoving(controlID, NULL, tmpArray);
			if (false == isMovingCallResult) // API���ó����ˣ�Ҫͣ����
			{
				gErrorCode = PI_GetError(controlID);
				PI_TranslateError(gErrorCode, gErrorMsg, sizeof(gErrorMsg));
				cout << "����˶�״̬ʧ�ܣ�" << gErrorMsg << endl;
				return -1;
			}
			isMovingFlag = 0;
			for(int i =0 ;i<PI_AXIS_MAXNUM;i++)
			{
				isMovingFlag |= tmpArray[i]; 
			}
			if(false == isMovingFlag) // �˶�ֹͣ��
			{
				cout << "�˶�����" << endl;
				return 0;
			}
			Sleep(10);
		}
	}

	cout << "��������˶��ɹ�" << endl;
	return 0;
}

AAROBOTLIB_API int AARobotMoveRelative2(const char *axisString, const double *posArray)
{
	int result = 0;
	BOOL tmpArray[6];
	BOOL isMovingCallResult = 0;
	BOOL isMovingFlag = 0;

	double curPos[6] = {0};

	if (gControllerID < 0)
	{
		cout << "׼����������˶�ʱ������δ��������������Ҫ����" << endl;
		result = AARobotInit(gComPort);
		if (result < 0)
		{
			cout << "׼����������˶�ʱ������������ʧ��" << result << endl;
			return result;
		}
	}

	if (false == PI_qPOS(gControllerID, NULL, curPos))
	{
		gErrorCode = PI_GetError(gControllerID);
		PI_TranslateError(gErrorCode, gErrorMsg, sizeof(gErrorMsg));
		cout << "��ѯ��ǰλ��ʧ�ܣ�" << gErrorMsg << endl;
		return -3;
	}
	//cout << "�˶�ǰ��[ " << curPos[0] << ", " << curPos[1] << ", " << curPos[2]
	//<< ", " << curPos[3] << ", " << curPos[4] << "," << curPos[5] << " ]" << endl;
	printf("�˶�ǰ��[%.4lf, %.4lf, %.4lf, %.4lf, %.4lf, %.4lf]\n", curPos[0], curPos[1], curPos[2], curPos[3], curPos[4], curPos[5]);

	if(false == PI_MRT(gControllerID, axisString, posArray))
	{
		gErrorCode = PI_GetError(gControllerID);
		PI_TranslateError(gErrorCode, gErrorMsg, sizeof(gErrorMsg));
		cout << "���й�������ϵ�ڵ�����˶�ʧ�ܣ�" << gErrorMsg << endl;
		return -1;
	}
	else // ��Ҫ���˶�����
	{
		while (1)
		{
			isMovingCallResult = PI_IsMoving(gControllerID, NULL, tmpArray);
			if (false == isMovingCallResult) // API���ó����ˣ�Ҫͣ����
			{
				gErrorCode = PI_GetError(gControllerID);
				PI_TranslateError(gErrorCode, gErrorMsg, sizeof(gErrorMsg));
				cout << "����˶�״̬ʧ�ܣ�" << gErrorMsg << endl;
				return -2;
			}

			if (false == PI_qPOS(gControllerID, NULL, curPos))
			{
				gErrorCode = PI_GetError(gControllerID);
				PI_TranslateError(gErrorCode, gErrorMsg, sizeof(gErrorMsg));
				cout << "��ѯ��ǰλ��ʧ�ܣ�" << gErrorMsg << endl;
				return -3;
			}
			//cout << "�˶��У�[ " << curPos[0] << ", " << curPos[1] << ", " << curPos[2]
			//		<< ", " << curPos[3] << ", " << curPos[4] << "," << curPos[5] << " ]" << endl;
			printf("�˶��У�[%.4lf, %.4lf, %.4lf, %.4lf, %.4lf, %.4lf]\n", curPos[0], curPos[1], curPos[2], curPos[3], curPos[4], curPos[5]);
			
			isMovingFlag = tmpArray[0];
			if(false == isMovingFlag) // �˶�ֹͣ��
			{
				cout << "�˶�����" << endl;

				if (false == PI_qPOS(gControllerID, NULL, curPos))
				{
					gErrorCode = PI_GetError(gControllerID);
					PI_TranslateError(gErrorCode, gErrorMsg, sizeof(gErrorMsg));
					cout << "��ѯ��ǰλ��ʧ�ܣ�" << gErrorMsg << endl;
					return -3;
				}
				//cout << "�˶���[ " << curPos[0] << ", " << curPos[1] << ", " << curPos[2]
				//<< ", " << curPos[3] << ", " << curPos[4] << "," << curPos[5] << " ]" << endl;
				printf("�˶���[%.4lf, %.4lf, %.4lf, %.4lf, %.4lf, %.4lf]\n", curPos[0], curPos[1], curPos[2], curPos[3], curPos[4], curPos[5]);

				return 0;
			}
			Sleep(10);
		}
	}

	cout << "��������˶��ɹ�" << endl;
	return 0;
}

/*............................��ȡ�����˶�Ӧ��ĵ�ǰλ��..........................................
 *   @Input:		axisString			��Ҫ�˶���������ɵ��ַ���
 *												����Ϊ  X��Y��Z��U��V��W����
 *												�����м䣬��Ҫ�ÿո�ָ�
 *												���磬��Ҫ��ȡX��Y��Z�ᵱǰλ��ʱ��axisString Ϊ ��   "X  Y  Z"
 *												��Ҫѡ��U��Vʱ��axisStringΪ��      "U  V"
 *   @Output:		posArray			��ȡ��ĵ�ǰ��ƫ���������ף���ǰƫ�ƽǶȣ��ȣ�
 *												��һ�����飬���м������ݾ���Ҫ��������Ԫ��									
 *   @Return�� >= 0 �ɹ��� < 0 ʧ��
 */
AAROBOTLIB_API int AARobotCurrentPosition2(const char *axisString, double *posArray)
{
	int result = 0;
	double curPos[6] = {0};

	if (gControllerID < 0)
	{
		cout << "����δ��������������Ҫ����" << endl;
		result = AARobotInit(gComPort);
		if (result < 0)
		{
			cout << "����������ʧ��" << result << endl;
			return result;
		}
	}

	if (false == PI_qPOS(gControllerID, axisString, posArray))
	{
		gErrorCode = PI_GetError(gControllerID);
		PI_TranslateError(gErrorCode, gErrorMsg, sizeof(gErrorMsg));
		cout << "��ѯ��ǰλ��ʧ�ܣ�" << gErrorMsg << endl;
		return -1;
	}

	return 0;
}

AAROBOTLIB_API int AARobotCurrentPosition(int controlID,const char *axisString, double *posArray)
{
	int result = 0;
	if (controlID < 0)
	{
		cout << "����δ��������������Ҫ����" << endl;
	}

	if (false == PI_qPOS(controlID, axisString, posArray))
	{
		gErrorCode = PI_GetError(controlID);
		PI_TranslateError(gErrorCode, gErrorMsg, sizeof(gErrorMsg));
		cout << "��ѯ��ǰλ��ʧ�ܣ�" << gErrorMsg << endl;
		return -1;
	}

	return 0;

}

/*.......................������TCPIP���ӳ�ʼ��..................................
 *   @Input:   NULL
 *   @Return�� >= 0 �ɹ��� < 0 ʧ��
 */
AAROBOTLIB_API int AARobotTCPIPInit()
{
	int result = 0;
	char axis[] = "X Y Z U V Z";

	gControllerID =ConnectFirstFoundHexapodViaTCPIP();
	if (gControllerID<0)
	{
		gErrorCode = PI_GetError(gControllerID);
		PI_TranslateError(gErrorCode, gErrorMsg, sizeof(gErrorMsg));
		cout << "�޷����ӿ�������" << gErrorMsg << endl;
		return -2;
	}

	cout << "�����������ɹ�" << endl;

	if(referenceIfNeeded(gControllerID, axis) == false)
	{
		gErrorCode = PI_GetError(gControllerID);
		PI_TranslateError(gErrorCode, gErrorMsg, sizeof(gErrorMsg));
		cout << "�޷���ȡ�����" << gErrorMsg << endl;
		return -3;
	}

	cout << "��ȡ�����ɹ�" << endl << "��ʼ���ɹ�" << endl;

	return 0;
}

AAROBOTLIB_API int AARobotTCPIPInitOne(const char* ip, int port)
{
	int result = 0;
	char axis[] = "X Y Z U V Z";
	int ControllerID = 0;
	/*
	//����Ƿ��Ѿ�������
	if(AARobotIsConnectedByIpAndPort(ip,port) == true)
	{
		PRINTF("�Ѿ���������,%d",AA_PI_HAVE_CONNECT);
		return AA_PI_HAVE_CONNECT;
	}
	*/
	ControllerID = PI_ConnectTCPIP(ip ,port);
	if (ControllerID<0)
	{
		gErrorCode = PI_GetError(ControllerID);
		PI_TranslateError(gErrorCode, gErrorMsg, sizeof(gErrorMsg));
		cout << "�޷����ӿ�������" << gErrorMsg << endl;
		return -1;
	}

	cout << "�����������ɹ�" << endl;

	if(referenceIfNeeded(ControllerID, axis) == false)
	{
		gErrorCode = PI_GetError(ControllerID);
		PI_TranslateError(gErrorCode, gErrorMsg, sizeof(gErrorMsg));
		cout << "�޷���ȡ�����" << gErrorMsg << endl;
		return -1;
	}

	cout << "��ȡ�����ɹ�" << endl << "��ʼ���ɹ�" << endl;



	return 	ControllerID;

}

AAROBOTLIB_API bool AARobotIsConnected (int ID)
{
	return PI_IsConnected ( ID);
}

AAROBOTLIB_API  bool AARobotIsConnectedByIpAndPort  (const char* ip, int port) 
{
	char szFoundDevices[EnumDevCharLen];
	int devNum = 0;
	int in = 0;
	char *p[MaxPINum] = {NULL};
	char *buf = NULL; 
	char cport[10] = {0};
	if (NULL == ip || port<0){ return false;};

	PRINTF("searching TCPIP devices...\n");
	if((devNum = PI_EnumerateTCPIPDevices(szFoundDevices,EnumDevCharLen-1,""))<=0)
	{
		return false;
	}
	buf = szFoundDevices;
	while((p[in] = strtok(buf, "\n")) != NULL)   
	{      
		in++;
		buf = NULL;  
	} 
	if (in != devNum)
	{
		return false;
	}
	sprintf(cport,"%d",port);
	for (int i = 0;i<devNum;i++)
	{
		_strupr(p[i]);
		if((strstr(p[i],cport) != NULL) && (strstr(p[i],ip) != NULL)&& (strstr(p[i],"CONNECTED") != NULL))		
		{
			return true;
		}
	}
}

AAROBOTLIB_API void AARobotCloseConnection (int ID)
{
	return PI_CloseConnection( ID);
}

AAROBOTLIB_API int AARobotAbsMove(int controlID,const char *axisString,const double*pos)
{
	//��λ��ʱ�Ȳ�����
	BOOL ret = TRUE;
	int result = 0;
	BOOL tmpArray[6] = {false};
	BOOL isMovingCallResult = 0;
	int isMovingFlag = 0;

	int axisnum = CountAxisInstrAxis(axisString);
	ret = PI_MOV(controlID,axisString,pos);
	if (ret != TRUE )
	{
		ret = PI_GetError(controlID);
		PI_TranslateError(ret, gErrorMsg, sizeof(gErrorMsg));
		return FALSE;
	}
	else // ��Ҫ���˶�����
	{
		while (1)
		{
			isMovingCallResult = PI_IsMoving(controlID, NULL, tmpArray);
			if (false == isMovingCallResult) // API���ó����ˣ�Ҫͣ����
			{
				gErrorCode = PI_GetError(controlID);
				PI_TranslateError(gErrorCode, gErrorMsg, sizeof(gErrorMsg));
				cout << "����˶�״̬ʧ�ܣ�" << gErrorMsg << endl;
				return FALSE;
			}
			isMovingFlag = 0;
			for(int i =0 ;i<PI_AXIS_MAXNUM;i++)
			{
				isMovingFlag |= tmpArray[i]; 
			}
			if(false == isMovingFlag) // �˶�ֹͣ��
			{
				cout << "�˶�����" << endl;
				return 1;
			}
			Sleep(10);
		}
	}
	return ret;
}
 AAROBOTLIB_API int AARobotDfhHome(int controlID,const char *axisString)
{
	BOOL ret = TRUE;

	double homeposition[PI_AXIS_MAXNUM] = {0.0};
	//�Ȳ�ѯ�µ�ǰԭ��λ��
	PI_qDFH(controlID,axisString,homeposition);


	ret = PI_DFH(controlID,axisString);
	if (ret != TRUE )
	{
		ret = PI_GetError(controlID);
		PI_TranslateError(ret, gErrorMsg, sizeof(gErrorMsg));
		return FALSE;
	}

	return ret;

}
AAROBOTLIB_API int AARobotGoHome(int controlID,const char *axisString)
{
	//�ڻ�ԭʱ������
	BOOL ret = TRUE;
	ret = PI_GOH(controlID,axisString);
	if (ret != TRUE )
	{
		ret = PI_GetError(controlID);
		PI_TranslateError(ret, gErrorMsg, sizeof(gErrorMsg));
		return FALSE;
	}

	return ret;
}
// ���������ٶ�
AAROBOTLIB_API int setSystemVelocity(int ID,  double speedArr)
{
	if((PI_VLS(ID,speedArr))!=TRUE)
	{
		return FALSE;
	}
	return TRUE;
}

//��ȡ�ٶ�
 AAROBOTLIB_API int getSystemVelocity(int ID, double* speedArr)
{
	
	if((PI_qVLS(ID, speedArr))!=TRUE)
	{
		return FALSE;
	}
	return TRUE;
}

AAROBOTLIB_API int getPivotPointCoordinates(int ID, const char* axisArr, double* posArr)//��SPI?����
{
	if (TRUE != PI_qSPI(ID, axisArr, posArr)) {
		printf("get position failed\n");
		return FALSE;
	}

	return TRUE;
}


 AAROBOTLIB_API int getKLTCoordinates(int ID, const char* szStartCoordSystem, const char* szEndCoordSystem, char* buffer, int bufsize)
 {

	 return  PI_qKLT(ID, szStartCoordSystem, szEndCoordSystem, buffer, bufsize);

 }

 AAROBOTLIB_API int enableCoordinate(int ID, const char* szNameOfCoordSystem)
 {
	 return PI_KEN(ID,szNameOfCoordSystem);//ʹ��һ������ϵ
 }

 AAROBOTLIB_API int defineKSTCoordinate(int ID, const char* szNameOfCoordSystem, const char* axisArr, double* ToolCoordinateValue)
 {
	  if(TRUE != PI_KST(ID,szNameOfCoordSystem,axisArr,ToolCoordinateValue)) 
	  {
		  return FALSE;
	  }
	  return TRUE;
 }
 AAROBOTLIB_API int deleteOneCoordinate(int ID, const char* szNameOfCoordSystem)
 {
	  int result = PI_KRM( ID, szNameOfCoordSystem); //ɾ��һ������ϵ
      return result;
 }
 
 AAROBOTLIB_API int activeOneCoordinate(int ID, const char* szNameOfCoordSystem)
 {
	 int result = PI_KEN( ID, szNameOfCoordSystem); //����һ������ϵ
	 return result;
 }

 AAROBOTLIB_API int queryActiveCoordinates(int ID, const char* szNamesOfCoordSystems, char* buffer, int bufsize)
 {
	 int result = PI_qKET( ID, szNamesOfCoordSystems,buffer,bufsize); //��ѯ���м��������ϵ������������
	 return result;
 }

 AAROBOTLIB_API int setPosSoftLimit(int ID, const char* szAxes, const double* pdValueArray)
 {
	 int result = PI_PLM(ID, szAxes, pdValueArray) ; //����������λ
	 return result;
 }
 AAROBOTLIB_API int setNegSoftLimit(int ID, const char* szAxes, const double* pdValueArray)
 {
	 int result = PI_NLM(ID, szAxes, pdValueArray) ; 
	 return result;
 }
 AAROBOTLIB_API int activeSoftLimit(int ID, const char* szAxes, const int* pbValueArray)
 {
	 int result = PI_SSL(ID, szAxes, pbValueArray) ; 
	 return result;
 }
 

 //BOOL PI_KRM (int ID, const char* szNameOfCoordSystem) //ɾ��һ������ϵ

 //BOOL PI_KST (int ID, const char* szNameOfCoordSystem, const char* szAxes, const double* pdValueArray) //һ���µ�kst����
	
 //BOOL PI_qKET (int ID, const char* szTypes, char* buffer, int bufsize)//������������ѯ���������ϵ

 //BOOL PI_PLM (int ID, const char* szAxes, const double* pdValueArray) //��������λ

  //BOOL PI_NLM (int ID, const char* szAxes, const double* pdValueArray) //���ø���λ

//BOOL PI_KEN (int ID, const char* szNameOfCoordSystem)//����һ������ϵ
 //BOOL PI_SSL (int ID, const char* szAxes, const BOOL* pbValueArray)   ��������λ

 //BOOL PI_qKET (int ID, const char* szNamesOfCoordSystems, char* buffer, int bufsize) //��������szNamesOfCoordSystems����Ϊ��