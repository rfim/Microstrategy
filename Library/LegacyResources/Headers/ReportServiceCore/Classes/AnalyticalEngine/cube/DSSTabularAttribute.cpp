/*
 *  DSSTabularAttribute.cpp
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 12/29/09.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#include "DSSTabularAttribute.h"
#import "DSSDataColumn.h"
#import "DSSAttributeElementProxy.h"
#import "DSSDataRefColumn.h"

DSSTabularAttribute::DSSTabularAttribute():mArity(0), mnForms(0), mWHNullPosition(-1)
{
	mUnitType = DssTemplateAttribute;
}

DSSTabularAttribute::~DSSTabularAttribute()
{
	int lnForms = mvColumns.size();
	for (int i = 0; i < lnForms; i++)
	{
		if (mvColumns[i])
		{
			delete mvColumns[i];
			mvColumns[i] = NULL;
		}
	}
}

int DSSTabularAttribute::Init(int index, int arity)
{
	mUnitID = index;
	mArity = arity;
	
	return S_OK;
}

int DSSTabularAttribute::AddFormDataColumn(int iBaseFormID, int iBaseFormType, DSSDataColumn *ipColumn)
{
	mvBaseFormID.push_back(iBaseFormID);
	mvBaseFormType.push_back((EnumDSSBaseFormType)iBaseFormType);
	mvColumns.push_back(ipColumn);
	mnForms = mvBaseFormID.size();
	return S_OK;
}

int DSSTabularAttribute::Count()
{
	return mvColumns.size();
}

int DSSTabularAttribute::CountRows()
{
	if (!mvColumns.empty() && mvColumns[0])
		return mvColumns[0]->getRowCount();
	else
		return 0;
}

int DSSTabularAttribute::Item(int iForm, DSSDataColumn **oppColumn)
{
	if (iForm < 0 || iForm >= mvColumns.size() || !oppColumn)
		return E_INVALIDARG;
    
	*oppColumn = mvColumns[iForm];
	return S_OK;
}

int DSSTabularAttribute::FindFormWithLocale(int iAttribute, int iBaseFormID, int iLocale, int &orColumn)
{
	for (int i = 0; i < mvColumns.size(); i++)
	{
		if (iBaseFormID ==	mvBaseFormID[i])
		{
			orColumn = i;
			return S_OK;
		}
	}
	return S_FALSE;
}

int DSSTabularAttribute::getArity()
{
	return mArity;
}

int DSSTabularAttribute::getBaseFormID(int iForm)
{
	if (iForm >= 0 && iForm < mvBaseFormID.size())
		return mvBaseFormID[iForm];
	else
		return -1;
}

EnumDSSBaseFormType DSSTabularAttribute::getBaseFormType(int iForm)
{
	if (iForm >= 0 && iForm < mvBaseFormID.size())
		return mvBaseFormType[iForm];
	else
		return DssBaseFormReserved;
}

int DSSTabularAttribute::FindElement(DSSBaseElementProxy *ipElement, int &orKey)
{
	EnumDSSElementType lType = ipElement->getElementType();
    
	if (lType != DssElementConcrete)
	{
		//409782, null element
		if (lType == DssElementNULL && mNullElementIndex >= 0)
		{
			orKey = mNullElementIndex;
			return S_OK;
		}
		return S_FALSE;
	}
    
    //510012, 10/11/2011, need check unit GUID first
    if (!MBase::IsEqualGUID(this->mGUID, ipElement->getObjectID()))
    {
        return S_FALSE;     //the element comes from a different object
    }
    
	int hr = S_OK;
	int lArity = mArity;
	
	if (mvIDFormDataBuffer.empty())
	{
		_ASSERT(mvColumns.size() >= lArity);
		for (int i = 0; i < lArity; i++)
		{
            if (!mvColumns[i])
                return S_FALSE;
            
			int lSize = mvColumns[i]->getSizeOfData();
			
			if (mvIDFormDataBuffer.size() <= lSize)
				mvIDFormDataBuffer.resize(lSize + 4);	//include the end character of string
		}
	}
	
	if (lArity == 1)
	{
		MBase::CString lFormVal = static_cast<DSSAttributeElementProxy*> (ipElement)->getValue(0);
		DSSDataFlag lFlag = DssDataOk;
		if(lFormVal=="" || lFormVal == "~!@@~~!@@~")
			lFlag = DssDataNull;
        
		unsigned char *lpData = NULL;
		int lDataSize = 0;
        
		DSSDataColumn *lpColumn = mvColumns[0];
		
		MDataType::DSSBigDecimal lTempBD;
		if (lpColumn->getDataType() == DssDataTypeBigDecimal)
		{
			lTempBD.StrToBigDecimal(lFormVal.c_str());
			lpData = (unsigned char*) &lTempBD;
			lDataSize = sizeof(MDataType::DSSBigDecimal);
		}
		else if (!DSSAEHelper::IsStringColumn(lpColumn))
		{
			int lSize = lpColumn->getSizeOfData();
			AE_ASSERT(lSize <= mvIDFormDataBuffer.size());
			
			hr = DSSAEHelper::ConvertStringToData(lFormVal.c_str(), lpColumn->getDataType(), &mvIDFormDataBuffer[0], lSize, lDataSize);
			CHECK_HR;
			lpData = &mvIDFormDataBuffer[0];
		}
		else
		{
			lpData = (unsigned char*)lFormVal.c_str();
			lDataSize = lFormVal.length();
		}
		
		//linear search O(n*m)		TODO:[opt]
		int lnRows = lpColumn->getRowCount();
		int i = 0;
		for (i = 0; i < lnRows; i++)
		{
			if (lpColumn->CompareToData(i, lpData, lDataSize, lFlag) == 0)
			{
				orKey = i;		//found
				return S_OK;
			}
		}
	}
	else if(mvColumns.size() > 0)
	{
		//TODO:[opt] linear search O(n*m)
		int lnRows = mvColumns[0]->getRowCount();
		int r = 0;
		for (r = 0; r < lnRows; r++)
		{
			int s = 0;
			for (s = 0; s < lArity; s++)
			{
				MBase::CString lFormVal = static_cast<DSSAttributeElementProxy*> (ipElement)->getValue(s);
				DSSDataFlag lFlag = DssDataOk;
				if(lFormVal=="" || lFormVal == "~!@@~~!@@~")
					lFlag = DssDataNull;
				
				unsigned char *lpData = NULL;
				int lDataSize = 0;
				
				DSSDataColumn *lpColumn = mvColumns[s];
				MDataType::DSSBigDecimal lTempBD;
				if (lpColumn->getDataType() == DssDataTypeBigDecimal)
				{
					lTempBD.StrToBigDecimal(lFormVal.c_str());
					lpData = (unsigned char*) &lTempBD;
					lDataSize = sizeof(MDataType::DSSBigDecimal);
				}
				else if (!DSSAEHelper::IsStringColumn(lpColumn))
				{
					int lSize=lpColumn->getSizeOfData();
					AE_ASSERT(lSize <= mvIDFormDataBuffer.size());
					
					hr = DSSAEHelper::ConvertStringToData(lFormVal.c_str(), lpColumn->getDataType(), &mvIDFormDataBuffer[0], lSize, lDataSize);
					CHECK_HR;
					lpData = &mvIDFormDataBuffer[0];
				}
				else
				{
					lpData = (unsigned char*)lFormVal.c_str();
					lDataSize = lFormVal.length();
				}
				
				if (lpColumn->CompareToData(r, lpData, lDataSize, lFlag) != 0)
					break;
			}//end of for (s = 0; s < lArity; s++)
			
			if (s == lArity)		//found
			{
				orKey = r;
				return S_OK;
			}
		} //end of for (r = 0; r < lnRows; r++)
	}
	
	orKey = -1;
	return S_FALSE;
}

int DSSTabularAttribute::FindElement(EnumDSSDataType * ipTypes, Int32 * ipcbDatas, unsigned char ** ippDatas, DSSDataFlag * ipFlags, int &orKey)
{
	orKey = -1;
	if(mvColumns.size() > 0)
	{
		int lnRows = mvColumns[0]->getRowCount();
		for (int r = 0; r < lnRows; r++)
		{
			int s = 0;
			for (s = 0; s < mArity; s++)
			{
				DSSDataColumn *lpColumn = mvColumns[s];
                CHECK_PTR(lpColumn);
				if (lpColumn->CompareToData(r, ippDatas[s], ipcbDatas[s], ipFlags[s]) != 0)
					break;
			}//end of for (s = 0; s < lArity; s++)
			
			if (s == mArity)		//found
			{
				orKey = r;
				return S_OK;
			}
		} //end of for (r = 0; r < lnRows; r++)
	}
	
	return S_FALSE;
}

int DSSTabularAttribute::createElement(int key, DSSObjectContext *ipObjectContext, DSSBaseElementProxy **oppElement, int iGlobalKey, int iGlobalKeyVer)
{
    return createNewDrivedElement(key, ipObjectContext, NULL, oppElement, iGlobalKey, iGlobalKey);
}

int DSSTabularAttribute::createNewDrivedElement(int key, DSSObjectContext *ipObjectContext, GUID * ipConsolidationElementID, DSSBaseElementProxy **oppElement, int iGlobalKey, int iGlobalKeyVer)
{
	if (!oppElement)
		return E_POINTER;
    
	if (key < 0)
		return createSpecialElement(DssElementSubtotal, -key, ipObjectContext, oppElement);
	
	int hr = S_OK;
	if (mArity > 0 && mvKeyFormID.empty())	//TODO: change the constructor of attributeElementProxy to avoid using mvKeyFormID
	{
		mvKeyFormID.assign(mvBaseFormID.begin(), mvBaseFormID.begin() + mArity);
		mvKeyFormType.assign(mvBaseFormType.begin(), mvBaseFormType.begin() + mArity);
	}
	
	int lArity = mArity;
	vector<std::string> lvBaseFormValues(lArity);
	for (int i = 0; i < lArity; i++)
	{
		DSSDataColumn *lpColumn = mvColumns[i];
		CHECK_PTR(lpColumn);
		
		const char *lpData = NULL;
		int lSizeInByte = 0;
		EnumDSSDataType lType = DssDataTypeUnknown;
		DSSDataFlag lFlag = DssDataOk;
		hr = lpColumn->getData(key, &lpData, lSizeInByte, lType, lFlag);
		CHECK_HR;
		
		//lwang, 418545, format the form value string according to base form type instead column data type
		if (lType == DssDataTypeDate || lType == DssDataTypeTime || lType == DssDataTypeTimeStamp)
		{
			EnumDSSBaseFormType lBaseFormType = mvKeyFormType[i];
			if (lBaseFormType == DssBaseFormDate)
				lType = DssDataTypeDate;
			else if (lBaseFormType == DssBaseFormTime)
				lType = DssDataTypeTime;
			else if (lBaseFormType == DssBaseFormDateTime)
				lType = DssDataTypeTimeStamp;
		}
        
		//411051, null from value
		if (lFlag == S_OK)
		{
			hr = DSSAEHelper::ConvertToString(lpData, lSizeInByte, lType, lFlag, lvBaseFormValues[i]);
			CHECK_HR;
		}
		else
		{
			lvBaseFormValues[i] = "~!@@~~!@@~";
		}
        
	}
	
    if (!ipConsolidationElementID)
    {
        *oppElement =  new DSSAttributeElementProxy(ipObjectContext, mGUID,
                                                mvKeyFormID, mvKeyFormType, lvBaseFormValues);
    }
    else
    {
        *oppElement =  new DSSAttributeElementProxy(ipObjectContext, mGUID, *ipConsolidationElementID, mvKeyFormID, mvKeyFormType, lvBaseFormValues);
    }

    //junwang, 04/19/2012
    if(*oppElement)
        (*oppElement)->setGlobalKey(iGlobalKey, iGlobalKeyVer);
    
	return S_OK;
}


int	DSSTabularAttribute::getAttributeElementIDForAutoText(int key, DSSObjectContext *ipObjectContext, MBase::CString &irAttributeIDText)
{
    int hr = S_OK;
    
    //hyan,2012/05/09
    //TQMS591932
	if (mArity > 0 && mvKeyFormID.empty())	//TODO: change the constructor of attributeElementProxy to avoid using mvKeyFormID
	{
		mvKeyFormID.assign(mvBaseFormID.begin(), mvBaseFormID.begin() + mArity);
		mvKeyFormType.assign(mvBaseFormType.begin(), mvBaseFormType.begin() + mArity);
	}
    
    MBase::CString lstr=DSSAEHelper::Guid2String(mGUID);
    
    irAttributeIDText.append(lstr);
    
    
    
    int lArity = mArity;
	vector<std::string> lvBaseFormValues(lArity);
	for (int i = 0; i < lArity; i++)
	{
		DSSDataColumn *lpColumn = mvColumns[i];
		CHECK_PTR(lpColumn);
		
		const char *lpData = NULL;
		int lSizeInByte = 0;
		EnumDSSDataType lType = DssDataTypeUnknown;
		DSSDataFlag lFlag = DssDataOk;
		hr = lpColumn->getData(key, &lpData, lSizeInByte, lType, lFlag);
		CHECK_HR;
        
        
		//lwang, 418545, format the form value string according to base form type instead column data type
		if (lType == DssDataTypeDate || lType == DssDataTypeTime || lType == DssDataTypeTimeStamp)
		{
			EnumDSSBaseFormType lBaseFormType = mvKeyFormType[i];
			if (lBaseFormType == DssBaseFormDate)
				lType = DssDataTypeDate;
			else if (lBaseFormType == DssBaseFormTime)
				lType = DssDataTypeTime;
			else if (lBaseFormType == DssBaseFormDateTime)
				lType = DssDataTypeTimeStamp;
		}
        
		//411051, null from value
		if (lFlag == S_OK)
		{
			hr = DSSAEHelper::ConvertToString(lpData, lSizeInByte, lType, lFlag, lvBaseFormValues[i]);
			CHECK_HR;
            
            irAttributeIDText.append(":");
            
            irAttributeIDText.append(lvBaseFormValues[i]);
		}
		else
		{
			lvBaseFormValues[i] = "~!@@~~!@@~";
		}
        
	}
    
    return S_OK;
}

int DSSTabularAttribute::size()
{
	int lSize = sizeof(this);
	
	lSize += DSSTabularUnit::size();
	
	lSize += DSSAEHelper::SizeOfPVector(mvColumns);
	
	lSize += DSSAEHelper::SizeOfVector(mvBaseFormID);
	
	lSize += DSSAEHelper::SizeOfVector(mvBaseFormType);
	
	lSize += DSSAEHelper::SizeOfVector(mvKeyFormID);
	
	lSize += DSSAEHelper::SizeOfVector(mvKeyFormType);
	
	lSize += DSSAEHelper::SizeOfVector(mvIDFormDataBuffer);
    
	return lSize;
}

int DSSTabularAttribute::setData(int iBaseFormID, int iOffset, char *ipData, int iSizeInByte)
{
	int lColumnID = 0;
	int hr = FindFormWithLocale(0, iBaseFormID, 0, lColumnID);
	CHECK_HR;
	
	return mvColumns[lColumnID]->setData(iOffset, ipData, iSizeInByte, ipData != NULL ? DssDataOk : DssDataNull);
	
}

//set attribute form value by a text string
int DSSTabularAttribute::setData(int iBaseFormID, int iOffset, MBase::CString &irText)
{
	int hr = S_OK;
	
	int lColumnID = 0;
	hr = FindFormWithLocale(0, iBaseFormID, 0, lColumnID);
	CHECK_HR;
	
	DSSDataColumn *lpColumn = mvColumns[lColumnID];
	
	if (irText.empty())
	{
		//set as NULL
		hr = lpColumn->setData(iOffset, NULL, 0, DssDataNull);
		CHECK_HR;
        
		return S_OK;
	}
	
	EnumDSSDataType lDataType = lpColumn->getDataType();
	if (lDataType == DssDataTypeBigDecimal)
	{
		MDataType::DSSBigDecimal lTempBD(irText.c_str());
        
		hr = lpColumn->setData(iOffset, (char*)&lTempBD, sizeof(MDataType::DSSBigDecimal), DssDataOk);
		CHECK_HR;
	}
	else if (!DSSAEHelper::IsStringColumn(lpColumn))
	{
		int lBufferSize = 0;
		lBufferSize = lpColumn->getSizeOfData();
		CHECK_HR;
		
		vector<char> lvNumericalData(lBufferSize);
		
		int lDataSize = 0;
		hr = DSSAEHelper::ConvertStringToData(irText.c_str(), lDataType, &lvNumericalData[0], lBufferSize, lDataSize);
		CHECK_HR;
		
		hr = lpColumn->setData(iOffset, &lvNumericalData[0], lDataSize, DssDataOk);
		CHECK_HR;
	}
	else
	{
		hr = lpColumn->setData(iOffset, (char*)irText.c_str(), irText.length() + 1, DssDataOk);
		CHECK_HR;
	}
	
	return S_OK;
}

//junwang, 04/19/2012
bool DSSTabularAttribute::compareElement(DSSBaseElementProxy *ipElement, int iKey)
{
    int index = 0;
    return FindElement(ipElement, &iKey, 1, index);
}

bool DSSTabularAttribute::FindElement(DSSBaseElementProxy *ipElement, const int* ipKey, int iNumKeys, int &orKeyIndex)
{
    EnumDSSElementType lType = ipElement->getElementType();
    
	if (lType != DssElementConcrete)
	{
		//409782, null element
		if (lType == DssElementNULL && mNullElementIndex >= 0)
		{
            for (int i = 0; i < iNumKeys; i++)
            {
                if (ipKey[i] == mNullElementIndex)
                {
                    orKeyIndex = i;
                    return true;
                }
            }
		}
		return false;
	}
    
    int hr = S_OK;
	int lArity = mArity;
	
	if (mvIDFormDataBuffer.empty())
	{
		_ASSERT(mvColumns.size() >= lArity);
		for (int i = 0; i < lArity; i++)
		{
            if (!mvColumns[i])
                return false;
            
			int lSize = mvColumns[i]->getSizeOfData();
			
			if (mvIDFormDataBuffer.size() <= lSize)
				mvIDFormDataBuffer.resize(lSize + 4);	//include the end character of string
		}
	}
    
    if (lArity>0)
    {
        DSSDataFlag lFlag = DssDataOk;
        unsigned char *lpData = NULL;
        //de37687, msun, declare it outside the loop so it will not be deleted during the loop
        MBase::CString lFormVal;
        int lDataSize = 0;
        
        MDataType::DSSBigDecimal lTempBD;
        
        int lnRows = mvColumns[0]->getRowCount();
        int k = 0;
        for (k = 0; k < iNumKeys; k++)
        {
            int iKey = ipKey[k];
            if(iKey < 0 || iKey >= lnRows)
                continue;
            
            int s = 0;
            for (; s<lArity; s++)
            {
                DSSDataColumn *lpColumn = mvColumns[s];
                //de24832,de38105, msun, the datatype of a refcolumn is ambiguous
                if (lpColumn->IsRefColumn())
                {
                    (static_cast<DSSDataRefColumn*>(lpColumn))->syncDataTypeWithDataColumn();
                }
                
                if (!lpData || lArity > 1)
                {
                    lFormVal = static_cast<DSSAttributeElementProxy*> (ipElement)->getValue(s);
                    if(lFormVal=="" || lFormVal == "~!@@~~!@@~")
                        lFlag = DssDataNull;
                    
                    if (lpColumn->getDataType() == DssDataTypeBigDecimal)
                    {
                        //627066, lpData has point to a real object.
                        lTempBD.StrToBigDecimal(lFormVal.c_str());
                        lpData = (unsigned char*) &lTempBD;
                        lDataSize = sizeof(MDataType::DSSBigDecimal);
                    }
                    else if (!DSSAEHelper::IsStringColumn(lpColumn))
                    {
                        int lSize=lpColumn->getSizeOfData();
                        AE_ASSERT(lSize <= mvIDFormDataBuffer.size());
                        
                        hr = DSSAEHelper::ConvertStringToData(lFormVal.c_str(), lpColumn->getDataType(), &mvIDFormDataBuffer[0], lSize, lDataSize);
                        
                        CHECK_HR;
                        lpData = &mvIDFormDataBuffer[0];
                    }
                    else
                    {
                        lpData = (unsigned char*)lFormVal.c_str();
                        lDataSize = lFormVal.length();
                    }
                }//end of if (!lpData && lArity > 1)
                
                if (lpColumn->CompareToData(iKey, lpData, lDataSize, lFlag) != 0)
                    break;
            }//end of for (; s<lArity; s++)
            
            if (s==lArity)
            {
                orKeyIndex = k;
                return true; //found
            }
        }//end of for (k = 0; k < iNumKeys; k++)
    }
	
    return false;
}
int DSSTabularAttribute::createMMapMemory(const char * ipFilePath, bool ibUseNewFile)
{
    int hr = S_OK;
    if (mvColumns.size() > 0)
    {
        for(int i = 0; i < mvColumns.size(); i++)
        {
            if (mvColumns[i])
                hr = mvColumns[i]->createMMapMemory(ipFilePath, ibUseNewFile);
            CHECK_HR;
            
            ibUseNewFile = false;
        }
    }
    return hr;
}

int DSSTabularAttribute::MapFileBuffer()
{
    int hr = S_OK;
    for(int i = 0; i < mvColumns.size(); i++)
    {
        if (mvColumns[i])
            hr = mvColumns[i]->MapFileBuffer();
        CHECK_HR;
    }
    return hr;
}

int DSSTabularAttribute::UnMapFileBuffer()
{
    int hr = S_OK;
    for(int i = 0; i < mvColumns.size(); i++)
    {
        if (mvColumns[i])
            hr = mvColumns[i]->UnMapFileBuffer();
        CHECK_HR;
    }
    return hr;
}

int DSSTabularAttribute::GetAllForm(vector<int>& orvForms)
{
    orvForms.clear();
    
    int lnForm = mvBaseFormID.size();
    for (int i = 0; i < lnForm; i++)
        orvForms.push_back(mvBaseFormID[i]);
    
    return S_OK;
}

int DSSTabularAttribute::getWHNullPosition()
{
    return mWHNullPosition;
}
void DSSTabularAttribute::setWHNullPosition(int iPosition)
{
    mWHNullPosition = iPosition;
}

int DSSTabularAttribute::replaceColumn(vector<DSSDataColumn*> ipNewColumn)
{
    int lnForms = mvColumns.size();
	for (int i = 0; i < lnForms; i++)
	{
		if (mvColumns[i])
		{
			delete mvColumns[i];
		}
        mvColumns[i] = ipNewColumn[i];
	}
    
    return S_OK;
}