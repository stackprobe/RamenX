void noop_v_vp(void *);

template <class Element_t>
autoList<Element_t> *makeList(int count, Element_t initValue)
{
	errorCase(count < 0 || TSLD < count);

	autoList<Element_t> *list = new autoList<Element_t>();

	for(int index = 0; index < count; index++)
	{
		list->AddElement(initValue);
	}
	return list;
}

autoList<double> *makeDRndList(int count, int seed);
autoList<int> *makeRRndList(int count, int seed, int minval, int maxval);
endlessQueue<double> *makeDRndSet(int count, int seed);
endlessQueue<int> *makeRRndSet(int count, int seed, int minval, int maxval);

int atoi_x(char *line);
char *xcout(char *format, ...);
char *strx(char *line);

char *q_readLine(autoQueue<uchar> *i);
void q_writeLine(autoList<uchar> *i, char *line);
void q_writeLine_x(autoList<uchar> *i, char *line);

void trimResourceLines(autoList<char *> *lines);
void relationalPathFilter(autoList<char *> *paths, char *homePath, int homePathIsFile);

int RGB2I(int r, int g, int b, int a = 0xff);
extern int Color_R;
extern int Color_G;
extern int Color_B;
extern int Color_A;
void I2RGB(int color);

int IsCrashed_Box(double x1, double y1, double w1, double h1, double x2, double y2, double w2, double h2);
int IsCrashed_BoxCircle(double x1, double y1, double w1, double h1, double x2, double y2, double r2);
int IsCrashed_Circle(double x1, double y1, double r1, double x2, double y2, double r2);

extern double Vect_X;
extern double Vect_Y;
void P2Vector(double x1, double y1, double x2, double y2, double r);
double P2Angle(double x1, double y1, double x2, double y2);

void PIRanger(double &r, int c_max = 1);

char *c_toString(int value);
