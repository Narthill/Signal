// unittester.cpp : �������̨Ӧ�ó������ڵ㡣
//���Ե�Ԫ

#include "stdafx.h"
#define FALSE 0
#define TRUE 1

#include "../RobotLib/RobotLib.h"

int _tmain(int argc, _TCHAR* argv[])
{
	char axis[] = "Z"; //��

	//���ӿ�����
	int ID;
	int iComPort=5;
	int iBaudRate=115200;
	ID=connectToRobot(iComPort,iBaudRate);
	if (ID<0)
	{
		printf("Connect falied.\n");
		return FALSE;
	}
	if(!Reference(ID, axis))
	{
		return FALSE;
	}

	char szNameOfCoordSystem[]="Tool";
	double ToolCoordinateValue=0.4;
	 if(DefineToolCoordinate(ID, szNameOfCoordSystem,axis, &ToolCoordinateValue)==FALSE)
	 {
		 return FALSE;
	 }

	 if(EnabledCoordinateSystems(ID,szNameOfCoordSystem)==FALSE)
	 {
		 return FALSE;
	 }
	 double curr;
	 getCurrentPosition(ID, axis, &curr);
	 printf("%lf\n",curr);



	return 0;
}


