void GmDrawCharacter(double x, double y, int h, int leftmode, double zoom = 1.0);
void GmDrawStand(double x, double y, int leftmode);
void GmDrawBack(double x, double y, int leftmode);
void GmDrawFront(double x, double y, int leftmode);

extern const int GmBackDogezaMax;

void GmDrawBackDogeza(double x, double y, int count, int leftmode, int walkmode);
void GmDrawJump(double x, double y, int count, int leftmode);
void GmDrawJumpBack(double x, double y, int count, int leftmode);
void GmDrawJumpFront(double x, double y, int count, int leftmode);
void GmDrawDash(double x, double y, int count, int leftmode);

extern const int GmDashEndMax;

void GmDrawDashEnd(double x, double y, int count, int leftmode);
void GmDrawAirBack(double x, double y, int count, int leftmode);
void GmDrawAirFront(double x, double y, int count, int leftmode);

extern const int GmAirDashEndMax;

void GmDrawAirDashEnd(double x, double y, int count, int leftmode);

extern const int GmUAttackMax;
extern const int GmCAttackMax;
extern const int GmDAttackMax;

void GmDrawUAttack(double x, double y, int count, int leftmode);
void GmDrawCAttack(double x, double y, int count, int leftmode);
void GmDrawDAttack(double x, double y, int count, int leftmode);

extern const int GmHitMax;

void GmDrawUHit(double x, double y, int count, int leftmode);
void GmDrawDHit(double x, double y, int count, int leftmode);

void GmDrawAirDash(double x, double y, double rot);
