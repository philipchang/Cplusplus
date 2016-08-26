#ifndef RECODE_H
#define RECODE_H

#include "mtltmp/mcRTree.h"

class _PNT_OPER_ : public MTLTMP::mcGeoc<double>
{
public:
	static double GetNearNil() { return 1E-12; }
	static double GetNearMax() { return 1E20; }
};

class CRecode
{
public:
	CRecode()
	{
		buff = NULL;
		size = 0;
		maxs = 0;
	}
	~CRecode()
	{
		if (buff)
			free(buff);
	}


public:
	long AddPoint(double x, double y)
	{
		double pnts[2] = { x, y };
		long   idxs = -1;

		void*  data;
		double dist;
		tree.SearchNear(pnts, &data, &dist);
		if (dist < 1E-12)
		{
			idxs = (double *)data - (double *)buff;
		}
		else
		{
			idxs = size;

			if (size >= maxs)
			{
				maxs = maxs + 1024 * 1024 * 4;
				buff = (double *)realloc(buff, maxs * sizeof(double));
			}

			buff[size++] = x;
			buff[size++] = y;
			buff[size++] = x;
			buff[size++] = y;
			tree.InsertData(buff + idxs);
		}

		return idxs / 4;
	}

protected:
	double* buff;
	long    size;
	long    maxs;

	MTLTMP::RSTree<double, _PNT_OPER_> tree;
};

//CRecode::CRecode()
//{
//	buff = NULL;
//	size = 0;
//	maxs = 0;
//}
//
//CRecode::~CRecode()
//{
//	if (buff)
//		free(buff);
//}
//
//long CRecode::AddPoint(double x, double y)
//{
//	double pnts[2] = { x, y };
//	long   idxs = -1;
//
//	void*  data;
//	double dist;
//	tree.SearchNear(pnts, &data, &dist);
//	if (dist < 1E-12)
//	{
//		idxs = (double *)data - (double *)buff;
//	}
//	else
//	{
//		idxs = size;
//
//		if (size >= maxs)
//		{
//			maxs = maxs + 1024 * 1024 * 4;
//			buff = (double *)realloc(buff, maxs * sizeof(double));
//		}
//
//		buff[size++] = x;
//		buff[size++] = y;
//		buff[size++] = x;
//		buff[size++] = y;
//		tree.InsertData(buff + idxs);
//	}
//
//	return idxs / 4;
//}

#endif