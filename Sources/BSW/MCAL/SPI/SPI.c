#include "SPI.h"
#include "derivative.h"

void SPI0_Init(void)
{
	SPI0BR = 0x21; //3*4=12, 24M/12=2Mhz
	
	SPI0CR2_MODFEN = 1;     
	SPI0CR1_MSTR = 1;
	SPI0CR1_LSBFE = 0;//Msb
	SPI0CR1_CPOL = 0;//0 Active-high clocks selected. In idle state SCK is low.
	SPI0CR1_CPHA = 1;//sample at even edge 
  SPI0CR1_SSOE = 1;
	SPI0CR1_SPE = 1;//1 SPI enabled, port pins are dedicated to SPI functions
}

void SPI1_Init(void)
{
	SPI1BR = 0x22; //3*8=24, 24M/24=1Mhz
	
	SPI1CR2_MODFEN = 1;     
	SPI1CR1_MSTR = 1;
	
	SPI1CR1_LSBFE = 1;//Lsb
	SPI1CR1_CPOL = 0;//0 Active-high clocks selected. In idle state SCK is low.
	SPI1CR1_CPHA = 1;//sample at even edge 
  SPI1CR1_SSOE = 1;
	SPI1CR1_SPE = 1;//1 SPI enabled, port pins are dedicated to SPI functions
}

/************************************************/
unsigned char SPI0_Send_byte(unsigned char u8Data)
{
	unsigned int cnt;
  
  cnt = 0xFFF;	
	do{
		cnt--;
	}while((!SPI0SR_SPTEF)&&(cnt));
	
	if(cnt){
		SPI0DRL=u8Data;
			return 0;	
	} 
	else{
	  return 1;
	} 
} //can work

/*unsigned char SPI0_Send_byte(unsigned char u8Data)
{
	unsigned int cnt;
  unsigned char data = 0;
  
  cnt = 0xFFF;	
	do{
		cnt--;
	}while((!SPI0SR_SPTEF)&&(cnt));
	
	if(cnt){
		SPI0DRL=u8Data;
	} 

	cnt = 0xFFF;
	do{
		cnt--;
	}while((!SPI0SR_SPIF)&&(cnt));
	
	if(cnt)
	{
		//data = SPI0DRL;//不能读
	}

	return(data);
} */


/************************************************/
unsigned char SPI0_Receive_byte(void)
{
  unsigned int cnt;
	unsigned char data = 0;
	
	cnt = 0xFFF;	
	do{
		cnt--;
	}while((!SPI0SR_SPTEF)&&(cnt));
	SPI0DRL=0xFF; //写一次到数据寄存器启动一次发送提供时钟，对方的数据移位过来
	
	cnt = 0xFFF;
	do{
		cnt--;
	}while((!SPI0SR_SPIF)&&(cnt));
	
	if(cnt)
	{
		data = SPI0DRL;
	}

	return(data);
}











unsigned char SPI1_Send_byte(unsigned char u8Data)
{
	unsigned int cnt;
	cnt = 0xFFF;
	
	do{
		cnt--;
	}while((!SPI1SR_SPTEF)&&(cnt));
	(void)SPI1SR;
	
	if(cnt){
		SPI1DRL=u8Data;
		return 0;	
	} 
	else{
	  return 1;
	}
}

unsigned char SPI1_Receive_byte(void)
{
  unsigned int cnt;
	unsigned char data = 0;
	
	(void)SPI1SR; //
	SPI1DRL=0xFF;
	
	cnt = 0xFFF; 
	do{
		cnt--;
	}while((!SPI1SR_SPIF)&&(cnt));
	
	if(cnt)
	{
	  (void)SPI1SR;
		data = SPI1DRL;
	}

	return(data);
}

unsigned char SPI1_SendRecv_byte(unsigned char u8Data)
{
  unsigned int cnt;
	unsigned char data = 0;
	
	cnt = 0xFFF;
	do{
		cnt--;
	}while((!SPI1SR_SPTEF)&&(cnt));
	(void)SPI1SR;
	
	if(cnt){
		SPI1DRL=u8Data;
	}else{
	  return 0xFF;
	}
		
	cnt = 0xFFF; 
	do{
		cnt--;
	}while((!SPI1SR_SPIF)&&(cnt));
	
	if(cnt)
	{
	  (void)SPI1SR;
		data = SPI1DRL;
	}

	return(data);
}
/************************************************/
void SPI0_High_rate(void)
{
  SPI0BR = 0x22; 
  SPI0BR = 0x00;//最快2分频 		
}
