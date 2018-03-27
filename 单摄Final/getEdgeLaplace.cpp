#include <opencv2\opencv.hpp>
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;

typedef struct{
	vector<double> horizontal_lap;
	vector<double> vertical_lap;
	vector<Point> horizontal_pt;
	vector<Point> vertical_pt;
}HV_Lap;
int showPicture(Mat &img,const string windowname);
int savePicture(Mat &img, string filename);

double laplace(Mat I, Point Loc, int edge_size);
HV_Lap getEdgeLaplace(Mat img, int block_size, float thr, int edge_size,  double f, double scale, double pixel_size, double d_obj, double size, double block,int * Laplace_Flag);
/*
%get all edge laplace
%============================================
%Author:Chame & Ocean(Get the pixel value using pointer)
%Date : 2017.6.5
%============================================
%Input
% img : input image
% block_size : 1 / 2 size of block to determine edge;
% thr: if value > thr, it will be edge;
% edge_size: 1/2 size of block to determine edge;
% f: the focal length of the lens;
% scale: the image scale's scale;
% pixel_size: the size of CCD pixel;
% d_obj: the object distance of the target;
% size: to determine the image window;
% block: to determine the image window;
%--------------------------------------------
%Output
% horizontal_lap: laplace of horizontal edges;
% vertical_lap: laplace of vertical edges;
% horizontal_pt: locations of horizontal points;
% vertical_pt: locations of vertical points;
%============================================
*/
HV_Lap getEdgeLaplace(Mat img, int block_size, float thr, int edge_size,  double f, double scale, double pixel_size, double d_obj, double size, double block,int * Laplace_Flag)
{
	double d_img = 1 / (1 / f - 1 / d_obj);
	double beta = d_img / d_obj;   
	
	//�������ѡ���׼λ��
	vector<Point> x_edge, y_edge;
	double i_center = 4208 / 2 * scale, j_center = 3120 / 2 * scale;//д����
	double i_left = ceil(i_center - 280 * beta / pixel_size);
	double i_right = ceil(i_center + 280 * beta / pixel_size);
	double j_top = ceil(j_center - 200 * beta / pixel_size);
	double j_bottom = ceil(j_center + 200 * beta / pixel_size);

	 i_center = 4208 / 2 * scale, j_center = 3120 / 2 * scale;
	 i_left = ceil(i_center - 250 * beta / pixel_size);
	 i_right = ceil(i_center + 250 * beta / pixel_size);
	 j_top = ceil(j_center - 185 * beta / pixel_size);
	 j_bottom = ceil(j_center + 185 * beta / pixel_size);

	//-16.0��ͼ
	//i_center = 395;//383;//440;
	//j_center = 345;//352;// 340.0;
	//i_left = 97;//78;//123.0;
	//i_right = 705;//700;//772.0;
	//j_top = 118;//125;//104.0;
	//j_bottom = 565;//585;//583.0;
	
	block =140;//Ҫ��ͼ����
	size = 75;
	//����ģ��
	Mat h_x = Mat::ones(block_size * 2 + 1, block_size * 2 + 1, CV_32F);
	//h_x.rowRange(0, block_size+1 ) = -1;//�Ϻ��°�
	h_x.rowRange( block_size + 1,2*block_size+1) = -1;//�ϰ��º�

	Mat h_y = Mat::ones(block_size * 2 + 1, block_size * 2 + 1, CV_32F);
	
	
	h_y.colRange( block_size + 1,2*block_size+1) = -1;//����Һ�
	//h_y.colRange(0, block_size - 1) = -1;
	savePicture(h_x,"x����ģ��.jpg");
	savePicture(h_y,"y����ģ��.jpg");
	//flip(img, img, 0);//�����費��Ҫ
	//Center�б�ѡȡ�����
	Mat value_x, value_y;
	filter2D(img(cvRect(i_center - size, j_center - size, block, block)), value_x, img.depth(), h_x, Point(-1, -1), (0, 0), BORDER_REFLECT);
	

	filter2D(img(cvRect(i_center - size, j_center - size, block, block)), value_y, img.depth(), h_y, Point(-1, -1), (0, 0), BORDER_REFLECT);
//	showPicture(img(cvRect(i_center-size , j_center-size , block, block)),"img");
//	showPicture(value_x,"value_x");
//	showPicture(value_y,"value_y");
	savePicture(img(cvRect(i_center-size , j_center-size , block, block)),"����Сͼ.jpg");
	savePicture(value_x,"x���������˲�ͼ.jpg");
	savePicture(value_y,"y���������˲�ͼ.jpg");

	//�ҵ�����Դ�����ֵ�ĵĵ��λ�ã������Ǽ������ƣ�ȷ���бߵ�λ��
	for (int j = block_size; j < block - block_size; j++)
	{
		float* data_x = value_x.ptr<float>(j);
		float* data_y = value_y.ptr<float>(j);
		for (int i = block_size ; i < block - block_size ; i++)
		{
			if (data_x[i] > thr)
			{
				Mat roi(value_x, Rect(i - block_size, j - block_size, 2 * block_size + 1, 2 * block_size + 1));
				Point maxPoint,maxPoint_Img;
				minMaxLoc(roi, 0, 0, 0, &maxPoint);
				maxPoint.x = maxPoint.x + i - block_size;
				maxPoint.y = maxPoint.y + j - block_size;
				if ( i == maxPoint.x && j == maxPoint.y ){
					maxPoint_Img.x = i + i_center - size;
					maxPoint_Img.y = j + j_center - size;
					x_edge.push_back(maxPoint_Img);

				}
				
			}
			if (data_y[i] > thr)
			{
				Mat roi(value_y, Rect(i - block_size, j - block_size, 2 * block_size + 1, 2 * block_size + 1));
				Point maxPoint, maxPoint_Img;
				minMaxLoc(roi, 0, 0, 0, &maxPoint);
				maxPoint.x = maxPoint.x + i - block_size;
				maxPoint.y = maxPoint.y + j - block_size;
				if (i == maxPoint.x && j == maxPoint.y){
					maxPoint_Img.x = i + i_center - size;
					maxPoint_Img.y = j + j_center - size;
					y_edge.push_back(maxPoint_Img);
				}
			}
		}
	}


	//Right�б�ѡȡ
	filter2D(img(cvRect(i_right - size, j_center - size, block, block)), value_x, img.depth(), h_x, Point(-1, -1), (0, 0), BORDER_REFLECT);
	for (int j = block_size; j < block - block_size; j++)
	{
		float* data_x = value_x.ptr<float>(j);
		for (int i = block_size; i < block - block_size; i++)
		{
			if (data_x[i] > thr)
			{
				Mat roi(value_x, Rect(i - block_size, j - block_size, 2 * block_size + 1, 2 * block_size + 1));
				Point maxPoint, maxPoint_Img;
				minMaxLoc(roi, 0, 0, 0, &maxPoint);
				maxPoint.x = maxPoint.x + i - block_size;
				maxPoint.y = maxPoint.y + j - block_size;
				if (i == maxPoint.x && j == maxPoint.y){
					maxPoint_Img.x = i + i_right - size;
					maxPoint_Img.y = j + j_center - size;
					x_edge.push_back(maxPoint_Img);
				}

			}
		}
	}

	savePicture(img(cvRect(i_right-size , j_center-size , block, block)),"�ұ�Сͼ.jpg");
	savePicture(value_x,"x�����ұ��˲�ͼ.jpg");

	//Left�б�ѡȡ
	filter2D(img(cvRect(i_left - size, j_center - size, block, block)), value_x, img.depth(), h_x, Point(-1, -1), (0, 0), BORDER_REFLECT);
	for (int j = block_size; j < block - block_size; j++)
	{
		float* data_x = value_x.ptr<float>(j);
		for (int i = block_size; i < block - block_size; i++)
		{
			if (data_x[i] > thr)
			{
				Mat roi(value_x, Rect(i - block_size, j - block_size, 2 * block_size + 1, 2 * block_size + 1));
				Point maxPoint, maxPoint_Img;
				minMaxLoc(roi, 0, 0, 0, &maxPoint);
				maxPoint.x = maxPoint.x + i - block_size;
				maxPoint.y = maxPoint.y + j - block_size;
				if (i == maxPoint.x && j == maxPoint.y){
					maxPoint_Img.x = i + i_left - size;
					maxPoint_Img.y = j + j_center - size;
					x_edge.push_back(maxPoint_Img);
				}
			}
		}
	}

	savePicture(img(cvRect(i_left-size , j_center-size , block, block)),"���Сͼ.jpg");
	savePicture(value_x,"x��������˲�ͼ.jpg");
	//Top�б�ѡȡ
	filter2D(img(cvRect(i_center - size, j_top - size, block, block)), value_y, img.depth(), h_y, Point(-1, -1), (0, 0), BORDER_REFLECT);
	for (int j = block_size; j < block - block_size; j++)
	{
		float* data_y = value_y.ptr<float>(j);
		for (int i = block_size; i < block - block_size; i++)
		{
			if (data_y[i] > thr)
			{
				Mat roi(value_y, Rect(i - block_size, j - block_size, 2 * block_size + 1, 2 * block_size + 1));
				Point maxPoint, maxPoint_Img;
				minMaxLoc(roi, 0, 0, 0, &maxPoint);
				maxPoint.x = maxPoint.x + i - block_size;
				maxPoint.y = maxPoint.y + j - block_size;
				if (i == maxPoint.x && j == maxPoint.y){
					maxPoint_Img.x = i + i_center - size;
					maxPoint_Img.y = j + j_top - size;
					y_edge.push_back(maxPoint_Img);
				}
			}
		}
	}

	savePicture(img(cvRect(i_center-size , j_top-size , block, block)),"����Сͼ.jpg");
	savePicture(value_y,"x���򶥱��˲�ͼ.jpg");
	block = 140.0;//�ײ�̫խ�����
	//Bottom�б�ѡȡ
	filter2D(img(cvRect(i_center - size, j_bottom - size, block, block)), value_y, img.depth(), h_y, Point(-1, -1), (0, 0), BORDER_REFLECT);
	for (int j = block_size; j < block - block_size; j++)
	{
		float* data_y = value_y.ptr<float>(j);
		for (int i = block_size; i < block - block_size; i++)
		{
			if (data_y[i] > thr)
			{
				Mat roi(value_y, Rect(i - block_size, j - block_size, 2 * block_size + 1, 2 * block_size + 1));
				Point maxPoint, maxPoint_Img;
				minMaxLoc(roi, 0, 0, 0, &maxPoint);
				maxPoint.x = maxPoint.x + i - block_size;
				maxPoint.y = maxPoint.y + j - block_size;
				if (i == maxPoint.x && j == maxPoint.y){
					maxPoint_Img.x = i + i_center - size;
					maxPoint_Img.y = j + j_bottom - size;
					y_edge.push_back(maxPoint_Img);
				}
			}
		}
	}

	savePicture(img(cvRect(i_center-size , j_bottom-size , block, block)),"�ױ�Сͼ.jpg");
	savePicture(value_y,"x����ױ��˲�ͼ.jpg");

	//�ж�ѡ���Ƿ�׼ȷ
	if (x_edge.size() != 3 || y_edge.size() != 3){
		cout << "Error in getting edge!" << endl;
		* Laplace_Flag=-1;
		HV_Lap str1;
		return str1;
	}
	Mat imgpaint = img.clone();
	

	
	for (int a = 0;a<x_edge.size();a++)
	{

		circle(imgpaint, x_edge[a], 10, cvScalar(255, 0, 0), 2);
	}
	for (int a = 0;a<y_edge.size();a++)
	{
		circle(imgpaint, y_edge[a], 10, cvScalar(255, 0, 0), 2);
	}

	//����ͼƬ���ı�ԭ����ͼ��
	string filename = "�бߵ�_";
	static int index = 1;
	char buf[64] ={0};
	sprintf(buf,"%d",index++);
	filename.append(buf);
	filename.append(".jpg") ;
	savePicture(imgpaint,filename);
	//showPicture(imgpaint,"�бߵ�");
	imgpaint.release();

	HV_Lap str;
	Point Loc_center_x, Loc_left, Loc_right, Loc_center_y, Loc_top, Loc_bottom;
	Loc_center_x = x_edge[0];
	Loc_center_y = y_edge[0];
	Loc_right = x_edge[1];
	Loc_left = x_edge[2];
	Loc_top = y_edge[1];
	Loc_bottom = y_edge[2];

	str.horizontal_pt.push_back(Loc_left);
	str.horizontal_pt.push_back(Loc_center_x);
	str.horizontal_pt.push_back(Loc_right);
	str.vertical_pt.push_back(Loc_top);
	str.vertical_pt.push_back(Loc_center_y);
	str.vertical_pt.push_back(Loc_bottom);
	//cout<<img<<endl;
	//�����б������laplaceֵ
	str.horizontal_lap.push_back(laplace(img, Loc_left, edge_size));
	str.horizontal_lap.push_back(laplace(img, Loc_center_x, edge_size));
	str.horizontal_lap.push_back(laplace(img, Loc_right, edge_size));
	str.vertical_lap.push_back(laplace(img, Loc_top, edge_size));
	str.vertical_lap.push_back(laplace(img, Loc_center_y, edge_size));
	str.vertical_lap.push_back(laplace(img, Loc_bottom, edge_size));


	* Laplace_Flag=0;
	return str;
}

int savePicture(Mat &img, string filename)
{
	///��ʱ��Ϊ����img��Ϊcvflaot_32
	Mat I = img.clone();
	I = I*255;  
	I.convertTo(I, CV_8U);
	imwrite(filename,I);
	I.release();
	return 0;

}
int showPicture(Mat &img, const string windowname)
{
	cvNamedWindow(windowname.c_str(),WINDOW_NORMAL);
	imshow(windowname.c_str(),img);
	waitKey();
	cvDestroyWindow(windowname.c_str());
	return 0;
}

double laplace(Mat I, Point Loc, int edge_size)
{
	
	Mat myKernel = (Mat_<float>(3, 3) << -1, -1, -1, -1, 8, -1, -1, -1, -1);
	int x = Loc.x - edge_size;
	int y = Loc.y - edge_size;
	Mat lap_temp = I(cvRect(x, y, 2 * edge_size + 1, 2 * edge_size + 1));

	//cout<<lap_temp<<endl;
	Mat lap_temp2; double sum_lap = 0;
	filter2D(lap_temp, lap_temp2, lap_temp.depth(), myKernel, Point(-1, -1), (0, 0), BORDER_DEFAULT);
	for (int k = 0; k < lap_temp2.rows; k++){
		float* data = lap_temp2.ptr<float>(k);
		for (int n = 0; n < lap_temp2.cols; n++){
			sum_lap = sum_lap + pow(data[n], 2);
		}
	}
	//cout<<I<<endl;
	//cout<<lap_temp<<endl;
//	showPicture(lap_temp,"la1");
//	showPicture(lap_temp2,"la2");

	savePicture(lap_temp,"laplaceǰ.jpg");

	savePicture(lap_temp2,"laplace��.jpg");
	double lap = sum_lap / pow(double(2 * edge_size - 1), int(2));
	return lap;
}