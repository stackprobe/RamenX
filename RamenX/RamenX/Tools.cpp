#include "Stdinc.h"

void noop_v_vp(void *vp)
{
	// noop
}

int atoi_x(char *line)
{
	int value = atoi(line);

	memFree(line);
	return value;
}
char *xcout(char *format, ...)
{
	char *buffer;
	va_list marker;

	va_start(marker, format);

	for(int size = strlen(format) + 128; ; size *= 2)
	{
		buffer = (char *)memAlloc(size + 20);
		int retval = _vsnprintf(buffer, size + 10, format, marker);
		buffer[size + 10] = '\0'; // ”O‚Ì‚½‚ß•Â‚¶‚éB

		if(0 <= retval && retval <= size)
		{
			break;
		}
		memFree(buffer);

		errorCase(TSLD / 2 < size);
	}
	va_end(marker);

	return buffer;
}
char *strx(char *line)
{
	return (char *)memClone(line, strlen(line) + 1);
}

char *q_readLine(autoQueue<uchar> *i)
{
	errorCase(!i);

	autoList<char> *buffer = new autoList<char>();

	for(; ; )
	{
		int chr = i->Dequeue('\n');

		if(chr == '\r')
		{
			chr = i->Dequeue();
			errorCase(chr != '\n');
			break;
		}
		if(chr == '\n')
		{
			break;
		}
		buffer->AddElement(chr);
	}
	buffer->AddElement('\0');

	char *line = buffer->UnbindList(NULL);

	delete buffer;
	return line;
}
void q_writeLine(autoList<uchar> *i, char *line)
{
	errorCase(!i);
	errorCase(!line);

	for(char *p = line; *p; p++)
	{
		i->AddElement(*p);
	}
	i->AddElement('\r');
	i->AddElement('\n');
}
void q_writeLine_x(autoList<uchar> *i, char *line)
{
	q_writeLine(i, line);
	memFree(line);
}

void trimResourceLines(autoList<char *> *lines)
{
	for(int index = 0; index < lines->GetCount(); index++)
	{
		char *line = lines->GetElement(index);

		if(line[0] == ';') // ? Comment
		{
			line[0] = '\0';
		}
		if(line[0] == '\0')
		{
			memFree(lines->DesertElement(index--));
		}
	}
}
void relationalPathFilter(autoList<char *> *paths, char *homePath, int homePathIsFile)
{
	if(homePathIsFile)
	{
		char *p = strrchr(homePath, '\\'); // Š¿š‚ğ‘z’è‚µ‚È‚¢B

		errorCase(!p);
		*p = '\0';
	}
	for(int index = 0; index < paths->GetCount(); index++)
	{
		char *path = paths->GetElement(index);

		if(path[0] && path[1] == ':' && path[2] == '\\') // ? ƒtƒ‹ƒpƒX
		{
			// noop
		}
		else // ‘Š‘ÎƒpƒX‚ÆŒ©‚È‚·Bex. "ABC\\DEF.dat"
		{
			path = xcout("%s\\%s", homePath, path);

			memFree(paths->GetElement(index));
			paths->SetElement(index, path);
		}
	}
}

int RGB2I(int r, int g, int b, int a)
{
	m_range(r, 0, 0xff);
	m_range(g, 0, 0xff);
	m_range(b, 0, 0xff);
	m_range(a, 0, 0xff);

	return (int)(
		((uint)r << 24) |
		((uint)g << 16) |
		((uint)b <<  8) |
		((uint)a <<  0)
		);
}
int Color_R;
int Color_G;
int Color_B;
int Color_A;
void I2RGB(int color)
{
	Color_R = (int)(((uint)color >> 24) & 0xff);
	Color_G = (int)(((uint)color >> 16) & 0xff);
	Color_B = (int)(((uint)color >>  8) & 0xff);
	Color_A = (int)(((uint)color >>  0) & 0xff);
}

int IsCrashed_Box(double x1, double y1, double w1, double h1, double x2, double y2, double w2, double h2)
{
	return x2 < x1 + w1 && x1 < x2 + w2 && y2 < y1 + h1 && y1 < y2 + h2;
}
int IsCrashed_BoxCircle(double x1, double y1, double w1, double h1, double x2, double y2, double r2)
{
	if(x2      < x1 && y2      < y1) return m_pow(x1      - x2) + m_pow(y1      - y2) < m_pow(r2);
	if(x2      < x1 && y1 + h1 < y2) return m_pow(x1      - x2) + m_pow(y1 + h1 - y2) < m_pow(r2);
	if(x1 + w1 < x2 && y2      < y1) return m_pow(x1 + w1 - x2) + m_pow(y1      - y2) < m_pow(r2);
	if(x1 + w1 < x2 && y1 + h1 < y2) return m_pow(x1 + w1 - x2) + m_pow(y1 + h1 - y2) < m_pow(r2);

	return
		x1 - r2 < x2 && x2 < x1 + w1 + r2 &&
		y1 - r2 < y2 && y2 < y1 + h1 + r2;
}
int IsCrashed_Circle(double x1, double y1, double r1, double x2, double y2, double r2)
{
	return m_pow(x1 - x2) + m_pow(y1 - y2) < m_pow(r1 + r2);
}

double Vect_X;
double Vect_Y;
/*
	(x1, y1) -> (x2, y2) ‚Ì’·‚³ r ‚ÌƒxƒNƒgƒ‹‚ğ Vect_X, Vect_Y ‚ÉŠi”[‚·‚éB
*/
void P2Vector(double x1, double y1, double x2, double y2, double r)
{
	errorCase(r == 0.0);

	Vect_X = x2 - x1;
	Vect_Y = y2 - y1;

	double rrd = sqrt(m_pow(Vect_X) + m_pow(Vect_Y));

	if(rrd != 0.0)
	{
		r /= rrd;

		Vect_X *= r;
		Vect_Y *= r;
	}
}

/*
	r =~ GetAngle(cos(r), sin(r))
*/
static double GetAngle(double x, double y)
{
	if(y < 0.0) return 2.0 * PI - GetAngle(x, -y);
	if(x < 0.0) return PI - GetAngle(-x, y);

	if(x == 0.0) return PI / 2.0;
	if(y == 0.0) return 0.0;

	double r1 = 0.0;
	double r2 = 0.5 * PI;
	double rm;
	double t = y / x;

	for(int count = 10; ; count--) // 10‰ñ‚Å‚¾‚¢‚½‚¢0.000767ˆÈ‰º‚ÌŒë·
	{
		rm = (r1 + r2) / 2.0;

		if(!count)
			break;

		if(t < tan(rm))
			r2 = rm;
		else
			r1 = rm;
	}
	return rm;
}
/*
	(x1, y1) -> (x1 + 1, y1) ‚©‚ç (x1, y1) -> (x2, y2) ‚ªŒv‰ô‚è‚É‰½“xŒX‚¢‚Ä‚¢‚é‚©•Ô‚·B
	ˆêü == 2 * PI
	Œv‚Ì3‚Ì•ûŒü‚©‚ç‚Å‚ ‚é‚±‚Æ‚É’ˆÓ
*/
double P2Angle(double x1, double y1, double x2, double y2)
{
	return GetAngle(x2 - x1, y2 - y1);
}

autoList<double> *makeDRndList(int count, int seed)
{
	localizeRnd();
	initrnd(seed);

	autoList<double> *list = new autoList<double>();

	for(int index = 0; index < count; index++)
	{
		list->AddElement(drnd());
	}
	unlocalizeRnd();

	return list;
}
autoList<int> *makeRRndList(int count, int seed, int minval, int maxval)
{
	localizeRnd();
	initrnd(seed);

	autoList<int> *list = new autoList<int>();

	for(int index = 0; index < count; index++)
	{
		list->AddElement(rrnd(minval, maxval));
	}
	unlocalizeRnd();

	return list;
}
endlessQueue<double> *makeDRndSet(int count, int seed)
{
	return new endlessQueue<double>(makeDRndList(count, seed));
}
endlessQueue<int> *makeRRndSet(int count, int seed, int minval, int maxval)
{
	return new endlessQueue<int>(makeRRndList(count, seed, minval, maxval));
}

void PIRanger(double &r, int c_max) // r ‚ğ (0.0) - (2.0 * PI) ‚Ì”ÍˆÍ‚É‹¸³‚·‚éB
{
	const double pi2 = 2.0 * PI;

	if(r < 0.0)
	{
		for(int c = 0; c < c_max; c++)
		{
			r += pi2;

			if(0.0 <= r)
				break;
		}
	}
	else if(pi2 < r)
	{
		for(int c = 0; c < c_max; c++)
		{
			r -= pi2;

			if(r <= pi2)
				break;
		}
	}
}

char *c_toString(int value)
{
	const int STR_MAX = 10;
	static char strs[STR_MAX][12];
	static int strndx = 0;
	char *str;

	str = strs[strndx];
	strndx = (strndx + 1) % STR_MAX;

	if(0 <= value && value <= 9)
	{
		const char *zennum = "‚O‚P‚Q‚R‚S‚T‚U‚V‚W‚X";

		str[0] = zennum[value * 2];
		str[1] = zennum[value * 2 + 1];
		str[2] = '\0';
	}
	else
		sprintf(str, "%d", value);

	return str;
}
