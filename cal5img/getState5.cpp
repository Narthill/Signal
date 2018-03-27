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
}State; // get the current state of the initial image including u, v and z
typedef struct{
	vector<double> horizontal_lap;
	vector<double> vertical_lap;
	vector<Point> horizontal_pt;
	vector<Point> vertical_pt;
}HV_Lap;

HV_Lap getEdgeLaplace(Mat img, int block_size, float thr, int edge_size, int error, double f, double scale, double pixel_size, double d_obj, double size, double block);
double getangle(vector<double> pt_o, vector<double> Y);
int maxloc(double a[], int size);



State getState5(Mat img_1, Mat img_2, Mat img_3, Mat img_4, Mat img_5, double pixel_size, double scale)
{
	//�����趨��������Ҫ����Ų����������
	double f = 3.31;                      //��ͷ����Ϊ3.31mm
	double d_obj = 600;					  //���Ϊ600mm
	double size = 90, block = 140;        //ѡ�򴰿����ϵ��ڻ�׼������90���ش���block����Ϊ140����
	int edge_size = 10;                   //�б߿�ߴ糤��Ϊ15����
	int error = 40;                       //ѡ��error��ֵΪ40
	int block_size = 15;                  //����ֵ�����ж��б�λ�óߴ糤��Ϊ19����
	float thr = pow((2 * (float)block_size + 1), 2) / 2 * 0.2;   //����ֵ�����ж�����

	State state;
	vector<Mat> img;  vector<HV_Lap> hv_lap;
	double center[5], right[5], left[5], top[5], bottom[5];
	img.push_back(img_1); img.push_back(img_2); img.push_back(img_3); img.push_back(img_4); img.push_back(img_5);
	//��������ͼ���laplace��,������deltaZ
	for (int i = 0; i < 5; i++){
		hv_lap.push_back(getEdgeLaplace(img[i], block_size, thr, edge_size, error, f, scale, pixel_size, d_obj, size, block));
		center[i] = (hv_lap[i].horizontal_lap[1] + hv_lap[i].vertical_lap[1]) / 2;
		left[i] = hv_lap[i].horizontal_lap[0];
		right[i] = hv_lap[i].horizontal_lap[2];
		top[i] = hv_lap[i].vertical_lap[0];
		bottom[i] = hv_lap[i].vertical_lap[2];
	}
	state.deltaZ = maxloc(center, 5)*0.003 - 0.006;
	
	//����AngleX
	vector<double> Y_x, pt_x;
	Y_x.push_back(maxloc(left, 5)*0.003 - 0.006);
	Y_x.push_back(maxloc(center, 5)*0.003 - 0.006);
	Y_x.push_back(maxloc(right, 5)*0.003 - 0.006);
	for (int i = 0; i < 3; i++)
	{
		pt_x.push_back((hv_lap[2].horizontal_pt[i].x)*pixel_size);
	}
	state.x_angle = getangle(pt_x, Y_x);
	//����AngleY
	vector<double> Y_y, pt_y;
	Y_y.push_back(maxloc(top, 5)*0.003 - 0.006);
	Y_y.push_back(maxloc(center, 5)*0.003 - 0.006);
	Y_y.push_back(maxloc(bottom, 5)*0.003 - 0.006);
	for (int i = 0; i < 3; i++)
	{
		pt_y.push_back((hv_lap[2].vertical_pt[i].y)*pixel_size);
	}
	state.y_angle = getangle(pt_y, Y_y);
	return state;
}
/*���Ƕ�*/
double getangle(vector<double> pt_o, vector<double> Y)
{
	//�����뽹����ϱ����б��һ�κ���
	czy::Fit fit_l;
	fit_l.linearFit(pt_o, Y);
	//������ϵõ���б�ʵõ���б�Ƕȣ���λΪ��
	double angle = atan(fit_l.getFactor(1)) / 3.14159265 * 180;
	return angle;
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