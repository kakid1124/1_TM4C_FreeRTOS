#include <stdint.h>

// period is 16-bit number of PWM clock cycles in one period (3<=period)
// duty is number of PWM clock cycles output is high  (2<=duty<=period-1)

// PWM clock rate = processor clock rate/SYSCTL_RCC_PWMDIV
//                = BusClock/32 
//                = 80 MHz/32 = 2.5 MHz

// PWM Frequency = 400 Hz --> period = 6250 (PWM_clock)
// Pulse Width = 1~2 ms --> duty = 2500~5000
// Resolution: Delta_duty = 5 clock (2us)

// Output on PE4/M1PWM2 --> MOTOR 4
// Output on PF0/M1PWM4 --> MOTOR 1
// Output on PC5/M0PWM7 --> MOTOR 2
// Output on PB4/M0PWM2 --> MOTOR 3

extern float Motors_PWM[4];		// [1000.0, 2000.0] us

extern "C" {
	void Motor_PWM_Init(uint16_t period, uint16_t duty);
	
	void pulseMotors(uint8_t quantity);
	void Update_MotorsPWM(void);
}
