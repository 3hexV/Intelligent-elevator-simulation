#ifndef _USART_H_
#define _USART_H_


/*
	Name:数据格式类(用于Usart1)
	Des：规定数据格式，发送数据
	Auth:3hex
	Other:依赖printf()的重定向
*/

typedef unsigned char byte;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;


#define PACK_HEAD 		0xb7 // 包头
#define PACK_END  		0x7b // 包尾
#define PACK_MAX_LEN  0xff  // 最大包长 0:无限制

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


