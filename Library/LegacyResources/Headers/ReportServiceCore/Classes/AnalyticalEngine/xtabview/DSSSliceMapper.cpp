
#include "DSSSliceMapper.h"

#include "DSSAEHelper.h"

#define HASHMULT (2)


// Class DSSSliceMapper 


DSSSliceMapper::DSSSliceMapper(int levelNum, int MetricNum, int AttributeNum)
: mLevelHash(NULL), mpLevel(NULL), mpLevel2(NULL), mpLevelMap(NULL), mMetricNum(MetricNum),
mMaxLevelNum(levelNum), mNextLevelIndex(0), mLevelHashDim(levelNum * HASHMULT),
mpTagNum(NULL), mpTagNum2(NULL), mpTagNum3(NULL), mAttributeNum(AttributeNum)
{
    mMapLeveID2LevelIndex.resize(mMaxLevelNum);
    fill(mMapLeveID2LevelIndex.begin(), mMapLeveID2LevelIndex.end(), -1);
}


DSSSliceMapper::~DSSSliceMapper()
{
    ClearBuffer();    
}


int DSSSliceMapper::AddSlice(bool *level,
                             int tag,
                             EnumDssSliceFlag SliceFlag,
                             int metric,
                             int SliceID,
                             int levelID,
                             EnumDSSRuntimeSubtotalType lExtremeType,
                             int iModelIndex,
                             int iTRMetricID,
                             int nXVariable,
                             int *ipXVariables,
                             EnumDSSObjectType *ipXVariableTypes,
                             int *ipXIndexMetrics)
{
//    cout<<"level:";
//    for(int i =0;i < mAttributeNum; i++)
//    {
//        cout<<level[i]<<" ";
//    }
//    cout<<endl;
//    cout<<"tag:"<<tag<<endl;
//    cout<<"sliceflag:";
//    if(SliceFlag == DssOriginalSlice)
//        cout<<"original slice"<<endl;
//    else
//        cout<<"display slice"<<endl;
//    cout<<"metric:"<<metric<<endl;
    
    
    CHECK_PTR(level);
    int hr = S_OK;
	if ( metric < 0 || metric >= mMetricNum )
        return hr = S_FALSE;
    
 	int Index = 0;
 	if ( FindLevelInHash(level, &Index) )
 		if ( AddLevelInHash(level, &Index) )
 			return hr = E_FAIL;	// can neither find nor add the level
    
    // cout<<"Index:"<<Index<<endl;
    mMapLeveID2LevelIndex[levelID] = Index;
    
 	DSSTaggedSlices *Slices = NULL;
	if ( FindTag(Index, tag, SliceFlag, &Slices) ){
 		hr = AddTag(Index, tag, SliceFlag, lExtremeType, iModelIndex, iTRMetricID, nXVariable, ipXVariables, ipXVariableTypes, ipXIndexMetrics, &Slices);	// we can always add a tag (unless we go out of memory, but we handle that elsewere)
		CHECK_HR;
	}
    
	AE_ASSERT(Slices);
 	Slices->mpSliceID[metric]=SliceID;
	
	return hr = S_OK;
}



int DSSSliceMapper::SearchID(bool *level, int tag, EnumDssSliceFlag SliceFlag, int metric, int *SliceID, EnumDSSRuntimeSubtotalType *opExtremeType)
{
    int hr = S_OK;
	CHECK_PTR(level);
	CHECK_PTR(SliceID);
    
	if ( metric < 0 || metric >= mMetricNum )
        return hr = S_FALSE;
    
 	DSSTaggedSlices *Slices = NULL;
 	if ( FindTag(level, tag, SliceFlag, &Slices) )		// find the tag
 		return hr = S_FALSE;
 	
 	*SliceID = Slices->mpSliceID[metric];	// get the slice Id
 	if (*SliceID == -1)
 		return hr = S_FALSE;
    if(opExtremeType)
        *opExtremeType = Slices->mExtremeType;
 	return hr = S_OK;
    
}


int DSSSliceMapper::Count(int levelID, int *count, EnumDssSliceFlag iSliceFlag)
{
    int hr = S_OK;    
	CHECK_PTR(count);
	
    // tehe: level id does not exist
    if(mMapLeveID2LevelIndex[levelID] == -1)
        return E_FAIL;
    
    // 6/19/2012 tehe
    if(iSliceFlag == DssDisplayExtremeSlice)
        *count = mpTagNum3[mMapLeveID2LevelIndex[levelID]];
    else
        *count = mpTagNum[mMapLeveID2LevelIndex[levelID]];
	return hr = S_OK;
}

int DSSSliceMapper::Item(int levelID, int ITag, bool **level, int *tag, EnumDssSliceFlag iSliceFlag, DSSModelInfo **oppModelInfo)
{
    int hr = S_OK;
//	if (Ilevel >= mNextLevelIndex)
//		return hr = E_FAIL;
    //AE_MSG xhan
    
    // 6/19/2012 tehe
    int lLevelIndex = mMapLeveID2LevelIndex[levelID];
    if(lLevelIndex == -1)
        return E_FAIL;
    
	CHECK_PTR(level);
	CHECK_PTR(tag);
    
	DSSTaggedSlices *Slices = iSliceFlag == DssDisplayExtremeSlice ? mpLevel3[lLevelIndex] : mpLevel[lLevelIndex];
    
    
	for (int i=0; i<ITag; i++)	// look for the right tag
	{
		if(Slices == NULL)
			return hr = E_FAIL;
        //AE_MSG xhan
        
		Slices = Slices->mpNext;
	}
    
	CHECK_PTR(Slices);
    
	*tag = Slices->mTag;
	*level = &(mpLevelMap[lLevelIndex*mAttributeNum]);
    
    if(oppModelInfo)
        *oppModelInfo = Slices->mpModelInfo;
    
	return hr = S_OK;
    
    /*	if (Ilevel >= mNextLevelIndex)
     return hr = E_FAIL;
     
     if (level == NULL)
     return E_FAIL;
     
     *level = &(mpLevelMap[Ilevel*mAttributeNum]);
     return S_OK;
     */
}

int DSSSliceMapper::levelHashFunc(bool *level)
{
	int value=0,  i = 0 ;
    
	for (i=0; i<mAttributeNum; i++)
	{
		value <<= 1;
		if (level[i])
			value ^= 1;
	}
    
    
	//Xhan 07/22/01 We must make sure that return value is a positive value
	int lRetVal = value % mLevelHashDim;
	return (lRetVal >= 0) ? lRetVal : (-lRetVal);
    
}

int DSSSliceMapper::FindLevelInHash(bool *level, int *Index)
{
	int Hindex = levelHashFunc(level);
    
	DSSLevelLookup *curLev = mLevelHash[Hindex];
    
	while ( curLev != NULL && !cmpLevels(level, curLev->mlevel) ) // scan the linked list
 		curLev = curLev->next;
    
	if ( curLev == NULL )	// level is not there
 		return 1;
    
	*Index = curLev->index;	// return the index
	return 0;
}

int DSSSliceMapper::AddLevelInHash(bool  *level, int *Index)
{
	if (mNextLevelIndex >= mMaxLevelNum)
		return 1;
	
 	DSSLevelLookup *newLevel = new DSSLevelLookup();
	int Hindex = levelHashFunc(level);
	newLevel->next = mLevelHash[Hindex];
	mLevelHash[Hindex] = newLevel;
	newLevel->index = *Index = mNextLevelIndex;
	mNextLevelIndex++;
    
	for (int i=0; i<mAttributeNum; i++)
		mpLevelMap[(*Index)*mAttributeNum + i] = level[i];
	newLevel->mlevel = &(mpLevelMap[(*Index)*mAttributeNum]);
    
	return 0;
    
}

int DSSSliceMapper::FindTag(bool *level, int tag, EnumDssSliceFlag SliceFlag, DSSTaggedSlices **Slices)
{
   	int Hindex = levelHashFunc(level);
	DSSLevelLookup *curLev = mLevelHash[Hindex];
	DSSTaggedSlices *curSlices;
	
	while ( curLev != NULL ) // scan the linked list
	{
        if (cmpLevels(level, curLev->mlevel))	// if level is the same, try to find the tag.
        {
            
			if(SliceFlag == DssOriginalSlice)
			{
                
				//in the case of 0 tag in the level {index}, since we allocate an array of 0 element
				//mpLevel[Index] is still not null.
                
				if(mpTagNum[curLev->index] > 0)
				{
					curSlices = mpLevel[curLev->index];
					while ( curSlices != NULL && (tag != curSlices -> mTag || SliceFlag != curSlices -> mSliceFlag) )
					 	curSlices = curSlices->mpNext;
					if (curSlices != NULL)
					{
						*Slices = curSlices;
						return 0;
					}
				}
			}
			else if(SliceFlag == DssDisplayIndexSlice)
			{
				if(mpTagNum2[curLev->index] > 0)
				{
					curSlices = mpLevel2[curLev->index];
					while ( curSlices != NULL && (tag != curSlices -> mTag || SliceFlag != curSlices -> mSliceFlag) )
					 	curSlices = curSlices->mpNext;
					if (curSlices != NULL)
					{
						*Slices = curSlices;
						return 0;
					}
				}
			}
            else
            {
                if(mpTagNum3[curLev->index] > 0)
                {
                    curSlices = mpLevel3[curLev->index];
                    while ( curSlices != NULL && (tag != curSlices -> mTag || SliceFlag != curSlices -> mSliceFlag) )
                        curSlices = curSlices->mpNext;
                    if (curSlices != NULL)
                    {
                        *Slices = curSlices;
                        return 0;
                    }
                }
            }
        }
        
        // level not match or tag not match.
  		curLev = curLev->next;
	}
    
	// all bets off if it comes here.
 	return 1;
    
}

int DSSSliceMapper::FindTag(int Index, int tag, EnumDssSliceFlag SliceFlag, DSSTaggedSlices **Slices)
{
   
	DSSTaggedSlices *curSlices;	
	
	if(SliceFlag == DssOriginalSlice)
	{
        
		//in the case of 0 tag in the level {index}, since we allocate an array of 0 element
		//mpLevel[Index] is still not null.
        
		if(mpTagNum[Index] > 0)
			curSlices = mpLevel[Index];
		else
			return 1;
	}
	else if(SliceFlag == DssDisplayIndexSlice)
	{
		if(mpTagNum2[Index] > 0)
			curSlices = mpLevel2[Index];
		else
			return 1;
	}
    else
    {
        if(mpTagNum3[Index] > 0)
            curSlices = mpLevel3[Index];
        else
            return 1;
    }
    
	while ( curSlices != NULL && (tag != curSlices -> mTag || SliceFlag != curSlices -> mSliceFlag) )
	 	curSlices = curSlices->mpNext;
    
    if ( curSlices == NULL )
	 	return 1;
    
    *Slices = curSlices;
    return 0;
   
}

int DSSSliceMapper::AddTag(int Index,
                           int tag,
                           EnumDssSliceFlag SliceFlag,
                           EnumDSSRuntimeSubtotalType ExtremeType,
                           int iModelIndex,
                           int iTRMetricID,
                           int nXVariable,
                           int *ipXVariables,
                           EnumDSSObjectType *ipXVariableTypes,
                           int *ipXIndexMetricSlices,
                           DSSTaggedSlices **Slices)
{
    int hr = S_OK;   
	DSSTaggedSlices *newSlices = new DSSTaggedSlices();
	CHECK_NEWED_PTR(newSlices);
	newSlices->mpSliceID = new int[mMetricNum];
	CHECK_NEWED_PTR(newSlices->mpSliceID);
    
	for (int i=0; i<mMetricNum; i++)
 		newSlices->mpSliceID[i]=-1;
	newSlices->mTag = tag;
	newSlices->mSliceFlag = SliceFlag;
    newSlices->mExtremeType = ExtremeType;
    newSlices->mpModelInfo = NULL;
    
	//add to the right array
	if(SliceFlag == DssOriginalSlice)
	{
		newSlices->mpNext = mpLevel[Index];
		mpLevel[Index] = newSlices;
		mpTagNum[Index]++;	
	}
	else if(SliceFlag == DssDisplayIndexSlice)
	{
		newSlices->mpNext = mpLevel2[Index];
		mpLevel2[Index] = newSlices;
		mpTagNum2[Index]++;	
	}
    else
    {
        newSlices->mpNext = mpLevel3[Index];
        mpLevel3[Index] = newSlices;
        mpTagNum3[Index]++;
        if (iModelIndex > -1)
        {
            DSSModelInfo *lpModelInfo = new DSSModelInfo;
            lpModelInfo->mModelIndex = iModelIndex;
            lpModelInfo->mTRMetricID = iTRMetricID;
            lpModelInfo->mnVariable = nXVariable;
            if (nXVariable > 0)
            {
                lpModelInfo->mpVariables = new Int32[nXVariable];
                memmove(lpModelInfo->mpVariables, ipXVariables, sizeof(Int32) * nXVariable);
                lpModelInfo->mpVariableTypes = new EnumDSSObjectType[nXVariable];
                memmove(lpModelInfo->mpVariableTypes, ipXVariableTypes, sizeof(EnumDSSObjectType) * nXVariable);
                lpModelInfo->mpIndexMetricSlices = new Int32[nXVariable];
                memmove(lpModelInfo->mpIndexMetricSlices, ipXIndexMetricSlices, sizeof(Int32) * nXVariable);
            }
            else
            {
                lpModelInfo->mpVariables = NULL;
                lpModelInfo->mpVariableTypes = NULL;
                lpModelInfo->mpIndexMetricSlices = NULL;
            }
            newSlices->mpModelInfo = lpModelInfo;
        }
    }
    
	//set to the output parameter
	CHECK_PTR(Slices);
	*Slices = newSlices;	
    
	return hr = S_OK;
    
}

int DSSSliceMapper::GetLevel(int levelID, bool **level)
{
    int hr = S_OK;
//	if (Ilevel >= mNextLevelIndex)
//		return hr = E_FAIL;
    //AE_MSG xhan
//    if(Ilevel >= mMaxLevelNum)
//        return E_FAIL;

    // 6/19/2012 tehe
    int levelIndex = mMapLeveID2LevelIndex[levelID];
    if(levelIndex == -1)
        return E_FAIL;
    
	CHECK_PTR(level);
    
	*level = &(mpLevelMap[levelIndex*mAttributeNum]);
	return hr = S_OK;
    
    
}

int DSSSliceMapper::CountLevels(int *count)
{
	*count = mNextLevelIndex;
	return S_OK;
}

//int DSSSliceMapper::CountLevels(int *count)
//{
//    int hr = S_OK;   
//	*count = mNextLevelIndex;
//	return hr = S_OK;
//    
//    
//}


// Additional Declarations

bool DSSSliceMapper::cmpLevels(bool *lvl1, bool *lvl2)
{
	for (int i=0; i<mAttributeNum; i++)
		if (lvl1[i] != lvl2[i])
			return false;
    
	return true;
}

int DSSSliceMapper::Init(void){
 	int hr = S_OK;
    int i = 0;
    
	mpLevel = new DSSTaggedSlices *[mMaxLevelNum];
	CHECK_NEWED_PTR(mpLevel);
	mpLevel2 = new DSSTaggedSlices *[mMaxLevelNum];
	CHECK_NEWED_PTR(mpLevel2);
    mpLevel3 = new DSSTaggedSlices *[mMaxLevelNum];
	CHECK_NEWED_PTR(mpLevel3);
	mpLevelMap = new bool [mMaxLevelNum * mAttributeNum];
	CHECK_NEWED_PTR(mpLevelMap);
	mpTagNum = new int[mMaxLevelNum];
	CHECK_NEWED_PTR(mpTagNum);
	mpTagNum2 = new int[mMaxLevelNum];
	CHECK_NEWED_PTR(mpTagNum2);
    mpTagNum3 = new int[mMaxLevelNum];
	CHECK_NEWED_PTR(mpTagNum3);
	mLevelHash = new DSSLevelLookup *[mLevelHashDim];
	CHECK_NEWED_PTR(mLevelHash);
    
	for (i=0; i<mMaxLevelNum; i++)
	{
		mpLevel[i] = NULL;
		mpLevel2[i] = NULL;
        mpLevel3[i] = NULL;
        
		mpLevelMap[i] = NULL;
		mpTagNum[i] = 0;
		mpTagNum2[i] = 0;
        mpTagNum3[i] = 0;
		for (int j=0; j<mAttributeNum; j++)
			mpLevelMap[i*mAttributeNum + j] = false;
	}
	for (i=0; i<mLevelHashDim; i++)
	{
		mLevelHash[i] = NULL;
	}
    
	return hr = S_OK;
}

// tehe 4/19/2012: memory leak
void DSSSliceMapper::ClearBuffer()
{
    if(mpLevel)
    {
        for (int i=0; i<mMaxLevelNum; i++) {
//            if(!mpLevel[i])
//            {
//                delete mpLevel[i];
//                mpLevel[i] = NULL;
//            }
            DSSTaggedSlices *lpTagSlice = mpLevel[i];
            while (lpTagSlice) {
                DSSTaggedSlices *lpTagSliceNext = lpTagSlice->mpNext;
                delete lpTagSlice;
                lpTagSlice = lpTagSliceNext;
            }
        }
        delete [] mpLevel;
        mpLevel = NULL;
    }
    
    if(mpLevel2)
    {
        for (int i=0; i<mMaxLevelNum; i++) {
//            if(!mpLevel2[i])
//            {
//                delete mpLevel2[i];
//                mpLevel2[i] = NULL;
//            }
            DSSTaggedSlices *lpTagSlice = mpLevel2[i];
            while (lpTagSlice) {
                DSSTaggedSlices *lpTagSliceNext = lpTagSlice->mpNext;
                delete lpTagSlice;
                lpTagSlice = lpTagSliceNext;
            }

            
        }
        delete [] mpLevel2;
        mpLevel2 = NULL;
        
    }
    
    //yimliu, 2014/06/20, Memory leak.
    if(mpLevel3)
    {
        for (int i=0; i<mMaxLevelNum; i++) {
            DSSTaggedSlices *lpTagSlice = mpLevel3[i];
            while (lpTagSlice) {
                DSSTaggedSlices *lpTagSliceNext = lpTagSlice->mpNext;
                delete lpTagSlice;
                lpTagSlice = lpTagSliceNext;
            }
        }
        delete [] mpLevel3;
        mpLevel3 = NULL;
    }
    
    if(mpLevelMap)
    {
        delete [] mpLevelMap;
        mpLevelMap = NULL;
    }
    
    if(mpTagNum)
    {
        delete [] mpTagNum;
        mpTagNum = NULL;
    }
    
    if(mpTagNum2)
    {
        delete [] mpTagNum2;
        mpTagNum2 = NULL;
    }
    
    //yimliu, 2014/06/20, Memory leak.
    if(mpTagNum3)
    {
        delete [] mpTagNum3;
        mpTagNum3 = NULL;
    }
    
    if(mLevelHash)
    {
        for (int i=0; i<mLevelHashDim; i++) {
//            if(!mLevelHash[i])
//            {
//                delete mLevelHash[i];
//                mLevelHash[i] = NULL;
//            }
            DSSLevelLookup *lpLevelLookup = mLevelHash[i];
            while (lpLevelLookup) {
                DSSLevelLookup *lpLevelLookupNext = lpLevelLookup->next;
                delete lpLevelLookup;
                lpLevelLookup = lpLevelLookupNext;
            }
        }
        delete [] mLevelHash;
        mLevelHash = NULL;
        
    }
    mMapLeveID2LevelIndex.clear();
}

//int DSSSliceMapper::getTagsByLevel(bool *iLevel, vector<int> & ovTags)
//{
//    int lIndex = 0;
//    if(FindLevelInHash(iLevel, &lIndex))
//    {
//        return S_FALSE;
//    }
//    else
//    {
//        DSSTaggedSlices *lpSlices = mpLevel[lIndex];
//        while (lpSlices) {
//            ovTags.push_back(lpSlices->mTag);
//            lpSlices = lpSlices->mpNext;
//        }
//        return S_OK;
//    }
//}

int DSSSliceMapper::getSlicesByLevel(bool *iLevel, std::vector<int> &ivMetrics, std::vector<int> &ovSliceID)
{
//    vector<int> lTags;
//    int lRet = getTagsByLevel(iLevel,lTags);
//    if(lRet == S_OK)
//    {
//        for(int i=0;i<ivMetrics.size();i++)
//        {
//            for(int j=0;j<lTags.size();j++)
//            {
//                int lSliceID = -1;
//                lRet = SearchID(iLevel, lTags[j], DssOriginalSlice, ivMetrics[i], &lSliceID);
//                if(lRet == S_OK)
//                {
//                    ovSliceID.push_back(lSliceID);
//                    lRet = SearchID(iLevel, lTags[j], DssDisplayIndexSlice, ivMetrics[i], &lSliceID);
//                    if(lRet == S_OK)
//                        ovSliceID.push_back(lSliceID);
//                }
//            }
//        }
//    }
    // 6/29/2012 tehe
    if(ivMetrics.empty())
        return S_OK;
    
    int levelIndex = -1;
    int hr = FindLevelInHash(iLevel, &levelIndex);
    if(hr != 0)
        return  S_OK;
    
    DSSTaggedSlices *lpTaggedSlices = mpLevel[levelIndex];
    while(lpTaggedSlices != NULL)
    {
        for(int i=0; i< ivMetrics.size(); i++)
        {
            if (ivMetrics[i] >= mMetricNum) continue;
            if(lpTaggedSlices->mpSliceID != NULL && lpTaggedSlices->mpSliceID[ivMetrics[i]] != -1)
            {
                ovSliceID.push_back(lpTaggedSlices->mpSliceID[ivMetrics[i]]);
            }
        }
        lpTaggedSlices = lpTaggedSlices->mpNext;
    }
    
    lpTaggedSlices = mpLevel2[levelIndex];
    while (lpTaggedSlices != NULL) 
    {
        for(int i=0; i< ivMetrics.size(); i++)
        {
            if (ivMetrics[i] >= mMetricNum) continue;
            if(lpTaggedSlices->mpSliceID != NULL && lpTaggedSlices->mpSliceID[ivMetrics[i]] != -1)
            {
                ovSliceID.push_back(lpTaggedSlices->mpSliceID[ivMetrics[i]]);
            }
        }
        lpTaggedSlices = lpTaggedSlices->mpNext;
    }
    return S_OK;
}

// tehe: return S_OK if we add a new level,else return S_FAlSE if the level is alreay exist, one level will corresponding one level index but may corresponding many level ids
int DSSSliceMapper::AddLevel(bool  *level, int *Index, int levelID)
{
    int hr = FindLevelInHash(level, Index);
    if(hr == 0)
    {
        mMapLeveID2LevelIndex[levelID] = *Index;
        
        //974338; increase mNextLevelIndex to make mMapLeveID2LevelIndex consistent with CountLevels()
        if (mNextLevelIndex < mMaxLevelNum)
            mNextLevelIndex++;
        
        return S_FALSE;
    }
            
    hr =  AddLevelInHash(level, Index);
    if(!hr)
    {
        mMapLeveID2LevelIndex[levelID] = *Index;
        return S_OK;
    }
    
    return  E_FAIL; // if can't find or add, return fail
}

int DSSSliceMapper::ReplaceSliceByLevel(bool* level, int iSliceID, int iMetric, int iTag, EnumDssSliceFlag iSliceFlag)
{
    int Index = -1;
    int hr = FindLevelInHash(level, &Index);
    if(hr == 0 && Index >= 0)
    {
        CHECK_PTR(level);
        int hr = S_OK;
        if ( iMetric < 0 || iMetric >= mMetricNum )
            return hr = S_FALSE;
        
        
        DSSTaggedSlices *Slices = NULL;
        if ( FindTag(Index, iTag, iSliceFlag, &Slices) == 0 ){
            AE_ASSERT(Slices);
            Slices->mpSliceID[iMetric]=iSliceID;
        }
    }
    return S_OK;
}
