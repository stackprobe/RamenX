int LoadSound(autoList<uchar> *image);
int DuplSound(int h);
void UnloadSound(int h);

void SoundPlay(int h, int endless_mode = 0, int from_top = 1);
void SetVolume(int h, double volume);
void SoundStop(int h);
