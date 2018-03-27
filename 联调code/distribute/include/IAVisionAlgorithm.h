/************************************************************************
����:�Ӿ��㷨ҵ��ӿ�
����:����OPENCV HALCON USERDEFINE�Ȼ����㷨��Ϊ��Ϊͨ�õ�ҵ���㷨��������
	 AA�������豸���㷨����
˵��:������������,�ӿھ�����ͨ�á�
IAVO_ ��ʶOpencv�ӿ�
IAVH_ ��ʶHalcon�ӿ�
IAVU_ ��ʶ�Զ���ӿ�

************************************************************************/
#ifndef _I_IAVISIONALGORITHM__H_
#define _I_IAVISIONALGORITHM__H_

#ifdef IAVISIONALGORITHM_EXPORTS
#define IAVISION_API __declspec(dllexport)
#else
#define IAVISION_API __declspec(dllimport)
#endif

#include <opencv2\opencv.hpp>
using namespace cv;

#include <HalconCpp.h>
using namespace HalconCpp;


#include "HDevEngineCpp.h" //�Զ�����ļ�����halcon���Ƿ����������
using namespace HDevEngineCpp;

#ifndef WINAPI
#define WINAPI __stdcall
#endif



#define IAV_PRINTF(logmsg, ...) printf(logmsg, __VA_ARGS__)


typedef enum _IAV_ERROR_CODE
{
	         
	IAV_ERROR =-1 ,
	IAV_OK = 0,                       
}IAV_ERROR_CODE;

//ROI����
typedef enum _IAVH_ROI_TYPE
{
	IAVH_RETANGLE = 0,         
	IAVH_CIRCLE ,
	IAVH_INVALID
}IAVH_ROI_TYPE;
//ROI ����1�ṹ
typedef struct _IAVH_ROI_RETANGLE1{
	double Row1;
	double Column1;
	double Row2;
	double Column2;

}PIAVH_ROI_RETANGLE1,*PIAVH_RETANGLE1;
//ROI Բ1�ṹ
typedef struct _IAVH_ROI_CIRCLE{
	double Row;
	double Column;
	double rad;

}PIAVH_ROI_CIRCLE,*PIAVH_CIRCLE;

//ROI����
typedef struct _IAVH_ROI_STRUCT{
	IAVH_ROI_TYPE type;
	union {  
		PIAVH_ROI_RETANGLE1 rect;  
		PIAVH_ROI_CIRCLE cir;
	};
}IAVH_ROI_STRUCT,*PIAVH_ROI_STRUCT;




//HALCON���� 
#define IAVH_PARAM_LEN 32
//��״����
typedef struct _SelectShapePARAM{
	char Features[IAVH_PARAM_LEN];
	char Operation[IAVH_PARAM_LEN];
	double min;
	double max;
	_SelectShapePARAM()
	{
		strcpy_s(Features,"area");
		strcpy_s(Operation,"and");

		//Operation= "and";
		min = 1000;
		max = 2000;
	}
}SelectShapePARAM,*PSelectShapePARAM;


//�Զ���ֵ����
typedef struct _BinaryThresholdPARAM{
	char Method[IAVH_PARAM_LEN];
	char LightDark[IAVH_PARAM_LEN];
	_BinaryThresholdPARAM()
	{
		strcpy_s(Method,"max_separability");
		strcpy_s(LightDark,"dark");
	}
}BinaryThresholdPARAM,*PBinaryThresholdPARAM;

//�Զ���ֵ����
typedef struct _ScaleImagePARAM{
	double add;
	double mul;
	_ScaleImagePARAM()
	{
		add = 0;
		mul = 1;
	}
}ScaleImagePARAM,*PScaleImagePARAM;



typedef struct _IAVH_PARAM_STRUCT{
	SelectShapePARAM selectShape;  
	BinaryThresholdPARAM binarythreshold;
	ScaleImagePARAM scaleImage;


}IAVH_PARAM_STRUCT,*PIAVH_PARAM_STRUCT;




//���߽ӿ� Opencv
/************************************************************************
����:	��opencv������ʾͼ��
����:	��ʾͼ�����: Mat &img
		��������:     const string windowname
���:	��
����ֵ:	�ɹ�
˵��:	��ʾ���������������,������
*************************************************************************/
IAVISION_API int IAVO_ShowPicture(const string windowname,Mat &img);
IAVISION_API int IAVO_SavePicture(string filename,Mat &img);
IAVISION_API Mat IAVO_ImgResize(Mat& img, double scale);
IAVISION_API int IAVO_ReadPicture(string filename,Mat &img);

/************************************************************************
����:	��opencv������ʾͼ��
����:	��ʾͼ�����: Mat &img
		��������:     const string windowname
���:	��
����ֵ:	�ɹ�
˵��:	��ʾ���������������,������
*************************************************************************/
IAVISION_API int IAVO_ShowWindow(const string windowname,Mat &img,int windoww =0,int windowh = 0);

//IAVISION_API int IAV_ADD(int a,int b);
//IAVISION_API int IAV_ADD(float a,float b);


//AA�㷨 Opencv
/************************************************************************
����:�Ӿ��㷨ҵ��ӿ�
����:����OPENCV HALCON USERDEFINE�Ȼ����㷨��Ϊ��Ϊͨ�õ�ҵ���㷨��������
	 AA�������豸���㷨����
************************************************************************/
IAVISION_API vector<Point2d> IAV_GetCornerInOnePic(Mat &B);



//���߽ӿ� halcon
/************************************************************************
����:	��halcon������ʾͼ��
����:	��ʾͼ�����: HObject& Object
���:	��
����ֵ:	�ɹ�0
˵��:	��ʾ���������������,������,������
		�˺������Ի�̫ǿ��,�����Ȳ����ų�ȥ��������Ҫ���ٿ���
*************************************************************************/
IAVISION_API int IAVH_ShowPicture(HObject& Object);

/************************************************************************
����:	��halcon�󶨵Ĵ�����ʾͼ��
����:	��ʾͼ�����: ����halconid,HObject& Object
���:	��
����ֵ:	�ɹ�0
˵��:	��ʾ���������������,������,������
	
*************************************************************************/
IAVISION_API int IAVH_DispImage(int hwinID,HObject& Object);

/************************************************************************
����:	��ȡͼ��
����:	��ʾͼ�����: HObject& Object
���:	��
����ֵ:	�ɹ�0
˵��:	
	
*************************************************************************/
IAVISION_API int IAVH_ReadImage(HObject& Object,char *filename);


/************************************************************************
����:	��halcon�󶨵Ĵ�����ʾָ��λ�û�ʮ�ֲ�
����:	����halconid,
		double* pt
		double size,
		double angle
���:	��
����ֵ:	�ɹ�0
˵��:	
	
*************************************************************************/
IAVISION_API int IAVH_PaintCross(int hwinID,double* pt,double size = 6.0,double angle = 0.0);


/************************************************************************
����:	��ָ���Ĵ������������ƾ��ο�
����:	���ھ��             : int WindowHandle

���:	���ο����Ͻǵ�Row����: double Row1
		���ο����Ͻǵ�Col����: double Column1
		���ο����½ǵ�Row����: double Row2
		���ο����½ǵ�Col����: double Column2

����ֵ:	�ɹ�0
˵��:	��������ȷ�����ε�һ���ǣ������γɾ��ο��ڴ������ε��м��ٵ������󣬿����ƶ�����
        ����ӽ�һ�ߡ�ץס�������޸ľ��εĳߴ��ڴ�ֱ���������ˮƽ����
*************************************************************************/
IAVISION_API int IAVH_DrawRectangle1(int WindowHandle,double *Row1, double *Column1, double *Row2, double *Column2);

/************************************************************************
����:	����ͼ��
����:	ͼ�����: HObject& Object
���:	��
����ֵ:	�ɹ�0
˵��:	���ṩ����·�����ļ�������׺,��.//iamge//1.jpg,������,������
		��ʱ�ṩ.jpg��,bmp���ָ�ʽ
*************************************************************************/
IAVISION_API int IAVH_SavePicture(HObject& Object,string filename);

/************************************************************************
����:	����ͼ��
����:	ͼ�����: unsigned char* buff
���:	��
����ֵ:	�ɹ�0
˵��:	���ṩ����·�����ļ�������׺,��.//iamge//1.jpg,������,������
		��ʱ�ṩ.jpg��,bmp���ָ�ʽ
*************************************************************************/
IAVISION_API int IAVH_SavePictureInByte(unsigned char* buff,string filename,int w,int h,int chNum);


/************************************************************************
����:	���洰��ͼ��
����:	ͼ�����: HObject& Object
���:	��
����ֵ:	�ɹ�
˵��:	���ṩ����·�����ļ�������׺,��.//iamge//1.jpg,������,������
		��ʱ�ṩ.jpg��,bmp���ָ�ʽ
*************************************************************************/
IAVISION_API int IAVH_SaveWindowPicture(long winID,string filename);

/************************************************************************
����:	����ͼ��
����:	ͼ�����: HObject& Object
		���ͼ��: HObject& OutObject
		�Ƿ��Ǳ�������: bool isfactor ture,��w,h��ʶ����ϵ��������Ϊ���ź�ĳ��ȺͿ��
		ͼ��������ߴ�С:double w,double h
		���Ų�ֵ����: interp ="constant"
���:	��
����ֵ:	�ɹ�0
˵��:	���ṩ����·�����ļ�������׺,��.//iamge//1.jpg,������,������
		��ʱ�ṩ.jpg��,bmp���ָ�ʽ
*************************************************************************/
IAVISION_API int IAVH_ImgResize(HObject& Object,HObject& OutObject, double w,double h,bool isfactor = true,string  interp ="constant");

/************************************************************************
����:	��ȡͼ�����ROI
����:	ͼ�����: HObject& Object
		���ROI: HObject& OutObject
		ROI����: 0���Σ�1,Բ
		�Ƿ���ʾ����(������):bool showWid
���:	��
����ֵ:	�ɹ�0
˵��:	���ṩ����·�����ļ�������׺,��.//iamge//1.jpg,������,������
		��ʱ�ṩ.jpg��,bmp���ָ�ʽ
*************************************************************************/
IAVISION_API int IAVH_GetImgRetangleROI(HObject& Object,HObject& OutObject, IAVH_ROI_STRUCT roiP,bool showWid);

//AA�㷨  halcon
/************************************************************************
����:	��byte��ͼ��ת��ΪHOject��ʽ��ͼ��
����:	ͼ��ָ��: unsigned char * pBmp
		ͼ��ߴ�: int w,int h
		ͨ��������int chNum
���:	���ROI:  HObject& Object
����ֵ:	�ɹ�0
˵��:	��ʱֻ֧�ֵ�/3ͨ��ͼ��
*************************************************************************/
IAVISION_API int IAVH_ImgByte2HObject(HObject &Object, unsigned char * pBmp,int w,int h,int chNum = 1);


/************************************************************************
����:	��ȡͼ�����ROI��һ��ͼ�������
����:	ͼ�����: HObject& Object
		����:     IAVH_PARAM_STRUCT param 
		�Ƿ���ʾ����(������):bool showWid
���:	���: double* center
����ֵ:	�ɹ�0
˵��:	
*************************************************************************/
IAVISION_API int IAVH_GetImgROICenterByArea(HObject& Object,double* center,IAVH_PARAM_STRUCT param,bool showWid);

/************************************************************************
����:	��ȡͼ�����ROI��������ͨ��������
����:	ͼ�����: HObject& Object
		����:     IAVH_PARAM_STRUCT param 
		�Ƿ���ʾ����(������):bool showWid
���:	���: double* center
����ֵ:	�ɹ�0
˵��:	
*************************************************************************/
IAVISION_API int IAVH_GetImgROIAreas(HObject& Object,vector<double>&area,IAVH_PARAM_STRUCT param,bool showWid);

/************************************************************************
����:	��ȡ˫������ĽǶ�u v w
����:	һ��˫���5���ǵ�: vector<Point> pointA
		һ��˫���5���ǵ�: vector<Point> pointB
		������ͷ�﷽���(mm): double Dis
		һ�����صĴ�С: double pixel_size
		��ͷ����: double f
		��ͷ���: double D
		ͼ�������: rows
		ͼ�������: cols

���:	�����ĽǶ�u v w: double u v w
����ֵ:	�ɹ�0
˵��:	
*************************************************************************/
IAVISION_API int IAVH_GetPara(	vector<Point> pointA,
								vector<Point> pointB,
								double Dis, 
								double pixel_size, 
								double f, 
								double D, 
								int rows,
								int cols,
								double u,
								double v,
								double w         );


/************************************************************************
����:	��ֵ�˲���ȥ����������
����:	ԭ�Ҷ�ͼ��      : HObject &HOBJECT
		�˲�������״    : char* MaskType
		�˲����ڵĴ�С  : int Radius
		��Ե����ģʽ    : char* Margin

���:	�˲����ͼ��    : HObject &OutHobject
����ֵ:	�ɹ�0
˵��:	��������״��������Ե����ģʽ�����й̶����Ϳ�ѡ��
         ������״: "circle"��"square"ֻ������ģʽ
		 ��Ե����ģʽ: "continued"��"cyclic"��"mirrored"
		 "continued": ʹ�ñ�Ե�������
		 "cyclic"   : ʹ�ñ߽�ֵѭ�����
		 "mirrored":  �߽�����ʹ�þ���Գ�
*************************************************************************/
IAVISION_API int IAVH_Median_Filter(HObject &Hobject, char* MaskType, int Radius, char* Margin, HObject &OutHobject);


/************************************************************************
����:	����halcon����������
����:	����id,long iWinIDA,iWinIDB
		ͼ�����: HObject& ImageA  HObject& ImageB
		ͼƬ��ROI���ο�����: double arr[][] 
		·����string ProcedurePath
		�������:    vector<double> &inparam
		�Ƿ���ʾ:int display
���:	vector<double> &outparam
		������:int error
		������Ϣ��hmessage
����ֵ:	�ɹ�0
˵��:	
*************************************************************************/
IAVISION_API int IAVH_AAProcedureProcess(long iWinIDA,long iWinIDB,HObject& ImageA,HObject& ImageB,double arr[][7*4],string ProcedurePath,vector<double> &inparam,vector<double> &outparam,int display,int &error,string &hmessage);


/************************************************************************
����:	��ȡͼ�����ROI��������ͨ������ұ�����ǵ�
����:	ͼ�����: HObject& Object
		����:     IAVH_PARAM_STRUCT param 
		�Ƿ���ʾ����(������):bool showWid
���:	���: double* center
����ֵ:	�ɹ�0
˵��:	
*************************************************************************/
IAVISION_API int IAVH_GetImgROIRightHarryPoint(HObject& Object,vector<double>&point,IAVH_PARAM_STRUCT param,bool showWid);




///����Ϊ�Զ���ģ��
/************************************************************************
����:	Hobject---->Mat
����:
		ͼ�����: HObject& Hobj
���:	
����ֵ:	�ɹ�Mat
˵��:	
*************************************************************************/
IAVISION_API Mat HImageToMat(HObject &Hobj)  ;


/************************************************************************
����:	Mat ---->Hobject
����:
		ͼ�����: HObject& Hobj
���:	
����ֵ:	�ɹ�HObject
˵��:	
*************************************************************************/
IAVISION_API HObject MatToHImage(Mat& pImage)  ;

#endif 