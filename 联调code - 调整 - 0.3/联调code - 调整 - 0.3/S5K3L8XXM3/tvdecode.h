#ifndef _TV_DECODE_H_
#define _TV_DECODE_H_

void EdageDirectLineAvg(unsigned char *pDisplayBuffer, int width, int height);
void Bob(unsigned char *pDisplayBuffer, int width, int height);
void weave(unsigned char *pDisplayBuffer, int width, int height);
void DynamicModule(unsigned char *pDisplayBuffer, int width, int height);
////////////////��̬ELA///////////////////////////
void DynamicELA(unsigned char *pDisplayBuffer, int width, int height);// �Ƽ�
void ModifiedELA(unsigned char *pDisplayBuffer, int width, int height);
//***********************��������Ӧ***********************//
void EdageAdption(unsigned char *pDisplayBuffer, int width, int height);

#endif