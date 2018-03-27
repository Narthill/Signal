

#include <iostream>
#include <opencv2\opencv.hpp>
#include "math.h"
#include <numeric>

#include "harris.h"
#include "math.h"

#include "AAalgorithm.h"

#include "AA_define_tool.h"

#include"Config.h"

//#include "IAVisionAlgorithm.h"
#include "AA_pic_param.h"

using namespace cv;
using namespace std;

extern  AA_Globle_Param g_AAParam;//����ȫ�ֱ���
extern AA_Globle_Pic_Param g_AA_PicParam;
string BimageName = "B";
string IimageName = "I";
char mycount = 'a';

/*
%calPara()
%============================================
%Author:Ocean
%Date:2017.6.15
%============================================
%Nargin Set
% I:The unfixed image;
% B:The fixed image;
% pixel size: set as 1.12��m;
% Dis:The distance of the two cameras;
% f: The focus of the camera;
% D: The objective distance;
%--------------------------------------------
%Output
% adjust_u/v/w
%============================================
*/
//��ֽ����4����ķ��������ķ��������̶�ͼB���ƶ�ͼ������ֵ�����࣬��࣬���Ϊ u,v��w;
Para calPara(Mat B, Mat I, double Dis, double pixel_size, double f, double D){
	vector<Point> PointI, PointB;
	Para Para;
	PointI = getCorner(I);
	PointB = getCorner(B);
	double d = 1 / (1 / f - 1 / D);
	double dis = Dis * d / D;
	double dis_pixel = dis / pixel_size;
	//B�Ƕ�
	double kb1 = (double)(PointB[1].y - PointB[2].y) / (double)(PointB[1].x - PointB[2].x);
	double kb2 = (double)(PointB[3].x - PointB[4].x) / (double)(PointB[3].y - PointB[4].y) * -1;
	double angleB1 = atan(kb1) / 3.1415926 * 180;
	double angleB2 = atan(kb2) / 3.1415926 * 180;
	//I�Ƕ�
	double kI1 = (double)(PointI[1].y - PointI[2].y) / (double)(PointI[1].x - PointI[2].x);
	double kI2 = (double)(PointI[3].x - PointI[4].x) / (double)(PointI[3].y - PointI[4].y) * -1;
	double angleI1 = atan(kI1) / 3.1415926 * 180;
	double angleI2 = atan(kI2) / 3.1415926 * 180;
	Para.w = ((angleI1 - angleB1) + (angleI2 - angleB2))/2;
	//����uv
	Point centerB, centerI; 
	centerB.x = 0; centerB.y = 0;
	centerI.x = 0; centerI.y = 0;
	for (int i = 0; i < PointB.size(); i++){
		centerB.x = centerB.x + PointB[i].x;
		centerB.y = centerB.y + PointB[i].y;
		centerI.x = centerI.x + PointI[i].x;
		centerI.y = centerI.y + PointI[i].y;
	}
	centerB.x = centerB.x / PointB.size();
	centerB.y = centerB.y / PointB.size();
	centerI.x = centerI.x / PointI.size();
	centerI.y = centerI.y / PointI.size();
	int delta_x = centerI.x - centerB.x - dis_pixel;
	int delta_y = centerI.y - centerB.y;
	Para.v = (delta_y * pixel_size / f) / 3.1415926 * 180;
	Para.u = (delta_x * pixel_size / f) / 3.1415926 * 180;
	return Para;
}



/*
%getCorner()
%���ķ�
%============================================
%Author:Ocean
%Date:2017.6.15
%============================================
%Nargin Set
% I:The unfixed image;
% B:The fixed image;
%--------------------------------------------
%Output
% Points of I and B;
%============================================
*/
//�����ĵ÷�������Դ�������ĵ÷�����ʲô��ͬ
vector<Point> getCorner(Mat I){
	vector<Point> PointI;


	//ѡ���ʼλ�����ã�CAD������ͼ���ͨ����ʽ���㣬���ϵ�λ��  �����32.5cm �̼��22cm
	Point center, left, right, top, bottom;
	center.x = 1800; center.y = 1400;
	left.x = 200; left.y = 1400;
	right.x = 3500; right.y = 1400;
	top.x = 1800; top.y = 300;
	bottom.x = 1800; bottom.y = 2500;
	//�����趨
	double size = 500;//ѡ���С
	double Centroidthreshold =0.5; //������ֵ,��0.3-->0.5

	double scale = g_AAParam.doubleAAscale;	
	double pixel_size = g_AAParam.pixel_size/g_AAParam.doubleAAscale;
	double d_img = 1 / (1 / g_AAParam.focus - 1 / g_AAParam.objdis);
	double beta = d_img / g_AAParam.objdis; //�Ŵ���      ���/���


	center.x = g_AAParam.doubleAApiccenterX*scale;
	center.y = g_AAParam.doubleAApiccenterY*scale;
	//center.x = g_AAParam.pic_lenth / 2 * scale, center.y = g_AAParam.pic_width / 2 * scale;
	left.x= ceil(center.x - g_AAParam.doubleAAmapdislength * beta / pixel_size);
	right.x = ceil(center.x + g_AAParam.doubleAAmapdiswidth * beta / pixel_size);
	left.y =  right.y = center.y;

	top.y = ceil(center.y - g_AAParam.mapdiswidth * beta / pixel_size);
	bottom.y = ceil(center.y + g_AAParam.mapdiswidth * beta / pixel_size);
	top.x =bottom.x = center.x ;
	//???
	//center.x = 1800; center.y = 1200;
	//left.x = 200; left.y = 1200;
	//right.x = 3500; right.y = 1200;
	//top.x = 1800; top.y = 100;
	//bottom.x = 1800; bottom.y = 2300;
	size = g_AAParam.doubleAAsize;
	Centroidthreshold = g_AAParam.doubleAACentroidthreshold;


	vector<Point> loc;
	loc.push_back(center); loc.push_back(left); loc.push_back(right); loc.push_back(top); loc.push_back(bottom);

	//ѡ�����������
	vector<Mat> I_block;
	for (int i = 0; i < 5; i++){
		I_block.push_back(I(Rect(loc[i].x, loc[i].y, size, size)));
		/*
		CvPoint pt,pt2;
		pt.x = loc[i].x;
		pt.y = loc[i].y;
		pt2.x = loc[i].x +size;
		pt2.y = loc[i].y +size;
	    rectangle(I,pt,pt2,cvScalar(255, 0, 0),1,8,0);
		*/
	}
	/*
	Mat d;
	resize(I, d, Size(1600, 1200));
	imshow("d",d);
	*/
	
	//����õ�fixͼ������
	double max_thx = 0;
	double min_thx = size * 255;
	double max_thy = 0;
	double min_thy = size * 255;
	double th_x, th_y;
	//����õ�ͼ��������������
	for (int i = 0; i < 5; i++){
		Point temp, temp1;
		temp1.x = 1; temp1.y = 1;
		double sum_x[500], sum_y[500];          //��Ҫ����size��ֵ�趨һ��
		vector<double> x_th, x_thh, y_th, y_thh;
		//������
		for (int j = 0; j < size; j++){
			sum_x[j] = 0;
			sum_y[j] = 0;
			for (int k = 0; k < size; k++){
				sum_x[j] = sum_x[j] + I_block[i].at<float>(j, k);
				sum_y[j] = sum_y[j] + I_block[i].at<float>(k, j);
			}
			if (sum_x[j] > max_thx){
				max_thx = sum_x[j];
			}
			if (sum_x[j] < min_thx){
				min_thx = sum_x[j];
			}
			if (sum_y[j] > max_thy){
				max_thy = sum_y[j];
			}
			if (sum_y[j] < min_thy){
				min_thy = sum_y[j];
			}
		}
		th_x = (max_thx - min_thx) * Centroidthreshold + min_thx;
		th_y = (max_thy - min_thy) * Centroidthreshold + min_thy;
		for (int j = 0; j < size; j++){
			if (sum_x[j] > th_x){
				x_th.push_back(sum_x[j]);
				x_thh.push_back(sum_x[j] * j);
			}
			if (sum_y[j] > th_y){
				y_th.push_back(sum_y[j]);
				y_thh.push_back(sum_y[j] * j);
			}
		}
		double sum_xth = accumulate(x_th.begin(), x_th.end(), 0);
		double sum_xhh = accumulate(x_thh.begin(), x_thh.end(), 0);
		double sum_yth = accumulate(y_th.begin(), y_th.end(), 0);
		double sum_yhh = accumulate(y_thh.begin(), y_thh.end(), 0);
		temp.y = sum_xhh / sum_xth;
		temp.x = sum_yhh / sum_yth;
		PointI.push_back(temp + loc[i] - temp1);
		circle(I, PointI[i],3,Scalar(255,0,0),-1);
		//msg("����:%d,%d,%d,%")
	}
	
	

	Mat d;
	resize(I, d, Size(1600, 1200));
	imshow("d",d);
	
	return PointI;
}


//�ڶ��ַ�������ȡ�ǵ㷨���ں��ʷ�Χ���ҽǵ㣬Ȼ��Ƚ�����ͼ�Ľǵ�

Points getCorner2(Mat B, Mat I){
	//Harris���ǵ�
	Points Points;
	harris HarrisB, HarrisI;
	vector<Point> TempB, TempI, PointB, PointI;
	HarrisB.detect(B);
	HarrisI.detect(I);
	HarrisB.getCorners(TempB, 0.01);
	HarrisI.getCorners(TempI, 0.01);
	//ɸѡ���½ǽǵ�
	for (int i = 0; i < TempB.size(); i++){
		float a = B.at<float>(TempB[i].y-10 , TempB[i].x);
		if (TempB[i].x == 323 && TempB[i].y==28 )
		{
			cout<<a<<","<<endl;
		}
		
		if (B.at<float>(TempB[i].y - 10, TempB[i].x) < 0.20 )//ԭ��Ϊ0.15 x 255
		{
			float b = B.at<float>(TempB[i].y - 10, TempB[i].x);
			PointB.push_back(TempB[i]);
			//cout <<TempB[i] << endl;
		}
	}
	//&& B.at<float>(TempB[i].y, TempB[i].x - 10) > 0.5 && B.at<float>(TempB[i].y, TempB[i].x + 10) > 0.5
	PointI.clear();
	for (int i = 0; i < TempI.size(); i++){
		float a = I.at<float>(TempI[i].y-10 , TempI[i].x);
		if (I.at<float>(TempI[i].y -10, TempI[i].x) < 0.25){
			PointI.push_back(TempI[i]);
			//cout <<TempI[i] << endl;
		}
	}
	if (PointB.size() != 5 || PointI.size() != 5){
		cout << "Default in finding points��" << endl;
		cout << "Default in finding points��" << endl;
		cout << "Default in finding points��" << endl;
	}
	/*
	Point P; P.x = 2; P.y = 2;
	for (int i = 0; i < PointI.size(); i++){
	rectangle(B, PointB[i], PointB[i] + P, cvScalar(255, 0, 0), 2);
	}
	imshow("123", B);
	waitKey(0);
	system("pause");
	*/
	Points.PointB = PointB;
	Points.PointI = PointI;
	return Points;
}
Para calPara2(Mat B, Mat I, double Dis, double pixel_size, double f, double D)
{
	Points Points;
	Para Para;
	Points = getCorner2(B, I);
	vector<Point> PointB = Points.PointB;
	vector<Point> PointI = Points.PointI;
	Point Btl, Btr, Bbl, Bbr, Bc, Itl, Itr, Ibl, Ibr, Ic;
	double d = 1 / (1 / f - 1 / D);
	double dis = Dis * d / D;
	double dis_pixel = dis / pixel_size;
	//�ǵ����
	for (int i = 0; i < PointB.size(); i++){
		if (PointB[i].x<160 && PointB[i].y<100){
			Btl = PointB[i];
		}
		else if (PointB[i].x>300 && PointB[i].y<160){
			Btr = PointB[i];
		}
		else if (PointB[i].x<160 && PointB[i].y>200){
			Bbl = PointB[i];
		}
		else if (PointB[i].x>300 && PointB[i].y>200){
			Bbr = PointB[i];
		}
		else{
			Bc = PointB[i];
		}
	}
	for (int i = 0; i < PointI.size(); i++){
		if (PointI[i].x<160 && PointI[i].y<100){
			Itl = PointI[i];
		}
		else if (PointI[i].x>300 && PointI[i].y<160){
			Itr = PointI[i];
		}
		else if (PointI[i].x<160 && PointI[i].y>200){
			Ibl = PointI[i];
		}
		else if (PointI[i].x>300 && PointI[i].y>200){
			Ibr = PointI[i];
		}
		else{
			Ic = PointI[i];
		}
	}
	//bw�Ƕ�
	double kb1 = (double)(Btl.y - Btr.y) / (double)(Btl.x - Btr.x);
	double kb2 = (double)(Bbl.y - Bbr.y) / (double)(Bbl.x - Bbr.x);
	double kb3 = -(double)(Btr.x - Bbr.x) / (double)(Btr.y - Bbr.y);
	double kb4 = -(double)(Btl.x - Bbl.x) / (double)(Btl.y - Bbl.y);
	double kB = (kb1 + kb2 + kb3 + kb4) / 4;
	double angleB = atan(kB) / 3.1415926 * 180;
	//lw�Ƕ�
	double kI1 = (double)(Itl.y - Itr.y) / (double)(Itl.x - Itr.x);
	double kI2 = (double)(Ibl.y - Ibr.y) / (double)(Ibl.x - Ibr.x);
	double kI3 = -(double)(Itr.x - Ibr.x) / (double)(Itr.y - Ibr.y);
	double kI4 = -(double)(Itl.x - Ibl.x) / (double)(Itl.y - Ibl.y);
	double kI = (kI1 + kI2 + kI3 + kI4) / 4;
	double angleI = atan(kI) / 3.1415926 * 180;
	Para.w = angleI - angleB;

	//����uv
	Point centerB, centerI; 
	centerB.x = 0; centerB.y = 0;
	centerI.x = 0; centerI.y = 0;
	for (int i = 0; i < Points.PointB.size(); i++){
		centerB.x = centerB.x + Points.PointB[i].x;
		centerB.y = centerB.y + Points.PointB[i].y;
		centerI.x = centerI.x + Points.PointI[i].x;
		centerI.y = centerI.y + Points.PointI[i].y;
	}
	centerB.x = centerB.x / Points.PointB.size();
	centerB.y = centerB.y / Points.PointB.size();
	centerI.x = centerI.x / Points.PointB.size();
	centerI.y = centerI.y / Points.PointB.size();
	int delta_x = centerI.x - centerB.x - dis_pixel;
	int delta_y = centerI.y - centerB.y;
	Para.v = (delta_y * pixel_size / f) / 3.1415926 * 180;
	Para.u = (delta_x * pixel_size / f) / 3.1415926 * 180;
	return Para;

}

bool MyCmp_x(Point &a ,Point &b )
{
	if( a.x < b.x)
		return true;
	else
		return false;
}

//��3�ַ����ҽǵ�
Para calPara3(Mat B, Mat I, double Dis, double pixel_size, double f, double D)
{
	Points Points;
	Para Para;

	medianBlur(B, B,5);//��ֵ�˲�
	medianBlur(I, I,3);
	savePicture(I,"movMat.jpg");
	savePicture(B,"fixMat.jpg");

	Points = getCorner3(B, I);

	vector<Point> PointB = Points.PointB;
	vector<Point> PointI = Points.PointI;
	Point Btl, Btr, Bbl, Bbr, Bc, Itl, Itr, Ibl, Ibr, Ic;
	double d = 1 / (1 / f - 1 / D);//�����
	double dis = Dis * d / D;//��������ͷ���������
	double dis_pixel = dis / pixel_size; //��ȡ������ͷ�����ؾ���
	//�ǵ���� ��������� ������������м��һ��


	if( 5 != PointB.size() || 5 != PointI.size() )
	{
		//�ҵ��Ľǵ㲻��5��
		Para.u=10000;
		Para.v=10000;
		Para.w=10000;
		msg("��ȡ5���ǵ�ʧ��");
		return Para;
	}
	//���갴 xֵ ��������
	sort(PointB.begin(), PointB.end(),MyCmp_x);
	Bc=PointB[2];
	if( PointB[0].y > PointB[1].y )
	{
		Btl=PointB[1];
		Bbl=PointB[0];
	}
	else
	{
		Btl=PointB[0];
		Bbl=PointB[1];
	}
	if( PointB[3].y > PointB[4].y )
	{
		Btr=PointB[4];
		Bbr=PointB[3];
	}
	else
	{
		Btr=PointB[3];
		Bbr=PointB[4];
	}

	//���갴 xֵ ��������
	sort(PointI.begin(), PointI.end(),MyCmp_x);
	Ic=PointI[2];
	if( PointI[0].y > PointI[1].y )
	{
		Itl=PointI[1];
		Ibl=PointI[0];
	}
	else
	{
		Itl=PointI[0];
		Ibl=PointI[1];
	}
	if( PointI[3].y > PointI[4].y )
	{
		Itr=PointI[4];
		Ibr=PointI[3];
	}
	else
	{
		Itr=PointI[3];
		Ibr=PointI[4];
	}
	////��¼���ļ�
	//if (!CFG_INSTANCE->is_file_exist("data\\doubleAA.csv"))
	//{
	//	CFG_INSTANCE->write_file("data\\doubleAA.csv","����,�����x,�����y,���ҵ�x,���ҵ�y,���ĵ�x,���ĵ�y,	\
	//		�����x,�����y,���ҵ�x,���ҵ�y,�����ϱߵ�б��K1,�����±ߵ�б��K2,�����ұߵ�б��K3,������ߵ�б��K4,ƽ��б��K \n");

	//}

	
	//bw�Ƕ�  yΪͼ����з����ϵ����� xΪ�з����ϵ�
	double kb1 = abs( (double)(Btl.y - Btr.y) / (double)(Btl.x - Btr.x) );//�������������б��
	double kb2 = abs( (double)(Bbl.y - Bbr.y) / (double)(Bbl.x - Bbr.x) );//�������������б��
	double kb3 = abs( (double)(Btr.x - Bbr.x) / (double)(Btr.y - Bbr.y) );//�ұ����������б�ʵĵ���
	double kb4 = abs( (double)(Btl.x - Bbl.x) / (double)(Btl.y - Bbl.y) );//������������б�ʵĵ���
	double kB = (kb1 + kb2 + kb3 + kb4) / 4;                        //4��ֵȡƽ��б��
	//double angleB = atan(kB) / 3.1415926 * 180;						//��ɽǶ�                  
	//lw�Ƕ�
	double kI1 = abs( (double)(Itl.y - Itr.y) / (double)(Itl.x - Itr.x) );
	double kI2 = abs( (double)(Ibl.y - Ibr.y) / (double)(Ibl.x - Ibr.x) );
	double kI3 = abs( (double)(Itr.x - Ibr.x) / (double)(Itr.y - Ibr.y) );
	double kI4 = abs( (double)(Itl.x - Ibl.x) / (double)(Itl.y - Ibl.y) );
	double kI = (kI1 + kI2 + kI3 + kI4) / 4;
	//double angleI = atan(kI) / 3.1415926 * 180;
	//Para.w = angleB - angleI;                                   //�����ǶȲ�ֵΪw,��z�������򿴣���Ҫ��תw

	//4���ǵ�Χ�ɵ��Ǹ�ƽ���ı���
	//�����ߵ�ԭͼб�� ���ƶ�ͼб�� �� ��Ҫת�ĽǶ�֮��Ĺ�ϵ 
	
	double sita = (abs(kI - kB))/(1+kI*kB); 
	double tempVal_1 = (double)(Btl.y - Btr.y)/(double)(Btr.x - Btl.x);
	double tempVal_2 = (double)(Itl.y - Itr.y)/(double)(Itr.x - Itl.x);

	if( 0 < tempVal_2 - tempVal_1 )
	{
		Para.w = - atan(sita)*180/3.1415926;
	}
	else
	{
		Para.w = atan(sita)*180/3.1415926;
	}
	
	//����uv
	Point centerB, centerI; 
	centerB.x = 0; centerB.y = 0;
	centerI.x = 0; centerI.y = 0;
	for (int i = 0; i < Points.PointB.size(); i++){
		centerB.x = centerB.x + Points.PointB[i].x; //�ۼ�5��������з����ϵ�����
		centerB.y = centerB.y + Points.PointB[i].y;
		centerI.x = centerI.x + Points.PointI[i].x;
		centerI.y = centerI.y + Points.PointI[i].y;
	}
	centerB.x = centerB.x / Points.PointB.size();//ȡƽ��ֵ
	centerB.y = centerB.y / Points.PointB.size();
	centerI.x = centerI.x / Points.PointB.size();
	centerI.y = centerI.y / Points.PointB.size();

	int delta_x = centerI.y - centerB.y ;//��׼ͼΪB��ͼ����ߣ����� I.x
	int delta_y = centerI.x - centerB.x- dis_pixel;
	//���� v ��ʽ
	int beta_x;
	if( delta_x < 0 )
	{
		beta_x = abs( centerI.y - B.rows/2 );
	} 
	else
	{
		beta_x = abs( centerB.y - B.rows/2 );
	}
	double Val_a = beta_x / f; 
	double Val_b = (abs(delta_x) + beta_x)/f;
	//ȷ������
	if( delta_x > 0 )
	{
		Para.v = atan( abs(Val_a - Val_b)/(1+Val_a*Val_b) ) / 3.1415926 * 180;
	}
	else
	{
		Para.v = -atan( abs(Val_a - Val_b)/(1+Val_a*Val_b) ) / 3.1415926 * 180;
	}

	//���� u
	int bate_y;
	if( delta_y < 0 )
	{
		bate_y = abs( centerB.x - B.cols/2 + dis_pixel );
	}
	else
	{
		bate_y = abs( centerI.x - B.cols/2 );
	}
	Val_a = bate_y / f;
	Val_b = ( abs(delta_y)+bate_y )/f;
	//ȷ������
	if( delta_y > 0 )
	{
		Para.u = atan( abs( Val_a - Val_b )/(1+Val_a*Val_b) ) / 3.1415926 * 180;
	}
	else
	{
		Para.u = -atan( abs( Val_a - Val_b )/(1+Val_a*Val_b) ) / 3.1415926 * 180;
	}
	//Para.u = atan(delta_y * pixel_size / f) / 3.1415926 * 180;
	//Para.v = atan(delta_x * pixel_size / f) / 3.1415926 * 180;
	//Para.u=0; 
	////Para.v=0;
	//Para.w=0; 
	CFG_INSTANCE->Close("data\\doubleAA.csv");

	return Para;

}

Points getCorner3(Mat B, Mat I)
{
	Points Points;
	vector<Point> TempB,TempI, PointB, PointI;

	int maxCorners = 20;

	vector<Point2f> corners;  
	double qualityLevel = 0.01;  
	double minDistance = 10;  
	int blockSize = 3;  
	bool useHarrisDetector = false;  
	double k = 0.04;  
	/// Copy the source image  
	Mat cormat;  
	int bxs = 190,bxlen = 420,bys =50,bylen=550;
	int ixs= 220,ixlen=450,iys=50,iylen=550;
	Rect areaB(bxs,bys,bxlen,bylen);
	Rect areaI(ixs,iys,ixlen,iylen);

	//Rect areaB(170,50,495,550);
	//Rect areaI(220,50,480,550);
	Mat maskB(B.rows,B.cols,CV_8UC1);
	Mat maskI(I.rows,I.cols,CV_8UC1);
	for(int i =0; i<B.rows;i++)
	{
		for(int j=0;j<B.cols;j++)
		{
			maskB.at<uchar>(i,j)=0;
			maskI.at<uchar>(i,j)=0;
		}
	}
	
	//circle( B, Point(190,50), 4, Scalar(0,255,0), 2, 8, 0 ); 
	//circle( B, Point(665,50), 4, Scalar(0,255,0), 2, 8, 0 );
	//circle( I, Point(220,50), 4, Scalar(0,255,0), 2, 8, 0 ); 
	//circle( I, Point(700,50), 4, Scalar(0,255,0), 2, 8, 0 );
	//showPicture(B,"B" );
	//showPicture(I,"I" );

	maskB(areaB).setTo(255);//��roi������Ϊ255
	maskI(areaI).setTo(255);

	//circle( B, Point(bxs,bys), 4, Scalar(0,255,0), 2, 8, 0 ); //��һ����190�У�
	//circle( B, Point(bxs+bxlen,bys), 4, Scalar(0,255,0), 2, 8, 0 );
	//circle( B, Point(bxs,bys+bylen), 4, Scalar(0,255,0), 2, 8, 0 ); 
	//circle( B, Point(bxs+bxlen,bys+bylen), 4, Scalar(0,255,0), 2, 8, 0 );

	//circle( I, Point(ixs,iys), 4, Scalar(0,255,0), 2, 8, 0 ); 
	//circle( I, Point(ixs+ixlen,iys), 4, Scalar(0,255,0), 2, 8, 0 );
	//circle( I, Point(ixs,iys+iylen), 4, Scalar(0,255,0), 2, 8, 0 ); 
	//circle( I, Point(ixs+ixlen,iys+iylen), 4, Scalar(0,255,0), 2, 8, 0 );
	//showPicture(B,"B" );
	//showPicture(I,"I" );
	goodFeaturesToTrack( B,TempB,  maxCorners,   0.05, 30, maskB, 3, 0, 0.4); 
	goodFeaturesToTrack( I,TempI,  maxCorners,   0.03, 10, maskI, 3, 0, 0.4);
	if (TempB.size()!=20 || TempI.size() !=20)
	{
		msg("û�ҵ�20����");
		return Points;

	}

	float bthro = 0.20,ithro = 0.38;
	//����
	//ɸѡ���½ǽǵ�,��ֵҪ��������
	PointB.clear();
	for (int i = 0; i < TempB.size(); i++){
		float a = B.at<float>(TempB[i].y-7 , TempB[i].x-7);
		if (a < bthro )//ԭ��Ϊ0.15 x 255
		{
			PointB.push_back(TempB[i]);
			//cout <<TempB[i] << endl;
		}
	}
	//&& B.at<float>(TempB[i].y, TempB[i].x - 10) > 0.5 && B.at<float>(TempB[i].y, TempB[i].x + 10) > 0.5
	PointI.clear();
	for (int i = 0; i < TempI.size(); i++){
		float a = I.at<float>(TempI[i].y-7 , TempI[i].x-7);
 		if (a< ithro){
			PointI.push_back(TempI[i]);
			circle(I, Point(TempI[i].x,TempI[i].y), 4, Scalar(0,255,0), 2, 8, 0 ); 
			//cout <<TempI[i] << endl;
		}
	}
	/*showPicture(I,"Ii" );*/

	///// ���Դ��� 
	if (PointB.size() != 5 || PointI.size() != 5){
		cout << "Default in finding points��" << endl;
		cout << "Default in finding points��" << endl;
		cout << "Default in finding points��" << endl;
	}
	for( int i = 0; i < TempI.size(); i++ ){   
		//circle( dst_norm_scaled,  corners[i], 5,  Scalar(255), 2, 8, 0 );   
		circle( B, TempB[i], 4, Scalar(0,255,0), 2, 8, 0 );   
		circle( I, TempI[i], 4, Scalar(0,255,0), 2, 8, 0 ); 
	}  
	for (int i=0;i<20;i++)
	{
		CFG_INSTANCE->write_file("data\\1.csv","%d,%d,%d,%d\n",TempI[i].x,TempI[i].y,TempI[i].x,TempI[i].y);
	}
	


	CFG_INSTANCE->Close("data\\1.csv");

	static  int  a = 0;
	char b[10] ={0};
	char i[10] ={0};
	sprintf(b,"b%d.jpg",a);
	sprintf(i,"i%d.jpg",a++);
	savePicture(B,b);
	savePicture(I,i);

	Points.PointB = PointB;
	Points.PointI = PointI;
	return Points;
}



//halcon��
Para calUVW(int A,int B,HObject &ObjectA,HObject& ObjectB)
{
	int ret = 0;
	Para p;
	vector<MyPoints> centerA;
	vector<MyPoints> centerB;
	ret = getAllRegionPoints(A,ObjectA, centerA);
	ret = getAllRegionPoints(B,ObjectB, centerB);
	//�������ĵ㵽ȫ�ֱ���
	copy_variable(A,B,&centerA,&centerB);
	p= calUVWByPoint(centerA,centerB);
	return p;
}

Para calUVWByPoint(vector<MyPoints>& centerA,vector<MyPoints>& centerB)
{
	Para Para;
	memset(&Para,0.0,sizeof(Para));
	double d = 1 / (1 / g_AAParam.focus - 1 / g_AAParam.objdis);//�����
	double dis = g_AAParam.doubleAAdis * d / g_AAParam.objdis;//��������ͷ���������
	double dis_pixel = dis / g_AAParam.pixel_size; //��ȡ������ͷ�����ؾ���

	MyPoints Btl = centerA[0];
	MyPoints Bbr = centerA[1];
	MyPoints Bc = centerA[2];
	MyPoints Bbl = centerA[3];
	MyPoints Btr = centerA[4];

	MyPoints Itl = centerB[0];
	MyPoints Ibr = centerB[1];
	MyPoints Ic =  centerB[2];
	MyPoints Ibl = centerB[3];
	MyPoints Itr = centerB[4];

	//bw�Ƕ�  yΪͼ����з����ϵ����� xΪ�з����ϵ�
	double kb1 = abs( (Btl.row - Btr.row) / (Btl.col - Btr.col) );//�������������б��
	double kb2 = abs( (Bbl.row - Bbr.row) / (Bbl.col - Bbr.col) );//�������������б��
	double kb3 = abs( (Btr.col - Bbr.col) / (Btr.row - Bbr.row) );//�ұ����������б�ʵĵ���
	double kb4 = abs( (Btl.col - Bbl.col) / (Btl.row - Bbl.row) );//������������б�ʵĵ���                 
	double kB1 = (kb1+kb2)/2;//����ֵȡƽ��б��
	double kB2 = (kb3+kb4)/2;
	//lw�Ƕ�
	double ki1 = abs( (Itl.row - Itr.row) / (Itl.col - Itr.col) );
	double ki2 = abs( (Ibl.row - Ibr.row) / (Ibl.col - Ibr.col) );
	double ki3 = abs( (Itr.col - Ibr.col) / (Itr.row - Ibr.row) );
	double ki4 = abs( (Itl.col - Ibl.col) / (Itl.row - Ibl.row) );
	double kI1 = (ki1+ki2)/2;
	double kI2 = (ki3+ki4)/2;
	//4���ǵ�Χ�ɵ��Ǹ�ƽ���ı���
	//�����ߵ�ԭͼб�� ���ƶ�ͼб�� �� ��Ҫת�ĽǶ�֮��Ĺ�ϵ 
	//k1 k2б�������k3 k4б����������Էֳ�������ƽ��ֵ������б�ʷֱ���� w'��w �������ֵ
	double sita1 = (abs(kI1 - kB1))/(1+kI1*kB1); 
	double sita2 = (abs(kI2 - kB2))/(1+kI2*kB2); 
	double tempVal_1 = (Btl.row - Btr.row)/(Btr.col - Btl.col);
	double tempVal_2 = (Itl.row - Itr.row)/(Itr.col - Itl.col);
	if( 0 < tempVal_2 - tempVal_1 )
	{
		Para.w = - ( atan(sita1)+atan(sita2) )*90/3.1415926;
	}
	else 
	{
		Para.w = ( atan(sita1)+atan(sita2) )*90/3.1415926;
	}

	//����uv
	MyPoints centerBp, centerIp; 
	centerBp.col = 0; centerBp.row = 0;
	centerIp.col = 0; centerIp.row = 0;
	for (int i = 0; i < centerA.size(); i++){
		centerBp.col = centerBp.col + centerA[i].col; //�ۼ�5��������з����ϵ�����
		centerBp.row = centerBp.row + centerA[i].row;
		centerIp.col = centerIp.col + centerB[i].col;
		centerIp.row = centerIp.row + centerB[i].row;
	}
	centerBp.col = centerBp.col / centerA.size();//ȡƽ��ֵ
	centerBp.row = centerBp.row / centerA.size();
	centerIp.col = centerIp.col / centerB.size();
	centerIp.row = centerIp.row / centerB.size();

	double delta_x = centerIp.row - centerBp.row ;//��׼ͼΪB��ͼ����ߣ����� I.x
	double delta_y = centerIp.col - centerBp.col- dis_pixel;

	
	//���� v ��ʽ
	double Val_a = ( centerBp.row - g_AAParam.mapdiswidth/2 ); 
	double Val_b = ( centerIp.row - g_AAParam.mapdiswidth/2 );
	if( Val_a*Val_b > 0 )
	{
		Val_a = abs(Val_a)*g_AAParam.pixel_size/g_AAParam.focus;
		Val_b = abs(Val_b)*g_AAParam.pixel_size/g_AAParam.focus;
		if( delta_x > 0 )
		{
			Para.v = atan( abs(Val_a - Val_b)/(1 + Val_a*Val_b) ) / 3.1415926 * 180;
		}
		else
		{
			Para.v = -atan( abs(Val_a - Val_b)/(1 + Val_a*Val_b) ) / 3.1415926 * 180;
		}
	}
	else
	{
		Val_a = abs(Val_a)*g_AAParam.pixel_size/g_AAParam.focus;
		Val_b = abs(Val_b)*g_AAParam.pixel_size/g_AAParam.focus;
		if( delta_x > 0 )
		{
			if( (Val_a+Val_b)/(1-Val_a*Val_b) > 0 )
			{
				Para.v = atan( (Val_a+Val_b)/(1-Val_a*Val_b) )/3.1415926*180;
			}
			else
			{
				Para.v = 90 - atan( (Val_a+Val_b)/(1-Val_a*Val_b) )/3.1415926*180;
			}
		}
		else
		{
			if( (Val_a+Val_b)/(1-Val_a*Val_b) > 0 )
			{
				Para.v = -atan( (Val_a+Val_b)/(1-Val_a*Val_b) )/3.1415926*180;
			}
			else
			{
				Para.v = -90 + atan( (Val_a+Val_b)/(1-Val_a*Val_b) )/3.1415926*180;
			}
		}
	}

	//���� u
	Val_a = ( centerBp.col+ dis_pixel- g_AAParam.mapdislength/2 );
	Val_b = ( centerIp.col - g_AAParam.mapdislength/2 );
 	if( Val_a*Val_b > 0 )
	{
		Val_a = abs(Val_a)*g_AAParam.pixel_size/g_AAParam.focus;
		Val_b = abs(Val_b)*g_AAParam.pixel_size/g_AAParam.focus;
		if( delta_y > 0 )
		{
			Para.u = atan( abs(Val_a - Val_b)/(1 + Val_a*Val_b) ) / 3.1415926 * 180;
		}
		else
		{
			Para.u = -atan( abs(Val_a - Val_b)/(1 + Val_a*Val_b) ) / 3.1415926 * 180;
		}
	}
	else
	{
		Val_a = abs(Val_a)*g_AAParam.pixel_size/g_AAParam.focus;
		Val_b = abs(Val_b)*g_AAParam.pixel_size/g_AAParam.focus;
		if( delta_y > 0 )
		{
			if( (Val_a+Val_b)/(1-Val_a*Val_b) > 0 )
			{
				Para.u = atan( (Val_a+Val_b)/(1-Val_a*Val_b) )/3.1415926*180;
			}
			else
			{
				Para.u = 90 - atan( (Val_a+Val_b)/(1-Val_a*Val_b) )/3.1415926*180;
			}
		}
		else
		{
			if( (Val_a+Val_b)/(1-Val_a*Val_b) > 0 )
			{
				Para.u = -atan( (Val_a+Val_b)/(1-Val_a*Val_b) )/3.1415926*180;
			}
			else
			{
				Para.u = -90 + atan( (Val_a+Val_b)/(1-Val_a*Val_b) )/3.1415926*180;
			}
		}
	}
	return Para;
}

int getAllRegionPoints(int vid,HObject Object,vector<MyPoints>& center)
{
	int ret = 0;
	ret =  calCenterPoints(vid,Object,AA_TOP,center);
	ret =  calCenterPoints(vid,Object,AA_BOTTOM,center);
	ret =  calCenterPoints(vid,Object,AA_CENTER,center);
	ret =  calCenterPoints(vid,Object,AA_LEFT,center);
	ret =  calCenterPoints(vid,Object,AA_RIGHT,center);
	return ret;
}

int calCenterPoints(int vid,HObject Object,int regiontype,vector<MyPoints>& center)
{
	int ret = 0;
	HObject OutObject;
	IAVH_ROI_STRUCT roiP;
	roiP.type = IAVH_RETANGLE;
	//��������.�£��У���,��������5�������10���㣬ÿ������������������20����
	roiP.rect.Row1 = g_AAParam.channel1Retangle[vid][regiontype].ltx;
	roiP.rect.Column1 = g_AAParam.channel1Retangle[vid][regiontype].lty;
	roiP.rect.Row2 = g_AAParam.channel1Retangle[vid][regiontype].rbx;
	roiP.rect.Column2 = g_AAParam.channel1Retangle[vid][regiontype].rby;
	//��ȡ����
	ret = IAVH_GetImgRetangleROI(Object,OutObject, roiP,0);
	IAVH_PARAM_STRUCT param ;
	param.selectShape.min = g_AA_PicParam.selectShape[vid].min;
	param.selectShape.max = g_AA_PicParam.selectShape[vid].max;
	param.scaleImage.mul = g_AA_PicParam.scaleImage[vid].mul;
	param.scaleImage.add = g_AA_PicParam.scaleImage[vid].add;
	double C[2] ={0};
	ret = IAVH_GetImgROICenterByArea(OutObject,C,param,0);
	MyPoints Ce = {0.0};
	Ce.row = C[0];
	Ce.col = C[1];
	center.push_back(Ce);
	return ret;
}



