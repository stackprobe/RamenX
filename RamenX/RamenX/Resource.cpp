#include "Stdinc.h"

Resource_t *CreateResource(char *resClusterFile, char *resFileListFile, int resno_max, int (*hdlLoader)(int, autoList<uchar> *), void (*hdlUnloader)(int, int))
{
	errorCase(!resClusterFile);
	errorCase(!resFileListFile);
	errorCase(resno_max < 0 || TSLD < resno_max);
	errorCase(!hdlLoader);
	errorCase(!hdlUnloader);

	Resource_t *i = (Resource_t *)memAlloc(sizeof(Resource_t));

	i->ResClusterFile = strx(resClusterFile);
	i->ResFileListFile = strx(resFileListFile);
	i->ResnoMax = resno_max;
	i->HandleLoader = hdlLoader;
	i->HandleUnloader = hdlUnloader;

	i->HandleList = (int *)memAlloc(resno_max * sizeof(int));

	for(int resno = 0; resno < resno_max; resno++)
	{
		i->HandleList[resno] = HANDLE_CLOSED;
	}
	i->ResFp = NULL;
	i->ResFileList = NULL;

	return i;
}

static autoList<uchar> *LoadResourceCluster(Resource_t *i, int resno)
{
	return ReadBinaryByResourceCluster(i->ResClusterFile, resno);
}
static autoList<uchar> *LoadResourceFile(Resource_t *i, int resno)
{
	if(!i->ResFileList)
	{
		i->ResFileList = readLines(i->ResFileListFile);
		trimResourceLines(i->ResFileList);
		relationalPathFilter(i->ResFileList, i->ResFileListFile, 1);

		errorCase(i->ResFileList->GetCount() != i->ResnoMax);
	}
	return readBinary(i->ResFileList->GetElement(resno));
}
static autoList<uchar> *LoadResource(Resource_t *i, int resno)
{
	static autoList<uchar> *(*resourceLoader)(Resource_t *, int);

	if(!resourceLoader)
		resourceLoader = accessible(i->ResClusterFile) ? LoadResourceCluster : LoadResourceFile;

	return resourceLoader(i, resno);
}
int GetHandle(Resource_t *i, int resno)
{
	errorCase(!i);
	errorCase(resno < 0 || i->ResnoMax <= resno);

	if(i->HandleList[resno] == HANDLE_CLOSED)
	{
SW_Lap();
		autoList<uchar> *image = LoadResource(i, resno);

SW_Lap();
		i->HandleList[resno] = i->HandleLoader(resno, image);
SW_Lap();
		delete image;
SW_Lap();
	}
	return i->HandleList[resno];
}
int IsHandleLoaded(Resource_t *i, int resno)
{
	errorCase(!i);
	errorCase(resno < 0 || i->ResnoMax <= resno);

	return i->HandleList[resno] != HANDLE_CLOSED;
}

void UnloadHandle(Resource_t *i, int resno)
{
	errorCase(!i);
	errorCase(resno < 0 || i->ResnoMax <= resno);

	if(i->HandleList[resno] != HANDLE_CLOSED)
	{
		i->HandleUnloader(resno, i->HandleList[resno]);
		i->HandleList[resno] = HANDLE_CLOSED;
	}
}
void UnloadAllHandle(Resource_t *i)
{
	errorCase(!i);

	for(int resno = 0; resno < i->ResnoMax; resno++)
	{
		UnloadHandle(i, resno);
	}
}
