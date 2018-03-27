#include <iostream>
#include <opencv2\opencv.hpp>
#include "math.h"
#include <numeric>

using namespace cv;
using namespace std;

typedef struct {
	vector<Point> PointB, PointI;
}Points;

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

Points getCorner(Mat B, Mat I){
	//ѡ���ʼλ�����ã�CAD������ͼ���ͨ����ʽ����
	Point center, left, right, top, bottom;
	center.x = 1800; center.y = 1400;
	left.x = 200; left.y = 1400;
	right.x = 3500; right.y = 1400;
	top.x = 1800; top.y = 300;
	bottom.x = 1800; bottom.y = 2500;
	vector<Point> loc;
	loc.push_back(center); loc.push_back(left); loc.push_back(right); loc.push_back(top); loc.push_back(bottom);
	//�����趨
	double size = 500;
	double th = 9000;
	Points Points;
	//ѡ�����������
	vector<Mat> B_block, I_block;
	for (int i = 0; i < 5; i++){
		B_block.push_back(B(Rect(loc[i].x, loc[i].y, size, size)));
		I_block.push_back(I(Rect(loc[i].x, loc[i].y, size, size)));
	}
	//����õ�fixͼ������
	for (int i = 0; i < 5; i++){
		Point temp,temp1;
		temp1.x = 1; temp1.y = 1;
		double sum_x[500], sum_y[500];          //��Ҫ����size��ֵ�趨һ��
		vector<double> x_th, x_thh, y_th, y_thh;
		//������
		for (int j = 0; j < size; j++){
			sum_x[j] = 0;
			sum_y[j] = 0;
			for(int k = 0; k < size; k++){
				sum_x[j] = sum_x[j] + B_block[i].at<float>(j,k);
				sum_y[j] = sum_y[j] + B_block[i].at<float>(k, j);
			}
			if (sum_x[j] > th){
				x_th.push_back(sum_x[j]);
				x_thh.push_back(sum_x[j] * j);
			}
			if (sum_y[j] > th){
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
		Points.PointB.push_back(temp + loc[i] - temp1);
	}
	//����õ�MOVEͼ������
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
			if (sum_x[j] > th){
				x_th.push_back(sum_x[j]);
				x_thh.push_back(sum_x[j] * j);
			}
			if (sum_y[j] > th){
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
		Points.PointI.push_back(temp + loc[i] - temp1);
	}
	return Points;
}