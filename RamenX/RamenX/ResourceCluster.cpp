#include "Stdinc.h"

#define UnaddHash(image, size) \
	(size) -= 16;

#define FIRST_PADDING 0xff

static void UnaddPadding(uchar *image, int &size)
{
	while(size)
	{
		size--;
		if(image[size] == FIRST_PADDING)
		{
			break;
		}
	}
}
autoList<uchar> *ReadBinaryByResourceCluster(char *file, int resno)
{
	FILE *fp = fileOpen(file, "rb");

	int resCount = readValue(fp);
	errorCase(resno < 0 || resCount <= resno);

	fileSeek(fp, SEEK_SET, (__int64)((1 + resno) * sizeof(int)));
	int entIndex = readValue(fp);

	fileSeek(fp, SEEK_SET, (__int64)((1 + resCount + entIndex) * sizeof(int)));
	int entAddress = readValue(fp);
	int entEndAddress = readValue(fp);

	int size = entEndAddress - entAddress;
	errorCase(size < 32);
	errorCase(size % 16 != 0);
	uchar *image = (uchar *)memAlloc(size);

	fileSeek(fp, SEEK_SET, (__int64)entAddress);
	fileRead(fp, image, size);
	fileClose(fp);

	DecryptResource(image, size, entAddress);

	UnaddHash(image, size);
	UnaddPadding(image, size);

	return new autoList<uchar>(image, size);
}
