/////////////////////////////////////////////////////////////
// All Rights Reserved by DOTHINKEY
// Last Update:2010 05 22
/////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "resource.h"
#include "AAvision.h"

#include "threads.h"
#include <process.h>    //for thread
#include "tvdecode.h"

#include "dtccm2.h" 

#include "AA_paint.h"
#include "AA_define_tool.h"
#include "AAvisionthreads.h"

//#include "HObject.h"

#define  MAXPICNUMEVESCENE 8

//FrameInfo m_FrameInfoA;
//FrameInfo m_FrameInfoB;


extern  AA_Globle_Param g_AAParam;//����ȫ�ֱ���

HObject m_pImage[MAXPICNUMEVESCENE] ;
 
//UINT AAFrameCnt = 0;
//UINT AAPlayCnt = 0;
void Camera_ThreadA(void *param)
{

	
    CAAVision *pVideo = (CAAVision *)param;
	pVideo->AAPlayCnt++;
	UINT timeStart = 0;
	UINT timeStop = 0;
	UINT FrameNum = 0;
	ULONG RetSize = 0;
	
	UINT DeviceErrCnt = 0;
	CString FpsInfo;
	int x = 0;

	int bRet = 0;
	WORD ImgH = 0, ImgV = 0;

	UINT CrcCount;
	
	GetMipiCrcErrorCount(&CrcCount,pVideo->m_iDevIDA);
	USHORT OldCrcCount = CrcCount;
	timeStart = GetTickCount();
	while (pVideo->m_iWorkStatusA != TESTOR_STOP)
	{
		if(pVideo->m_iWorkStatusA == TESTOR_START)
		{
			bRet = GrabFrame(pVideo->m_pCameraBufferA, pVideo->m_GrabSizeA,&RetSize,&(pVideo->m_FrameInfoA),pVideo->m_iDevIDA);
			if (bRet != DT_ERROR_OK)
			{
				//msg("�߳�ץͼʧ��!");
			}

			//if(bRet == DT_ERROR_OK)
			//{
			//	bRet = GrabFrame(pVideo->m_pCameraBufferA, pVideo->m_GrabSizeA,&RetSize,&(pVideo->m_FrameInfoA),pVideo->m_iDevIDA);
			//	if (bRet != DT_ERROR_OK)
			//	{
			//		msg("��2��ץͼʧ��!");
			//	}
			//	ResetFrameBuffer(pVideo->m_iDevIDA);

			//}
			/*if(bRet == DT_ERROR_OK)
			{
				GetMipiCrcErrorCount(&CrcCount,pVideo->m_iDevIDA);
				bRet = (CrcCount == OldCrcCount) ? DT_ERROR_OK  : DT_ERROR_FAILED;
				OldCrcCount = CrcCount;
			}*/

			if( bRet == DT_ERROR_OK)
			{
				pVideo->TripleBufferFlipA(&pVideo->m_pCameraBufferA, TRUE);

				pVideo->AAFrameCnt++;

				pVideo->m_iFrameCntA++; 

				if(pVideo->m_iFrameCntA % 10 == 0)
				{
					timeStop = GetTickCount() - timeStart;
					timeStart = GetTickCount();
					pVideo->m_fFrameFpsA = (float)(10000.0/timeStop);//����ʱ���Ǻ��룬��Ҫת��Ϊ�롣
				}
				DeviceErrCnt = 0;
			}
			else if(bRet == DT_ERROR_FAILED)
			{
				pVideo->m_iFrameErrA++;
				if(pVideo->m_iFrameErrA > 300)
				{
					AfxMessageBox("Err image too many!");
					break;
				}
				DeviceErrCnt = 0;
				Sleep(1);
			}
			else
			{
				DeviceErrCnt++;
				if(DeviceErrCnt > 2)
				{
					//AfxMessageBox("Can't grab image!");
					//break;
				}
			}
		}
	}
	if(pVideo->m_iWorkStatusA == TESTOR_STOP)
	{
		pVideo->AAFrameCnt = 0;
	}
    SetEvent(pVideo->m_hCameraThreadEventDeadA);
   // _endthread();
	 _endthreadex(0);
}

/*
* Display_Thread
* -------------
* function: while 'playing' get latest sensor buffer, apply colerconversion and display 
* in:       pointer to the CSimpleDlg class
* out:
* special:  this function is called and run as a threadread.
*/
void Display_ThreadA(void *param)
{
	CAAVision  *pVideo = (CAAVision *)param;
	UINT type = pVideo->CurrentSensorA.type;
	int width = pVideo->m_PreviewWidthA;
	int height = pVideo->m_PreviewHeightA;
	BYTE* pBmp = (BYTE*)malloc(width*height*4);

	
	/*m_pImage[pVideo->m_iCurIDA].Clear();*/
	//GenImageConst(&m_pImage[pVideo->m_iDevIDA],"byte",width, height);

    while (pVideo->m_iWorkStatusA != TESTOR_STOP)
    {
		//change display buffer.
		if((pVideo->m_iWorkStatusA == TESTOR_START))
		{
			if (!pVideo->TripleBufferFlipA(&pVideo->m_pDisplayBufferA, FALSE))
			{
				Sleep(1);
				continue;
			}
			
			if(pVideo->AAFrameCnt)
			{
				ImageProcess(pVideo->m_pDisplayBufferA, pBmp, width, height, &(pVideo->m_FrameInfoA),pVideo->m_iDevIDA);
				//��ͼ���ϻ���,����
				if ( TRUE == pVideo->m_paintflag && SINGLEAA == g_AAParam.AATYPE)
				{				
					//����
					int lineLength = 50;///�ߵĸ߶�
					int lineWidth = 10;//�ߵĳ���

					pVideo->PaintCrossCenter(pBmp,pVideo->m_FrameInfoA,lineLength,lineWidth);

					int lengthS = g_AAParam.size/(g_AAParam.scale);//С���γ���
					int lengthB = g_AAParam.block/(g_AAParam.scale);//����γ���

					int lenS = g_AAParam.mapdislength/(g_AAParam.scale); //���ľ��θ����Ͼ��εľ���
					int lenB = g_AAParam.mapdiswidth/(g_AAParam.scale);

					//����ѡȡ�ĵ���Ϊ����������ͬ�ľ���
					int x =0;
					int y =0;
					int bigx = 0;
					int bigy =0;
					pVideo->DecisionRectByTemplateType(g_AAParam.temPlate[pVideo->m_iDevIDA],lengthS,lengthB,pVideo->m_FrameInfoA.uWidth,pVideo->m_FrameInfoA.uHeight,x,y,bigx,bigy);
					if (x>=0 &&x<pVideo->m_FrameInfoA.uWidth &&y >=0 &&y<pVideo->m_FrameInfoA.uHeight)
					{					
						pVideo->PaintRetangle(pBmp,pVideo->m_FrameInfoA,x,y,lengthS,lengthS,lineWidth,2);//����С����	
					}
					if (bigx>=0 &&bigx<pVideo->m_FrameInfoA.uWidth &&bigy >=0 &&bigy<pVideo->m_FrameInfoA.uHeight)
					pVideo->PaintRetangle(pBmp,pVideo->m_FrameInfoA,bigx,bigy,lengthB,lengthB,lineWidth,1);//���Ĵ����

					
					pVideo->DecisionRectByTemplateType(g_AAParam.temPlate[pVideo->m_iDevIDA],lengthS,lengthB,pVideo->m_FrameInfoA.uWidth,pVideo->m_FrameInfoA.uHeight,x,y,bigx,bigy);
					x = (x- lenS) >0?(x- lenS):0;
					bigx = (bigx- lenS)>0?(bigx- lenS):0 ;
					if (x>=0 &&x<pVideo->m_FrameInfoA.uWidth &&y >=0 &&y<pVideo->m_FrameInfoA.uHeight)
					{
						pVideo->PaintRetangle(pBmp,pVideo->m_FrameInfoA,x,y,lengthS,lengthS,lineWidth,2);//leftС����
						
					}
					if (bigx>=0 &&bigx<pVideo->m_FrameInfoA.uWidth &&bigy >=0 &&bigy<pVideo->m_FrameInfoA.uHeight)
					pVideo->PaintRetangle(pBmp,pVideo->m_FrameInfoA,bigx,bigy,lengthB,lengthB,lineWidth,1);//left�����
			
					pVideo->DecisionRectByTemplateType(g_AAParam.temPlate[pVideo->m_iDevIDA],lengthS,lengthB,pVideo->m_FrameInfoA.uWidth,pVideo->m_FrameInfoA.uHeight,x,y,bigx,bigy);
					x = (x + lenS)< pVideo->m_FrameInfoA.uWidth?(x + lenS):pVideo->m_FrameInfoA.uWidth;
					bigx = (bigx+ lenS)<pVideo->m_FrameInfoA.uWidth? (bigx+ lenS):pVideo->m_FrameInfoA.uWidth;
					if (x>=0 &&x<pVideo->m_FrameInfoA.uWidth &&y >=0 &&y<pVideo->m_FrameInfoA.uHeight)
					{
						pVideo->PaintRetangle(pBmp,pVideo->m_FrameInfoA,x,y,lengthS,lengthS,lineWidth,2);//rightС����

					}
					if (bigx>=0 &&bigx<pVideo->m_FrameInfoA.uWidth &&bigy >=0 &&bigy<pVideo->m_FrameInfoA.uHeight)
					pVideo->PaintRetangle(pBmp,pVideo->m_FrameInfoA,bigx,bigy,lengthB,lengthB,lineWidth,1);//right�����


					pVideo->DecisionRectByTemplateType(g_AAParam.temPlate[pVideo->m_iDevIDA],lengthS,lengthB,pVideo->m_FrameInfoA.uWidth,pVideo->m_FrameInfoA.uHeight,x,y,bigx,bigy);
					y =(y - lenB)>0?(y - lenB):0;
					bigy = (bigy- lenB)>0?(bigy- lenB):0 ;
					if (x>=0 &&x<pVideo->m_FrameInfoA.uWidth &&y >=0 &&y<pVideo->m_FrameInfoA.uHeight)
					{
					pVideo->PaintRetangle(pBmp,pVideo->m_FrameInfoA,x,y,lengthS,lengthS,lineWidth,2);//topС����

					}
					if (bigx>=0 &&bigx<pVideo->m_FrameInfoA.uWidth &&bigy >=0 &&bigy<pVideo->m_FrameInfoA.uHeight)
					pVideo->PaintRetangle(pBmp,pVideo->m_FrameInfoA,bigx,bigy,lengthB,lengthB,lineWidth,1);//top�����

					pVideo->DecisionRectByTemplateType(g_AAParam.temPlate[pVideo->m_iDevIDA],lengthS,lengthB,pVideo->m_FrameInfoA.uWidth,pVideo->m_FrameInfoA.uHeight,x,y,bigx,bigy);
					y = y + lenB ;
					bigy = bigy+ lenB ;
					if (x>=0 &&x<pVideo->m_FrameInfoA.uWidth &&y >=0 &&y<pVideo->m_FrameInfoA.uHeight)
					{
						pVideo->PaintRetangle(pBmp,pVideo->m_FrameInfoA,x,y,lengthS,lengthS,lineWidth,2);//bottomС����

					}
					if (bigx>=0 &&bigx<pVideo->m_FrameInfoA.uWidth &&bigy >=0 &&bigy<pVideo->m_FrameInfoA.uHeight)
					pVideo->PaintRetangle(pBmp,pVideo->m_FrameInfoA,bigx,bigy,lengthB,lengthB,lineWidth,1);//bottom�����
	

				}		
				else if ( TRUE == pVideo->m_paintflag && DOUBLEAA == g_AAParam.AATYPE)//˫��
				{
					//������
					int lineWidth = 10;//�߿�
					//�ֱ����� ����������
					int ltx = (int)g_AAParam.channel1Retangle[pVideo->m_iCurIDA][AA_TOP].ltx;
					int lty = (int)g_AAParam.channel1Retangle[pVideo->m_iCurIDA][AA_TOP].lty;
					int rbx = (int)g_AAParam.channel1Retangle[pVideo->m_iCurIDA][AA_TOP].rbx;
					int rby = (int)g_AAParam.channel1Retangle[pVideo->m_iCurIDA][AA_TOP].rby;
					if (ltx>0&& ltx <pVideo->m_FrameInfoA.uHeight && rbx>0&& rbx <pVideo->m_FrameInfoA.uHeight
						&& lty>0&& lty <pVideo->m_FrameInfoA.uWidth && rby>0&& rby <pVideo->m_FrameInfoA.uWidth
						&& (rbx - ltx>lineWidth)&& (rby - lty>lineWidth))
					{
						pVideo->PaintRetangle(pBmp,pVideo->m_FrameInfoA,lty,ltx,rby-lty,rbx-ltx,lineWidth,AA_BLUE);//��
					}
					
					ltx = (int)g_AAParam.channel1Retangle[pVideo->m_iCurIDA][AA_BOTTOM].ltx;
					lty = (int)g_AAParam.channel1Retangle[pVideo->m_iCurIDA][AA_BOTTOM].lty;
					rbx = (int)g_AAParam.channel1Retangle[pVideo->m_iCurIDA][AA_BOTTOM].rbx;
					rby = (int)g_AAParam.channel1Retangle[pVideo->m_iCurIDA][AA_BOTTOM].rby;
					if (ltx>0&& ltx <pVideo->m_FrameInfoA.uHeight && rbx>0&& rbx <pVideo->m_FrameInfoA.uHeight
						&& lty>0&& lty <pVideo->m_FrameInfoA.uWidth && rby>0&& rby <pVideo->m_FrameInfoA.uWidth
						&& (rbx - ltx>lineWidth)&& (rby - lty>lineWidth))
					{
						pVideo->PaintRetangle(pBmp,pVideo->m_FrameInfoA,lty,ltx,rby-lty,rbx-ltx,lineWidth,AA_BLUE);//
					}
					ltx = (int)g_AAParam.channel1Retangle[pVideo->m_iCurIDA][AA_CENTER].ltx;
					lty = (int)g_AAParam.channel1Retangle[pVideo->m_iCurIDA][AA_CENTER].lty;
					rbx = (int)g_AAParam.channel1Retangle[pVideo->m_iCurIDA][AA_CENTER].rbx;
					rby = (int)g_AAParam.channel1Retangle[pVideo->m_iCurIDA][AA_CENTER].rby;
					if (ltx>0&& ltx <pVideo->m_FrameInfoA.uHeight && rbx>0&& rbx <pVideo->m_FrameInfoA.uHeight
						&& lty>0&& lty <pVideo->m_FrameInfoA.uWidth && rby>0&& rby <pVideo->m_FrameInfoA.uWidth
						&& (rbx - ltx>lineWidth)&& (rby - lty>lineWidth))
					{
						pVideo->PaintRetangle(pBmp,pVideo->m_FrameInfoA,lty,ltx,rby-lty,rbx-ltx,lineWidth,AA_BLUE);//
					}
					ltx = (int)g_AAParam.channel1Retangle[pVideo->m_iCurIDA][AA_LEFT].ltx;
					lty = (int)g_AAParam.channel1Retangle[pVideo->m_iCurIDA][AA_LEFT].lty;
					rbx = (int)g_AAParam.channel1Retangle[pVideo->m_iCurIDA][AA_LEFT].rbx;
					rby = (int)g_AAParam.channel1Retangle[pVideo->m_iCurIDA][AA_LEFT].rby;
					if (ltx>0&& ltx <pVideo->m_FrameInfoA.uHeight && rbx>0&& rbx <pVideo->m_FrameInfoA.uHeight
						&& lty>0&& lty <pVideo->m_FrameInfoA.uWidth && rby>0&& rby <pVideo->m_FrameInfoA.uWidth
						&& (rbx - ltx>lineWidth)&& (rby - lty>lineWidth))
					{
						pVideo->PaintRetangle(pBmp,pVideo->m_FrameInfoA,lty,ltx,rby-lty,rbx-ltx,lineWidth,AA_BLUE);//
					}
					ltx = (int)g_AAParam.channel1Retangle[pVideo->m_iCurIDA][AA_RIGHT].ltx;
					lty = (int)g_AAParam.channel1Retangle[pVideo->m_iCurIDA][AA_RIGHT].lty;
					rbx = (int)g_AAParam.channel1Retangle[pVideo->m_iCurIDA][AA_RIGHT].rbx;
					rby = (int)g_AAParam.channel1Retangle[pVideo->m_iCurIDA][AA_RIGHT].rby;
					if (ltx>0&& ltx <pVideo->m_FrameInfoA.uHeight && rbx>0&& rbx <pVideo->m_FrameInfoA.uHeight
						&& lty>0&& lty <pVideo->m_FrameInfoA.uWidth && rby>0&& rby <pVideo->m_FrameInfoA.uWidth
						&& (rbx - ltx>lineWidth)&& (rby - lty>lineWidth))
					{
						pVideo->PaintRetangle(pBmp,pVideo->m_FrameInfoA,lty,ltx,rby-lty,rbx-ltx,lineWidth,AA_BLUE);//
					}
					//���������괦����ʮ�ֲ�
					for( int i=0;i<AA_VISION_RETAGLELOC;i++)
					{
						pVideo->PaintLine(pBmp,pVideo->m_FrameInfoA,g_AAParam.centerpoint[pVideo->m_iDevIDA][2*i+1],g_AAParam.centerpoint[pVideo->m_iDevIDA][2*i],50,lineWidth,0,3);//ˮƽ��
						pVideo->PaintLine(pBmp,pVideo->m_FrameInfoA,g_AAParam.centerpoint[pVideo->m_iDevIDA][2*i+1],g_AAParam.centerpoint[pVideo->m_iDevIDA][2*i],50,lineWidth,1,3);//��ֱ��
					}
					

					//try
					//{
					//	//�����£��������ʾ����,halcon����opencv�Ƿ����ð󶨴��ڣ���ʾͼ����������ΪByte
					//	//EnterCriticalSection(&pVideo->m_windcsLock);//������ʧ�ܣ����л�������ĵط����ܼ���
					//	//��byte ��� HObject
					//	//m_pImage[pVideo->m_iDevIDA].Clear();
					//	NewImage(&m_pImage[pVideo->m_iCurIDA], pBmp,pVideo->m_FrameInfoA.uWidth,pVideo->m_FrameInfoA.uHeight);

					//	DispObj(m_pImage[pVideo->m_iCurIDA],pVideo->halconWinID);
					//	Sleep(1);
					//	//LeaveCriticalSection(&pVideo->m_windcsLock);
					//}
		
					//catch (...)
					//{
					//}



				}
				DisplayRGB24(pBmp, &pVideo->m_FrameInfoA,pVideo->m_iDevIDA);
			}

		}
		else
		{
			  Sleep(50);
		}
    }
	free(pBmp);
	pBmp = NULL;
    SetEvent(pVideo->m_hDisplayThreadEventDeadA);
    _endthread();
}

void NewImage(HObject *newhimg, unsigned char * pBmp,int w,int h)
{
	unsigned char  *image_red;
	unsigned char  *image_green;
	unsigned char  *image_blue;
	int            r,c;
	image_red = (unsigned char  *)malloc(w*h);
	image_green = (unsigned char  *)malloc(w*h);
	image_blue = (unsigned char  *)malloc(w*h);
	for (r=0; r<h; r++)
		for (c=0; c<w; c++)
		{
			//image_red[r*w+c] = c % 255;
			//image_green[r*w+c] = (c+64) % 255;
			//image_blue[r*w+c] = (c+128) % 255;
			image_red[r*w+c] = pBmp[3*(c + r*w) +0];
			image_green[r*w+c] = pBmp[3*(c + r*w) +1];
			image_blue[r*w+c] = pBmp[3*(c + r*w) +2];
		}
		GenImage3Extern(newhimg,"byte",w,h,(Hlong)image_red,(long)image_green,(long)image_blue,(long)free);
			
		
		//�Ƿ�ɾ��ָ��
		//free(image_blue);
		//free(image_green);
		//free(image_red);
}
void NewSigChannelImage(HObject *newhimg, unsigned char * pBmp,int w,int h)
{
	unsigned char  *image_red;

	int            r,c;
	image_red = (unsigned char  *)malloc(w*h);

	for (r=0; r<h; r++)
		for (c=0; c<w; c++)
		{

			image_red[r*w+c] = pBmp[3*(c + r*w) +0];
	/*		image_green[r*w+c] = pBmp[3*(c + r*w) +1];
			image_blue[r*w+c] = pBmp[3*(c + r*w) +2];*/
		}
		GenImage1Extern(newhimg,"byte",w,h,(Hlong)image_red,(long)free);

}

bool dispImage(int winID, int index)
{
	HObject Arrow, ArrowX, ArrowY;
	HTuple width, height, Point1Row, Point1Col, Point2Row, Point2Col;
	HTuple row,col, row1,col1;


	try
	{
		/*HalconCpp::ClearWindow(winID);*/
		if (MAXPICNUMEVESCENE <= index || !(m_pImage[index].IsInitialized()))
		{
			return false;
		}
		/*HalconCpp::ClearWindow(winID);*/
		//HalconCpp::DispImage(m_pImage[index],winID);
		//SetPart(winID,topy,topx,lowy,lowx);
		DispObj(m_pImage[index],winID);

	}
	catch(...)
	{
		return false;
	}
	//m_index = index;
	return true;
}

