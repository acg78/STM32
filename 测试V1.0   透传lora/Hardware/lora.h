/*
#ifndef __LORA_H
#define __LORA_H
//#include "sys.h"
 
//??????
typedef struct
{
   u16 addr;//????
   u8 chn;//??
   u8 power;//????
   u8 wlrate;//????
   u8 wltime;//????
   u8 mode;//????
   u8 mode_sta;//????
   u8 bps;//?????
   u8 parity;//???
}_LoRa_CFG;
 
#define LORA_AUX  PAin(4)    //LORA??????
#define LORA_MD0  PAout(15)  //LORA??????
 
//????(??:Kbps)
#define  LORA_RATE_0K3  0 //0.3
#define  LORA_RATE_1K2  1 //1.2
#define  LORA_RATE_2K4  2 //2.4
#define  LORA_RATE_4K8  3 //4.8
#define  LORA_RATE_9K6  4 //9.6
#define  LORA_RATE_19K2 5 //19.2
 
//????(??:?)
#define LORA_WLTIME_1S  0  //1?
#define LORA_WLTIME_2S  1  //2?
 
//????
#define LORA_MODE_GEN   0   //????
#define LORA_MODE_WK    1   //????
#define LORA_MODE_SLEEP 2   //????
 
//????  
#define LORA_PW_11dBm  0   //11dBm
#define LORA_PW_14Bbm  1   //14dBm
#define LORA_PW_17Bbm  2   //17dBm
#define LORA_PW_20Bbm  3   //20dBm
 
//????
#define LORA_STA_Tran 0 //????
#define LORA_STA_Dire 1 //????
 
//?????(??:bps)
#define LORA_TTLBPS_1200    0  //1200
#define LORA_TTLBPS_2400    1  //2400
#define LORA_TTLBPS_4800    2  //4800
#define LORA_TTLBPS_9600    3  //9600
#define LORA_TTLBPS_19200   4  //19200
#define LORA_TTLBPS_38400   5  //38400
#define LORA_TTLBPS_57600   6  //57600
#define LORA_TTLBPS_115200  7  //115200
 
//??????
#define LORA_TTLPAR_8N1  0 //8???
#define LORA_TTLPAR_8E1  1 //8???+1????
#define LORA_TTLPAR_8O1  2 //8???+1????
 
 
//??????
#define LORA_ADDR    1919                //????
#define LORA_CHN     19               //????
#define LORA_POWER   LORA_PW_20Bbm    //????
#define LORA_RATE    LORA_RATE_2K4   //????
#define LORA_WLTIME  LORA_WLTIME_1S   //????
#define LORA_MODE    LORA_MODE_GEN    //????
#define LORA_STA     LORA_STA_Tran    //????
#define LORA_TTLBPS  LORA_TTLBPS_115200 //???
#define LORA_TTLPAR  LORA_TTLPAR_8N1  //??? 
 
#define LORA_AUX  PAin(4)    //LORA??????
#define LORA_MD0  PAout(15)  //LORA??????
 
#define Dire_DateLen sizeof(Dire_Date)/sizeof(Dire_Date[0])
 
extern _LoRa_CFG LoRa_CFG;
extern u8 Lora_mode;
 
void lora_at_response(u8 mode);	
u8* lora_check_cmd(u8 *str);
u8 lora_send_cmd(u8 *cmd,u8 *ack,u16 waittime);
 
 
u8 LoRa_Configure(void);
void LoRa_Init(void);
void Aux_Int(u8 mode);
void LoRa_Set(void);
void LoRa_SendData(u8 Data[]);
u8* LoRa_ReceData(void);
 
 
 
#endif

*/