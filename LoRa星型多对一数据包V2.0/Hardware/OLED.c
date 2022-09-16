#include <stm32f10x.h>
#include "OLED_Font.h"


/*引脚配置*/
#define OLED_W_SCL(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)(x))
#define OLED_W_SDA(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction)(x))
u8 j;
extern u8 JU;
extern u8 d;
/*引脚初始化*/
void OLED_I2C_Init(void)
{
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;//开漏输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*空闲状态：I2C总线总线的SDA和SCL两条信号线同时处于高电平时，
	规定为总线的空闲状态。此时各个器件的输出级场效应管均处在截止状态，
	即释放总线，由两条信号线各自的上拉电阻把电平拉高*/
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}

/**
  * @brief  I2C开始
  * @param  无
  * @retval 无
  */
void OLED_I2C_Start(void)
{
	//起始信号：当SCL为高期间，SDA由高到低的跳变
	OLED_W_SDA(1);
	OLED_W_SCL(1);
	OLED_W_SDA(0);
	OLED_W_SCL(0);
}

/**
  * @brief  I2C停止
  * @param  无
  * @retval 无
  */
void OLED_I2C_Stop(void)
{
	//停止信号：当SCL为高期间，SDA由低到高的跳变
	OLED_W_SDA(0);
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}


/**
  * @brief  I2C发送一个字节
  * @param  Byte 要发送的一个字节
  * @retval 无
  */


void OLED_I2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		OLED_W_SDA(Byte & (0x80 >> i));//高位先出，右移再按位与
		OLED_W_SCL(1);
		OLED_W_SCL(0);
	}
	OLED_W_SCL(1);	//额外的一个时钟，不处理应答信号，
	OLED_W_SCL(0); //
}


/**
  * @brief  OLED写命令
  * @param  Command 要写入的命令
  * @retval 无
  */
void OLED_WriteCommand(uint8_t Command)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//从机地址固定
	OLED_I2C_SendByte(0x00);		//写命令co=0代表字节   D/C为0代表发送的是一个命令参考OLED手册
	OLED_I2C_SendByte(Command); 
	OLED_I2C_Stop();
}

/**
  * @brief  OLED写数据
  * @param  Data 要写入的数据
  * @retval 无
  */
void OLED_WriteData(uint8_t Data)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//从机地址
	OLED_I2C_SendByte(0x40);		//D/C为1   告诉OLED要写数据   参考OLED手册
	OLED_I2C_SendByte(Data);
	OLED_I2C_Stop();
}

/**
  * @brief  OLED设置光标位置
  * @param  Y 以左上角为原点，向下方向的坐标，范围：0~7
  * @param  X 以左上角为原点，向右方向的坐标，范围：0~127
  * @retval 无
  */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
	OLED_WriteCommand(0xB0 | Y);//低三位为页地址	也即设置Y位置
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//设置X位置低4位
	OLED_WriteCommand(0x00 | (X & 0x0F));			//设置X位置高4位
}

/**
  * @brief  Menu清屏
  * @param  无
  * @retval 无
  */
void OLED_MenuClear(void)
{  
	uint8_t i, j;
	for (j = 2; j < 8; j++)
	{
		OLED_SetCursor(j, 0);
		for(i = 0; i < 128; i++)
		{
			OLED_WriteData(0x00);
		}
	}
}
/**
  * @brief  OLED清屏
  * @param  无
  * @retval 无
  */
void OLED_Clear(void)
{  
	uint8_t i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetCursor(j, 0);
		for(i = 0; i < 128; i++)
		{
			OLED_WriteData(0x00);
		}
	}
}
/**
  * @brief  OLED显示16X16的Logo(包括汉字)
  * @param  Line 行位置，范围：0~7                    
  * @param  Column 列位置，范围：0~127
  * @param  Logo
  * @retval 无
  */
void OLED_ShowLogo(uint8_t Line, uint8_t Column,u8 j)
{      	
	uint8_t i;		//设置光标位置在上半部分
	OLED_SetCursor(Line,Column);
	for (i = 0; i < 16; i++)
	{
		OLED_WriteData(OLED16x16[j][i]);			//显示上半部分内容
	}
	OLED_SetCursor(Line + 1, Column);	//设置光标位置在下半部分
	for (i = 0; i < 16; i++)
	{
		OLED_WriteData(OLED16x16[j][i+16]);		//显示下半部分内容
	}
}

/**
  * @brief  OLED显示一个字符
  * @param  Line 行位置，范围：1~4                      //面向非程序员
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的一个字符，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		//设置光标位置在上半部分
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i]);			//显示上半部分内容
	}
	OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//设置光标位置在下半部分
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);		//显示下半部分内容
	}
}


/**
  * @brief  OLED显示跳跃小恐龙
  * @param  Line 行位置，范围：1~4     因为高度为16                 //面向非程序员
  * @param  Column 列位置，范围：1~16
  * @retval 无
  */
int OLED_ShowDinosaurJump(char reset)
{      	
	char speed_arr[]={1,1,3,3,4,4,5,6,7};
	static char speed_idx=sizeof(speed_arr)-1;
	static int height=0;
	static char dir=0;
	char speed=4;
	unsigned int i=0,j=0;
	unsigned char x,y;
	char offset=0;
	u8 byte;
	if(dir==0)
	{
		height+=speed_arr[speed_idx];
		speed_idx--;
		if(speed_idx<0) speed_idx=0;
	}
	if(dir==1)
	{
		height-=speed_arr[speed_idx];
		speed_idx++;
		if(speed_idx>sizeof(speed_arr)-1) speed_idx=sizeof(speed_arr)-1;
	}
	if(height>=31)
	{
		dir=1;
		height=31;
	}
	if(height<=0)
	{
		dir=0;
		height=0;
	}
	if(height<=7) offset=0;
	else if(height<=15) offset=1;
	else if(height<=23) offset=2;
	else if(height<=31) offset=3;
	else offset=4;
	
		OLED_SetCursor(5-offset,0);
		for(x=0;x<16;x++)
		{
			OLED_WriteData(Dinosaur[0][x]);	
		}
		OLED_SetCursor(5-offset+1,0);
		for(x=0;x<16;x++)
		{
			OLED_WriteData(Dinosaur[0][x+16]);	
		}


		if(dir==0)
		{
			OLED_SetCursor(5-offset+2,0);
			for(x=0;x<16;x++)
			{
				OLED_WriteData(Block[x]);	
			}
		}
		else
		{
			OLED_SetCursor(5-offset-1,0);
			for(x=0;x<16;x++)
			{
				OLED_WriteData(Block[x]);	
			}
		}



		if(d==15)
		{
			
			OLED_SetCursor(5,0);
			for(x=0;x<16;x++)
			{
				OLED_WriteData(Block[x]);	
			}
			
		height=0;
		JU=0;
		dir=0;
		offset=0;
		speed_idx=sizeof(speed_arr)-1;
		return 0;

		}

}

/**
  * @brief  OLED显示一帧小恐龙
  * @param  Line 行位置，范围：1~4     因为高度为16                 //面向非程序员
  * @param  Column 列位置，范围：1~16
  * @retval 无
  */
void OLED_ShowDinosaur(uint8_t Line, uint8_t Column)
{      	
	uint8_t i;
	j=j%2;
			//设置光标位置在上半部分
		OLED_SetCursor((Line - 1) * 2+1, (Column - 1) * 8);
		for (i = 0; i < 16; i++)
		{
			//设置光标位置在上半部分
			OLED_WriteData(Dinosaur[j][i+16]);		
		}
		
		OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);
		for (i = 0; i < 16; i++)
		{
			//设置光标位置在上半部分
			OLED_WriteData(Dinosaur[j][i]);		
		}

}

/**
  * @brief  OLED显示字符串
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OLED_ShowChar(Line, Column + i, String[i]);
	}
}

/**
  * @brief  OLED次方函数
  * @retval 返回值等于X的Y次方
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}


/**
  * @brief  OLED显示云朵

  */
void OLED_Clound()
{
	u8 i=0;
	u8 start=0;
	u8 byte=0;
	static int pose=128;
	u8 speed=3;
	u8 length=sizeof(Clound);
	
	OLED_SetCursor(0, pose);
	for(i=start;i<length+speed;i++)
	{
		if(pose+i>127) 
		{break;}
		if(i<length) 
		{byte=Clound[i];}
		else 
		{byte=0x0;}
		
		OLED_WriteData(byte);
	}
	
	pose=pose-speed;

}


/**
  * @brief  OLED显示小恐龙地面

  */
void OLED_Ground()
{      	
	static u8 pos=0;//当前位置
	u8 speed=5;//移动的速度
	unsigned int length=sizeof(Ground);
	u8 i=0;
	OLED_SetCursor(7, 0);
	for(i=0;i<128;i++)
	{
		OLED_WriteData(Ground[(i+pos)%length]);	
	}
	pos=pos+speed;
	if(pos>length)
	{
		pos=0;
	}	
}


/**
  * @brief  OLED显示一个照片128x64

  */
void OLED_ShowPicture128x64()
{      			    
	u8 t,i,Line=0;
    for(t=0;t<8;t++)
	{	
		OLED_SetCursor(Line, 0);
			Line++;
		for(i=0;i<128;i++)
			{
				OLED_WriteData(Hzk[t][i]);
				
			}
		
     }	
					
}


/**
  * @brief  OLED显示数字（十进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~4294967295
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED显示数字（十进制，带符号数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：-2147483648~2147483647
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
	uint8_t i;
	uint32_t Number1;
	if (Number >= 0)
	{
		OLED_ShowChar(Line, Column, '+');
		Number1 = Number;
	}
	else
	{
		OLED_ShowChar(Line, Column, '-');
		Number1 = -Number;
	}
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED显示数字（十六进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~0xFFFFFFFF
  * @param  Length 要显示数字的长度，范围：1~8
  * @retval 无
  */
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)							
	{
		SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
		if (SingleNumber < 10)
		{
			OLED_ShowChar(Line, Column + i, SingleNumber + '0');
		}
		else
		{
			OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
		}
	}
}

/**
  * @brief  OLED显示数字（二进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
  * @param  Length 要显示数字的长度，范围：1~16
  * @retval 无
  */
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
	}
}

/**
  * @brief  OLED初始化
  * @param  无
  * @retval 无
  */
void OLED_Init(void)
{
	uint32_t i, j;
	
	for (i = 0; i < 1000; i++)			//上电延时
	{
		for (j = 0; j < 1000; j++);
	}
	
	OLED_I2C_Init();			//端口初始化
	
	OLED_WriteCommand(0xAE);	//关闭显示
	
	OLED_WriteCommand(0xD5);	//设置显示时钟分频比/振荡器频率
	OLED_WriteCommand(0x80);
	
	OLED_WriteCommand(0xA8);	//设置多路复用率
	OLED_WriteCommand(0x3F);
	
	OLED_WriteCommand(0xD3);	//设置显示偏移
	OLED_WriteCommand(0x00);
	
	OLED_WriteCommand(0x40);	//设置显示开始行
	
	OLED_WriteCommand(0xA1);	//设置左右方向，0xA1正常 0xA0左右反置
	
	OLED_WriteCommand(0xC8);	//设置上下方向，0xC8正常 0xC0上下反置

	OLED_WriteCommand(0xDA);	//设置COM引脚硬件配置
	OLED_WriteCommand(0x12);
	
	OLED_WriteCommand(0x81);	//设置对比度控制
	OLED_WriteCommand(0xCF);

	OLED_WriteCommand(0xD9);	//设置预充电周期
	OLED_WriteCommand(0xF1);

	OLED_WriteCommand(0xDB);	//设置VCOMH取消选择级别
	OLED_WriteCommand(0x30);

	OLED_WriteCommand(0xA4);	//设置整个显示打开/关闭

	OLED_WriteCommand(0xA6);	//设置正常/倒转显示

	OLED_WriteCommand(0x8D);	//设置充电泵
	OLED_WriteCommand(0x14);

	OLED_WriteCommand(0xAF);	//开启显示
		
	OLED_Clear();				//OLED清屏
}
