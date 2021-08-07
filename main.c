#include "stm32f0xx.h"
#include "PWM_control.h"
#include "ADC_control.h"
#include "DMA_control.h"
#include "I_O_control.h"
#include "Communication.h"
#include "Contronller.h"
//#define  PSEUDO_COMMUTATION

int TIM1_Period;// currently configured as 1199 .PWM period, line 18, PWM.c - This value is stored i TIM1->ARR
int Frequency_ext=100;
int Frequency=0;
uint16_t ADC_Data[8]={0,0,0,0,0,0,0,0};
int Current_phase_A=0,Current_phase_B=0,Current_phase_C=0;// actual current per phase.
uint8_t  P5V_driver=0;
/*
uint8_t Step=0, Next_Step=0, Start_up=1;
int16_t Ramp_up_one_second=0;
int16_t Duty_old=0;
int zero_cross_period=0;
*/
uint8_t Rx_buff[8]={'0','0','0','0','0','0','0','0'};
uint8_t Tx_buff[8]={'S','0','0','0','0','0','0','E'};
int16_t Duty= First_PWM_width;
uint8_t Synchonous_check=0;
float Rotor_alpha=0, Roto_beta=0;
float angle=0;
uint32_t T_onA;
uint32_t Timer1_counter;
int	main(void)

	{
  	
		ADC_Common_config();
		DMA_ADC_config(0x40012440,ADC_Data);
		
		Timer1_PWM_Pin_Config();// Output_PWM_channel
		Timer1_PWM_config();
		I_O_function_config();
		
		////Do not touch this line below, I temporarily forget why should be this value
		TIM_SetCompare4(TIM1,TIM1_Period-1);// -100 is calibration param//do not modify channel 4, must be always this value unless a tuned event happen
		//using edge of OC4REF to trigger ADC conversion and automatically transfer data to array ADC_Data[8].
		//still do not understand why I have to use -100 to calib. ADC conversion moment does not happen at center of PWM pulse.
		//Timer3_sample_config();// Config timer for changing step at start up   
		UART1_Init(115200);
		DMA_UART1_config(Tx_buff,Rx_buff);
		Timer14_sample_config();
		
		//TIM_Cmd(TIM3,ENABLE);
		////////enable P5V_driver power for MOSFET trigger driver
		P5V_driver=1;//-P5V_driver;
		GPIO_WriteBit(GPIOB,GPIO_Pin_2,P5V_driver);
		//////////////////////////////////////////////
		//Ramp_up_one_second=First_PWM_width;
    TIM_SetCompare1(TIM1,500);
		TIM_SetCompare2(TIM1,500);
		TIM_SetCompare3(TIM1,500);
		//-------Start up before entering main loop--------------------
	//	Motor_Startup(&Step,&Next_Step,&Start_up);
	
		while(1){
		vector_rotate(0.3f,&angle,TIM14->ARR,TIM14->CNT,&Rotor_alpha,&Roto_beta);
		Space_vector_Modulation(Rotor_alpha,Roto_beta,&TIM1->CCR1,&TIM1->CCR2,&TIM1->CCR3);
		
//		T_onA=TIM1->CCR1;
//		Timer1_counter=TIM1->CNT;	
		//Toggle_PB9();
	}
}
	
		
		
		
		
		
void TIM3_IRQHandler(){
	
	

}

//----------------COM intterupt to adjust PWM out put in 3 channel--------------------------
void TIM1_BRK_UP_TRG_COM_IRQHandler(void){
	
	if(TIM_GetITStatus(TIM1,TIM_IT_COM)){
    TIM_ClearITPendingBit(TIM1,TIM_IT_COM);
		
	}
	
TIM_ClearITPendingBit(TIM1,TIM_IT_Update);

}
//-------------------------------------END-----------------------------------------------

//-----------Interrupt at center of PWM pulse to check zero crossing------------------------------

void TIM1_CC_IRQHandler(void){//line 7 PWM.c, connfig CCR interrupt only on channel 4.

	TIM_ClearITPendingBit(TIM1,TIM_IT_CC4);
	
	
	
}
//-------------END------------------------------------------------------------------------------

//-----------DMA interrupt to receive data from UART------------------------------
void DMA1_Channel2_3_IRQHandler(void){
		uint8_t i =0;
//	DMA_ClearITPendingBit(DMA1_IT_TC2);
	DMA_ClearITPendingBit(DMA1_IT_TC3);
	DMA_ClearFlag(DMA1_FLAG_TC3);
	DMA_ClearFlag(DMA1_FLAG_TC2);
	DMA_ClearITPendingBit(DMA1_IT_TC2);
//	DMA_Cmd(DMA1_Channel2,DISABLE);
//	DMA1_Channel2->CNDTR=8;

//	for(i=0;i< 8;i++) Tx_buff[i]=Rx_buff[i];
//	DMA_Cmd(DMA1_Channel2,ENABLE);
	if (Rx_buff[0] == 'S' && Rx_buff[7] == 'E'){
//		Duty_old=Duty;		
//		Duty = (Rx_buff[2]-48)*1000+(Rx_buff[3]-48)*100+(Rx_buff[4]-48)*10+(Rx_buff[5]-48);
				
				DMA_ClearITPendingBit(DMA1_IT_TC2);
				DMA_Cmd(DMA1_Channel2,DISABLE);
				DMA1_Channel2->CNDTR=8;
				Tx_buff[0]='S';
				Tx_buff[1]=' ';
				Tx_buff[2]='D';
				Tx_buff[3]='O';
				Tx_buff[4]='N';
				Tx_buff[5]='E';
				Tx_buff[6]=' ';
				Tx_buff[7]='E';
			
				DMA_Cmd(DMA1_Channel2,ENABLE);
				
				}
}

void TIM14_IRQHandler(){
	Toggle_PB9();
	TIM_ClearITPendingBit(TIM14,TIM_IT_Update);
}
void DMA1_Channel1_IRQHandler(void){
	    DMA_ClearFlag(DMA1_FLAG_TC1);
}