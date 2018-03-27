#pragma once
#ifdef AAROBOTLIB_EXPORTS
#define AAROBOTLIB_API __declspec(dllexport)
#else
#define AAROBOTLIB_API __declspec(dllimport)
#endif

#define AA_PI_HAVE_CONNECT -2 //��ʱ�õĴ�����,�������Թ���һ���������ļ�


/********************************************************************/
/*������:AARobotConnectTCPIPbyDescription                                     */
/*����:�ر�����                                                     */
/*����:const char* szDescription �����������ʶ                    */
/*                                    */
/*                                     */
/*���:��                                                           */
/*����ֵ:int ������id��<0������                                 */
/********************************************************************/
AAROBOTLIB_API int AARobotConnectTCPIPbyDescription (const char* szDescription) ;


/********************************************************************/
/*������:AARobotEnumerateTCPIPDevices                                     */
/*����:�ر�����                                                     */
/*����:char* szBuffer ö�ٵ������л����������ʶ                    */
/*    int iBufferSize    szBuffer��С                               */
/*    const char* szFilter  ������                                  */
/*���:��                                                           */
/*����ֵ:int ö�ٵ���������<0������                                 */
/********************************************************************/
AAROBOTLIB_API int AARobotEnumerateTCPIPDevices (char* szBuffer, int iBufferSize, const char* szFilter) ;

/*.......................������c�������ӳ�ʼ��..................................
 *   @Input:   comPort         ���ں�
 *   @Return�� >= 0 �ɹ��� < 0 ʧ��
 */
AAROBOTLIB_API int AARobotInit(int comPort);

/*.......................������TCPIP���ӳ�ʼ��..................................
 *   @Input:   NULL
 *   @Return�� >= 0 �ɹ��� < 0 ʧ��
 */
AAROBOTLIB_API int AARobotTCPIPInit();//��ɾ��

/********************************************************************/
/*������:AARobotTCPIPInitOne                                 */
/*����:��ʼ��ָ��ip�Ͷ˿ڵĿ�����                                   */
/*����:const char* ip ������ip                                      */
/*����:int port �������˿�                                          */
/*���:��                                                           */
/*����ֵ:0���ɹ�������:ʧ��                                         */
/********************************************************************/
AAROBOTLIB_API int AARobotTCPIPInitOne(const  char* ip, int port);//



/********************************************************************/
/*������:AARobotIsConnected                              */
/*����:ͨ��id���������Ƿ�����                                  */
/*����:int ID                                      */
/*                                          */
/*���:��                                                           */
/*����ֵ:1�������ӣ�û����                                        */
/********************************************************************/
AAROBOTLIB_API  bool AARobotIsConnected (int ID) ;

/********************************************************************/
/*������:AARobotIsConnectedByIpAndPort                              */
/*����:ͨ��ip��port���������Ƿ�����                                  */
/*����:int ID                                      */
/*                                          */
/*���:��                                                           */
/*����ֵ:1�������ӣ�û����                                        */
/********************************************************************/
AAROBOTLIB_API  bool AARobotIsConnectedByIpAndPort(const char* ip, int port) ;


/********************************************************************/
/*������:AARobotCloseConnection                               */
/*����:�ر�����                                  */
/*����:int ID                                      */
/*                                          */
/*���:��                                                           */
/*����ֵ:void                                     */
/********************************************************************/
AAROBOTLIB_API void AARobotCloseConnection (int ID);

/*.......................�����������ƫ�ƣ���Χ�������������ת..................................
 *   @Input:		axisString			��Ҫ�˶���������ɵ��ַ���
 *												����Ϊ  X��Y��Z��U��V��W����
 *												�����м䣬��Ҫ�ÿո�ָ�
 *												���磬��Ҫ�ƶ�X��Y��Zʱ��axisString Ϊ ��   "X  Y  Z"
 *												��Ҫѡ��U��Vʱ��axisStringΪ��      "U  V"
 *					posArray			��Ҫ�ߵ�ƫ���������ף���ƫ�ƽǶȣ��ȣ�
 *												��һ�����飬���м������ݾ���Ҫ��������Ԫ��
 *												���磬��ҪX��Y��Z���ƶ�0.1mm����posArray���ݳ���Ϊ3��3��Ԫ�ض�Ϊ0.1
 *   @Return�� >= 0 �ɹ��� < 0 ʧ��
 */
AAROBOTLIB_API int AARobotMoveRelative2(const char *axisString, const double *posArray);

AAROBOTLIB_API int AARobotMoveRelative(int controlID,const char *axisString, const double *posArray);


/*............................��ȡ�����˶�Ӧ��ĵ�ǰλ��..........................................
 *   @Input:		axisString			��Ҫ�˶���������ɵ��ַ���
 *												����Ϊ  X��Y��Z��U��V��W����
 *												�����м䣬��Ҫ�ÿո�ָ�
 *												���磬��Ҫ��ȡX��Y��Z�ᵱǰλ��ʱ��axisString Ϊ ��   "X  Y  Z"
 *												��Ҫѡ��U��Vʱ��axisStringΪ��      "U  V"
 *   @Output:		posArray			��ȡ��ĵ�ǰ��ƫ���������ף���ǰƫ�ƽǶȣ��ȣ�
 *												��һ�����飬���м������ݾ���Ҫ��������Ԫ��									
 *   @Return�� >= 0 �ɹ��� < 0 ʧ��
 */
AAROBOTLIB_API int AARobotCurrentPosition2(const char *axisString, double *posArray);//��ɾ����
AAROBOTLIB_API int AARobotCurrentPosition(int controlID,const char *axisString, double *posArray);


//�ƶ�������λ��
AAROBOTLIB_API int AARobotAbsMove(int controlID,const char *axisString,const double* pos);//

//���嵱ǰλ��Ϊԭ��
AAROBOTLIB_API int AARobotDfhHome(int controlID,const char *axisString);//

/***************************************************************/
/*�����˻�ԭ,ԭ��Ĭ������������ϵԭ��

*/
AAROBOTLIB_API int AARobotGoHome(int controlID,const char *axisString);//û��


// ����ϵͳ�ٶ�
//   @Input:   ID                ������ID              
//             speedArr          ���õ��ٶ�
//   @Return�� TRUE              �ɹ�
//             FALSE             ����
AAROBOTLIB_API int setSystemVelocity(int ID,  double speedArr);


//��ȡϵͳ�ٶ�
//   @Input:   ID                ������ID
//             axisArray         ��                
//   @Output:  speedArr          ��ȡ�ٶ�
//   @Return�� TRUE              �ɹ�
//             FALSE             ����
AAROBOTLIB_API int getSystemVelocity(int ID, double* speedArr);




// .........................��ȡ���ĵ�����.................................................
// ��ȡ���ĵ�����
//   @Input:   ID                ������ID
//             axisArray         ��                
//   @Output:  posArr            ���չ�������ϵԭ��ֵ
//   @Return�� TRUE              ��ȡ��������ϵԭ��ɹ�
//             FALSE             ����
AAROBOTLIB_API int getPivotPointCoordinates(int ID, const char* axisArr, double* posArr);




AAROBOTLIB_API int getKLTCoordinates(int ID, const char* szStartCoordSystem, const char* szEndCoordSystem, char* buffer, int bufsize);


//Enables an already defined coordinate system.
AAROBOTLIB_API int enableCoordinate(int ID, const char* szNameOfCoordSystem);


//����һ��KST����ϵ
AAROBOTLIB_API int defineKSTCoordinate(int ID, const char* szNameOfCoordSystem, const char* axisArr,  double* ToolCoordinateValue);
//ɾ��һ������ϵ
AAROBOTLIB_API int deleteOneCoordinate(int ID, const char* szNameOfCoordSystem);
//����һ������ϵ
 AAROBOTLIB_API int activeOneCoordinate(int ID, const char* szNameOfCoordSystem);
 //��ѯ���м��������ϵ������������
 AAROBOTLIB_API int queryActiveCoordinates(int ID, const char* szNamesOfCoordSystems, char* buffer, int bufsize);
 //����������λ
 AAROBOTLIB_API int setPosSoftLimit(int ID, const char* szAxes, const double* pdValueArray);
 //���ø�����λ
 AAROBOTLIB_API int setNegSoftLimit(int ID, const char* szAxes, const double* pdValueArray);
 //��������λ
 AAROBOTLIB_API int activeSoftLimit(int ID, const char* szAxes, const int* pbValueArray);