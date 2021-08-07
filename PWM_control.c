#include "PWM_control.h"


//Use PWM generation of TIM1_CH3N at PB1 or try to use TIM3_CH4 to check the independence
//USE quadrature support of TIM3_CH1 and TIM3_CH2 at PA6 and PA7 respectively

//Configure TIM1_PWM
void Timer1_PWM_config(void){
	
	TIM_TimeBaseInitTypeDef TIM_TimeBasePWMInitial;
	TIM_OCInitTypeDef TIM_OCPWMInitial;
	NVIC_InitTypeDef NVIC_Initial;
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE); 
	////////////Config Basic Timer//////////////////
	TIM_TimeBasePWMInitial.TIM_Prescaler=0;//
	Frequency=40000;
	TIM_TimeBasePWMInitial.TIM_Period=(SystemCoreClock/1/Frequency)-1;//PWM frequency=20kHz because of using center aligned mode
	TIM1_Period=TIM_TimeBasePWMInitial.TIM_Period;
	TIM_TimeBasePWMInitial.TIM_CounterMode=TIM_CounterMode_CenterAligned2;//Center Aligned 1, interrupt flag is set when TIM1_CNT > TIM1_CCR1 (counting down, 2: counting up, 3: up and down)
	TIM_TimeBasePWMInitial.TIM_ClockDivision=0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBasePWMInitial);//Config it
	
	//////////////////Config PWM Source//////////////////////////////
	TIM_OCPWMInitial.TIM_OCMode= TIM_OCMode_PWM1;// PWM1 : OC output active when TIM1_CNT<TIM1_CCR1 (counting up) and TIM1_CNT<TIM1_CCR1(counting down), inverse for TIM_OCMode_PWM1
	TIM_OCPWMInitial.TIM_OutputState=TIM_OutputState_Enable;//Enable Output
	TIM_OCPWMInitial.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCPWMInitial.TIM_OCPolarity=TIM_OCPolarity_High;//PWM output active High
	TIM_OCPWMInitial.TIM_OCNPolarity=TIM_OCNPolarity_High;//PWM output active High
	//TIM_OCPWMInitial.TIM_Pulse=500;
	TIM_OCPWMInitial.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCPWMInitial.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	////////////////Interrupt////////////////
	/*
	NVIC_Initial.NVIC_IRQChannel = TIM1_BRK_UP_TRG_COM_IRQn;// TIM1 Break, Update, Trigger and Commutation Interrupts// step communication.
	NVIC_Initial.NVIC_IRQChannelPriority=2;
	NVIC_Initial.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_Initial);
	*/
	
	
	
	//////////////Config dead time/////////////
	//TIM_BDTRInitStructure.TIM_DeadTime=10;
	//TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);
	//TIM_Cmd(TIM1,ENABLE);
	/////////////////Config every single channel////////////////
	TIM_OC1Init(TIM1,&TIM_OCPWMInitial);//channel 1
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);//config preloading Period

	TIM_OC2Init(TIM1,&TIM_OCPWMInitial);//channel 2
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);//config preloading Period

	TIM_OC3Init(TIM1,&TIM_OCPWMInitial);//channel 3
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);//config preloading Period
	
	//TIM_OCPWMInitial.TIM_OCMode= TIM_OCMode_PWM3;
	TIM_OC4Init(TIM1,&TIM_OCPWMInitial);//channel 4
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);//config preloading Period
			
		/*You must enable the corresponding preload register by setting the
	OCxPE bit in the TIMx_CCMRx register, and eventually the auto-reload preload register (in
	upcounting or center-aligned modes) by setting the ARPE bit in the TIMx_CR1 register.(p.380)*/
		//TIM_ARRPreloadConfig(TIM4, ENABLE);
		
 	//Config TImer master slave mode to output a trigger to ADC to start conversion
	TIM_SelectMasterSlaveMode(TIM1,TIM_MasterSlaveMode_Enable);
	TIM_SelectOutputTrigger(TIM1,TIM_TRGOSource_OC4Ref);//TIM_TRGOSource_OC4Ref: OC4REF signal is used as the trigger output (TRGO)// 
	//OCxRef signal depend on what!?//depend on comparing TIM_CNT and TIM_CCR
	TIM_Cmd(TIM1, ENABLE);//Enable Timer 1
	TIM_CtrlPWMOutputs(TIM1, ENABLE);		
	//TIM_ITConfig(TIM1, TIM_IT_Update|TIM_IT_CC4,ENABLE);// OUTPUT COMPARE INTERRUPT HAPPENS ONLY ON CHANNEL 4, CONFIG HERE!!
}
//Configure PWM pin in TIM1_CH3N PB1
void Timer1_PWM_Pin_Config(void){
	
	GPIO_InitTypeDef GPIO_PWM_Initial;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
	
	////Use PA8, PA9, PA10 for PWM output 1,2,3	
	GPIO_PWM_Initial.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
	GPIO_PWM_Initial.GPIO_Mode= GPIO_Mode_AF;
	GPIO_PWM_Initial.GPIO_Speed= GPIO_Speed_Level_2;
	GPIO_PWM_Initial.GPIO_OType= GPIO_OType_PP;//Push-pull mode
	GPIO_PWM_Initial.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_PWM_Initial);
	
	//////Use PB13, PB14, PB15 for PWM output_N 1,2,3
	GPIO_PWM_Initial.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init(GPIOB,&GPIO_PWM_Initial);
	
	//////Config AF mode/////////////////
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_2);// PA10 Alternate function 2 is TIM1_CH3
	////////////////////////////////////
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_2);// PA10 Alternate function 2 is TIM1_CH3

}
void Timer3_sample_config(void){
 
	/* TIM3 clock enable */
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitial;
		NVIC_InitTypeDef NVIC_Initial;
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	  TIM_TimeBaseInitial.TIM_Prescaler=(SystemCoreClock/TIM3_clock)-1; //timer3_clock=10kHz;
		TIM_TimeBaseInitial.TIM_Period =50000-1;//first setting for motor startup, take 40 pulses (2ms vs 20kHz pwm) before changing to next step in startup phases
	  //just thinking about tuning this value 
	  #ifdef PSEUDO_COMMUTATION	
	  TIM_TimeBaseInitial.TIM_Prescaler=(SystemCoreClock/1000)-1; //timer_clock=1kHz;
		TIM_TimeBaseInitial.TIM_Period = Sample_period-1;
		#endif
	
		TIM_TimeBaseInitial.TIM_CounterMode=TIM_CounterMode_Up;
		TIM_TimeBaseInitial.TIM_ClockDivision=0;
		TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitial);
		////////////////Interrupt////////////////
		NVIC_Initial.NVIC_IRQChannel = TIM3_IRQn;
		NVIC_Initial.NVIC_IRQChannelPriority=0;
		NVIC_Initial.NVIC_IRQChannelCmd=ENABLE;
		NVIC_Init(&NVIC_Initial);
		
		//TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//lack compare interrupt. Should be configured after startup success
		TIM_ITConfig(TIM3,TIM_IT_CC1|TIM_IT_Update,ENABLE);
		//TIM_Cmd(TIM3,ENABLE);
 }
void Sample_indicator_config(){
		GPIO_InitTypeDef GPIO_OutInitStructure;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
		
		GPIO_OutInitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
		GPIO_OutInitStructure.GPIO_Mode= GPIO_Mode_OUT;
		GPIO_OutInitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_OutInitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_OutInitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOA, &GPIO_OutInitStructure);
		
	}





void Timer14_sample_config(void){

	/* TIM6 clock enable */
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitial;
		NVIC_InitTypeDef NVIC_Initial;
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);
	  TIM_TimeBaseInitial.TIM_Prescaler=(SystemCoreClock/TIM14_clock)-1; //timer14_clock=1kHz;
		TIM_TimeBaseInitial.TIM_Period =100;//1 second period.
	  
	
		TIM_TimeBaseInitial.TIM_CounterMode=TIM_CounterMode_Up;
		TIM_TimeBaseInitial.TIM_ClockDivision=0;
		TIM_TimeBaseInit(TIM14,&TIM_TimeBaseInitial);
		////////////////Interrupt////////////////
		NVIC_Initial.NVIC_IRQChannel = TIM14_IRQn;
		NVIC_Initial.NVIC_IRQChannelPriority=4;
		NVIC_Initial.NVIC_IRQChannelCmd=ENABLE;
		NVIC_Init(&NVIC_Initial);
		
		TIM_ITConfig(TIM14,TIM_IT_Update,ENABLE);//lack compare interrupt. Should be configured after startup success
		TIM_Cmd(TIM14,ENABLE);
		
}
