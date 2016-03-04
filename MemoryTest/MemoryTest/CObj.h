#pragma once
#include <time.h> 
#include <vector>
#include <cstdlib>
#include <iostream>
using namespace std;

class COrg1
{
public:
	
	virtual void test(){};

public:
	int  a;
	double c;
	double b;
};

class COrg
{
public:
	virtual void test(){};

public:
	float a;
	int   c;
	double b;
	COrg1  mx;
};



class CTimerCls
{
public:
	CTimerCls(){ 
		m_Start=clock();
	}

	~CTimerCls(){ 
		m_Final=clock();		 
		cout<<(double)(m_Final-m_Start)/CLOCKS_PER_SEC<<" S"<<endl;
	}

private:
	clock_t m_Start, m_Final;
};

class CQueen
{
public:
	CQueen(int  MaxColumn):m_nTotalFunNum(0),m_nMaxColumn( MaxColumn ) {
		if(MaxColumn > 0)
			m_vtResult.resize(MaxColumn);
	}

	/*
	 * @brief: get the queen
	 */
	void Queen(){
		CTimerCls timer;
		RecursionQueen(0);
	}


private:

	/*
	 * @brief: recursion 
	 */
	void RecursionQueen(int nCurLev){
		for(int i = 0; i < m_nMaxColumn; ++i){

			m_vtResult[nCurLev] = i;
			if( bIsValidLev(nCurLev) ){ // cur val is valid, not wrong with pre lev val
				if(nCurLev == (m_nMaxColumn - 1) ) {
					++m_nTotalFunNum;
					
					if(m_nTotalFunNum == 192){
						for(int i = 0; i < m_vtResult.size(); ++i){
							cout<<(i+1)<<" row is "<<( 1 + m_vtResult[i])<<endl;
						}
					}					
				}
				else {
					RecursionQueen(nCurLev + 1);
				}
			}
		}
	}

	/*
	 * @brief: check curLev
	 */
	bool bIsValidLev(int nLev ){
		bool bRet = true;

		for(int i = 0; i < nLev; ++i){
			if( (abs(m_vtResult[i] - m_vtResult[nLev]) == abs(i - nLev)) 
				|| (m_vtResult[i] == m_vtResult[nLev])
				){
					bRet = false;
					break;
			}			
		}
		return bRet;
	}
	
private:
	// one item for a lev, the value is [1,n] for n* n;
	int				m_nTotalFunNum;
	int				m_nMaxColumn;
	vector<int>		m_vtResult; 
};

