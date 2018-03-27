
#ifndef DYNAMICLOAD_H
#define DYNAMICLOAD_H

// ������װ�Ľӿ������ﶨ��

//��ʼ��
int init();

//����������
int  ConnectRobot(int iComPort, int iBaudRate);

//����
bool ReferenceStage(int iID,char* szAxis);

//���ö�Ӧ��������Ӧ�������λ��Χ��������ֵ,LowOrHigh��ֵ:high=1,low=0
BOOL sSoftLimit(int ID, const char* axisArr, const double* SoftLimitLowEnd, int LowOrHigh);

// ��ȡ��Ӧ��������Ӧ�������λ��Χ��������ֵ,LowOrHigh��ֵ:high=1,low=0
BOOL gSoftLimit(int ID, const char* axisArr, double* SoftLowLimit, int LowOrHigh);

// ��ȡ��ǰ����
BOOL gCurrentPosition(int ID, const char* axisArr, double* posArr);

//���õ�ǰ����
BOOL sCurrentPosition(int ID, const char* axisArr,const double* posArr);

//�������ĵ�����
BOOL sPivotPointCoordinates(int ID, const char* axisArr,const double* Cordinate);

//��ȡ���ĵ�����
BOOL gPivotPointCoordinates(int ID, const char* axisArr, double* posArr);

//��ȡ��ǰӲ����λ��Χ��LowOrHigh=1����ȡ���ֵ��LowOrHigh=0����ȡ��Сֵ
BOOL gEndTravelRange(int ID, const char* axisArr, double* rangeArr, int LowOrHigh);


//�����ٶ�
BOOL sAxisVelocity(int ID, const char* axisArr, const double* speedArr);

//��ȡ���ٶ�
 BOOL gCurrentVelocity(int ID, const char* axisArr, double* speedArr);

// ����һ�β����ľ���
 BOOL sStepSizeDistance(int ID, const char* axisArr, double* stepArr);

// �ߵ�һ��Ŀ���
BOOL MoveToTarget(int ID, const char* axisArr, const double* targetArr);

// ��ԭ��
BOOL GoHomePosition(int ID, const char* axisArr);

//defines a new Tool coordinate system (KST type).
BOOL sDefineToolCoordinate(int ID, const char* szNameOfCoordSystem, const char* szAxes, const double* pdValueArray);


//Move szAxes relative to current position and orientation in Tool coordinate system.
BOOL sMoveRelativeToToolCoordinateSystem (int ID, const char* szAxes, const double* pdValueArray);

//Move szAxes relative to current position and orientation in Work coordinate system
BOOL sMoveRelativeToWorkCoordinateSystem (int ID, const char* szAxes, const double* pdValueArray);


//defines a new Work coordinate system
BOOL sNewWorkCoordinateSystem (int ID, const char* szNameOfCoordSystem, const char* szAxes, const double* pdValueArray);

//Enables an already defined coordinate system.
BOOL sEnabledCoordinateSystems (int ID, const char* EnabledCoordinateName);

//Lists enabled coordinate systems by name.
BOOL gListsEnabledCoordinateSystems(int ID, const char* EnabledCoordinateNameList, char* buffer, int bufsize);
#endif
