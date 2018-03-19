#ifndef _SORT_INCLUDES_H
#define _SORT_INCLUDES_H

//This is a merge sort. 
//When there is very few element, using bubble sort
//It is simple to implement and stable.
//The reason to avoid the STL sort/stable_sort is because the comparison
//object passing around there is not passed by reference. Thus, the 
//object can not have too much data memeber, in turn, it lost the
//flexibility it suppose to deliver.

//In addition, I would like to use iterative method instead of
//recursive one.

//XW: Merge sort is not in-place sort. 
//change to better algorithm if you want

//this implementation should be faster than STL's stable_sort and slower than sort
//however, if the comparison object has data memebers, it should be faster than both.

//keep the prototype however.

class _xtab_sort_error
{
public:
	int error;
	_xtab_sort_error(int);
};
inline _xtab_sort_error::_xtab_sort_error(int hr): error(hr) {}

#define _BUBBLE_SORT_BOUND 8
#define _STACK_SORT_BOUND 0	//larger than this, we could allocate buffer to do it

//#include "Base/Base/NewOperator.h"
//#include "Base/Base/Buffer.h"

template <class It, class Pred> inline
	void _xtab_sort0(It * _s, It * _e, Pred& _pr);

template <class It, class Pred> inline
	void _xtab_sort1(It * _s, It * _e, Pred& _pr, It * Buf);


template <class It, class Pred> inline
	int _xtab_sort(It * _s, It * _e, Pred& _pr)
{
	try 
	{
		int num = _e - _s;
		if(num < _BUBBLE_SORT_BOUND)
			_xtab_sort0(_s, _e, _pr);
		else
		{
			vector<It> lvBuffer(num);
			It * lBuf = &lvBuffer[0];
			_xtab_sort1(_s, _e, _pr, lBuf);
		}
		
		return S_OK;
	} 
	catch( _xtab_sort_error err )
	{
		return err.error;
	}
}

//junwang, 2012/05/08
template <class It, class Pred> inline
int _xtab_first_n(It * _s, It* _e, Pred& _pr, int n, bool ifInverse)
{
    int num = _e - _s;
    if(n>num)
        n = num;
    
    It temp;
    It* lpIt = NULL;
    if(!ifInverse)
    {
        //sort the first n elements
        lpIt = &(_s[n]);
        _xtab_sort(_s, lpIt, _pr);
        
        for (int i=n; i<num; i++)
        {
            int j = n-1;
            for (; j>=0; j--)
            {
                if(_pr(_s[i],_s[j]))
                {
                    continue;
                }
                break;
            }
            
            if(j < n-1)
            {
                temp = _s[i];
                _s[i] = _s[n-1];
                for(int k=n-1;k>j+1;k--)
                    _s[k] = _s[k-1];
                _s[j+1] = temp;
            }
        }
    }
    else
    {
        //sort last n elements
        lpIt = &(_s[num-n]);
        _xtab_sort(lpIt, _e, _pr);
        
        for(int i=num-n-1;i>=0;i--)
        {
            int j = num - n;
            for(; j<num;j++)
            {
                if(_pr(_s[j],_s[i]))
                {
                    continue;
                }
                break;
            }
            if(j>num-n)
            {
                temp = _s[i];
                _s[i] = _s[num-n];
                for(int k=num-n; k<j-1;k++)
                    _s[k] = _s[k+1];
                _s[j-1] = temp;
            }
        }
    }
    
    return S_OK;
}

template <class It, class Pred> inline
	void _xtab_sort0(It * _s, It * _e, Pred& _pr)
{
	//use bubble sort when the number of elements is small
	It temp;
	int i, j;
	int num = _e - _s;

	for(j = 0; j < num - 1; j ++)
	{
		for(i = 0; i < num - 1 - j; i ++)
		{
			
			//xhan 03/10/30, to guarantee bubble sort is a stable sort, i + 1 must before i
			if(_pr( _s[i + 1], _s[i]))
			{
				//exchange the position
				temp = _s[i];
				_s[i] = _s[i + 1];
				_s[i + 1] = temp;
			}
		}
	}
}

template <class It, class Pred> inline
	void _xtab_sort1(It * _s, It * _e, Pred& _pr, It * Buf)
{
	int num = _e - _s;
	int count; 
	It	*temp, * before, *after;
	int i, j, iMax, jMax;
	int cur, next;
	int k;

	int step_size = 1;
	after = _s;	//the array
	before = Buf;

	//we will merge [cur, next) with [next, min(_e, next + stepsize)]
	while(step_size < num)
	{
		//exchange before and after
		temp = before;	before = after;	after = temp;	
		
		count = 0;
		cur = 0;
		next = step_size;


		while(next < num)
		{

			
			i = cur;
			j = next;

			iMax = next;
			jMax = (num <(next + step_size))
				?num:(next + step_size);

			while(i < iMax && j < jMax)
			{
				//compare, either i move or j move
				//has to be this way to ensure is a stable sort
				if(_pr( before[j], before[i]))
					after[count ++] = before[j ++];
				else
					after[count ++] = before[i ++];


			}//while(i < iMax && j < jMax)

			//copy the rest
			for(k = j; k < jMax; k ++)
				after[count ++] = before[k];

			for(k = i; k < iMax; k ++)
				after[count ++] = before[k];

			cur = next + step_size;
			next = cur + step_size;
		}//while(next < num)

		//need to copy [count, num)
		for( ; count < num; count ++)
			after[count] = before[count];

		step_size *= 2;
	}//while(step_size < num)

	//if after is not _s
	//need to move it to there	
	if(after != _s)
		for(k = 0; k < num; k ++)
			_s[k] = after[k];
}




//This is the sort which will be used as the prime algorithm for N-Key sort
//in XTab. The sort is completed by repeated calling this function. Each time
//a function object, which represents a strict weak ordering is passed in.

//The sort will record its result in the groupMap, groupMap contains groups which
//are comprised of the sequence which are tied. Any group which has more than
//one element will need to be resolved by the next sort.

//at the start, nGroup should be 1, and opGroupMap should be [0, nElement]

//pr,	function object, should have an operator () and function IsEqual
//nElement,	# of element
//pElementMap,	the mapping of the ordering so far
//nGroup,	# of groups left (it would not exceed nElement/2).
//pGroupMap,	the map which contains the start and the end of each group
//opGroupMap,	place to hold the new map

//Return value
//onGroup,	# of groups left

//return true, if there is no grooup left
template<class Pred> inline 
	bool _group_sort(
		int nElement, int * pElementMap, 
		int nGroup, int * pGroupMap,
		Pred & pr,
		int * onGroup, int * opGroupMap)
{
	int i;

	*onGroup = 0; 

	int start, end, cur, next;
	for(i = 0; i < nGroup; i ++)
	{
		start = pGroupMap[2 * i];
		end = pGroupMap[2 * i + 1];

		//need to sort the group [start, end)
		_xtab_sort(pElementMap + start, pElementMap + end, pr);

		//need to put the ones tied into group and record groups with more
		//than one element.
		cur = start;

		while(cur < end - 1)
		{
			next = cur + 1;

			while(next < end && 
				pr.IsEqual(pElementMap[cur], pElementMap[next]))
				next ++;
			
			if(next > cur + 1)
			{
				opGroupMap[2 * (*onGroup)] = cur;
				opGroupMap[2 * (*onGroup) + 1] = next; 
				*onGroup = *onGroup + 1;
			}

			cur = next;
		}

	}

	return (*onGroup == 0);
}

/*
//this is often used,
//try to compare values potentially are null. The first step is to
//take care of the case that one of the value is null. 

//This function will compare the values depending on whether they are nulls.
//If both are valid, you should not use the function
inline bool gCompareInvalid(bool fInvalid1, bool fInvalid2)
{
	_ASSERTE(fInvalid1 || fInvalid2);

	if(fInvalid1)
		return fInvalid2 ? false : gTreatInvalidSmallest;

	return !gTreatInvalidSmallest;
}
*/

/*

DSSXTabSortMetric
{
	//need to separate GetCell into three part, 
	//first part compound the index
	//second part lookup the index
	//third part call the column to get the data.
	
	//we will use the lookup index here.

	DSSXTabSortMetric(	DSSXTabHeader *
						DSSXTabKey * pKey,		
						long Tag,
						long metric);
	//inside depend on header we will figure out for the thing
	//specified in DSSXTabKey what is on this axis and what is on the other axis.


	bool operator()(long iRow1, long iRow2)
	{
		//correctly compound the key, noting if anyone is considered to be
		//a total.

		//use DSSXTabView's lookup
		//could call lookup in this case
		//if the value is null

		//if not compare the value.
		//here need something to be able to compare things cross different type.
		//call get_numeric first, however, it may fail, then need to check type
		//in this case, it would be char or string, has to be the same to compare???
		//how to compare char with string or numeric and string with numeric.

		//need to make sure numeric compare right first. 
		//according to Excel, the numeric value will be first.

		//if the type mismatch, I will just sort according to the type.


	}
						

}
*/

#endif // _SORT_INCLUDES_H
