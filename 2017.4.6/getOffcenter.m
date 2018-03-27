function [x_offset, y_offset] = getOffcenter(img, standard_img, pixel_size)
%get off-center distance
%============================================
%Author:Chame
%Date:2017.3.24
%============================================
%Input
% img: input image
% standard_img: standard image;
% pixel_size: the size of CCD pixel
%--------------------------------------------
%Output
% x_offset, y_offset: horizontal(x) offset and vertical(y) offset
%============================================
narginchk(5,5);
%% ���ǵ�
fixedPoints = detectHarrisFeatures(standard_img);
movingPoints = detectHarrisFeatures(img);

fixedLocs = fixedPoints.Location;
movingLocs = movingPoints.Location;
%% ����ʵ��ͼ�����׼ͼ�������ƫ��
x_distance = mean(movingLocs(:,1) - fixedLocs(:,1));
y_distance = mean(movingLocs(:,2) - fixedLocs(:,2));
%% ת��Ϊ��ռ��ƶ�����
x_offset = x_distance * pixel_size;
y_offset = y_distance * pixel_size;
end