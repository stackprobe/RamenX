template <class Element_t>
class autoList
{
private:
	int Count;
	int ListSize;
	Element_t *List;

public:
	int ListLock;

	autoList()
	{
		this->Count = 0;
		this->ListSize = 0;
		this->List = (Element_t *)memAlloc(0);
		this->ListLock = 0;
	}
	autoList(int buffer_size)
	{
		errorCase(buffer_size < 0 || TSLD < buffer_size);

		this->Count = 0;
		this->ListSize = buffer_size;
		this->List = (Element_t *)memAlloc(buffer_size * sizeof(Element_t));
		this->ListLock = 0;
	}
	autoList(Element_t *list_bind, int count)
	{
		errorCase(!list_bind);
		errorCase(count < 0 || TSLD < count);

		this->Count = count;
		this->ListSize = count;
		this->List = list_bind;
		this->ListLock = 0;
	}
	autoList(const autoList &source)
	{
		error();
	}
	~autoList()
	{
		memFree(this->List);
	}

	void Clear()
	{
		this->Count = 0;
	}
	int GetCount()
	{
		return this->Count;
	}

	void SetElement(int index, Element_t element)
	{
		errorCase(index < 0 || this->Count <= index);

		this->List[index] = element;
	}
	Element_t GetElement(int index)
	{
		errorCase(index < 0 || this->Count <= index);

		return this->List[index];
	}
	Element_t GetElement_Endless(int index)
	{
		errorCase(!this->Count);

		index %= this->Count;

		if(!index)
			index *= -1;

		m_range(index, 0, this->Count - 1);

		return this->List[index];
	}
	Element_t *ElementAt(int index)
	{
		if(index == 0)
			return this->List;

		errorCase(index < 0 || this->Count <= index);

		return this->List + index;
	}

	void AddElement(Element_t element)
	{
		errorCase(this->ListLock);

		if(this->ListSize <= this->Count)
		{
			errorCase((TSLD / 3) * 2 < this->ListSize * sizeof(Element_t));

			if(this->ListSize < 16)
			{
				this->ListSize += 2;
			}
			else
			{
				this->ListSize += this->ListSize / 2; // *= 1.5
			}
			this->List = (Element_t *)memRealloc(this->List, this->ListSize * sizeof(Element_t));
		}
		this->List[this->Count] = element;
		this->Count++;
	}
	Element_t UnaddElement()
	{
		errorCase(this->Count < 1);

		this->Count--;
		return this->List[this->Count];
	}
	void InsertElement(int index, Element_t element)
	{
		errorCase(index < 0 || this->Count < index);

		this->AddElement(element); // dummy

		for(int i = this->Count - 1; index < i; i--)
		{
			this->List[i] = this->List[i - 1];
		}
		this->List[index] = element;
	}
	Element_t DesertElement(int index)
	{
		errorCase(index < 0 || this->Count <= index);

		Element_t element = this->List[index];

		this->Count--;

		for(int i = index; i < this->Count; i++)
		{
			this->List[i] = this->List[i + 1];
		}
		return element;
	}
	Element_t FastDesertElement(int index)
	{
		errorCase(index < 0 || this->Count <= index);

		Element_t element = this->List[index];

		this->Count--;
		this->List[index] = this->List[this->Count];

		return element;
	}
	void RemoveRange(int start, int count)
	{
		errorCase(start < 0 || this->Count < start);
		errorCase(count < 0 || this->Count - start < count);

		int index;

		for(index = start; index + count < this->Count; index++)
		{
			this->List[index] = this->List[index + count];
		}
		this->Count = index;
	}

	Element_t *CopyList(int *p_count)
	{
		if(p_count)
		{
			*p_count = this->Count;
		}
		return (Element_t *)memClone(this->List, this->Count * sizeof(Element_t));
	}
	void MemTrim()
	{
		this->ListSize = this->Count;
		this->List = (Element_t *)memRealloc(this->List, this->ListSize * sizeof(Element_t));
	}
	Element_t *UnbindList(int *p_count)
	{
		Element_t *list_unbind = this->List;

		if(p_count)
		{
			*p_count = this->Count;
		}
		this->Count = 0;
		this->ListSize = 0;
		this->List = NULL;

		return list_unbind;
	}

	void AddElements(Element_t *list, int count)
	{
		for(int index = 0; index < count; index++)
		{
			this->AddElement(list[index]);
		}
	}
	void AddElements(autoList<Element_t> *list)
	{
		this->AddElements(list->ElementAt(0), list->GetCount());
	}

	void Swap(int index1, int index2)
	{
		errorCase(index1 < 0 || this->Count <= index1);
		errorCase(index2 < 0 || this->Count <= index2);

		Element_t tmp = this->List[index1];

		this->List[index1] = this->List[index2];
		this->List[index2] = tmp;
	}
	void Reverse()
	{
		int i = 0;
		int j = this->Count - 1;

		while(i < j)
		{
			this->Swap(i, j);
			i++;
			j--;
		}
	}
};
