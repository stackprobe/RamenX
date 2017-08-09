enum
{
	SE_PAUSE,
	SE_HIT_PLAYER,
	SE_HIT_ENEMY,
	SE_ATTACK_01,
	SE_ATTACK_02,
	SE_ATTACK_03, // not used?
	SE_ATTACK_HEAVY,
	SE_DEATH_ENEMY,
	SE_BOMB,
	SE_MASTERSPARK_00,
	SE_MASTERSPARK_01,
	SE_MASTERSPARK_02,
	SE_MASTERSPARK_03,
	SE_KAIFUKU,
	SE_ADD_BOMB,
	SE_ADD_ZANKI,
	SE_DEATH_BOSS,
	SE_DEATH_BOSS_0001, // 全ステージ開放
	SE_SEMIDEATH_BOSS,
	SE_FIRE07_GREZE,
	SE_NEWRECORD,
	SE_DROPDOWNSKYHIENEMY,

	SE_MAX, // num of member
};

autoList<int> *S_SELoadedList(void);
autoList<double> *S_SEVolumeList(void);

int SE_Load(int resno, autoList<uchar> *image);
void SE_Unload(int resno, int h);
Resource_t *SE_Res(void);

void ReleaseSoundEffects(void);
int GetSERequestCount(void);

int SEFrame(void);

void SETouch(int resno);
void SEPlay(int resno);
void SEPlayEcho(int resno);
void SEStopEcho(void);
void UpdateSEVolume(void);
