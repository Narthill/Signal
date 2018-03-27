#include "stdafx.h"
#include "tvdecode.h"

////////////ELAȥ�����㷨///////////////////

void EdageDirectLineAvg(unsigned char *pDisplayBuffer, int width, int height)
{
	unsigned char *pBmp=new unsigned char [height*width*2];
	memcpy(pBmp,pDisplayBuffer,width*height*2);

	int i,j,k,m;
	int linebyte=width*2;		 
	unsigned char *p0,*p1;
	
	for (i=0;i<height;i++)
	{
		//ż����ʱ
		for (j=0;j<width;j=j+2)
		{ 
			if(i%2==0)
				{
				for(k=0;k<2;k++)
				 *(pDisplayBuffer+i*linebyte+j*2+k)=*(pBmp+i*linebyte/2+j*2+k);
				}
			else
				{
				 if (j<2||j>width-3)
					{
					 for(k=0;k<2;k++)
				 	 *(pDisplayBuffer+i*linebyte+j*2+k)=
					  (*(pBmp+(i-1)*linebyte/2+j*2+k)+*(pBmp+(i+1)*linebyte/2+j*2+k))/2;
					}
				 else			
					{
					 for(k=0;k<2;k++)
						 {
						  p0=(pBmp+(i-1)*linebyte/2+(j-(-2))*2+k);
						  p1=(pBmp+(i+1)*linebyte/2+(j+(-2))*2+k);
						  for(m=0;m<4;m=m+2)
							{
							 if (abs(*p0-*p1)>abs(*(pBmp+(i+1)*linebyte/2+(j+m)*2+k)-*(pBmp+(i-1)*linebyte/2+(j-m)*2+k)))
								 {p1=(pBmp+(i+1)*linebyte/2+(j+m)*2+k);
								  p0=(pBmp+(i-1)*linebyte/2+(j-m)*2+k);
								 }
							}								
						*(pDisplayBuffer+i*linebyte+j*2+k)=(*p0+*p1)/2;
						}						
					}
				}
		}
		//������ʱ
		for (j=1;j<width;j=j+2)
			{if (i%2==0)
				{for (k=0;k<2;k++)
				*(pDisplayBuffer+i*linebyte+j*2+k)=*(pBmp+i*linebyte/2+j*2+k);			
				}
			 else
			 {if (j<2||j>width-3)
				  for(k=0;k<2;k++)
					*(pDisplayBuffer+i*linebyte+j*2+k)=
					  (*(pBmp+(i-1)*linebyte/2+j*2+k)+*(pBmp+(i+1)*linebyte/2+j*2+k))/2;
			  else
				  {
				  for(k=0;k<2;k++)
						 {
					  	 p0=(pBmp+(i-1)*linebyte/2+(j-(-2))*2+k);
						 p1=(pBmp+(i+1)*linebyte/2+(j+(-2))*2+k);
						 for (m=0;m<4;m=m+2)
							 {
							 if (abs(*p0-*p1)>abs(*(pBmp+(i+1)*linebyte/2+(j+m)*2+k)-*(pBmp+(i-1)*linebyte/2+(j-m)*2+k)))
								 {
								  p1=(pBmp+(i+1)*linebyte/2+(j+m)*2+k);
								  p0=(pBmp+(i-1)*linebyte/2+(j-m)*2+k);
								 }
							 }						 							 
						 *(pDisplayBuffer+i*linebyte+j*2+k)=(*p0+*p1)/2;							 
						}
					  	  
				  }
			  }
			}
	}
	delete [] pBmp;
}

///////////////����ƽ�� Bob //////////////////////
void Bob(unsigned char *pDisplayBuffer, int width, int height)
{
	unsigned char *pBmp=new unsigned char [height*width*2];
	memcpy(pBmp,pDisplayBuffer,height*width*2);
	
	int i,j,k;
	int linebyte=width<<1;
	
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
			if(i%2==1)
			{
				for(k=0;k<2;k++)
					*(pDisplayBuffer+i*linebyte+j*2+k)=
					(*(pBmp+(i-1)*linebyte/2+j*2+k)+*(pBmp+(i+1)*linebyte/2+j*2+k))/2;		
			}
			else{
				for(k=0;k<2;k++)
					*(pDisplayBuffer+i*linebyte+j*2+k)=*(pBmp+i*linebyte/2+j*2+k);		
			}
		}
	}
	delete [] pBmp;
	
}

/////////////��֯��weave////////////
void weave(unsigned char *pDisplayBuffer, int width, int height)
{
	unsigned char *pBmp=new unsigned char [height*width*2];
	memcpy(pBmp,pDisplayBuffer,height*width*2);
	
	int i;
	int linebyte=width<<1;
	for (i=0;i<height;i++)
	{	
		if (i%2 == 0)
			memcpy(pDisplayBuffer+i*linebyte,pBmp+i/2*linebyte,linebyte);
		 		
		else		
			memcpy(pDisplayBuffer+i*linebyte,pBmp+(i/2+height/2)*linebyte,linebyte);		
	}
	delete [] pBmp;
}

////////////��̬����ȥ�����㷨///////////////////

void DynamicModule(unsigned char *pDisplayBuffer, int width, int height)
{
	long bmpsize=height*width*2;
	unsigned char *pBmp=new unsigned char [bmpsize];
	memcpy(pBmp,pDisplayBuffer,bmpsize);

	unsigned char *pBmp2=new unsigned char [bmpsize];
	memcpy(pBmp2,pDisplayBuffer,bmpsize);

	unsigned char *pBmp3=new unsigned char [bmpsize];
	memcpy(pBmp3,pDisplayBuffer,bmpsize);
	
	int i,j,k;
	int linebyte=width<<1;
	int MaxPixelDiff=0;           //������ز�
	int Threshold,Threshold1;   //����ֵ	 
	float MixCoe;               //���ϵ��
	int a=0,b=0;
				 
    weave(pBmp, width,height);

	Bob(pBmp2, width,height);			 
	 ////////////////////��̬����/////////////////////
	 for (i=0;i<height>>1;i++)
	 {				 
		 for(j=0;j<width;j++)
		 {
			 a+=*(pBmp3+i*linebyte+j*2+1);
			 b+=*(pBmp3+(i+height/2)*linebyte+j*2+1);
		 }
		 if (MaxPixelDiff<abs(a-b))
		 {
			 MaxPixelDiff=abs(a-b);
			 Threshold=(MaxPixelDiff+9)>>5;
		 }	 
	 }
	 for (i=0;i<height>>1;i++)
	 {  
		 a=0;
		 b=0;
		 for (j=0;j<width;j++)
		 {
			 a+=*(pBmp3+i*linebyte+j*2+1);
			 b+=*(pBmp3+(i+height/2)*linebyte+j*2+1);
			 Threshold1=(abs(a-b)+9)>>5;
		 }
		 for(j=0;j<width;j++)			 
			{ 
			 for (k=0;k<2;k++)
				 {
					 MixCoe = (float)(Threshold1-1)/Threshold;
					 *(pDisplayBuffer+2*i*linebyte+j*2+k) = (BYTE) (*(pBmp+2*i*linebyte+j*2+k)*MixCoe + *(pBmp2+2*i*linebyte+j*2+k)*(1-MixCoe));
					 *(pDisplayBuffer+(2*i+1)*linebyte+j*2+k) = (BYTE)(*(pBmp+(2*i+1)*linebyte+j*2+k)*MixCoe + *(pBmp2+(2*i+1)*linebyte+j*2+k)*(1-MixCoe));
				 }	
			}		 
	 }
	 delete [] pBmp;
	 delete [] pBmp2;
	 delete [] pBmp3;
}



////////////////��̬ELA///////////////////////////
void DynamicELA(unsigned char *pDisplayBuffer, int width, int height)
{
	unsigned char *pBmp=new unsigned char [height*width*2];
	memcpy(pBmp,pDisplayBuffer,height*width*2);
	
	unsigned char *pBmp2=new unsigned char [height*width*2];
	memcpy(pBmp2,pDisplayBuffer,height*width*2);
	
	unsigned char *pBmp3=new unsigned char [height*width*2];
	memcpy(pBmp3,pDisplayBuffer,height*width*2);
	
	int i,j,k;
	int linebyte=width*2;
	int MaxPixelDiff;           //������ز�
	int Threshold;              //����ֵ
	int Threshold1;
	float MixCoe;               //���ϵ��
	int a=0;
	int b=0;
	MaxPixelDiff=0;

	weave(pBmp, width,height);
	
	EdageDirectLineAvg(pBmp2, width, height);

	////////////////////��̬ELA/////////////////////
	for (i=0;i<height/2;i++)
	{				 
		for(j=0;j<width;j++)
		{
			a+=*(pBmp3+i*linebyte+j*2+1);
			b+=*(pBmp3+(i+height/2)*linebyte+j*2+1);
		}
		if (MaxPixelDiff<abs(a-b))
		{
			MaxPixelDiff=abs(a-b);
			Threshold=(MaxPixelDiff+9)/10;
		}	 
	}
	for (i=0;i<height/2;i++)
	{  
		a=0;
		b=0;
		for (j=0;j<width;j++)
		{
			a+=*(pBmp3+i*linebyte+j*2+1);
			b+=*(pBmp3+(i+height/2)*linebyte+j*2+1);
			Threshold1=(abs(a-b)+9)/10;
		}
		for(j=0;j<width;j++)			 
		{ 
			for (k=0;k<2;k++)
			{
				MixCoe = (float)(Threshold1-1)/Threshold;
				*(pDisplayBuffer+2*i*linebyte+j*2+k) = (BYTE) (*(pBmp+2*i*linebyte+j*2+k)*MixCoe + *(pBmp2+2*i*linebyte+j*2+k)*(1-MixCoe));
				*(pDisplayBuffer+(2*i+1)*linebyte+j*2+k) = (BYTE) (*(pBmp+(2*i+1)*linebyte+j*2+k)*MixCoe + *(pBmp2+(2*i+1)*linebyte+j*2+k)*(1-MixCoe));
			}	
		}		
	}
	delete [] pBmp;
	delete [] pBmp2;
	delete [] pBmp3;
}




////*******************������ELA********************////////
void ModifiedELA(unsigned char *pDisplayBuffer, int width, int height)
{
	unsigned char *pBmp=new unsigned char [height*width*2];
	memcpy(pBmp,pDisplayBuffer,height*width*2);
	
	unsigned char *p0,*p1;
	
	int i,j,k,m;
	int linebyte=width*2;
	int PixelDiff;

	for (i=0;i<height;i++)
		{
		for (j=0;j<width;j=j+2)
			{
			if (i%2==0)
				{for (k=0;k<2;k++)
				 *(pDisplayBuffer+i*linebyte+j*2+k)=*(pBmp+i*linebyte/2+j*2+k);
				} 
			else
				{if (j<2||j>width-3)
					{
					for(k=0;k<2;k++)
					*(pDisplayBuffer+i*linebyte+j*2+k)=
					  (*(pBmp+(i-1)*linebyte/2+j*2+k)+*(pBmp+(i+1)*linebyte/2+j*2+k))/2;
					} 
				else
					{
					PixelDiff=abs(*(pBmp+(i/2+height/2)*linebyte+j*2+1)-*(pBmp+(i/2+height/2)*linebyte+(j-2)*2+1))+
							   abs(*(pBmp+(i/2+height/2)*linebyte+j*2+1)-*(pBmp+(i/2+height/2)*linebyte+(j+2)*2+1))+
							   abs(*(pBmp+(i/2+height/2)*linebyte+j*2+1)-*(pBmp+(i-1)/2*linebyte+j*2+1))+
							   abs(*(pBmp+(i/2+height/2)*linebyte+j*2+1)-*(pBmp+(i+1)/2*linebyte+j*2+1));
					 if (PixelDiff<10)
						{
						 for(k=0;k<2;k++)
						 *(pDisplayBuffer+i*linebyte+j*2+k)=*(pBmp+(i/2+height/2)*linebyte+j*2+k);
						}
						 
					 else
						 {
						 for(k=0;k<2;k++)
							 {
							  p0=(pBmp+(i-1)*linebyte/2+(j-(-2))*2+k);
							  p1=(pBmp+(i+1)*linebyte/2+(j+(-2))*2+k);			
							  for (m=0;m<4;m=m+2)
								 {while (abs(*p0-*p1)>abs(*(pBmp+(i+1)*linebyte/2+(j+m)*2+k)-*(pBmp+(i-1)*linebyte/2+(j-m)*2+k)))
									 {
									  p1=(pBmp+(i+1)*linebyte/2+(j+m)*2+k);
									  p0=(pBmp+(i-1)*linebyte/2+(j-m)*2+k); 
									 }
								 }
							  *(pDisplayBuffer+i*linebyte+j*2+k)=(*p0+*p1)/2;
							 }		 
						} 					
					}
				}
			}
		for (j=1;j<width;j=j+2)
			{
			if (i%2==0)
				{
				for (k=0;k<2;k++)
				 *(pDisplayBuffer+i*linebyte+j*2+k)=*(pBmp+i*linebyte/2+j*2+k);
				} 
			else
				{if (j<2||j>width-3)
					{
					for(k=0;k<2;k++)
						*(pDisplayBuffer+i*linebyte+j*2+k)=
						  (*(pBmp+(i-1)*linebyte/2+j*2+k)+*(pBmp+(i+1)*linebyte/2+j*2+k))/2;
					} 
				else
					{
					PixelDiff=abs(*(pBmp+(i/2+height/2)*linebyte+j*2+1)-*(pBmp+(i/2+height/2)*linebyte+(j-2)*2+1))+
							  abs(*(pBmp+(i/2+height/2)*linebyte+j*2+1)-*(pBmp+(i/2+height/2)*linebyte+(j+2)*2+1))+
							  abs(*(pBmp+(i/2+height/2)*linebyte+j*2+1)-*(pBmp+(i-1)/2*linebyte+j*2+1))+
							  abs(*(pBmp+(i/2+height/2)*linebyte+j*2+1)-*(pBmp+(i+1)/2*linebyte+j*2+1));
					if (PixelDiff<5)
						{for(k=0;k<2;k++)
							*(pDisplayBuffer+i*linebyte+j*2+k)=*(pBmp+(i/2+height/2)*linebyte+j*2+k);
						} 
					else
						{for (k=0;k<2;k++)
							{
							  p0=(pBmp+(i-1)*linebyte/2+(j-(-2))*2+k);
							  p1=(pBmp+(i+1)*linebyte/2+(j+(-2))*2+k);
							  for (m=0;m<4;m=m+2)
							  {
							  while (abs(*p0-*p1)>abs(*(pBmp+(i+1)*linebyte/2+(j+m)*2+k)-*(pBmp+(i-1)*linebyte/2+(j-m)*2+k)))
								  {
									p1=(pBmp+(i+1)*linebyte/2+(j+m)*2+k);
									p0=(pBmp+(i-1)*linebyte/2+(j-m)*2+k); 
								  }
							  }
							  *(pDisplayBuffer+i*linebyte+j*2+k)=(*p0+*p1)/2;
							}
						}
					}
				}
			}
		}
}


//***********************��������Ӧ***********************//

void EdageAdption(unsigned char *pDisplayBuffer, int width, int height)
{
	unsigned char *pBmp=new unsigned char [height*width*2];
	memcpy(pBmp,pDisplayBuffer,height*width*2);			
	
	int linebyte=width*2;
	int i,j,k,m;
	//int MinPiexlDiff;
	int PixelAdd;
	int diffmd=32700;
	//int diffsd=32700;
	int diff=0;
	int md,md1;
	//int sd,sd1,m1,m2;
	PixelAdd=0;
	for (i=0;i<height;i++)
		{
		for (j=0;j<width;j=j+2)
			{
			if(i%2==0)     //ż���в���
				{for(k=0;k<2;k++)
				*(pDisplayBuffer+i*linebyte+j*2+k)=*(pBmp+i*linebyte/2+j*2+k);
				}
			else
				{
				if (j<4||j>width-5)  //ǰ���С�������
					{for(k=0;k<2;k++)
					*(pDisplayBuffer+i*linebyte+j*2+k)=
					 (*(pBmp+(i-1)*linebyte/2+j*2+k)+*(pBmp+(i+1)*linebyte/2+j*2+k))/2;
					} 
				else
					{					
					//������Ⱥ���ض���Сֵ
					for (md=-2;md<4;md=md+2)           
						{for (m=-2;m<4;m=m+2)
							{diff+=abs(*(pBmp+(i-1)*linebyte/2+(j-md+m)*2+1)
							  -*(pBmp+(i+1)*linebyte/2+(j+md+m)*2+1));
							}
						if (diffmd>diff)
							{
							 diffmd=diff;
							 md1=md;
					//		 m1=m;	  
							}
						diff=0;										
						}			
					for(k=0;k<2;k++)
						{
						for (m=-2;m<4;m=m+2)
							{//*(pDisplayBuffer+i*linebyte+j*2+k)=(*(pBmp+(i-1)*linebyte/2+(j-offset-m)*2+k)+*(pBmp+(i+1)*linebyte/2+(j+offset+m)*2+k))/2;
							PixelAdd+=(*(pBmp+(i-1)*linebyte/2+(j-md1+m)*2+k)+*(pBmp+(i+1)*linebyte/2+(j+md1+m)*2+k));
							}
						*(pDisplayBuffer+i*linebyte+j*2+k)=PixelAdd/6;
						PixelAdd=0;
						}	
					}
				}
			}
		for (j=1;j<width;j=j+2)
			{
			if(i%2==0)     //ż���в���
				{for(k=0;k<2;k++)
				*(pDisplayBuffer+i*linebyte+j*2+k)=*(pBmp+i*linebyte/2+j*2+k);
				}
			else
				{
				if (j<4||j>width-5)  //ǰ���С�������
					{for(k=0;k<2;k++)
					*(pDisplayBuffer+i*linebyte+j*2+k)=
					 (*(pBmp+(i-1)*linebyte/2+j*2+k)+*(pBmp+(i+1)*linebyte/2+j*2+k))/2;
					} 
				else
					{					
					//������Ⱥ���ض���Сֵ
					for (md=-2;md<4;md=md+2)           
						{for (m=-2;m<4;m=m+2)
							{diff+=abs(*(pBmp+(i-1)*linebyte/2+(j-md+m)*2+1)
								-*(pBmp+(i+1)*linebyte/2+(j+md+m)*2+1));
							}
						if (diffmd>diff)
							{
							diffmd=diff;
							md1=md;
				//			m1=m;	  
							}
						diff=0;										
						}
					
					for(k=0;k<2;k++)
						{						
						for (m=-2;m<4;m=m+2)
							{//*(pDisplayBuffer+i*linebyte+j*2+k)=(*(pBmp+(i-1)*linebyte/2+(j-offset-m)*2+k)+*(pBmp+(i+1)*linebyte/2+(j+offset+m)*2+k))/2;	
							PixelAdd+=(*(pBmp+(i-1)*linebyte/2+(j-md1+m)*2+k)+*(pBmp+(i+1)*linebyte/2+(j+md1+m)*2+k));
							}
						*(pDisplayBuffer+i*linebyte+j*2+k)=PixelAdd/6;
						PixelAdd=0;
						}
					}
				}
			}
		}
}
