#pragma once
#include <iostream>
#include <opencv2\opencv.hpp>
using namespace cv;
using namespace std;


//
//class Mat;
//4�ַ���������ͼ������
int ScanImageEfficiet(Mat & image);
int ScanImageIterator(Mat & image);
int ScanImageRandomAccess(Mat & image);
int ScanImageLUT(Mat & image);

int ScanImage(Mat & image);//��������4�з���