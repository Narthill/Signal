#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp> 
#include "polyfit.h"
#include <iostream>

using namespace std;
using namespace cv;
using namespace czy;

typedef struct {
	double x_angle;
	double y_angle;
	double deltaZ;
	double time_Edge;
	double time_Lap;
	vector<double> horizontal_lap;
	vector<double> vertical_lap;
	/*
	double Delta_Z_Center_x;
	double Delta_Z_Center_y;
	double Delta_Z_Left;
	double Delta_Z_Right;
	double Delta_Z_Top;
	double Delta_Z_Bottom;
	*/
}State; // get the current state of the initial image including u, v and z
typedef struct{
	vector<double> horizontal_lap;
	vector<double> vertical_lap;
	vector<Point> horizontal_pt;
	vector<Point> vertical_pt;
	double time_Edge;
}HV_Lap;

HV_Lap getAllLaplace(Mat img, int edge_size, int error);
State getState(Mat img_ori, Mat img_p, Mat img_m, double deltaZ_p, double deltaZ_m, double pixel_size);
vector<double> getangle(vector<double> lap_o, vector<double> lap_p, vector<double> lap_m, vector<double> pt_o, double deltaZ_p, double deltaZ_m, double pixel_size);
int maxloc(double a[], int size);
/*
%get the current state of the initial image including u, v and z
%============================================
%Author:Chame & Ocean(Using polyfit.h and vector)
%Date:2017.4.10
%============================================
%Input
% ima_ori: the original image in the initial position;
% deltaZ_p: the distance of image moving to the plus direction;
% deltaZ_m: the distance of image moving to the minus direction;
% img_p: the image moved deltaZ_p distance to the plus direction;
% img_m: the image moved deltaZ_m distance to the minus direction;
% pixel_size: the size of CCD pixel;
%--------------------------------------------
%Output
% deltaZ: the distance of the initial image to the focal plane;
% x_offset, y_offset: horizontal(x) offset and vertical(y) offset;
% x_angle, y_angle: horizontal(x) angle and vertical(y) angle;
%============================================
*/
State getState(Mat img_ori, Mat img_p, Mat img_m, double deltaZ_p, double deltaZ_m, double pixel_size)
{
	State state;
	int edge_size = 15;
	int error = 40;
	//��������ͼ���laplace��
	double time_Lap = static_cast<double>(getTickCount());
	HV_Lap hv_lap_o = getAllLaplace(img_ori, edge_size, error);
	HV_Lap hv_lap_p = getAllLaplace(img_p, edge_size, error);
	HV_Lap hv_lap_m = getAllLaplace(img_m, edge_size, error);

	state.horizontal_lap.push_back(hv_lap_p.horizontal_lap[0]);
	state.horizontal_lap.push_back(hv_lap_p.horizontal_lap[1]);
	state.horizontal_lap.push_back(hv_lap_p.horizontal_lap[2]);
	state.horizontal_lap.push_back(hv_lap_o.horizontal_lap[0]);
	state.horizontal_lap.push_back(hv_lap_o.horizontal_lap[1]);
	state.horizontal_lap.push_back(hv_lap_o.horizontal_lap[2]);
	state.horizontal_lap.push_back(hv_lap_m.horizontal_lap[0]);
	state.horizontal_lap.push_back(hv_lap_m.horizontal_lap[1]);
	state.horizontal_lap.push_back(hv_lap_m.horizontal_lap[2]);
	state.vertical_lap.push_back(hv_lap_p.vertical_lap[0]);
	state.vertical_lap.push_back(hv_lap_p.vertical_lap[1]);
	state.vertical_lap.push_back(hv_lap_p.vertical_lap[2]);
	state.vertical_lap.push_back(hv_lap_o.vertical_lap[0]);
	state.vertical_lap.push_back(hv_lap_o.vertical_lap[1]);
	state.vertical_lap.push_back(hv_lap_o.vertical_lap[2]);
	state.vertical_lap.push_back(hv_lap_m.vertical_lap[0]);
	state.vertical_lap.push_back(hv_lap_m.vertical_lap[1]);
	state.vertical_lap.push_back(hv_lap_m.vertical_lap[2]);

	state.time_Lap = ((double)getTickCount() - time_Lap) / getTickFrequency();
	state.time_Edge = hv_lap_o.time_Edge + hv_lap_m.time_Edge + hv_lap_p.time_Edge;
	vector<Point> x_pt_o = hv_lap_o.horizontal_pt;
	vector<Point> y_pt_o = hv_lap_o.vertical_pt;
	vector<double> x_lap_o = hv_lap_o.horizontal_lap;
	vector<double> y_lap_o = hv_lap_o.vertical_lap;
	vector<double> x_lap_p  = hv_lap_p.horizontal_lap;
	vector<double> y_lap_p = hv_lap_p.vertical_lap;
	vector<double> x_lap_m = hv_lap_m.horizontal_lap;
	vector<double> y_lap_m = hv_lap_m.vertical_lap;
	//����x�����ƫת�Ƕ�
	vector<double> x_pt;
	for (int i = 0; i < 3; i++)
	{
		x_pt.push_back(x_pt_o[i].x);
	}
	cout << endl << "Fitting!" << endl;
	cout << endl << "Delta_Z��" << endl;
	vector<double> Getangle_x = getangle(x_lap_o, x_lap_p, x_lap_m, x_pt, deltaZ_p, deltaZ_m, pixel_size);
	state.x_angle = Getangle_x[0];
	//����y�����ƫת�Ƕ�
	vector<double> y_pt;
	for (int i = 0; i < 3; i++)
	{
		y_pt.push_back(y_pt_o[i].y);
	}
	vector<double> Getangle_y = getangle(y_lap_o, y_lap_p, y_lap_m, y_pt, deltaZ_p, deltaZ_m, pixel_size);
	state.y_angle = Getangle_y[0];
	state.deltaZ = (Getangle_x[2] + Getangle_y[2])/2;
	/*
	state.Delta_Z_Center_x = Getangle_x[2];
	state.Delta_Z_Center_y = Getangle_y[2];
	state.Delta_Z_Left = Getangle_x[1];
	state.Delta_Z_Right = Getangle_x[3];
	state.Delta_Z_Top = Getangle_y[1];
	state.Delta_Z_Bottom = Getangle_y[3];
	*/
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
	deltaZ[1] = 2 * deltaZ[1];
	
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