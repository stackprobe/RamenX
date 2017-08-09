#define error() \
	error2(__FILE__, __LINE__, __FUNCTION__)

#define errorCase(status) \
	do { \
	if((status)) error(); \
	} while(0)

void error2(char *source, int lineno, char *function);

#define l_c(message) \
	l_x(strx((message)))

#define l_x(message) \
	l2_x(__FILE__, __LINE__, __FUNCTION__, (message))

void l2_x(char *source, int lineno, char *function, char *message);

extern int callCount_memAlloc;
extern int callCount_memRealloc;
extern int callCount_memFree;

extern int reuseCount_memAlloc;
extern int reuseCount_memRealloc;
extern int reuseCount_memFree;

int ms_GetMemPNum(void);
int ms_GetMemPNumMin(void);

void *memAlloc(int size);
void *memCalloc(int size);
void memFree(void *block);
void *memRealloc(void *block, int size);
void *memClone(void *block, int size);

// 'New Array'
#define na(type_t, count) \
	((type_t *)memCalloc(sizeof(type_t) * (count)))

// 'New Block'
#define nb(type_t) \
	(na(type_t, 1))

extern int fs_count;
extern int fs_max;
extern double fs_rate; // 0.0 - 1.0

#define forscene(frmmax) \
	for(fs_max = (frmmax), fs_count = 0; fs_count <= (frmmax) && (fs_rate = (double)fs_count / fs_max, 1); fs_count++)

#define ifsceneBegin(frmcnt, frmmax) \
	if(fs_count = (frmcnt), 0) {} ifscene((frmmax))

#define ifscene(frmmax) \
	else if(fs_count <= (frmmax) ? (fs_max = (frmmax), fs_rate = (double)fs_count / (frmmax), 1) : (fs_count -= (frmmax) + 1, 0))

#define ifsceneEnd \
	else

int IsWindowActive(void);
