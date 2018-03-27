#pragma once
#include "afxwin.h"

#include "MyStatic.h"
#include "HalconCpp.h"
using namespace HalconCpp;
#include <vector>
using namespace std;;

#include "IAVisionAlgorithm.h"

// CAA_PARAAM �Ի���

class CAA_PARAAM : public CDialogEx
{
	DECLARE_DYNAMIC(CAA_PARAAM)

public:
	CAA_PARAAM(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAA_PARAAM();

// �Ի�������
	enum { IDD = IDD_DLG_AA_PARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int bindOpencvWindow();
	virtual BOOL OnInitDialog();
	void DisplayAAglobleParam2EDit();
	void SaveAAglobleParam(bool save =0);

	void SaveAARegionParam(bool save =0);

private:	
	CEdit m_editzadjustnum;
	CEdit m_editz_adjustthr;
	CEdit   m_edtzadjustVal[10];  

	CEdit m_edituvadjustnum;
	CEdit m_edituv_adjustthr;
	CEdit m_edtuvadjustVal[10];

	//ģ������
	CEdit m_edttemplateType[8];

	CEdit m_edtpaintcenter[8];

public:

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedAatesttype();
	CButton m_AAtesttype;
	afx_msg void OnBnClickedSigtype();
	afx_msg void OnBnClickedDoutype();

	//����ѡ�����
	afx_msg void OnCbnSelchangeChannel();
	afx_msg void OnCbnSelchangeRegionallocation();
	afx_msg void OnCbnSelchangeRegionaltype();

protected:
	//�����ɹ̶�8ͨ���ģ�������Ϊ����ʵ�������
	void InitCombChannel();
	void InitCombRegionallocation();
	void InitCombRegionaltype();

	void InitCombFunSelect();

	void UpdateCombRegional();
public:
	CMyStatic m_debugvideo;
	afx_msg void OnBnClickedGrabpic();
	long halconWinID ;//halcon����
	HObject m_pImage[8];

	char m_OpencvWinstr[32];
	Mat m_opImage[8];
	int RegionLocation;//��¼��һ�ε�ROI����λ��
	int ImageChannel;       //��¼��һ��ѡ���ͨ�����
	int RegionType;   //��¼��һ��ѡ���ROI��������
	int Calcpoint();
	int CalcRegionArea(vector<double>&area);
	int CalcHalconProcess(int vida,int vidb);

	int CalcRegionMTF();
	int CalcRegionRightHarryPoint();

	afx_msg void OnBnClickedReadpic1();
	afx_msg void OnCbnSelchangeFunselet();
	afx_msg void OnBnClickedCheckHalcon();
	afx_msg void OnBnClickedBtnLoadhal();
	afx_msg void OnStnDblclickDebugVideo();
};
