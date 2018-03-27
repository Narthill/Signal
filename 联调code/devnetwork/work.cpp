

#include <process.h>//���̺߳���
#include "work.h"
#include <iostream>
#include <fstream>

#include "Config.h"

#include <winsock2.h>
#pragma comment(lib , "ws2_32.lib") 



#define DEVNETWORKCONFIG devnetwork.json
#define SERVERPORT  50001


NotifyWork g_call; //dlg�ṩ�Ļص�����



CWork* CWork::m_sInstance = NULL;
CWork* CWork::GetInstance()
{
	if(NULL == m_sInstance)
	{
		m_sInstance = new CWork();
	}
	return m_sInstance;
}

void CWork::ReleaseInstance()
{
	if (m_sInstance)
	{
		delete m_sInstance;
	}

}

CWork::CWork()
{
	InitializeCriticalSection(&vecCS);
	m_hEvtQuit = CreateEvent(NULL,FALSE, TRUE, NULL);//�Ƿ��ֶ�����,��ʼ��true,����

	for (int i=0;i<AAROBOTPROCESSNUM;i++)//�����������߳�
	{	
		AARobotProcessEvent[i] = CreateEvent(NULL, false, false, NULL);	//�����ֶ�����,��ʼ״̬Ϊfalse
	}
	//��ʼ������,Ӧ���ó�ʼ���б�ķ�ʽ
	m_bRun = false;
}

CWork::~CWork(void)
{
	StopWork();
	ReleaseRegistData();
	DeleteCriticalSection(&vecCS);  
}

int CWork::parseJsonFile(string filename)
{
	int result = 0;
	long size = 0;
	char *buffer = nullptr;
	struct json_token tokens[100];

	// step 1.1 �������ļ�
	ifstream in(filename);
	in.seekg(0, ios::end); //�����ļ�ָ�뵽�ļ�����β��
	size = (long)in.tellg();
	if (size <= 0){
		return -1;
	}
	in.seekg (0, ios::beg);

	buffer = new char[size];
	in.read (buffer, size);
	in.close();

	result = parse_json(buffer, size, tokens, sizeof(tokens) / sizeof(tokens[0]));
	if (result < 0){
		return result;
	}

	return result;
}

int CWork::Init()
{
	//�������ļ�
	CFG_INSTANCE;//��ȡ���ļ�
	Openport = CFG_INSTANCE->get_cfg_int("SystemCfg","OpenedPort");
	if (Openport == 0)
	{
		Openport = SERVERPORT;
	}
	//���̴߳���
	StartWork();


	return 0;

}

int CWork::RegistWorkNotify(WorkNotifyFun fun, void* param)
{
	NotifyWork* pNotify = NULL;
	vector<NotifyWork*>::iterator it;



	if(fun == NULL)return -1;

	EnterCriticalSection(&vecCS);
	for(it = m_vecNotifyWork.begin(); it != m_vecNotifyWork.end(); it++)
	{
		if (fun == (*it)->fun)
		{
			pNotify = *it;
			pNotify->param = param;
		}
	}
	if (!pNotify)
	{
		pNotify = new NotifyWork;
		pNotify->fun = fun;
		pNotify->param = param;
		m_vecNotifyWork.push_back(pNotify);
	}
	LeaveCriticalSection(&vecCS);
	return 0;
}

void CWork::ReleaseRegistData()
{
	vector<NotifyWork*>::iterator it;

	EnterCriticalSection(&vecCS);
	for(it = m_vecNotifyWork.begin(); it != m_vecNotifyWork.end(); it++)
	{
		delete *it;
	}
	m_vecNotifyWork.clear();
	LeaveCriticalSection(&vecCS);
}

vector<AANotifyResult> CWork::NotifyCall(int evt, AANotifyData *pdata)
{
	vector<AANotifyResult>ret ;

	vector<NotifyWork*>::iterator it;

	EnterCriticalSection(&vecCS);
	for (it = m_vecNotifyWork.begin(); it != m_vecNotifyWork.end(); it++)//����ֻ��һ��
	{
		ret = (*it)->fun(evt, *pdata, (*it)->param);
	}	
	LeaveCriticalSection(&vecCS);

	return ret ;
}

void CWork::StartWork()
{
	if(true == m_bRun) return;
	m_bRun = true;
	if (0 == WaitForSingleObject(m_hEvtQuit,INFINITE))
	{
		//pool_thread(WorkThread, this);	

		//��һ���߳�:�����������
		mg_mgr_init(&mgr, NULL);
		devnethandle = (HANDLE)_beginthreadex( NULL, 0, (unsigned int (__stdcall *)(void *))WorkThread, (LPVOID)this, 0, 0);	
		//�ڶ����߳�:1 2�Ż������յ�ָ�������
		AARobotProcesshandle[0] = (HANDLE)_beginthreadex( NULL, 0, (unsigned int (__stdcall *)(void *))AARobotProcess1, (LPVOID)this, 0, 0);	
		//�������߳�:3 4�Ż������յ�ָ�������
		AARobotProcesshandle[1] = (HANDLE)_beginthreadex( NULL, 0, (unsigned int (__stdcall *)(void *))AARobotProcess2, (LPVOID)this, 0, 0);	

		//���ĸ��߳�:1 2�Ż������յ�����ָ�������
		AARobotProcesshandle[2] = (HANDLE)_beginthreadex( NULL, 0, (unsigned int (__stdcall *)(void *))AARobotProcess3, (LPVOID)this, 0, 0);	
		//������߳�:3 4�Ż������յ�ָ�������
		AARobotProcesshandle[3] = (HANDLE)_beginthreadex( NULL, 0, (unsigned int (__stdcall *)(void *))AARobotProcess4, (LPVOID)this, 0, 0);	



	}
}

//ֹͣ�������˳�
void CWork::StopWork()
{
	if(m_bRun == false)return ;

	m_bRun = false;

	SetEvent(m_hEvtQuit);   //���˳��ź�	
	//���Ѵ����̣߳��˳�
	for (int i = 0;i< AAROBOTPROCESSNUM;i++)
	{
		SetEvent(AARobotProcessEvent[i]);
	}



	for (int i = 0;i< AAROBOTPROCESSNUM;i++)
	{
		WaitForSingleObject(AARobotProcesshandle[i], INFINITE);
		CloseHandle(AARobotProcesshandle[i]);
	}
	
	for (int i = 0;i< AAROBOTPROCESSNUM;i++)//�Ƿ���Ҫ
	{
		CloseHandle(AARobotProcessEvent[i]);
	}

	WaitForSingleObject(m_hEvtQuit,INFINITE);
	CloseHandle(m_hEvtQuit);

	WaitForSingleObject(devnethandle, INFINITE);
	CloseHandle(devnethandle);

	Sleep(1000);//temp
}

int CWork::Process()
{

	bindAsServer(0,Openport);//�����������
	while (m_bRun) {

		mg_mgr_poll(getMgr(), 100);
	
		Sleep(50);
	}	
	mg_mgr_free(getMgr());
	

	return 0;
}
//int WINAPI CWork::AARobotProcess1(void* param)//�������߳�
//{
//	CWork* pWork = (CWork*)param;
//	string msg;
//	int ret =0 ;
//	vector<AANotifyResult>result;
//	while (pWork->m_bRun)
//	{
//		WaitForSingleObject(pWork->AARobotProcessEvent[0], INFINITE);
//		if (pWork->m_bRun == false)
//		{
//			return 0;
//		}
//		//....����....
//		result = pWork->NotifyCall(DAA, &(pWork->noteMsg[0]));
//		struct mg_connection *nc = pWork->getNCList().front();
//
//		ret = pWork->composeResult(result,msg,0,STARTAA);
//		if (ret == 0)
//		{
//			list<struct mg_connection *>::iterator iter;
//			for(iter = pWork->getNCList().begin() ; iter != pWork->getNCList().end() ; iter++)//����Ϣ�������ӵ�������Ŀͻ�
//			{
//				mg_send(*iter,msg.c_str(),msg.length());
//			}
//
//		}
//
//
//		WaitForSingleObject(pWork->AARobotProcessEvent[0], 0);//��������,����ֻ�ǲ���,��Ҫ�ɲ�Ҫ
//		Sleep(50);
//	}
//
//	return 0;
//
//
//}
int WINAPI CWork::AARobotProcess1(void* param)//�������߳�
{
	CWork* pWork = (CWork*)param;
	string msg;
	int ret =0 ;
	vector<AANotifyResult>result;
	while (pWork->m_bRun)
	{
		WaitForSingleObject(pWork->AARobotProcessEvent[0], INFINITE);
		if (pWork->m_bRun == false)
		{
			return 0;
		}
		//....����....
		result = pWork->NotifyCall(DAA, &(pWork->noteMsg[0]));
		struct mg_connection *nc = pWork->getNCList().front();
		ret = pWork->NewcomposeResult(result,msg,result[0].type);
		if (ret == 0)
		{
			list<struct mg_connection *>::iterator iter;
			for(iter = pWork->getNCList().begin() ; iter != pWork->getNCList().end() ; iter++)//����Ϣ�������ӵ�������Ŀͻ�
			{
				mg_send(*iter,msg.c_str(),msg.length());
			}			
		}		
		WaitForSingleObject(pWork->AARobotProcessEvent[0], 0);//��������,����ֻ�ǲ���,��Ҫ�ɲ�Ҫ
		Sleep(50);
	}
	return 0;
}
int WINAPI CWork::AARobotProcess2(void* param)//�������߳�
{

	CWork* pWork = (CWork*)param;
	string msg;
	int ret =0 ;
	vector<AANotifyResult>result;
	while (pWork->m_bRun)
	{
		WaitForSingleObject(pWork->AARobotProcessEvent[1], INFINITE);
		if (pWork->m_bRun == false)
		{
			return 0;
		}
		//....����....
		result = pWork->NotifyCall(DAA, &(pWork->noteMsg[1]));
		struct mg_connection *nc = pWork->getNCList().front();	
		//ret = pWork->composeResult(result,msg,1,STARTAA);
		ret = pWork->NewcomposeResult(result,msg,result[0].type);
		if (ret == 0)
		{
			list<struct mg_connection *>::iterator iter;
			for(iter = pWork->getNCList().begin() ; iter != pWork->getNCList().end() ; iter++)//����Ϣ�������ӵ�������Ŀͻ�
			{
				mg_send(*iter,msg.c_str(),msg.length());
			}
		}
		WaitForSingleObject(pWork->AARobotProcessEvent[1], 0);//��������,����ֻ�ǲ���,��Ҫ�ɲ�Ҫ
		Sleep(50);
	}
	return 0;
}
int WINAPI CWork::AARobotProcess3(void* param)//�������߳�
{
	CWork* pWork = (CWork*)param;
	string msg;
	int ret =0 ;
	vector<AANotifyResult>result;
	while (pWork->m_bRun)
	{
		WaitForSingleObject(pWork->AARobotProcessEvent[2], INFINITE);
		if (pWork->m_bRun == false)
		{
			return 0;
		}
		//....����....
		result = pWork->NotifyCall(LIGHT, &(pWork->noteMsg[2]));
		struct mg_connection *nc = pWork->getNCList().front();
		ret = pWork->NewcomposeResult(result,msg,result[0].type);
		if (ret == 0)
		{
			list<struct mg_connection *>::iterator iter;
			for(iter = pWork->getNCList().begin() ; iter != pWork->getNCList().end() ; iter++)//����Ϣ�������ӵ�������Ŀͻ�
			{
				mg_send(*iter,msg.c_str(),msg.length());
			}

		}
		WaitForSingleObject(pWork->AARobotProcessEvent[2], 0);//��������,����ֻ�ǲ���,��Ҫ�ɲ�Ҫ
		Sleep(50);
	}
	return 0;
}
int WINAPI CWork::AARobotProcess4(void* param)//�������߳�
{

	CWork* pWork = (CWork*)param;
	string msg;
	int ret =0 ;
	vector<AANotifyResult>result;
	while (pWork->m_bRun)
	{
		WaitForSingleObject(pWork->AARobotProcessEvent[3], INFINITE);
		if (pWork->m_bRun == false)
		{
			return 0;
		}
		//....����....
		result = pWork->NotifyCall(LIGHT, &(pWork->noteMsg[3]));
		struct mg_connection *nc = pWork->getNCList().front();
		ret = pWork->NewcomposeResult(result,msg,result[0].type);
		if (ret == 0)
		{
			list<struct mg_connection *>::iterator iter;
			for(iter = pWork->getNCList().begin() ; iter != pWork->getNCList().end() ; iter++)//����Ϣ�������ӵ�������Ŀͻ�
			{
				mg_send(*iter,msg.c_str(),msg.length());
			}
		}
		WaitForSingleObject(pWork->AARobotProcessEvent[3], 0);//��������,����ֻ�ǲ���,��Ҫ�ɲ�Ҫ
		Sleep(50);
	}
	return 0;
}

int WINAPI CWork::WorkThread(void* param)
{
	CWork* pWork = (CWork*)param;
	return pWork->Process();
}

struct mg_mgr *CWork::getMgr()
{
	return &this->mgr;
}
int CWork::bindAsServer(int type,int port)
{
	struct mg_bind_opts bindOpts = {0};
	struct mg_connection *nc = nullptr;

	memset(&bindOpts, 0, sizeof(bindOpts));

	char cport[10] = {0};
	sprintf(cport,"%d",port);

	switch(type)
	{
	case 0://tcp������
		bindOpts.user_data = this;//������󶨽�ȥ
		nc = mg_bind_opt(&mgr,cport,eventHandler,bindOpts);
		if (NULL == nc)
		{
			return -1;
		}
		break;
	case 1:
		break;
	default:
		break;

	}

	return 0;

}
list<struct mg_connection *> &CWork::getNCList()
{
	return this->ncList;
}
void CWork::setStatus(ConnectStatus sts)
{
	this->status = sts;
}
int CWork::tryToReconnectOrReBind(struct mg_connection *nc)
{
	CWork *work = (CWork *)nc->user_data;

	//if (0 == mg_ncasecmp(CWork->getRole().c_str(), "server", strlen("server")))
	{
		//connectToServer(commDMO);
	}
	//else
	{
		// �Է���client���������server���Ͽ��˲�����ζ������Ǽ�����nc����
		if (nc->flags & MG_F_LISTENING)
		{
			bindAsServer(0,SERVERPORT);//��д����������Ϊ�����ļ�
		}
	}

	return 0;
}
int CWork::composeResult(vector<AANotifyResult>& result,string& msg,int index,int evttype)
{
	int ret = 0;
	msg.clear();
	string res[2];
	
	if (result.size() != 2 || index<0 || index >4)
	{
		return -1;
	}
	for (int i=0;i<result.size();i++)
	{
		if (result[i].result== 0)
		{
			res[i] = "1";
		}
		else
		{
			res[i] = "0";
		}
	}

	char visionid[10] = {0};
	

	if (evttype == STARTAA)
	{
		sprintf_s(visionid,"%d",noteMsg[index].funCode);
		msg.append("StartAA,");
	}
	else if (evttype == RESETAA)
	{
		sprintf_s(visionid,"%d",resetMsg.funCode);
		
		msg.append("ResetAA,");
	}
	

	msg.append(visionid);
	msg.append(",");
	msg.append(res[0]);
	msg.append(",");
	msg.append(res[1]);
	msg.append(";");

	/*
	if (noteMsg->visionid>=1 && noteMsg->visionid <= 2)
	{
		msg.append("1,");
		msg.append(res[0]);
		msg.append(",2,");
		msg.append(res[1]);
		msg.append(";");
		
	}
	else	if (noteMsg->visionid>=3 && noteMsg->visionid <=4)
	{
		msg.append("3,");
		msg.append(res[0]);
		msg.append(",4,");
		msg.append(res[1]);
		msg.append(";");

	}*/

	return ret;

}

//��װ˫���msg  :��ҪΪ����
//�յ������ظ�C|3|1|# 
//���Żظ����C|3|1,0,u1,v1,w1,u2,v2,w2|#  �ɹ�,������Ҫ���ŵ���
// C|3|1,1|#  �ɹ�,�Ҳ����ٵ���

int CWork::NewcomposeResult(vector<AANotifyResult>& result,string& msg,int evttype)
{
	int ret = 0;
	msg.clear();
	string res[2];	
	switch(evttype)
	{
	case DAA:
		{			
			int funCode = result[0].funID;
			char temp[1024] = {0};
			sprintf_s(temp, "C|%d|", funCode);
			msg = temp;
			switch(result[0].isfinsh)
			{
			case 0:
				{			
					if ( 1== result[0].result)//����Ҫ����
					{
						sprintf_s(temp, "1,0,%10.4lf,%10.4lf,%10.4lf,%10.4lf,%10.4lf,%10.4lf|#",result[0].tha[0], result[0].tha[1],result[0].tha[2]
						,result[0].thb[0],result[0].thb[1],result[0].thb[2]);
						msg.append(temp);
					}
					else if( 0 == result[0].result)//ͼ�����쳣
					{
						msg.append("0|#");
					}
				}
				break;
			case 1://����˵������������
				msg.append("1,1|#");
				break;
			default:
				break;				
			}
		}
		break;
	case STARTAA:
		{
			int funCode = result[0].funID;
			char temp[128] = {0};
			sprintf_s(temp, "C|%d|", funCode);
			msg = temp;
			sprintf_s(temp, "%d|#", result[0].result);//˫����ͷֻҪһ��ͬ�ͺ���
			msg.append(temp);
		}
		break;
	case LIGHT:
		{
			int funCode = result[0].funID;
			char temp[128] = {0};
			sprintf_s(temp, "C|%d|", funCode);
			msg = temp;
			if (1 == result[0].result)//����Ϊ����һ����������гɹ����ųɹ�
			{
				msg.append("1|#");
			}
			else
			{
				msg.append("0|#");
			}
		}
		break;

	case RESETAA: //������
		{
			int funCode = result[0].funID;
			char temp[128] = {0};
			sprintf_s(temp, "C|%d|", funCode);
			msg = temp;
			sprintf_s(temp, "%d|#", result[0].result);
			msg.append(temp);
		}
		break;
	default:
		break;
	}
	return ret;
}
int CWork::parseDAAMsg(int& notemsg,string msg,int len)
{
	//��Ϣ��ʽ:S|funCode|#
	int ret = 0;
	std::size_t found1 = msg.find(END_CHAR); ///�ȼ���֤һ��
	if (found1==std::string::npos || (found1 != len-1  && found1 != len-2))  
	{
		ret = -1;
		return ret;
	}
	//�ҵ�һ�������˵�ֵ
	std::size_t found2 = msg.find(SPLIT_FLAG);//SPLIT_FLAG Ϊ|
	if (found2==std::string::npos )
	{
		ret = -1;
		return ret;
	}
	string msg2 = msg.substr(found2+1,found1-found2-2);
	notemsg = atoi(msg2.c_str()); //�ҵ����ܺ�
	return ret;
}

int CWork::parseAAMsg(int& notemsg,string msg,int len)
{
	//��Ϣ��ʽ:StrartAA,1;
	//��Ϣ�ظ���ʽ:StrartAA,1,1,1; ��ʾ1 ok,2 ok
	int ret = 0;
	std::size_t found1 = msg.find(";"); ///�ȼ���֤һ��
	if (found1==std::string::npos || (found1 != len-1  && found1 != len-2))  
	{
		ret = -1;
		return ret;
	}
	//�ҵ�һ�������˵�ֵ
	std::size_t found2 = msg.find(",");
	if (found2==std::string::npos )
	{
		ret = -1;
		return ret;
	}
	string msg2 = msg.substr(found2+1,found1-found2-1);
	notemsg = atoi(msg2.c_str());

	return ret;
}
void CWork::eventHandler(struct mg_connection *nc, int ev, void *ev_data)
{
	char buf[256] = {0};
	int size = 0;	
	struct mbuf *io = &nc->recv_mbuf;
	struct websocket_message *wm = (struct websocket_message *) ev_data;
	CWork *work = (CWork *)nc->user_data;
	
	switch (ev) {
	case MG_EV_POLL:
		break;
	case MG_EV_ACCEPT:
		socket_address *sa;
		sa = (socket_address *)ev_data;
		mg_sock_addr_to_str(sa, buf, sizeof(buf),  MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);
		//�����б�ļ��������
		printf("accept connection  %#0x from %s.\n", nc, buf);
		work->setStatus(CONNECTED);
		work->getNCList().push_back(nc);
		break;
	case MG_EV_CONNECT:
		if (* (int *) ev_data != 0) {
			
		}
		else{	
			work->setStatus(CONNECTED);
			work->getNCList().push_back(nc);
		}
		break;
	case MG_EV_RECV: //tcp��Ϣ
		{
			//��Э�飺���߳�
			//�յ���Ϣ��mg_send(nc, msg, msgLen);				

			if (0== strncmp(io->buf,"StartAA",strlen("StartAA")))
			{
				char msgTemp[256] = {0};
				int iolen = io->len;
				strncpy_s(msgTemp,io->buf,io->len);
				mbuf_remove(io, io->len);
				//�ٴ���֤�����������ݵ���ȷ��
				int notemsg = 0;
				int ret = work->parseAAMsg(notemsg,msgTemp,iolen);
				//mg_send(nc,"StartAA,1,1,2,1;",strlen("StartAA,1,1,2,1;"));			
				if (ret == 0)
				{			
					if (notemsg ==1 || notemsg ==2)
					{
						work->noteMsg[0].funCode = notemsg;
						work->noteMsg[0].robotid1 = 1;
						work->noteMsg[0].robotid2 = 2;
						SetEvent(work->AARobotProcessEvent[0]);
					}
					else if (notemsg ==3 || notemsg ==4)
					{
						work->noteMsg[1].funCode = notemsg;
						work->noteMsg[1].robotid1 = 3;
						work->noteMsg[1].robotid2 = 4;
						SetEvent(work->AARobotProcessEvent[1]);
					}
					else
					{
						//test
					}				
				}
				else//��ʽ���ϸ�
				{
					mbuf_remove(io, io->len);
					mg_send(nc, "��ʽ���ϸ�", strlen("��ʽ���ϸ�"));	
				}
			}
			if (0== strncmp(io->buf,"ResetAA",strlen("ResetAA")))
			{
				char msgTemp[256] = {0};
				int iolen = io->len;
				strncpy_s(msgTemp,io->buf,io->len);
				mbuf_remove(io, io->len);
				//�ٴ���֤�����������ݵ���ȷ��
				int notemsg = 0;
				int ret = work->parseAAMsg(notemsg,msgTemp,iolen);
				if (ret == 0)
				{		//�;�ͷ֮ǰ����һ�����ֹ���,����������ֶ�Ӧ������������ʹ�����Ƕ���ԭ,����������
						work->resetMsg.funCode = notemsg;//������,

						if (notemsg == 1 || notemsg ==2)
						{
								work->resetMsg.robotid1 = 1;
								work->resetMsg.robotid2 = 2;
						}
						else if (notemsg == 3 || notemsg ==4)
						{
							work->resetMsg.robotid1 = 3;
							work->resetMsg.robotid2 = 4;
						}
						else
						{
							//test
						}											
						vector<AANotifyResult>result = work->NotifyCall(RESETAA, &(work->resetMsg));
						struct mg_connection *nc = work->getNCList().front();
						string msg;
						ret = work->composeResult(result,msg,0,RESETAA);
						if (ret == 0)
						{
							list<struct mg_connection *>::iterator iter;
							for(iter = work->getNCList().begin() ; iter != work->getNCList().end() ; iter++)//����Ϣ�������ӵ�������Ŀͻ�
							{
								mg_send(*iter,msg.c_str(),msg.length());
							}

						}
				}

				else//��ʽ���ϸ�
				{
					mbuf_remove(io, io->len);
					mg_send(nc, "��ʽ���ϸ�", strlen("��ʽ���ϸ�"));	
				}

			}

			if(0== strncmp(io->buf,RECVHEAD,strlen(RECVHEAD))) //�����յ�����Ϣͷ
			{
				
				char msgTemp[256] = {0};
				int iolen = io->len;
				strncpy_s(msgTemp,io->buf,io->len);
				if (msgTemp[iolen-1] != '#' )
				{
					return ;//��������ָ��
				}
				mbuf_remove(io, io->len);

				//�ٴ���֤�����������ݵ���ȷ��
				int notemsg = 0;
				int ret = work->parseDAAMsg(notemsg,msgTemp,iolen);
				//mg_send(nc,"C|3|1,0,0.1,0.1,0.1,0.1,0.1,0.1|#",strlen("C|3|1,0,0.1,0.1,0.1,0.1,0.1,0.1|#"));			
				if (ret == 0)
				{	
					//���������յ�����Ϣ��
					char temp[128] = {0};
					sprintf_s(temp, "C|%d|1|#", notemsg);

					//mg_send(nc,temp,strlen(temp));

					int a = send(nc->sock,temp,strlen(temp),0);//��tcp��ֱ�ӷ��ͣ�������mg_send

					if (notemsg == 3 /* || notemsg == 5 */)
					{
						work->noteMsg[0].funCode = notemsg; //����������Ϊ���Ӿ�ͨ��id,���������ͨ����Ӧ3��5�������ֻ��һ��ͨ����Ӧ3
						//work->noteMsg[0].robotid1 = 1;
						//work->noteMsg[0].robotid2 = 2;
						SetEvent(work->AARobotProcessEvent[0]);
					}
					else if (notemsg ==4 /* || notemsg ==6 */)
					{
						work->noteMsg[1].funCode = notemsg;
						//work->noteMsg[1].robotid1 = 3;
						//work->noteMsg[1].robotid2 = 4;
						SetEvent(work->AARobotProcessEvent[1]);
					}
					else if (notemsg == 1) //���������Ϊ�ǹ��ܺ�
					{
						work->noteMsg[2].funCode = notemsg; //ע�����ﴫ�ݵĲ�����ÿ���߳��Ƕ�����
						SetEvent(work->AARobotProcessEvent[2]);
					}
					else if (notemsg == 2)
					{
						work->noteMsg[3].funCode = notemsg;
						SetEvent(work->AARobotProcessEvent[3]);
					}
					else
					{
						//test
					}				
				}
				else//��ʽ���ϸ�
				{
					mbuf_remove(io, io->len);
					mg_send(nc, "��ʽ���ϸ�", strlen("��ʽ���ϸ�"));	
				}	
			}
			else//��ʽ���ϸ�
			{
				mbuf_remove(io, io->len);
				mg_send(nc, "��ʽ���ϸ�", strlen("��ʽ���ϸ�"));	
			}

		}
		
		break;
	case MG_EV_CLOSE:
		printf("close connection %#0x.\n", nc);
		work->setStatus(DISCONNECT);
		work->getNCList().remove(nc);
		work->tryToReconnectOrReBind(nc);
		break;	
	case MG_EV_WEBSOCKET_FRAME:
		SYSTEMTIME sys;
		GetLocalTime(&sys);
		memcpy(buf, wm->data, wm->size);
		printf("<< %s  [%02d:%02d;%02d]\n", buf, sys.wHour, sys.wMinute, sys.wSecond);

		if (0 == strncmp((char *)wm->data, "TTT", wm->size))
		{						
			string str = "";
			char temp[256] = {0};
			memset(buf, 0, sizeof(buf));

			if (0)
			{			
			}
			else
			{
				int magic[4];
				char buf[32];
				memset(buf, 0, sizeof(buf));
				for(int i = 0; i < 4; i++){
					magic[i] = rand()%256;
					//magic[i] = magic[i] < 16 ? (magic[i] + 16) : magic[i];
				}
				sprintf(buf, "%02x%02x%02x%02x", magic[0], magic[1], magic[2], magic[3]);
				//str.append("11223344;");
				str.append(buf);
			}

			printf(">> %s\n", str.c_str());
			mg_send_websocket_frame(nc, WEBSOCKET_OP_TEXT, str.c_str(), str.length()); 			
		}		
		break;
	default:
		break;
	}
	
}
void  CWork::test()
{
	/*NotifyData t;
	t.robotid1 = 1;
	NotifyCall(0, &t);*/
}

