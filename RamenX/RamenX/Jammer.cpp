#include "Stdinc.h"

#define J_COUNT 123

static void MiddleJam(autoList<uchar> *block)
{
	for(int index = 0; index < block->GetCount(); index++)
	{
		block->SetElement(index, block->GetElement(index) ^ 0xff);
	}
}
static void Jammer3(autoList<uchar> *block, int mode)
{
	if(mode)
	{
		for(int index = 1; index < block->GetCount(); index++)
		{
			block->SetElement(index, (block->GetElement(index) + 256 - block->GetElement(index - 1)) % 256);
		}
	}
	else
	{
		for(int index = block->GetCount() - 1; 0 < index; index--)
		{
			block->SetElement(index, (block->GetElement(index) + block->GetElement(index - 1)) % 256);
		}
	}
}
static void Jammer2(autoList<uchar> *block, int mode, int j_num)
{
	for(int count = 0; count < j_num; count++)
	{
		if(1 <= count)
		{
			block->Reverse();
		}
		Jammer3(block, mode);
	}
}
void Jammer(autoList<uchar> *block)
{
	errorCase(block->GetCount() < 1 || TSLD < block->GetCount());

	Jammer2(block, 1, J_COUNT);
	MiddleJam(block);
	Jammer2(block, 0, J_COUNT);
}
