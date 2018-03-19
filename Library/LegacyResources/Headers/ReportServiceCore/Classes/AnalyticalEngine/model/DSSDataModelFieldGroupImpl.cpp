/*
 *  DataModelFieldGroup.cpp
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 3/20/10.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#import <set>
#import <algorithm>
#import "DSSDataModelFieldGroupImpl.h"
#import "DSSViewMap.h"
#import "DSSDataSource.h"
#import "DSSDataModelTransaction.h"
#import "DSSBinaryDataReader.h"
#import "DSSRWFieldGroupNode.h"
#import "DSSUnitObject.h"
#import "DSSDataElements.h"
#import "DSSDataModelTransactionRWField.h"
#import "CSITransaction/DFCCSITransactionBase.h"
int FieldLink::size()
{
	int lSize = sizeof(EnumDSSRWLinkType) * mtype.size();
	lSize += DSSAEHelper::SizeOfPVector(mtext, sizeof(char));
	return lSize;
}

void ReleaseCString(vector<MBase::CString*> &irvpCString)
{
	vector<MBase::CString*>::iterator end = irvpCString.end();
	vector<MBase::CString*>::iterator it = irvpCString.begin();
	for (; it < end; it++)
	{
		if (*it)
		{
			delete *it;
			*it = NULL;
		}
	}
}

DSSDataModelFieldGroupImpl::DSSDataModelFieldGroupImpl(): mnRows(0), mnFields(0), mbHasLinks(false), mbHasThreshold(false),mpDataSource(NULL),mbbHasStaticFieldLookup(false)
{
}

DSSDataModelFieldGroupImpl::~DSSDataModelFieldGroupImpl()
{
	hCleanUp();
}

void DSSDataModelFieldGroupImpl::hCleanUp()
{
	if (mvText.size() > 0)
	{
		vector<MBase::CString*>::iterator it = mvText.begin();
		for (; it < mvText.end(); it++)
		{
			if (*it)
			{
				delete *it;
				*it = NULL;
			}
		}
		mvText.clear();
	}
	if (mvLinks.size() > 0)
	{
		vector<FieldLink*>::iterator it = mvLinks.begin();
		for(; it < mvLinks.end(); it++)
		{
			if (*it)
			{
				delete *it;
				*it = NULL;
			}
		}
		mvLinks.clear();
	}
	
	//static subfield lookup
	int lnFields = mvStaticSubField.size();
	int i = 0;
	for (i = 0; i < lnFields; i++)
	{
		ReleaseCString(mvStaticSubField[i]);
	}
	mvStaticSubField.clear();
	
	lnFields = mvStaticSubLink.size();
	for (i = 0; i <lnFields; i++)
	{
		int lnLinks = mvStaticSubLink[i].size();
		for (int k = 0; k < lnLinks; k++)
		{
			ReleaseCString(mvStaticSubLink[i][k]);
		}
	}
	mvStaticSubLink.clear();

	mvRawData.clear();
	mvThresholdID.clear();
	mvRowList.clear();
    
    if (mpDataSource)
	{
		delete mpDataSource;
		mpDataSource = NULL;
	}
}
int DSSDataModelFieldGroupImpl::Init()
{
	mType = DssModelFieldGroup;
	return 0;
}
int DSSDataModelFieldGroupImpl::Init(int nRows, int nFields)
{
	mnRows = nRows;
	mnFields = nFields;
	int count = nRows * nFields;
	
	mType = DssModelFieldGroup;
	mvText.resize(count);
	for (int i = 0; i < count; i++)
		mvText[i] = NULL;
	
	mvRawData.resize(count);
	
	return S_OK;
}

int DSSDataModelFieldGroupImpl::AddField(int index, MBase::CString * ipText, int iThresholdID)
{
	if (mvText[index] != NULL)
		return E_FAIL;
	
	mvText[index] = ipText;
	
	if (iThresholdID > 0)
	{
		if (!mbHasThreshold)
		{
			mbHasThreshold = true;
			int count = mnRows * mnFields;
			mvThresholdID.resize(count);
			memset(&mvThresholdID[0], 0, sizeof(int) * count);	//init to 0
		}
		mvThresholdID[index] = iThresholdID;
	}
	return S_OK;
}

int DSSDataModelFieldGroupImpl::AddLink(int index, FieldLink *ipLinnk)
{
	if (!mbHasLinks)
	{
		mbHasLinks = true;
		int count = mnRows * mnFields;
		mvLinks.resize(count);
		memset(&mvLinks[0], 0, sizeof(FieldLink*) * count);		//init to NULL
	}
	mvLinks[index] = ipLinnk;
	return S_OK;
}

int DSSDataModelFieldGroupImpl::AddStaticField(int iField, vector<MBase::CString *> & irvpFieldText)
{
	if (iField < 0 || iField >= mnFields)
		return E_INVALIDINDEX;
		
	if (mvStaticSubField.empty())
	{
		mvStaticSubField.resize(mnFields);
	}
	
	mvStaticSubField[iField].assign(irvpFieldText.begin(), irvpFieldText.end());
	
	return S_OK;
}

int DSSDataModelFieldGroupImpl::AddStaticFieldLink(int iField,  vector<vector<MBase::CString *> > & irvpLinkText)
{
	if (iField < 0 || iField >= mnFields)
		return E_INVALIDINDEX;
	
	if (mvStaticSubLink.empty())
	{
		mvStaticSubLink.resize(mnFields);
	}
	
	mvStaticSubLink[iField].assign(irvpLinkText.begin(), irvpLinkText.end());
	
	return S_OK;
}

int DSSDataModelFieldGroupImpl::AddField(int index, int iField, vector<MBase::CString *> &irvpSubText, int iThresholdID)
{
	if (iField < 0 || iField >= mnFields)
		return E_INVALIDINDEX;
	
	MBase::CString *lpText = new MBase::CString;

	//concatenate subfield text
	int lnSubFields = mvStaticSubField[iField].size();
	int count = 0;
	int f = 0;
	for (f = 0; f < lnSubFields; f++)
	{
		if (mvStaticSubField[iField][f]->empty() && count < irvpSubText.size())		//dynamic sub field
		{
			*lpText += *irvpSubText[count++];
		}
		else	//static sub field
		{
			*lpText += *mvStaticSubField[iField][f];
		}
	}
	
	int hr = AddField(index, lpText, iThresholdID);

	ReleaseCString(irvpSubText);

	return hr;
}

int DSSDataModelFieldGroupImpl::AddLink(int index, int iField, FieldLink *ipLinnk, vector<vector<MBase::CString *> > &irvpSubText)
{
	if (iField < 0 || iField >= mnFields)
		return E_INVALIDINDEX;
	
	int lnLinks = irvpSubText.size();
	int k = 0;
	for (k = 0; k < lnLinks; k++)
	{
		MBase::CString *lpLinkText = new MBase::CString;
		
		//concatenate sub link text
		int lnSubLinks = mvStaticSubLink[iField][k].size();
		int count  =0;
		for (int s = 0; s < lnSubLinks; s++)
		{
			if (mvStaticSubLink[iField][k][s]->empty() && count < irvpSubText[k].size())	//dynamic
			{
				*lpLinkText += *irvpSubText[k][count++];
			}
			else	//static
			{
				*lpLinkText += *mvStaticSubLink[iField][k][s];
			}
		}
		
		ipLinnk->mtext[k] = lpLinkText;
	}
	
	int hr = AddLink(index, ipLinnk);
	
	for (k = 0; k < lnLinks; k++)
	{
		ReleaseCString(irvpSubText[k]);
	}

	return hr;
}


int DSSDataModelFieldGroupImpl::CreateViewMap(vector<int> &irGBPath)
{
	int hr = S_OK;

	int lnGBUnits = mvUnits.size();
	if(lnGBUnits > 0)
	{
		int lnRows = irGBPath.size() / lnGBUnits;
		if (lnRows * mvUnits.size() != irGBPath.size())
		{
			//484762, check if the missing unit is subtotal
			//because of increment loading, the current slice only contains a subtotal element for an unit,
			//mvUnits is missing that unit due a server assumption. It's too late to make server side change
			//so we make some changes here
			vector<int> lvTemp;
			lvTemp.reserve(irGBPath.size());
			for (int i = 0; i < irGBPath.size(); i++)
			{
				//remove all subtotal from group-by path
				if (irGBPath[i] >= 0)
				{
					lvTemp.push_back(irGBPath[i]);
				}
			}
			
			lnRows = lvTemp.size() / (lnGBUnits + 1);
			if (lnRows * mvUnits.size() != lvTemp.size())
			{
				//too many subtotals are removed
				return E_FAIL;
			}
			
			irGBPath.assign(lvTemp.begin(), lvTemp.end());
		}
		
		//TODO:[perf], it may already e sorted. If so, no need to sort and use mvRowList
		vector<int> lvSortedList;
		vector<int> lvDistinctList;
		hSelectDistinct(irGBPath, lnGBUnits, lnGBUnits, lvSortedList, lvDistinctList);

        int lnDistinctRows = lvDistinctList.size() > 0 ? lvDistinctList.size() : lvSortedList.size();
        int* lpDistinctList = NULL;
        if(irGBPath.size() > 0)
            lpDistinctList = lvDistinctList.size() > 0 ? &lvDistinctList[0] : &lvSortedList[0];
        
		vector<int> lvKeys(lnDistinctRows * lnGBUnits);
		int *lpKeys = &lvKeys[0];
		int i = 0;
		for (i = 0; i < lnDistinctRows; i++)
		{
            if(lpDistinctList)
                memmove(lpKeys, &irGBPath[lpDistinctList[i] * (lnGBUnits)], sizeof(int) * lnGBUnits);
			lpKeys += lnGBUnits;
		}
		
		vector<int> lvGBUnits(lnGBUnits);
		for (i = 0; i < lnGBUnits; i++)
			lvGBUnits[i] = i;
		
		hr = hCreateViewMap(lnGBUnits, &lvGBUnits[0], lnDistinctRows, &lvKeys[0]);
		CHECK_HR;
		
		//mvRowList.resize(lnDistinctRows);
		mvRowOffset.clear();
        mvRowList.clear();
        mvRowOffset.push_back(0);
		vector<int> lvRowList;
		i = 0;
		int j = 1;
		while(i < lnRows && j <= lnDistinctRows)
		{
            int next = -1;
            if(lpDistinctList)
                next = j < lnDistinctRows ? lpDistinctList[j] : -1;		//set to -1 to ignore next
			int cur = lvSortedList[i];			//current row offset
			while (cur != next && i < lnRows)
			{
				lvRowList.push_back(cur);
				i++;
				if (i >= lnRows)
					break;
				cur = lvSortedList[i];
			}
			
			std::sort(lvRowList.begin(), lvRowList.end());		//keep the elements order (by its original row offset)
			
			//mvRowList[j - 1].reserve(lvRowList.size());
			vector<int>::iterator it = lvRowList.begin();
			for (; it != lvRowList.end(); it++)
			{
				//mvRowList[j - 1].push_back(*it);
                mvRowList.push_back(*it);
			}
			mvRowOffset.push_back(mvRowList.size());
			lvRowList.clear();
			j++;
		}
	}
	else if (lnGBUnits == 0)
	{
		hr = hCreateViewMap(lnGBUnits, NULL, 1, NULL);
		CHECK_HR;
        mvRowList.clear();
        mvRowOffset.clear();
		//vector<int> lvRowList(mnRows);
		for (int i = 0; i < mnRows; i++)
		{
		//	lvRowList[i] = i;
            mvRowList.push_back(i);
		}
        mvRowOffset.push_back(0);
        mvRowOffset.push_back(mnRows);
		//mvRowList.push_back(lvRowList);
	}
	else
		return E_FAIL;
	
	return S_OK;
}

//new create view map method when the group by paths have been sorted
int DSSDataModelFieldGroupImpl::CreateViewMap2(vector<int> &irGBPath)
{
	int hr = S_OK;
    
	int lnGBUnits = mvUnits.size();
	if(lnGBUnits > 0)
	{
		int lnRows = irGBPath.size() / lnGBUnits;
		if (lnRows * mvUnits.size() != irGBPath.size())
		{
			//484762, check if the missing unit is subtotal
			//because of increment loading, the current slice only contains a subtotal element for an unit,
			//mvUnits is missing that unit due a server assumption. It's too late to make server side change
			//so we make some changes here
			vector<int> lvTemp;
			lvTemp.reserve(irGBPath.size());
			for (int i = 0; i < irGBPath.size(); i++)
			{
				//remove all subtotal from group-by path
				if (irGBPath[i] >= 0)
				{
					lvTemp.push_back(irGBPath[i]);
				}
			}
			
			lnRows = lvTemp.size() / (lnGBUnits + 1);
			if (lnRows * mvUnits.size() != lvTemp.size())
			{
				//too many subtotals are removed
				return E_FAIL;
			}
			
			irGBPath.assign(lvTemp.begin(), lvTemp.end());
		}
		
		//TODO:[perf], it may already e sorted. If so, no need to sort and use mvRowList
		vector<int> lvSortedList;
		vector<int> lvDistinctList;
		hr = hSelectDistinctFromSortedKeys(irGBPath, lnGBUnits, lvSortedList, lvDistinctList);
        CHECK_HR;
		int lnDistinctRows = lvDistinctList.size() > 0 ? lvDistinctList.size() : lvSortedList.size();
        int* lpDistinctList = NULL;
        if(irGBPath.size() > 0)
            lpDistinctList = lvDistinctList.size() > 0 ? &lvDistinctList[0] : &lvSortedList[0];
        
        int* lpKeys = new int [lnDistinctRows * lnGBUnits];
        int* lpKeys2 = lpKeys;
		int i = 0;
  		for (i = 0; i < lnDistinctRows; i++)
		{
            if(lpDistinctList)
                memmove(lpKeys2, &irGBPath[lpDistinctList[i] * lnGBUnits], sizeof(int) * lnGBUnits);
			lpKeys2 += lnGBUnits;
		}
		
		vector<int> lvGBUnits(lnGBUnits);
		for (i = 0; i < lnGBUnits; i++)
			lvGBUnits[i] = i;
		
        
        char* pGBPage = NULL;
        int* pGBPaths = NULL;
        int GBPageSize = 0;
        int GBKeyStart = 0;
        int GBPagePosInFile = 0;
#ifndef __ANDROID__    
        
        int lDataSize = lnDistinctRows * lnGBUnits * sizeof(int);
        if (lDataSize >= MEM_GOVERNOR_SIZE)
        {
            //TODO: CHECK SECURITY MODE
            if(DataSourceUtil::isMappedFileSupported())
            {
                mFilePathMMP = mDataSourcePath + mNodeKey.c_str() + "mmp";
                bool ibUseNewFile = true;
                const void *lpData = lpKeys;
                hr = DataSourceUtil::createMappedFileBuffer(1, &lpData, &lDataSize, mFilePathMMP.c_str(), &ibUseNewFile, (void**)&pGBPage, &GBPageSize, (void**)&pGBPaths, &GBKeyStart, &GBPagePosInFile);
                CHECK_HR;

                delete [] lpKeys;
                lpKeys = pGBPaths;
            }
        }
        
#endif		
        
		hr = hCreateViewMap(lnGBUnits, &lvGBUnits[0], lnDistinctRows, lpKeys, mFilePathMMP, pGBPage, GBPageSize, GBPagePosInFile);
		CHECK_HR;
		
		mvRowList.clear();
        mvRowOffset.clear();
        mvRowOffset.push_back(0);
		vector<int> lvRowList;
		i = 0;
		int j = 1;
		while(i < lnRows && j <= lnDistinctRows)
		{
			int next = -1;
            if(lpDistinctList)
                next = j < lnDistinctRows ? lpDistinctList[j] : -1;		//set to -1 to ignore next
			int cur = lvSortedList[i];			//current row offset
			while (cur != next && i < lnRows)
			{
				lvRowList.push_back(cur);
				i++;
				if (i >= lnRows)
					break;
				cur = lvSortedList[i];
			}
			
			std::sort(lvRowList.begin(), lvRowList.end());		//keep the elements order (by its original row offset)
			
			//mvRowList[j - 1].reserve(lvRowList.size());
			vector<int>::iterator it = lvRowList.begin();
			for (; it != lvRowList.end(); it++)
			{
				//mvRowList[j - 1].push_back(*it);
                mvRowList.push_back(*it);
			}
			mvRowOffset.push_back(mvRowList.size());
			lvRowList.clear();
			j++;
		}
	}
	else if (lnGBUnits == 0)
	{
		hr = hCreateViewMap(lnGBUnits, NULL, 1, NULL);
		CHECK_HR;
        mvRowList.clear();
        mvRowOffset.clear();
		//vector<int> lvRowList(mnRows);
		for (int i = 0; i < mnRows; i++)
		{
			//lvRowList[i] = i;
            mvRowList.push_back(i);
		}
        mvRowOffset.push_back(0);
        mvRowOffset.push_back(mnRows);
		//mvRowList.push_back(lvRowList);
	}
	else
		return E_FAIL;
	
	return S_OK;
}
//select distinct rows based on the first N keys
//irvKeyTable: a 2-D array, each row has nKeys keys
//nKeys: number of keys used for select distinct
int DSSDataModelFieldGroupImpl::hSelectDistinctFromSortedKeys(vector<int> &irvKeyTable, int nKeys, vector<int> &orSortedList, vector<int> &orDistinctList)
{
	if (nKeys == 0 || irvKeyTable.size() == 0)
		return 0;
	
	int lnRows = irvKeyTable.size() / nKeys;		//nTotalKeys can be greater than nKeys
	orSortedList.resize(lnRows);
    
	int i = 0;
	for (i = 0; i < lnRows; i++)
		orSortedList[i] = i;
	
	//1. sort the original rows		TODO[opt]: use bucket sort
	int *lpKeys = &irvKeyTable[0];

    
	//2. select distinct
	orDistinctList.reserve(lnRows);
	int *lpPre = NULL;
	int *lpCur = NULL;
	if (lnRows > 0)
	{
		lpPre = lpKeys + nKeys * orSortedList[0];
		orDistinctList.push_back(orSortedList[0]);
	}
	
	for (i = 1; i < lnRows; i++)
	{
		lpCur = lpKeys + nKeys * orSortedList[i];
		for (int j = nKeys -1; j >= 0; --j)
		{
			if (lpCur[j] != lpPre[j])
			{
				orDistinctList.push_back(orSortedList[i]);
				break;
			}
		}
		lpPre = lpCur;
	}
	return 0;
}

int DSSDataModelFieldGroupImpl::CountRows()
{
	return mnRows;
}

int DSSDataModelFieldGroupImpl::CountFields()
{
	return mnFields;
}

int DSSDataModelFieldGroupImpl::Select(int nPaths, int nGBUnits, vector<GUID> &irGBUnitID, int *ipGBPaths, vector<int> &orvElements)
{
	vector<int> lvSliceID;
	int lnLists = mvRowList.size();
    
    int hr = mpViewMap->MapGBKeys();
    CHECK_HR;
    
	hr = hSelectWindows(nPaths, nGBUnits, irGBUnitID, ipGBPaths, lvSliceID);
	CHECK_HR;
    
    hr = mpViewMap->UnMapGBKeys();
    CHECK_HR;
	
	int lnSlices = lvSliceID.size();
	
	set<int> lsList;
	for (int i = 0; i < lnSlices; i++)
	{
		int offset = lvSliceID[i];
		if (offset < lnLists)
        {
            lsList.insert(mvRowList.begin() + mvRowOffset[offset], mvRowList.begin() +mvRowOffset[offset+1]);
			//lsList.insert(mvRowList[offset].begin(), mvRowList[offset].end());
        }
	}
	int count = lsList.size();
	orvElements.reserve(count);
	set<int>::iterator it = lsList.begin();
	for (; it != lsList.end(); it++)
	{
		orvElements.push_back(*it);
	}
	std::sort(orvElements.begin(), orvElements.end());
	
	return S_OK;
}

int DSSDataModelFieldGroupImpl::getFieldText(int row, int field, MBase::CString &orText)
{
	if (row < 0 || row >= mnRows)
		return E_INVALIDINDEX;
	// bxia TQMS 497802, adding bound and null check
	int offset = row * mnFields + field;
	if(mvText.size() > offset && mvText[offset] != NULL)
		orText = *mvText[offset];
	else	
		orText = "";
	return 0;
}

int DSSDataModelFieldGroupImpl::getThresholdID(int row, int field)
{
	if (!mbHasThreshold)
		return -1;
	else if (row >= 0 && row < mnRows)
		return mvThresholdID[row * mnFields + field] - 1;	//TODO:[test] 0-based or 1-based
	else
		return -1;
}

// jimzhang, Sep/29/2012, to support recalculate of threshold in transaction
int DSSDataModelFieldGroupImpl::setThresholdID(int row, int field, int iThresholdID)
{
    if (row < 0 || row >= mnRows || mnFields < 0 || field >= mnFields)
		return E_INVALIDINDEX;
    
    if ( 0 == mvThresholdID.size())
    {
        int lnCells = mnRows * mnFields;
        mvThresholdID.resize(lnCells);
        memset(&mvThresholdID[0], 0, sizeof(int) * lnCells);	//init to 0*/
    }
    
    mvThresholdID[row * mnFields + field] = iThresholdID;	//TODO:[test] 0-based or 1-based
    mbHasThreshold = true;
    return S_OK;
}

int DSSDataModelFieldGroupImpl::getLink(int row, int field, FieldLink **oppLink)
{
	if (!mbHasLinks)
		return S_OK;
	else if (row < 0 || row >= mnRows)
		return E_INVALIDINDEX;
	if (oppLink)
		*oppLink = mvLinks[row * mnFields + field];
	return S_OK;
}

bool DSSDataModelFieldGroupImpl::HasThreshold(int row, int field)
{
	if (!mbHasThreshold)
		return false;
	else
		return mvThresholdID[row * mnFields + field] > 0;
}

bool DSSDataModelFieldGroupImpl::HasLinks(int row, int field)
{
	if (!mbHasLinks)
		return false;
	else
		return mvLinks[row * mnFields + field] != NULL;
}

int DSSDataModelFieldGroupImpl::size()
{
	int lSize = sizeof(this);
	
	lSize += hCalculateSizeOfBase();
	
	lSize += DSSAEHelper::SizeOfPVector(mvText, sizeof(char));
	
	lSize += DSSAEHelper::SizeOfVector(mvThresholdID);
	
	lSize += DSSAEHelper::SizeOfPVector(mvLinks);
	
	//lSize += DSSAEHelper::SizeOfVoVector(mvRowList);
	lSize += mvRowList.size() * sizeof(int);
    
    lSize += mvRowOffset.size() * sizeof(int);
    
	//mvStaticSubField;
	int lnFields = mvStaticSubField.size();
	lSize += lnFields * sizeof(vector<MBase::CString*>);
	int i = 0;
	for (i = 0; i < lnFields; i++)
	{
		lSize += DSSAEHelper::SizeOfPVector(mvStaticSubField[i], sizeof(char));
	}
	
	//mvStaticSubLink;
	lnFields = mvStaticSubLink.size();
	lSize += lnFields * sizeof(vector<vector<MBase::CString*> >);
	for (i = 0; i <lnFields; i++)
	{
		int lnLinks = mvStaticSubLink[i].size();
		lSize += lnLinks * sizeof(vector<MBase::CString*>);
		for (int k = 0; k < lnLinks; k++)
		{
			lSize += DSSAEHelper::SizeOfPVector(mvStaticSubLink[i][k], sizeof(char));
		}
	}
	
	return lSize;
}

int DSSDataModelFieldGroupImpl::setFieldText(int row, int field, MBase::CString &irNewText)
{
	if (row < 0 || row >= mnRows)
		return E_INVALIDINDEX;
	
	MBase::CString *lpFieldText = mvText[row * mnFields + field];
	CHECK_PTR(lpFieldText);
	
	*mvText[row * mnFields + field] = irNewText;
	return S_OK;
}
	
int DSSDataModelFieldGroupImpl::AddRawData(int index, AERawData &irVal)
{
	if (index < 0 || index >= mvRawData.size())
		return E_INVALIDINDEX;
	
	irVal.CopyTo(mvRawData[index]);
	
	return S_OK;
}

int DSSDataModelFieldGroupImpl::getRawData(int row, int field, const char **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag)
{
	if (!oppData || !opDataSize || !opDataType || !opDataFlag)
		return E_INVALIDARG;
	
	int index = row * mnFields + field;
	if (index < 0 || index >= mvRawData.size())
		return E_INVALIDINDEX;
	
	AERawData *lpData = &mvRawData[index];
	*oppData = lpData->mValue;
	*opDataSize = lpData->mSizeInByte;
	*opDataType = lpData->mDataType;
	*opDataFlag = lpData->mFlag;
	
	if (*opDataType == DssDataTypeMissing)
	{
		//475653, if the raw data is null, try to find out the data type from other not-null value
		int count = mvRawData.size();
		for (int i = 0; i < mnRows; i++)
		{
			int index = i * mnFields + field;
			if (index < count && mvRawData[index].mDataType != DssDataTypeMissing)
			{
				*opDataType = mvRawData[index].mDataType;
				break;
			}
		}
	}
	
	return S_OK;
}

int DSSDataModelFieldGroupImpl::setRawData(int row, int field, MBase::CString &irText,EnumDSSDataType iDataType)
{
	int index = row * mnFields + field;
	if (index < 0 || index >= mvRawData.size())
		return E_INVALIDINDEX;
	
	int hr = S_OK;
	
	AERawData lRawData = mvRawData[index];		//copy from original data first
	
	if (irText.empty())
	{
		lRawData.mFlag = DssDataNull;
		lRawData.mValue = NULL;			//it's safe to set to NULL as lRawData doesn't own its data
	}
	else
	{
		EnumDSSDataType lDataType = lRawData.mDataType;
	
		if (lDataType == DssDataTypeBigDecimal)
		{
			MDataType::DSSBigDecimal lTempBD(irText.c_str());
			lRawData.mValue = (char*)&lTempBD;
			lRawData.mFlag = DssDataOk;
			lRawData.TakeOwnership();
			
		}
		if (lDataType != DssDataTypeVarChar && lDataType != DssDataTypeMissing)
		{
			int lBufferSize = lRawData.mSizeInByte;
			lRawData.mValue = new char[lBufferSize];
			
			int lDataSize = 0;
			hr = DSSAEHelper::ConvertStringToData(irText.c_str(), lDataType, lRawData.mValue, lBufferSize, lDataSize);
			if (hr != S_OK)
			{
				_ASSERT(false);		//catch unexpected case
				return S_OK;
			}
            //580789
            if(lDataType == DssDataTypeBigDecimal)
            {
                delete [] lRawData.mValue;
                lRawData.mValue = (char*)(new MDataType::DSSBigDecimal());
                ((MDataType::DSSBigDecimal*)(lRawData.mValue))->StrToBigDecimal(irText.c_str());
            }
			lRawData.mFlag = DssDataOk;
			lRawData.TakeOwnership();
		}
		else
		{
            if (lDataType == DssDataTypeMissing && (iDataType == DssDataTypeDate || iDataType == DssDataTypeTime || iDataType == DssDataTypeTimeStamp))
            {
                lRawData.mDataType = iDataType;
                int lBufferSize = sizeof(MDataType::DSSDateTime);
                lRawData.mValue = new char[lBufferSize];
                hr = DSSAEHelper::ConvertStringToData(irText.c_str(), iDataType, lRawData.mValue, lBufferSize, lRawData.mSizeInByte);
                if (hr != S_OK)
                {
                    _ASSERT(false);		//catch unexpected case
                    return S_OK;
                }
                lRawData.mFlag = DssDataOk;
                lRawData.TakeOwnership();
            }
            else 
            {
                if (lDataType == DssDataTypeMissing)
                {
                    //original value is NULL? treate as string
                    lRawData.mDataType = DssDataTypeVarChar;
                }
                
                lRawData.mSizeInByte = irText.length() + 1;
                lRawData.mValue = (char*)irText.c_str();
                lRawData.mFlag = DssDataOk;
                lRawData.TakeOwnership();
            }
		}
	}
		
	mvRawData[index].Assign(lRawData);	

	return S_OK;
}



void DSSDataModelFieldGroupImpl::ReleaseUnusedMemory()
{
	if (!mpTransactionModel)
	{
		hCleanUp();		//clean up itself
		DSSDataModelBase::ReleaseUnusedMemory();
	}
	return;
}

int DSSDataModelFieldGroupImpl::getBinaryTransactionModel (bool includeCommitted, unsigned char** oppBinaryModel, int* opSize)
{
	int hr = S_OK;
	if(mpTransactionModel)
	{
		hr = mpTransactionModel->SaveToBinary(includeCommitted, oppBinaryModel, opSize);
		CHECK_HR;
	}
	return S_OK;
}


int DSSDataModelFieldGroupImpl::setBinaryTransactionModel (unsigned char * ipBinaryModel, int iSize)
{
	int hr = S_OK;
	hr = this->EnsureModelData();
	CHECK_HR;

    hr = this->EnsureTransactionData();
    CHECK_HR;
    
    return LoadBinaryTransactionModel(ipBinaryModel, iSize, mpTransactionModel,this);
}

//make LoadBinaryTransactionModel as static method so that the code can be shared by new rwd engine
int DSSDataModelFieldGroupImpl::LoadBinaryTransactionModel (unsigned char * ipBinaryModel, int iSize,
                                                            DSSDataModelTransaction *ipTransactionModel, DSSDataModelFieldGroupImpl * iFieldGroupModel)
{
    int hr = S_OK; 
    
	if(ipTransactionModel && ipBinaryModel)
	{
		vector<int>* lpDummy = NULL;
		vector<int>* lpMap2Field = NULL;
        vector<int>* lpMap2WBForm = NULL;
        vector<int>* lpOfflineMap2Field = NULL;
		hr = ipTransactionModel->getColumnMappingInfo(&lpDummy, &lpMap2Field, &lpMap2WBForm);
		CHECK_HR;
        
        hr = ipTransactionModel->getOfflineColumnMappingInfo(&lpDummy, &lpOfflineMap2Field, &lpMap2WBForm);
		CHECK_HR;
        
        int lnColumnCount = lpMap2Field ? lpMap2Field->size() : 0;
        int lnOfflineColumnCount = lpOfflineMap2Field ? lpOfflineMap2Field->size() : 0;
		DSSBinaryDataReader lBinaryReader;
		
		hr = lBinaryReader.Init(ipBinaryModel, iSize, false);
		CHECK_HR;
		
		//TODO: how to use current slice flag?
		int currentSliceFlag = -1;
		hr = lBinaryReader.ReadInt(&currentSliceFlag);
		CHECK_HR;
		
		int lModelType = -1;
		hr = lBinaryReader.ReadInt(&lModelType);
		CHECK_HR;
		
		if(lModelType != DssModelFieldGroup)
			return E_UNEXPECTED;
		
		//load the column header info
		int lnHeaderColumn = 0;
		hr = lBinaryReader.ReadInt(&lnHeaderColumn);
		CHECK_HR;
		
		//actually for field group model the lnHeaderColumn should always be 0
		for(int i = 0; i < lnHeaderColumn; i++)
		{
			int lColumnIndex = -1;
			hr = lBinaryReader.ReadInt(&lColumnIndex);
			CHECK_HR;
			
			GUID lUnitGUID = GUID_NULL;
			char* lpChar = (char*)(&lUnitGUID);
			hr = lBinaryReader.ReadByteArray(lpChar, sizeof(GUID));
			CHECK_HR;
			
			//find the tabular unit
//TODO:			int lUnit = FindUnit(lUnitGUID, DssTemplateReserved);
			DSSTabularUnit* lpUnit = NULL; //TODO: Item(lUnit);	
			CHECK_PTR(lpUnit);
			
			int lBaseForm = -1;
			hr = lBinaryReader.ReadInt(&lBaseForm);
			CHECK_HR;
			
			hr = ipTransactionModel->setColumnInfo(lColumnIndex, lpUnit, lBaseForm);
			CHECK_HR;
		}
		
        //load offline column data type info, not used by grid model
        int lnOfflineColumn = 0;
        hr = lBinaryReader.ReadInt(&lnOfflineColumn);
        CHECK_HR;
        
        //assert(lnOfflineColumnCount == lnOfflineColumn);
        
        for(int i = 0; i < lnOfflineColumn; i++)
        {
            int lDataType = 0;
            hr = lBinaryReader.ReadInt(&lDataType);
            CHECK_HR;
            
            hr = ipTransactionModel->setColumnDataType(lnColumnCount + i, (EnumDSSDataType)lDataType);
            CHECK_HR;
        }
        
		//populate the transaction records
		int lnRecord = 0;
		hr = lBinaryReader.ReadInt(&lnRecord);
		CHECK_HR;
		
        bool lbSetCommit = false;
        EnumDSSTransactionDataChangeType lFlag = DssTransactionDataReserved;
		for(int i = 0; i < lnRecord; i++)
		{
			lFlag = DssTransactionDataReserved;
			hr = lBinaryReader.ReadInt((int*)(&lFlag));
			CHECK_HR;
			
            //when saving the transaction records, the committed records will be saved first
            //so when we meet a Update  type records, we need to set commit for all the previous records
            if(lFlag == DssTransactionDataUpdate)
            {
                if(i > 0 && !lbSetCommit)
                {
                    hr = ipTransactionModel->setAllCommit();
                    CHECK_HR;
                }
                lbSetCommit = true;
            }
            
			int lOriginalRow = 0;
			hr = lBinaryReader.ReadInt(&lOriginalRow);
			CHECK_HR;
			
			int lOriginalCol = 0;
			hr = lBinaryReader.ReadInt(&lOriginalCol);
			CHECK_HR;
			
            int lbMarked = 0;
            hr = lBinaryReader.ReadInt(&lbMarked);
            CHECK_HR;
            
            int lbValidate = 1;
            hr = lBinaryReader.ReadInt(&lbValidate);
            CHECK_HR;
            
			int lRow = lOriginalRow;
			int lCol = lOriginalCol;
			
			//load the changed cells
			int lnChangedField = 0;
			hr = lBinaryReader.ReadInt(&lnChangedField);
			CHECK_HR;
			
			//get the original data first
			vector<AERawData> lvOriginalData;
			lvOriginalData.resize(lnOfflineColumnCount + lnColumnCount);
            
            
            
            // Sep/5/2012, do not populate raw data here, and the reason is in new RWD, getRawData() is an interface of FG iterator,
            //      but FG iterator has not been created at this stage. In new RWD, raw data of transaction record will be populated
            //      in ReInit() of FG iterator.
            if (iFieldGroupModel) {
                for (int i = 0; i < lpMap2Field->size(); i++)
                {
                    if ((*lpMap2Field)[i] >= 0)
                    {
                        //473405, we need submit the raw data instead of formatted field text
                        //hr = mpFieldGroupModel->getFieldText(lRow, (*lpMap2Field)[i], lText);
                        //TODO:
                        hr = iFieldGroupModel->getRawData(lRow, (*lpMap2Field)[i],
                                        (const char**)&lvOriginalData[i].mValue,
                                        &lvOriginalData[i].mSizeInByte,
                                        &lvOriginalData[i].mDataType,
                                        &lvOriginalData[i].mFlag);
                        CHECK_HR;
                    }
                }

            }
			
			for(int j = 0; j < lnChangedField; j++)
			{
				int lColumnIndex = -1;
				hr = lBinaryReader.ReadInt(&lColumnIndex);
				CHECK_HR;
				
				//new text value
				int lStrLength = 0;
				hr = lBinaryReader.ReadInt(&lStrLength);
				CHECK_HR;
				
                if (lStrLength+1 == 0) //check if integer overflow, resulting in buffer overflow
                    return DISP_E_OVERFLOW;
				char* lpText = new char[lStrLength + 1];
				memset(lpText,0,lStrLength+1);
				hr = lBinaryReader.ReadByteArray(lpText, lStrLength);
				CHECK_HR;
				
				std::string lText = lpText;
				delete[] lpText;
				
                // load display text for DIC                
                lStrLength = 0;
                hr = lBinaryReader.ReadInt(&lStrLength);
                CHECK_HR;
                
                char* lpDisplayText = NULL;
                std::string lDisplayText = lText;
                if(lStrLength >=0 )
                {
                    if (lStrLength+1 == 0) //check if integer overflow, resulting in buffer overflow
                        return DISP_E_OVERFLOW;
                    lpDisplayText = new char[lStrLength + 1];
                    memset(lpDisplayText,0,lStrLength+1);
                    hr = lBinaryReader.ReadByteArray(lpDisplayText, lStrLength);
                    CHECK_HR;
                    
                    lDisplayText = lpDisplayText;
                    delete lpDisplayText;
                }
                
				//offset, not used for FG
				int lKey = 0;
				hr = lBinaryReader.ReadInt(&lKey);
				CHECK_HR;
				
				//load original AERawData
				AERawData lRawData;
				
				hr = lBinaryReader.ReadInt((int*)(&lRawData.mFlag));
				CHECK_HR;
				
				hr = lBinaryReader.ReadInt((int*)(&lRawData.mDataType));
				CHECK_HR;
				
				hr = lBinaryReader.ReadInt(&lRawData.mSizeInByte);
				CHECK_HR;
				
				lRawData.mValue = new char[lRawData.mSizeInByte];
				hr = lBinaryReader.ReadByteArray(lRawData.mValue, lRawData.mSizeInByte);
				CHECK_HR;	
				
				lvOriginalData[lColumnIndex] = lRawData;
				
				//load original Field Text
				lStrLength = 0;
				hr = lBinaryReader.ReadInt(&lStrLength);
				CHECK_HR;
				
                if (lStrLength+1 == 0) //check if integer overflow, resulting in buffer overflow
                    return DISP_E_OVERFLOW;
				char* lpField = new char[lStrLength + 1];
				memset(lpField,0,lStrLength+1);
				hr = lBinaryReader.ReadByteArray(lpField, lStrLength);
				CHECK_HR;
				
				std::string lFieldText = lpField;
				delete[] lpField;
				
				hr = ipTransactionModel->setOriginalFieldText(lRow, lCol, lColumnIndex, lFieldText);
				CHECK_HR;
				
				//need to adjust col ordiinal
                if(lColumnIndex < lnColumnCount)
                {
                    lCol = (*lpMap2Field)[lColumnIndex];
                }
                else if(lColumnIndex < lnColumnCount + lnOfflineColumnCount)
                {
                    lCol = (*lpOfflineMap2Field)[lColumnIndex - lnColumnCount];
                }
                //dummy, not used for FG
                vector<AERawData> lvDisplayData(lvOriginalData.size());
				hr = ipTransactionModel->AddRecord(lRow, lCol, lColumnIndex, lText, &lvOriginalData, &lvDisplayData);
				CHECK_HR;
				
                if(lpDisplayText)
                {
                    hr = ipTransactionModel->UpdateField(lRow, lCol, lColumnIndex, &lDisplayText,true);
                    CHECK_HR;
                }
                else
                {
                    hr = ipTransactionModel->UpdateField(lRow, lCol, lColumnIndex, &lText);
                    CHECK_HR;	
                }
			}
	
			hr = ipTransactionModel->setRecordFlag(lOriginalRow, lOriginalCol, lFlag);
			CHECK_HR;
		}
        if(lnRecord > 0 && lFlag == DssTransactionDataCommit && !lbSetCommit)
        {
            hr = ipTransactionModel->setAllCommit();
            CHECK_HR;
            // lbSetCommit = true;
        }
                
        DSSDataModelTransactionRWField *lpNewRWDTrxModel = dynamic_cast<DSSDataModelTransactionRWField *>(ipTransactionModel);
        
        if (! lpNewRWDTrxModel) // jimzhang, Nov/15/2012, skip loading raw data in new RWD logic, as these data is not saved in new RWD.
        {
            int lnModifedRawData = 0;
            hr = lBinaryReader.ReadInt(&lnModifedRawData);
            CHECK_HR;
            
            for(int i = 0; i < lnModifedRawData; i++)
            {
                int lRow = -1;
                hr = lBinaryReader.ReadInt(&lRow);
                CHECK_HR;
                
                int lField = -1;
                hr = lBinaryReader.ReadInt(&lField);
                CHECK_HR;
                
                int lStrLength = 0;
                hr = lBinaryReader.ReadInt(&lStrLength);
                CHECK_HR; 
                
                char* lpText = new char[lStrLength + 1];
                memset(lpText,0,lStrLength+1);
                hr = lBinaryReader.ReadByteArray(lpText, lStrLength);
                CHECK_HR;
                
                std::string lText = lpText;
                delete[] lpText;
                
                //setRawData(lRow, lField, lText);  //TODO: make setRawData as static
            }
        }        
        
        if (lpNewRWDTrxModel)
        {
            // Sep/7/2012 In new RWD, mapping of GB elements to row block (DSSDataModelTransactionRWField::mmGBElements2BlockOff) should be loaded.
                        
            int lnMapping = 0; // size of DSSDataModelTransactionRWField::mmGBElements2BlockOff
            hr = lBinaryReader.ReadInt(&lnMapping);
            CHECK_HR;
            
            if (lnMapping)
            {
                int lnGBUnits = 0; //GB Unit count
                hr = lBinaryReader.ReadInt(&lnGBUnits);
                CHECK_HR;
                
                for (int iMapping = 0; iMapping < lnMapping; ++iMapping)
                {
                    vector<vector<int> > lvGBElemetns(lnGBUnits);
                    for (int iGBUnit = 0; iGBUnit < lnGBUnits; ++iGBUnit)
                    {
                        vector<int> lvUnitGBElements;
                        hr = lBinaryReader.ReadIntArray(lvUnitGBElements);
                        CHECK_HR;
                        
                        lvGBElemetns[iGBUnit] = lvUnitGBElements;
                    }
                    
                    int lBlockOff = 0;
                    hr = lBinaryReader.ReadInt(&lBlockOff);
                    CHECK_HR;
                    
                    lpNewRWDTrxModel->AddGBElements2BlockOff(lvGBElemetns, lBlockOff);
                }
                
                int lBlockSize = 0;
                hr = lBinaryReader.ReadInt(&lBlockSize);
                CHECK_HR;
                
                lpNewRWDTrxModel->setBlockSize(lBlockSize);
            }
        }
	}
	return S_OK;
}

int DSSDataModelFieldGroupImpl::EnsureTransactionData()
{
    int hr = S_OK;
    
    if(!mpObjContext)
        return E_FAIL;
    
    DSSRWNode* lpNode = mpObjContext->getNode(mNodeKey);
    CHECK_PTR(lpNode);
    
    AE_ASSERT(lpNode->getType() == DssRWNodeFieldGroup);
    
    DSSRWFieldGroupNode * lpThisNode = static_cast<DSSRWFieldGroupNode*> (lpNode);
    
    if(!mpTransactionModel)
    {
        if (lpThisNode->IsTransactionEnabled())	//transaction enabled
        {
            mpTransactionModel = new DSSDataModelTransaction();
        }
    }
    	
    //return OK if not a transaction enabled xtab model
	if (!mpTransactionModel)
		return S_OK;
	
    DSSTransactionReport *lpTargetReport = lpThisNode->getTransactionReport();
	CHECK_PTR(lpTargetReport);
    
	if (mpTransactionModel->getStatus() != XTAB_INITIALIZED)
	{
		hr = mpTransactionModel->Init(lpTargetReport, this);
		CHECK_HR;
	}
    return EnsureTransactionData(mpTransactionModel, lpThisNode);
}

//make EnsureTransactionData as static method so that the code can be shared by new rwd engine
int DSSDataModelFieldGroupImpl::EnsureTransactionData(DSSDataModelTransaction *ipTransactionModel, DSSRWFieldGroupNode  *ipFGNode, DFCCSITransactionBase* ipCSI)
{
    int hr = S_OK;

    vector<int>* lpMapTrxColumn = NULL;
    hr = ipTransactionModel->getColumnMappingInfo(&lpMapTrxColumn, NULL,NULL);
    CHECK_HR;
    
    //return if the maps are already set
    if(lpMapTrxColumn && lpMapTrxColumn->size() > 0)
        return S_OK;

    
	map<int, DSSColumnMappingInfo*>* lpColumnInfoMap = ipFGNode->getTransactionReport()->getColumnMap();
	int lnTargetColumns = 0;
    //782890
    map<int, DSSColumnMappingInfo*>::iterator lIter;
	for (lIter = lpColumnInfoMap->begin(); lIter != lpColumnInfoMap->end(); lIter++)
	{
        if(lIter->first > lnTargetColumns)
            lnTargetColumns = lIter->first;
    }

    
	vector<int> mapToFields;
    mapToFields.resize(lnTargetColumns);
	
	int i = 0;
	for (int i = 0; i < lnTargetColumns; i++)
		mapToFields[i] = -1;
	
	DSSFields *lpFields = ipFGNode->getFields();
	int lnFields = lpFields->Count();
	
	//1,check document level mapping first
	for (i = 0; i < lnFields; i ++)
	{
		DSSField *lpField = lpFields->Item(i);
		if (lpField->IsMappedForTransaction())
		{
			int lColumnIndex = lpField->getColumnIndex();
			lColumnIndex --;	//it's 1-based
			AE_ASSERT(lColumnIndex >= 0 && lColumnIndex < lnTargetColumns);
			mapToFields[lColumnIndex] = i;
		}
	}
	
	//2, build the map from source field to transaction report's template
	for (i = 0; i < lnTargetColumns; i++)
	{
		if (mapToFields[i] != -1)
			continue;
        
        int lSourceFieldID = -1;
        if(lpColumnInfoMap->find(i+1) != lpColumnInfoMap->end())
        {
            DSSColumnMappingInfo *lpColumnInfo = (*lpColumnInfoMap)[i + 1];	//column mapping info is 1-based
            CHECK_PTR(lpColumnInfo);
            

            for (int j = 0; j < lnFields; j++)
            {
                hr = hFindSourceField(lpColumnInfo, lpFields->Item(j));
                if (hr == S_OK)
                {
                    lSourceFieldID = j;
                    break;
                }
            }
            
            if (lSourceFieldID < 0)		//can't find the source for transaction report
                return E_FAIL;
		}
		mapToFields[i] = lSourceFieldID;
	}
	
	vector<int> lvDummy(lnTargetColumns);
	ipTransactionModel->setMapTransactionColumnToMetric(lvDummy, mapToFields,lvDummy);

    

    if(!ipCSI)
    {
        return S_OK;
    }
    
    set<Int32, less<Int32>, MBase::Allocator<Int32> >* lpPlaceHolders = ipCSI->GetPlaceHolderSet();
    if(!lpPlaceHolders)
    {
        return S_OK;
    }
    
    int lnOfflineTargetColumns = lpPlaceHolders->size();
    if(lnOfflineTargetColumns <= 0)
    {
        return S_OK;
    }
    
    
	vector<int> mvOfflineMapToFields(lnOfflineTargetColumns,-1);
	
	
	//1,check document level mapping first
	for (i = 0; i < lnFields; i ++)
	{
		DSSField *lpField = lpFields->Item(i);
		if (lpField->IsMappedForOfflineTransaction())
		{
			int lColumnIndex = lpField->getOfflineColumnIndex();
			lColumnIndex --;	//it's 1-based
			AE_ASSERT(lColumnIndex >= 0 && lColumnIndex < lnTargetColumns);
			mvOfflineMapToFields[lColumnIndex] = i;
		}
	}
	
	//2, build the map from source field to transaction report's template
	for (i = 0; i < lnOfflineTargetColumns; i++)
	{
		if (mvOfflineMapToFields[i] != -1)
			continue;
        
        //suppose offline columns must be mapped
        mvOfflineMapToFields.clear();
        return S_OK;
	}
    
    vector<int> lvDummy2(lnOfflineTargetColumns);
    vector<EnumDSSDataType> mvOfflineDataType(lnOfflineTargetColumns, DssDataTypeReserved);
    vector<int> mapOfflineToTrxColumn(lnOfflineTargetColumns, -1);
    
    //data type is loaded from binary
    for(int i = 0; i < lnOfflineTargetColumns; i++)
    {
        if(mvOfflineMapToFields[i] >= 0)
        {
            for(int j = 0; j < mapToFields.size(); j++)
            {
                if(mapToFields[j] == mvOfflineMapToFields[i])
                {
                    mapOfflineToTrxColumn[i] = j;
                }
            }
        }
    }
	ipTransactionModel->setOfflineTransaction(lvDummy, mvOfflineMapToFields,lvDummy, mvOfflineDataType, mapOfflineToTrxColumn);
    
    
    return S_OK;
}


int DSSDataModelFieldGroupImpl::hFindSourceField(DSSColumnMappingInfo * ipColumnInfo, DSSField *ipField)
{
	EnumDSSRWFieldType lFieldType = ipField->getType();
	if (lFieldType == DssRWFieldObjectValue)
	{
		DSSObjectInfo *lpFieldObject = ipField->getFieldObject();
		
		if (ipColumnInfo->getType() == DssTypeMetric)
		{
			if (lpFieldObject->getType() == DssTypeMetric
				&& MBase::IsEqualGUID(ipColumnInfo->getMetricID(), lpFieldObject->getID()))
				return S_OK;
		}
		else if (lpFieldObject->getType() == DssTypeAttribute) 
		{
			GUID lAttributeID = ipColumnInfo->getAttributeID();
			GUID lBaseFormID = ipColumnInfo->getFormID();
			
			DSSAttributeFormWrappers* lpFieldForms = ipField->getAttributeForms();
			
			DSSUnitObject *lpUnitObject = lpFieldObject->getObjectContext()
			->getDataElements()->getDefaultDataElement()->getUnitObjects()->FindByID(lpFieldObject->getID());
			
			if (lpUnitObject)
				lpUnitObject->getBaseFormTypes();
			
			if (lpFieldForms->Count() == 0)
			{
				//TODO: somehow the forms is 0
				if (MBase::IsEqualGUID(lpFieldObject->getID(), lAttributeID))
					return S_OK;
			}			
			else
			{
                for (int i = 0; i < lpFieldForms->Count(); i++)
                {
					if (MBase::IsEqualGUID(lpFieldObject->getID(), lAttributeID)
						&& MBase::IsEqualGUID(lpFieldForms->Item(i)->getID(), lBaseFormID))
                        return S_OK;
                }
			}
		}
	}
	else if (lFieldType == DssRWFieldConcatenation || lFieldType == DssRWFieldText)		//TODO: I only see Text, not concatenation
	{
		DSSFields *lpSubFields = ipField->getChildFields();
		
		if (lpSubFields)
		{
			for (int i = 0; i < lpSubFields->Count(); i++)
			{
				if (hFindSourceField(ipColumnInfo, lpSubFields->Item(i)) == S_OK)
					return S_OK;
			}
		}
	}
	return S_FALSE;
}
int DSSDataModelFieldGroupImpl::setDataSource(DSSDataSource *ipDataSource)
{
	mpDataSource = ipDataSource;
	return S_OK;
}

int DSSDataModelFieldGroupImpl::LoadData(vector<int>& irvRows)
{
    int hr = S_OK;
    if(mpDataSource)
    {
        hr = mpDataSource->LoadFieldData(this, irvRows);
        CHECK_HR;
    }
    return hr;
}

int DSSDataModelFieldGroupImpl::getRowOrdinal(int& iRow)
{
    if(iRow < 0 || iRow >= mvRowList.size())
        return E_INVALIDINDEX;
    iRow = mvRowList[iRow];
    return S_OK;
}

int DSSDataModelFieldGroupImpl::LoadInputControlModel(std::string iKey, DSSDataModelControlImpl** oppControl)
{
    return  mpModelDataSource->LoadInputControlModel(this, iKey, oppControl);
}
