#include "Stdinc.h"

template <class Element_t>
class endlessQueue
{
private:
	autoList<Element_t> *List;
	int RIndex;

	void init(autoList<Element_t> *list)
	{
		this->List = list;
		this->RIndex = 0;
	}

public:
	endlessQueue()
	{
		this->init(new autoList<Element_t>());
	}
	endlessQueue(autoList<Element_t> *list_bind)
	{
		this->init(list_bind);
	}
	endlessQueue(const endlessQueue &source)
	{
		error();
	}
	~endlessQueue()
	{
		delete this->List;
	}

	autoList<Element_t> *GetList()
	{
		return this->List;
	}
	void Reset()
	{
		this->RIndex = 0;
	}
	Element_t Dequeue()
	{
		errorCase(this->List->GetCount() < 1);

		this->RIndex %= this->List->GetCount();
		return this->List->GetElement(this->RIndex++);
	}
};
