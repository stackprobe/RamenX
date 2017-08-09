extern int SW_DelayCount;

void SW_Start2(void);
void SW_Lap2(char *source, int lineno, char *function);

#if 1

#define SW_Start() 1
#define SW_Lap()   1

#else

#define SW_Start() SW_Start2()
#define SW_Lap()   SW_Lap2(__FILE__, __LINE__, __FUNCTION__)

#endif
