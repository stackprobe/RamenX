#include "Stdinc.h"

template <class Element_t>
class autoQueue
{
private:
	autoList<Element_t> *List;
	int KeepCount;
	int TopIndex;

	void init(autoList<Element_t> *list, int keepCount)
	{
		errorCase(!list);
		errorCase(keepCount < 0 || TSLD < keepCount);

		this->List = list;
		this->KeepCount = keepCount;
		this->TopIndex = 0;
	}

public:
	autoQueue()
	{
		this->init(new autoList<Element_t>(), 16);
	}
	autoQueue(int keepCount)
	{
		this->init(new autoList<Element_t>(), keepCount);
	}
	autoQueue(autoList<Element_t> *list_bind, int keepCount)
	{
		this->init(list_bind, keepCount);
	}
	autoQueue(const autoQueue &source)
	{
		error();
	}
	~autoQueue()
	{
		delete this->List;
	}

	autoList<Element_t> *GetList()
	{
		return this->List;
	}
	void Clear()
	{
		this->List->Clear();
		this->TopIndex = 0;
	}
	int GetCount()
	{
		errorCase(this->List->GetCount() < this->TopIndex);

		return this->List->GetCount() - this->TopIndex;
	}
	Element_t Dequeue()
	{
		Element_t e = this->List->GetElement(this->TopIndex);

		this->TopIndex++;

		if(this->GetCount() == 0)
		{
			this->List->Clear();
			this->TopIndex = 0;
		}
		else if(this->KeepCount < this->TopIndex)
		{
			this->List->RemoveRange(0, this->TopIndex);
			this->TopIndex = 0;
		}
		return e;
	}
	Element_t Dequeue(Element_t eoq)
	{
		if(this->GetCount())
		{
			return this->Dequeue();
		}
		return eoq;
	}
};
