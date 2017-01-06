#include "tm4c123gh6pm.h"
#include "PWM.h"
#include "Timer0A.h"

// period is 16-bit number of PWM clock cycles in one period (3<=period)
// duty is number of PWM clock cycles output is high  (2<=duty<=period-1)

// PWM clock rate = processor clock rate/SYSCTL_RCC_PWMDIV
//                = BusClock/32 
//                = 80 MHz/32 = 2.5 MHz

// PWM Frequency = 400 Hz --> period = 6250 (PWM_clock)
// Pulse Width = 1~2 ms --> duty = 2500~5000
// Resolution: Delta_duty = 5 clock (2us)


// Output on PF0/M1PWM4 --> MOTOR 1
// Output on PC5/M0PWM7 --> MOTOR 2
// Output on PB4/M0PWM2 --> MOTOR 3
// Output on PE4/M1PWM2 --> MOTOR 4

float Motors_PWM[4] = {900.0f,900.0f,900.0f,900.0f}; // [1000,2000]

void Motor_PWM_Init(uint16_t period, uint16_t duty){
	SYSCTL_RCGCPWM_R |= 0x03;							// 1) activate PWM0 & PWM1
	SYSCTL_RCGCGPIO_R |= 0x36; 						// 2) activate port F, E, C, B
	while((SYSCTL_PRGPIO_R&0x36) == 0){};	// ready?

// Output on PE4/M1PWM2 --> MOTOR 4		
	GPIO_PORTE_AFSEL_R |= 0x10;						// enable alt function on PE4
	GPIO_PORTE_PCTL_R &= ~0x000F0000;			// configure PE4 as M1PWM2
	GPIO_PORTE_PCTL_R |= 0x00050000;
	GPIO_PORTE_AMSEL_R &= ~0x10;					// disable analog functionality on PE4
	GPIO_PORTE_DEN_R |= 0x10;							// enable digital I/O on PE4
				
// Output on PF0/M1PWM4 --> MOTOR 1		
	GPIO_PORTF_LOCK_R = 0x4C4F434B;   // unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
		
	GPIO_PORTF_AFSEL_R |= 0x01;						// enable alt function on PF0
	GPIO_PORTF_PCTL_R &= ~0x0000000F;			// configure PF0 as M1PWM4
	GPIO_PORTF_PCTL_R |= 0x00000005;
	GPIO_PORTF_AMSEL_R &= ~0x01;					// disable analog functionality on PF0
	GPIO_PORTF_DEN_R |= 0x01;							// enable digital I/O on PF0	
		
// Output on PC5/M0PWM7 --> MOTOR 2	
	GPIO_PORTC_AFSEL_R |= 0x20;						// enable alt function on PC5
	GPIO_PORTC_PCTL_R &= ~0x00F00000;			// configure PC5 as M0PWM7
	GPIO_PORTC_PCTL_R |= 0x00400000;
	GPIO_PORTC_AMSEL_R &= ~0x20;					// disable analog functionality on PC5
	GPIO_PORTC_DEN_R |= 0x20;							// enable digital I/O on PC5

// Output on PB4/M0PWM2 --> MOTOR 3
	GPIO_PORTB_AFSEL_R |= 0x10;						// enable alt function on PB4
	GPIO_PORTB_PCTL_R &= ~0x000F0000;			// configure PB4 as M0PWM2
	GPIO_PORTB_PCTL_R |= 0x00040000;
	GPIO_PORTB_AMSEL_R &= ~0x10;					// disable analog functionality on PB4
	GPIO_PORTB_DEN_R |= 0x10;							// enable digital I/O on PB4
	
// Configure PWM		
	SYSCTL_RCC_R |= SYSCTL_RCC_USEPWMDIV;	// 3) use PWM divider
	SYSCTL_RCC_R &= ~SYSCTL_RCC_PWMDIV_M;	//    clear PWM divider field
	SYSCTL_RCC_R |= SYSCTL_RCC_PWMDIV_32;	//    configure for /32 divider
	
	PWM1_1_CTL_R = 0;											// 4) re-loading down-counting mode
	PWM1_1_GENA_R = 0xC8;
	PWM1_2_CTL_R = 0;											
	PWM1_2_GENA_R = 0xC8;
	PWM0_3_CTL_R = 0;											
	PWM0_3_GENB_R = 0xC08;
	PWM0_1_CTL_R = 0;											
	PWM0_1_GENA_R = 0xC8;
	
	PWM1_1_LOAD_R = period - 1;						// 5) cycles needed to count down to 0
	PWM1_2_LOAD_R = period - 1;
	PWM0_3_LOAD_R = period - 1;
	PWM0_1_LOAD_R = period - 1;
	
	PWM1_1_CMPA_R = duty - 1;							// 6) count value when output rises
	PWM1_2_CMPA_R = duty - 1;
	PWM0_3_CMPB_R = duty - 1;
	PWM0_1_CMPA_R = duty - 1;
	
	PWM1_1_CTL_R |= 0x00000001;						// 7) start timer on PWM1 Generator 1
	PWM1_2_CTL_R |= 0x00000001;						// 7) start timer on PWM1 Generator 2
	PWM0_3_CTL_R |= 0x00000001;						// 7) start timer on PWM0 Generator 3
	PWM0_1_CTL_R |= 0x00000001;						// 7) start timer on PWM0 Generator 1
	
	PWM1_ENABLE_R |= 0x00000014;					// 8) enable PB4/M1PWM2 and PF0/M1PWM4	
	PWM0_ENABLE_R |= 0x00000084;					// 8) enable PC5/M0PWM7 and PB4/M0PWM2
}

void Motor4_PWM_duty(int16_t duty){
	PWM1_1_CMPA_R = duty - 1;
}

void Motor1_PWM_duty(int16_t duty){
	PWM1_2_CMPA_R = duty - 1;
}

void Motor2_PWM_duty(int16_t duty){
	PWM0_3_CMPB_R = duty - 1;
}

void Motor3_PWM_duty(int16_t duty){
	PWM0_1_CMPA_R = duty - 1;
}

void pulseMotors(uint8_t quantity){
	uint8_t i,j;
	for(i = 0; i < quantity; i++){
		for(j = 0; j < 4; j++){Motors_PWM[j] = 1100.0;}
		Update_MotorsPWM();
		Delay_ms(5);
		
		for(j = 0; j < 4; j++){Motors_PWM[j] = 900.0;}
		Update_MotorsPWM();
		Delay_ms(5);
	}
}

void Update_MotorsPWM(void){
	Motor4_PWM_duty((int16_t)(Motors_PWM[3] * 2.5f));
	Motor2_PWM_duty((int16_t)(Motors_PWM[1] * 2.5f));
	Motor3_PWM_duty((int16_t)(Motors_PWM[2] * 2.5f));
	Motor1_PWM_duty((int16_t)(Motors_PWM[0] * 2.5f));
}
