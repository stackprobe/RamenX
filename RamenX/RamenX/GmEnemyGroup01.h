int CheckTamaShoumetsu(GmEnemy_t *i);

GmEnemy_t MakeTama(
	void *shooter_maker,
	double x, double y, double x_add, double y_add, int pic_resno,
	double x_add2 = 0.0, double y_add2 = 0.0,
	double rot_r = 0.0, double rot_r_add = 0.0, double rot_r_add_rate = 1.0,
	double rot_h = 0.0, double rot_h_add = 0.0, double rot_h_add2 = 0.0,
	double rot_h_max = -1.0
	);
GmEnemy_t MakeDoll(double x, double y, int level);
GmEnemy_t MakeAlice(double x, double y, int level);
GmEnemy_t MakeCube(double x, double y, double rx, double ry, double rx_add, double ry_add, double rx_scale, double ry_scale, int pic_resno, double scale, int hp);
GmEnemy_t MakeCube_P(double x, double y);
GmEnemy_t MakeCube_R(double x, double y);
GmEnemy_t MakeCube_G(double x, double y);
GmEnemy_t MakeCube_B(double x, double y);
GmEnemy_t MakeCube_Y(double x, double y);
GmEnemy_t MakeCube_W(double x, double y);
GmEnemy_t MakeCube_X(double x, double y);
GmEnemy_t MakeSuica(double x, int level);
GmEnemy_t MakeBubble(double x, double y, int level);
GmEnemy_t MakeMarisa(double x, int level);
GmEnemy_t MakeAya(double x, double y, int level);
GmEnemy_t MakeUdonge(double x, double y, int level);
