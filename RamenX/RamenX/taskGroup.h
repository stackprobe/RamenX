typedef struct taskInfo_st
{
	int (*TaskFunc)(void *);
	void *TaskData;
	void (*DataReleaseFunc)(void *);
}
taskInfo_t;

typedef struct taskGroup_st
{
	autoList<taskInfo_t> *List;
}
taskGroup_t;

taskGroup_t *CreateTaskGroup(void);
void ReleaseTaskGroup(taskGroup_t *tg);
void ExecuteTaskGroup(taskGroup_t *tg);
void ClearTaskGroup(taskGroup_t *tg);

template <class TaskData_t>
TaskData_t *AddTask(taskGroup_t *tg, int (*tf)(TaskData_t *), TaskData_t *td = NULL, void (*rf)(TaskData_t *) = NULL)
{
	errorCase(!tg);
	errorCase(!tf);

	if(!rf)
		rf = (void (*)(TaskData_t *))noop_v_vp;

	taskInfo_t ti;

	ti.TaskFunc = (int (*)(void *))tf;
	ti.TaskData = (void *)td;
	ti.DataReleaseFunc = (void (*)(void *))rf;

	tg->List->AddElement(ti);

	return td;
}
