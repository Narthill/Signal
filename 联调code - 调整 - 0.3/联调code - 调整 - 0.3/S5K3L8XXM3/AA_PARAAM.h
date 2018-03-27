#pragma once
#include "afxwin.h"


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
	virtual BOOL OnInitDialog();
	void DisplayAAglobleParam2EDit();
	void SaveAAglobleParam(bool save =0);
private:	
	CEdit m_editzadjustnum;
	CEdit m_editz_adjustthr;
	CEdit   m_edtzadjustVal[10];  

	CEdit m_edituvadjustnum;
	CEdit m_edituv_adjustthr;
	CEdit m_edtuvadjustVal[10];

	//ģ������
	CEdit m_edttemplateType[8];

public:

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedAatesttype();
	CButton m_AAtesttype;
	afx_msg void OnBnClickedSigtype();
	afx_msg void OnBnClickedDoutype();
};
