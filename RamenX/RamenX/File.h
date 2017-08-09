int accessible(char *path);
int getFileSize(char *file);

FILE *fileOpen(char *file, char *mode);
void fileClose(FILE *fp);
void fileRead(FILE *fp, void *block, int size);
void fileWrite(FILE *fp, void *block, int size);
void fileSeek(FILE *fp, int origin, __int64 offset);

autoList<uchar> *readBinary(char *file);
void writeBinary(char *file, autoList<uchar> *block);
int readValue(FILE *fp);

int readChar(FILE *fp);
char *readLine(FILE *fp);
autoList<char *> *readLines(char *file);
