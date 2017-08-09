typedef struct PictureGroup_st
{
	Resource_t *Res;
	void (*SIFilter)(int, int, int);
	void (*GHFilter)(int);
}
PictureGroup_t;

autoList<PictureGroup_t *> *S_PictureGroupList(void);

PictureGroup_t *MakePictureGroup(void (*si_filter)(int, int, int), void (*gh_filter)(int));

PictureGroup_t *GetDefaultPictureGroup(void);
PictureGroup_t *GetPictureGroup(void);
void SetPictureGroup(PictureGroup_t *i);
