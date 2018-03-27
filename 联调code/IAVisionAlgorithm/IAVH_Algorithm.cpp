#include <iostream>
#include "IAVisionAlgorithm.h"

using namespace std;

#define CHECK_HOBJECT_VALID(Object) 	if (false == Object.IsInitialized())	\
	{	\
		IAV_PRINTF("ͼ��Ϊ��!");	\
		return IAV_ERROR;	\
	}	\

#define CHECK_PICFILENAME_VALID(filenamne ,fileformt)		{	int pos = std::string::npos;	\
if(string::npos != (pos = filename.rfind(".jpg")) || string::npos != (pos = filename.rfind(".bpm") ))	\
{	\
	int len = filename.length();	\
	fileformt = filename.substr(pos+1, len- pos) ;	\
}	\
else	\
{	\
	IAV_PRINTF("ͼ���ʽ����!!");	\
	return IAV_ERROR;	\
}	\
}
//Halcon�ӿ�

int IAVH_ShowPicture(HObject& Object)
{
	HTuple HalWinId,H_Width,H_Height;//����id,ͼ���С 
	long halconWinID ,Width,Height;  //����id,ͼ���С 
	int ret = IAV_OK;
	int rin = IAV_OK;
	CHECK_HOBJECT_VALID(Object)
	try
	{	
		GetImageSize(Object, &H_Width,  &H_Height);
		Width = (long)H_Width.L();
		Height = (long)H_Height.L();
		OpenWindow(0,0,512,512, 0, "visible", "", &HalWinId);
		halconWinID = (long)HalWinId.L();
		SetPart(halconWinID,0,0,Height,Width);		
		DispObj(Object,halconWinID);
		Sleep(1000);
		cin>> rin; //�����ã������������
		HalconCpp::CloseWindow(halconWinID);
	}
	catch (...)
	{
		IAV_PRINTF("�����쳣!!");
		ret = IAV_ERROR;
	}
	return ret;
}
int IAVH_DispImage(int hwinID,HObject& Object)
{
	HObject Arrow, ArrowX, ArrowY;
	HTuple width, height, Point1Row, Point1Col, Point2Row, Point2Col;
	HTuple row,col, row1,col1;

	int ret = IAV_OK;
	CHECK_HOBJECT_VALID(Object)

	try
	{
		HalconCpp::ClearWindow(hwinID);


		//��ʾͼ��ʱ,����ͼ���С��ʾ
		HTuple w,h;
		GetImageSize(Object,&w,&h);
		SetPart(hwinID,0,0,h,w);
		//HalconCpp::DispImage(Object,hwinID);//
		HalconCpp::DispObj(Object,hwinID);
		/*HalconCpp::DispColor(Object,hwinID);*/
		
	}
	catch(...)
	{
		IAV_PRINTF("�����쳣!!");
		ret = IAV_ERROR;
	}
	return ret;
}
int IAVH_ReadImage(HObject& Object,char *filename)
{
	int ret = IAV_OK;

	try
	{
		Object.Clear();
		HalconCpp::ReadImage(&Object, filename);

	}
	catch(...)
	{
		IAV_PRINTF("�����쳣!!");
		ret = IAV_ERROR;
	}
	return ret;
}
int IAVH_SavePicture(HObject& Object,string filename)
{
	int ret = IAV_OK;
	CHECK_HOBJECT_VALID(Object)
	//�ļ���⣬ĩβ�к�׺��.jpg����.bpm

	string fileformt;
	CHECK_PICFILENAME_VALID(filenamne ,fileformt)
	//����ע�� �������ɾ��
	//int pos = std::string::npos;	
	//if(string::npos != (pos = filename.rfind(".jpg")) || string::npos != (pos = filename.rfind(".bpm") ))
	//{
	//	int len = filename.length();
	//	fileformt = filename.substr(pos, len- pos) ;

	//}
	//else
	//{
	//	IAV_PRINTF("ͼ���ʽ����!!");
	//	return IAV_ERROR;
	//}
	try
	{
		WriteImage(Object,fileformt.c_str(),0,filename.c_str());
	}
	catch (...)
	{
		IAV_PRINTF("�����쳣!!");
		ret = IAV_ERROR;
	}
	return ret;
}
 int IAVH_SavePictureInByte(unsigned char* buff,string filename,int w,int h,int chNum)
{
	HObject Object;
	
	int ret = IAV_OK;
	if (NULL == buff)
	{
		IAV_PRINTF("�����쳣!!");
		return IAV_ERROR;
	}

	ret = IAVH_ImgByte2HObject(Object,buff,w,h,chNum);
	if (IAV_OK != ret)
	{
		IAV_PRINTF("�����쳣!!");
		return IAV_ERROR;
	}
	CHECK_HOBJECT_VALID(Object)
		//�ļ���⣬ĩβ�к�׺��.jpg����.bpm
	string fileformt;
	CHECK_PICFILENAME_VALID(filenamne ,fileformt)
	try
	{
		WriteImage(Object,fileformt.c_str(),0,filename.c_str());
	}
	catch (...)
	{
		IAV_PRINTF("�����쳣!!");
		ret = IAV_ERROR;
	}
	return ret;

}

int IAVH_SaveWindowPicture(long winID,string filename)
{
	int ret = IAV_OK;	
	//�ļ���⣬ĩβ�к�׺��.jpg����.bpm
	string fileformt;
	CHECK_PICFILENAME_VALID(filenamne ,fileformt)
	try
	{
		HalconCpp::DumpWindow(winID, fileformt.c_str(), filename.c_str());	
	}
	catch (...)
	{
		IAV_PRINTF("�����쳣!!");
		ret = IAV_ERROR;
	}
	return ret;
}

int IAVH_ImgResize(HObject& Object,HObject& OutObject, double w,double h,bool isfactor,string  interp)
{
	int ret = IAV_OK;	
	CHECK_HOBJECT_VALID(Object)
	try
	{
		if (true == isfactor)
		{
			ZoomImageFactor(Object,&OutObject,w,h,interp.c_str());
		}
		else
		{
			ZoomImageSize(Object,&OutObject,w,h,interp.c_str());
		}	
	}
	catch (...)
	{
		IAV_PRINTF("�����쳣!!");
		ret = IAV_ERROR;
	}
	return ret;

}

int IAVH_GetImgRetangleROI(HObject& Object,HObject& OutObject, IAVH_ROI_STRUCT roiP,bool showWid)
{
	int ret = IAV_OK;	
	HObject grayObject,roiarea;
	CHECK_HOBJECT_VALID(Object)
	try
	{
		Rgb1ToGray(Object,&grayObject);
		switch(roiP.type)
		{
		case IAVH_RETANGLE:
			{//���ϵ㣬���µ�ȷ������
				GenRectangle1(&roiarea,roiP.rect.Row1,roiP.rect.Column1,roiP.rect.Row2,roiP.rect.Column2);				
			break;
			}
		case IAVH_CIRCLE:
			{
				GenCircle(&roiarea,roiP.cir.Row,roiP.cir.Column,roiP.cir.rad);	
			break;
			}
		default:
			ret = IAV_ERROR;
			return ret;
		}
		ReduceDomain(grayObject,roiarea,&OutObject);
	}
	catch (...)
	{
		IAV_PRINTF("�����쳣!!");
		ret = IAV_ERROR;
	}
	return ret;


}

int IAVH_ImgByte2HObject(HObject &Object, unsigned char * pBmp,int w,int h,int chNum)
{
	int ret = IAV_OK;	
	unsigned char  *image_red = NULL;
	unsigned char  *image_green= NULL;
	unsigned char  *image_blue= NULL;
	int            r,c;
	try
	{	
		image_red = (unsigned char  *)malloc(w*h);
		if( 3 == chNum )
		{				
			image_green = (unsigned char  *)malloc(w*h);
			image_blue = (unsigned char  *)malloc(w*h);
		}

	}catch (...)
	{
		IAV_PRINTF("�����쳣!!");
		free(image_red);
		free(image_green);
		free(image_blue);
		return IAV_ERROR;
	}


	if (3 == chNum)
	{	
		for (r=0; r<h; r++)
		{
			for (c=0; c<w; c++)
			{
				image_red[r*w+c] = pBmp[3*(c + r*w) +0];
				image_green[r*w+c] = pBmp[3*(c + r*w) +1];
				image_blue[r*w+c] = pBmp[3*(c + r*w) +2];
			}
		}
		GenImage3Extern(&Object,"byte",w,h,(Hlong)image_red,(long)image_green,(long)image_blue,(long)free);
	}
	else
	{
		for (r=0; r<h; r++)
		{
			for (c=0; c<w; c++)
			{
				//image_red[r*w+c] = pBmp[3*(c + r*w) +0];
				//image_green[r*w+c] = pBmp[3*(c + r*w) +1];
				//image_blue[r*w+c] = pBmp[3*(c + r*w) +2];
				image_red[r*w+c] = 0.30 * pBmp[3*(c + r*w) +0] + 0.59 * pBmp[3*(c + r*w) +1] + 0.11 *pBmp[3*(c + r*w) +2];
			}
		}
		GenImage1Extern(&Object,"byte",w,h,(Hlong)image_red,(long)free);
	}
	return ret;		
}

int IAVH_GetImgROICenterByArea(HObject& Object,double* center,IAVH_PARAM_STRUCT param,bool showWid)
{
	int ret = IAV_OK;	
	/*double minarea = 5000,maxarea = 200000;*/
	HObject grayObject,roiarea,connectrdR,Slected, Grid,objectselect;
	HTuple usedThreshold,area,row,col,index;
	CHECK_HOBJECT_VALID(Object)	
	try
	{
		//�Ҷ�����
		//����ͼ�����ͼ�񣬺���Ҫ���ɶ�̬�ģ�����Ҫ�ֶ�������������Զ�����
		//ScaleImage(Object, &Grid, 255/(90-0), -255*0/(90-0));
		ScaleImage(Object, &Grid, param.scaleImage.mul,param.scaleImage.add);
		BinaryThreshold(Grid,&roiarea,param.binarythreshold.Method,param.binarythreshold.LightDark,&usedThreshold);
		Connection(roiarea,&connectrdR);
		SelectShape(connectrdR,&Slected,param.selectShape.Features,param.selectShape.Operation,param.selectShape.min,param.selectShape.max);
		AreaCenter(Slected,&area,&row,&col);
		//���ҵ���Region���򣬰��������(����)��index�洢������֮ǰ�����е��±�
		TupleSortIndex(area, &index);
		//�õ�area����ĳ��ȣ�ѡ���������Region����
		SelectObj(connectrdR, &objectselect, index[index.TupleLength()-1]+1);
		AreaCenter(objectselect,&area,&row,&col);
		center[0] = row.D();
		center[1] = col.D();
	}
	catch (...)
	{
		IAV_PRINTF("�����쳣!!");
		ret = IAV_ERROR;
	}
	return ret;
}
int IAVH_GetImgROIAreas(HObject& Object,vector<double>&areas,IAVH_PARAM_STRUCT param,bool showWid)
{
	int ret = IAV_OK;	
	/*double minarea = 5000,maxarea = 200000;*/
	HObject grayObject,roiarea,connectrdR,Slected, Grid,objectselect;
	HTuple usedThreshold,area,row,col,index;
	CHECK_HOBJECT_VALID(Object)	
	try
	{
		//�Ҷ�����
		//����ͼ�����ͼ�񣬺���Ҫ���ɶ�̬�ģ�����Ҫ�ֶ�������������Զ�����
		//ScaleImage(Object, &Grid, 255/(90-0), -255*0/(90-0));
		ScaleImage(Object, &Grid, param.scaleImage.mul,param.scaleImage.add);
		BinaryThreshold(Grid,&roiarea,param.binarythreshold.Method,param.binarythreshold.LightDark,&usedThreshold);
		Connection(roiarea,&connectrdR);
		SelectShape(connectrdR,&Slected,param.selectShape.Features,param.selectShape.Operation,1,param.selectShape.max);
		AreaCenter(Slected,&area,&row,&col);
		//���ҵ���Region���򣬰��������(����)��index�洢������֮ǰ�����е��±�
		TupleSortIndex(area, &index);
		//�õ�area����ĳ��ȣ�ѡ���������Region����
		//SelectObj(connectrdR, &objectselect, index[index.TupleLength()-1]+1);
		//AreaCenter(objectselect,&area,&row,&col);
		//center[0] = row.D();
		//center[1] = col.D();

		for (int i = 0; i<index.TupleLength();i++)
		{
			areas.push_back(area[i]);
		}
		

	}
	catch (...)
	{
		IAV_PRINTF("�����쳣!!");
		ret = IAV_ERROR;
	}
	return ret;
}


int IAVH_PaintCross(int hwinID,double* pt,double size,double angle)
{
	int ret = IAV_OK;
	HObject Cross;
	try
	{
		GenCrossContourXld(&Cross,pt[0],pt[1],size,angle);
		DispObj(Cross,hwinID);
		//DispCross(hwinID,pt[0],pt[1],size,angle);����ֱ������

	}

	catch (...)
	{
		IAV_PRINTF("�����쳣!!");
		ret = IAV_ERROR;
	}

	return ret;

}


int IAVH_DrawRectangle1(int WindowHandle,double *Row1, double *Column1, double *Row2, double *Column2)
{
	int ret = IAV_OK;
	HTuple handle(WindowHandle),row1,row2,col1,col2;
	try
	{	
		DrawRectangle1(handle, &row1, &col1, &row2, &col2);
		*Row1 = row1.TupleReal();
		*Column1 = col1.TupleReal();
		*Row2 = row2.TupleReal();
		*Column2 = col2.TupleReal();
	}
	catch(...)
	{
		ret = IAV_ERROR;
	}
	return ret;
}


//�ǳ��򵥵�������
bool MyCmp_x(Point &a ,Point &b )
{
	if( a.x < b.x)
		return true;
	else
		return false;
}

int IAVH_GetPara(vector<Point> pointB,vector<Point> pointI,double Dis,double pixel_size,double f,double D,int rows,int cols,double u,double v,double w)
{
	Point Btl, Btr, Bbl, Bbr, Bc, Itl, Itr, Ibl, Ibr, Ic;
	double d = 1 / (1 / f - 1 / D);//�����
	double dis = Dis * d / D;//��������ͷ���������
	double dis_pixel = dis / pixel_size; //��ȡ������ͷ�����ؾ���
	//���갴 xֵ ��������
	sort(pointB.begin(), pointB.end(),MyCmp_x);
	Bc=pointB[2];
	if( pointB[0].y > pointB[1].y )
	{
		Btl=pointB[1];
		Bbl=pointB[0];
	}
	else
	{
		Btl=pointB[0];
		Bbl=pointB[1];
	}
	if( pointB[3].y > pointB[4].y )
	{
		Btr=pointB[4];
		Bbr=pointB[3];
	}
	else
	{
		Btr=pointB[3];
		Bbr=pointB[4];
	}

	//���갴 xֵ ��������
	sort(pointI.begin(), pointI.end(),MyCmp_x);
	Ic=pointI[2];
	if( pointI[0].y > pointI[1].y )
	{
		Itl=pointI[1];
		Ibl=pointI[0];
	}
	else
	{
		Itl=pointI[0];
		Ibl=pointI[1];
	}
	if( pointI[3].y > pointI[4].y )
	{
		Itr=pointI[4];
		Ibr=pointI[3];
	}
	else
	{
		Itr=pointI[3];
		Ibr=pointI[4];
	}

	//bw�Ƕ�  yΪͼ����з����ϵ����� xΪ�з����ϵ�
	double kb1 = abs( (double)(Btl.y - Btr.y) / (double)(Btl.x - Btr.x) );//�������������б��
	double kb2 = abs( (double)(Bbl.y - Bbr.y) / (double)(Bbl.x - Bbr.x) );//�������������б��
	double kb3 = abs( (double)(Btr.x - Bbr.x) / (double)(Btr.y - Bbr.y) );//�ұ����������б�ʵĵ���
	double kb4 = abs( (double)(Btl.x - Bbl.x) / (double)(Btl.y - Bbl.y) );//������������б�ʵĵ���
	double kB = (kb1 + kb2 + kb3 + kb4) / 4;                        //4��ֵȡƽ��б��                
	//lw�Ƕ�
	double kI1 = abs( (double)(Itl.y - Itr.y) / (double)(Itl.x - Itr.x) );
	double kI2 = abs( (double)(Ibl.y - Ibr.y) / (double)(Ibl.x - Ibr.x) );
	double kI3 = abs( (double)(Itr.x - Ibr.x) / (double)(Itr.y - Ibr.y) );
	double kI4 = abs( (double)(Itl.x - Ibl.x) / (double)(Itl.y - Ibl.y) );
	double kI = (kI1 + kI2 + kI3 + kI4) / 4;

	//4���ǵ�Χ�ɵ��Ǹ�ƽ���ı���
	//�����ߵ�ԭͼб�� ���ƶ�ͼб�� �� ��Ҫת�ĽǶ�֮��Ĺ�ϵ 
	double sita = (abs(kI - kB))/(1+kI*kB); 
	double tempVal_1 = (double)(Btl.y - Btr.y)/(double)(Btr.x - Btl.x);
	double tempVal_2 = (double)(Itl.y - Itr.y)/(double)(Itr.x - Itl.x);

	if( 0 < tempVal_2 - tempVal_1 )
	{
		w = - atan(sita)*180/3.1415926;
	}
	else
	{
		w = atan(sita)*180/3.1415926;
	}

	//����uv
	Point centerB, centerI; 
	centerB.x = 0; centerB.y = 0;
	centerI.x = 0; centerI.y = 0;
	for (int i = 0; i < pointB.size(); i++){
		centerB.x = centerB.x + pointB[i].x; //�ۼ�5��������з����ϵ�����
		centerB.y = centerB.y + pointB[i].y;
		centerI.x = centerI.x + pointI[i].x;
		centerI.y = centerI.y + pointI[i].y;
	}
	centerB.x = centerB.x / pointB.size();//ȡƽ��ֵ
	centerB.y = centerB.y / pointB.size();
	centerI.x = centerI.x / pointB.size();
	centerI.y = centerI.y / pointB.size();

	int delta_x = centerI.y - centerB.y ;//��׼ͼΪB��ͼ����ߣ����� I.x
	int delta_y = centerI.x - centerB.x- dis_pixel;
	//���� v ��ʽ
	int beta_x;
	if( delta_x < 0 )
	{
		beta_x = abs( centerI.y - rows/2 );
	} 
	else
	{
		beta_x = abs( centerB.y - rows/2 );
	}
	double Val_a = beta_x / f; 
	double Val_b = (abs(delta_x) + beta_x)/f;
	//ȷ������
	if( delta_x > 0 )
	{
		v = atan( abs(Val_a - Val_b)/(1+Val_a*Val_b) ) / 3.1415926 * 180;
	}
	else
	{
		v = -atan( abs(Val_a - Val_b)/(1+Val_a*Val_b) ) / 3.1415926 * 180;
	}

	//���� u
	int bate_y;
	if( delta_y < 0 )
	{
		bate_y = abs( centerB.x - cols/2 + dis_pixel );
	}
	else
	{
		bate_y = abs( centerI.x - cols/2 );
	}
	Val_a = bate_y / f;
	Val_b = ( abs(delta_y)+bate_y )/f;
	//ȷ������
	if( delta_y > 0 )
	{
		u = atan( abs( Val_a - Val_b )/(1+Val_a*Val_b) ) / 3.1415926 * 180;
	}
	else
	{
		u = -atan( abs( Val_a - Val_b )/(1+Val_a*Val_b) ) / 3.1415926 * 180;
	}
	return 0;
}


int IAVH_Median_Filter(HObject &Hobject, char* MaskType, int Radius, char* Margin, HObject &OutHobject)
{
	int ret = IAV_OK;
	HTuple mask(MaskType);
	HTuple rad((Hlong)Radius);
	HTuple margin(Margin);
	//��ֵ�˲�
	try
	{
		MedianImage(Hobject, &OutHobject, mask, rad, margin);
	}
	catch (...)
	{
		IAV_PRINTF("��ֵ�˲��쳣!");
		ret = IAV_ERROR;
	}
	
	return ret;
}
int IAVH_GetImgROIRightHarryPoint(HObject& Object,vector<double>&point,IAVH_PARAM_STRUCT param,bool showWid)
{
	int ret = IAV_OK;
	HTuple Row =0.0,Col=0.0;
	HTuple index;
	HObject grayobject,filter,out;
	//��ֵ�˲�
	try
	{
		//Rgb1ToGray(Object,&grayobject);
		//MedianImage(grayobject,&filter,"circle",3,"mirrored");
		//ScaleImage(filter, &Object, param.scaleImage.mul,param.scaleImage.add);
		PointsHarris(Object, 3, 20, 0.01, 1000, &Row, &Col);
		TupleSortIndex(Row, &index);
		//�����������Ľǵ��ҳ���
		point.push_back(Row[index.TupleLength()-1]);
		point.push_back(Col[index.TupleLength()-1]);
	}
	catch (...)
	{
		IAV_PRINTF("�ҽǵ��쳣!");
		ret = IAV_ERROR;
	}

	return ret;

}
//AA����halcon���̴������
int IAVH_AAProcedureProcess(long iWinIDA,long iWinIDB,HObject& ImageA,HObject& ImageB,double arr[][7*4],string ProcedurePath,vector<double> &inparam,vector<double> &outparam,int display,int &error,string &hmessage)
{
	bool res = true;
	char szParamName[256] ={0};
	//Input
	HTuple PointA,PointB;//��Ӧͨ����ROI���ο�����
	HTuple  InputTuple;
	HObject InputObj;
	
	HObject m_vecObjA,m_vecObjB;

	//OutPut
	HTuple  Message,m_bIsPass;
	HObject OutputObj,Imagea,Imageb;
	HTuple  ErrorCode, OutValue;
	LARGE_INTEGER  start,end,litmp;
	//1������·�� �� ����	
	HTuple hv_ProPathName;
	if (ProcedurePath.empty())
	{
		error = -1;
		hmessage = "halcon������򲻴���!";
		return false;
	}
	int DivIndex = ProcedurePath.find_last_of('\\');

	//1.1����
	char szTemp[512] ={0};
	sprintf_s(szTemp,"%s",ProcedurePath.c_str());
	szTemp[DivIndex] = '\0';

	char* pProcPath = szTemp;
	char* pChar = &(szTemp[DivIndex+1]);
	char* pProcName = strtok(pChar,".");
	//2������halcon����
	//2.1����
	HDevEngine HalEngine;
	HDevProcedure HalProc;
	HDevProcedureCall  HalProcCall;
	/*HalEngine.SetProcedurePath(pProcPath);*/
	try
	{
		HalEngine.SetProcedurePath(pProcPath);
		HalProc.LoadProcedure(pProcName);
		HalProcCall = HalProc.CreateCall();

	}
	catch (...)
	{
		hmessage = "�ļ�����ʧ��";
		error = -2;
		return false;
	}

	//2.2 �����������
	//HObject ho_Image;
	//HalconCpp::ReadImage(&ho_Image,"F:/vstest/UseHDevEngine/Debug/Test.png");
	//HalProcCall.SetInputIconicParamObject(1,m_pImage[Imageindex]);//m_pImage[Imageindex]);

	//CopyObj(ImageA, &Imagea, 1, 1);//Դ��Ŀ��,������1��ʼ,��������
	//ConcatObj(ImageA, m_pImage[Imageindex], &ImageA);//��������ͼ��
	HalProcCall.SetInputIconicParamObject("ImageA",ImageA);
	HalProcCall.SetInputIconicParamObject("ImageB",ImageB);


	int ParanNum = inparam.size();//�������
	for (int i = 0;i< ParanNum;i++)
	{
		//memset(szParamName,0,sizeof(szParamName));
		//sprintf_s(szParamName,"Input%d",i);
		double testDouble = inparam[i];

		InputTuple[i] = testDouble;
	}
	HalProcCall.SetInputCtrlParamTuple("invalue", InputTuple);
	HalProcCall.SetInputCtrlParamTuple("display", display);
	//�������
	for( int i=0;i<20;i++)
	{
		PointA[i] = arr[0][i];
		PointB[i] = arr[1][i];
	}
	HalProcCall.SetInputCtrlParamTuple("PointA", PointA);
	HalProcCall.SetInputCtrlParamTuple("PointB", PointB);
	//2.3 ����
	try
	{

		Message.Clear();
		m_vecObjA.Clear();
		m_vecObjB.Clear();

		Message = HTuple();
		m_vecObjA = HObject();
		m_vecObjB = HObject();
		m_bIsPass = false;
		QueryPerformanceFrequency(&litmp);//��ȡϵͳʱ��Ƶ��
		QueryPerformanceCounter(&start) ; 

		HalProcCall.Execute();
		ErrorCode = HalProcCall.GetOutputCtrlParamTuple("error");
		Message = HalProcCall.GetOutputCtrlParamTuple("message");
		if (0 <= ErrorCode)
		{
			OutValue = HalProcCall.GetOutputCtrlParamTuple("outValue");
			m_vecObjA = HalProcCall.GetOutputIconicParamObject("outA"); 
			m_vecObjB = HalProcCall.GetOutputIconicParamObject("outB"); 
			m_bIsPass = true;
			//����
			int outlen = OutValue.TupleLength().I();
			double outtemp = 0.0;
			for (int i = 0;i< outlen ;i++ )
			{
				outtemp = OutValue[i].D();
				outparam.push_back(outtemp);
			}
		}
		else if (0 < ErrorCode)
		{
			m_bIsPass = false;
		}
		else if (1 == display)
		{
			m_vecObjA = HalProcCall.GetOutputIconicParamObject("outA"); 
			m_vecObjB = HalProcCall.GetOutputIconicParamObject("outB");

			if (iWinIDA>=3600)
			{
				IAVH_DispImage(iWinIDA,m_vecObjA);
			}
			if (iWinIDB>=3600)
			{
				IAVH_DispImage(iWinIDB,m_vecObjB);
			}
		}
		else
		{
			hmessage =  "Halcon������ʧ��!";
			error = -3;
			res = error;
		}

		//ж�س���ÿ�ζ�̬���س����ж�س����ʱ��,���ǲ����¼��س����ֻ���֣��޸�halcon������ܷ�Ӧ����������,
		//���Բ���1������ģʽ�ͣ�2����ģʽѡ��и�ȫ�ֵĵ��Ա�־���Կ���
		HalEngine.UnloadProcedure(pProcName);
		QueryPerformanceCounter(&end) ; 
		double dTime = (double)(end.QuadPart-start.QuadPart) / (double)litmp.QuadPart;    //��ȡʱ�䵥λΪ��	
		char szBuf[125] = {0};
		sprintf_s(szBuf, ",��ʱ��%0.3lf", dTime);
		Message.Append(szBuf);
		error = 0;
		hmessage = "OK";
	}
	catch (...)
	{
		HalEngine.UnloadProcedure(pProcName);
		hmessage =  "Halcon������ʧ��!";
		error = -3;
		return false;
	}

	return res;
}