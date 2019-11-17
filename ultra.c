
#include "ultra.h"


void Ultrasonic_init(void){
int delay;	
                              /*************PortA,B_Init*************/
SYSCTL_RCGCGPIO_R|=0x03  ;
delay= SYSCTL_RCGC2_R;
SYSCTL_RCGCTIMER_R|=0x0C;        // T2 & T3
delay=SYSCTL_RCGCTIMER_R;
	//portA trigger
GPIO_PORTA_LOCK_R=0x4C4F434B ;
GPIO_PORTA_CR_R|=0xF0;   //A4,5,6,7
GPIO_PORTA_DIR_R|=0xF0;
GPIO_PORTA_DEN_R|=0xF0;
GPIO_PORTA_AFSEL_R&=~0xF0;
GPIO_PORTA_AMSEL_R&=~0xF0;
GPIO_PORTA_PCTL_R=0;

	//PortB echo
GPIO_PORTB_LOCK_R=0x4C4F434B ;
GPIO_PORTB_CR_R|=0x0F;
GPIO_PORTB_DIR_R&=~0x0F;
GPIO_PORTB_DEN_R|=0x0F;
GPIO_PORTB_AFSEL_R|=0x0F;
GPIO_PORTB_AMSEL_R&=~0x0F;
GPIO_PORTB_PCTL_R&=~0x0000FFFF;
GPIO_PORTB_PCTL_R|=  0x00007777;	

	
	                           /**************Timer0,1_Init****************/
	
	TIMER2_CTL_R&=~0x0101 ;    //disable timer A&B
	TIMER3_CTL_R&=~0x0101 ; 
	TIMER2_CFG_R  =0x04;
	TIMER3_CFG_R  =0x04;
	TIMER2_TAMR_R|=0x17;
	TIMER3_TAMR_R|=0x17;
	TIMER2_TBMR_R|=0x17;
	TIMER3_TBMR_R|=0x17;
	TIMER2_CTL_R|=0x0C0C ;    //both edges
	TIMER3_CTL_R|=0x0C0C ;
	   

	
	
	


}


void distance_in_cm(int* dis){
static  unsigned long high,low;

//echo
	
	GPIO_PORTA_DATA_R|=0x80;
  delay_microSec(10);	
  GPIO_PORTA_DATA_R&=~0x80;
	
TIMER2_CTL_R|=0x0001 ; 
TIMER2_ICR_R|=0x04;
while((TIMER2_RIS_R&0x04)==0)	{}
high=(TIMER2_TAR_R);
TIMER2_ICR_R|=0x04;
while((TIMER2_RIS_R&0x04)==0)	{}
TIMER2_CTL_R&=~0x0001 ; 
low=	TIMER2_TAR_R;
TIMER2_ICR_R|=0x04;
dis[0]=(low-high)/1000;
	
	GPIO_PORTA_DATA_R|=0x40;
  delay_microSec(10);	
  GPIO_PORTA_DATA_R&=~0x40;
	/*
//echo	
	
	TIMER2_CTL_R|=0x0100 ;
	TIMER2_ICR_R|=0x400;
while((TIMER2_RIS_R&0x400)==0)	{}
high=(TIMER2_TBR_R);
TIMER2_ICR_R|=0x400;
while((TIMER2_RIS_R&0x400)==0)	{}
TIMER2_CTL_R&=~0x0100 ; 
low=	TIMER2_TBR_R;
TIMER2_ICR_R|=0x400;
dis[1]=(low-high)/1000;

	
		GPIO_PORTA_DATA_R|=0x20;
  delay_microSec(10);	
  GPIO_PORTA_DATA_R&=~0x20;

//echo
	
	
TIMER3_CTL_R|=0x0001 ; 
//TIMER3_ICR_R|=0x04;
while((TIMER3_RIS_R&0x04)==0)	{}
high=(TIMER3_TAR_R);
TIMER3_ICR_R|=0x04;
while((TIMER3_RIS_R&0x04)==0)	{}
TIMER3_CTL_R&=~0x0001 ; 
low=	TIMER3_TAR_R;
	TIMER3_ICR_R|=0x04;
dis[2]=(low-high)/1000;	
	
	GPIO_PORTA_DATA_R|=0x10;
  delay_microSec(10);	
  GPIO_PORTA_DATA_R&=~0x10;

//echo
TIMER3_CTL_R|=0x0100 ; 
TIMER3_ICR_R|=0x400;
while((TIMER3_RIS_R&0x400)==0)	{}
high=(TIMER3_TBR_R);
TIMER3_ICR_R|=0x400;
while((TIMER3_RIS_R&0x400)==0)	{}
TIMER3_CTL_R&=~0x0100 ; 
low=	TIMER3_TBR_R;
dis[3]=(low-high)/1000;
	*/
}

void delay(void){
	    // USE Timer0A T0ccp0 on PB6 
    unsigned long  delay ;
	  SYSCTL_RCGCGPIO_R|=0x20;
	  delay= SYSCTL_RCGC2_R;
    SYSCTL_RCGCTIMER_R |=1;     // Timer0
    delay = SYSCTL_RCGCTIMER_R;
	//PF0 init
	  GPIO_PORTF_LOCK_R =0x4C4F434B;
    GPIO_PORTF_CR_R |=0x1; 
	  GPIO_PORTF_DIR_R |=0x1;  // output
	  GPIO_PORTF_DEN_R |=0x1;
	  GPIO_PORTF_AFSEL_R |=0x1;
    GPIO_PORTF_PCTL_R  &=~ 0xFFFFFFFF;  
	  GPIO_PORTF_PCTL_R  =   0x00000007;   //T0ccp0
    GPIO_PORTF_AMSEL_R &=~0x1;
    
	
//Timer0 Init
    TIMER0_CTL_R =0;    // disable timer while init
    TIMER0_CFG_R |=0x4;     // 16 bit mode 
    TIMER0_TAMR_R |=0x02; //  periodic mode
    TIMER0_TAMR_R &=~0x10; // count down 
    TIMER0_TAILR_R = 16-1;      // 16000===> 1ms
    
    TIMER0_CTL_R |=0x01;      // enable timer0

}
 


void delay_microSec(int time){
int i;
	 for(i=0;i<time;i++){
    while ((TIMER0_RIS_R &0x01)==0){}
  TIMER0_ICR_R |=0x01;
}
}

