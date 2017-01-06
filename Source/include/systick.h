#ifdef __cplusplus
extern "C" {
#endif

// Initialize SysTick with busy wait running at bus clock.
void SysTick_Init(void);
void SysTick_Init_8MHz(void);

#ifdef __cplusplus
}
#endif
