/*
 * Sumo.c
 *
 *  Created on: November, 2017
 *  Author: Sahar Elnagar
 */

#include "motors.h"
#include "IR.h"
#include "ultra.h"
#define SCB_CPAC (*((volatile unsigned int*)0xE000ED88))
int flag=0; // indicator for IR
int ir[4];
int us[4];
int count=1,ccc=0;
//check white ground every 25ms
void SysTick_Handler(void){ 
	
  IR_Read(ir);
  if(ir[0]<2000||ir[1]<2000||ir[2]<2000||ir[3]<2000)
   flag=1;   //White
  else{
  count++;
	 if(count==2){
		distance_in_cm(us);
	 flag=0; //Black
		 count=0;
	 }
		  
}
	ccc++;
}

void EnableInterrupts(void);
void SysTick_Init(void);
void Move(char c);
int main(){
//int duty0=8000,duty1=0,duty2=0,duty3=0;
int flags[4]={0};
//PWM_Init();
//ADC_Init();
Ultrasonic_init();
delay();
//delay_microSec(4000000);
 SysTick_Init();

 
	

while(1){
	Move('F'); 
	Motor_Duty(8000,8000,8000,8000);
 if(flag==1){ 
	                     //**********white ground**********//
	
	// Move_Backward //
	
if(ir[0]<2000&&ir[1]<2000){
 Move('B');
Motor_Duty(7000,7000,7000,7000);
}
   // move_Right

else if((ir[1]<2000||ir[2]<2000)&&ir[3]>2000&&ir[0]>2000){
 Move('R');
Motor_Duty(7000,7000,7000,7000);
}
  // Move_Forward

else if(ir[2]<2000&&ir[3]<2000){
 Move('F');
Motor_Duty(7000,7000,7000,7000);
}
      // Move_Left
else if((ir[3]<2000||ir[0])&&ir[2]>2000&&ir[1]>2000){
 Move('R');
Motor_Duty(7000,7000,7000,7000);
}}



                    //********* Black ground**********//

 else{
	

	while(us[0]>140&&us[1]>140&&us[2]>140&&us[3]>140&&flag==0){
   
		//turn right till an ultrasonic detects the enemy
		
		Move('R');
   Motor_Duty(7000,7000,7000,7000);
	}
if((us[0]<140&&us[1]>140&&us[2]>140&&us[3]>140)){  // Front ultra detects

			Move('B');
   Motor_Duty(7000,7000,7000,7000);
	
	
	}
	
	else if((us[0]>140&&us[1]<140&&us[2]>140&&us[3]>140)||flags[1]==1){  //Left ultra detects
		flags[1]=1;
		Move('B');
		Motor_Duty(8000,8000,8000,8000);
		delay_microSec(150000); 
		
		
		while(us[0]>140&&flag==0){
			Move('L');
		
		}
		flags[1]=0;
	}
	 	else if((us[0]>140&&us[1]>140&&us[2]>140&&us[3]<140)||flags[2]==1){  //Right ultra detects
		flags[2]=1;
		Move('B');
		Motor_Duty(8000,8000,8000,8000);
		delay_microSec(150000); 
			
			while(us[0]>140&&flag==0){
			Move('R');
		
		}
		flags[2]=0;
			
		
	}
		else if((us[0]>140&&us[1]>140&&us[2]<140&&us[3]>140)||flags[3]==1){ //Back ultra detects
			flags[3]=1;
    Move('F');
	  Motor_Duty(8000,8000,8000,8000);
		delay_microSec(150000);
		while(us[0]>140&&flag==0){
		Move('R');
		}
		flags[3]=0;
		
}}
}
}






                     /*****************
	      
	0---1     0---1    0---1     0---1
	D0&&D4    D1&&D5   D2&&E4    D3&&E5
	
	wheel_0   wheel_1  wheel_2   wheel_3
	
	Backward  Backward Backward  Backward

	
	               wheel_1************wheel_0 
	                      *          * 
                        *          * 
	                      *          * 
	               wheel_2************wheel_3
								 
								 
	                     ******************/
void Move(char c){

if(c=='R'){

      // move_Right

	/*
		0---1     1---0    1---0     0---1
  	D0&&A2    D1&&A3   D2&&E4    D3&&E5
	
	  wheel_0   wheel_1  wheel_2   wheel_3
	

	*/

GPIO_PORTD_DATA_R&=~0x09;	
GPIO_PORTD_DATA_R|=0x06; 
GPIO_PORTA_DATA_R&=~0x8;	
GPIO_PORTA_DATA_R|=0x4; 
GPIO_PORTE_DATA_R&=~0x10;	
GPIO_PORTE_DATA_R|=0x20;
}

if(c=='L'){
      // Move_Left
   
	/*
		1---0     0---1    0---1     1---0
  	D0&&A2    D1&&A3   D2&&E4    D3&&E5
	
	  wheel_0   wheel_1  wheel_2   wheel_3
	

	*/
GPIO_PORTD_DATA_R&=~0x06;
GPIO_PORTD_DATA_R|=0x09; 
GPIO_PORTA_DATA_R&=~0x4;	
GPIO_PORTA_DATA_R|=0x8; 
GPIO_PORTE_DATA_R&=~0x20;	
GPIO_PORTE_DATA_R|=0x10;
}
	
if(c=='F'){
     
	  // Move_Forward
	/*
		1---0     1---0    1---0     1---0
  	D0&&A2    D1&&A3   D2&&E4    D3&&E5
	
	  wheel_0   wheel_1  wheel_2   wheel_3
	

	*/
GPIO_PORTA_DATA_R&=~0xC;	
GPIO_PORTD_DATA_R|=0x0F;    
GPIO_PORTE_DATA_R&=~0x30;

}

if(c=='B'){


	/*
	         Move_Backward
		0---1     0---1    0---1     0---1
  	D0&&D4    D1&&D5   D2&&E4    D3&&E5
	
	  wheel_0   wheel_1  wheel_2   wheel_3
	
	  Backward  Backward Backward  Backward
	*/
	                               
GPIO_PORTD_DATA_R&=~0x0F;   
GPIO_PORTA_DATA_R|= 0xC;
GPIO_PORTE_DATA_R|=0x30;

}	

}

  void SysTick_Init(void){
	NVIC_ST_CTRL_R= 0 ;            // disable systick 
	NVIC_ST_RELOAD_R= 400000-1 ;     // 40 Hz---->25ms
	NVIC_ST_CURRENT_R= 0 ;
	NVIC_SYS_PRI3_R &=~0XFF000000 ;  // priority 0
	NVIC_ST_CTRL_R= 0X07 ;  // enable SYSTICK with Interrupts
	EnableInterrupts();

}
	


void SystemInit (void){
         SCB_CPAC |= 0x00F00000;
 }