
extern "C" {
void SSI0_Init(void);
unsigned char SPI0_Transfer(unsigned char data);
void SPI0_Sendbyte(unsigned char data);
unsigned char SPI0_Getbyte(void);
uint32_t SPI0_GetByte_N(unsigned char * data);
}
