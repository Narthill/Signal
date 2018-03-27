#pragma once
#include "afxcmn.h"


// AA_PICPARAM �Ի���

class AA_PICPARAM : public CDialogEx
{
	DECLARE_DYNAMIC(AA_PICPARAM)

public:
	AA_PICPARAM(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~AA_PICPARAM();

// �Ի�������
	enum { IDD = IDD_DLG_AA_PICPARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	//afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangePicchannel();
	void UpdateCombProcessParam();

	void InitCombChannel();
	void InitPicType();
	afx_msg void OnCbnSelchangePicprocesstype();

	//��һ���б����CEDIT������
	#define MOVEDATAEDITID  10001
	CListCtrl m_L_INPUT;
	int m_SelMoveRow;
	int m_SelMoveCol;
	CEdit* pCommonEdit;

	void InitInputList();
	afx_msg void OnNMDblclkListParam(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnOK();
	afx_msg void OnBnClickedSavepicparam();

	int bindOpencvWindow();
};
