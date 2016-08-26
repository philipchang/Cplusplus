#ifndef __MAPTRIXC_SORT_LIB__
#define __MAPTRIXC_SORT_LIB__
//	MaptrixC Template Library

namespace MTLTMP
{

//	void isort(_T *Min, _T *Max);
//	void qsort(_T *Min, _T *Max);
//	void rsort(_T *src, _T *tmp, int len);

//	void isortptr(_T **Min, _T **Max);
//	void qsortptr(_T **Min, _T **Max);

//	void isort(_T *Sort, _V *Vals, int Min, int Max);
//	void qsort(_T *Sort, _V *Vals, int Min, int Max);

//	void rsort(_T *src, _T *tmp, int len)

template <class _T>
void isort(_T *Min, _T *Max)
{
	_T *m, *n, v;
	for(m = Min; ++m <= Max;)
	{
		v = m[0];
		for(n = m; Min < n && v < n[-1]; n--)
			n[0] = n[-1];
		n[0] = v;
	}
}

template <class _T>
void qsort(_T *Min, _T *Max)
{
	int Len = Max - Min;
	if (Len > 24)
	{
		_T *Mid = Min + Len / 2;
		_T *Tmp = (*Min < *Mid)?
				  (*Mid < *Max ? Mid : (*Min < *Max ? Max : Min)):
				  (*Max < *Mid ? Mid : (*Min < *Max ? Min : Max));

		if (Tmp != Min)
		{
			_T val = Min[0];
			Min[0] = Tmp[0];
			Tmp[0] = val;
		}

		_T v = Min[0];
		_T*m = Min;
		_T*n = Max;

		while(m < n)
		{
			while(v < *n && m < n) n--;
			if (m < n)
			{
				*m = *n;
				while(*++m < v && m < n);

				if (m < n) *n-- = *m;
			}
		}

		*m = v;
		if ((n = m - 1) > Min)
			qsort(Min, n);
		if ((n = m + 1) < Max)
			qsort(n, Max);
	}
	else
	{
		isort(Min, Max);
	}
}

template <class _T>
void isortptr(_T **Min, _T **Max)
{
	_T **m, **n, *v;
	for(m = Min; ++m <= Max;)
	{
		v = m[0];
		for(n = m; Min < n && *v < *n[-1]; n--)
			n[0] = n[-1];
		n[0] = v;
	}
}

template <class _T>
void qsortptr(_T **Min, _T **Max)
{
	int Len = Max - Min;
	if (Len > 24)
	{
		_T **Mid = Min + Len / 2;
		_T **Tmp = (**Min < **Mid)?
				   (**Mid < **Max ? Mid : (**Min < **Max ? Max : Min)) :
				   (**Max < **Mid ? Mid : (**Min < **Max ? Min : Max));

		if (Tmp != Min)
		{
			_T*val = Min[0];
			Min[0] = Tmp[0];
			Tmp[0] = val;
		}

		_T *v = Min[0];
		_T**m = Min;
		_T**n = Max;

		while(m < n)
		{
			while(*v < **n && m < n) n--;
			if (m < n)
			{
				*m = *n;
				while(**++m < *v && m < n);

				if (m < n) *n-- = *m;
			}
		}

		*m = v;
		if ((n = m - 1) > Min)
			qsortptr(Min, n);
		if ((n = m + 1) < Max)
			qsortptr(n, Max);
	}
	else
	{
		isortptr(Min, Max);
	}
}

template <class _T, class _V>
void isort(_T *Sort, _V *Vals, int Min, int Max)
{
	int m, n;
	_T  s;
	_V  v;
	for(m = Min; ++m <= Max;)
	{
		s = Sort[m];
		v = Vals[m];
		for(n = m; Min < n && s < Sort[n - 1]; n--)
		{
			Sort[n] = Sort[n - 1];
			Vals[n] = Vals[n - 1];
		}

		Sort[n] = s;
		Vals[n] = v;
	}
}

template <class _T, class _V>
void qsort(_T *Sort, _V *Vals, int Min, int Max)
{
	if (Max - Min > 24)
	{
		int Mid = (Min + Max) / 2;
		int Tmp = (Sort[Min] < Sort[Mid])?
				  (Sort[Mid] < Sort[Max] ? Mid : (Sort[Min] < Sort[Max] ? Max : Min)) :
				  (Sort[Max] < Sort[Mid] ? Mid : (Sort[Min] < Sort[Max] ? Min : Max));
		if (Tmp != Min)
		{
			_T   sort = Sort[Min];
			_V   vals = Vals[Min];
			Sort[Min] = Sort[Tmp];
			Vals[Min] = Vals[Tmp];
			Sort[Tmp] = sort;
			Vals[Tmp] = vals;
		}

		int m = Min;
		int n = Max;
		_T  s = Sort[Min];
		_V  v = Vals[Min];

		while(m < n)
		{
			while(s < Sort[n] && m < n) n--;
			if (m < n)
			{
				Sort[m] = Sort[n];
				Vals[m] = Vals[n];

				while(Sort[++m] < s && m < n);

				if (m < n)
				{
					Sort[n] = Sort[m];
					Vals[n] = Vals[m];
					n--;
				}
			}
		}

		Sort[m] = s;
		Vals[m] = v;

		if ((n = m - 1) > Min)
			qsort(Sort, Vals, Min, n);
		if ((n = m + 1) < Max)
			qsort(Sort, Vals, n, Max);
	}
	else
	{
		isort(Sort, Vals, Min, Max);
	}
}

template <class _T>
void rsort(_T *src, _T *tmp, int len, int bit)
{
	int idx, sum, *cur;
	_T* ptr;
	int cnt[256] = { 0 };

	ptr = src;
	idx = len;
	do
	{
		cnt[((*ptr++) >> bit) & 0xFF]++;
	}
	while(--idx != 0);

	cur = cnt;
	idx = 256;
	sum = 0;
	do
	{
		int c = *cur;
		*cur++ = sum;
		sum += c;
	}
	while(--idx != 0);

	ptr = src;
	idx = len;
	do
	{
		sum = *ptr++;
		tmp[cnt[(sum >> bit) & 0xFF]++] = sum;
	}
	while(--idx != 0);
}

template <class _T>
void rsort(_T *src, _T *tmp, int len)
{
	rsort(src, tmp, len,  0);
	rsort(tmp, src, len,  8);
	rsort(src, tmp, len, 16);
	rsort(tmp, src, len, 24);
}

}

#endif
