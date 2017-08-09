enum
{
	MUSIC_TITLE,
	MUSIC_FIELD_01,
	MUSIC_FIELD_02,
	MUSIC_FIELD_03,
	MUSIC_FIELD_04,
	MUSIC_DUMMY_01,
	MUSIC_DUMMY_02,
	MUSIC_BOSS,
	MUSIC_OLDENDING,
	MUSIC_ENDING,
	MUSIC_GAMEOVER,
	MUSIC_POSTGAMEOVER,
	MUSIC_EXTRA,

	MUSIC_MAX, // num of member
};

autoList<double> *S_MusicVolumeList(void);

int Mus_Load(int resno, autoList<uchar> *image);
void Mus_Unload(int resno, int h);
Resource_t *Mus_Res(void);

void ReleaseMusics(void);
int GetMusicRequestCount(void);

void MusicFrame(void);

void MusicPlay(int resno, int endless_mode = 1, int from_top = 1);
void MusicFadeout(int frmcnt);
void MusicStop(void);
void UpdateMusicVolume(void);
