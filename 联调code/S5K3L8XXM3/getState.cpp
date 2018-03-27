#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp> 
#include "polyfit.h"
#include <iostream>
#include "AA_define_tool.h"

#include "AAalgorithm.h"

using namespace std;
using namespace cv;
using namespace czy;

extern  AA_Globle_Param g_AAParam;//����ȫ�ֱ���



/*
%get the current state of the initial image including u, v and z
%============================================
%Author:Chame & Ocean(Using polyfit.h and vector)
%Date:2017.6.5
%============================================
%Input
% ima_ori: the original image in the initial position;
% ima_p: the positive image in the initial position;
% ima_m: the minus image in the initial position;
% deltaZ_p: the distance of image moving to the plus direction;
% deltaZ_m: the distance of image moving to the minus direction;
% pixel_size: the size of CCD pixel;
% scale: the image scale's scale;
%--------------------------------------------
%Output
% deltaZ: the distance of the initial image to the focal plane;
% x_offset, y_offset: horizontal(x) offset and vertical(y) offset;
% x_angle, y_angle: horizontal(x) angle and vertical(y) angle;
%============================================
*/

Mat Img_resize(Mat I, double scale){
	if (I.channels() == 3){
		cvtColor(I, I, CV_RGB2GRAY);
	}
	int height = I.rows * scale;
	int width = I.cols * scale;
	resize(I, I, Size(width, height));
	I.convertTo(I, CV_32F);
	I = I / 255;
	return I;
}
string ConvertToString(double value)
{
	stringstream ss;
	ss << value;
	return ss.str();
}

int calZUV(Mat img_ori, Mat img_p, Mat img_m, double deltaZ_p, double deltaZ_m,int vid,State &result)
{	
	//1,��ʼ�����
	int ret = 0;
	HV_Lap hv_lap_o,hv_lap_p,hv_lap_m;
	memset(&result,0,sizeof(State));
	memset(&hv_lap_o,0,sizeof(HV_Lap));
	memset(&hv_lap_o,0,sizeof(HV_Lap));
	memset(&hv_lap_o,0,sizeof(HV_Lap));

	//2������ÿ��ͼƬ
	ret = calHarryPointsAndMTFValueInOnePic(img_ori,vid,hv_lap_o);
	if (0 != ret)
	{
		msg("��ȡvid:%d MTFʧ��!,ret = %d",vid,ret);
		return ret;
	}
	ret = calHarryPointsAndMTFValueInOnePic(img_p,vid,hv_lap_p);
	if (0 != ret)
	{
		msg("��ȡvid:%d MTFʧ��!,ret = %d",vid,ret);
		return ret;
	}
	ret = calHarryPointsAndMTFValueInOnePic(img_m,vid,hv_lap_m);
	if (0 != ret)
	{
		msg("��ȡvid:%d MTFʧ��!,ret = %d",vid,ret);
		return ret;
	}

	//3������
	//����������е�˳��Ϊ:���-�м�-�ұ�-  �ϱ� -�м�--�±�
	//ԭ��λ��
	vector<Point> x_pt_o = hv_lap_o.horizontal_pt;//ˮƽ3����
	vector<Point> y_pt_o = hv_lap_o.vertical_pt;//��ֱ3����

	vector<double> x_lap_o = hv_lap_o.horizontal_lap;//ˮƽ3��������˹
	vector<double> y_lap_o = hv_lap_o.vertical_lap;//��ֱ3��������˹

	//����λ��
	vector<double> x_lap_p  = hv_lap_p.horizontal_lap;
	vector<double> y_lap_p = hv_lap_p.vertical_lap;
	//����λ��
	vector<double> x_lap_m = hv_lap_m.horizontal_lap;
	vector<double> y_lap_m = hv_lap_m.vertical_lap;

	vector<double> Getangle_x;
	vector<double> Getangle_y;
	//����x�����ƫת�Ƕ�
	vector<double> x_pt;
	for (int i = 0; i < 3; i++)
	{
		x_pt.push_back(x_pt_o[i].x);
	}
	cout << endl << "Fitting!" << endl;
	cout << endl << "Delta_Z��" ;
	Getangle_x = getangle(x_lap_o, x_lap_p, x_lap_m, x_pt, deltaZ_p, deltaZ_m, g_AAParam.pixel_size/g_AAParam.scale);
	result.x_angle = Getangle_x[0];
	//����y�����ƫת�Ƕ�
	vector<double> y_pt;
	for (int i = 0; i < 3; i++)
	{
		y_pt.push_back(y_pt_o[i].y);
	}
	Getangle_y = getangle(y_lap_o, y_lap_p, y_lap_m, y_pt, deltaZ_p, deltaZ_m, g_AAParam.pixel_size/g_AAParam.scale);
	result.y_angle = Getangle_y[0];
	result.deltaZ = (Getangle_x[2] + Getangle_y[2])/2;
	cout << result.deltaZ ;
	return ret;

}

State getState(Mat img_ori, Mat img_p, Mat img_m, double deltaZ_p, double deltaZ_m, double pixel_size,double scale,int cameraID,int * flag)
{
	//�����趨��������Ҫ����Ų����������
	double f = 3.31;                      //��ͷ����Ϊ3.31mm
	double d_obj = 500;					  //���Ϊ600mm
	double size = 90, block = 140;        //ѡ�򴰿����ϵ��ڻ�׼������90���ش���block����Ϊ140����
	int edge_size = 15;                   //�б߿�ߴ糤��Ϊ15����
	int block_size = 19;                  //����ֵ�����ж��б�λ�óߴ糤��Ϊ19����
	float thr = pow((2 * (float)block_size + 1), 2) / 2 * 0.2;   //����ֵ�����ж�����

	int temType = g_AAParam.temPlate[cameraID];

	f= g_AAParam.focus;
	d_obj = g_AAParam.objdis;
	size =g_AAParam.size;
	block = g_AAParam.block;
	edge_size = g_AAParam.edge_size;
	block_size = g_AAParam.block_size;
	thr = pow((2 * (float)block_size + 1), 2) / 2 * 0.2;

	//��������ͼ���laplace��
	State state= {0};
	State state1 = {0};
	int Laplace_Flag_o = -1;
	int Laplace_Flag_p = -1;
	int Laplace_Flag_m = -1;
	HV_Lap hv_lap_o = getEdgeLaplace(img_ori,cameraID, &Laplace_Flag_o);
	HV_Lap hv_lap_p = getEdgeLaplace(img_p,cameraID, &Laplace_Flag_p);
	HV_Lap hv_lap_m = getEdgeLaplace(img_m, cameraID,&Laplace_Flag_m);
	if(Laplace_Flag_o<0 || Laplace_Flag_p<0 || Laplace_Flag_m<0)
	{
		*flag= AA_lAP_ERROR;
		return state1;
	}
	else
	{
		*flag = AA_lAP_OK;
	}
	//����������е�˳��Ϊ:���-�м�-�ұ�-  �ϱ� -�м�--�±�
	//ԭ��λ��
	vector<Point> x_pt_o = hv_lap_o.horizontal_pt;//ˮƽ3����
	vector<Point> y_pt_o = hv_lap_o.vertical_pt;//��ֱ3����
	vector<double> x_lap_o = hv_lap_o.horizontal_lap;//ˮƽ3��������˹
	vector<double> y_lap_o = hv_lap_o.vertical_lap;//��ֱ3��������˹

	//ǰ��λ������
	vector<double> x_lap_p  = hv_lap_p.horizontal_lap;
	vector<double> y_lap_p = hv_lap_p.vertical_lap;
	//����λ������
	vector<double> x_lap_m = hv_lap_m.horizontal_lap;
	vector<double> y_lap_m = hv_lap_m.vertical_lap;

    //�жϵ������
	if (x_lap_o[1] < x_lap_m[1] || x_lap_o[1] < x_lap_p[1]){//ˮƽ--λ��
		*flag=  AA_lAP_SIGNEL;
		if (x_lap_m[1] <=  x_lap_p[1]) {state.OneSide_flag[0] = 1;}
		if (x_lap_m[1] >  x_lap_p[1]) {state.OneSide_flag[0] = -1;}
		cout<<"x_lap_o[1]:"<<x_lap_o[1]<<"x_lap_m[1]:"<<x_lap_m[1]<<"x_lap_p[1]:"<<x_lap_p[1]<<endl;
		cout << "center_x is error" << endl;
	}
	if (y_lap_o[1] < y_lap_m[1] || y_lap_o[1] < y_lap_p[1]){//��ֱ�м�λ��
		*flag= AA_lAP_SIGNEL;
		if (y_lap_m[1] <=  y_lap_p[1]) {state.OneSide_flag[1] = 1;}
		if (y_lap_m[1] >  y_lap_p[1]) {state.OneSide_flag[1] = -1;}
		cout<<"y_lap_o[1]:"<<y_lap_o[1]<<"y_lap_m[1]:"<<y_lap_m[1]<<"y_lap_p[1]:"<<y_lap_p[1]<<endl;
		cout << "center_y is error" << endl;
	}
	if (x_lap_o[0] < x_lap_m[0] || x_lap_o[0] < x_lap_p[0]){
		*flag= AA_lAP_SIGNEL;
		if (x_lap_m[0] <=  x_lap_p[0]) {state.OneSide_flag[2] = 1;}
		if (x_lap_m[0] >  x_lap_p[0]) {state.OneSide_flag[2] = -1;}
		cout<<"x_lap_o[0]:"<<x_lap_o[0]<<"x_lap_m[0]:"<<x_lap_m[0]<<"x_lap_p[0]:"<<x_lap_p[0]<<endl;
		cout << "left is error" << endl;
	
	}
	if (x_lap_o[2] < x_lap_m[2] || x_lap_o[2] < x_lap_p[2]){
		*flag= AA_lAP_SIGNEL;
		if (x_lap_m[2] <=  x_lap_p[2]) {state.OneSide_flag[3] = 1;}
		if (x_lap_m[2] >  x_lap_p[2]) {state.OneSide_flag[3] = -1;}
		cout<<"x_lap_o[2]:"<<x_lap_o[2]<<"x_lap_m[2]:"<<x_lap_m[2]<<"x_lap_p[2]:"<<x_lap_p[2]<<endl;
		cout << "right is error" << endl;
	
	}
	if (y_lap_o[0] < y_lap_m[0] || y_lap_o[0] < y_lap_p[0]){
		*flag= AA_lAP_SIGNEL;
		if (y_lap_m[0] <=  y_lap_p[0]) {state.OneSide_flag[4] = 1;}
		if (y_lap_m[0] >  y_lap_p[0]) {state.OneSide_flag[4] = -1;}
		cout<<"y_lap_o[0]:"<<y_lap_o[0]<<"y_lap_m[0]:"<<y_lap_m[0]<<"y_lap_p[0]:"<<y_lap_p[0]<<endl;
		cout << "top is error" << endl;
	}
	if (y_lap_o[2] < y_lap_m[2] || y_lap_o[2] < y_lap_p[2]){
		*flag= AA_lAP_SIGNEL;
		if (y_lap_m[2] <=  y_lap_p[2]) {state.OneSide_flag[5] = 1;}
		if (y_lap_m[2] >  y_lap_p[2]) {state.OneSide_flag[5] = -1;}
		cout<<"y_lap_o[2]:"<<y_lap_o[2]<<"y_lap_m[2]:"<<y_lap_m[2]<<"y_lap_p[2]:"<<y_lap_p[2]<<endl;
		cout << "bottom is error" << endl;
	}
	vector<double> Getangle_x;
	vector<double> Getangle_y;
	//*flag=0;
	//����x�����ƫת�Ƕ�
	vector<double> x_pt;
	for (int i = 0; i < 3; i++)
	{
		x_pt.push_back(x_pt_o[i].x);
	}
	cout << endl << "Fitting!" << endl;
	cout << endl << "Delta_Z��" ;
	Getangle_x = getangle(x_lap_o, x_lap_p, x_lap_m, x_pt, deltaZ_p, deltaZ_m, g_AAParam.pixel_size/g_AAParam.scale);
	state.x_angle = Getangle_x[0];
	//����y�����ƫת�Ƕ�
	vector<double> y_pt;
	for (int i = 0; i < 3; i++)
	{
		y_pt.push_back(y_pt_o[i].y);
	}
	Getangle_y = getangle(y_lap_o, y_lap_p, y_lap_m, y_pt, deltaZ_p, deltaZ_m, g_AAParam.pixel_size/g_AAParam.scale);
	state.y_angle = Getangle_y[0];
	state.deltaZ = (Getangle_x[2] + Getangle_y[2])/2;
	cout << state.deltaZ ;
	return state;
}
/*���Ƕ�*/
vector<double> getangle(vector<double> lap_o, vector<double> lap_p, vector<double> lap_m, vector<double> pt_o, double deltaZ_p, double deltaZ_m, double pixel_size)
{
	vector<double> deltaZ;
	vector<double> x;   
	x.push_back(deltaZ_p); x.push_back(0); x.push_back(deltaZ_m);
	//���ƶ���Χ�ָ�Ϊ100�ݣ�������϶�������
	double dx = (deltaZ_m - deltaZ_p) / 100;
	double x_temp[101] = { 0 };
	double lap[101] = { 0 };
	for (int i = 0; i < 101; i++)
	{
		x_temp[i] = deltaZ_p + i * dx;
	}
	
	for (int i = 0; i < lap_o.size(); i++)
	{
		vector<double> y;
		czy::Fit fit;
		y.push_back(lap_p[i]);
		y.push_back(lap_o[i]);
		y.push_back(lap_m[i]);
		//��������ͼͬһλ�õ�ģ��������϶������ߣ��õ���ֵ����λ��
		fit.polyfit(x, y, 2, true);
		for (int m = 0; m < 101; m++){
			lap[m] = fit.getY(x_temp[m]);
		}
		int max_loc = maxloc(lap, 101);
		deltaZ.push_back(deltaZ_p + max_loc * dx);
	}
	for (int i = 0; i < 3; i++)
	{
		pt_o[i] = pt_o[i] * pixel_size;
	}
	double deltaZZ = deltaZ[1];   //Ϊ�˷������ĵ�deltaZ
	deltaZ[1] = 2 * deltaZ[1]; //Ϊ��*2?
	
	//�����뽹����ϱ����б��һ�κ���
	czy::Fit fit_l;
	fit_l.linearFit(pt_o, deltaZ);
	//������ϵõ���б�ʵõ���б�Ƕȣ���λΪ��
	double angle = atan(fit_l.getFactor(1)) / 3.14159265 * 180;
	vector<double> angleAZ;
	angleAZ.push_back(angle);
	angleAZ.push_back(deltaZ[0]);
	angleAZ.push_back(deltaZZ);
	angleAZ.push_back(deltaZ[2]);
	return angleAZ;
}

/*
�õ����������ֵ��λ��
*/
int maxloc(double a[], int size)
{
	double max = 0;
	int maxloc = 0;
	for (int i = 0; i < size; i++)
	{
		if (max < a[i])
		{
			max = a[i];
			maxloc = i;
		}
	}
	return maxloc;
}

int calHarryPointsAndMTFValueInOnePic(Mat img,int vid,HV_Lap &result)
{
	int ret = 0;
	Point edgePoint;
	double lapVaule = 0.0;

	ret = calEdgePointInRegion(img,LEFT,vid,edgePoint,lapVaule);
	if (!ret)
	{	
		result.horizontal_lap.push_back(lapVaule);
		result.horizontal_pt.push_back(edgePoint);
	}
	else
	{
		ret= -1;
		msg("��ȡͨ��%d������:%d ʧ��",vid,LEFT);
		return ret;
	}


	ret = calEdgePointInRegion(img,CENTERX,vid,edgePoint,lapVaule);
	if (!ret)
	{	
		result.horizontal_lap.push_back(lapVaule);
		result.horizontal_pt.push_back(edgePoint);
	}
	else
	{
		ret= -1;
		msg("��ȡͨ��%d������:%d ʧ��",vid,CENTERX);
		return ret;
	}

	ret = calEdgePointInRegion(img,RIGHT,vid,edgePoint,lapVaule);
	if (!ret)
	{	
		result.horizontal_lap.push_back(lapVaule);
		result.horizontal_pt.push_back(edgePoint);
	}
	else
	{
		ret= -1;
		msg("��ȡͨ��%d������:%d ʧ��",vid,RIGHT);
		return ret;
	}

	ret = calEdgePointInRegion(img,TOP,vid,edgePoint,lapVaule);
	if (!ret)
	{	
		result.vertical_lap.push_back(lapVaule);
		result.vertical_pt.push_back(edgePoint);
	}
	else
	{
		ret= -1;
		msg("��ȡͨ��%d������:%d ʧ��",vid,TOP);
		return ret;
	}


	ret = calEdgePointInRegion(img,CENTERY,vid,edgePoint,lapVaule);
	if (!ret)
	{	
		result.vertical_lap.push_back(lapVaule);
		result.vertical_pt.push_back(edgePoint);
	}
	else
	{
		ret= -1;
		msg("��ȡͨ��%d������:%d ʧ��",vid,CENTERY);
		return ret;
	}

	ret = calEdgePointInRegion(img,BOTTOM,vid,edgePoint,lapVaule);
	if (!ret)
	{	
		result.vertical_lap.push_back(lapVaule);
		result.vertical_pt.push_back(edgePoint);
	}
	else
	{
		ret= -1;
		msg("��ȡͨ��%d������:%d ʧ��",vid,BOTTOM);
		return ret;
	}
	return ret;
}

int calEdgePointInRegion(Mat img,int vid,int regiontype,Point &edgePoint,double &lapVaule)
{
	int ret = 0;

	double f= g_AAParam.focus;
	double d_obj = g_AAParam.objdis;
	int size =g_AAParam.size;
	int block = g_AAParam.block;
	int edge_size = g_AAParam.edge_size;
	int block_size = g_AAParam.block_size;
	double thr = pow((2 * (float)block_size + 1), 2) / 2 * 0.2;
	double scale = g_AAParam.scale;	
	double d_img = 1 / (1 / g_AAParam.focus - 1 / g_AAParam.objdis);
	double beta = d_img / g_AAParam.objdis; 
	double pixel_size = g_AAParam.pixel_size/g_AAParam.scale;


	//����ģ��:(ԭ��:ѡ�������Ϊ��0,���ϵ�(�ϰ��º�,����Һ�),1,���ϵ�(�ϰ��º�,����Ұ�)��
	//2,���µ�(�Ϻ��°�,����Һ�),3,���µ�(�Ϻ��°�,����Һ�)

	int temPlate = g_AAParam.temPlate[vid];
	Mat h_x = Mat::ones(g_AAParam.block_size * 2 + 1, g_AAParam.block_size * 2 + 1, CV_32F);
	Mat h_y = Mat::ones(g_AAParam.block_size * 2 + 1, g_AAParam.block_size * 2 + 1, CV_32F);
	SelectTemplateType(temPlate,block_size,  h_x,h_y);//ѡ��ģ������
	//�Ƿ�ģ�巴��:����ģ�� ȡˮƽ�㣬����ģ��ȡ��ֱ�㣬����û��Ӱ��
	Mat temp;
	temp = h_x;
	h_x = h_y;
	h_y = temp;
	savePicture(h_x,"ˮƽ����ģ��.jpg");
	savePicture(h_y,"��ֱ����ģ��.jpg");

	//Center�б�ѡȡ�����
	double col ,row;
	//��ͼ���Ĳ�һ��������,����޷���׼��ͼ�������⣬ע��
	col = g_AAParam.paintCenter[vid].X*g_AAParam.scale,row = g_AAParam.paintCenter[vid].Y  * g_AAParam.scale;
	Mat value;
	int x_center_flag=0;
	int x =0,y= 0;
	double i_left = ceil(col - g_AAParam.mapdislength * beta / pixel_size); //����ÿ��ͼ��˳��һ��
	double i_right = ceil(col + g_AAParam.mapdislength * beta / pixel_size);
	double j_top = ceil(row - g_AAParam.mapdiswidth * beta / pixel_size);
	double  j_bottom = ceil(row + g_AAParam.mapdiswidth * beta / pixel_size);

	//����: ��������������ȷ���������ҵ�����
	//AA_TOP,
	//	AA_BOTTOM,
	//	AA_CENTER,
	//	AA_LEFT,
	//	AA_RIGHT,
	//	AA_LEFT_LOC,
	//	AA_RIGHT_LOC,

	switch(regiontype)
	{
	case CENTERX:
		break;
	case CENTERY: //��ʱ��AA_LEFT_LOCӳ��Ϊ��ֱ�����y
		h_x = h_y;
		break;
	case LEFT:
		col = ceil(col - g_AAParam.mapdislength * beta / pixel_size);
		break;
	case RIGHT:
		col = ceil(col + g_AAParam.mapdislength * beta / pixel_size);
		break;
	case TOP:
		h_x = h_y;
		row = ceil(row - g_AAParam.mapdiswidth * beta / pixel_size);
		break;
	case BOTTOM:
		h_x = h_y;
		row = ceil(row + g_AAParam.mapdiswidth * beta / pixel_size);
		break;
	}
	//��ͼ����ֵ�˲�
	medianBlur(img, img, 3);

	//int x = g_AAParam.channel1Retangle[vid][regiontype].lty* g_AAParam.scale +20;
	//int y = g_AAParam.channel1Retangle[vid][regiontype].ltx* g_AAParam.scale;
	//int rheight = g_AAParam.channel1Retangle[vid][regiontype].rbx* g_AAParam.scale - g_AAParam.channel1Retangle[vid][regiontype].ltx* g_AAParam.scale;
	//int rwid = g_AAParam.channel1Retangle[vid][regiontype].rby * g_AAParam.scale- g_AAParam.channel1Retangle[vid][regiontype].lty* g_AAParam.scale;

	IAVO_ShowPicture("�˲�ǰ����ͼ",img);

	IAVO_SavePicture("./image//�˲�ǰ����ͼ.jpg",img);

	//filter2D(img(cvRect(x, y, rwid, rheight)), value, img.depth(), temPlate, Point(-1, -1), (0, 0), BORDER_REFLECT);
	//filter2D(img(cvRect(x, y, rwid, rheight)), value, img.depth(), h_x, Point(-1, -1), (0, 0), BORDER_REFLECT);
	getFilterValue(temPlate,img,h_x,col,row,size,block,value,regiontype);

	//Mat blockimg = img(cvRect(col, row, block, block));
	//int a1 = blockimg.rows;
	//a1 = blockimg.cols;

	//Mat sizeimg = img(cvRect(col, row, block, block));
	//a1 = sizeimg.rows;
	//a1 = sizeimg.cols;

	//IAVO_ShowPicture("�˲�ǰ",blockimg);

	
	IAVO_SavePicture("./image//value.jpg",value);
	//getFilterValue(temPlate,img,h_x,i_center,j_center,size,block,value_x,regiontype);
	IAVO_SavePicture("./image//�˲�ǰ.jpg",img(cvRect(x, y, block, block)));
	for (int j = block_size; j < block - block_size; j++)
	{
		float* data_x = value.ptr<float>(j);
		for (int i = block_size ; i < block - block_size ; i++)
		{
			double p = data_x[i];
			Point maxP,minP;
			double maxVal,minVal;
			minMaxLoc(value, &minVal, &maxVal, &minP, &maxP);
			if (i==110)
			{
				p = data_x[i];
			}
			if (data_x[i] > thr)
			{
				Mat roi(value, Rect(i - block_size, j - block_size, 2 * block_size + 1, 2 * block_size + 1));
				Point maxPoint,maxPoint_Img;
				//IAVO_ShowPicture("test",roi);
				
				int a = roi.channels();
				minMaxLoc(roi, 0, 0, 0, &maxPoint);
				maxPoint.x = maxPoint.x + i - block_size;
				maxPoint.y = maxPoint.y + j - block_size;
				if ( i == maxPoint.x && j == maxPoint.y ){
					maxPoint_Img.x = i + col - size;
					maxPoint_Img.y = j + row - size;
					edgePoint = (maxPoint_Img);
					x_center_flag=1;

				}

			}
		}
	}

	Mat imgpaint = img.clone();
	circle(imgpaint, edgePoint, 10, cvScalar(255, 0, 0), 2);
	//����ͼƬ���ı�ԭ����ͼ��
	string filename = "�бߵ�_";
	char buf[64] ={0};
	sprintf(buf,"%d_%d",vid,regiontype);
	filename.append(buf);
	filename.append(".jpg") ;
	IAVO_SavePicture(filename,imgpaint);
	//IAVO_ShowPicture("ԭͼ",img);

	IAVO_ShowPicture("�бߵ�",imgpaint);
	//����������˹ֵ
	lapVaule = laplace(img, edgePoint, edge_size);
	return ret;
}