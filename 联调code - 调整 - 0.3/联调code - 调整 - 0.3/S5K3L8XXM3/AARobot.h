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

	
private:
	string				m_szIP;
	int					m_Port;
	int					m_robotID;
	bool				m_connected;


};