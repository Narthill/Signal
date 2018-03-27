#include "checkHa.h"
#include "IAVisionAlgorithm.h"
#include <math.h>
//#include <opencv\cv.h>
//#include <opencv\highgui.h>
//#include <opencv2\gpu\device\vec_math.hpp>
#include <iostream>  
//#include <algorithm> 
//#include <vector>
//using namespace std;

//template<class T>  
//bool greaterThanPtr(T a ,T b)  
//{  
//	return a > b;  
//}

//void MygoodFeaturesToTrack( InputArray _image, OutputArray _corners,
//                              int maxCorners, double qualityLevel, double minDistance,
//                              InputArray _mask, int blockSize,
//                              bool useHarrisDetector, double harrisK )
//{
//    //�����Ҫ��_imageȫͼ���������_mask����cv::Mat()�����������Ȥ����
//	Mat image = _image.getMat(), mask = _mask.getMat();  
//
//    CV_Assert( qualityLevel > 0 && minDistance >= 0 && maxCorners >= 0 );  //�Բ�����һЩ����Ҫ��
//    CV_Assert( mask.empty() || (mask.type() == CV_8UC1 && mask.size() == image.size()) );
//
//    Mat eig, tmp;   //eig�洢ÿ������Э����������С����ֵ��tmp�������澭���ͺ��eig
//    if( useHarrisDetector )
//        cornerHarris( image, eig, blockSize, 3, harrisK ); //blockSize�Ǽ���2*2Э�������Ĵ��ڴ�С��sobel���Ӵ���Ϊ3��harrisK�Ǽ���Harris�ǵ�ʱ��Ҫ��ֵ
//    else
//        cornerMinEigenVal( image, eig, blockSize, 3 );  //����ÿ�����ض�Ӧ��Э����������С����ֵ��������eig��
//
//    double maxVal = 0;
//    minMaxLoc( eig, 0, &maxVal, 0, 0, mask );   //maxVal������eig�����ֵ
//    threshold( eig, eig, maxVal*qualityLevel, 0, THRESH_TOZERO );  //��ֵ����ΪmaxVal����qualityLevel�����ڴ���ֵ�ı��ֲ��䣬С�ڴ���ֵ�Ķ���Ϊ0
//    
//	//Ĭ����3*3�ĺ����ͣ�����֮�󣬳��˾ֲ����ֵ���ԭ����ͬ�������Ǿֲ����ֵ�㱻  
//    //3*3�����ڵ����ֵ��ȡ�����粻��⣬�ɿ�һ�»Ҷ�ͼ�������ԭ��  
//	dilate( eig, tmp, Mat());  //tmp�б���������֮���eig
//
//    Size imgsize = image.size(); 
//
//    vector<const float*> tmpCorners;  //��Ŵ�ѡ���Ľǵ��ַ
//
//    // collect list of pointers to features - put them into temporary image 
//    for( int y = 1; y < imgsize.height - 1; y++ )
//    {
//        const float* eig_data = (const float*)eig.ptr(y);  //���eig��y�е��׵�ַ
//        const float* tmp_data = (const float*)tmp.ptr(y);  //���tmp��y�е��׵�ַ
//        const uchar* mask_data = mask.data ? mask.ptr(y) : 0;
//
//        for( int x = 1; x < imgsize.width - 1; x++ )
//        {
//            float val = eig_data[x];
//            if( val != 0 && val == tmp_data[x] && (!mask_data || mask_data[x]) )  //val == tmp_data[x]˵�����Ǿֲ�����ֵ
//                tmpCorners.push_back(eig_data + x);  //������λ��
//        }
//    }
//
//	//-----------�˷ָ��������Ǹ�������ֵ��ѡ���Ľǵ㣬���ǳ�֮Ϊ���ǵ�----------//
//	//-----------�˷ָ������»�Ҫ����minDistance��һ��ɸѡ�ǵ㣬��Ȼ�ܴ�����������ǳ�֮Ϊǿ�ǵ�----------//
//
////    sort( tmpCorners, greaterThanPtr<float>() );  //������ֵ�������У�ע����һ������Ҫ������ĺܶ���˼·���ǽ���������������еĻ�����
//    vector<Point2f> corners;
//    size_t i, j, total = tmpCorners.size(), ncorners = 0;
//
//    //����ĳ����е���΢����⣬��Ҫ�Լ���ϸ����
//	if(minDistance >= 1)  
//    {
//         // Partition the image into larger grids
//        int w = image.cols;
//        int h = image.rows;
//
//        const int cell_size = cvRound(minDistance);   //�����������ȡ��
//
//	//�������cell_size������һ�����δ���grid(��Ȼ�����grid�������vector<vector>������������������˵�ľ��δ��ڣ���Ϊ�˱������,���ǽ�grid�����һ��grid_width * grid_height�ľ��δ��ڱȽϺ�)������cell_size˵��grid���������һ�������൱��_image�����minDistance�����أ�����Ϊʲô����cell_size - 1����ὲ
//        const int grid_width = (w + cell_size - 1) / cell_size; 
//        const int grid_height = (h + cell_size - 1) / cell_size;
//
//        std::vector<std::vector<Point2f> > grid(grid_width*grid_height);  //vector������vector��grid���������õ�ǿ�ǵ�����
//
//        minDistance *= minDistance;  //ƽ�������������㣬ʡ�Ŀ�����
//
//        for( i = 0; i < total; i++ )     // �ոմ�ѡ�����ǵ㣬��Ҫ��������������һ�ֵĿ���
//        {
//            int ofs = (int)((const uchar*)tmpCorners[i] - eig.data);  //tmpCorners�б����˽ǵ�ĵ�ַ��eig.data����eig�ڴ����׵�ַ
//            int y = (int)(ofs / eig.step);   //�ǵ���ԭͼ���е���
//            int x = (int)((ofs - y*eig.step)/sizeof(float));  //��ԭͼ���е���
//
//            bool good = true;  //����Ϊ��ǰ�ǵ��ܽ��տ��飬���ܱ���������
//
//            int x_cell = x / cell_size;  //x_cell��y_cell�ǽǵ㣨y,x����grid�еĶ�Ӧ����
//            int y_cell = y / cell_size;
//
//            int x1 = x_cell - 1;  // (y_cell��x_cell����4��������
//            int y1 = y_cell - 1;  //����֪��Ϊʲôǰ��grid_width����ʱҪ����cell_size - 1�˰ɣ�����Ϊ��ʹ�ã�y,x����grid�е�4�������ض����ڣ�Ҳ����˵(y_cell��x_cell�������Ϊ�߽�����
//            int x2 = x_cell + 1;  
//            int y2 = y_cell + 1;
//
//            // boundary check���ٴ�ȷ��x1,y1,x2��y2���ᳬ��grid�߽�
//            x1 = std::max(0, x1);  //�Ƚ�0��x1�Ĵ�С
//            y1 = std::max(0, y1);
//            x2 = std::min(grid_width-1, x2);
//            y2 = std::min(grid_height-1, y2);
//
//            //��סgrid�����һ�����أ��൱��_image�������minDistance������
//			for( int yy = y1; yy <= y2; yy++ )  // ��
//            {
//                for( int xx = x1; xx <= x2; xx++ )  //��
//                {
//                    vector <Point2f> &m = grid[yy*grid_width + xx];  //����
//
//                    if( m.size() )  //���(y_cell��x_cell)��4�������أ�Ҳ����(y,x)��minDistance�������������б�������ǿ�ǵ�
//                    {               
//                        for(j = 0; j < m.size(); j++)   //��ǰ�ǵ���Χ��ǿ�ǵ㶼����������ǰ�ǵ��һ��
//                        {
//                            float dx = x - m[j].x;
//                            float dy = y - m[j].y;
//			   //ע�����(y,x)��minDistance�������������б�������ǿ�ǵ㣬��˵����ǿ�ǵ�����(y,x)֮ǰ�ͱ����Թ��ģ�����ΪtmpCorners���Ѱ�������ֵ�������У�����ֵԽ��˵���ǵ�Խ�ã�����˵���Ȳ��Ե�һ���Ǹ��õĽǵ㣬Ҳ�����ѱ����ǿ�ǵ�һ�����ڵ�ǰ�ǵ㣬��������ֻҪ�ȽϾ��룬��������������������������ӵ���ǰ���ԵĽǵ�
//                            if( dx*dx + dy*dy < minDistance )
//                            {                              							
//				good = false;
//                                goto break_out;
//                            }
//                        }
//                    }
//                }   // ��
//            }    //��
//
//            break_out:
//
//            if(good)
//            {
//                // printf("%d: %d %d -> %d %d, %d, %d -- %d %d %d %d, %d %d, c=%d\n",
//                //    i,x, y, x_cell, y_cell, (int)minDistance, cell_size,x1,y1,x2,y2, grid_width,grid_height,c);
//                grid[y_cell*grid_width + x_cell].push_back(Point2f((float)x, (float)y));
//
//                corners.push_back(Point2f((float)x, (float)y));
//                ++ncorners;
//
//                if( maxCorners > 0 && (int)ncorners == maxCorners )  //����ǰ���Ѱ��������У���ncorners����maxCorners��ʱ������ѭ��ֱ�Ӻ���tmpCorners��ʣ�µĽǵ㣬����ʣ�µĽǵ�Խ��Խ��
//                    break;
//            }
//        }
//    }
//    else    //�������ر���û���ĸ������������뵱ǰ��������minDistance < 1,���ֱ�ӱ����ѡ�Ľǵ�
//    {
//        for( i = 0; i < total; i++ )
//        {
//            int ofs = (int)((const uchar*)tmpCorners[i] - eig.data);
//            int y = (int)(ofs / eig.step);   //��ѡ�Ľǵ���ԭͼ���е���
//            int x = (int)((ofs - y*eig.step)/sizeof(float));  //��ͼ���е���
//
//            corners.push_back(Point2f((float)x, (float)y));
//            ++ncorners;
//            if( maxCorners > 0 && (int)ncorners == maxCorners )  
//                break;
//        }
//    }
//
//    Mat(corners).convertTo(_corners, _corners.fixedType() ? _corners.type() : CV_32F);
//
//    /*
//    for( i = 0; i < total; i++ )
//    {
//        int ofs = (int)((const uchar*)tmpCorners[i] - eig.data);
//        int y = (int)(ofs / eig.step);
//        int x = (int)((ofs - y*eig.step)/sizeof(float));
//
//        if( minDistance > 0 )
//        {
//            for( j = 0; j < ncorners; j++ )
//            {
//                float dx = x - corners[j].x;
//                float dy = y - corners[j].y;
//                if( dx*dx + dy*dy < minDistance )
//                    break;
//            }
//            if( j < ncorners )
//                continue;
//        }
//
//        corners.push_back(Point2f((float)x, (float)y));
//        ++ncorners;
//        if( maxCorners > 0 && (int)ncorners == maxCorners )
//            break;
//    }
//*/
//}

void testcheckHa()
{
	Mat image;
	image = imread("test.jpg",IMREAD_UNCHANGED);
	vector<Point> TempB;
	int maxCorners =20;
	Mat maskB(image.rows,image.cols,CV_8UC1);
	double qualityLevel = 0.05;  //Ʒ������
	double minDistance = 10;  
	int blockSize = 3;  
	bool useHarrisDetector = false;  
	double k = 0.04;  

	//MygoodFeaturesToTrack(image,TempB,  maxCorners,   qualityLevel, minDistance, maskB, blockSize, useHarrisDetector, k);
	//if (TempB.size()!=maxCorners)
	//{
	//	printf("û�ҵ�%d����",maxCorners);
	//	return ;

	//}
	float ithro =0.25;
	for (int i = 0; i < TempB.size(); i++){
		float a = image.at<float>(TempB[i].y-7 , TempB[i].x-7);
		if (a< ithro){
			/*PointI.push_back(TempB[i]);*/
			circle(image, Point(TempB[i].x,TempB[i].y), 4, Scalar(0,255,0), 2, 8, 0 ); 
			//cout <<TempI[i] << endl;
		}
	}

	IAVO_ShowPicture("1",image);


}