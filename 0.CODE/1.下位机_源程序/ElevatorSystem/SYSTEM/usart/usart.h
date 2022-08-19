#ifndef _USART_H_
#define _USART_H_


/*
	Name:���ݸ�ʽ��(����Usart1)
	Des���涨���ݸ�ʽ����������
	Auth:3hex
	Other:����printf()���ض���
*/

typedef unsigned char byte;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;


#define PACK_HEAD 		0xb7 // ��ͷ
#define PACK_END  		0x7b // ��β
#define PACK_MAX_LEN  0xff  // ������ 0:������

byte dataXOR(byte *tmpData, byte len);
void sendHexMessageToUsart_1(byte *data,byte dataType,unsigned int dataLen);
void usart1Init(void);
int putCharToU1(int c);
int getCharFromU1(void);
int checkCharFromU1(void);

void usart2Init(void);
void sendHexMessageToUsart_2(byte *data,byte dataType,unsigned int dataLen);
int putCharToU2(int c);
int getCharFromU2(void);
int checkCharFromU2(void);

void usart3Init(void);
void sendHexMessageToUsart_3(byte *data,byte dataType,unsigned int dataLen);
int putCharToU3(int c);
int getCharFromU3(void);
int checkCharFromU3(void);

#endif


