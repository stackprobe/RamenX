#include "Stdinc.h"

// ---- Trigger ----

#define TRG_DEATHCMAX 10

typedef struct Trigger_st
{
	void (*PassedEvent)(double);
}
Trigger_t;

static int TriggerActor(GmEnemy_t *i, Trigger_t *ed)
{
	if(i->X < GmDc.Player.X)
	{
		ed->PassedEvent(i->X);
		GmCommonShootEffect_L(i->X, i->Y, D_W_STAR | DTP);
		return 0;
	}
	return 1;
}
static void TriggerDrawer(GmEnemy_t *i, Trigger_t *ed)
{
	GmBeforeDrawEnemy(i);
	DrawBegin(i->X - GmDc.CameraX, i->Y, Pic(D_W_STAR | DTP));
	DrawZoom(2.0);
	DrawRotate(LFrmCnt * 0.03);
	DrawEnd();
	GmAfterDrawEnemy(i);
}
static int TriggerIsCrashed(GmEnemy_t *i, Trigger_t *ed, double x, double y, double w, double h)
{
	return 0;
}
static void ReleaseTrigger(Trigger_t *i)
{
	memFree(i);
}
GmEnemy_t MakeTrigger(double x, void (*passedEvent)(double xPos))
{
	Trigger_t *ed = nb_(Trigger_t);

	ed->PassedEvent = passedEvent;

	return MakeEnemy(MakeTrigger, x, 50, 0, 0, TriggerActor, TriggerDrawer, TriggerIsCrashed, ReleaseTrigger, ed);
}
