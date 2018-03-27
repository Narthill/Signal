// AA_PARAAM.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HSFoxx.h"
#include "AA_PARAAM.h"
#include "afxdialogex.h"

#include"AA_define_tool.h"
#include "Config.h"

#include "HSFoxxDlg.h"//Ϊ�˱������ź�ͨ��

#include "IAVisionAlgorithm.h"

#include "AA_PICPARAM.h"

#include "AA_pic_param.h"



extern  AA_Globle_Param g_AAParam;//����ȫ�ֱ���
extern string g_reginalchannel[];
extern string g_reginallocation[];
extern string g_reginaltype[];
extern string g_funselect[];
extern bool g_debugwindow;

extern AA_Globle_Pic_Param g_AA_PicParam;
// CAA_PARAAM �Ի���

IMPLEMENT_DYNAMIC(CAA_PARAAM, CDialogEx)

CAA_PARAAM::CAA_PARAAM(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAA_PARAAM::IDD, pParent)
{
	for (int i = 0;i<sizeof(m_pImage)/sizeof(HObject);i++)
	{
		m_pImage[i].Clear();
	}
}

CAA_PARAAM::~CAA_PARAAM()
{
	//cvDestroyWindow(m_OpencvWinstr);
	HalconCpp::CloseWindow(halconWinID);
	g_debugwindow = false;

}

void CAA_PARAAM::DoDataExchange(CDataExchange* pDX)
{
	//Zֵ	
	DDX_Control(pDX, IDC_EDIT_ZADJUST, m_editzadjustnum);
	DDX_Control(pDX, IDC_EDIT_THRO, m_editz_adjustthr);
	DDX_Control(pDX, IDC_EDIT0_Z, m_edtzadjustVal[0]);
	DDX_Control(pDX, IDC_EDIT1_Z, m_edtzadjustVal[1]);
	DDX_Control(pDX, IDC_EDIT2_Z, m_edtzadjustVal[2]);
	DDX_Control(pDX, IDC_EDIT3_Z, m_edtzadjustVal[3]);
	DDX_Control(pDX, IDC_EDIT4_Z, m_edtzadjustVal[4]);
	DDX_Control(pDX, IDC_EDIT5_Z, m_edtzadjustVal[5]);
	DDX_Control(pDX, IDC_EDIT6_Z, m_edtzadjustVal[6]);
	DDX_Control(pDX, IDC_EDIT7_Z, m_edtzadjustVal[7]);
	DDX_Control(pDX, IDC_EDIT8_Z, m_edtzadjustVal[8]);
	DDX_Control(pDX, IDC_EDIT9_Z, m_edtzadjustVal[9]);

	//UV
	DDX_Control(pDX, IDC_EDIT_UVADJUST, m_edituvadjustnum);
	DDX_Control(pDX, IDC_EDIT_UVTHRO, m_edituv_adjustthr);
	DDX_Control(pDX, IDC_EDIT0_UV, m_edtuvadjustVal[0]);
	DDX_Control(pDX, IDC_EDIT1_UV, m_edtuvadjustVal[1]);
	DDX_Control(pDX, IDC_EDIT2_UV, m_edtuvadjustVal[2]);
	DDX_Control(pDX, IDC_EDIT3_UV, m_edtuvadjustVal[3]);
	DDX_Control(pDX, IDC_EDIT4_UV, m_edtuvadjustVal[4]);
	DDX_Control(pDX, IDC_EDIT5_UV, m_edtuvadjustVal[5]);
	DDX_Control(pDX, IDC_EDIT6_UV, m_edtuvadjustVal[6]);
	DDX_Control(pDX, IDC_EDIT7_UV, m_edtuvadjustVal[7]);
	DDX_Control(pDX, IDC_EDIT8_UV, m_edtuvadjustVal[8]);
	DDX_Control(pDX, IDC_EDIT9_UV, m_edtuvadjustVal[9]);

	//ģ������
	DDX_Control(pDX, IDC_EDIT_CHANNEL1, m_edttemplateType[0]);
	DDX_Control(pDX, IDC_EDIT_CHANNEL2, m_edttemplateType[1]);
	DDX_Control(pDX, IDC_EDIT_CHANNEL3, m_edttemplateType[2]);
	DDX_Control(pDX, IDC_EDIT_CHANNEL4, m_edttemplateType[3]);
	DDX_Control(pDX, IDC_EDIT_CHANNEL5, m_edttemplateType[4]);
	DDX_Control(pDX, IDC_EDIT_CHANNEL6, m_edttemplateType[5]);
	DDX_Control(pDX, IDC_EDIT_CHANNEL7, m_edttemplateType[6]);
	DDX_Control(pDX, IDC_EDIT_CHANNEL8, m_edttemplateType[7]);


	//������

	DDX_Control(pDX, IDC_EDIT_PAINTCENT1, m_edtpaintcenter[0]);
	DDX_Control(pDX, IDC_EDIT_PAINTCENT2, m_edtpaintcenter[1]);
	DDX_Control(pDX, IDC_EDIT_PAINTCENT3, m_edtpaintcenter[2]);
	DDX_Control(pDX, IDC_EDIT_PAINTCENT4, m_edtpaintcenter[3]);
	DDX_Control(pDX, IDC_EDIT_PAINTCENT5, m_edtpaintcenter[4]);
	DDX_Control(pDX, IDC_EDIT_PAINTCENT6, m_edtpaintcenter[5]);
	DDX_Control(pDX, IDC_EDIT_PAINTCENT7, m_edtpaintcenter[6]);
	DDX_Control(pDX, IDC_EDIT_PAINTCENT8, m_edtpaintcenter[7]);

	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_DEBUG_VIDEO, m_debugvideo);
}


BEGIN_MESSAGE_MAP(CAA_PARAAM, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAA_PARAAM::OnBnClickedOk)
	
	ON_BN_CLICKED(IDC_SIGTYPE, &CAA_PARAAM::OnBnClickedSigtype)
	ON_BN_CLICKED(IDC_DOUTYPE, &CAA_PARAAM::OnBnClickedDoutype)
	ON_CBN_SELCHANGE(IDC_CHANNEL, &CAA_PARAAM::OnCbnSelchangeChannel)
	ON_CBN_SELCHANGE(IDC_REGIONALLOCATION, &CAA_PARAAM::OnCbnSelchangeRegionallocation)
	ON_CBN_SELCHANGE(IDC_REGIONALTYPE, &CAA_PARAAM::OnCbnSelchangeRegionaltype)
	ON_BN_CLICKED(IDC_GRABPIC, &CAA_PARAAM::OnBnClickedGrabpic)

	ON_BN_CLICKED(IDC_READPIC1, &CAA_PARAAM::OnBnClickedReadpic1)
	ON_CBN_SELCHANGE(IDC_FUNSELET, &CAA_PARAAM::OnCbnSelchangeFunselet)
	ON_BN_CLICKED(IDC_CHECK_HALCON, &CAA_PARAAM::OnBnClickedCheckHalcon)
	ON_BN_CLICKED(IDC_BTN_LOADHAL, &CAA_PARAAM::OnBnClickedBtnLoadhal)
	ON_STN_DBLCLK(IDC_DEBUG_VIDEO, &CAA_PARAAM::OnStnDblclickDebugVideo)
END_MESSAGE_MAP()


// CAA_PARAAM ��Ϣ�������

int CAA_PARAAM::bindOpencvWindow()
{
	int ret = 0;

	CRect rt;
	m_debugvideo.GetClientRect(&rt);

	namedWindow(m_OpencvWinstr,WINDOW_AUTOSIZE );//���ô�����  
	HWND hWndl=(HWND )cvGetWindowHandle (m_OpencvWinstr);//hWnd ��ʾ���ھ��,��ȡ���ھ��
	HWND hParent1=::GetParent (hWndl );//GetParent����һ��ָ���Ӵ��ڵĸ����ھ��
	::SetParent (hWndl, m_debugvideo.m_hWnd );  //��mfc������Ϊopencv�ĸ���
	::ShowWindow (hParent1 ,SW_HIDE );//ShowWindowָ����������ʾ


	return ret ;
}
BOOL CAA_PARAAM::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	strcpy_s(m_OpencvWinstr,"OPencvWindow");
	//��ʼ��zֵ
	CString str;
	int  adjustnum =0;
	double adjust_thr =0;
	double adjustvalue[10]={0};
	adjustnum = CFG_INSTANCE->get_cfg_int(AA_Adjustparam,AAadjust_zMaxNum);
	adjust_thr = CFG_INSTANCE->get_cfg_double(AA_Adjustparam,AAadjust_zthr);
	ReadAAAdjustZValue(adjustvalue,adjustnum);
	str.Format("%d", adjustnum );
	m_editzadjustnum.SetWindowText(str);
	str.Format("%.6lf", adjust_thr );
	m_editz_adjustthr.SetWindowText(str);

	for(int i=0;i<adjustnum;i++)
	{	
		str.Format("%.6lf", adjustvalue[i] );
		m_edtzadjustVal[i].SetWindowText(str);
	}

	//��ʼ��uvֵ
	adjustnum = CFG_INSTANCE->get_cfg_int(AA_Adjustparam,AAadjust_uvMaxNum);
	adjust_thr = CFG_INSTANCE->get_cfg_double(AA_Adjustparam,AAadjust_uvthr);
	ReadAAAdjustUVValue(adjustvalue,adjustnum);
	str.Format("%d", adjustnum );
	m_edituvadjustnum.SetWindowText(str);
	str.Format("%.6lf", adjust_thr );
	m_edituv_adjustthr.SetWindowText(str);
	for(int i=0;i<adjustnum;i++)
	{	
		str.Format("%.6lf", adjustvalue[i] );
		m_edtuvadjustVal[i].SetWindowText(str);
	}

	//��ʾ����ѡ��������
	InitCombChannel();
	InitCombRegionallocation();
	InitCombRegionaltype();

	//����ѡ��
	InitCombFunSelect();

	//��ʾȫ�ֱ�����ֵ
	DisplayAAglobleParam2EDit();


	//�󶨴���
	//���Ű���opencv�Ĵ���
	//bindOpencvWindow();

	CRect rt;
	m_debugvideo.GetClientRect(&rt);
	HTuple HalWinId;
	OpenWindow(0,0,rt.right-rt.left,rt.bottom - rt.top, (long)m_debugvideo.m_hWnd, "visible", "", &HalWinId);
	halconWinID = (long)HalWinId.L();
	//SetPart(halconWinID,0,0,g_AAParam.pic_width,g_AAParam.pic_lenth);//��ͼ���Сӳ�䵽ͼƬ��

	//SetPart(halconWinID,0,0,g_AAParam.pic_width*g_AAParam.scale,g_AAParam.pic_lenth*g_AAParam.scale);//��С������ʾ

	//SetPart(halconWinID,-1,-1,-1,-1);//����ʱͼ���һ����

	//SetPart(halconWinID,0,0,-1,-1);//����ʱͼ���һ����



	
	g_debugwindow = true;

	str.Format("%s", g_AAParam.halconprocess.c_str() );
	GetDlgItem(IDC_EDIT_HALPROCESS)->SetWindowTextA(str);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}



void CAA_PARAAM::OnBnClickedOk()
{
	// ����z
	CString str;
	int adjustnum =0;
	m_editzadjustnum.GetWindowText(str);
	adjustnum = atoi(str.GetBuffer());
	CFG_INSTANCE->modify_cfg_int(AA_Adjustparam,AAadjust_zMaxNum,adjustnum);
	double adjustthr =0;
	m_editz_adjustthr.GetWindowText(str);
	 adjustthr = atof(str.GetBuffer());
	CFG_INSTANCE->modify_cfg_double(AA_Adjustparam,AAadjust_zthr,adjustthr);
	double adjustvalue[AA_zzMaxnum] ={0.0};
	for(int i=0;i<AA_zzMaxnum;i++)
	{
		m_edtzadjustVal[i].GetWindowText(str);
		adjustvalue[i] = atof(str.GetBuffer());

	}
	SaveAAAdjustZValue(adjustvalue,adjustnum);

	//uv
	m_edituvadjustnum.GetWindowText(str);
	adjustnum = atoi(str.GetBuffer());
	CFG_INSTANCE->modify_cfg_int(AA_Adjustparam,AAadjust_uvMaxNum,adjustnum);
	m_edituv_adjustthr.GetWindowText(str);
	adjustthr = atof(str.GetBuffer());
	CFG_INSTANCE->modify_cfg_double(AA_Adjustparam,AAadjust_uvthr,adjustthr);
	for(int i=0;i<AA_uvMaxnum;i++)
	{
		m_edtuvadjustVal[i].GetWindowText(str);
		adjustvalue[i] = atof(str.GetBuffer());
	}
	SaveAAAdjustUVValue(adjustvalue,adjustnum);

	//����ȫ�ֲ���
	SaveAAglobleParam();

	//CDialogEx::OnOK();
}
void CAA_PARAAM::DisplayAAglobleParam2EDit()
{
	CString str;
	str.Format("%.6lf", g_AAParam.focus);
	GetDlgItem(IDC_EDIT_focus)->SetWindowText(str);
	str.Format("%.6lf", g_AAParam.objdis);
	GetDlgItem(IDC_EDIT_objdis)->SetWindowText(str);
	str.Format("%.6lf", g_AAParam.pixel_size);
	GetDlgItem(IDC_EDIT_pixel_size)->SetWindowText(str);
	str.Format("%.6lf", g_AAParam.scale);
	GetDlgItem(IDC_EDIT_scale)->SetWindowText(str);
	str.Format("%d", g_AAParam.mapsize);
	GetDlgItem(IDC_EDIT_mapsize)->SetWindowText(str);
	str.Format("%d", g_AAParam.pic_lenth);
	GetDlgItem(IDC_EDIT_pic_length)->SetWindowText(str);
	str.Format("%d", g_AAParam.pic_width);
	GetDlgItem(IDC_EDIT_pic_height)->SetWindowText(str);
	str.Format("%.6lf", g_AAParam.mapdislength);
	GetDlgItem(IDC_EDIT_mapdislength)->SetWindowText(str);
	str.Format("%.6lf", g_AAParam.mapdiswidth);
	GetDlgItem(IDC_EDIT_mapdiswidth)->SetWindowText(str);
	str.Format("%d", g_AAParam.size);
	GetDlgItem(IDC_EDIT_size)->SetWindowText(str);
	str.Format("%d", g_AAParam.block);
	GetDlgItem(IDC_EDIT_block)->SetWindowText(str);
	str.Format("%d", g_AAParam.edge_size);
	GetDlgItem(IDC_EDIT_edgesize)->SetWindowText(str);
	str.Format("%d", g_AAParam.block_size);
	GetDlgItem(IDC_EDIT_blocksize)->SetWindowText(str);

	

	for(int i=0;i<AA_MAXVISION;i++)
	{
		//��ʾģ������
		str.Format("%d", g_AAParam.temPlate[i]);
		m_edttemplateType[i].SetWindowText(str);
		//��ʾ������
		str.Format("%.1f,%.1f", g_AAParam.paintCenter[i].X,g_AAParam.paintCenter[i].Y);
		m_edtpaintcenter[i].SetWindowText(str);

	}
	//˵��:
	str = "����ģ��:(ԭ��:ѡ�������Ϊ��0,���ϵ�(�ϰ��º�,����Һ�),1,���ϵ�(�ϰ��º�,����Ұ�) 2,���µ�(�Ϻ��°�,����Һ�) 3,���µ�(�Ϻ��°�,����Һ�)";
	GetDlgItem(IDC_BEIZHU)->SetWindowText(str);
	
	




	//aa����ѡ��
	if (SINGLEAA == g_AAParam.AATYPE)
	{
		
		((CButton*)GetDlgItem(IDC_SIGTYPE))->SetCheck(TRUE); 
	}
	else
	{
		((CButton*)GetDlgItem(IDC_DOUTYPE))->SetCheck(TRUE); 
	}
	
	//˫�����
	str.Format("%.6lf", g_AAParam.doubleAAdis);
	GetDlgItem(IDC_EDIT_DOUBLEDIS)->SetWindowText(str);
	str.Format("%.6lf", g_AAParam.doubleAAscale);
	GetDlgItem(IDC_EDIT_DOUBLESCALE)->SetWindowText(str);

	str.Format("%.6lf", g_AAParam.doubleAAmapdislength);
	GetDlgItem(IDC_EDIT_DOUBLEDISLENGTH)->SetWindowText(str);
	str.Format("%.6lf", g_AAParam.doubleAAmapdiswidth);
	GetDlgItem(IDC_EDIT_DOUBLEDISWIHTH)->SetWindowText(str);
	str.Format("%.6lf", g_AAParam.doubleAAsize);
	GetDlgItem(IDC_EDIT_DOUBLESIZE)->SetWindowText(str);
	str.Format("%.6lf", g_AAParam.doubleAApiccenterX);
	GetDlgItem(IDC_EDIT_DOUBLECENTERX)->SetWindowText(str);
	str.Format("%.6lf", g_AAParam.doubleAApiccenterY);
	GetDlgItem(IDC_EDIT_DOUBLECENTERY)->SetWindowText(str);
	str.Format("%.6lf", g_AAParam.doubleAACentroidthreshold);
	GetDlgItem(IDC_EDIT_DOUBLECENTHRES)->SetWindowText(str);

	//��ʾѡ������ĵ�λ������ξ�Ҫ��ʾ���ε����ϵ�����µ�
	//��ȡѡ���ͨ��������λ�ã��������ͣ���ʾ��ȷ�ĵ�λ
	UpdateCombRegional();


}
void CAA_PARAAM::SaveAARegionParam(bool save)
{
	//����ѡ��������λ�����ı���ȫ�ֲ�����ȫ�ֲ����������ڴ���SaveAA_Globle_ParamValue���棬�ڴ浽�ļ���������ʱ�򱣴�
	//�Ȳ鿴������������ݣ�ͨ��������λ�ã���������
	int nChannelCur = ((CComboBox*)GetDlgItem(IDC_CHANNEL))->GetCurSel();
	int nRegionLocationCur = ((CComboBox*)GetDlgItem(IDC_REGIONALLOCATION))->GetCurSel();
	int nRegionTypeCur = ((CComboBox*)GetDlgItem(IDC_REGIONALTYPE))->GetCurSel();
	if (nChannelCur>=0 && nRegionLocationCur>=0 )
	{
		//((CComboBox*)GetDlgItem(IDC_CHANNEL))->GetLBText(nCur,str);
		//�Ȱ��չ̶���8��ͨ����,0��ʶ1ͨ����1��ʶ2ͨ��
		CString strpx1,strpx2,strpy1,strpy2;		
		GetDlgItem(IDC_EDIT_POINTX1)->GetWindowText(strpx1);
		GetDlgItem(IDC_EDIT_POINTX2)->GetWindowText(strpx2);
		GetDlgItem(IDC_EDIT_POINTY1)->GetWindowText(strpy1);
		GetDlgItem(IDC_EDIT_POINTY2)->GetWindowText(strpy2);
		switch(nRegionTypeCur)
		{
		case AATYPE_RETANGLE1:
			{

				g_AAParam.channel1Retangle[nChannelCur][nRegionLocationCur].ltx = atof(strpx1.GetBuffer());
				g_AAParam.channel1Retangle[nChannelCur][nRegionLocationCur].lty = atof(strpy1.GetBuffer());
				g_AAParam.channel1Retangle[nChannelCur][nRegionLocationCur].rbx = atof(strpx2.GetBuffer());
				g_AAParam.channel1Retangle[nChannelCur][nRegionLocationCur].rby = atof(strpy2.GetBuffer());
				//��ȫ�ֲ����������ڴ�
				double r[4*AA_VISION_RETAGLELOC] = {0};
				/*int i=0;*/
				for (int loc=0;loc<AA_VISION_RETAGLELOC;loc++)
				{
					r[4*loc] = g_AAParam.channel1Retangle[nChannelCur][loc].ltx;
					r[4*loc+1] =g_AAParam.channel1Retangle[nChannelCur][loc].lty ;
					r[4*loc+2] = g_AAParam.channel1Retangle[nChannelCur][loc].rbx ;
					r[4*loc+3] = g_AAParam.channel1Retangle[nChannelCur][loc].rby ;
					/*i++;*/
				}
				SaveAA_Reginal_ParamValue(r,4*AA_VISION_RETAGLELOC,nChannelCur+1,nRegionTypeCur,save);

			}
			break;
		default:
			break;
		}
	}
}
void CAA_PARAAM::SaveAAglobleParam(bool save)
{
	CString str;
	GetDlgItem(IDC_EDIT_focus)->GetWindowText(str);
	g_AAParam.focus = atof(str.GetBuffer());

	GetDlgItem(IDC_EDIT_objdis)->GetWindowText(str);
	g_AAParam.objdis = atof(str.GetBuffer());

	GetDlgItem(IDC_EDIT_pixel_size)->GetWindowText(str);
	g_AAParam.pixel_size = atof(str.GetBuffer());

	GetDlgItem(IDC_EDIT_scale)->GetWindowText(str);
	g_AAParam.scale = atof(str.GetBuffer());

	GetDlgItem(IDC_EDIT_mapsize)->GetWindowText(str);
	g_AAParam.mapsize= atoi(str.GetBuffer());

    GetDlgItem(IDC_EDIT_pic_length)->GetWindowText(str);
	g_AAParam.pic_lenth = atoi(str.GetBuffer());
	
	
	GetDlgItem(IDC_EDIT_pic_height)->GetWindowText(str);
	g_AAParam.pic_width = atoi(str.GetBuffer());
	g_AAParam.mapdiswidth = atof(str.GetBuffer());

	
	GetDlgItem(IDC_EDIT_mapdislength)->GetWindowText(str);
	g_AAParam.mapdislength = atof(str.GetBuffer());
	
	GetDlgItem(IDC_EDIT_mapdiswidth)->GetWindowText(str);
	g_AAParam.mapdiswidth = atof(str.GetBuffer());

	
	GetDlgItem(IDC_EDIT_size)->GetWindowText(str);
	g_AAParam.size = atoi(str.GetBuffer());
	

	GetDlgItem(IDC_EDIT_block)->GetWindowText(str);
	g_AAParam.block = atoi(str.GetBuffer());
	
	GetDlgItem(IDC_EDIT_edgesize)->GetWindowText(str);
	g_AAParam.edge_size = atoi(str.GetBuffer());
	
	GetDlgItem(IDC_EDIT_blocksize)->GetWindowText(str);
    g_AAParam.block_size = atoi(str.GetBuffer());


	int index =0;
	
	for(int i=0;i<AA_MAXVISION;i++)
	{
		m_edttemplateType[i].GetWindowText(str);
		g_AAParam.temPlate[i] = atoi(str.GetBuffer());


		m_edtpaintcenter[i].GetWindowText(str);
		index = str.Find(',');
		if (index != -1)
		{		
			{
				CString temp = str.Mid(0,index);
				g_AAParam.paintCenter[i].X = atof(temp.GetBuffer());
				temp = str.Mid(index+1);
				g_AAParam.paintCenter[i].Y = atof(temp.GetBuffer());

			}
		}
	}




	//˫��
	GetDlgItem(IDC_EDIT_DOUBLESCALE)->GetWindowText(str);
	g_AAParam.doubleAAscale = atof(str.GetBuffer());
	GetDlgItem(IDC_EDIT_DOUBLEDIS)->GetWindowText(str);
	g_AAParam.doubleAAdis = atof(str.GetBuffer());

	GetDlgItem(IDC_EDIT_DOUBLEDISLENGTH)->GetWindowText(str);
	g_AAParam.doubleAAmapdislength = atof(str.GetBuffer());
	GetDlgItem(IDC_EDIT_DOUBLEDISWIHTH)->GetWindowText(str);
	g_AAParam.doubleAAmapdiswidth = atof(str.GetBuffer());
	GetDlgItem(IDC_EDIT_DOUBLESIZE)->GetWindowText(str);
	g_AAParam.doubleAAsize = atof(str.GetBuffer());
	GetDlgItem(IDC_EDIT_DOUBLECENTERX)->GetWindowText(str);
	g_AAParam.doubleAApiccenterX = atof(str.GetBuffer());
	GetDlgItem(IDC_EDIT_DOUBLECENTERY)->GetWindowText(str);
	g_AAParam.doubleAApiccenterY = atof(str.GetBuffer());

	GetDlgItem(IDC_EDIT_DOUBLECENTHRES)->GetWindowText(str);
	g_AAParam.doubleAACentroidthreshold = atof(str.GetBuffer());
	

	//�����������
	SaveAARegionParam(save);


	//����ȫ�ֺ����������
	SaveAA_Globle_ParamValue(true);//���浽hash���ﻹҪ�������浽�ļ�

}




void CAA_PARAAM::OnBnClickedSigtype()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_AAParam.AATYPE = SINGLEAA;
}


void CAA_PARAAM::OnBnClickedDoutype()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_AAParam.AATYPE = DOUBLEAA;
}


void CAA_PARAAM::OnCbnSelchangeChannel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	UpdateCombRegional();
}


void CAA_PARAAM::OnCbnSelchangeRegionallocation()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateCombRegional();
}


void CAA_PARAAM::OnCbnSelchangeRegionaltype()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateCombRegional();
}

////�����ɹ̶�8ͨ���ģ�������Ϊ����ʵ�������
void CAA_PARAAM::InitCombChannel()
{

	//int count = 0;	
	//int i=0;
	//if (NULL == MAINWINDOW_INSTANCE)
	//{
	//	msg("������Ϊ��!");
	//	return ;
	//}
	//map<string ,CAAVision*>mapVision;
	//map<string ,CAAVision*>::iterator mapit;
	//mapVision = MAINWINDOW_INSTANCE->GetVisionMap();	
	//count = mapVision.size();
	//if (count<=0)
	//{
	//	msg("mapVisionΪ��!");
	//}
	//ZERO_CHK(count);
	//CComboBox* pComb = (CComboBox*)GetDlgItem(IDC_CHANNEL);
	//pComb->ResetContent();
	////pComb->InsertString(0, "");

	//for (mapit= mapVision.begin();mapit!= mapVision.end();mapit++)
	//{
	//	pComb->InsertString(i, mapit->first.c_str());
	//}

	//pComb->SetCurSel(0);


	//
	CComboBox* pComb = (CComboBox*)GetDlgItem(IDC_CHANNEL);
	pComb->ResetContent();
	//pComb->InsertString(0, "");

	for (int i= 0;i<AA_MAXVISION;i++)
	{
		pComb->InsertString(i, g_reginalchannel[i].c_str());
	}

	pComb->SetCurSel(0);
}
void CAA_PARAAM::InitCombRegionallocation()
{
	CComboBox* pComb = (CComboBox*)GetDlgItem(IDC_REGIONALLOCATION);
	pComb->ResetContent();
	//pComb->InsertString(0, "");

	for (int i= 0;i<AA_MAX;i++)
	{
		pComb->InsertString(i, g_reginallocation[i].c_str());
	}

	pComb->SetCurSel(0);
}
void CAA_PARAAM::InitCombRegionaltype()
{
	CComboBox* pComb = (CComboBox*)GetDlgItem(IDC_REGIONALTYPE);
	pComb->ResetContent();
	//pComb->InsertString(0, "");

	for (int i= 0;i<AATYPE_MAX;i++)
	{
		pComb->InsertString(i, g_reginaltype[i].c_str());
	}

	pComb->SetCurSel(0);
}

void CAA_PARAAM::InitCombFunSelect()
{
	CComboBox* pComb = (CComboBox*)GetDlgItem(IDC_FUNSELET);
	pComb->ResetContent();
	//pComb->InsertString(0, "");
	//����g_funselect�е���Ŀ��Ŀ
	int itemnum = 0;
	for (int i =0;i<100;i++)
	{
		if (g_funselect[i] == "����")
		{
			break;
		}
		itemnum++;
	}

	for (int i= 0;i<itemnum;i++)
	{
		pComb->InsertString(i, g_funselect[i].c_str());
	}

	pComb->SetCurSel(0);

}
void CAA_PARAAM::UpdateCombRegional()
{
	CString str;
	double row1,row2,col1,col2;
	int nChannelCur = ((CComboBox*)GetDlgItem(IDC_CHANNEL))->GetCurSel();
	ZERO_CHK(nChannelCur >= 0);
	int nRegionLocationCur = ((CComboBox*)GetDlgItem(IDC_REGIONALLOCATION))->GetCurSel();
	ZERO_CHK(nRegionLocationCur >= 0);
	int nRegionTypeCur = ((CComboBox*)GetDlgItem(IDC_REGIONALTYPE))->GetCurSel();
	ZERO_CHK(nRegionTypeCur >= 0);

	switch(nRegionTypeCur)
	{
	case AATYPE_RETANGLE1:
		{
			//��ͼƬ����ʾ���п�
			{
				str.Format("%.1lf", g_AAParam.channel1Retangle[nChannelCur][nRegionLocationCur].ltx);
				GetDlgItem(IDC_EDIT_POINTX1)->SetWindowText(str);
				str.Format("%.1lf", g_AAParam.channel1Retangle[nChannelCur][nRegionLocationCur].lty);
				GetDlgItem(IDC_EDIT_POINTY1)->SetWindowText(str);
				str.Format("%.1lf", g_AAParam.channel1Retangle[nChannelCur][nRegionLocationCur].rbx);
				GetDlgItem(IDC_EDIT_POINTX2)->SetWindowText(str);
				str.Format("%.1lf", g_AAParam.channel1Retangle[nChannelCur][nRegionLocationCur].rby);
				GetDlgItem(IDC_EDIT_POINTY2)->SetWindowText(str);
			}
		}
		//����¼
		RegionLocation = nRegionLocationCur;
		ImageChannel = nChannelCur;
		RegionType = nRegionTypeCur;
		break;
	default:
		break;
	}

}

void CAA_PARAAM::OnBnClickedGrabpic()
{
	int ret = 0;
	// TODO:����ѡ���ͨ�������㲻ͬͨ����ͼ��
	int nChannelCur = ((CComboBox*)GetDlgItem(IDC_CHANNEL))->GetCurSel();
	ZERO_CHK(nChannelCur >= 0);

	int cameraID = nChannelCur;
	unsigned char * pBmp = NULL;
	pBmp =	MAINWINDOW_INSTANCE->GetPicture(cameraID);
	if(pBmp == NULL)
	{
		msg("visonid:%d��ȡԭʼͼƬʧ��,",cameraID);
		return;
	}
	
	ret = IAVH_ImgByte2HObject(m_pImage[nChannelCur], pBmp,g_AAParam.pic_lenth,g_AAParam.pic_width,1);
	if (0 == ret && halconWinID >=0)
	{
		IAVH_DispImage(halconWinID,m_pImage[nChannelCur]);
	}
	free(pBmp);
}


int CAA_PARAAM::Calcpoint()
{
	//int ret = 0;
	//// TODO:����ѡ���ͨ�������㲻ͬͨ����ͼ��
	//int nChannelCur = ((CComboBox*)GetDlgItem(IDC_CHANNEL))->GetCurSel();
	//ZERO_CHK2(nChannelCur >= 0,-1);
	//int nRegionLocationCur = ((CComboBox*)GetDlgItem(IDC_REGIONALLOCATION))->GetCurSel();
	//ZERO_CHK2(nRegionLocationCur >= 0 ,-1);
	//int nRegionTypeCur = ((CComboBox*)GetDlgItem(IDC_REGIONALTYPE))->GetCurSel();
	//ZERO_CHK2(nRegionTypeCur >= 0,-1);
	//if (!m_pImage[nChannelCur].IsInitialized())
	//{
	//	AfxMessageBox("���Ȼ�ȡͼ��!");
	//	return -1;
	//}
	//HObject OutObject;
	//IAVH_ROI_STRUCT roiP;
	//if (0 == nRegionTypeCur)//ѡ��0
	//{
	//	roiP.type = IAVH_RETANGLE;
	//	//��������.�£��У���,��������5�������10���㣬ÿ������������������20����
	//	roiP.rect.Row1 = g_AAParam.channel1Retangle[nChannelCur][nRegionLocationCur].ltx;
	//	roiP.rect.Column1 = g_AAParam.channel1Retangle[nChannelCur][nRegionLocationCur].lty;
	//	roiP.rect.Row2 = g_AAParam.channel1Retangle[nChannelCur][nRegionLocationCur].rbx;
	//	roiP.rect.Column2 = g_AAParam.channel1Retangle[nChannelCur][nRegionLocationCur].rby;

	//}	
	////��ȡ����
	//ret = IAVH_GetImgRetangleROI(m_pImage[nChannelCur],OutObject, roiP,0);
	////����ָ�������ڵ�������
	//double center[2] = {0.0};
	//IAVH_PARAM_STRUCT param ;
	//param.selectShape.min = g_AA_PicParam.selectShape[nChannelCur].min;
	//param.selectShape.max = g_AA_PicParam.selectShape[nChannelCur].max;
	//ret = IAVH_GetImgROICenterByArea(OutObject,center,param,0);

	//IAVH_PaintCross(halconWinID,center,60);

	//char temp[128] = {0};
	//sprintf_s(temp,"����Ϊ:(%lf,%lf)",center[0],center[1]);
	//MessageBox(temp);
	//return ret;

	//�޸�
	int ret = 0;
	HObject OutObject;
	IAVH_ROI_STRUCT roiP[5];
	double center[2] = {0.0};
	IAVH_PARAM_STRUCT param ;
	// TODO:����ѡ���ͨ�������㲻ͬͨ����ͼ��
	int nChannelCur = ((CComboBox*)GetDlgItem(IDC_CHANNEL))->GetCurSel();
	ZERO_CHK2(nChannelCur >= 0,-1);
	int nRegionTypeCur = ((CComboBox*)GetDlgItem(IDC_REGIONALTYPE))->GetCurSel();
	ZERO_CHK2(nRegionTypeCur >= 0,-1);
	if (!m_pImage[nChannelCur].IsInitialized())
	{
		AfxMessageBox("���Ȼ�ȡͼ��!");
		return -1;
	}
	for( int i=0;i<AA_VISION_RETAGLELOC;i++ )
	{
		roiP[i].type = IAVH_RETANGLE;
		roiP[i].rect.Row1 = g_AAParam.channel1Retangle[nChannelCur][i].ltx;
		roiP[i].rect.Column1 = g_AAParam.channel1Retangle[nChannelCur][i].lty;
		roiP[i].rect.Row2 = g_AAParam.channel1Retangle[nChannelCur][i].rbx;
		roiP[i].rect.Column2 = g_AAParam.channel1Retangle[nChannelCur][i].rby;
	}
	for( int i=0;i<AA_VISION_RETAGLELOC;i++ )
	{
		//��ȡ����
		ret = IAVH_GetImgRetangleROI(m_pImage[nChannelCur],OutObject, roiP[i],0);
		if( ret != 0 )
		{
			return -1;
		}
		//����ָ�������ڵ�������
		param.selectShape.min = g_AA_PicParam.selectShape[nChannelCur].min;
		param.selectShape.max = g_AA_PicParam.selectShape[nChannelCur].max;
		param.scaleImage.mul = g_AA_PicParam.scaleImage[nChannelCur].mul;
		param.scaleImage.add = g_AA_PicParam.scaleImage[nChannelCur].add;
		ret = IAVH_GetImgROICenterByArea(OutObject,center,param,0);
		if( ret != 0 )
		{
			return -1;
		}
		IAVH_PaintCross(halconWinID,center,60);

	}
	return ret;
}
int CAA_PARAAM::CalcRegionArea(vector<double>& area)
{
	//����ָ��ROI�ڵ�Region���(������Region�����ҵ�)���ټ���ƽ��ֵ������5��ROI�����е�����Region�����������ƽ��ֵ
	//�������Գ����󣬾Ϳ������ú��ʵ����ֵ
	int ret = 0;
	double avg = 0 , Avg_Area = 0;
	HObject OutObject,Grid,roiarea,connectrdR,Slected;
	HTuple usedThreshold,Area,row,col,median;
	IAVH_ROI_STRUCT roiP[5];
	// TODO:����ѡ���ͨ�������㲻ͬͨ����ͼ��
	int nChannelCur = ((CComboBox*)GetDlgItem(IDC_CHANNEL))->GetCurSel();
	ZERO_CHK2(nChannelCur >= 0,-1);
	int nRegionLoctionCur = ((CComboBox*)GetDlgItem(IDC_REGIONALLOCATION))->GetCurSel();
	ZERO_CHK2(nRegionLoctionCur >= 0,-1);

	int nRegionTypeCur = ((CComboBox*)GetDlgItem(IDC_REGIONALTYPE))->GetCurSel();
	ZERO_CHK2(nRegionTypeCur >= 0,-1);

	if (!m_pImage[nChannelCur].IsInitialized())
	{
		AfxMessageBox("���Ȼ�ȡͼ��!");
		return -1;
	}



	//for( int i=0;i<AA_VISION_RETAGLELOC;i++ )
	//{
	//	roiP[i].type = IAVH_RETANGLE;
	//	roiP[i].rect.Row1 = g_AAParam.channel1Retangle[nChannelCur][i].ltx;
	//	roiP[i].rect.Column1 = g_AAParam.channel1Retangle[nChannelCur][i].lty;
	//	roiP[i].rect.Row2 = g_AAParam.channel1Retangle[nChannelCur][i].rbx;
	//	roiP[i].rect.Column2 = g_AAParam.channel1Retangle[nChannelCur][i].rby;
	//}
	//for( int i=0;i<AA_VISION_RETAGLELOC;i++ )
	//{
	//	//��ȡ����
	//	ret = IAVH_GetImgRetangleROI(m_pImage[nChannelCur],OutObject, roiP[i],0);
	//	if( ret != 0 )
	//	{
	//		return -1;
	//	}
	//	//�Ҷ�����
	//	//����ͼ�����ͼ�񣬺���Ҫ���ɶ�̬�ģ�����Ҫ�ֶ�������������Զ�����
	//	ScaleImage(OutObject, &Grid, 255/(90-0), -255*0/(90-0));
	//	BinaryThreshold(Grid,&roiarea,"max_separability","dark",&usedThreshold);
	//	Connection(roiarea,&connectrdR);
	//	SelectShape(connectrdR,&Slected,"area","and",0,9999999);
	//	AreaCenter(Slected,&Area,&row,&col);
	//	//���ֵ
	//	TupleMean(Area, &median);
	//	avg = median.TupleReal();
	//	Avg_Area = Avg_Area + avg;
	//}
	//Avg_Area = Avg_Area/5;
	////g_AA_PicParam.selectShape[nChannelCur].min = Avg_Area;
	//char number[10];
	//sprintf(number,"%.1f",Avg_Area);
	//MessageBox(TEXT(number),TEXT("�������������"),0);


	//���㵥�������������ҵ�����ͨ����������С
	IAVH_ROI_STRUCT roitemp;
	roitemp.type = (_IAVH_ROI_TYPE)nRegionTypeCur;
	roitemp.rect.Row1 = g_AAParam.channel1Retangle[nChannelCur][nRegionLoctionCur].ltx;
	roitemp.rect.Column1 = g_AAParam.channel1Retangle[nChannelCur][nRegionLoctionCur].lty;
	roitemp.rect.Row2 = g_AAParam.channel1Retangle[nChannelCur][nRegionLoctionCur].rbx;
	roitemp.rect.Column2 = g_AAParam.channel1Retangle[nChannelCur][nRegionLoctionCur].rby;

	ret = IAVH_GetImgRetangleROI(m_pImage[nChannelCur],OutObject,roitemp,0);
	if( ret != 0 )
	{
		return -1;
	}

	//������������ڵ���Ͱ����Ĵ�С
	IAVH_PARAM_STRUCT param ;
	param.selectShape.min = g_AA_PicParam.selectShape[nChannelCur].min;
	param.selectShape.max = g_AA_PicParam.selectShape[nChannelCur].max;
	param.scaleImage.mul = g_AA_PicParam.scaleImage[nChannelCur].mul;
	param.scaleImage.add = g_AA_PicParam.scaleImage[nChannelCur].add;
	vector<double>areas;
	IAVH_GetImgROIAreas(OutObject,area,param,0);

	vector<double>::iterator it;
	string str = "�����СΪ:";
	char areas_str[256] = {0};
	int i = 0;
	for (it = area.begin();it!= area.end();it++)
	{	
		sprintf(areas_str,"��%d��:%.1lf ",++i,*it);
		str.append(areas_str);
	}

	MessageBox(TEXT(str.c_str()),TEXT("����������б�"),0);


	return ret;
}
int CAA_PARAAM::CalcRegionMTF()
{
	int ret = 0;
	// TODO:����ѡ���ͨ�������㲻ͬͨ����ͼ��
	int nChannelCur = ((CComboBox*)GetDlgItem(IDC_CHANNEL))->GetCurSel();
	ZERO_CHK2(nChannelCur >= 0,-1);
	int nRegionLoctionCur = ((CComboBox*)GetDlgItem(IDC_REGIONALLOCATION))->GetCurSel();
	ZERO_CHK2(nRegionLoctionCur >= 0,-1);

	int nRegionTypeCur = ((CComboBox*)GetDlgItem(IDC_REGIONALTYPE))->GetCurSel();
	ZERO_CHK2(nRegionTypeCur >= 0,-1);

	//���㵥�������������ҵ�����ͨ����������С
	//IAVH_ROI_STRUCT roitemp;
	//roitemp.type = (_IAVH_ROI_TYPE)nRegionTypeCur;
	//roitemp.rect.Row1 = g_AAParam.channel1Retangle[nChannelCur][nRegionLoctionCur].ltx;
	//roitemp.rect.Column1 = g_AAParam.channel1Retangle[nChannelCur][nRegionLoctionCur].lty;
	//roitemp.rect.Row2 = g_AAParam.channel1Retangle[nChannelCur][nRegionLoctionCur].rbx;
	//roitemp.rect.Column2 = g_AAParam.channel1Retangle[nChannelCur][nRegionLoctionCur].rby;

	Point edgePoint;
	double lapVaule =-1.0;
	IAVO_ShowPicture(m_OpencvWinstr,m_opImage[nChannelCur]);

	Mat result = IAVO_ImgResize(m_opImage[nChannelCur],g_AAParam.scale);

	IAVO_ShowPicture(m_OpencvWinstr,result);
	int a = m_opImage[nChannelCur].channels();
	//nRegionLoctionCur = 0;//�����������ĵ�ˮƽ����
	switch(nRegionLoctionCur)
	{
	case AA_CENTER:
		nRegionLoctionCur = CENTERX;
		break;
	case AA_LEFT_LOC: //��ʱ��AA_LEFT_LOCӳ��Ϊ��ֱ�����y
		nRegionLoctionCur = CENTERY;
		break;
	case AA_LEFT:
		nRegionLoctionCur = LEFT;
		break;
	case AA_RIGHT:
		nRegionLoctionCur = RIGHT;
		break;
	case AA_TOP:
		nRegionLoctionCur = TOP;
		break;
	case AA_BOTTOM:
		nRegionLoctionCur = BOTTOM;
		break;
	}

	calEdgePointInRegion(result,nChannelCur,nRegionLoctionCur,edgePoint,lapVaule);
	char areas_str[256] = {0};
	sprintf_s(areas_str,"�е�:(%d,%d),MTF :%lf",edgePoint.x,edgePoint.y,lapVaule);
	MessageBox(areas_str);
	return ret;
}
int CAA_PARAAM:: CalcRegionRightHarryPoint()
{
	int ret = 0;
	int nChannelCur = ((CComboBox*)GetDlgItem(IDC_CHANNEL))->GetCurSel();
	ZERO_CHK2(nChannelCur >= 0,-1);
	int nRegionLoctionCur = ((CComboBox*)GetDlgItem(IDC_REGIONALLOCATION))->GetCurSel();
	ZERO_CHK2(nRegionLoctionCur >= 0,-1);

	int nRegionTypeCur = ((CComboBox*)GetDlgItem(IDC_REGIONALTYPE))->GetCurSel();
	ZERO_CHK2(nRegionTypeCur >= 0,-1);

	m_pImage[nChannelCur] = MatToHImage(m_opImage[nChannelCur]);
	//IAVH_DispImage(halconWinID,m_pImage[nChannelCur]);

	//���㵥�������������ҵ�����ͨ����������С
	IAVH_ROI_STRUCT roitemp;
	roitemp.type = (_IAVH_ROI_TYPE)nRegionTypeCur;
	roitemp.rect.Row1 = g_AAParam.channel1Retangle[nChannelCur][nRegionLoctionCur].ltx;
	roitemp.rect.Column1 = g_AAParam.channel1Retangle[nChannelCur][nRegionLoctionCur].lty;
	roitemp.rect.Row2 = g_AAParam.channel1Retangle[nChannelCur][nRegionLoctionCur].rbx;
	roitemp.rect.Column2 = g_AAParam.channel1Retangle[nChannelCur][nRegionLoctionCur].rby;

	HObject OutObject;
	ret = IAVH_GetImgRetangleROI(m_pImage[nChannelCur],OutObject,roitemp,0);
	if( ret != 0 )
	{
		return -1;
	}

	vector<double>point;
	IAVH_PARAM_STRUCT param;
	param.scaleImage.mul = g_AA_PicParam.scaleImage[nChannelCur].mul;
	param.scaleImage.add = g_AA_PicParam.scaleImage[nChannelCur].add;
	IAVH_GetImgROIRightHarryPoint(OutObject,point, param,0);
	if (point.empty())
	{
		ret = -1;
		MessageBox("û���ҵ��ǵ�!");
	}
	else
	{
		double harryPt[2] = {0.0};
		harryPt[0] = point[0];
		harryPt[1] = point[1];
		IAVH_PaintCross(halconWinID,harryPt,60);

		if (AA_CENTER == nRegionLoctionCur)
		{	
			g_AAParam.paintCenter[nChannelCur].X=harryPt[1];
			g_AAParam.paintCenter[nChannelCur].Y=harryPt[0];
		}
	}


	return ret;

}
int CAA_PARAAM:: CalcHalconProcess(int vaid,int vbid)
{
	int ret = 0;
	//���㷨�ӿ��м�����������������������������������ӣ�
	//�������Գ����󣬾Ϳ������ú��ʵ����ֵ
	

	int iWinIDA = -1,iWinIDB = -1;
	vector<double> inparam;
	vector<double> outparam;
	int display =0,error =0;
	string hmessage;

	//CString str;
	//char strindexA[10]= {0};
	//char strindexB[10]= {0};
	//int indexa = 0,indexb= 0;
	///*str.Format("%s", g_AAParam.halconprocess.c_str() );*/
	//GetDlgItem(IDC_EDIT_TESTA)->GetWindowTextA(str);
	//sprintf_s(strindexA,"%d",str.GetBuffer());
	//indexa = atoi(strindexA);
	//GetDlgItem(IDC_EDIT_TESTB)->GetWindowTextA(str);
	//sprintf_s(strindexB,"%d",str.GetBuffer());
	//indexb = atoi(strindexB);
	inparam.push_back(1.1);
	outparam.push_back(1.1);
	double ROIpoint[2][AA_VISION_RETAGLELOC*4];
	for( int i=0;i<AA_VISION_RETAGLELOC;i++ )
	{
		ROIpoint[0][4*i] = g_AAParam.channel1Retangle[vaid][i].ltx;
		ROIpoint[0][4*i+1] = g_AAParam.channel1Retangle[vaid][i].lty;
		ROIpoint[0][4*i+2] = g_AAParam.channel1Retangle[vaid][i].rbx;
		ROIpoint[0][4*i+3] = g_AAParam.channel1Retangle[vaid][i].rby;

		ROIpoint[1][4*i] = g_AAParam.channel1Retangle[vbid][i].ltx;
		ROIpoint[1][4*i+1] = g_AAParam.channel1Retangle[vbid][i].lty;
		ROIpoint[1][4*i+2] = g_AAParam.channel1Retangle[vbid][i].rbx;
		ROIpoint[1][4*i+3] = g_AAParam.channel1Retangle[vbid][i].rby;
	}
	ret = IAVH_AAProcedureProcess(iWinIDA,iWinIDB,m_pImage[vaid],m_pImage[vbid],ROIpoint,g_AAParam.halconprocess,inparam,outparam,display,error,hmessage);

	char r[256] = {0.0};
	char tempstr[256] = {0.0};
	for (int i=0;i<outparam.size();i++)
	{
		sprintf_s(tempstr,"%.4f  ",outparam[i]);
		strcat_s(r,tempstr);
	}

	
	MessageBox(r);
	if (ret == 1)
	{
		MessageBox("OK");
	}
	return ret;

}

//��ͼ��ʾ
void CAA_PARAAM::OnBnClickedReadpic1()
{
	CString strFilePath;
	int index = 0;
	const char pszFilter[] = _T("(*.*)|*.*|*.bmp|*.bmp|(*.jpg,*.jpeg,*.jpe)|*.jpg|(*.png)|*.png|(*.tif)|*.tif||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,pszFilter, this);
	int nChannelCur = ((CComboBox*)GetDlgItem(IDC_CHANNEL))->GetCurSel();
	if (nChannelCur>0)
	{
		index = nChannelCur;
	}
	if(dlg.DoModal() == IDOK)
	{
		strFilePath = dlg.GetPathName();
		if (0 == strFilePath.GetLength())
		{
			return;
		}
		if (DOUBLEAA == g_AAParam.AATYPE)//����ֻ����opencv
		{
			IAVH_ReadImage(m_pImage[index], strFilePath.GetBuffer());
			IAVH_DispImage(halconWinID,m_pImage[index]);
		}
		else
		{
			CRect rt;
			m_debugvideo.GetClientRect(&rt);
			IAVO_ReadPicture(strFilePath.GetBuffer(),m_opImage[index]);

			m_pImage[index] = MatToHImage(m_opImage[index]);
			IAVH_DispImage(halconWinID,m_pImage[index]);
			//IAVO_ShowWindow(m_OpencvWinstr,m_opImage[index],rt.Width(),rt.Height());
		}

	}
	else
	{
		return ;
	}
}

//���Թ���ѡ��ѡ��ͬ�Ĺ���������
void CAA_PARAAM::OnCbnSelchangeFunselet()
{

	CString str;
	int nCur = ((CComboBox*)GetDlgItem(IDC_FUNSELET))->GetCurSel();
	if (nCur <=0)
	{
		MessageBox("��ѡ������Ĺ���!");
		return;
	}

	double row1,row2,col1,col2;
	int nChannelCur = ((CComboBox*)GetDlgItem(IDC_CHANNEL))->GetCurSel();
	ZERO_CHK(nChannelCur >= 0);
	int nRegionLocationCur = ((CComboBox*)GetDlgItem(IDC_REGIONALLOCATION))->GetCurSel();
	ZERO_CHK(nRegionLocationCur >= 0);
	int nRegionTypeCur = ((CComboBox*)GetDlgItem(IDC_REGIONALTYPE))->GetCurSel();
	ZERO_CHK(nRegionTypeCur >= 0)


	((CComboBox*)GetDlgItem(IDC_FUNSELET))->GetLBText(nCur,str);
	if (!str.Compare("������"))
	{
		if( halconWinID >= 3600 /*&& nChannelCur == ImageChannel && nRegionTypeCur == RegionType*/ )
		{
			HTuple WindowHandle(halconWinID);
			//��ͼƬ�л�ȡ���ο�
			IAVH_DrawRectangle1(halconWinID,&row1,
				&col1,
				&row2,
				&col2
				);
			//���¾��ο�����굽���棬
			str.Format("%.1lf", row1);
			GetDlgItem(IDC_EDIT_POINTX1)->SetWindowText(str);
			str.Format("%.1lf", col1);
			GetDlgItem(IDC_EDIT_POINTY1)->SetWindowText(str);
			str.Format("%.1lf", row2);
			GetDlgItem(IDC_EDIT_POINTX2)->SetWindowText(str);
			str.Format("%.1lf", col2);
			GetDlgItem(IDC_EDIT_POINTY2)->SetWindowText(str);
			g_AAParam.channel1Retangle[nChannelCur][nRegionLocationCur].ltx = row1;
			g_AAParam.channel1Retangle[nChannelCur][nRegionLocationCur].lty = col1;
			g_AAParam.channel1Retangle[nChannelCur][nRegionLocationCur].rbx = row2;
			g_AAParam.channel1Retangle[nChannelCur][nRegionLocationCur].rby = col2;
		}
		
	}
	else if (!str.Compare("��������������"))
	{
		Calcpoint();
	}
	else if (!str.Compare("ͼ���������"))
	{
		AA_PICPARAM dlg;

		//m_pMainWnd = &dlg;
		int nResponse = dlg.DoModal();
		if (nResponse == IDOK)
		{
		}
		else if (nResponse == IDCANCEL)
		{
		}
	}
	else if (!str.Compare("�����������"))
	{
		vector<double> area;
		CalcRegionArea(area);
	}
	else if (!str.Compare("��������MTF"))
	{
		CalcRegionMTF();
	}
	else if (!str.Compare("��������ǵ�"))//��ʱֻ�������½ǵ�
	{
		CalcRegionRightHarryPoint();
	}
	else if (!str.Compare("halcon���̼���")	)
	{
		int ret =0 ;
		CString str;
		char strindexA[10]= {0};
		char strindexB[10]= {0};
		int indexa = 0,indexb= 1;
		/*str.Format("%s", g_AAParam.halconprocess.c_str() );*/
		GetDlgItem(IDC_EDIT_TESTA)->GetWindowTextA(str);
		string temp=str.GetBuffer();
		sprintf_s(strindexA,"%d",temp.c_str());
		str.ReleaseBuffer();
		indexa = atoi(temp.c_str());
		GetDlgItem(IDC_EDIT_TESTB)->GetWindowTextA(str);
		temp=str.GetBuffer();
		sprintf_s(strindexB,"%d",temp.c_str());
		str.ReleaseBuffer();
		indexb = atoi(temp.c_str());

		if (indexa>8 || indexa <0 || indexb>8 ||indexb <0)
		{
			MessageBox("��������ͼ��������!");
			return ;
		}

		ret = CalcHalconProcess(indexa,indexb);
	}
	
}


void CAA_PARAAM::OnBnClickedCheckHalcon()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (((CButton *)GetDlgItem(IDC_CHECK_HALCON))->GetCheck())
	{
		g_AAParam.iscall_halprocess =  true;
	}
	else
	{
		g_AAParam.iscall_halprocess =  false;
	}
	
}


void CAA_PARAAM::OnBnClickedBtnLoadhal()
{
	CString strFilePath;
	const char pszFilter[] = _T("(*.*)|*.*|*.hdvp|*.hdpl");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,pszFilter, this);

	if(IDOK == dlg.DoModal())
	{
		strFilePath = dlg.GetPathName();
		if (0 == strFilePath.GetLength())
		{
			return;
		}
		//strFilePath.Replace('\\','/');
		g_AAParam.halconprocess = strFilePath.GetBuffer();
		GetDlgItem(IDC_EDIT_HALPROCESS)->SetWindowText(strFilePath);
	}

}


void CAA_PARAAM::OnStnDblclickDebugVideo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

