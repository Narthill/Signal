// RobotLib.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "RobotLib.h"
#include "stdio.h"
#include <string.h>
#include <stdlib.h>

#include "DynamicLoad.h"

#include "mtGCS.h"


ROBOTLIB_API int unitTest(void)
{
	return init();
}

//���ӻ�����
 ROBOTLIB_API int connectToRobot(int iComPort, int iBaudRate)
{
	int ID=ConnectRobot(iComPort,iBaudRate);
	return ID;
}




//Referebce����
ROBOTLIB_API BOOL Reference(int ID, char* axis)
{
	if(ReferenceStage(ID,axis)==FALSE)
	{
	//	printf("Referebce falied\n");
		return FALSE;
	}
	return TRUE;
}


// ��������λ��LowOrHigh=1���������ֵ��LowOrHigh=0��������Сֵ
ROBOTLIB_API BOOL setPositionSoftLimit(int ID, const char* axisArr, const double* SoftLimitLowEnd, int LowOrHigh)
{
	if(sSoftLimit( ID,axisArr, SoftLimitLowEnd, LowOrHigh)==FALSE)
	{
	//	printf("set the position soft limit\n");
		return FALSE;
	}
	return TRUE;
}

// ��ȡ����λ��LowOrHigh=1����ȡ���ֵ��LowOrHigh=0����ȡ��Сֵ
ROBOTLIB_API BOOL getPositionSoftLimit(int ID, const char* axisArr, double* SoftLowLimit, int LowOrHigh)
{
	if(gSoftLimit(ID,axisArr,SoftLowLimit,LowOrHigh)==FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

//��ȡ��ǰ����
ROBOTLIB_API BOOL getCurrentPosition(int ID, const char* axisArr, double* posArr)
{
	if(gCurrentPosition(ID,axisArr,posArr)==FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

//���õ�ǰ����
ROBOTLIB_API BOOL setCurrentPosition(int ID, const char* axisArr, const double* posArr)
{
	if(sCurrentPosition(ID,axisArr,posArr)== FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

//�������ĵ�����
ROBOTLIB_API BOOL setPivotPointCoordinates(int ID, const char* axisArr,const double* pdValueArray)
{
	if(sPivotPointCoordinates(ID,axisArr,pdValueArray)==FALSE)
	{
		return FALSE; 
	}
	return TRUE;
}


// ��ȡ���ĵ�����
ROBOTLIB_API BOOL getPivotPointCoordinates(int ID, const char* axisArr, double* posArr)
{
	if(gPivotPointCoordinates(ID, axisArr,posArr)==FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

//��ȡ��ǰӲ����λ��Χ��LowOrHigh=1����ȡ���ֵ��LowOrHigh=0����ȡ��Сֵ
ROBOTLIB_API BOOL getEndTravelRange(int ID, const char* axisArr, double* rangeArr, int LowOrHigh)
{
	if((gEndTravelRange(ID, axisArr, rangeArr, LowOrHigh))==FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

// �����ٶ�
ROBOTLIB_API BOOL setAxisVelocity(int ID, const char* axisArr, const double* speedArr)
{
	if((sAxisVelocity(ID, axisArr,speedArr))==FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

//��ȡ�ٶ�
ROBOTLIB_API BOOL getCurrentVelocity(int ID, const char* axisArr, double* speedArr)
{
	if((gCurrentVelocity(ID,axisArr, speedArr))==FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

// ����һ�β����ľ���
ROBOTLIB_API BOOL setStepSizeDistance(int ID, const char* axisArr, double* stepArr)
{
	if((sStepSizeDistance(ID, axisArr,stepArr))==FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

// �ߵ�һ��Ŀ���
ROBOTLIB_API BOOL MoveTo(int ID, const char* axisArr, const double* targetArr)
{
	if((MoveToTarget(ID,axisArr,targetArr))==FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

// ��ԭ��
ROBOTLIB_API BOOL GoToHomePosition(int ID, const char* axisArr)
{
	if((GoHomePosition(ID,axisArr))==FALSE)
	{
		return FALSE;
	}
	return TRUE;
}


//defines a new Tool coordinate system (KST type).
ROBOTLIB_API BOOL DefineToolCoordinate(int ID, const char* szNameOfCoordSystem, const char* szAxes, const double* pdValueArray)
{
	if(sDefineToolCoordinate(ID,szNameOfCoordSystem,szAxes,pdValueArray)==FALSE)
	{
		return FALSE;
	}
	return TRUE;
}


//Move szAxes relative to current position and orientation in Tool coordinate system.
ROBOTLIB_API BOOL MoveRelativeToToolCoordinateSystem (int ID, const char* szAxes, const double* pdValueArray)
{
	if(sMoveRelativeToToolCoordinateSystem (ID,szAxes,pdValueArray)==FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

//Move szAxes relative to current position and orientation in Work coordinate system
ROBOTLIB_API BOOL MoveRelativeToWorkCoordinateSystem (int ID, const char* szAxes, const double* pdValueArray)
{
	if(sMoveRelativeToWorkCoordinateSystem ( ID, szAxes,  pdValueArray)==FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

//defines a new Work coordinate system
ROBOTLIB_API BOOL NewWorkCoordinateSystem (int ID, const char* szNameOfCoordSystem, const char* axisArr, const double* pdValueArray)
{
	if(!sNewWorkCoordinateSystem(ID, szNameOfCoordSystem, axisArr, pdValueArray))
	{
		return FALSE;
	}
	return TRUE;
}



//Enables an already defined coordinate system.
//   @Input:   ID                    ������ID
//             EnabledCoordinateName Ҫ���õ�����ϵ����
//   @Return�� TRUE                  �ɹ�
//             FALSE                 ����
ROBOTLIB_API BOOL EnabledCoordinateSystems (int ID, const char* EnabledCoordinateName)
{
	if(sEnabledCoordinateSystems (ID,EnabledCoordinateName)==FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

//Lists enabled coordinate systems by name.
//   @Input:   ID                        ������ID
//             EnabledCoordinateNameList Ҫ���õ�����ϵ����
//             buffer                    ����
//             bufsize                   �����С
//   @Return�� TRUE                      �ɹ�
//             FALSE                     ����
ROBOTLIB_API BOOL ListsEnabledCoordinateSystems(int ID, const char* EnabledCoordinateNameList, char* buffer, int bufsize)
{
	if(gListsEnabledCoordinateSystems(ID,EnabledCoordinateNameList,buffer,bufsize)==FALSE)
	{
		return FALSE;
	}
	return TRUE;

}