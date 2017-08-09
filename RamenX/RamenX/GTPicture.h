// -- Curtain --

extern double GTActWhiteLevel;
extern int GTCurtainSleep;

autoQueue<double> *S_GTWhiteLevelQueue(void);

void GTDrawCurtain(double whilteLevel);
void GTCurtainFrame(void);
void GTEarlyCurtain(void);

#define GTDrawBlackWall() \
	GTDrawCurtain(-1.0)

#define GTDrawWhiteWall() \
	GTDrawCurtain(1.0)

void GTSetCurtain(double startWhiteLevel, double destWhiteLevel, int frmcnt);
void GTSetCurtain(double whiteLevel, int frmcnt);
void GTSetCurtain(double whiteLevel);

// -- Effect --

extern taskGroup_t *GTEffectList;
extern int GTEffectSleep;

taskGroup_t *GTGetEffectList(void);
void GTEffectFrame(void);
void GTEarlyEffect(void);

// ----

void GTEndFrame(void);
