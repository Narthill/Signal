
#pragma once
//#include "HDevEngineCpp.h"
//using namespace HDevEngineCpp;

#include <vector>
#include <string>
#include "AAalgorithm.h"
using namespace std;
#define PI_AXIS_MAXNUM 6
#define PI_ROBOT_MAXNUM 4
#define NETDEVICESLEN 1000
#define TIMER_EDIT_TEXT         3   //���±༭���ַ���
#define TIMER_SHOW_MSGBOX       9   //�ӳٵ�����Ϣ��ʾ��
#define OnePIcharMaxLen         64

#define PRINTF(msg, ...) printf(msg, __VA_ARGS__)


#define LOG_MAX_BUFF    1024
#define AAROBOTBIND "AAROBOTBIND"
#define AAROBOT1 "robot1"
#define AAROBOT2 "robot2"
#define AAROBOT3 "robot3"
#define AAROBOT4 "robot4"

#define  AA_Z "AAadjust_z"

#define AA_Adjustparam "AA_Adjustparam"
#define AAadjust_zMaxNum "AAadjust_zMaxNum"
#define AA_zzMaxnum  10
#define AAadjust_zthr "AAadjust_zthr"//z��ֵ

#define  AA_UV "AAadjust_uv"
#define AAadjust_uvMaxNum "AAadjust_uvMaxNum"
#define AA_uvMaxnum  10
#define AAadjust_uvthr "AAadjust_uvthr"//uv��ֵ

//�㷨�õ�����Թ̶��ĳߴ�
#define AA_Globle_param "AA_Globle_param" //��
#define AA_focus  "focus" //����
#define AA_objdis "objdis" //���
#define AA_size	"size"    //ѡ��С��ߴ�
#define AA_block	"block" //ѡ����ߴ�
#define AA_edge_size	"edge_size" //������˹ֵ�߾�
#define AA_block_size	"block_size" //ģ��߳���һ��
#define AA_pic_width	"pic_width"  //ͼ���С���Ƿ���Ҫ������
#define AA_pic_length	"pic_length" //
#define AA_scale	"scale"         //ͼ�����ű���
#define AA_pixel_size "pixel_size"  // ���ش�С
#define AA_t_mapsize	"t_mapsize"  //��ͼС�ڿ��С
#define AA_t_mapdiswidth	"t_mapdiswidth" //��ͼ���߷����������ڿ�֮��ľ���
#define AA_t_mapdislength	"t_mapdislength" //��ͼ�̱߷����������ڿ�֮��ľ���

//����˫����ֵ������
#define Adjuest_Method	"Adjuest_Method"
#define doubleAA_Uth	"doubleAAUth"
#define doubleAA_Vth	"doubleAAVth"
#define doubleAA_Wth	"doubleAAWth"
#define doubleAA_Unum	"doubleAAUnum"
#define doubleAA_Vnum	"doubleAAVnum"
#define doubleAA_Wnum	"doubleAAWnum"



//5�㷨����ʱ��z���
#define AA_Mirco_disBig 0.006
#define AA_Mirco_disSmall 0.003


//�Ӿ�������
#define AA_MAXVISION 8
#define AA_VISION_RETAGLELOC 7  //˳��Ϊ���������� ��λ �Ҷ�λ


#define AAVISION_PATH "AAVISION_PATH"
#define AACARERA       "Camera_"

#define AAVISION_MCLK "AAVISION_MCLK"
#define Mclk    "Mclk"

#define AAVISION_POWER "AAVISION_POWER"
#define Avdd "Avdd"
#define Dvdd "Dvdd"
#define Dovdd "Dovdd"
#define Afvcc "Afvcc"
#define Vpp  "Vpp"

#define AAVISION_AF "AAVISION_AF"
#define FocusPos "FocusPos"

#define AAVISION_GRAIN      "AAVISION_GRAIN"
#define AAGRAIN "grain"
#define AAVISION_EXPOSURE  "AAVISION_EXPOSURE"
#define AAEXPOSURE "exposure"

#define AAVISION_USERNAME "AAVISION_USERNAME"
#define UserName8 "UserName8"
#define UserName7 "UserName7"
#define UserName6 "UserName6"
#define UserName5 "UserName5"
#define UserName4 "UserName4"
#define UserName3 "UserName3"
#define UserName2 "UserName2"
#define UserName1 "UserName1"

//ÿ���Ӿ�ͨ����ģ����ܲ�һ��,Ҳ����ѡȡ�����ĵ㲻һ��,���ɽ�������õ���ʽ
#define AAVISION_TEMPLET "AAVISION_TEMPLET"
#define Camera_1 "Camera_1"
#define Camera_2 "Camera_2"
#define Camera_3 "Camera_3"
#define Camera_4 "Camera_4"
#define Camera_5 "Camera_5"
#define Camera_6 "Camera_6"
#define Camera_7 "Camera_7"
#define Camera_8 "Camera_8"



//�����˳�ʼλ������
#define AA_Robot_InitialPos "AA_Robot_InitialPos" //��
#define AA_Robot1_InitPos  "Robot1_InitPos" //������1
#define AA_Robot2_InitPos  "Robot2_InitPos" //������1
#define AA_Robot3_InitPos  "Robot3_InitPos" //������1
#define AA_Robot4_InitPos  "Robot4_InitPos" //������1
//������ץȡ��ͷλ��
#define AA_Robot_CameraPos "AARobot_CameraPos"
#define AA_Robot1_CameraPos "Robot1_CameraPos"
#define AA_Robot2_CameraPos "Robot2_CameraPos"
#define AA_Robot3_CameraPos "Robot3_CameraPos"
#define AA_Robot4_CameraPos "Robot4_CameraPos"

//��Ƶͨ����ѡȡλ��
#define AA_Reginal_param "AA_Reginal_param"
#define Channel_Retagle "Channel_Retagle"
#define Channe2_Retagle "Channe2_Retagle"
#define Channe3_Retagle "Channe3_Retagle"
#define Channe4_Retagle "Channe4_Retagle"
#define Channe5_Retagle "Channe5_Retagle"
#define Channe6_Retagle "Channe6_Retagle"
#define Channe7_Retagle "Channe7_Retagle"
#define Channe8_Retagle "Channe8_Retagle"



#define AACoreCfg "AACoreCfg"
#define savepicture "savepicture" //������ͼƬҲӦ����ȫ�ֲ�����ȥ,ͬAA_TEST_TYPE:0:����,1:˫��

#define AA_TEST_TYPE "AA_TEST_TYPE"  //AA_TEST_TYPE:0:����,1:˫��
#define isRobot "isRobot"            //�˶������Ƿ��ǻ�����
#define AA_DOUBLEAASCALE "doubleAAscale" //˫�����С����
#define AA_DOUBLEAADIS "doubleAAdis" //˫�����ʱ����оƬ�ľ���
#define AA_DOUBLEAASIZE "doubleAAsize"  //ѡ���С
#define AA_DOUBLEAADISWIDTH "doubleAAmapdiswidth" //���·������
#define AA_DOUBLEAADISLENGTH "doubleAAmapdislength" //���ҷ������
#define AA_DOUBLEAAPICCENTERX "doubleAApiccenterX" //����С�������x
#define AA_DOUBLEAAPICCENTERY "doubleAApiccenterY" //����С�������y
#define AA_DOUBLEAACENTROIDTHRESHOLD "doubleAACentroidthreshold" //������ֵ

#define AA_DOUBLEAAU_THRESHOLD "doubleAAUth" //u��ֵ
#define AA_DOUBLEAAV_THRESHOLD "doubleAAVth" //v��ֵ
#define AA_DOUBLEAAW_THRESHOLD "doubleAAWth" //
#define AA_DOUBLEAAU_NUM "doubleAAUnum" //u��������
#define AA_DOUBLEAAV_NUM "doubleAAVnum" //
#define AA_DOUBLEAAW_NUM "doubleAAWnum" //
#define AA_AdjuestMethod  "Adjuest_Method"

//���Ӹ�������ͽǶ�
#define AA_DOUBLEAAW_LOCDIS "double_locdis" //
#define AA_DOUBLEAAW_LOCANGLE  "double_locangle"



//ͼ��򲼾�:1,3�� ��һ����3������3��1����������5����,������1���֣����2����
#define AAVISONTOP 5
#define AAVISONLR 0 //���� ��û��
#define AAVISONDIS 2

enum AATESTTYPE //˫�㻹�ǵ���
{
	SINGLEAA,//����
	DOUBLEAA//˫��
};

enum AATESTREGIONALLOCATION //����λ��
{
	AA_TOP,
	AA_BOTTOM,
	AA_CENTER,
	AA_LEFT,
	AA_RIGHT,
	AA_LEFT_LOC,
	AA_RIGHT_LOC,
	AA_MAX
};

enum AATESTREGIONALTYPE //��������
{
	AATYPE_RETANGLE1,
	/*AATYPE_RETANGLE2,
	AATYPE_CIRCLE,*/
	AATYPE_MAX
};


enum INITPOSINDEX //��ʼλ��
{
	AAINDEX1,//�Ӿ�ͷǰ��ʼλ��
	AAINDEX2//����ǰ��ʼλ��
};

enum ROBOTINDEX
{
	robotindex1,
	robotindex2,
	robotindex3,
	robotindex4
};




#define AA_Paint "AAPaint" //��ͼ
#define AA_VISION_CENTET "VISION_CENTET"  //����ǰ׺


#define AA_TOOL "TOOL"  //��������ϵ
#define AA_TOOL2 "TOOL2"

//���������
#define  INVALID_RESULT -0xFFFF
#define AA_OK 0
#define AA_BASE_ERROR_CODE -100
#define AA_GET_CURRENTPT_FAIL AA_BASE_ERROR_CODE-1 //��ȡ��ǰλ��ʧ��
#define AA_GO_RERATIVEPT_FAIL AA_BASE_ERROR_CODE-2  //���λ��ʧ��

#define AA_Z_ADJUST_BIG_FAIL      AA_BASE_ERROR_CODE-20  //Z�᷶Χ̫��

//���������
#define AA_GET_PICTIRE_FAIL AA_BASE_ERROR_CODE-100 //��ȡͼ��ʧ��

//����������
#define AA_INVALID_INPUT AA_BASE_ERROR_CODE-200 //�����������
#define AA_ADJUST_FAIL AA_BASE_ERROR_CODE-201


#define AAVISION_MAXGRAIN 0x0100
#define AAVISION_MAXEXPURE 0x005ff


#define ZERO_CHK(p) if (!(p)) \
{    \
	return; \
}
#define ZERO_CHK2(p,q) if (!(p)) \
{    \
	return q; \
}

typedef struct{ 
	double X;//��
	double Y;
}AA_POINT;



typedef struct{ 
	double ltx;
	double lty;
	double rbx;
	double rby;
}AA_RETANGLE1;



typedef struct{ 
	int mapsize;
	int	size;
	int	block;
	int	edge_size;
	int	block_size;
	int	pic_width;
	int	pic_lenth;		
	double focus;
	double objdis;
	double	scale;
	double pixel_size;
	double	mapdiswidth;
	double	mapdislength;
	bool    iscall_halprocess;
	string  halconprocess;
	int temPlate[AA_MAXVISION];
	double robot1InitPos2[PI_AXIS_MAXNUM];
	double robot2InitPos2[PI_AXIS_MAXNUM];
	double robot3InitPos2[PI_AXIS_MAXNUM];
	double robot4InitPos2[PI_AXIS_MAXNUM];

	double robot1InitPos1[PI_AXIS_MAXNUM];
	double robot2InitPos1[PI_AXIS_MAXNUM];
	double robot3InitPos1[PI_AXIS_MAXNUM];
	double robot4InitPos1[PI_AXIS_MAXNUM];


	AA_POINT paintCenter[AA_MAXVISION];

	int AATYPE;
	int IsRobot;
	//˫���������
	double doubleAAscale;
	double doubleAAdis;
	double doubleAACentroidthreshold;
	double doubleAAsize;//ѡ���С
	double doubleAAmapdiswidth; //�����ĵ�ľ���1
	double doubleAAmapdislength;//�����ĵ�ľ���2
	double doubleAApiccenterX; //��ͼ����x
	double doubleAApiccenterY;//��ͼ����y

	//˫����ֵ������
	int AdjuestMethod;
	int doubleAAUnum;
	int doubleAAVnum;
	int doubleAAWnum;
	double doubleAAUth;
	double doubleAAVth;
	double doubleAAWth;

	double double_locdis;
	double double_locangle;


	//��������
	AA_RETANGLE1 channel1Retangle[AA_MAXVISION][AA_VISION_RETAGLELOC];
	//���������е����꣬��һά����0~7ͨ�� �ڶ�ά���� �������� ÿ��������rows��cols 
	double centerpoint[AA_MAXVISION][AA_VISION_RETAGLELOC*2];

}AA_Globle_Param;

//pi ��ip��port
typedef struct{ 
	string ip;
	int	port;

}PI_NET_PARAM;

enum AA_CAMERA_PARAM
{
	AA_EXP,
	AA_GRAIN
};
enum AA_ROBOT_AXIS
{
	AA_AXISX,
	AA_AXISY,
	AA_AXISZ,
	AA_AXISU,
	AA_AXISV,
	AA_AXISW
};

//���ú�����

void ReadAA_Globle_ParamValue();
void SaveAA_Globle_ParamValue(bool save=0);

//��ȡ���澵ͷ��ʼλ��
void ReadAA_InitPos_ParamValue();
void SaveAA_InitPos_ParamValue(double *pos,int num,int robot_idx,int index,bool save=0);

//��ȡ����ѡ������ĵ�λ,��ʱֻ�о��ε����ϵ�����µ�
void ReadAA_Reginal_ParamValue();
void SaveAA_Reginal_ParamValue(double *pos,int num,int robot_idx,int index,bool save=0);

void ReadAAAdjustZValue(double *z,int num);//��ȡAA������zֵ
void SaveAAAdjustZValue(double *z,int num,bool save= 0);

void ReadAAAdjustUVValue(double *z,int num);//��ȡAA������UVֵ
void SaveAAAdjustUVValue(double *z,int num,bool save= 0);


 int ParsePIipAndPort(string szdevbuf,PI_NET_PARAM &pinetparam);//����pi��ip�Ͷ˿�

 int ReadPosStr_toDouble(const char *robot_RedCamPos,double * robot_red_cam_pos );
 double stringToDouble(const string& str);

// void ReadRobotBind(map<int,string >&robotIDmap);
 //void SaveRobotBind(map<int,string >&robotIDmap,bool save= 0);


 int ReadPaintParam();
 void SavePaintParam(bool save= 0);

 string FindbinderUsernameByName(string username);

 void msg(const char* lpszFmt,...);

 void copy_variable(int a,int b,vector<MyPoints> *A,vector<MyPoints> *B);