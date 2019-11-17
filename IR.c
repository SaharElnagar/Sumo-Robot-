#include "IR.h"

 
 
 
 
 void ADC_Init(void){
	 
	     
   int delay ;
	 SYSCTL_RCGC2_R|=0X10;                // Run Clock on Port E
	 delay=SYSCTL_RCGC2_R;
   SYSCTL_RCGC0_R |= 0x00010000;       // 6) activate ADC0
	 delay=SYSCTL_RCGC0_R;
   SYSCTL_RCGC0_R &= ~0x00000300;      // 7) configure for 125K
	 GPIO_PORTE_LOCK_R=0X4C4F434B;
	 GPIO_PORTE_CR_R|=0X0F;
	 GPIO_PORTE_DIR_R&=~0X0F;            // Input signal
	 GPIO_PORTE_DEN_R&=~0X0F;            // Disable Digital
	 GPIO_PORTE_AMSEL_R|=0X0F;           // Enable Analog
	 GPIO_PORTE_AFSEL_R|=0X0F;           // Alternative Function
	 
	 ADC0_ACTSS_R &=~0xF;             // Disable  sequence 1,2,3,4 during init
   ADC0_SSPRI_R = 0x1203;              // Seq 1  priority0 
	 ADC0_EMUX_R &= ~0xFFFF;             // the trigger source for Sample (Seq 1,2,3,4) is the processor
	 
	 ADC0_SSMUX0_R &= ~0xF;           // clear SS1 field
	 ADC0_SSMUX1_R &= ~0xF; 
	 ADC0_SSMUX2_R &= ~0xF; 
	 ADC0_SSMUX3_R &= ~0xF; 
   ADC0_SSMUX0_R |= 0x0000;  	         //set channel Ain0 (PE3) for first sample
   ADC0_SSMUX1_R |= 0x0001;                                      // set channel Ain1 (PE2) for second sample                                  
	 ADC0_SSMUX2_R |= 0x0002;																		//	set channel Ain2 (PE1) for third sample
	 ADC0_SSMUX3_R |= 0x0003;																		//	set channel Ain0 (PE0) for fourth sample
	 ADC0_SSCTL0_R = 0x0006;             // no TS0 D0, yes IE0 END0
	 ADC0_SSCTL1_R = 0x0006;
	 ADC0_SSCTL2_R = 0x0006;
	 ADC0_SSCTL3_R = 0x0006;
	
   ADC0_ACTSS_R |= 0x000F;             // enable sample sequencer 1,3
	 
   }
                                    // ***************************//
	 
	 
	 

 void IR_Read(int* result){
      
      ADC0_PSSI_R = 0x0001;            // 1) initiate SS1
      while((ADC0_RIS_R&0x01)==0){};   // 2) wait for conversion done
      result[3] = ADC0_SSFIFO0_R&0xFFF;   // 3) read data
      ADC0_ISC_R = 0x0001;             // 4) acknowledge completion
			ADC0_PSSI_R = 0x0002;
			while((ADC0_RIS_R&0x02)==0){};   // 2) wait for conversion done
      result[2] = ADC0_SSFIFO1_R&0xFFF;   // 3) read data
      ADC0_ISC_R = 0x0002;             // 4) acknowledge completion
			ADC0_PSSI_R= 0x0004;
			while((ADC0_RIS_R&0x04)==0){};   // 2) wait for conversion done
      result[1] = ADC0_SSFIFO2_R&0xFFF;   // 3) read data
      ADC0_ISC_R = 0x0004;             // 4) acknowledge completion
			ADC0_PSSI_R = 0x0008;
			while((ADC0_RIS_R&0x08)==0){};   // 2) wait for conversion done
      result[0  ] = ADC0_SSFIFO3_R&0xFFF;   // 3) read data
      ADC0_ISC_R = 0x0008;             // 4) acknowledge completion
      
 }

