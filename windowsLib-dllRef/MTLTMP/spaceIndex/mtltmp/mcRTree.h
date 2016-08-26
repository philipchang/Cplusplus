#ifndef __MAPTRIXC_RTREE_LIB__
#define __MAPTRIXC_RTREE_LIB__
//	MaptrixC Template Library

//	compile option
//	#define _FOR_DEBUG_

#include "mcBaseDef.h"
#include "mcSort.h"
#include "mcGeoc.h"
#include "mcPackList.h"

#ifdef _FOR_DEBUG_
#include <windows.h>
#endif

namespace MTLTMP
{

template <class _T>
class _DEF_OPER_;
template <class _TYPE_, class _OPER_ = _DEF_OPER_<_TYPE_>, int _SIZE_ = 16>
class RNode;
template <class _TYPE_, class _OPER_ = _DEF_OPER_<_TYPE_>, int _SIZE_ = 16>
class RTree
{
public:
	RTree();
	~RTree();

	// 搜索距离最近
	void SearchNear(_TYPE_ *Pnt, void **Data, _TYPE_ *Dist);
	// 搜索距离最近的多个对象
	void SearchNear(_TYPE_ *Pnt, void **Data, _TYPE_ *Dist, int &Size);
	// 搜索指定范围
	void SearchRect(_TYPE_ *Mbr, PList **Data);

	void MakeRtree(_TYPE_ **Items, int nSize, int nCoor); // Items按照空间顺序进行排序

protected:
	typedef RNode<_TYPE_, _OPER_, _SIZE_> NBASE;

public:
	NBASE *Root;
};

// R*-Tree
template <class _TYPE_, class _OPER_ = _DEF_OPER_<_TYPE_>, int _SIZE_ = 16>
class RSNode;
template <class _TYPE_, class _OPER_ = _DEF_OPER_<_TYPE_>, int _SIZE_ = 16>
class RSTree : public RTree<_TYPE_, _OPER_, _SIZE_>
{
public:
	RSTree();

	void InsertData(_TYPE_ *Src);
	void InsertData(_TYPE_ *Src, int nLevel, BOOL *OverflowTab);

protected:
	typedef RTree <_TYPE_, _OPER_, _SIZE_> TBASE;
	typedef RSNode<_TYPE_, _OPER_, _SIZE_> RNODE;
	typedef typename TBASE::NBASE NBASE;
};

/////////////////////////////////////////////////////////////////////////////
//
// internal objects define
//
/////////////////////////////////////////////////////////////////////////////

template <class _T>
class QNode
{
public:
	// 搜索距离最近或指定范围
	_T    Rect[4];
	_T    Dist;
	void *Data;
};

template <class _T>
class QNear
{
public:
	// 搜索距离最近的多个对象
	_T    Rect[4];	// 输入
	_T    fMax;
	int   nMax;

	_T   *Dist;		// 输出
	void**Data;
	int   nNum;

	int   nPos;		// 内部

public:
	void  Adjust();
	void  Update(_T K, void *V);
	void  Append(_T K, void *V);
	BOOL  Breakn(_T K);
};

template <class _TYPE_, class _OPER_, int _SIZE_>
class RNode
{
public:
	~RNode();

	void SearchNear(QNode<_TYPE_> *Search); // 搜索距离最近
	void SearchNear(QNear<_TYPE_> *Search); // 搜索距离最近的多个对象
	void SearchRect(QNode<_TYPE_> *Search); // 搜索指定范围

	void SelectRect(QNode<_TYPE_> *Search);

#ifdef _FOR_DEBUG_
	void Draw(HDC hDC);
#endif

public:
	class RSort
	{
	public:
		_TYPE_ _K;
		void*  _V;
		bool operator < (const RSort& C) { return _K < C._K; }
	};

public:
	_TYPE_ Mbr[4];
	WORD   Level;
	WORD   Count;
	void  *Entry[_SIZE_];
};

template <class _TYPE_, class _OPER_, int _SIZE_>
class RSNode : public RNode<_TYPE_, _OPER_, _SIZE_>
{
protected:
	typedef RNode <_TYPE_, _OPER_, _SIZE_> NBASE;
	typedef RSTree<_TYPE_, _OPER_, _SIZE_> RTREE;
	typedef RSNode<_TYPE_, _OPER_, _SIZE_> RNODE;
	typedef typename NBASE::RSort RSort;

public:
	RSNode(int nLevel, RTREE *pRtree);

	void InsertData(_TYPE_ *Src, BOOL *OverflowTab);
	void*ChooseSubtree(_TYPE_ *Src);

protected:
	void InsertData(_TYPE_ *Src);
	void InsertData(RSort  *Src, int nSize);
	void AdjustSelf();
	void AdjustTree(RNODE *Node);
	void ReinsertIt(_TYPE_ *Src, BOOL *OverflowTab);
	void SplitRNode(_TYPE_ *Src, BOOL *OverflowTab);

	void MinCoor(int nDim);
	void MaxCoor(int nDim);
	void CopyCoor(_TYPE_ *Dst, _TYPE_ *Src);
	void Invalid(_TYPE_ *Src);
	void*FindLeastEnlarge(_TYPE_ *Src);
	void*FindLeastOverlap(_TYPE_ *Src);

	void UpdateMbr(_TYPE_ *Src);

public:
	_TYPE_ Area;
	RNODE *Upper;
	RTREE *Rtree;
	void  *Frm[4];
};

/////////////////////////////////////////////////////////////////////////////
//
// internal objects define end
//
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//
// RTree
//
/////////////////////////////////////////////////////////////////////////////

template <class _TYPE_, class _OPER_, int _SIZE_>
RTree<_TYPE_, _OPER_, _SIZE_>::RTree()
{
	Root = NULL;
}

template <class _TYPE_, class _OPER_, int _SIZE_>
RTree<_TYPE_, _OPER_, _SIZE_>::~RTree()
{
	if (Root)
		delete Root;
}

template <class _TYPE_, class _OPER_, int _SIZE_>
void RTree<_TYPE_, _OPER_, _SIZE_>::SearchNear(_TYPE_ *Pnt, void **Data, _TYPE_ *Dist)
{
	if (Root)
	{
		QNode<_TYPE_> n;

		n.Data = NULL;
		n.Dist = _OPER_::GetNearMax();
		n.Rect[0] = Pnt[0];
		n.Rect[1] = Pnt[1];
		Root->SearchNear(&n);

		*Data = n.Data;
		*Dist = n.Dist;
	}
	else
	{
		*Data = NULL;
	}
}

template <class _TYPE_, class _OPER_, int _SIZE_>
void RTree<_TYPE_, _OPER_, _SIZE_>::SearchNear(_TYPE_ *Pnt, void **Data, _TYPE_ *Dist, int &Size)
{
	if (Root)
	{
		QNear<_TYPE_> n;

		n.nMax = Size;
		n.nNum = 0;
		n.fMax = _OPER_::GetNearMax();
		n.Data = Data;
		n.Dist = Dist;
		n.Rect[0] = Pnt[0];
		n.Rect[1] = Pnt[1];
		Root->SearchNear(&n);

		Size = n.nNum;
	}
	else
	{
		Size = 0;
	}
}

template <class _TYPE_, class _OPER_, int _SIZE_>
void RTree<_TYPE_, _OPER_, _SIZE_>::SearchRect(_TYPE_ *Mbr, PList **Data)
{
	if (Root)
	{
		QNode<_TYPE_> n;

		n.Data = NULL;
		n.Rect[0] = Mbr[0];
		n.Rect[1] = Mbr[1];
		n.Rect[2] = Mbr[2];
		n.Rect[3] = Mbr[3];
		Root->SearchRect(&n);

		*Data = (PList *)n.Data;
	}
	else
	{
		*Data = NULL;
	}
}

template <class _TYPE_, class _OPER_, int _SIZE_>
void RTree<_TYPE_, _OPER_, _SIZE_>::MakeRtree(_TYPE_ **Items, int nSize, int nCoor)
{
	if (Root)
	{
		delete Root;
		Root = NULL;
	}

	int nLens = ((2 + _SIZE_) * nSize / (_SIZE_ * _SIZE_) + 2);
	NBASE **Nodes = (NBASE **)new BYTE[(nLens < 256 ? 256 : nLens) * sizeof(NBASE*)];
	if (Nodes == NULL)
		return;
	#undef EVALUATE_SIZE

	NBASE**pCurr = (NBASE**)Items;
	NBASE**pLast = (NBASE**)Items + nSize;
	NBASE**pTemp = (NBASE**)Nodes;

	int Level = 0;
	NBASE *pNode = NULL;
	while(pLast - pCurr >= 2)
	{
		BOOL bLast = FALSE;
		while(pCurr < pLast)
		{
			// pLast - pCurr = 剩下处理的对象数据，在同一级，为了防止不同的
			// Node装载的对象，出现非常不均匀的现象，对最后四个可容纳的
			// Node进行判断。如果出现对象不足的情况，选取可容纳所有对象
			// 的最小Node数目，来平均装载这些对象
			if (!bLast)
			{
				nSize = (int)(pLast - pCurr);
				if (nSize < _SIZE_ * 4)
				{
					bLast = TRUE;
					if (nSize < _SIZE_ * 1)
					{
					}
					else if (nSize < _SIZE_ * 2)
					{
						nSize = (nSize + 1) / 2;
					}
					else if (nSize < _SIZE_ * 3)
					{
						nSize = (nSize + 2) / 3;
					}
					else
					{
						nSize = (nSize + 3) / 4;
					}
				}
				else
				{
					nSize = _SIZE_;
				}
			}

			_TYPE_ MinX, MinY, MaxX, MaxY;
			MinX = _OPER_::GetNearMax();
			MinY = _OPER_::GetNearMax();
			MaxX =-_OPER_::GetNearMax();
			MaxY =-_OPER_::GetNearMax();

			void **pItem;
			pNode = new NBASE;
			pItem = pNode->Entry;

			int m;
			for(m = 0; m < nSize && pCurr < pLast; m++)
			{
				_TYPE_ *Coor = (_TYPE_ *)*pCurr++;

				if (MinX > Coor[0])
					MinX = Coor[0];
				if (MinY > Coor[1])
					MinY = Coor[1];

				if (Level == 0 && nCoor == 2)
				{
					if (MaxX < Coor[0])
						MaxX = Coor[0];
					if (MaxY < Coor[1])
						MaxY = Coor[1];
				}
				else
				{
					if (MaxX < Coor[2])
						MaxX = Coor[2];
					if (MaxY < Coor[3])
						MaxY = Coor[3];
				}

				*pItem++ = Coor;
			}

			pNode->Level  = Level;
			pNode->Count  = m;
			pNode->Mbr[0] = MinX;
			pNode->Mbr[1] = MinY;
			pNode->Mbr[2] = MaxX;
			pNode->Mbr[3] = MaxY;

			*pTemp++ = pNode;
		}

		if (Level++)
		{
		//	pCurr = pLast;
			pLast = pTemp;
		}
		else
		{
			pCurr = Nodes;
			pLast = pTemp;
		}
	}
	delete Nodes;
	Root = pNode;
}

/////////////////////////////////////////////////////////////////////////////
//
// RTree end
//
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//
// RSTree
//
/////////////////////////////////////////////////////////////////////////////

template <class _TYPE_, class _OPER_, int _SIZE_>
RSTree<_TYPE_, _OPER_, _SIZE_>::RSTree()
{
	TBASE::Root = (NBASE *)new RNODE(0, this);
}

template <class _TYPE_, class _OPER_, int _SIZE_>
void RSTree<_TYPE_, _OPER_, _SIZE_>::InsertData(_TYPE_ *Src)
{
	BOOL OverflowTab[32] = { 0 };

	RNODE *Best = (RNODE *)TBASE::Root;
	int i;
	for(i = Best->Level; i > 0; i--)
		Best = (RNODE *)Best->ChooseSubtree(Src);

	Best->InsertData(Src, OverflowTab);
}

template <class _TYPE_, class _OPER_, int _SIZE_>
void RSTree<_TYPE_, _OPER_, _SIZE_>::InsertData(_TYPE_ *Src, int nLevel, BOOL *OverflowTab)
{
	RNODE *Best = (RNODE *)TBASE::Root;
	int i;
	for(i = Best->Level; i > nLevel; i--)
		Best = (RNODE *)Best->ChooseSubtree(Src);

	Best->InsertData(Src, OverflowTab);
}

/////////////////////////////////////////////////////////////////////////////
//
// RSTree end
//
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//
// RNode
//
/////////////////////////////////////////////////////////////////////////////

#if _MSC_VER >= 1400
#pragma warning(disable:4311)
#pragma warning(disable:4312)
#endif

template <class _TYPE_, class _OPER_, int _SIZE_>
RNode<_TYPE_, _OPER_, _SIZE_>::~RNode()
{
	if (Level)
	{
		RNode **Item = (RNode **)Entry;
		int i;
		for(i = 0; i < Count; i++)
			delete Item[i];
	}
}

template <class _TYPE_, class _OPER_, int _SIZE_>
void RNode<_TYPE_, _OPER_, _SIZE_>::SearchNear(QNode<_TYPE_> *Search)
{
	RSort List[_SIZE_];
	if (Level == 0)
	{
		RNode **Item = (RNode **)Entry;

		int i;
		for(i = 0; i < Count; i++)
		{
			List[i]._K = _OPER_::Pt2RectDist((_TYPE_ *)Item[i], (_TYPE_ *)Search);
			List[i]._V = Item[i];
		}
		isort(List, List + --i);
	}
	else
	{
		RNode **Item = (RNode **)Entry;

		int i;
		for(i = 0; i < Count; i++)
		{
			List[i]._K = _OPER_::Pt2RectDist((_TYPE_ *)Item[i], (_TYPE_ *)Search);
			List[i]._V = Item[i];
		}
		isort(List, List + --i);
	}

	if (Level == 0)
	{
		int i;
		for(i = 0; i < Count; i++)
		{
			if (Search->Dist > List[i]._K)
			{
				Search->Dist = List[i]._K;
				Search->Data = List[i]._V;

				if (Search->Dist <= _OPER_::GetNearNil())
					return;
			}
			else
				return;
		}
	}
	else
	{
		int i;
		for(i = 0; i < Count; i++)
		{
			if (Search->Dist > List[i]._K)
			{
				((RNode *)List[i]._V)->SearchNear(Search);

				if (Search->Dist <= _OPER_::GetNearNil())
					return;
			}
			else
				return;
		}
	}
}

template <class _TYPE_, class _OPER_, int _SIZE_>
void RNode<_TYPE_, _OPER_, _SIZE_>::SearchNear(QNear<_TYPE_> *Search)
{
	RSort List[_SIZE_];
	{
		RNode **Item = (RNode **)Entry;

		int i;
		for(i = 0; i < Count; i++)
		{
			List[i]._K = _OPER_::Pt2RectDist((_TYPE_ *)Item[i], (_TYPE_ *)Search);
			List[i]._V = Item[i];
		}
		isort(List, List + --i);
	}

	if (Level == 0)
	{
		int i;
		for(i = 0; i < Count; i++)
		{
			if (Search->nNum < Search->nMax)
			{
				Search->Append(List[i]._K, List[i]._V);

				if (++Search->nNum >= Search->nMax)
					Search->Adjust();
			}
			else if(Search->fMax > List[i]._K)
			{
				Search->Update(List[i]._K, List[i]._V);
				Search->Adjust();
			}
			else
				return;
		}
	}
	else
	{
		int i;
		for(i = 0; i < Count; i++)
		{
			if (Search->fMax > List[i]._K)
			{
				((RNode *)List[i]._V)->SearchNear(Search);
				if (Search->Breakn(_OPER_::GetNearNil()))
					return;
			}
			else
				return;
		}
	}
}

template <class _TYPE_, class _OPER_, int _SIZE_>
void RNode<_TYPE_, _OPER_, _SIZE_>::SearchRect(QNode<_TYPE_> *Search)
{
	RNode **Item = (RNode **)Entry;
	if (Level == 0)
	{
		PList *Query = (PList *)Search->Data;

		int nSize; void **pList;
		if (Query)
		{
			nSize = Query->Size;
			pList = Query->Data();
		}
		else
			nSize = PList::PACK_LISTSIZE;

		int i;
		for(i = 0; i < Count; i++)
		{
			if (_OPER_::Intersects((_TYPE_ *)Item[i], (_TYPE_ *)Search))
			{
				if (nSize >= PList::PACK_LISTSIZE)
				{
					if (Query)
						Query->Size = nSize;

					PList *pData = PList::Create();
					pData->Next = Query;

					nSize = 0;
					Query = pData;
					pList = pData->Data();
				}
				pList[nSize++] = Item[i];
			}
		}
		if (Query)
		{
			Query->Size  = nSize;
			Search->Data = Query;
		}
	}
	else
	{
		int i;
		for(i = 0; i < Count; i++)
		{
			if (_OPER_::IsRcInside((_TYPE_ *)Item[i], (_TYPE_ *)Search))
				Item[i]->SelectRect(Search);
			else
			if (_OPER_::Intersects((_TYPE_ *)Item[i], (_TYPE_ *)Search))
				Item[i]->SearchRect(Search);
		}
	}
}

template <class _TYPE_, class _OPER_, int _SIZE_>
void RNode<_TYPE_, _OPER_, _SIZE_>::SelectRect(QNode<_TYPE_> *Search)
{
	RNode **Item = (RNode **)Entry;
	if (Level == 0)
	{
		PList *Query = (PList *)Search->Data;

		int nSize; void **pList;
		if (Query)
		{
			nSize = Query->Size;
			pList = Query->Data();
		}
		else
			nSize = PList::PACK_LISTSIZE;

		int i;
		for(i = 0; i < Count; i++)
		{
			if (nSize >= PList::PACK_LISTSIZE)
			{
				if (Query)
					Query->Size = nSize;

				PList *pData = PList::Create();
				pData->Next = Query;

				nSize = 0;
				Query = pData;
				pList = pData->Data();
			}
			pList[nSize++] = Item[i];
		}
		if (Query)
		{
			Query->Size  = nSize;
			Search->Data = Query;
		}
	}
	else
	{
		int i;
		for(i = 0; i < Count; i++)
		{
			Item[i]->SelectRect(Search);
		}
	}
}

#ifdef _FOR_DEBUG_
template <class _TYPE_, class _OPER_, int _SIZE_>
void RNode<_TYPE_, _OPER_, _SIZE_>::Draw(HDC hDC)
{
	RNode **Item = (RNode **)Entry;
	if (Level != 0)
	{
		int i;
		for(i = 0; i < Count; i++)
			Item[i]->Draw(hDC);
	}

	DWORD Clr[] =
	{
		0x0000FF, 0x00FF00, 0xFF0000,
		0xFFFF00, 0xFF00FF, 0x00FFFF,
		0x000080, 0x008000, 0x800000,
		0x808000, 0x800080, 0x008080
	};

	HPEN hPen = ::CreatePen(PS_SOLID, 1, Clr[Level]); //PS_DASH
	HPEN hOld = (HPEN)::SelectObject(hDC, hPen);
	if (Level == 0)
	{
	#define MoveTo(A, B, C) MoveToEx(A, B, C, NULL)
	//	MoveTo(hDC, ((int)Mbr[0] - 30239145) / 700, ((int)Mbr[1] - 10333268) / 600);
	//	LineTo(hDC, ((int)Mbr[2] - 30239145) / 700, ((int)Mbr[1] - 10333268) / 600);
	//	LineTo(hDC, ((int)Mbr[2] - 30239145) / 700, ((int)Mbr[3] - 10333268) / 600);
	//	LineTo(hDC, ((int)Mbr[0] - 30239145) / 700, ((int)Mbr[3] - 10333268) / 600);
	//	LineTo(hDC, ((int)Mbr[0] - 30239145) / 700, ((int)Mbr[1] - 10333268) / 600);
		MoveTo(hDC, Mbr[0], Mbr[1]);
		LineTo(hDC, Mbr[2], Mbr[1]);
		LineTo(hDC, Mbr[2], Mbr[3]);
		LineTo(hDC, Mbr[0], Mbr[3]);
		LineTo(hDC, Mbr[0], Mbr[1]);
	#undef MoveTo
	}

	::SelectObject(hDC, hOld);
	::DeleteObject(hPen);
}
#endif

#if _MSC_VER >= 1400
#pragma warning(default:4311)
#pragma warning(default:4312)
#endif

/////////////////////////////////////////////////////////////////////////////
//
// RNode end
//
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//
// RSNode
//
/////////////////////////////////////////////////////////////////////////////

template <class _TYPE_, class _OPER_, int _SIZE_>
RSNode<_TYPE_, _OPER_, _SIZE_>::RSNode(int nLevel, RTREE *pRtree)
{
	NBASE::Count = 0;
	NBASE::Level = nLevel;
	Rtree = pRtree;
	Upper = NULL;

	Invalid(NBASE::Mbr);
}

template <class _TYPE_, class _OPER_, int _SIZE_>
void RSNode<_TYPE_, _OPER_, _SIZE_>::InsertData(_TYPE_ *Src, BOOL *OverflowTab)
{
	if (NBASE::Count < _SIZE_)
	{
		// 插入对象
		InsertData(Src);
	}
	else if(!OverflowTab[NBASE::Level])
	{
		// 重新插入
		ReinsertIt(Src, OverflowTab);
	}
	else
	{
		// 节点分裂
		SplitRNode(Src, OverflowTab);
	}
}

template <class _TYPE_, class _OPER_, int _SIZE_>
void *RSNode<_TYPE_, _OPER_, _SIZE_>::ChooseSubtree(_TYPE_ *Src)
{
	RNODE**Item = (RNODE**)NBASE::Entry;
	RNODE *Best = NULL;

	_TYPE_ Area = _OPER_::GetNearMax();
	int i;
	for(i = NBASE::Count; i-- > 0;)
	{
		RNODE *Temp = Item[i];
		if (_OPER_::IsRcInside(Src, (_TYPE_ *)Temp))
		{
			if (Area > Temp->Area)
			{
				Best = Temp;
				Area = Temp->Area;
			}
		}
	}

	if (Best != NULL)
		return Best;

	if (NBASE::Level >= 2)
		return FindLeastEnlarge(Src);
	else
		return FindLeastOverlap(Src);
}

template <class _TYPE_, class _OPER_, int _SIZE_>
void RSNode<_TYPE_, _OPER_, _SIZE_>::InsertData(_TYPE_ *Src)
{
	NBASE::Entry[NBASE::Count++] = (void *)Src;
	if (NBASE::Level)
		((RNODE *)Src)->Upper = this;

	if (!_OPER_::IsRcInside(Src, NBASE::Mbr))
	{
		UpdateMbr(Src);

		Area = _OPER_::GetArea(NBASE::Mbr);
		if (Upper)
			Upper->AdjustTree(this);
	}
}

template <class _TYPE_, class _OPER_, int _SIZE_>
void RSNode<_TYPE_, _OPER_, _SIZE_>::InsertData(RSort *Src, int nSize)
{
	RNODE **Item = (RNODE**)NBASE::Entry;
	NBASE::Count = nSize;

	int i;
	for(i = nSize; i--;)
		Item[i] = (RNODE *)Src[i]._V;

	if (NBASE::Level)
	{
		for(i = nSize; i--;)
			Item[i]->Upper = this;
	}

	AdjustSelf();
}

template <class _TYPE_, class _OPER_, int _SIZE_>
void RSNode<_TYPE_, _OPER_, _SIZE_>::AdjustSelf()
{
	Invalid(NBASE::Mbr);

	_TYPE_ **Item = (_TYPE_**)NBASE::Entry;

	int i;
	for(i = NBASE::Count; i-- > 0;)
		UpdateMbr(Item[i]);

	Area = _OPER_::GetArea(NBASE::Mbr);
	if (Upper)
		Upper->AdjustTree(this);
}

template <class _TYPE_, class _OPER_, int _SIZE_>
void RSNode<_TYPE_, _OPER_, _SIZE_>::AdjustTree(RNODE *Node)
{
	bool bNext = FALSE;

	_TYPE_ *NMbr = Node->Mbr;

	int i = 0;
	for(; i < 2; i++)
	{
		if (NBASE::Mbr[i] > NMbr[i])
		{
			NBASE::Mbr[i] = NMbr[i];
			RNODE::Frm[i] = Node;

			bNext = TRUE;
		}
		else if(NBASE::Mbr[i] < NMbr[i])
		{
			if (RNODE::Frm[i] == Node)
			{
				MinCoor(i);
				bNext = TRUE;
			}
		}
	}
	for(; i < 4; i++)
	{
		if (NBASE::Mbr[i] < NMbr[i])
		{
			NBASE::Mbr[i] = NMbr[i];
			RNODE::Frm[i] = Node;

			bNext = TRUE;
		}
		else if (NBASE::Mbr[i] > NMbr[i])
		{
			if (RNODE::Frm[i] == Node)
			{
				MaxCoor(i);
				bNext = TRUE;
			}
		}
	}

	if (bNext)
	{
		Area = _OPER_::GetArea(NBASE::Mbr);
		if (Upper)
			Upper->AdjustTree(this);
	}
}

template <class _TYPE_, class _OPER_, int _SIZE_>
void RSNode<_TYPE_, _OPER_, _SIZE_>::ReinsertIt(_TYPE_ *Src, BOOL *OverflowTab)
{
#define _FULL_SIZE_ (_SIZE_ + 1)

	// 重新插入
	OverflowTab[NBASE::Level] = TRUE;
	_OPER_::CombineRc(Src, NBASE::Mbr);

	RSort List[_FULL_SIZE_];
	{
		// 可以使用中心点作为原点，也可以使用左下角作为原点
		// Mbr[0] = (Mbr[0] + Mbr[2]) / 2;
		// Mbr[1] = (Mbr[1] + Mbr[3]) / 2;

		RNODE **Item = (RNODE**)NBASE::Entry;

		int i = NBASE::Count;
		List[i]._K = _OPER_::GetDistance(NBASE::Mbr, Src);
		List[i]._V = Src;

		while(i--)
		{
			_TYPE_ *Tmp = (_TYPE_ *)Item[i];
			List[i]._K = _OPER_::GetDistance(NBASE::Mbr, Tmp);
			List[i]._V = Tmp;
		}

		isort(List, List + NBASE::Count);
	}

	int i = (int)(_SIZE_ * 0.9);
	InsertData(List, i);

	while(i <= _SIZE_)
		Rtree->InsertData((_TYPE_ *)List[i++]._V, NBASE::Level, OverflowTab);

#undef _FULL_SIZE_
}

template <class _TYPE_, class _OPER_, int _SIZE_>
void RSNode<_TYPE_, _OPER_, _SIZE_>::SplitRNode(_TYPE_ *Src, BOOL *OverflowTab)
{
#define _FULL_SIZE_ (_SIZE_ + 1)

	// 节点分裂
	RSort CenterX[_FULL_SIZE_];
	RSort CenterY[_FULL_SIZE_];

	{
		// 按照不同的方向排序
		RNODE **Item = (RNODE**)NBASE::Entry;

		int i = NBASE::Count;
		CenterX[i]._K = Src[0];
		CenterY[i]._K = Src[1];
		CenterX[i]._V = Src;
		CenterY[i]._V = Src;

		while(i--)
		{
			_TYPE_ *Tmp = (_TYPE_ *)Item[i];
			CenterX[i]._K = Tmp[0];
			CenterY[i]._K = Tmp[1];
			CenterX[i]._V = Tmp;
			CenterY[i]._V = Tmp;
		}

		isort(CenterX, CenterX + NBASE::Count);
		isort(CenterY, CenterY + NBASE::Count);
	}

	RSort *BestDim = NULL;
	int BestPos = 0;
	{
		// 获取最佳的分裂方向和分裂位置
		_TYPE_ CombPrevA[(_SIZE_ / 4 + 8) * 4];
		_TYPE_ CombBackA[(_SIZE_ / 4 + 8) * 4];
		_TYPE_ CombPrevB[(_SIZE_ / 4 + 8) * 4];
		_TYPE_ CombBackB[(_SIZE_ / 4 + 8) * 4];
		_TYPE_ PrevTemp[4];
		_TYPE_ BackTemp[4];

		_TYPE_ *BestPrev, *BestBack;

		_TYPE_ AreaInt = _OPER_::GetNearMax();
		_TYPE_ AreaSum = _OPER_::GetNearMax();
		_TYPE_ PeriSum = _OPER_::GetNearMax();

		int SplitFactor = (int)(_FULL_SIZE_ * 0.4);
		int SplitLength = _FULL_SIZE_ - SplitFactor * 2;

		// 获取最佳的分裂方向
		int i;
		for(i = 0; i < 2; i++)
		{
			RSort *CenterNode;
			_TYPE_ *CombPrev, *CombBack;
			if (i == 0)
			{
				CenterNode = CenterX;
				CombPrev = CombPrevA;
				CombBack = CombBackA;
			}
			else
			{
				CenterNode = CenterY;
				CombPrev = CombPrevB;
				CombBack = CombBackB;
			}

			int n, m, k;
			Invalid(PrevTemp);
			Invalid(BackTemp);
			for(n = 0, m = NBASE::Count; n < SplitFactor;)
			{
				_OPER_::CombineRc((_TYPE_ *)CenterNode[n++]._V, PrevTemp);
				_OPER_::CombineRc((_TYPE_ *)CenterNode[m--]._V, BackTemp);
			}

			CopyCoor(CombPrev, PrevTemp);
			CopyCoor(CombBack, BackTemp);
			for(k = 0; k++ < SplitLength;)
			{
				_OPER_::CombineRc((_TYPE_ *)CenterNode[n++]._V, PrevTemp);
				_OPER_::CombineRc((_TYPE_ *)CenterNode[m--]._V, BackTemp);

				CopyCoor(CombPrev + ((k) << 2), PrevTemp);
				CopyCoor(CombBack + ((k) << 2), BackTemp);
			}

			_TYPE_ Temp = (_TYPE_)0;
			for(k = 0; k <= SplitLength; k++)
			{
				_TYPE_ *CombA = CombPrev + ((k) << 2);
				_TYPE_ *CombB = CombBack + ((SplitLength - k) << 2);

				Temp += _OPER_::GetPerimeter(CombA) + _OPER_::GetPerimeter(CombB);
			}

			if (PeriSum > Temp)
			{
				PeriSum = Temp;
				BestDim = CenterNode;
				BestPrev = CombPrev;
				BestBack = CombBack;
			}
		}

		// 获取最佳的分裂位置
		{
			int k;
			for(k = 0; k <= SplitLength; k++)
			{
				_TYPE_*CombA = BestPrev + ((k) << 2);
				_TYPE_*CombB = BestBack + ((SplitLength - k) << 2);

				_TYPE_ AreaI = _OPER_::IntersectArea(CombA, CombB);
				_TYPE_ AreaS = _OPER_::GetArea(CombA) + _OPER_::GetArea(CombB);
				if (AreaInt > AreaI)
				{
					AreaInt = AreaI;
					AreaSum = AreaS;

					BestPos = k;
				}
				else if(AreaInt == AreaI)
				{
					if (AreaSum > AreaS) 
					{
						AreaSum = AreaS;

						BestPos = k;
					}
				}
			}
		}

		BestPos += SplitFactor;
	}

	// 开始分裂节点
	InsertData(BestDim, BestPos);

	if (Upper == NULL)
	{
		Upper = new RNODE(NBASE::Level + 1, Rtree);
		Upper->InsertData((_TYPE_ *)this, OverflowTab);

		Rtree->Root = Upper;
	}

	RNODE *NewNode;
	NewNode = new RNODE(NBASE::Level, Rtree);
	NewNode->InsertData(BestDim + BestPos, _FULL_SIZE_ - BestPos);

	Upper->InsertData((_TYPE_ *)NewNode, OverflowTab);

#undef _FULL_SIZE_
}

template <class _TYPE_, class _OPER_, int _SIZE_>
void RSNode<_TYPE_, _OPER_, _SIZE_>::MinCoor(int nDim)
{
	RNODE **Item = (RNODE **)NBASE::Entry;

	int i = NBASE::Count;
	_TYPE_ *Tmp = (_TYPE_ *)Item[--i];

	NBASE::Mbr[nDim] = Tmp[nDim];
	RNODE::Frm[nDim] = Tmp;

	while(i)
	{
		Tmp = (_TYPE_ *)Item[--i];
		if (NBASE::Mbr[nDim] > Tmp[nDim])
		{
			NBASE::Mbr[nDim] = Tmp[nDim];
			RNODE::Frm[nDim] = Tmp;
		}
	}
}

template <class _TYPE_, class _OPER_, int _SIZE_>
void RSNode<_TYPE_, _OPER_, _SIZE_>::MaxCoor(int nDim)
{
	RNODE **Item = (RNODE **)NBASE::Entry;

	int i = NBASE::Count;
	_TYPE_ *Tmp = (_TYPE_ *)Item[--i];

	NBASE::Mbr[nDim] = Tmp[nDim];
	RNODE::Frm[nDim] = Tmp;

	while(i)
	{
		Tmp = (_TYPE_ *)Item[--i];
		if (NBASE::Mbr[nDim] < Tmp[nDim])
		{
			NBASE::Mbr[nDim] = Tmp[nDim];
			RNODE::Frm[nDim] = Tmp;
		}
	}
}

template <class _TYPE_, class _OPER_, int _SIZE_>
inline void RSNode<_TYPE_, _OPER_, _SIZE_>::CopyCoor(_TYPE_ *Dst, _TYPE_ *Src)
{
	Dst[0] = Src[0];
	Dst[1] = Src[1];
	Dst[2] = Src[2];
	Dst[3] = Src[3];
}

template <class _TYPE_, class _OPER_, int _SIZE_>
void RSNode<_TYPE_, _OPER_, _SIZE_>::Invalid(_TYPE_ *Src)
{
	Src[0] = _OPER_::GetNearMax();
	Src[1] = _OPER_::GetNearMax();
	Src[2] =-_OPER_::GetNearMax();
	Src[3] =-_OPER_::GetNearMax();
}

template <class _TYPE_, class _OPER_, int _SIZE_>
void *RSNode<_TYPE_, _OPER_, _SIZE_>::FindLeastEnlarge(_TYPE_ *Src)
{
	RNODE**Item = (RNODE**)NBASE::Entry;
	RNODE *Best = NULL;

	_TYPE_ Incr = _OPER_::GetNearMax();
	_TYPE_ Area = _OPER_::GetNearMax();

	int i;
	for(i = NBASE::Count; i-- > 0;)
	{
		RNODE *Temp = (RNODE *)Item[i];

		_TYPE_ Comb = _OPER_::CombineArea(Src, (_TYPE_ *)Temp);
		_TYPE_ Adds = Comb - Temp->Area;

		if (Incr > Adds)
		{
			Best = Temp;
			Incr = Adds;
			Area = Comb;
		}
		else if(Incr == Adds && Area > Comb)
		{
			Best = Temp;
			Area = Comb;
		}
	}

	return Best;
}

template <class _TYPE_, class _OPER_, int _SIZE_>
void *RSNode<_TYPE_, _OPER_, _SIZE_>::FindLeastOverlap(_TYPE_ *Src)
{
	RNODE**Item = (RNODE**)NBASE::Entry;
	RNODE *Best = NULL;

	_TYPE_ Incr = _OPER_::GetNearMax();
	_TYPE_ Area = _OPER_::GetNearMax();

	int i;
	for(i = NBASE::Count; i-- > 0;)
	{
		RNODE *Temp = (RNODE *)Item[i];

		_TYPE_ CombEntry[4];
		_OPER_::CombineRc(Src, (_TYPE_ *)Temp, CombEntry);

		_TYPE_ Adds = _OPER_::GetArea(CombEntry) - Temp->Area;
		_TYPE_ Over = (_TYPE_)0;
		for(int m = 0; m < NBASE::Count; m++)
		{
			if (m != i)
				Over += _OPER_::IntersectArea(CombEntry, (_TYPE_ *)Item[m]);
		}

		if (Area > Over)
		{
			Best = Temp;
			Area = Over;
			Incr = Adds;
		}
		else if(Area == Over && Incr > Adds)
		{
			Best = Temp;
			Incr = Adds;
		}
	}

	return Best;
}

template <class _TYPE_, class _OPER_, int _SIZE_>
void RSNode<_TYPE_, _OPER_, _SIZE_>::UpdateMbr(_TYPE_ *Src)
{
	if (NBASE::Mbr[0] > Src[0])
	{
		NBASE::Mbr[0] = Src[0];
		RNODE::Frm[0] = Src;
	}
	if (NBASE::Mbr[1] > Src[1])
	{
		NBASE::Mbr[1] = Src[1];
		RNODE::Frm[1] = Src;
	}
	if (NBASE::Mbr[2] < Src[2])
	{
		NBASE::Mbr[2] = Src[2];
		RNODE::Frm[2] = Src;
	}
	if (NBASE::Mbr[3] < Src[3])
	{
		NBASE::Mbr[3] = Src[3];
		RNODE::Frm[3] = Src;
	}
}

/////////////////////////////////////////////////////////////////////////////
//
// RSNode end
//
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//
// _DEF_OPER_
//
/////////////////////////////////////////////////////////////////////////////

template <class _T>
class _DEF_OPER_ : public mcGeoc<_T>
{
public:
	static _T GetNearNil() { return (_T)0; }
	static _T GetNearMax() { return (_T)0x7FFFFFFF; }
};

/////////////////////////////////////////////////////////////////////////////
//
// _DEF_OPER_ end
//
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//
// QNear
//
/////////////////////////////////////////////////////////////////////////////

template <class _T>
void QNear<_T>::Adjust()
{
	int n = nNum;

	fMax = Dist[--n];
	nPos = n;

	while(n--)
	{
		if (fMax < Dist[n])
		{
			fMax = Dist[n];
			nPos = n;
		}
	}
}

template <class _T>
inline void QNear<_T>::Update(_T K, void *V)
{
	Dist[nPos] = K;
	Data[nPos] = V;
}

template <class _T>
inline void QNear<_T>::Append(_T K, void *V)
{
	Dist[nNum] = K;
	Data[nNum] = V;
}

template <class _T>
inline BOOL QNear<_T>::Breakn(_T K)
{
	return fMax <= K && nNum >= nMax;
}

/////////////////////////////////////////////////////////////////////////////
//
// QNear end
//
/////////////////////////////////////////////////////////////////////////////

}

#endif
