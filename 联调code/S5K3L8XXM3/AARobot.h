/***********************************************************************
AARobot�ࣺ��װ��PI��������
***********************************************************************/

#pragma once
#include <string>
using namespace  std;
#define MAX_IPV4_LEN 16

class CAARobot
{
public:
	static int EnumerateAATCPIPDevices(char* szBuffer, int iBufferSize, const char* szFilter);
	CAARobot();
	CAARobot(	string	szIP,int Port,int robotID,bool connected);
	~CAARobot();

	/*static */int ConnectAARobotByIPAndPort(const char* ip, int port);//����
	int GetAAAbsCurrentPosition(int controlID,const char *axisString, double *posArray);
	int ConnectAARobotbyDescription(const char* szDescription) ;
	int MoveAARelative(int robotID,const char *axisString, const double *posArray);//�����λ��
	int MoveAAAbsolute(int robotID,const char *axisString, const double *posArray);//�߾���λ��
	bool IsAAConnected (int robotID);
	static bool IsAAConnectedByIpAndPort(const char* ip, int port);
	void CloseAAConnection (int robotID);

	string& GetAARobotIP();
	int GetAARobotPort();
	int GetAARobotID();
	bool GetAARobotConnected();

	void SetAARobotIP(string& ip);
	void SetAARobotPort(int port);
	void SetAARobotID(int robotID);
	void SetAARobotConnected(bool flag);

	//��ȡSPI����ϵ��Ϣ,��ʱû��
	int GetPivotPointCoordinates(int robotID, const char* axisArr, double* posArr);
	//��ȡKLT����ϵ��Ϣ
	int GetKLTCoordinates(int ID, const char* szStartCoordSystem, const char* szEndCoordSystem, char* buffer, int bufsize);
	//��ȡϵͳ�ٶ�mm/s
	int GetSystemVelocity(int ID, double* speedArr);
	//����ϵͳ�ٶ�
	int SetSystemVelocity(int ID,  double speedArr);
	//ʹ��һ������ϵ
	int EnableCoordinate(int ID, const char* szNameOfCoordSystem);

	//����һ��kst����ϵ
	int DefineKSTCoordinate(int ID, const char* szNameOfCoordSystem, const char* axisArr,  double* ToolCoordinateValue);
	//ɾ��һ������ϵ
	int DeleteOneCoordinate(int ID, const char* szNameOfCoordSystem);
	//����һ������ϵ
	int ActiveOneCoordinate(int ID, const char* szNameOfCoordSystem);
	//��ѯ���м��������ϵ������������
    int QueryActiveCoordinates(int ID, const char* szNamesOfCoordSystems, char* buffer, int bufsize);
	//����������λ
	int SetPosSoftLimit(int ID, const char* szAxes, const double* pdValueArray);
	//���ø�����λ
	int SetNegSoftLimit(int ID, const char* szAxes, const double* pdValueArray);
	//��������λ
	int ActiveSoftLimit(int ID, const char* szAxes, const int* pbValueArray);
	
private:
	string				m_szIP;
	int					m_Port;
	int					m_robotID;
	bool				m_connected;


};