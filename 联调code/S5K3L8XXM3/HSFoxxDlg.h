// HSFoxxDlg.h : header file
//

#if !defined(AFX_HSFOXXDLG_H__045BC92C_F0AA_438B_B102_C789FAC4F43D__INCLUDED_)
#define AFX_HSFOXXDLG_H__045BC92C_F0AA_438B_B102_C789FAC4F43D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//20141203 added..
//if don't need display ,please use this...
//#define _ONLY_GRAB_IMAGE_

#include "dtccm2.h" //dtccm2 lib header files..
#include "imagekit.h" //
#include "afxwin.h"
#include <opencv2\opencv.hpp>
#include <HalconCpp.h>
#include <iostream>
#include <string>
#include<map>
#include "afxcmn.h"
#include "MyStatic.h" 
#include "AA_define_tool.h"
#include "devnetwork.h"
using namespace cv;
using namespace std;
using namespace HalconCpp;
enum RUNMODE //�ϵ�
{
	RUNMODE_PLAY=0,
	RUNMODE_PAUSE,
	RUNMODE_STOP,
};

enum NEWRUNMODE
{
	TESTOR_START=1,
	TESTOR_STOP,
};

#define  AA_MAXVISION 8

enum EM_PIROBOT_MVTYPE
{
	EMOVE_CP = 0,       //�����˶�
	EMOVE_LONG_STEP,    //������
	EMOVE_MEDIU_STEP,   //�еȾ���
	EMOVE_SHORT_STEP,   //�̾���
};

#define PI_NUM 8
#define PI_MAX_AXIS 6


#define DAAROBOTPROCESSNUM 4


/////////////////////////////////////////////////////////////////////////////
// CHSFoxxDlg dialog
class CAARobot ;//����һ����
class CAAVision ;
struct AANotifyData;

#define MAINWINDOW_INSTANCE CHSFoxxDlg::GetInstance()

class CHSFoxxDlg :  public CDialog //public CDialogEx
{
// Construction
private:
	void SetMoveType(int mvType);   //�����˶�ģʽ m_iMoveType
	int             m_iMoveType;        //��ǰ�˶�ģʽ EM_STATION_MVTYPE
	CEdit   m_edtStep[PI_MAX_AXIS];  //��������ؼ�
	double  m_stepVal[PI_MAX_AXIS];  //����ֵ
	CButton m_btMoveL[PI_MAX_AXIS];  //�����ƶ���ؼ�
	CButton m_btMoveR[PI_MAX_AXIS];  //�����ƶ�
	double             m_posVal[PI_MAX_AXIS];        //cur pos
	CEdit   m_edtVal[PI_MAX_AXIS];   //��ǰλ������
	void MoveOffset(int iAxis, int iFlag = 1);        //1���� -1 ����

	void ZoomPicture(int index );
	void InitZoomPicture(int index );
	void ChangeVideoWind(int index );

	bool m_dlgExit;

	int AARobotProcessParam[DAAROBOTPROCESSNUM*2];//�����̲߳���,��ʱ����Ϊ���Σ�ֻ��Ҫ�����Ӿ�ͨ��������һ�����ܺŶ�Ӧ�����Ӿ�����
	AANotifyResult AARobotProcessResult[DAAROBOTPROCESSNUM];//������

	HANDLE AARobotWakeResult[DAAROBOTPROCESSNUM];//�ȴ�STARTAA �������¼�
	HANDLE dlgAARobotProcessEvent[DAAROBOTPROCESSNUM];//�����¼�
	HANDLE AARobotProcesshandle[DAAROBOTPROCESSNUM];//�����߳̾��

	HANDLE AARobotResetAAWakeResult[DAAROBOTPROCESSNUM];//��λ����¼�
	int AARobotResetProcessResult[DAAROBOTPROCESSNUM];//��λ������

	map<int,string >robotIDmap;//װ�����ļ��е�ip����ŵ�

	//����picture ������
	bool m_pictureFlag[8];

	//˫�� ͬ���漰����
	CRITICAL_SECTION m_csRoot12;
	CRITICAL_SECTION m_csRoot34;
	//HANDLE m_eventRobot12;
	//HANDLE m_eventRobot34;
	volatile int m_intRobot12x;
	volatile int m_intRobot34x;
	


public:
	HWND  GetHWndByVisionId(int visionID);
	int GetRobotIDByIndex(string strindex);
	int GetVisionIDByIndex(int index,string& cameraName);
	string GetVisionUserNameByIndex(int strindex);
	static int WINAPI AARobotProcess1(void* param);
	static int WINAPI AARobotProcess2(void* param);
	static int WINAPI AARobotProcess3(void* param);
	static int WINAPI AARobotProcess4(void* param);
	static vector<AANotifyResult> WINAPI WorkNotify(int evt, AANotifyData data, void* lparam);
	LRESULT OnMsgNotify(WPARAM wParam, LPARAM lParam);//
    afx_msg LRESULT OnMsgTimer(WPARAM wParam, LPARAM lParam);
	BOOL bGetI2CDataFromLibFile(CString filename, pSensorTab pSensor);
	BOOL bLoadLibIniFile(CString sfilename);
	BOOL LoadLibIniFile(pSensorTab pCurrentSensor,string sfilename);//Ϊ�Ӿ����ṩ��
	BOOL TripleBufferFlip(LPBYTE *pBuf, BOOL bPrimaryBuffer);
	void GetADValue();
	int DTOS_Test();
	void KillDataBuffer();
	BOOL bCloseCamera();

	BOOL CloseAAVision();//�õ���

	BOOL bOpenDevice();

	int EnumerateDothinkeyDevice();
	BOOL	m_isTV;

	USHORT*    m_pSensorPara;
	USHORT*    m_pSensorSleepPara;

	//20130823 added..
	USHORT*		m_pAF_InitParaList;
	USHORT*		m_pAF_AutoParaList;
	USHORT*		m_pAF_FarParaList;
	USHORT*		m_pAF_NearParaList;
	USHORT*		m_pExposure_ParaList;
	USHORT*		m_pGain_ParaList;

///////////////////////////////////////////////////////////////////////
	BYTE m_RunMode;
	USHORT m_PreviewWidth;
	USHORT m_PreviewHeight;
	LPBYTE     m_pTripleBuffer;  //buffer between display and camera 
 	LPBYTE     m_pDisplayBuffer; //display buffer
	LPBYTE     m_pCameraBuffer;  //camera buffer.
    HANDLE    m_hCameraThreadEventDead; //thread for video 
    HANDLE    m_hDisplayThreadEventDead;//thread for display
	BOOL      m_bTripleBufferDirty;     //video update flag

	ULONG m_GrabSize;
	

	CRITICAL_SECTION m_csLock;                //use for display update data

	CMyStatic m_wndVideo[AA_MAXVISION];
	SensorTab CurrentSensor;   ///��Ϊ�����Ӿ�ͨ������תվ

	int m_nVisionDevID;
	int m_roi_state;

///////////////////////////////////////////////////////////////////////
	CHSFoxxDlg(CWnd* pParent = NULL);	// standard constructor
	~CHSFoxxDlg();

	enum { IDD = IDD_HSFOXX_DIALOG };

	int m_nPiDevID;//��ǰ������id

public:
	virtual BOOL DestroyWindow();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnPlay();
	afx_msg void OnLoad();
	afx_msg void OnOseasy();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editMsg;
	afx_msg void OnBnClickedSavepicture();
	LPBYTE AAVisionGrabPicture(int visionID);
	BOOL bSaveBmpFile(CString sfilename, BYTE* pBuffer, UINT width, UINT height);
	float m_fAvdd;
	float m_fDovdd;
	float m_fDvdd;
	float m_fAfvcc;
	float m_fVpp;
	float m_fMclk;
	UINT m_FocusPos;
	UINT m_FocusPosTemp;
	afx_msg void OnBnClickedSetpower();
	afx_msg void OnBnClickedSetmclk();
	afx_msg void OnBnClickedTestRoi();

	afx_msg void OnBnClickedEnum();
	afx_msg void OnBnClickedFocusPos();

	afx_msg void OnBnClickedStartaa();

	int StartAA(int robotid,int visonid);
	int StartDoubleAA(int robotid,int visonidfixID,int visonidmovID);
	int StartDoubleAAByOpenCV(int robotid,int visonidfixID,int visonidmovID);

	AANotifyResult  NewStartDoubleAA(int vaid,int vbid);


	int GoInitPosRobot(int robotid,const char* axisstring,int posIndex,double* pos = NULL);
	
	

	void SetInitPosRobot(int robotid,const char* axisstring,int posIndex,double* pos );

	int AdjustZvalue(int robotID,int cameraID);
	int AdjustUVvalue(int robotID,int cameraID);
	int AdjustUVvalue2(int robotID,int cameraID);
	int FiveAdjustvalue(int robotID,int cameraID);
	int AdjustXYvalue(int robotID,int cameraID);

	//���漸�����Ƕ�ͼ����,���Ժϲ�Ϊһ��
	int AdjustZvalueReadPic(int robotID,int cameraID);
	int AdjustUVvalueReadPic(int robotID,int cameraID);
	int FiveAdjustvalueReadPic(int robotID,int cameraID);
	int AdjustXYvalueReadPic(int robotID,int cameraID);

	//˫���ͼ����
	int DobuleAATestReadPic(int robotID,int cameraID);
	//ͼƬ�ߴ�
	int pictureCols;
	int pictureRows;
	LPBYTE GetPicture(int caramID);
	BOOL GetPictureMat(Mat * PictureMat,int caramID);
	BOOL GetPictureHObject(HObject &PictureHobject,int caramID);
	BOOL ReadPictureMat(Mat * PictureMat,string filename, int caramID);//�ȱ���caramID,��������ɼ���ͼ�񶼲�һ��
	BOOL SavePicture(CString sfilename,int cameraID);

	map<string ,CAARobot*>mapRobot;//ip �ͻ�����,��public���map���������� get set
	//����������,���ܶ��̲߳���map
	map<string ,CAAVision*>mapVision; //���ָ��Ӿ�����
	map<string ,CAAVision*>& GetVisionMap();
	static CHSFoxxDlg* GetInstance();
	CComboBox m_KitListCtrl;
	CComboBox m_PiList;
	CString m_DevName[AA_MAXVISION];
	CButton m_playvision[AA_MAXVISION];

	CButton m_spincenter;

	//
	int GetSensorRegValue(int visionID,int type);//��ȡֵ
	int SetSensorRegValue(int visionID,int type,USHORT value);//����

	CAAVision* FindVisionByID(int visionID);
	CAAVision* FindVisionInMap(string username); //������Ӿ��Ƿ���map�У��ȿ���һ��ʼ�Ͳ��usb�ӿ�,����;���
	string FindVisionUsernameByID(int visionID);
	int FindBinderIDByID(int visionID);//ͨ��id�ҵ�˫��󶨵�����һ���Ӿ�id

	//8��ͼ��ÿ��ͼƬ����5��ROI�������������
	Point coordinate[AA_MAXVISION][AA_VISION_RETAGLELOC];
	//��ȡһ��ָ��ͨ���е�ROI���ε������������������,ÿ��ͨ��5������
	void CalcCenterPoint(int vaid, int vbid, HObject &pirtureA, HObject &pictureB, int graymin,int graymax,int areamin,int areamax);

	//������
	CAARobot* FindRobotByID(int robotID);
	int AdjustSpinCenter(int robotID,int whichaxis,double value);

	afx_msg void OnClickedGoHome();
	
	afx_msg void OnEnChangeInitY();
	
	afx_msg void OnBnClickedEnumnetdevice();
	afx_msg void OnBnClickedConnect();
	afx_msg void OnBnClickedLongdis();
	afx_msg void OnBnClickedMiddis();
	afx_msg void OnBnClickedShortdis();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedCurposition();
	afx_msg LRESULT OnDisplayMsg(WPARAM wP, LPARAM lP);

	afx_msg void OnBnClickedBtnAaparam();
	afx_msg void OnCbnSelchangePitlist();
	int ConnectPIRobot(string &str);
	int IsconnectedInMapRobot(string ip, int port);//Ѹ���һ������Ƿ�����,������� 1,�Ѿ����ӣ�0,û�����ӣ�-1,û����map��
	int FindRobotIDByIp(string ip);
	int FindRobotIndexByID(int id);
	afx_msg void OnBnClickedPlayVision0();
	afx_msg void OnBnClickedPlayVision1();
	afx_msg void OnBnClickedPlayVision2();
	afx_msg void OnBnClickedPlayVision3();
	afx_msg void OnBnClickedPlayVision4();
	afx_msg void OnBnClickedPlayVision5();
	afx_msg void OnBnClickedPlayVision6();
	afx_msg void OnBnClickedPlayVision7();

	int OpenAAVision(string visionName,CButton &bt);
	int __OpenAAVision(string visionName,CButton &bt);
	
	
	afx_msg void OnStnClickedVideo0();
	afx_msg void OnStnClickedVideo1();
	afx_msg void OnStnClickedVideo2();
	afx_msg void OnStnClickedVideo3();
	afx_msg void OnStnClickedVideo4();
	afx_msg void OnStnClickedVideo5();
	afx_msg void OnStnClickedVideo6();
	afx_msg void OnStnClickedVideo7();
	afx_msg void OnBnClickedConnectclose();
	
	afx_msg void OnBnClickedPicturecenter();
	afx_msg void OnBnClickedReadpicTest();
	afx_msg void OnBnClickedPiHome();
	afx_msg void OnBnClickedSetinitpos();
	afx_msg void OnBnClickedPiHome1();
	afx_msg void OnBnClickedSetinitpos1();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton1();
	CSliderCtrl m_grain;
	CSliderCtrl m_exp;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedSpinCenter();

	afx_msg void OnThemechangedVideo0(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedDrawcenterpoint();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HSFOXXDLG_H__045BC92C_F0AA_438B_B102_C789FAC4F43D__INCLUDED_)
