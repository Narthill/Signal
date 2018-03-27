// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� ROBOTLIB_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// ROBOTLIB_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef ROBOTLIB_EXPORTS
#define ROBOTLIB_API __declspec(dllexport)
#else
#define ROBOTLIB_API __declspec(dllimport)
#endif

typedef int BOOL;

// ��һ����̬���ص���ʾ
ROBOTLIB_API int unitTest(void);

//.......................�����ϻ����ˣ�����ȡ���ã�..................................
// �����ϻ�����
//   @Input:   iComPort         ����
//             iBaudRate        ������ 115200
//   @Return�� ID��             ���ӳɹ�
//             FALSE            ����
ROBOTLIB_API int connectToRobot(int iComPort, int iBaudRate);


//............................��������................................................
// Reference,����
//   @Input:   ID                ������ID
//             axisArray         ���
//   @Return�� TRUE              ���óɹ�
//             FALSE             ����
ROBOTLIB_API BOOL Reference(int ID, char* axisArray);


// .........................��������λ.................................................
// ���ö�Ӧ��������Ӧ�������λ��Χ�����ֵ
//   @Input:   ID                ������ID
//             axisArray         ��
//             SoftLimitLowEnd   ��������λ��Χ�����ֵ�������ֵ
//             LowOrHigh         LowOrHigh=1Ϊ����λ��Χ�����ֵ,LowOrHigh=0Ϊ����λ��Χ�����ֵ
//   @Return�� TRUE              ���óɹ�
//             FALSE             ����
ROBOTLIB_API BOOL setPositionSoftLimit(int ID, const char* axisArr, const double* SoftLimitLowEnd, int LowOrHigh);


// .........................��ȡ����λ.................................................
// ��ȡ��Ӧ��������Ӧ�������λ��Χ�����ֵ�����ֵ
//   @Input:   ID                ������ID
//             axisArray         ��                
//             LowOrHigh         LowOrHigh=1Ϊ����λ��Χ�����ֵ,LowOrHigh=0Ϊ����λ��Χ�����ֵ
//   @Output:  SoftLimitLowEnd   ��������λ��Χ�����ֵ�������ֵ 
//   @Return�� TRUE              ��ȡ����λ�ɹ�
//             FALSE             ����
ROBOTLIB_API BOOL getPositionSoftLimit(int ID, const char* axisArr, double* SoftLowLimit, int LowOrHigh);


// .........................��ȡ��ǰλ�õ�����.................................................
// ��ȡ��ǰ����
//   @Input:   ID                ������ID
//             axisArray         ��                
//   @Output:  posArr            ���յ�ǰ����
//   @Return�� TRUE              ��ȡ��ǰ����ɹ�
//             FALSE             ����
ROBOTLIB_API BOOL getCurrentPosition(int ID, const char* axisArr, double* posArr);

// .........................���õ�ǰ����.................................................
// ���õ�ǰ����
//   @Input:   ID                ������ID
//             axisArray         ��                
//             posArr            ���õ�ǰ����
//   @Return�� TRUE              ��ȡ��ǰ����ɹ�
//             FALSE             ����
ROBOTLIB_API BOOL setCurrentPosition(int ID, const char* axisArr, const double* posArr);


// .........................�������ĵ�����.................................................
// �������ĵ�����
//   @Input:   ID                ������ID
//             axisArray         ��                
//   @Return�� TRUE              ��ȡ��������ϵԭ��ɹ�
//             FALSE             ����
ROBOTLIB_API BOOL setPivotPointCoordinates(int ID, const char* axisArr,const double* pdValueArray);


// .........................��ȡ���ĵ�����.................................................
// ��ȡ���ĵ�����
//   @Input:   ID                ������ID
//             axisArray         ��                
//   @Output:  posArr            ���չ�������ϵԭ��ֵ
//   @Return�� TRUE              ��ȡ��������ϵԭ��ɹ�
//             FALSE             ����
ROBOTLIB_API BOOL getPivotPointCoordinates(int ID, const char* axisArr, double* posArr);



// .........................��ȡ��ǰӲ����λ��Χ.................................................
// ��ȡ��ǰӲ����λ��Χ
//   @Input:   ID                ������ID
//             axisArray         ��                
//             LowOrHigh         LowOrHigh=1Ϊ��ȡ��ǰӲ����λ��Χ�����ֵ,LowOrHigh=0Ϊ��ȡ��ǰӲ����λ��Χ�����ֵ
//   @Output:  rangeArr          ���յ�ǰӲ����λ��Χ��LowOrHigh=1Ϊ��ȡ��ǰӲ����λ��Χ�����ֵ,LowOrHigh=0Ϊ��ȡ��ǰӲ����λ��Χ�����ֵ
//   @Return�� TRUE              ��ȡ��ǰӲ����λ��Χ�ɹ�
//             FALSE             ����
ROBOTLIB_API BOOL getEndTravelRange(int ID, const char* axisArr, double* rangeArr, int LowOrHigh);



// �����ٶ�
//   @Input:   ID                ������ID
//             axisArray         ��                
//             speedArr          ���õ��ٶ�
//   @Return�� TRUE              �ɹ�
//             FALSE             ����
ROBOTLIB_API BOOL setAxisVelocity(int ID, const char* axisArr, const double* speedArr);


//��ȡ�ٶ�
//   @Input:   ID                ������ID
//             axisArray         ��                
//   @Output:  speedArr          ��ȡ�ٶ�
//   @Return�� TRUE              �ɹ�
//             FALSE             ����
ROBOTLIB_API BOOL getCurrentVelocity(int ID, const char* axisArr, double* speedArr);


// ����һ�β����ľ���
//   @Input:   ID                ������ID
//             axisArray         ��                
//             speedArr          ����һ�β����ľ���
//   @Return�� TRUE              �ɹ�
//             FALSE             ����
ROBOTLIB_API BOOL setStepSizeDistance(int ID, const char* axisArr, double* stepArr);

// �ߵ�һ��Ŀ���
//   @Input:   ID                ������ID
//             axisArray         ��                
//             targetArr          Ŀ����ֵ
//   @Return�� TRUE              �ɹ�
//             FALSE             ����
ROBOTLIB_API BOOL MoveTo(int ID, const char* axisArr, const double* targetArr);

// ��ԭ��
//   @Input:   ID                ������ID
//             axisArray         ��                
//   @Return�� TRUE              �ɹ�
//             FALSE             ����
ROBOTLIB_API BOOL GoToHomePosition(int ID, const char* axisArr);



//defines a new Tool coordinate system (KST type).
//   @Input:   ID                    ������ID
//             szNameOfCoordSystem   ������ϵͳ������
//             axisArray             ��   
//             ToolCoordinateValue          
//   @Return�� TRUE                  �ɹ�
//             FALSE                 ����
ROBOTLIB_API BOOL DefineToolCoordinate(int ID, const char* szNameOfCoordSystem, const char* axisArr, const double* ToolCoordinateValue);


//Move szAxes relative to current position and orientation in Tool coordinate system.
//   @Input:   ID                    ������ID
//             axisArray             ��   
//             MoveToolDistance      �ƶ��������ֵ����������ϵ��   
//   @Return�� TRUE                  �ɹ�
//             FALSE                 ����
ROBOTLIB_API BOOL MoveRelativeToToolCoordinateSystem (int ID, const char* axisArr, const double* MoveToolDistance);

//Move szAxes relative to current position and orientation in Work coordinate system
//   @Input:   ID                    ������ID
//             axisArray             ��   
//             MoveToolDistance      �ƶ��������ֵ����������ϵ��   
//   @Return�� TRUE                  �ɹ�
//             FALSE                 ����
ROBOTLIB_API BOOL MoveRelativeToWorkCoordinateSystem (int ID, const char* axisArr, const double* pdValueArray);


//defines a new Work coordinate system
//   @Input:   ID                    ������ID
//             szNameOfCoordSystem   ��������ϵ������
//             axisArray             ��   
//             MoveToolDistance      ���ֵ����������ϵ��   
//   @Return�� TRUE                  �ɹ�
//             FALSE                 ����
ROBOTLIB_API BOOL NewWorkCoordinateSystem (int ID, const char* szNameOfCoordSystem, const char* axisArr, const double* pdValueArray);


//Enables an already defined coordinate system.
//   @Input:   ID                    ������ID
//             EnabledCoordinateName Ҫ���õ�����ϵ����
//   @Return�� TRUE                  �ɹ�
//             FALSE                 ����
ROBOTLIB_API BOOL EnabledCoordinateSystems (int ID, const char* EnabledCoordinateName);

//Lists enabled coordinate systems by name.
//   @Input:   ID                        ������ID
//             EnabledCoordinateNameList Ҫ���õ�����ϵ����
//             buffer                    ����
//             bufsize                   �����С
//   @Return�� TRUE                      �ɹ�
//             FALSE                     ����
ROBOTLIB_API BOOL ListsEnabledCoordinateSystems(int ID, const char* EnabledCoordinateNameList, char* buffer, int bufsize);