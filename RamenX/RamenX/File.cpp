#include "Stdinc.h"

int accessible(char *path)
{
	return !_access(path, 0);
}
int getFileSize(char *file)
{
	FILE *fp = fileOpen(file, "rb");

	if(_fseeki64(fp, 0I64, SEEK_END) != 0)
		error();

	__int64 size = _ftelli64(fp);

	fileClose(fp);
	errorCase(size < 0i64 || (__int64)TSLD < size);
	return (int)size;
}

FILE *fileOpen(char *file, char *mode)
{
	errorCase(!file);
	errorCase(!mode);

	for(int c = 0; ; c++)
	{
		FILE *fp = fopen(file, mode);

		if(fp)
			return fp;

		if(c == 3)
		{
			printfDx("Can not open '%s' as '%s' mode.\n", file, mode);
			error();
		}
		Sleep(5);
	}
	error();
	return NULL; // Dummy
}
void fileClose(FILE *fp)
{
	if(fclose(fp) != 0)
	{
		error();
	}
}
void fileRead(FILE *fp, void *block, int size)
{
	errorCase(!fp);
	errorCase(!block);
	errorCase(size < 0 || TSLD < size);

	if(fread(block, 1, size, fp) != size)
	{
		error();
	}
}
void fileWrite(FILE *fp, void *block, int size)
{
	errorCase(!fp);
	errorCase(!block);
	errorCase(size < 0 || TSLD < size);

	if(fwrite(block, 1, size, fp) != size)
	{
		error();
	}
}
void fileSeek(FILE *fp, int origin, __int64 offset) // origin, offset ‚Ì•À‚Ñ‚Í fseek() ‚Æ‹t
{
	if(_fseeki64(fp, offset, origin) != 0)
	{
		error();
	}
}

autoList<uchar> *readBinary(char *file)
{
	errorCase(!file);

	int size = getFileSize(file);
	uchar *image = (uchar *)memAlloc(size);

	FILE *fp = fileOpen(file, "rb");

	fileRead(fp, image, size);
	fileClose(fp);

	return new autoList<uchar>(image, size);
}
void writeBinary(char *file, autoList<uchar> *block)
{
	errorCase(!file);
	errorCase(!block);

	FILE *fp = fileOpen(file, "wb");

	fileWrite(fp, block->ElementAt(0), block->GetCount());
	fileClose(fp);
}
int readValue(FILE *fp)
{
	uint value = (uint)readChar(fp);

	value |= (uint)readChar(fp) << 8;
	value |= (uint)readChar(fp) << 16;
	value |= (uint)readChar(fp) << 24;

	errorCase(TSLD < value);

	return (int)value;
}

int readChar(FILE *fp)
{
	int chr = fgetc(fp);

	errorCase(chr == EOF && ferror(fp));
	return chr;
}
char *readLine(FILE *fp)
{
	static autoList<char> *buffer;

	if(!buffer)
		buffer = new autoList<char>();

	buffer->Clear();

	for(; ; )
	{
		int chr = readChar(fp);

		if(chr == EOF)
		{
			if(buffer->GetCount() == 0)
			{
				return NULL;
			}
			break;
		}
		if(chr == '\n')
		{
			break;
		}
		buffer->AddElement(chr);
	}
	buffer->AddElement('\0');
	return buffer->CopyList(NULL);
}
autoList<char *> *readLines(char *file)
{
	FILE *fp = fileOpen(file, "rt");
	autoList<char *> *lines = new autoList<char *>();

	for(; ; )
	{
		char *line = readLine(fp);

		if(!line)
		{
			break;
		}
		lines->AddElement(line);
	}
	lines->MemTrim();
	return lines;
}
