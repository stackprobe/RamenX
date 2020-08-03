#include "Stdinc.h"

taskGroup_t *CreateTaskGroup(void)
{
	taskGroup_t *tg = (taskGroup_t *)memAlloc(sizeof(taskGroup_t));

	tg->List = new autoList<taskInfo_t>(4096); // HACK
//	tg->List = new autoList<taskInfo_t>();
	return tg;
}
void ReleaseTaskGroup(taskGroup_t *tg)
{
	errorCase(!tg);

	ClearTaskGroup(tg);
	delete tg->List;
	memFree(tg);
}
void ExecuteTaskGroup(taskGroup_t *tg)
{
	errorCase(!tg);

	for(int index = 0; index < tg->List->GetCount(); index++)
	{
		taskInfo_t *ti = tg->List->ElementAt(index);
		void *td = ti->TaskData;

		if(!ti->TaskFunc(td)) // ? タスク終了
		{
			ti->DataReleaseFunc(td);
			tg->List->FastDesertElement(index);
			index--;
		}
	}
}
void ClearTaskGroup(taskGroup_t *tg)
{
	errorCase(!tg);

	while(tg->List->GetCount())
	{
		taskInfo_t ti = tg->List->UnaddElement();

		ti.DataReleaseFunc(ti.TaskData);
	}
}
