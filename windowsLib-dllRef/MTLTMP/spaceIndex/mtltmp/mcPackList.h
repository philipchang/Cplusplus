#ifndef __MAPTRIXC_PACKAGE_LIST__
#define __MAPTRIXC_PACKAGE_LIST__

#ifndef malloc
	#include <malloc.h>
#endif

class PList
{
public:
	enum { PACK_LISTSIZE = 1024 };

	static PList *Create()
	{
		PList *List = (PList *)new BYTE[sizeof(PList) + PACK_LISTSIZE * sizeof(void *)];
		if (List != NULL)
		{
			List->Size = 0;
			List->Next = 0;
		}
		return List;
	}

	~PList()
	{
		if (Next)
			delete Next;
	}

	int GetSize()
	{
		int nNum = Size;

		PList *Temp;
		for(Temp = Next; Temp != NULL; Temp = Temp->Next)
			nNum += Temp->Size;

		return nNum;
	}

public:
	PList *Next;
	int    Size;
	void **Data() { return (void **)(this + 1); }
};

#endif
