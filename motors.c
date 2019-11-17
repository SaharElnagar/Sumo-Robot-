#include "motors.h" 

void PWM_Init(void){
	                  
                            //**** Init PWM0 & PWM1 on PB6 & PB7 ****//
	int delay;
 SYSCTL_RCGC0_R|=0x00100000;               // Enable PWM Clock
	delay=SYSCTL_RCGC0_R;
	SYSCTL_RCGC2_R|=0X02;                    //Enable the clock to PORT B
	delay=SYSCTL_RCGC2_R;
	GPIO_PORTB_LOCK_R=0X4C4F434B;
	GPIO_PORTB_CR_R|=0XF0;
	GPIO_PORTB_DIR_R|=0XF0;                // Output signal
	GPIO_PORTB_DEN_R|=0XF0;                // Enable Digital
	GPIO_PORTB_AMSEL_R&=~0XF0;             // Disable Analog
	GPIO_PORTB_AFSEL_R|=0XF0;              // Alternative Function
	GPIO_PORTB_PCTL_R&=~0XFFFF0000;
	GPIO_PORTB_PCTL_R|= 0X44440000;
	GPIO_PORTB_DR8R_R |=0XF0;
	SYSCTL_RCC_R &=~0x00100000;           // Disable PWM divider clock and use system clock 
	PWM0_0_CTL_R=0 ;  	                  // COUNT DOWN MODE, disable during config
	PWM0_1_CTL_R=0 ;
	PWM0_0_GENA_R |=0XC8 ;                // LOW ON LOAD , HIGH ON CMPA
	PWM0_0_GENB_R |=0XC08 ;               // LOW ON LOAD , HIGH ON CMPB
	PWM0_1_GENA_R |=0XC8 ;                // LOW ON LOAD , HIGH ON CMPA
	PWM0_1_GENB_R |=0XC08 ;               // LOW ON LOAD , HIGH ON CMPB
	PWM0_0_LOAD_R=8000-1 ;                // PWM FREQUENCY= 2000 HZ
	PWM0_1_LOAD_R=8000-1 ;
	
	//Direction_Init >>PORTD&&PORTE<<
	SYSCTL_RCGC2_R|=0X19;  
	delay=SYSCTL_RCGC2_R;
	GPIO_PORTE_LOCK_R=0X4C4F434B;
	GPIO_PORTE_CR_R|=0X30;
	GPIO_PORTE_DIR_R|=0X30;                // Output signal
	GPIO_PORTE_DEN_R|=0X30;                // Enable Digital
	GPIO_PORTE_AMSEL_R&=~0X30;             // Disable Analog
	GPIO_PORTE_AFSEL_R&=~0X30;              
	GPIO_PORTE_PCTL_R=0;
		//postD pins>>0,1,2,3<<
	GPIO_PORTD_LOCK_R=0X4C4F434B;
	GPIO_PORTD_CR_R|=0X0F;
	GPIO_PORTD_DIR_R|=0X0F;                // Output signal
	GPIO_PORTD_DEN_R|=0X0F;                // Enable Digital
	GPIO_PORTD_AMSEL_R&=~0X0F;             // Disable Analog
	GPIO_PORTD_AFSEL_R&=~0X0F;              
	GPIO_PORTD_PCTL_R=0;
	
			//postA pins>>2,3<<
	GPIO_PORTA_LOCK_R=0X4C4F434B;
	GPIO_PORTA_CR_R|=0X0C;
	GPIO_PORTA_DIR_R|=0X0C;                // Output signal
	GPIO_PORTA_DEN_R|=0X0C;                // Enable Digital
	GPIO_PORTA_AMSEL_R&=~0X0C;             // Disable Analog
	GPIO_PORTA_AFSEL_R&=~0X0C;              
	GPIO_PORTA_PCTL_R=0;
	
	
}

void Motor_Duty(unsigned long duty0,unsigned long duty1,unsigned long duty2,unsigned long duty3){
	
  PWM0_0_CMPB_R=duty0-2;      // DECIDE DUTY PB7	
	PWM0_0_CMPA_R=duty1-2 ;      // DECIDE DUTY PB6 	
	PWM0_1_CMPB_R=duty2-2;      // DECIDE DUTY PB5	
	PWM0_1_CMPA_R=duty3-2 ;      // DECIDE DUTY PB4 
	PWM0_0_CTL_R=0X00000001 ;  // START PWM 
	PWM0_1_CTL_R=0X00000001 ;  // START PWM 
  PWM0_ENABLE_R=0X0000000F;  // ENABLE PWM 
	
}
