#include "Stdinc.h"

static PictureGroup_t *DefaultGroup;
static PictureGroup_t *CurrGroup;

singleton(PictureGroupList, autoList<PictureGroup_t *>, new autoList<PictureGroup_t *>())

PictureGroup_t *MakePictureGroup(void (*si_filter)(int, int, int), void (*gh_filter)(int))
{
	PictureGroup_t *i = (PictureGroup_t *)memAlloc(sizeof(PictureGroup_t));

	i->Res = NULL;
	i->SIFilter = si_filter; // NULL ok
	i->GHFilter = gh_filter; // NULL ok

	S_PictureGroupList()->AddElement(i);
	return i;
}

PictureGroup_t *GetDefaultPictureGroup(void)
{
	if(!DefaultGroup)
		DefaultGroup = MakePictureGroup(NULL, NULL);

	return DefaultGroup;
}
PictureGroup_t *GetPictureGroup(void)
{
	if(!CurrGroup)
		CurrGroup = GetDefaultPictureGroup();

	return CurrGroup;
}
void SetPictureGroup(PictureGroup_t *i) // i: NULL == Default
{
	CurrGroup = i;
}
