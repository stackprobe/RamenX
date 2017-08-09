extern const int MikoPosTbl[128];

GmEnemy_t MakeMikoWound(double x, double y, GmEnemy_t (*next_maker)(double, double));

GmEnemy_t MakeMiko01(double x, double y);
GmEnemy_t MakeMiko02(double x, double y);
void Miko03Touch(void);
GmEnemy_t MakeMiko03(double x, double y);
GmEnemy_t MakeMiko04(double x, double y);
void Miko05Touch(void);
GmEnemy_t MakeMiko05(double x, double y);
GmEnemy_t MakeMiko06(double x, double y);
GmEnemy_t MakeMiko07(double x, double y);

GmEnemy_t MakeFire02(double x, double y, double x_vec, double y_vec);
GmEnemy_t MakeFire03(double x, double y, double speed_rate);
GmEnemy_t MakeFire04(double x, double y, double x_vec, double y_vec, int color_green);
GmEnemy_t MakeFire05(double x, double y, double x_dest, int rndIndex);
GmEnemy_t MakeFire05Ex(double x, double y, double r, double r_add, int chr_color);
GmEnemy_t MakeFire06(double x, double y, double r, int pr1, int pr2);
GmEnemy_t MakeFire07(double x, double y, double x_add, double y_add);

GmEnemy_t MakeMikoDeath(double x, double y);
