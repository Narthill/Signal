// MyStatic.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HSFoxx.h"
#include "MyStatic.h"

#include"AA_define_tool.h"
extern  AA_Globle_Param g_AAParam;//����ȫ�ֱ���
extern bool g_debugwindow;

// CMyStatic

IMPLEMENT_DYNAMIC(CMyStatic, CStatic)

CMyStatic::CMyStatic()
{
	m_windsizetype = false;
}

CMyStatic::~CMyStatic()
{
}


BEGIN_MESSAGE_MAP(CMyStatic, CStatic)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CMyStatic ��Ϣ�������




void CMyStatic::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect tempRect1;
	double curposition_x;
	double curposition_y;
	//tempPicture1->GetWindowRect(&tempRect1);
	//CWnd::GetWindowRect�õ������൱��Ļ�ľ������겻����Կͻ����ģ�Ҫת��һ��
	/*ScreenToClient(&tempRect1);*/
	//tempPicture2->GetWindowRect(&tempRect2);
	//ͼƬ�ؼ��ĳ��Ϳ�
	GetWindowRect(tempRect1);
	/*::GetClientRect(GetSafeHwnd(), &tempRect1);  */ 
	int hight = tempRect1.bottom - tempRect1.top;
	int wight = tempRect1.right - tempRect1.left;
	////Ȼ��ѵ�ǰ�������תΪ�����rect������  
	/*::ScreenToClient(GetSafeHwnd(), &point);*/ 
	double pw = g_AAParam.pic_lenth *1.0;	
	double ph = g_AAParam.pic_width *1.0;	
	pw = pw/wight;
	curposition_x = pw * point.x;
	ph = ph/hight;
	curposition_y= ph * point.y;

	CString str;
	str.Format("����Ϊ:(%.1f,%.1f)", curposition_y,curposition_x);
	if (false == g_debugwindow)
	{
		GetParent()->GetDlgItem(IDC_PICVALUE)->SetWindowText(str);	
	}
	else
	{
		GetParent()->GetDlgItem(IDC_DEBUG_PICVALUE)->SetWindowText(str);	
	}
	
	UpdateData(false); 
	CStatic::OnMouseMove(nFlags, point);
}
