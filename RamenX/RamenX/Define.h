#define _CRT_SECURE_NO_WARNINGS

#include <conio.h>
#include <ctype.h>
#include <direct.h>
#include <dos.h>
#include <fcntl.h>
#include <io.h>
#include <limits.h>
#include <malloc.h>
#include <math.h>
#include <mbstring.h>
#include <process.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys\types.h> // sys\\stat.h より先に include すること。
#include <sys\stat.h>
#include <time.h>

#include <DxLib.h>

typedef unsigned char uchar;
typedef unsigned __int32 uint;
typedef unsigned __int64 uint64;

#define TSLD 2100000000 // ThreShoLD

#define HANDLE_CLOSED -1
#define PI 3.141592653589793
#define NOT_NN -1 // NOT a Natural Number, 非マイナス整数(Indexなど)の無効値として

/*
	ex.
		singleton(IntList, int, (int *)na(int, 100))           <- プロトタイプは int *S_IntList(void);
		static singleton(ClassABC, ClassABC, new ClassABC())   <- static にも出来るよ。
*/
#define singleton(var_name, type_t, init_op) \
	type_t *S_##var_name(void) { \
		static type_t *p; \
		if(!p) { \
			p = (init_op); \
		} \
		return p; \
	}

#define lengthof(list) \
	(sizeof((list)) / sizeof(*(list)))

#define m_min(value1, value2) \
	((value1) < (value2) ? (value1) : (value2))

#define m_max(value1, value2) \
	((value1) < (value2) ? (value2) : (value1))

#define m_range(var, minval, maxval) \
	do { \
	(var) = m_max((var), (minval)); \
	(var) = m_min((var), (maxval)); \
	} while(0)

#define ec_range(value, minval, maxval) \
	errorCase((value) < (minval) || (maxval) < (value))

#define m_compCond(status1, status2) ((status1) ? (status2) : !(status2))
#define m_d2i(value) ((int)((value) + 0.5))
#define m_pow(value) ((value) * (value))

#define m_countDown(var) \
	((var) < 0 ? (var)++ : 0 < (var) ? (var)-- : (var))

#define m_abssub(value1, value2) \
	((value1) < (value2) ? (value2) - (value1) : (value1) - (value2))

/*
	rate: 0.0 -> 1.0 == fast -> slow
*/
#define m_countApprox(var, dest, rate, margin) \
	do { \
	(var) -= (dest); \
	(var) *= (rate); \
	(var) += (dest); \
	m_abssub((var), (dest)) <= (margin) && ((var) = (dest)); \
	} while(0)

// var1 -> var2, var2 -> var1 する。
#define m_swap(var1, var2, vartmp) \
	do { \
	(vartmp) = (var1); \
	(var1) = (var2); \
	(var2) = (vartmp); \
	} while(0)

// var1 -> var2, var2 -> var3, var3 -> var1 する。
#define m_rot3(var1, var2, var3, vartmp) \
	do { \
	(vartmp) = (var1); \
	(var1) = (var2); \
	(var2) = (var3); \
	(var3) = (vartmp); \
	} while(0)

// var1 <= var2 となる様にする。
#define m_sort2(var1, var2, vartmp) \
	do { \
	if((var2) < (var1)) { \
		m_swap((var1), (var2), (vartmp)); \
	} \
	} while(0)

// var1 <= var2 <= var3 となる様にする。
#define m_sort3(var1, var2, var3, vartmp) \
	do { \
	m_sort2((var1), (var2), (vartmp)); \
	m_sort2((var2), (var3), (vartmp)); \
	m_sort2((var1), (var2), (vartmp)); \
	} while(0)

#define m_isNear(x1, y1, x2, y2, r) \
	(m_pow((x1) - (x2)) + m_pow((y1) - (y2)) < m_pow((r)))
