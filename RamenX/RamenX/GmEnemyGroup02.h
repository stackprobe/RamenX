GmEnemy_t MakeDamageArea(double x, double y, double w, double h, double r, int lifeCount);

#define MakeDamageArea_Box(x, y, w, h, lifeCount) \
	MakeDamageArea(x, y, w, h, 0.0, lifeCount)
#define MakeDamageArea_Circle(x, y, r, lifeCount) \
	MakeDamageArea(x, y, 0.0, 0.0, r, lifeCount)

GmEnemy_t MakeD3Tama01(double x, double y, double x_add, double y_add, int pic_resno);
GmEnemy_t MakeMawarudonge(double x, double y, double x_add, double y_add, int level);

typedef enum MapItemEffect_et
{
	MIE_KAIFUKU,
	MIE_ADD_BOMB,
	MIE_ADD_ZANKI,
}
MapItemEffect_t;

GmEnemy_t MakeMapItem(double x, double y, MapItemEffect_t effect);
GmEnemy_t MakeProvidence(double x);
