function offset = getOffcenter(img, standard_img, s, f, ccd_size)
%get off-center distance
%============================================
%Author:Chame
%Date:2017.1.14
%============================================
%Input
% img: input image
% standard_img: standard image;
% s: the distance from the checkboard to the CCD sensor
% f: focus distance 
%--------------------------------------------
%Output
% offset: [x_offset y_offset]
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
x_distance = x_distance * ccd_size;
y_distance = y_distance * ccd_size;

u = (s + sqrt(s^2 - 4*s*f)) / 2;
v = (s - sqrt(s^2 - 4*s*f)) / 2;

x_offset = x_distance ./ (v/u + 1);
y_offset = y_distance ./ (v/u + 1);
offset = [x_offset, y_offset];
end