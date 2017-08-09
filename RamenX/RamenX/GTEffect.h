typedef struct GTCommonEffect_st
{
	int ByCamera;
	int PicResno;
	int PicResnoInc;
	int FrmCnt;
	double X;
	double Y;
	double Z;
	double R;
	double A;
	double XAdd;
	double YAdd;
	double ZAdd;
	double RAdd;
	double AAdd;
	double XAdd2;
	double YAdd2;
	double ZAdd2;
	double RAdd2;
	double AAdd2;
	int BlendAddMode;
}
GTCommonEffect_t;

int GTCommonEffect(GTCommonEffect_t *i);

extern int GTACE_BlendAddMode;
extern taskGroup_t *GTACE_ExtraTaskGroup;

GTCommonEffect_t *GTAddCommonEffect(
	int by_camera,
	int pic_resno,
	int pic_resno_inc,
	int frmnum,
	double x, double y,
	double z = 1.0,
	double r = 0.0,
	double a = 1.0,
	double x_add = 0.0,
	double y_add = 0.0,
	double z_add = 0.0,
	double r_add = 0.0,
	double a_add = 0.0,
	double x_add2 = 0.0,
	double y_add2 = 0.0,
	double z_add2 = 0.0,
	double r_add2 = 0.0,
	double a_add2 = 0.0
	);
