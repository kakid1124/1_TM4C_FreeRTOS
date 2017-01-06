#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "GPIO.h"
#include "SPI.h"


// SSI0Clk 			 - PA2 --> CLK
// SSI0Rx	(MISO) - PA4 --> MISO
// SSI0Tx (MOSI) - PA5 --> MOSI
// 								 PE2 --> CSN
//								 PB3 --> CE



void SSI0_Init(void){
	SYSCTL_RCGCSSI_R |= 0x01;  // activate SSI0
	SYSCTL_RCGCGPIO_R |= 0x13; // activate port A, B, E
	while((SYSCTL_PRGPIO_R&0x0013) == 0){};// ready?
		
//	SYSCTL_RCGC1_R |= SYSCTL_RCGC1_SSI0;	// kich hoat SSI0
//	SYSCTL_RCGC2_R |= (SYSCTL_RCGC2_GPIOA+SYSCTL_RCGC2_GPIOB+SYSCTL_RCGC2_GPIOE); // kich hoat cong A, B, E
//	volatile unsigned long delay = SYSCTL_RCGC2_R;	
	
	GPIO_PORTA_AFSEL_R |= 0x3C;           // enable alt funct on PA2,3,4,5
	// configure PA2,3,4,5 as SSI
	GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFF00F0FF)+0x00222200;
	GPIO_PORTA_DEN_R |= 0x3C;             // enable digital I/O on PA2,3,4,5	
	GPIO_PORTA_AMSEL_R = 0;               // disable analog functionality on PA
	
	GPIO_PORTB_DIR_R |= 0x08;			// PB3 la ngo ra (CE)
	GPIO_PORTB_DEN_R |= 0x08;			// 
	GPIO_PORTB_AFSEL_R &= ~0x08;	// 
		
	GPIO_PORTE_DIR_R |= 0x04;		// PE2 la ngo ra (CSN)
	GPIO_PORTE_DEN_R |= 0x04;			//
	GPIO_PORTE_AFSEL_R &= ~0x04;	//	

	SSI0_CR1_R &= ~SSI_CR1_SSE;           // disable SSI
	SSI0_CR1_R &= ~SSI_CR1_MS;            // master mode
	// clock divider for SSIClk 
	SSI0_CPSR_R = (SSI0_CPSR_R&~SSI_CPSR_CPSDVSR_M) + 40;
	SSI0_CR0_R &= ~(SSI_CR0_SCR_M |       // SCR = 0 (2 Mbps data rate)
									SSI_CR0_SPH |         // SPH = 0
									SSI_CR0_SPO);         // SPO = 0
	// FRF = Freescale SPI frame format: FRF = 0x0
	SSI0_CR0_R = (SSI0_CR0_R&~SSI_CR0_FRF_M)+SSI_CR0_FRF_MOTO;
	// DSS = 8-bit data: DSS = 0x7
	SSI0_CR0_R = (SSI0_CR0_R&~SSI_CR0_DSS_M)+SSI_CR0_DSS_8;

	SSI0_CR1_R |= SSI_CR1_SSE;            // enable SSI
	
	unsigned char dummy;
	while(SPI0_GetByte_N(&dummy)){};
}

unsigned char SPI0_Transfer(unsigned char data){
	unsigned char receive;
	while((SSI0_SR_R & SSI_SR_TNF)==0){};// SSI Transmit FIFO Not Full		
	SSI0_DR_R = data;                  		// data out
	while((SSI0_SR_R & SSI_SR_RNE)==0){};// SSI Receive FIFO Not Empty
	receive = SSI0_DR_R;  							// acknowledge response
	return receive;
}

void SPI0_Sendbyte(unsigned char data){
	while((SSI0_SR_R & SSI_SR_TNF)==0){};	// SSI Transmit FIFO Not Full
	SSI0_DR_R = data;                  		// data out
	while(SSI0_SR_R & SSI_SR_BSY){};			// wait until done
}

unsigned char SPI0_Getbyte(void){
	unsigned char receive;
	while((SSI0_SR_R & SSI_SR_RNE)==0){};// SSI Receive FIFO Not Empty
	receive = SSI0_DR_R;                 // get data
	while(SSI0_SR_R & SSI_SR_BSY){};		// wait until done
	return receive;
}

uint32_t SPI0_GetByte_N(unsigned char * data){
	if(SSI0_SR_R & SSI_SR_RNE){	// Receive FIFO Not Empty
		*data = SSI0_DR_R;
		return 1;
	}
	else 
		return 0;
}
