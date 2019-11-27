#include "stm32f4xx.h"                  // Device header
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


void usartinit(void);
void USART_Puts(USART_TypeDef* USARTx,volatile char *s);
char USART_read(void);
static void Delay(__IO uint32_t bekle);
void SysTick_Handler(void);
void Bekleme(void);
void servoup(void);
void servodown(void);
void Gpio_led_init();
char str[50];
uint32_t i=0,a=0;
char gelenVeri[50] ;
volatile uint32_t Delay_ms; 




int main(){
	
	/*
	usartinit();
	sprintf(str,"Firatmaca \r \n");
	USART_Puts(USART2,str);

	*/
	usartinit();
	Gpio_led_init();
	
	  while(1){
	sprintf(str,"Firatmaca \r \n");
	USART_SendData(USART2,*str);
	       /*
        if(gelenVeri[1] == '1')
        { 
					
					 GPIO_ResetBits(GPIOD,GPIO_Pin_12);
					GPIO_ResetBits(GPIOD,GPIO_Pin_13);
					GPIO_ResetBits(GPIOD,GPIO_Pin_14);
					GPIO_ResetBits(GPIOD,GPIO_Pin_15);
				}
				else{
			
					 GPIO_SetBits(GPIOD,GPIO_Pin_12);
					GPIO_SetBits(GPIOD,GPIO_Pin_13);
					GPIO_SetBits(GPIOD,GPIO_Pin_14);
					GPIO_SetBits(GPIOD,GPIO_Pin_15);
				}
 */
	Bekleme();
	Delay(1);
	 servoup();
	Delay(1);
	servodown();
	Delay(1);
	 servoup();
		
}
}
void Gpio_led_init(){
GPIO_InitTypeDef GPIO_InitStructure ;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT; 
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;  
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz; 
	GPIO_Init(GPIOD,&GPIO_InitStructure); 
}	


void usartinit(void){
 GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); // transmitter tx A2 GPIOya bagli usartta
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF; //Alternatif fonksiyonlar(input,output,adc disinda baska sey oldugu)
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);//USARTtan bilgiyi GPIOya atacaz,CPUya tanitmamiz lazim
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
	
	USART_InitStructure.USART_BaudRate=115200; // Saniye içinde hat üzerinden kaç tane bit gönderilmesi gerektigi
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART2,&USART_InitStructure);
	
	USART_Cmd(USART2,ENABLE);
}

void USART_Puts(USART_TypeDef* USARTx,volatile char *s) // char karakter sayisi kadar döndürüyor
{
 while(*s)
 {
  while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	 USART_SendData(USARTx,*s);
	 *s++;
 }	 
}	

char USART_read(void){
	while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
  char tmp = USART_ReceiveData(USART2);
	  	gelenVeri[a]=tmp;
	    USART_SendData(USART2, tmp);
	     a++;
	  return tmp;
}

void servoup(void){
		Bekleme();
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
		
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12 | GPIO_Pin_13 ;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd= GPIO_PuPd_UP;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4);
		
 
	TIM_TimeBaseStructure.TIM_Period=19999; //arr degeri
	TIM_TimeBaseStructure.TIM_Prescaler=84;//PrescalerValue; 84
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //0 'dan 20 bine kadar sayar Count 0 -> CNT
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
	TIM_Cmd(TIM4,ENABLE);
 
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;  //Output compare or Output control mode Set pulse when CNT==CCRx
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High; // Active High, i.e 0 -> 1 starts duty cycle
	// Kanal 1	
	
 //CNT,ARR,CNT
 
	//TIM_ARRPreloadConfig(TIM4,ENABLE);
	
		Delay(1);
	   TIM_OCInitStructure.TIM_Pulse=1750;//CCR1_Val; Initial duty cycle
	  TIM_OC1Init(TIM4,&TIM_OCInitStructure);
	  TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);
		Delay(1);
/*
    TIM_OCInitStructure.TIM_Pulse=2000;//CCR1_Val; Initial duty cycle
	  TIM_OC1Init(TIM4,&TIM_OCInitStructure);
	  TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);
		Delay(5);
		TIM_OCInitStructure.TIM_Pulse=1;//CCR1_Val; Initial duty cycle
	  TIM_OC1Init(TIM4,&TIM_OCInitStructure);
	  TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);
		Delay(5);
		*/
	
	
}
void servodown(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
		
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12 | GPIO_Pin_13 ;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd= GPIO_PuPd_UP;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4);
		
 
	TIM_TimeBaseStructure.TIM_Period=19999; //arr degeri
	TIM_TimeBaseStructure.TIM_Prescaler=84;//PrescalerValue; 84
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //0 'dan 20 bine kadar sayar Count 0 -> CNT
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
	TIM_Cmd(TIM4,ENABLE);
 
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;  //Output compare or Output control mode Set pulse when CNT==CCRx
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High; // Active High, i.e 0 -> 1 starts duty cycle
	// Kanal 1	
	
 //CNT,ARR,CNT
 
	//TIM_ARRPreloadConfig(TIM4,ENABLE);
		Delay(1);
	  TIM_OCInitStructure.TIM_Pulse=1;//CCR1_Val; Initial duty cycle
	  TIM_OC1Init(TIM4,&TIM_OCInitStructure);
	  TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);
		Delay(1);
}


void SysTick_Handler(void) 
	{
	Delay_ms++;
	}
 
static void Delay(__IO uint32_t bekle)
	{                                              
  uint32_t t= Delay_ms;
  while ((Delay_ms - t) < bekle);
  }
 
void Bekleme(void)
	{
 
	if (SysTick_Config(SystemCoreClock / 1000)) //1ms bekleme
		{
			while (1){};
	  }
  }

 
	
