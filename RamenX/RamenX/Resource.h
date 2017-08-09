typedef struct Resource_st
{
	char *ResClusterFile;
	char *ResFileListFile;
	int ResnoMax;
	int (*HandleLoader)(int, autoList<uchar> *);
	void (*HandleUnloader)(int, int);

	int *HandleList; // [ResnoMax], HANDLE_CLOSED == ñ¢ÉçÅ[Éh

	FILE *ResFp;
	autoList<char *> *ResFileList; // [ResnoMax]
}
Resource_t;

Resource_t *CreateResource(char *resClusterFile, char *resFileListFile, int resno_max, int (*hdlLoader)(int, autoList<uchar> *), void (*hdlUnloader)(int, int));

int GetHandle(Resource_t *i, int resno);
int IsHandleLoaded(Resource_t *i, int resno);

void UnloadHandle(Resource_t *i, int resno);
void UnloadAllHandle(Resource_t *i);
