/*
 *  DSSRWFieldGroupModelIterator.cpp
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 9/26/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#include "DSSRWFieldGroupModelIterator.h"
#import "DSSRWSectionModelIterator.h"
#import "DSSRWFieldGroupModel.h"
#import "DSSXTabBitVector.h"
#import "DSSField.h"
#import "DSSRWFieldGroupNode.h"
#import "DSSTabularAttribute.h"
#import "DSSDataRefColumn.h"
#import "RWDJoin.h"
#import "DSSCubeDataSlice.h"
#import "DSSXTabViewHelper.h"
#import "DSSXTabView.h"
#import "DSSConsolidation.h"
#import "DSSConsolidationElement.h"
#import "DSSRWCompare.h"
#import "DSSSort_includes.h"
#import "DSSCustomGroup.h"
#import "DSSTabularCustomGroup.h"
#import "DSSStrongPtr.h"
#import "DSSDataElement.h"
#import "DSSDataElements.h"
#import "DSSRWDDICInputControlIterator.h"
#import "DSSRWFieldGroupCalculator.h"
#import "DSSExpNode.h"
#import "DSSCExpression.h"
#import "DSSFunctionServer.h"
#import "MsiCOM/MsiCOM/Msi_atlbase.h"
#import "CSITransaction/DFCCSITransactionBase.h"
#import "DSSOfflineTransactionCSIInfo.h"
//hyan, add for number formatting 
#import "DSSFormatHelper.h"

//hyan,add for autotext
#import "DSSDocumentInstance.h"
#import "DSSDocumentDefinition.h"

#import "DSSDataModelTransactionRWField.h"
#import "DSSCellHandle.h"
#import "DSSDataModelFieldGroupImpl.h"

#import "DSSTabularMetrics.h"
#import "DSSFunctionObject.h"
void DSSRWFieldGroupModelIterator::hEscape(MBase::CString &irText, const char &irChar, const char *ipReplaceStr)
{
	int count = 0;
	int lMax = irText.size();
	int lPos = 0;
	while ((lPos = irText.find(irChar, lPos)) != irText.npos
		   && count < lMax)
	{
		irText.replace(lPos, 1, ipReplaceStr);
		lPos+=strlen(ipReplaceStr);
		count++;
	}
	return;
}

void DSSRWFieldGroupModelIterator::hEncodeXML(MBase::CString &irText)
{
	hEscape(irText, '&', "&amp;");
	
	hEscape(irText, '"', "&quot;");
	
	hEscape(irText, '<', "&lt;");
	
	hEscape(irText, '>', "&gt;");
	
	hEscape(irText, '\'', "&apos;");
	
	return;
}

void DSSRWFieldGroupModelIterator::hEncodeURL(MBase::CString &irText)
{

    size_t lLength = irText.length();
    const char* lpData = irText.c_str();
    MBase::CString lText;
    for (int i = 0; i < lLength; i++, lpData++)
    {
        char lChar = *lpData;
        //do not encode A-Z, a-z, -, _, ., *
        //space will be translated to +
        //rest are translated to %HH where HH is the 2-digit hex representation of the
        //lower 8-bits of the character
        if ((lChar >= 'A' && lChar <= 'Z')
            || (lChar >= 'a' && lChar <= 'z')
            || (lChar >= '0' && lChar <= '9')
            || (lChar == '-')
            || (lChar == '_')
            || (lChar == '.')
            || (lChar == '*') )
        {
            lText += lChar;
        }
        else if (lChar == ' ')
        {
            lText += "%20";
        }
        else
        {
            //first shift 4 bits to right, to get the bit 4-7
            wchar_t lFirst, lSecond;
            lFirst = (lChar >> 4) & 0xF;
            if (lFirst >= 10)
                lFirst = lFirst - 10 + 'A';
            else
                lFirst = lFirst + '0';
            
            //lower 4 bits
            lSecond = lChar & 0xF;
            if (lSecond >= 10)
                lSecond = lSecond - 10 + 'A';
            else
                lSecond = lSecond + '0';
            
            lText += '%';
            lText += lFirst;
            lText += lSecond;
        }
    }
    
    lText += "\0";    
    
    irText = lText;
    
    return;
}

DSSRWFieldGroupModelIterator::DSSRWFieldGroupModelIterator(): mnUnits(0), mnRows(0), mCurrent(0), mBaseCurrent(0), mnFields(0), 
mpIndexTable(NULL), mpBaseIndexTable(NULL), mpFunctionServer(NULL), mbHasTransactionEnabled(false)
{
}

DSSRWFieldGroupModelIterator::~DSSRWFieldGroupModelIterator()
{
    if(mpDataModel)
    {
        mpDataModel->removeNodeKey2Iterator(DSSRWNodeModelIterator::mNodeKey, this);
    }
    
    hCleanUp();
}

int DSSRWFieldGroupModelIterator::Init(DSSRWDataModel *ipDataModel, DSSRWNodeModel *ipNodeModel, DSSRWNode *ipNode, DSSRWSectionModelIterator *ipParentIterator)
{
    int hr = S_OK;
    hr = DSSRWNodeModelIterator::Init(ipDataModel, ipNode, ipNodeModel, ipParentIterator);
    CHECK_HR;
    
     mpDataModel->addNodeKey2Iterator(ipNode->getKey(), this);
    mGlobalLookupVer = mpDataModel->getGlobalLookupVer();
    // jimzhang nov/25/2011 As DSSRWIterator::getRWNode() is called by viewer level, initialize data member of DSSRWIterator here.
    DSSRWIterator::mType = DssIteratorFieldGroup;
    DSSRWIterator::mpObjectContext = ipNode->getObjectContext();
    DSSRWIterator::mNodeKey = ipNode->getKey();
    
    
    DSSRWNodeModelIterator::mNodeKey=ipNode->getKey();
    
    DSSRWFieldGroupNode * lpThisNode = dynamic_cast<DSSRWFieldGroupNode *>(mpNode);
    if (NULL == lpThisNode)
        return E_FAIL;  
    
    DSSFields *lpFields = lpThisNode->getFields();
    CHECK_PTR(lpFields);    
    mnFields = lpFields->Count();
    
    hr = ReInit();
    return hr;
}

int DSSRWFieldGroupModelIterator::ReInit()
{
    int hr = S_OK;
    
    hReInitMembers();
    
    bool lbIsModelUpdated = false;
    hr = mpDataModel->updateNodeModel(DSSRWIterator::mNodeKey, lbIsModelUpdated);
    CHECK_HR;
        
    DSSRWNodeModelIterator::mpNodeModel = mpDataModel->getNodeModel(DSSRWIterator::mNodeKey);
    CHECK_PTR(DSSRWNodeModelIterator::mpNodeModel);
    
    DSSRWFieldGroupModel *lpFGModel = dynamic_cast<DSSRWFieldGroupModel *>(DSSRWNodeModelIterator::mpNodeModel);
    CHECK_PTR(lpFGModel);
    
    // 5/28/2012 tehe: load the field group model
    bool lbDataLoaded = false;
    hr = lpFGModel->EnsureModelData(&lbDataLoaded);
    CHECK_HR;
    
    if(mGlobalLookupVer != mpDataModel->getGlobalLookupVer())
        mGlobalLookupVer = mpDataModel->getGlobalLookupVer();
    
    vector< vector<Int32> > lvGroupByElements;
    hr = DSSRWNodeModelIterator::mpParentIterator->getGroupByElements(lvGroupByElements);
    CHECK_HR;
    
    // If "lpFGModel->IsInitlaLoading" is ture, current stage is in partial/initial loading. In addition, 
    //      field&link text and threshold ID has been loaded in FG model, and there is no view dataset.
    if (lpFGModel->IsInitlaLoading)
    {
        mnRows = lpFGModel->mRowCount;
        mnDisplayRows = mnRows;
        mnFields = lpFGModel->mFieldCount;
        
        mvRows.resize(mnDisplayRows);
        for (int i = 0; i < mnDisplayRows; ++i)
            mvRows[i] = i;
        mCurrent = 0;
        mBaseCurrent = 0;
        
        int lnCells = mnDisplayRows*mnFields;
        mvFeildsText.resize(lnCells);
        mvThresholdID.resize(lnCells);
        mvLinkFieldText.resize(lnCells);
        memset(&mvThresholdID[0], 0xff, sizeof(int) * lnCells);
        memset(&mvLinkFieldText[0], 0, sizeof(FieldLink*) * lnCells);
        
        // 1. populate field text
        vector<MBase::CString>& lvFeildsText = lpFGModel->mvFieldText;
        //tliao, 2/14/2014, 844549, check if it is "NGB is empty" case
        //tliao, 3/10/2014, 857690, for empty CGB, we use the normal workflow: show field texts directly
        int lNGBLevel = DSSRWNodeModelIterator::mpParentIterator->getLevel();
        
        if (lnCells > 0 && lNGBLevel >= 0 && lvGroupByElements.size() > 0 && lvGroupByElements[0].size() == 0)
        {
            //if yes, we only show static text
            DSSRWFieldGroupNode * lpThisNode = dynamic_cast<DSSRWFieldGroupNode *>(mpNode);
            
            DSSFields *lpFields = lpThisNode->getFields();
            CHECK_PTR(lpFields);
            
            MBase::CString emptyString = "";
            for (int i = 0; i < mnFields; i ++)
            {
                DSSField *lpField = lpFields->Item(i);
                if (lpField->getType() == DssRWFieldLabel)
                {
                    for (int j = 0; j < mnDisplayRows; j++)
                    {
                        mvFeildsText[j*mnFields+i].assign(lvFeildsText[j*mnFields+i]);
                    }
                }
            }
        }
        else
        {
            for (int i = 0; i < lnCells; ++i)
                mvFeildsText[i].assign(lvFeildsText[i]);
        }
        
        // 2. populate threshold
        vector<int>& lvThresholdID = lpFGModel->mvThreshold;        
        vector<int>& lvThresholdFieldOrder = lpFGModel->mvThresholdVectorID; 
        
        int lnFields = lvThresholdFieldOrder.size();        //749400, lpFGModel->mvThreshold will be empty if mnRows = 0
        int lnThresholdField = 0;
        for (int iField = 0; iField < lnFields; ++iField)
            if (0 <= lvThresholdFieldOrder[iField])
                lnThresholdField++;
        
        for (int iField = 0; iField < lnFields; ++iField)
        {
            int lThresholdFieldOrder = lvThresholdFieldOrder[iField];
            if (0 <= lThresholdFieldOrder)
            {                
                for (int iRow = 0, lOff1 = iField, lOff2 = lThresholdFieldOrder; iRow < mnDisplayRows; 
                     ++iRow, lOff1 += mnFields, lOff2 += lnThresholdField)
                {
                    mvThresholdID[lOff1] = lvThresholdID[lOff2];
                }
            }
        }
        
        // 3. populate field link(including link type and text)
        vector<int> &lvHasLink = lpFGModel->mvLinkTextVectorID;
        int lnLinkField = 0;
        for (int iField = 0; iField < lvHasLink.size(); ++iField)
            if (0 <= lvHasLink[iField])
                ++lnLinkField;
                
        vector<vector<int> > &lvvLinkType = lpFGModel->mvvLinkTextType;
        vector<vector<string> > &lvvLinkText = lpFGModel->mvvLinkText;
        
        for (int iField = 0, iLinkField = 0; iField < lnFields; ++iField)
        {
            if (0 <= lvHasLink[iField])
            {
                for (int iRow = 0, lOff1 = iField, lOff2 = iLinkField; iRow < mnDisplayRows;
                     ++iRow, lOff1 += mnFields, lOff2 += lnLinkField)
                {
                    int lnLink = lvvLinkType[lOff2].size();
                    
                    FieldLink *lpFieldLink = new FieldLink(lnLink);
                    if (! lpFieldLink)
                        continue;
                    
                    for (int iLink = 0; iLink < lnLink; ++iLink)
                    {
                        lpFieldLink->mtype[iLink] = (EnumDSSRWLinkType)lvvLinkType[lOff2][iLink];
                        MBase::CString *lpLinkText = new MBase::CString();
                        lpLinkText->assign(lvvLinkText[lOff2][iLink]);
                        lpFieldLink->mtext[iLink] = lpLinkText;
                    }
                    
                    mvLinkFieldText[lOff1] = lpFieldLink;
                }
                
                ++iLinkField;
            }
        }
        
        return S_OK;
    }
       
    hr = lpFGModel->hAdjustConflictGBElements(lvGroupByElements);
    CHECK_HR;    
            
    vector<Int32> lvSortUnits;
    JoinedIndexTable* lpIndexTable = NULL;
    hr = lpFGModel->hSliceAndJoin(lpFGModel->mvGroupByUnits, lvGroupByElements, false, &lpIndexTable, true, lvSortUnits);
    CHECK_HR;

    mvSortUnits.assign(lvSortUnits.begin(), lvSortUnits.end());    
    EnumDSSFieldGroupDataLevel lDataLevel = lpFGModel->mDataLevel;    
    vector<DSSRTTemplateInfo*>& lpRTTemplateInfo = lpFGModel->mvpRTTemplateInfo;
    
    bool lfHasMetric = false;
	vector<Int32> lvRTTemplateWithMetrics;        
	for ( Int32 i = 0; i < lpRTTemplateInfo.size(); ++i)
	{
		GUID lID;
		hr = lpRTTemplateInfo[i]->GetReportID(&lID);
		CHECK_HR;      
        
		if (lpRTTemplateInfo[i]->mvMetricList.size() > 0)
		{
			lfHasMetric = true;
			lvRTTemplateWithMetrics.push_back(i);
		}
	}
    
	if (lpIndexTable)
	{
		mpIndexTable = lpIndexTable;
        mnRows = mpIndexTable->mnRow; 
        Int32 lnUnit = mpIndexTable->mvColumnMap.size();       
		if (mnRows == 0 && lnUnit == 0 && lfHasMetric)
		{
			//ctang 2011-5-25 if there is only metric in header section, the join tabular data is empty, but we need one row to display
			if(lDataLevel != DssFieldGroupDataLevelDefaultReport)
				mnRows = 1;
		}
	}
	else
	{
		// jimzhang May/14/2012 
        //      (1) When document detail section contians only static field(s), execution will reach here.
        //      At this point, there is no view dataset for this FG, and just return one row.
        //      (2) For non-detail section, just set "mnRows" to 1 as server side.
        mnRows = 1;
	}
    
	mvGroupByElements = lvGroupByElements;    
	mnUnits = 0;
	if(mpIndexTable) //ctang 2011-4-15 mnUnit should be the unit count in join tabular data
	{
		mnUnits = mpIndexTable->mvColumnMap.size();                        
	}
	else
	{
		mnUnits++; // add metrics
	}
    
    if (mnRows > 0)
	{
		mvRows.resize(mnRows);
		for (Int32 i = 0; i < mnRows; i++)
			mvRows[i] = i;
		
		mnRows = mvRows.size();
	}

    if (mnRows > 1)
	{
        //TQMS640804,hyan,2012/08/07
        //follow server's logic, for Not_Detail section, we donot allow null on top
        bool lbNullOnTop=false;
        
        if(DssFieldGroupDataLevelDefaultReport==lDataLevel)
            lbNullOnTop=mpDataModel->GetDisplayNullOnTop();
        
		DSSRWFieldGroupCompare lCmp;
		hr = lCmp.Init(this, lbNullOnTop, lvSortUnits);
		CHECK_HR;
        
        // jimzhang dec/29/2011 ToDo Implement below logic of server. There is no method to implement GetUserLocale() on iPad currently.
        /*
		//set user data locale for lCmp
		Int32 lLocale = DssLocaleNeutral;
		hr = mpDataModel->GetUserLocale(&lLocale);
		CHECK_HR;
        
		lCmp.InitLocale(lLocale);
        
		// lwang, 08/14/2007, TQMS 276457: call _xtab_sort_mcm() to use MCM governed buffer 
		MBase::Buffer* lpBuffer = MContractMgr::CreateGovernedBuffer(1, 1, _T("CDSSRWFieldGroupModelIterator"));
		if(!lpBuffer)	// if unsuccessful, we assume it is 2-tier and we create normal buffer
			lpBuffer = MBase::CreateBuffer(1, 1);
		MBase::BufferPtr lBufferMCMPtr(lpBuffer);*/
        
        // jimzhag dec/29/2011 Below two lines of code is dummy.
        Int32 lLocale = 0;
        lCmp.InitLocale(lLocale);
        
        //junwang, 05/08/2012, peformance optimization
        if (lDataLevel == DssFieldGroupDataLevelDefaultReport) 
            hr =  _xtab_sort(&mvRows[0], &mvRows[0] + mnRows, lCmp);
        else
            hr = _xtab_first_n(&mvRows[0], &mvRows[0] + mnRows, lCmp, 1, false);
        
		CHECK_HR;
	}
    
    //junwang, 2012/03/20, #564367
    if(mnRows<=0)
    {
        if (DssFieldGroupDataLevelDefaultReport != lDataLevel)
            mnRows++;
        if(mnRows>0)
        {
            mvRows.resize(mnRows);
            for (int i=0; i<mnRows; i++)
            {
                mvRows[i] = i;
            }
        }
    }
    
    // jimzhang Feb/09/2012 Don't return error here, even if there may be error in hReInitFieldsMapping() and hReInitFieldData() 
    //  and hBuildTransactionColumnMap(). For users, it's better to display partial results with some cells missing or un-formatted
    //  than display nothing. In addition, displaying partial results with some cells missing or un-formatted is more helpful for debugging.
    
    //if (lbIsModelUpdated || lbDataLoaded) // jimzhang, Feb/6/2013, TQMS 715018, always call hReInitFieldsMapping(), since "lbIsModelUpdated" and "lbDataLoaded" checking is invalid now.
        hReInitFieldsMapping();
    
    if (mpDataModel->GetTrimEmptySections() && lpIndexTable && DssFieldGroupDataLevelDefaultReport == lDataLevel && lvRTTemplateWithMetrics.size() > 0)
        mvIsTrimed.assign(mnRows, true);
    
    hReInitFieldData();
    
    if (mpDataModel->GetTrimEmptySections() && lpIndexTable && DssFieldGroupDataLevelDefaultReport == lDataLevel && lvRTTemplateWithMetrics.size() > 0)
    {        
        vector<Int32>::iterator iterRows = mvRows.begin();
        vector<int>::iterator iterThreshold = mvThresholdID.begin();
        vector<MBase::CString>::iterator iterText = mvFeildsText.begin();
        vector<FieldLink*>::iterator iterLink = mvLinkFieldText.begin();
        
        for (int iRow = 0; iRow < mnRows; ++iRow)
        {
            if (mvIsTrimed[iRow])
            {
                iterRows = mvRows.erase(iterRows);
                iterThreshold = mvThresholdID.erase(iterThreshold, iterThreshold + mnFields);
                iterText = mvFeildsText.erase(iterText, iterText + mnFields);
                iterLink = mvLinkFieldText.erase(iterLink, iterLink + mnFields);
            }
            else
            {
                ++iterRows;
                iterThreshold += mnFields;
                iterText += mnFields;
                iterLink += mnFields;
            }
        }
            
        mnDisplayRows = mnRows = mvRows.size();
    }
    
    hBuildTransactionColumnMap(mpNode);
    
    //fali, 935809, only build offline mapping for smart client
    if (mpDataModel != NULL && mpDataModel->getObjectContext()->IsSmartClient())
    {
         hBuildOfflineTransactionColumnMap(mpNode);
    }
    
    hBuildTransactionWriteBackColumnMap(mpNode);
    
    mbHasTransactionEnabled = !mvMapToWBColumn.empty() || !mvOfflineMapToWBColumn.empty();
    
    if (mbHasTransactionEnabled)
    {
        DSSDataModelTransactionRWField *lpTrxModel = dynamic_cast<DSSDataModelTransactionRWField *>(lpFGModel->getTransactionDataModel());        
        if (lpTrxModel)
        {
            int lBlockOff = lpTrxModel->getBlockOffByGBElements(mvGroupByElements, mnRows);
            lpTrxModel->EnsureRawData(this, lBlockOff, mnRows);
        }
        
        if (! mmTrxFieldDataType.size())
            hReInitTrxFieldDataType(); // get data type and date size of editable field
    }
    
    //fali, 935809, only build offline mapping for smart client
    if (mpDataModel != NULL && mpDataModel->getObjectContext()->IsSmartClient())
    {
        hSetOfflineTransactionMapping();
    }
    
    // jzeng 09/22/15 reinit ddic iterator
    ReInitInputControlIterator();
    
    return hr;
}

int DSSRWFieldGroupModelIterator::hBuildTransactionWriteBackColumnMap(DSSRWNode * ipNode)
{
    if (! mmField2WBForm.empty()) //map has been built
		return S_OK;
	
	if (!ipNode)
		return E_POINTER;
	
	DSSRWFieldGroupNode * lpThisNode = static_cast<DSSRWFieldGroupNode*> (ipNode);
//	DSSTransactionReport *lpTargetReport = lpThisNode->getTransactionReport();
//	CHECK_PTR(lpTargetReport);
	
	DSSDataModelTransaction *lpTransactionModel = getTransactionDataModel();
	if (!lpTransactionModel)
		return S_OK;				//transaction report is not enabled.
		
    mmField2WBForm.resize(mnFields);
	
	int i = 0;	
    for (i = 0; i < mnFields; ++i)
        mmField2WBForm[i] = -1;
	
	DSSFields *lpFields = lpThisNode->getFields();
	int lnFields = lpFields->Count();
	
	for (i = 0; i < lnFields; i ++)
	{
		DSSField *lpField = lpFields->Item(i);
		if (lpField->IsMappedForTransaction() || lpField->IsMappedForOfflineTransaction())
		{
            DSSRWControl *lpControl = lpField->getInputControl();
            if (lpControl)
            {
                DSSAttributeFormWrappers* lpWBForms = lpControl->getWriteBackForms();
                if(lpWBForms && lpWBForms->Count()>0)
                {
                    DSSAttributeFormWrapper* lpWBForm = lpWBForms->Item(0);
                    if(lpWBForm)
                    {
                        int lWBBaseFormID = lpWBForm->getBaseFormID();
                        GUID lControlObjectID = lpControl->getSourceID();
                        DSSObjectInfo *lpObjectInfo = mpDataModel->getObjectContext()->FindObjectH(lControlObjectID);
                        if(!lpObjectInfo)
                            continue;
                        DSSTabularAttribute * lpTabularAttribute = mpDataModel->GetTabularAttributeByGUID(lpObjectInfo->getID());
                        int lColumnID = -1;
                        if (lpTabularAttribute && (S_OK == lpTabularAttribute->FindFormWithLocale(0, lWBBaseFormID, 0, lColumnID)))
                        {
                            //we got a match, same attribute and same base form
                            mmField2WBForm[i] = lWBBaseFormID;
                        }
                    }//end of if(lpWBForm)
                }
            }
		}
    }
    return S_OK;
}


int DSSRWFieldGroupModelIterator::hReInitFieldData()
{
    int hr = S_OK;
    
    DSSRWFieldGroupModel *lpFGModel = static_cast<DSSRWFieldGroupModel *>(DSSRWNodeModelIterator::mpNodeModel);
    CHECK_PTR(lpFGModel);
    
    // jimzhang May/15/2012 For dynamic field(s) on header/footer section, only one row is displayed. So set "lnDisplayRows" to 1 can 
    //      avoid generating display text, threshold, and linked text for those rows that are not displayed.
    mnDisplayRows = DssFieldGroupDataLevelDefaultReport == lpFGModel->mDataLevel ? mnRows : 1;
    
    int lnCells = mnDisplayRows*mnFields;
    
    mvFeildsText.clear();
    mvFeildsText.resize(lnCells);
    
    mvThresholdID.clear();
    mvThresholdID.resize(lnCells);
    
    //hyan,2012/03/19
    mvLinkFieldText.clear();
    mvLinkFieldText.resize(lnCells);
    
    for (int i = 0; i < lnCells ; ++i)
    {
        mvThresholdID[i] = -1;
        mvLinkFieldText[i] = NULL;
    }
    
    bool lbHasError = false;
    
    for (int i = 0; i < mnFields; ++i)
    {
        DSSField *lpField = NULL;
        hr = hFindField(i, &lpField);
        if (S_OK != hr)
        {
            lbHasError = true;
            continue;
        }
        
        hr = hReInitThreshold(i, lpField);
        if (S_OK != hr)
            lbHasError = true;
        
        hr = hReInitFieldTextAndLink(i, -1, lpField, NULL, NULL);
        if (S_OK != hr) 
            lbHasError = true;        
    }
    
    mCurrent = 0;
    
    return lbHasError ? S_FALSE : S_OK;
}

// jimzhang Feb/17/2012 
// Description:
//      (1) hReInitMembers() will intialize data members of DSSRWFieldGroupModelIterator in ReInit() step.
//      On server side, when making a change in selector()(e.g. NGB, CGB, UC), a new FieldGroup iterator will be created,
//      and its data members will be initialized by constuctor. On ipad side, ReInit() of FieldGroup iterator will be called
//      when makeing a change in selector, instead of newing a new iterator. So on iPad side, data members of FG iterator 
//      should be initialized mamualy on ReInit() step.
//      (2) Maybe more data members should be intialized here.
void DSSRWFieldGroupModelIterator::hReInitMembers()
{
    mnUnits = 0;
    
    mvRows.clear();
    mnRows = 0;
    mnDisplayRows = 0;
    mCurrent = -1;
    
    mBaseCurrent = -1;
    
    mvSortUnits.clear();
    
    if (mpIndexTable)
    {
        delete mpIndexTable;
        mpIndexTable = NULL;
    }
    if (mpBaseIndexTable)
    {
        delete mpBaseIndexTable;
        mpBaseIndexTable = NULL;
    }
    
    mvGroupByElements.clear();
    
    mvFeildsText.clear();
    //mmCellIndex2RawData.clear();
	mvThresholdID.clear();
    
    int lnLinkFieldText = mvLinkFieldText.size();
    for (int i = 0; i < lnLinkFieldText; ++i)
        delete mvLinkFieldText[i];
    mvLinkFieldText.clear();
}

int DSSRWFieldGroupModelIterator::hReInitFieldsMapping()
{
    int hr = S_OK;
    
    bool lbHasError = false;
    
    mmFieldToRTTIndex.clear();
    mmFieldToMetricIndex.clear();
    
    for (int i = 0; i < mnFields; ++i)
    {
        DSSField *lpField = NULL;
        hr = hFindField(i, &lpField);
        CHECK_HR;
        
        hr = hReInitFieldMapping(lpField);
        if (S_OK != hr)
            lbHasError = true;
    }
    
    hr = lbHasError ? S_FALSE : S_OK;
    
    return hr;
}

// jimzhang Feb/13/2012 
// Description: 
//      The function of hReInitFieldMapping() is similar to hInitValues() in server side.
//      hReInitFieldMapping() will construct mapping info for Field at Init() (and ReInit(), 
//      depend on if node model is updated) step of FG iterator,
//      e.g. mapping Field to metic index if this Field contains a metric object. 
//      These mapping info serves as cache at every ReInit() step.
int DSSRWFieldGroupModelIterator::hReInitFieldMapping(DSSField *ipField)
{
    int hr = S_OK;
    
    std::string lKey = ipField->getKey();
    EnumDSSRWFieldType lType = ipField->getType();
    switch (lType)
    {
        case DssRWFieldObjectValue:
        {
            DSSObjectInfo* lpObjInfo = ipField->getFieldObject();
            CHECK_PTR(lpObjInfo);
            
            GUID lObjID = lpObjInfo->getID();
            EnumDSSObjectType lObjType = lpObjInfo->getType();            
            switch (lObjType)
            {
                case DssTypeMetric:
                {
                    DSSRWFieldGroupModel *lpFGModel = static_cast<DSSRWFieldGroupModel *>(DSSRWNodeModelIterator::mpNodeModel);
                    CHECK_PTR(lpFGModel);
                    
                    GUID lReportGUID = ipField->getDataSourceObjectID();
                    vector<DSSRTTemplateInfo*> &lvRTT = lpFGModel->mvpRTTemplateInfo; 
                    int lnRTT = lvRTT.size();
                    int lRTTIndex = 0;
                    for (lRTTIndex = 0; lRTTIndex < lnRTT; ++lRTTIndex)
                    {
                        if (lReportGUID == lvRTT[lRTTIndex]->mReportID)
                            break;
                    }
                    
                    if (lRTTIndex == lnRTT)
                        return E_FAIL;
                                        
                    DSSViewDataSet *lpViewDataset = lvRTT[lRTTIndex]->mViewPtr;
                    int lnMetric = lpViewDataset->getTabularData()->getMetricsCount();
                    int lMetricIndex = 0;
                    for (lMetricIndex = 0; lMetricIndex < lnMetric; ++lMetricIndex)
                    {
                        if (lObjID == lpViewDataset->getTabularData()->getMetricID(lMetricIndex))
                            break;
                    }
                    
                    if (lMetricIndex == lnMetric)
                        return E_FAIL;
                    
                    mmFieldToRTTIndex.insert(std::map<MBase::CString, Int32>::value_type(lKey, lRTTIndex));
                    mmFieldToMetricIndex.insert(std::map<MBase::CString, Int32>::value_type(lKey, lMetricIndex));
                }
                    break;
                    
                default:
                    break;
            }
        }
            break;
            
        case DssRWFieldConcatenation:
        {
            DSSFields* lpChildren = ipField->getChildFields();
            if (NULL != lpChildren)
            {
                int lnChildren = lpChildren->Count();
                for (int iChild = 0; iChild < lnChildren; ++iChild)
                {
                    DSSField* lpChild = lpChildren->Item(iChild);
                    CHECK_PTR(lpChild);
                    
                    hr = hReInitFieldMapping(lpChild);
                    CHECK_HR;
                }
            }
        }
            break;
            
        default:
            break;
    }
    
    return hr;
}

// get data type and data size of editable field, and save them in mmTrxFieldDataType and mmTrxFieldDataSize
int DSSRWFieldGroupModelIterator::hReInitTrxFieldDataType()
{
    int hr = S_OK;
    
    DSSRWFieldGroupNode *lpThisNode = dynamic_cast<DSSRWFieldGroupNode *>(mpNode);
    if (NULL == lpThisNode)
        return E_FAIL;
    
    DSSFields *lpFields = lpThisNode->getFields();
    CHECK_PTR(lpFields);    
    
    DSSRWFieldGroupModel *lpFGModel = dynamic_cast<DSSRWFieldGroupModel *>(DSSRWNodeModelIterator::mpNodeModel);
    CHECK_PTR(lpFGModel);
    
    DSSDataModelTransactionRWField *lpTrxModel = dynamic_cast<DSSDataModelTransactionRWField *>(lpFGModel->getTransactionDataModel());
    CHECK_PTR(lpTrxModel);
    
    for (int iField = 0; iField < mnFields; ++iField)
    {
        if (mvMapToWBColumn[iField] < 0 && (mvOfflineMapToWBColumn.size() == 0 ||  mvOfflineMapToWBColumn[iField] < 0))
            continue;
        
        EnumDSSDataType lTrxColDataType = lpTrxModel->getColumnDataType(mvMapToWBColumn[iField]);
        int lTrxColDataSize = 0;
        DSSAEHelper::SizeOfType(lTrxColDataType, &lTrxColDataSize);
        
        mmTrxFieldDataType[iField] = lTrxColDataType;
        mmTrxFieldDataSize[iField] = lTrxColDataSize;
        
        if(lTrxColDataType != DssDataTypeMissing)
            continue;
        
        DSSField *lpField = NULL;
        hr = hFindField(iField, &lpField);
        CHECK_HR;
        
        if (DssRWFieldConcatenation != lpField->getType())
            continue;
        
        DSSFields *lpChildren = lpField->getChildFields();
        if (! lpChildren || 1 != lpChildren->Count())
            continue;
        
        DSSField *lpChild = lpChildren->Item(0);
        CHECK_PTR(lpChild);
        
        EnumDSSRWFieldType lChildFieldType = lpChild->getType();
        if (DssRWFieldObjectValue == lChildFieldType)
        {
            DSSObjectInfo *lpObjInfo = lpChild->getFieldObject();
            CHECK_PTR(lpObjInfo);
            
            EnumDSSObjectType lObjType = lpObjInfo->getType();
            if (DssTypeMetric == lObjType)
            {
                DSSViewDataSet *lpViewDataset = NULL;
                GUID lReportGUID = lpChild->getDataSourceObjectID();
                
                const vector<DSSRTTemplateInfo*> &lrvpRTTemplateInfo = lpFGModel->mvpRTTemplateInfo;
                int lnRTT = lrvpRTTemplateInfo.size();
                for (int iRTT = 0; iRTT < lnRTT; ++iRTT)
                {
                    if (lrvpRTTemplateInfo[iRTT]->mReportID == lReportGUID)
                    {
                        lpViewDataset = lrvpRTTemplateInfo[iRTT]->mViewPtr;
                        break;
                    }
                }
                
                if (! lpViewDataset)
                    continue;
                
                DSSCube *lpCube = lpViewDataset->GetCube();
                CHECK_PTR(lpCube);
                
                DSSCalculationPlan *lpCalcPlan = lpViewDataset->getCalculationPlan();
                CHECK_PTR(lpCalcPlan);
                
                EnumDSSDataType lpDataType = DssDataTypeReserved;
                int lDataSize = 0;
                
                GUID lMetricGUID = lpObjInfo->getID();
                int lnLevels = lpCalcPlan->getNumLevels();
                for (int iLevel = 0; iLevel < lnLevels; ++iLevel)
                {
                    bool lbIsCalculated = false;
                    int lSliceID = -1;
                    hr = lpCalcPlan->getMetricSliceID(iLevel, lMetricGUID, &lbIsCalculated, &lSliceID);
                    if (S_OK != hr)
                        continue;
                    
                    DSSCubeDataSlice *lpCubeSlice = lpCube->getSliceByID(lSliceID);
                    if (! lpCubeSlice)
                        continue;
                    
                    DSSDataColumn *lpColumn = lpCubeSlice->getColumn();
                    if (! lpColumn)
                        continue;
                    
                    lpDataType = lpColumn->getDataType();
                    lDataSize = lpColumn->getSizeOfData();
                    break;
                }
                
                mmTrxFieldDataType[iField] = lpDataType;
                mmTrxFieldDataSize[iField] = lDataSize;
            }
            else if (DssTypeAttribute == lObjType)
            {
                GUID lAttrGUID = lpObjInfo->getID();
                DSSTabularAttribute *lpTabularAttr = mpDataModel->GetTabularAttributeByGUID(lAttrGUID);
                CHECK_PTR(lpTabularAttr);
                
                int lBaseFormID = -1;
                if (mmField2WBForm.size() > 0 && mmField2WBForm[iField] != -1)
                {
                    lBaseFormID = mmField2WBForm[iField];
                    DSSRWControl *lpControl = lpField->getInputControl();
                    GUID lControlObjectID = lpControl->getSourceID();
                    DSSObjectInfo *lpObjectInfo = mpDataModel->getObjectContext()->FindObjectH(lControlObjectID);
                    lpTabularAttr = mpDataModel->GetTabularAttributeByGUID(lpObjectInfo->getID());
                }
                else
                {
                    GUID lFormGUID = GUID_NULL;
                    
                    DSSAttributeFormWrappers *lpDispFormWrappers = lpChild->getAttributeForms();
                    int lnDispForms = lpDispFormWrappers->Count();
                    if (lnDispForms > 0)
                    {
                        if (1 != lnDispForms)
                            continue;
                        
                        DSSAttributeFormWrapper *lpAttrFormWrapper = lpDispFormWrappers->Item(0);
                        CHECK_PTR(lpAttrFormWrapper);
                        
                        lBaseFormID = lpAttrFormWrapper->getBaseFormID();
                        lFormGUID = lpAttrFormWrapper->getID();
                    }
                    else
                    {
                        DSSGlobalLookUpTable *lpLookUp = dynamic_cast<DSSGlobalLookUpTable*>(lpTabularAttr);
                        CHECK_PTR(lpLookUp);
                        
                        vector<int> lvUnionForms;
                        hr = lpLookUp->GetDisplayFormUnionForControl(lvUnionForms);
                        CHECK_HR;
                        
                        lnDispForms = lvUnionForms.size();
                        if (1 != lnDispForms)
                            continue;
                        
                        lBaseFormID = lvUnionForms[0];
                        
                        DSSAttribute *lpAttr = dynamic_cast<DSSAttribute *>(lpObjInfo);
                        CHECK_PTR(lpAttr);
                        
                        DSSAttributeFormWrappers *lpFormWrappers = lpAttr->getFormWrappers();
                        CHECK_PTR(lpFormWrappers);
                        
                        int lnFormWrappers = lpFormWrappers->Count();
                        for (int iForm = 0; iForm < lnFormWrappers; ++iForm)
                        {
                            DSSAttributeFormWrapper *lpFormWrapper = lpFormWrappers->Item(iForm);
                            if (lpFormWrapper->getBaseFormID() == lBaseFormID)
                            {
                                lFormGUID = lpFormWrapper->getID();
                                break;
                            }
                        }
                    }
                    
                    if (lBaseFormID < 0 || GUID_NULL == lFormGUID)
                        continue;
                }
                
                int lColumnID = 0;
                hr = lpTabularAttr->FindFormWithLocale(0, lBaseFormID, 0, lColumnID);
                CHECK_HR;
                
                DSSDataColumn *lpColumn = NULL;
                hr = lpTabularAttr->Item(lColumnID, &lpColumn);
                CHECK_HR;
                
                if(lpColumn)
                {
                    mmTrxFieldDataType[iField] = lpColumn->getDataType();
                    mmTrxFieldDataSize[iField] = lpColumn->getSizeOfData();
                }else
                {
                    mmTrxFieldDataType[iField] = DssDataTypeReserved;
                    mmTrxFieldDataSize[iField] = 0;
                }
            }
        } // END: if (DssRWFieldObjectValue == lChildFieldType)
        else if (DssRWFieldLabel == lpChild->getType())
        {
            mmTrxFieldDataType[iField] = DssDataTypeVarChar;
        }        
    }
    
    return hr;
}

int DSSRWFieldGroupModelIterator::hReInitFieldTextAndLink(int iMainFieldIndex, int iLinkLocalIndex, DSSField *ipField, DSSFormat *ipFormat, string *iParentKey)
{
    int hr = S_OK;
    bool lbHasError = false;
    
    std::string lKey = ipField->getKey();
    EnumDSSRWFieldType lType = ipField->getType();
    switch (lType)
    {
        case DssRWFieldShape:
        case DssRWFieldLine:
        case DssRWFieldLabel:
        {
            //de37305, msun, 6/22/2016, lpText will pointers to a temporary memory
            //const char *lpText = ipField->getText().c_str();
            
            int lnCells = mnDisplayRows * mnFields;
            for (int lOff = iMainFieldIndex; lOff < lnCells; lOff += mnFields)
            {
                if (-1 == iLinkLocalIndex)
                    mvFeildsText[lOff].append(ipField->getText());
                else
                {
                    string lpLinkText = ipField->getText();
                    hGetEncodedLinkText(lpLinkText, mvLinkFieldText[lOff]->mtext[iLinkLocalIndex], lType, mvLinkFieldText[lOff]->mtype[iLinkLocalIndex], DssAutoTextReserved);
                }
            }
        }
            break;
            
        case DssRWFieldImage:
        {
            DSSFields* lpChildren = ipField->getLinkChildFields(DssRWLinkImage);
            if (lpChildren)
            {
                int lnChildren = lpChildren->Count();
                for (int iChild = 0; iChild < lnChildren; ++iChild)
                {
                    DSSField* lpChild = lpChildren->Item(iChild);
                    CHECK_PTR(lpChild);
                    
                    hr = hReInitFieldTextAndLink(iMainFieldIndex, -1, lpChild, ipFormat, iParentKey);
                    CHECK_HR;
                }
            }
            
            vector<int> lvLinkFieldIndex;            
            for (int i = 1; i < DssRWLinkLastOne; ++i)
            {
                // Text of DssRWLinkImage type link has been saved in mvFeildsText for displaying this image, so it needn't be saved in mvLinkFieldText again.
                if (DssRWLinkImage == i)
                    continue;
                
                DSSFields *lpChildren = ipField->getLinkChildFields(static_cast<EnumDSSRWLinkType>(i));
                if (lpChildren)
                    lvLinkFieldIndex.push_back(i);
            }
            int lnFieldLinks = lvLinkFieldIndex.size();
            
            if (! lnFieldLinks)
                break;
            
            for (int iRow = 0, lOff = iMainFieldIndex; iRow < mnDisplayRows; ++iRow, lOff += mnFields)
            {
                FieldLink *lpFieldLink = new FieldLink(lnFieldLinks);
                for (int i = 0; i < lnFieldLinks; ++i)
                {
                    lpFieldLink->mtype[i] = (EnumDSSRWLinkType)(lvLinkFieldIndex[i]);
                    lpFieldLink->mtext[i] = new MBase::CString;
                }
                
                mvLinkFieldText[lOff] = lpFieldLink;
            }
            
            for (int i = 0; i < lnFieldLinks; ++i)
            {
                DSSFields *lpChildren = ipField->getLinkChildFields(static_cast<EnumDSSRWLinkType>(lvLinkFieldIndex[i]));
                if (lpChildren)
                {
                    DSSFormat *lpFormat=ipField->getFormat();
                    string lParentKey=ipField->getKey();
                    
                    int lnChildren = lpChildren->Count();
                    for (int iChild = 0; iChild < lnChildren; ++iChild)
                    {
                        DSSField* lpChild = lpChildren->Item(iChild);
                        CHECK_PTR(lpChild);
                        
                        hr = hReInitFieldTextAndLink(iMainFieldIndex, i, lpChild, lpFormat, &lParentKey);
                        CHECK_HR;
                    }
                }
            }
        }
            break;
            
        case DssRWFieldCalculation:
        {
            // Check if expression of calculation field is resolved. If not, just return expression text.
            bool lfIsCalculationResolved = ipField->getCalculationResolved();
            if (! lfIsCalculationResolved)
            {
                // jimzhang Jul/27/2012 ToDo. Server side uses CDSSField::mValue as expression text, but iPad side
                //      use DSSField::mText as expression text. "mValue" and "mText" are not inconsistent, e.g. when "mValue"
                //      is "{Profit+Revenue}", "mText" is "Profit+Revenue". 
                std::string lExprText = ipField->getText();
                mCurrent = 0;
                for (int lOff = mCurrent * mnFields + iMainFieldIndex; mCurrent < mnDisplayRows; ++mCurrent, lOff += mnFields)
                {
                    if (-1 == iLinkLocalIndex)
                        mvFeildsText[lOff].append(lExprText.c_str());
                    else
                        hGetEncodedLinkText(lExprText, mvLinkFieldText[lOff]->mtext[iLinkLocalIndex], lType, mvLinkFieldText[lOff]->mtype[iLinkLocalIndex], DssAutoTextReserved);
                }
                return S_OK;
            }
            
            DSSCExpression *lpExp = ipField->getExpression();
            CHECK_PTR(lpExp);
            
            DSSExpNode *lpExpNode = lpExp->Root();
            CHECK_PTR(lpExpNode);
            
            if (! mpFunctionServer)
            {
                mpFunctionServer = new DSSFunctionServer();
                CHECK_PTR(mpFunctionServer);
            }
            
            // jimzhang May/23/2012 begin Cache format objects when threshold ID is -1, 0, 1, 2, ... <lnThresholds-1>
            DSSField *lpMainField = NULL;
            hFindField(iMainFieldIndex, &lpMainField);
            DSSThresholds *lpThresholds = lpMainField->getThresholds();
            int lnThresholds = lpThresholds->Count();            
            vector<DSSFormat *> lvFormat;
            if (lnThresholds)
            {
                lvFormat.resize(lnThresholds);
                for (int i = 0; i < lnThresholds; ++i)
                {
                    DSSFormat *lpFormat = getFieldFormat(ipField, EnumFormattingFieldTypeCalculationField, ipFormat, lpThresholds->Item(i));
                    lvFormat[i] = lpFormat;
                }
            }
            DSSFormat *lpFieldFormat = getFieldFormat(ipField, EnumFormattingFieldTypeCalculationField, ipFormat, NULL);
            // jimzhang May/23/2012 end
            
            const vector<MBase::CString> & lvNullDispForFG = mpDataModel->getNullDispForFG();
                        
            mCurrent = 0;
            for (int lOff = mCurrent * mnFields + iMainFieldIndex; mCurrent < mnDisplayRows; ++mCurrent, lOff += mnFields)
            {
                AERawData lRawData;
                hr = DSSRWFieldGroupCalculator::Calculate(this, lpExpNode, ipField, mpFunctionServer, &lRawData);
                if (FAILED(hr))
                {
                    lbHasError = true;
                    continue;
                }
                
                if (DssDataOk != lRawData.mFlag)
                {
                    if (-1 == iLinkLocalIndex)
                    {
                        if (DssDataNull == lRawData.mFlag)
                            mvFeildsText[lOff].append(lvNullDispForFG[0]);
                        else if (DssDataUnCalculated == lRawData.mFlag)
                            mvFeildsText[lOff].append(lvNullDispForFG[1]);
                        else if (DssDataMissing == lRawData.mFlag)
                            mvFeildsText[lOff].append(lvNullDispForFG[2]);
                        else
                            mvFeildsText[lOff].append(lvNullDispForFG[3]);
                    }
                    else 
                    {
                        MBase::CString lLinkText;
                        if (DssDataNull == lRawData.mFlag)
                            lLinkText = lvNullDispForFG[0];
                        else if (DssDataUnCalculated == lRawData.mFlag)
                            lLinkText = lvNullDispForFG[1];
                        else if (DssDataMissing == lRawData.mFlag)
                            lLinkText = lvNullDispForFG[2];
                        else
                            lLinkText = lvNullDispForFG[3];
                        hGetEncodedLinkText(lLinkText, mvLinkFieldText[lOff]->mtext[iLinkLocalIndex], lType, mvLinkFieldText[lOff]->mtype[iLinkLocalIndex], DssAutoTextReserved);
                    }
                    
                    continue;
                }
                
                int lThresholdID = mvThresholdID[lOff];
                DSSFormat *lpFormat = -1 == lThresholdID ? lpFieldFormat : lvFormat[lThresholdID];
                int lFormType=0;
                int lColor=0;
                MBase::CString lResultStr;
                hr = DSSFormatHelper::getFormatValue(lRawData.mValue, lRawData.mSizeInByte, lRawData.mDataType, lRawData.mFlag, lFormType, lpFormat, &lColor, lResultStr);
                if (S_OK != hr)
                {
                    lbHasError = true;
                    continue;
                }
                
                if (-1 == iLinkLocalIndex)
                    mvFeildsText[lOff].append(lResultStr);
                else
                    hGetEncodedLinkText(lResultStr, mvLinkFieldText[lOff]->mtext[iLinkLocalIndex], lType, mvLinkFieldText[lOff]->mtype[iLinkLocalIndex], DssAutoTextReserved);
            }
        }
            break;
            
        case DssRWFieldObjectName:
        {
            return E_NOTIMPL;
        }
            break;
            
        case DssRWFieldAutoText:
        {
            bool lbStaticAutoText=true;
            
            std::string lAutoTextValue;
            hr = mpDataModel->getAutoText(lKey,lAutoTextValue);
            CHECK_HR;
            
            if (! lAutoTextValue.length())
            {
                GUID lDataSourceID=ipField->getDataSourceObjectID();
                if(MBase::IsGUIDNULL(lDataSourceID))
                {
                    DSSDocumentInstance *lpDocumentInstance=mpDataModel->getDocumentInstance();
                    lDataSourceID=lpDocumentInstance->getDefaultDataSetID();
                }
                
                EnumDSSAutoText lAutoTextType=ipField->getAutoTextType();
                
                if (DssAutoTextDocument == lAutoTextType)
                {
                    DSSDocumentDefinition *lpDocumentDefinition=DSSRWIterator::mpObjectContext->getDocumentDefinition();
                    lAutoTextValue = lpDocumentDefinition->getName();
                }
                else if (DssAutoTextProject == lAutoTextType)
                    hr = mpDataModel->getAutoText(lDataSourceID, "PROJECT", lAutoTextValue);
                else if (DssAutoTextDateTime == lAutoTextType)
                    ;// ToDo we need get the current time of Cilent Side
                else if (DssAutoTextUser == lAutoTextType)
                    hr=mpDataModel->getAutoText(lDataSourceID, "USER", lAutoTextValue);
                else if (DssAutoTextReport == lAutoTextType)
                    hr=mpDataModel->getAutoText(lDataSourceID, "REPORTNAME", lAutoTextValue);
                else if (DssAutoTextReportDescription == lAutoTextType)
                    hr=mpDataModel->getAutoText(lDataSourceID, "REPORTDESCRIPTION", lAutoTextValue);
                else if (DssAutoTextReportExecutionTime == lAutoTextType)
                    hr=mpDataModel->getAutoText(lDataSourceID, "EXECUTIONTIME", lAutoTextValue);
                else if (DssAutoTextReportFilterDetails == lAutoTextType)
                    hr=mpDataModel->getAutoText(lDataSourceID, "FILTERDETAILS", lAutoTextValue);
                else if (DssAutoTextReportPromptDetails == lAutoTextType)
                    hr=mpDataModel->getAutoText(lDataSourceID, "PROMPTDETAILS", lAutoTextValue);
                else if (DssAutoTextAttributeElementID==lAutoTextType||DssAutoTextAttributeLongElementID==lAutoTextType)
                {
                    lbStaticAutoText=false;
                    
                    
                    DSSAttribute *lpAttribute=dynamic_cast<DSSAttribute*>(ipField->getFieldObject());
                    
                    if(lpAttribute)
                    {
                        GUID lObjID=lpAttribute->getID();
                        
                        DSSTabularAttribute *lpTabularAttribute=mpDataModel->GetTabularAttributeByGUID(lObjID);
                        DSSGlobalLookUpTable* lpLookUp = dynamic_cast<DSSGlobalLookUpTable*>(lpTabularAttribute);
                        CHECK_PTR(lpLookUp);
                        
                        int lUnitIndex = -1;
                        hr = FindUnit(lObjID, &lUnitIndex);
                        CHECK_HR;
                        
                        mCurrent=0;
                        
                        for (int lOff = mCurrent * mnFields + iMainFieldIndex; mCurrent < mnDisplayRows; ++mCurrent, lOff += mnFields)
                        {
                            int lGlobalKey = -1;
                            hr = getElementIndex(mvRows[mCurrent], lUnitIndex, &lGlobalKey);
                            CHECK_HR;                
                            
                            if(lGlobalKey == DSSXTAB_TOTAL_KEY)
                                continue;
                            
                            bool lfHasDE = false;     
                            int lLocalKey = -1;
                            DSSTabularUnit* lpTabularUnit = NULL;
                            hr = lpLookUp->GetGlobalUnit(lGlobalKey, &lpTabularUnit, &lLocalKey, &lfHasDE);             
                            CHECK_HR_PTR(lpTabularUnit);
                            
                            DSSBaseElementProxy *lpElement=NULL;
                            hr = lpTabularUnit->createElement(lLocalKey, mpDataModel->getObjectContext(), &lpElement);
                            CHECK_HR;
                            
                            if(lpElement)
                            {
                                // TQMS 763290,751433,619843, convert long element ID to short one.
                                //lAutoTextValue=lpElement->getElementID();
                                lAutoTextValue = DSSBaseElementsProxy::ConvertFromLongToShort(lpElement->getElementID());
                            }                            
                            
                            if (-1 == iLinkLocalIndex)                    
                                mvFeildsText[lOff].append(lAutoTextValue);
                            else
                                hGetEncodedLinkText(lAutoTextValue, mvLinkFieldText[lOff]->mtext[iLinkLocalIndex], lType, mvLinkFieldText[lOff]->mtype[iLinkLocalIndex], lAutoTextType);
                        }
                    }
                    
                    //                        
                    //                    if (iParentKey)
                    //                    {
                    //                        map<string,MBase::CString>::iterator iter = mvAttributeElementID.find(*iParentKey);
                    //                        if(iter!=mvAttributeElementID.end())
                    //                            lAutoTextValue=iter->second;
                    //                    }
                }
                else if (DssAutoTextCurrentSelectionElementID == lAutoTextType || DssAutoTextCurrentSelectionID == lAutoTextType)
                {
                    // Jul/16/2013, DSSField::mText is like “&CURRENTSELECTIONELEMENTID:W47&”, so we should extract key from it.

                    string lpFieldText = ipField->getText();
                    char *lpBegin = strstr(lpFieldText.c_str(), ":");
                    if (! lpBegin)
                        return S_FALSE;
                    
                    char *lpEnd = strstr(lpBegin, "&");
                    if (! lpEnd)
                        return S_FALSE;
                    
                    string lControlKey(lpBegin+1, lpEnd-lpBegin-1);
                    DSSRWControl* lpControl = mpDataModel->getObjectContext()->getControl(lControlKey);
                    if (! lpControl)
                    {
                        lAutoTextValue.append("{&CurrentSelectionID:");
						lAutoTextValue.append(lControlKey);
						lAutoTextValue.append("&}");
                    }
                    else
                    {
                        
                        EnumDSSCurrentElementStatus lStatus = lpControl->getCurrentElementsStatus();
                        if (DssCurrentElementUnset == lStatus)
                        {
                            // Unset, the field vaule is set to empty
                            lAutoTextValue = "";
                            return S_OK;
                        }
                        
                        DSSBaseElementsProxy * lpElements = lpControl->getCurrentElements();
                        CHECK_PTR(lpElements);
                        
                        int lnElement = lpElements->Count();
                        DSSObjectInfo *lpSrcObj = lpControl->getSourceObject();
                        EnumDSSObjectType lSrcType = lpSrcObj->getType();
                        
                        for (int iElem = 0; iElem < lnElement ; iElem++)
                        {
                            if (! lAutoTextValue.empty())
                                lAutoTextValue.append(";");
                            
                            string lElemText;
                            
                            DSSBaseElementProxy *lpElement = lpElements->Item(iElem);
                            CHECK_PTR(lpElement);
                            
                            EnumDSSElementType lElemType = lpElement->getElementType();
                            
                            if (lElemType == DssElementAll || lElemType == DssElementNULL)
                            {
                                lAutoTextValue = "";
                            }
                            else if (lElemType == DssElementSubtotal)
                            {
                                if ( lAutoTextType == DssAutoTextCurrentSelectionID )
                                    lAutoTextValue.append("Total");
                                else
                                    lAutoTextValue.append("");
                            }
                            else if (lElemType == DssElementConcrete && lSrcType == DssTypeAttribute)
                            {
                                if ( lAutoTextType == DssAutoTextCurrentSelectionID )
                                {
                                    DSSAttribute *lpAttr = dynamic_cast<DSSAttribute *>(lpSrcObj);
                                    if (! lpAttr)
                                        return S_OK;
                                    
                                    lElemText = lpElement->getDisplayText();
                                }
                                else
                                {
                                    lElemText = lpElement->getElementID();
                                    lElemText.append("^");
                                    //de22478, msun, encode the display text into url link
                                    //we should escape the special charactors that is used in the elementAnswerString
                                    MBase::CString lTempText = lpElement->getDisplayText();
                                    hEscape(lTempText, ',',"~!@@1~");
                                    hEscape(lTempText, ';',"~!@@2~");
                                    hEscape(lTempText, '^',"~!@@3~");
                                    hEncodeURL(lTempText);
                                    lElemText.append(lTempText);
                                }
                                
                                lAutoTextValue.append(lElemText);
                            }
                            else if (DssElementFilter == lElemType)
                            {
                                if ( lAutoTextType == DssAutoTextCurrentSelectionElementID )
                                    lAutoTextValue.append(lpElement->getElementID());
                                
                            }
                            else if (DssElementConsolidation == lElemType)
                            {
                                if ( lAutoTextType == DssAutoTextCurrentSelectionElementID )
                                    lAutoTextValue.append(lpElement->getElementID());
                            }
                            else
                            {
                                lAutoTextValue.clear();
                                lAutoTextValue.append("{&CurrentSelection");
                                if (lAutoTextType == DssAutoTextCurrentSelectionID)
                                    lAutoTextValue.append("ID");
                                else
                                    lAutoTextValue.append("ElementID");
                                
                                lAutoTextValue.append(":");
                                lAutoTextValue.append(lControlKey.c_str());
                                lAutoTextValue.append("&}");
                                
                                return S_OK;
                            }
                        } // END: for (int iElem = 0; iElem < lnElement ; iElem++)
                    }
                }
                else
                {
                    lAutoTextValue="";
                }
                
                CHECK_HR;
            } // END: if (! lAutoTextValue.length())
            
            if(lbStaticAutoText)
            {
                int lnCells = mnDisplayRows * mnFields;
                for (int lOff = iMainFieldIndex; lOff < lnCells; lOff += mnFields)
                {
                    if (-1 == iLinkLocalIndex)                    
                        mvFeildsText[lOff].append(lAutoTextValue);
                    else
                        hGetEncodedLinkText(lAutoTextValue, mvLinkFieldText[lOff]->mtext[iLinkLocalIndex], lType, mvLinkFieldText[lOff]->mtype[iLinkLocalIndex], ipField->getAutoTextType());
                }
            }
        }
            break;
            
        case DssRWFieldObjectValue:
        {
            DSSObjectInfo* lpObjInfo = ipField->getFieldObject();
            CHECK_PTR(lpObjInfo);
            
            GUID lObjID = lpObjInfo->getID();
            EnumDSSObjectType lObjType = lpObjInfo->getType();
            switch (lObjType)
            {
                case DssTypeMetric:
                {
                    int lRTTIndex = -1;
                    int lMetricIndex = -1;
                    
                    map<MBase::CString, int>::iterator iter1 = mmFieldToRTTIndex.find(lKey);
                    map<MBase::CString, int>::iterator iter2 = mmFieldToMetricIndex.find(lKey);
                    if (iter1 == mmFieldToRTTIndex.end() || iter2 == mmFieldToMetricIndex.end())
                    {
                        GUID lReportGUID = ipField->getDataSourceObjectID();
                        hr = hGetMetricIndex(lReportGUID, lObjID, lRTTIndex, lMetricIndex);
                        CHECK_HR;
                    }
                    else
                    {
                        lRTTIndex = iter1->second;
                        lMetricIndex = iter2->second;
                    }
                    
                    return hReInitMetricField(iMainFieldIndex, iLinkLocalIndex, ipField, ipFormat, lRTTIndex, lMetricIndex);
                }
                    break;
                    
                case DssTypeAttribute:
                {
                    return hReInitAttrField(iMainFieldIndex, iLinkLocalIndex, ipField, ipFormat, iParentKey, lObjID);
                }
                    break;
                    
                case DssTypeFilter:
                {
                    //junwang, 01/09/2012, get texts from DSSTabularCustomGroup
                    DSSCustomGroup* lpCG = dynamic_cast<DSSCustomGroup*>(lpObjInfo);
                    CHECK_PTR(lpCG);
                    
                    int lUnitIndex = -1;
                    hr = FindUnit(lObjID, &lUnitIndex);
                    CHECK_HR;
                    
                    mCurrent = 0;
                    for (int  lOff = iMainFieldIndex; mCurrent < mnDisplayRows; ++mCurrent, lOff += mnFields)
                    {
                        int lGlobalKey = -1;
                        hr = getElementIndex(mvRows[mCurrent], lUnitIndex, &lGlobalKey);
                        if (S_OK != hr)
                        {
                            lbHasError = true;
                            continue;
                        }
                        
                        if (DSSXTAB_TOTAL_KEY == lGlobalKey)
                        {
                            // jimzhang dec/30/2011 ToDo "Total" is hard code
                            if (-1 == iLinkLocalIndex)
                                mvFeildsText[lOff].append("Total");
                            else
                                mvLinkFieldText[lOff]->mtext[iLinkLocalIndex]->append("Total");
                            
                            continue;
                        }
                        
                        DSSTabularUnit* lpTabularUnit = NULL;
                        int lLocalKey = -1;
                        mpDataModel->GetTabularUnit(lObjID, lGlobalKey, &lpTabularUnit, &lLocalKey);
                        if (! lpTabularUnit)
                        {
                            lbHasError = true;
                            continue;
                        }
                        
                        DSSTabularCustomGroup* lpTCG = dynamic_cast<DSSTabularCustomGroup*>(lpTabularUnit);
                        if (! lpTCG)
                        {
                            lbHasError = true;
                            continue;
                        }
                        
                        XTabCustomGroupElement* lpCGElement = lpTCG->getElement(lLocalKey);
                        if (! lpCGElement)
                        {
                            lbHasError = true;
                            continue;
                        }
                        
                        if (-1 == iLinkLocalIndex)
                            mvFeildsText[lOff].append(lpCGElement->displayName);
                        else
                            hGetEncodedLinkText(lpCGElement->displayName, mvLinkFieldText[lOff]->mtext[iLinkLocalIndex], lType, mvLinkFieldText[lOff]->mtype[iLinkLocalIndex], DssAutoTextReserved);
                    }
                }
                    break;
                case DssTypeConsolidation:
                {
                    //yimliu, 2014/07/16, 920357, 919585.
                    return hReInitConsolidationFieldSimple(iMainFieldIndex, iLinkLocalIndex, ipField, ipFormat, iParentKey, lObjID);
                    /*
                    //junwang, 01/09/2012, get texts from DSSTabularConsolidation
                    DSSConsolidation *lpCS = dynamic_cast<DSSConsolidation *>(lpObjInfo);
                    CHECK_PTR(lpCS);
                    
                    int lUnitIndex = -1;
                    hr = FindUnit(lObjID, &lUnitIndex);
                    CHECK_HR;
                    
                    mCurrent = 0;
                    for (int lOff = iMainFieldIndex; mCurrent < mnDisplayRows; ++mCurrent, lOff += mnFields)
                    {
                        int lGlobalKey = -1;
                        hr = getElementIndex(mvRows[mCurrent], lUnitIndex, &lGlobalKey);
                        if (S_OK != hr)
                        {
                            lbHasError = true;
                            continue;
                        }
                        
                        if (DSSXTAB_TOTAL_KEY == lGlobalKey)
                        {
                            // jimzhang dec/30/2011 ToDo "Total" is hard code
                            if (-1 == iLinkLocalIndex)
                                mvFeildsText[lOff].append("Total");
                            else
                                mvLinkFieldText[lOff]->mtext[iLinkLocalIndex]->append("Total");
                            
                            continue;
                        }
                        else if (-1 == lGlobalKey)
                        {
                            // jimzhang Mar/28/2012 Global key equals -1 means that the type of this element is DssElementNULL.
                            //      In this case, for Consolidation, server's logic only assign empty string to this element's value.
                            if (-1 == iLinkLocalIndex)
                                mvFeildsText[lOff].append("");
                            else
                                mvLinkFieldText[lOff]->mtext[iLinkLocalIndex]->append("");
                            
                            continue;
                        }
                        
                        DSSTabularUnit* lpTabularUnit = NULL;
                        int lLocalKey = -1;
                        mpDataModel->GetTabularUnit(lObjID, lGlobalKey, &lpTabularUnit, &lLocalKey);
                        if (! lpTabularUnit)
                        {
                            lbHasError = true;
                            continue;
                        }
                        
                        DSSTabularConsolidation* lpTCS = dynamic_cast<DSSTabularConsolidation*>(lpTabularUnit);
                        if (! lpTCS)
                        {
                            lbHasError = true;
                            continue;
                        }
                        
                        XTabConsolidationElement* lpCSElement = lpTCS->getElement(lLocalKey);
                        if (! lpCSElement)
                        {
                            lbHasError = true;
                            continue;
                        }
                        
                        if (-1 == iLinkLocalIndex)
                            mvFeildsText[lOff].append(lpCSElement->value);
                        else
                            hGetEncodedLinkText(lpCSElement->value, mvLinkFieldText[lOff]->mtext[iLinkLocalIndex], lType, mvLinkFieldText[lOff]->mtype[iLinkLocalIndex], DssAutoTextReserved);
                    }
                     */
                }
                    break;
                    
                default:
                    return E_NOTIMPL; 
            }
        }
            break;
            
        case DssRWFieldConcatenation:
        {
            vector<int> lvLinkFieldIndex;
            
            for (int i = 1; i < DssRWLinkLastOne; ++i)
            {
                DSSFields *lpChildren = ipField->getLinkChildFields(static_cast<EnumDSSRWLinkType>(i));
                if (lpChildren)
                    lvLinkFieldIndex.push_back(i);
            }
            int lnFieldLinks = lvLinkFieldIndex.size();
            
            DSSFields *lpChildren = ipField->getChildFields();
            if (lpChildren)
            {
                DSSFormat *lpFormat = ipField->getFormat();
                string lParentKey = ipField->getKey();
                
                int lnChildren = lpChildren->Count();
                for (int iChild = 0; iChild < lnChildren; ++iChild)
                {
                    DSSField* lpChild = lpChildren->Item(iChild);
                    CHECK_PTR(lpChild);
                    
                    if (lnFieldLinks > 0)
                        hr = hReInitFieldTextAndLink(iMainFieldIndex, -1, lpChild, lpFormat, &lParentKey);
                    else
                        hr = hReInitFieldTextAndLink(iMainFieldIndex, -1, lpChild, lpFormat, iParentKey);
                   
                    if (FAILED(hr)) // do not return error if hr is E_FALSE
                        return hr;
                }
            }
            
            if (! lnFieldLinks)
                return hr;
            
            for (int iRow = 0, lOff = iMainFieldIndex; iRow < mnDisplayRows; ++iRow, lOff += mnFields)
            {
                FieldLink *lpFieldLink = new FieldLink(lnFieldLinks);
                for (int i = 0; i < lnFieldLinks; ++i)
                {
                    lpFieldLink->mtype[i] = (EnumDSSRWLinkType)(lvLinkFieldIndex[i]);
                    lpFieldLink->mtext[i] = new MBase::CString;
                }
                
                mvLinkFieldText[lOff] = lpFieldLink;
            }
            
            for (int i = 0; i < lnFieldLinks; ++i)
            {
                DSSFields *lpChildren = ipField->getLinkChildFields(static_cast<EnumDSSRWLinkType>(lvLinkFieldIndex[i]));
                if (lpChildren)
                {
                    DSSFormat *lpFormat=ipField->getFormat();
                    string lParentKey=ipField->getKey();
                    
                    int lnChildren = lpChildren->Count();
                    for (int iChild = 0; iChild < lnChildren; ++iChild)
                    {
                        DSSField* lpChild = lpChildren->Item(iChild);
                        CHECK_PTR(lpChild);
                        
                        hr = hReInitFieldTextAndLink(iMainFieldIndex, i, lpChild, lpFormat, &lParentKey);
                        CHECK_HR;
                    }
                }
            }
        }
            break;
            
        case DssRWFieldHTML:
        {
            vector<int> lvLinkFieldIndex;
            
            for (int i = 1; i < DssRWLinkLastOne; ++i)
            {
                DSSFields *lpChildren = ipField->getLinkChildFields(static_cast<EnumDSSRWLinkType>(i));
                if (lpChildren)
                    lvLinkFieldIndex.push_back(i);
            }
            int lnFieldLinks = lvLinkFieldIndex.size();
            
            if (! lnFieldLinks)
                return hr;
            
            for (int iRow = 0, lOff = iMainFieldIndex; iRow < mnDisplayRows; ++iRow, lOff += mnFields)
            {
                FieldLink *lpFieldLink = new FieldLink(lnFieldLinks);
                for (int i = 0; i < lnFieldLinks; ++i)
                {
                    lpFieldLink->mtype[i] = (EnumDSSRWLinkType)(lvLinkFieldIndex[i]);
                    lpFieldLink->mtext[i] = new MBase::CString;
                }
                
                mvLinkFieldText[lOff] = lpFieldLink;
            }
            
            for (int i = 0; i < lnFieldLinks; ++i)
            {
                DSSFields *lpChildren = ipField->getLinkChildFields(static_cast<EnumDSSRWLinkType>(lvLinkFieldIndex[i]));
                if (lpChildren)
                {
                    DSSFormat *lpFormat=ipField->getFormat();
                    string lParentKey=ipField->getKey();
                    
                    int lnChildren = lpChildren->Count();
                    for (int iChild = 0; iChild < lnChildren; ++iChild)
                    {
                        DSSField* lpChild = lpChildren->Item(iChild);
                        CHECK_PTR(lpChild);
                        
                        hr = hReInitFieldTextAndLink(iMainFieldIndex, i, lpChild, lpFormat, &lParentKey);
                        CHECK_HR;
                    }
                }
            }
        }
            break;
            
        case DssRWFieldReserved:
        {
            // jimzhang Feb/08/2012 The reason to return S_OK for DssRWFieldReserved is as belows:
            // (1) Special FG only has one Field. This Field only has threshold (i.e. doesn't have text),  
            //  and the type of this Field is DssRWFieldReserved.
            // (2) As there is no method to distinguish special FG from normal FG, hReInitFieldTextAndLink() will be called for Field of special FG.
            return S_OK;
        }
            break;
            
        default:
            return E_NOTIMPL;
    }
    
    return S_OK;    
}

int DSSRWFieldGroupModelIterator::hReInitThreshold(int iFieldIndex, DSSField *ipField)
{       
    int hr = S_OK;
    bool lbHasError = false;
    
    DSSThresholds *lpThresholds = ipField->getThresholds();
    assert(NULL != lpThresholds);
    
    if (! mpFunctionServer)
    {
        mpFunctionServer = new DSSFunctionServer();
        CHECK_PTR(mpFunctionServer);
    }
    
    vector<int> lvTag;
    
    int lnThresholds = lpThresholds->Count();
    for (int iThreshold = 0; iThreshold < lnThresholds; ++iThreshold)
    {
        DSSThreshold *lpThreshold = lpThresholds->Item(iThreshold);
        assert(NULL != lpThreshold);
        
        if (! lpThreshold->getIsEnabled())
            continue;
        
        int lScope = lpThreshold->getScope();
        
        DSSCExpression *lpExp = lpThreshold->getExpression();
        if (!lpExp)
            continue;
        
        DSSExpNode *lpRoot = lpExp->Root();
        if (!lpRoot || DssNodeOperator != lpRoot->getType())
            continue;
        
        mCurrent = 0;
        for (int lOff = mCurrent * mnFields + iFieldIndex; mCurrent < mnDisplayRows; ++mCurrent, lOff = lOff + mnFields)
        {
            if (-1 != mvThresholdID[lOff])
                continue;
            
            int lTag = -1;
            if (DssThresholdScopeMetricAndSubtotal != lScope)
            {   
                if (! lvTag.size())
                {
                    lvTag.resize(mnDisplayRows);                    
                    
                    int lCurrentBackup = mCurrent;
                    
                    for (mCurrent = 0; mCurrent < mnDisplayRows; ++mCurrent)
                    {
                        hr = getTag(&lTag);
                        if (S_OK != hr)
                            // jimzhang May/22/2012 "-2" is an invalid value which represents that it's fail to calculate this "tag".
                            lvTag[mCurrent] = -2;
                        else
                            lvTag[mCurrent] = lTag;
                    }
                    
                    mCurrent = lCurrentBackup;
                }
                
                lTag = lvTag[mCurrent];
                if (-2 == lTag) // jimzhang May/22/2012 "-2" is an invalid value which represents that it's fail to calculate this "tag".
                    continue;
                
                if ( (DssThresholdScopeMetricOnly == lScope) && (lTag > 0) )
                    continue;                
                if ( (DssThresholdScopeSubtotalOnly == lScope) && (lTag <= 0) )
                    continue;
            }
            
            //yimliu, 2014/05/29, 883809, Memory leak. The object must have ownership to delete its data.
            /*
             AERawData lRawData;
             */
            //yimliu, 2014/06/20, 833809, Memory leak. Fix it somewhere else.
            /*
             AERawData lRawData(true);
             */
            AERawData lRawData;
            
            hr = DSSRWFieldGroupCalculator::Calculate(this, lpRoot, ipField, mpFunctionServer, &lRawData);
            if (S_OK != hr || DssDataOk != lRawData.mFlag)
            {
                lbHasError = true;
                continue;
            }
            double lResult = .0;
            DSSAEHelper::ConvertToNumeric(lRawData.mValue, lRawData.mSizeInByte, lRawData.mDataType, lRawData.mFlag, lResult);
            if (lResult)
                mvThresholdID[lOff] =  iThreshold;
        } // END: for (mCurrent = 0; mCurrent < mnDisplayRows; ++mCurrent)
    } // END: for (int iThreshold = 0; iThreshold < lnThresholds; iThreshold++)
    
    return lbHasError ? S_FALSE : S_OK;
}

int DSSRWFieldGroupModelIterator::getCommandList(DSSCellHandle &irHandle, vector<EnumManipulationCommandTypes> &orvCommand)
{
    //there's no event on field group
	return S_OK;
}

//hyan,how do we get the right format of FG???
DSSFormat* DSSRWFieldGroupModelIterator::getFieldFormat(DSSField *ipField)
{
    DSSFormat *lpFormat=NULL;    
    lpFormat=ipField->getFormat();    
    if(lpFormat)
        return lpFormat;
    
    DSSObjectInfo* lpObjInfo = ipField->getFieldObject();
    DSSMetric* lpMetric=dynamic_cast<DSSMetric*>(lpObjInfo);    
    if(lpMetric)
    {
        lpFormat=lpMetric->getGridFormat();        
        if(lpFormat)
            return lpFormat;
    }    
    
    return lpFormat;    
}


DSSFormat* DSSRWFieldGroupModelIterator::getFieldFormat(DSSField *ipField,EnumFormattingFieldType iFieldType,DSSFormat* ipParentFieldFormat,DSSThreshold *ipThreshold)
{
    DSSFormat *lpFormat=NULL;
    
    
    if(ipThreshold)
    {
        lpFormat=ipThreshold->getFormat();
        
        if(lpFormat)
        {
            // jzeng, 2015/06/01 DE12487 check whether threshold is format to general(9)
            CComVariant* lpVal = lpFormat->getPropertyValue("FormattingNumber", "Category");
            if (lpVal && lpVal->iVal == 9)
            {
                //de22000, lizhang, should not override lpFormat as we will need to use it if getFieldFormat(ipField) returns null
                DSSFormat *lpNewFormat = getFieldFormat(ipField);
                if(lpNewFormat)
                    return lpNewFormat;
//                lpFormat = getFieldFormat(ipField);
//                if (lpFormat)
//                    return lpFormat;
            }
            //hyan,2012/04/25
            //TQMS585175, we need to check whether the threshold contain number formatting info
            
            lpVal = lpFormat->getPropertyValue("FormattingNumber", "Format");
            
            if(lpVal)
            {
                return lpFormat;
            }
        }
    }
    
    switch(iFieldType)
    {
        case EnumFormattingFieldTypeAttribute:
        {
            if ( NULL == ipParentFieldFormat)
                lpFormat = getFieldFormat(ipField);
            else
                lpFormat = ipParentFieldFormat;

        }
            break;
            
        case EnumFormattingFieldTypeMetric:
        {
            if(ipParentFieldFormat)
            {
                CComVariant* lpVal = NULL;
                lpVal = ipParentFieldFormat->getPropertyValue("FormattingNumber", "Category");
                
                //hyan, 2012/05/09, #587096, we need to check whether the category is general(9)
                if(lpVal==NULL||lpVal->iVal == 9)
                    lpFormat=getFieldFormat(ipField);
                else
                    lpFormat=ipParentFieldFormat;
            }
            else
            {
                lpFormat=getFieldFormat(ipField);
            }
            
            
            if(lpFormat)
            {
                CComVariant* lpVal = NULL;
                
                lpVal = lpFormat->getPropertyValue("FormattingNumber", "Format");
                
                bool lbGeneral = false;//xiazhou, 950831, 2014/09/10. Check if the format is General.
                if(lpVal)
                    lbGeneral = (lpVal->vt == VT_I2) ? lpVal->iVal==9 : lpVal->ToString()=="General";
                
                if(!lpVal || lbGeneral)
                    lpFormat=mpDataModel->getDefaultFormatOfCF();
            }

        }
            break;
            
        case EnumFormattingFieldTypeCalculationField:
        {
            if(ipParentFieldFormat)
            {
                CComVariant* lpVal = NULL;
                lpVal = ipParentFieldFormat->getPropertyValue("FormattingNumber", "Category");
                
                if(lpVal==NULL)
                    lpFormat=getFieldFormat(ipField);
                else
                    lpFormat=ipParentFieldFormat;
            }
            else
            {
                lpFormat=getFieldFormat(ipField);
            }
            
            
            //2012/03/26
            //TQMS569840
            if(lpFormat==NULL)
            {
                lpFormat=mpDataModel->getDefaultFormatOfCF();
            }

        }
            break;
            
    default:
        break;
    }
    
    return lpFormat;    

}

int DSSRWFieldGroupModelIterator::hFindField(int iFieldIndex, DSSField **oppField)
{
	if (!oppField)
		return E_POINTER;
    
    *oppField = NULL;
    
    DSSRWFieldGroupNode * lpThisNode = dynamic_cast<DSSRWFieldGroupNode *>(mpNode);
    if (NULL == lpThisNode)
        return E_FAIL;
      
    DSSFields *lpFields = lpThisNode->getFields();
    CHECK_PTR(lpFields);
    
    *oppField = lpFields->Item(iFieldIndex);
    CHECK_PTR(*oppField);
    
    return S_OK;
}

// @Param2: the RTTemplateInfo's offset in DSSRWFieldGroupModel::mvpRTTemplateInfo.
int DSSRWFieldGroupModelIterator::getMetricValue(int iRow, int iRTTIndex, int iMetricIndex, const char *&orpData, int &orDataSize, EnumDSSDataType &orDataType, DSSDataFlag &orDataFlag)
{
    if (iRow >= mnRows)
		return E_UNEXPECTED;
    
    if(!mpIndexTable)
		return E_FAIL;    
    
    int hr = S_OK;
    
    // 1. Get keys in XTabView for current row in JoinedIndexTable
    DSSRWFieldGroupModel *lpFGModel = static_cast<DSSRWFieldGroupModel *>(DSSRWNodeModelIterator::mpNodeModel);
    CHECK_PTR(lpFGModel);
    
    DSSViewDataSet *lpViewDataSet = lpFGModel->mvpRTTemplateInfo[iRTTIndex]->mViewPtr;
    
	int lnColumns = mpIndexTable->mvColumnMap.size();
    // jimzhang Apr/25/2012 TQMS 583553 For special field group, row count of joined index may be 0. In this case, 
	//		set flag of this metric is DssDataInvalid, and it will be considered as NULL by caller.
    if (0 < lnColumns && ! mpIndexTable->mnRow)
    {
        orpData = NULL;
        orDataSize = 0;
        orDataType = DssDataTypeUnknown;
        orDataFlag = DssDataInvalid;
        return S_OK;
    }
    
    vector<int> lvKeysInJoinedIndex(lnColumns);
	for (int i = 0; i < lnColumns; ++i)
	{
		int lKey = -1; 
		hr = mpIndexTable->GetKey(iRow, i, &lKey);
		CHECK_HR;
        lvKeysInJoinedIndex[i] = lKey;
	}
    
    bool lbHasSubtotal = false;
    
    DSSTabularData *lpTabularData = lpViewDataSet->getTabularData();
    
    int lDummyIndex = 0;
    int lDummyUnitIndex = 0;
    hr = lpTabularData->getDummyUnitIndex(&lDummyUnitIndex);
    CHECK_HR;
    
    int lnUnits = lpViewDataSet->Count();    
    vector<int> lvKeysInViewDataset;
    lvKeysInViewDataset.resize(lnUnits);
    
    bool lDisplay = true;
	set<Int32> lsCG;
	set<Int32> lsCGTotal;
	vector<DSSTabularConsolidation *> lvCS;
    
    for (int i = 0; i < lnUnits; ++i)
    {
        DSSTabularUnit *lpUnit = NULL;
        hr = lpTabularData->Item(i, &lpUnit);
        CHECK_HR;
        
        GUID lGUID = GUID_NULL;
        hr = lpUnit->getGUID(lGUID);
        CHECK_HR;
        
        EnumDSSTemplateUnitType lType = lpUnit->getUnitType();
		if (DssTemplateMetrics != lType)
        {            
            int lGlobalIndex = -1;
            if (lDummyUnitIndex == i)
            {
                DSSGUnitContainer *lpGUnitContainer = NULL;
                hr = mpDataModel->GetGlobalLookupContainer(&lpGUnitContainer);
                CHECK_PTR(lpGUnitContainer);
                
                lGlobalIndex = lpGUnitContainer->count();
            }
            else
            {
                lGlobalIndex = mpDataModel->FindUnit(lGUID);
            }
            
            if (lGlobalIndex < 0)
                return E_FAIL;
            
            int lLocalIndex = -1;
            for (int j = 0; j < lnColumns; ++j)
            {
                if (mpIndexTable->mvColumnMap[j] == lGlobalIndex)
                {
                    lLocalIndex = j;
                    break;
                }
            }
            
            if (lLocalIndex >= 0)
				lvKeysInViewDataset[i] = lvKeysInJoinedIndex[lLocalIndex];
			else
				lvKeysInViewDataset[i] = DSSXTAB_TOTAL_KEY;
            
            if (lDummyUnitIndex == i)
                lDummyIndex = lvKeysInViewDataset[i];
            
            if (DssTemplateCustomGroup == lType)
			{
                if (DSSXTAB_TOTAL_KEY == lvKeysInViewDataset[i])
					lsCGTotal.insert(i); //since in Consolidation and DE case, CS unit has a chid CG, so this may not mean subtotal
				else
					lsCG.insert(i);
			}
			else if (DssTemplateConsolidation == lType)
			{                
                DSSTabularConsolidation *lpCS = dynamic_cast<DSSTabularConsolidation *>(lpUnit);
                CHECK_PTR(lpCS);
                lvCS.push_back(lpCS);
                
                DSSGUnitContainer *lpContainer = NULL;
                hr = mpDataModel->GetGlobalLookupContainer(&lpContainer);
                CHECK_PTR(lpContainer);
                
                int lCompoundNull = -1;
                hr = lpContainer->GetGlobalRowCount(lGlobalIndex, &lCompoundNull, false);
                CHECK_HR;              
                
                if (lvKeysInViewDataset[i] >= 0 && lCompoundNull != lvKeysInViewDataset[i])
				{
                    XTabConsolidationElement *lpXTabCSElement = lpCS->getElement(lvKeysInViewDataset[i]);
                    //tliao, #689475
                    if (lpXTabCSElement)
                    {
                        int lOrdinal = lpXTabCSElement->ordinal;
                        if(lOrdinal < 0)
                        {
                            lDisplay = false;
                            break;
                        }
                    }
				}
                
				if (DSSXTAB_TOTAL_KEY == lvKeysInViewDataset[i])
                {
					lbHasSubtotal = true;
                }
			}
			else if (DSSXTAB_TOTAL_KEY == lvKeysInViewDataset[i])
            {
				lbHasSubtotal = true;
            }
        }
        else
        {
            lvKeysInViewDataset[i] = DSSXTAB_TOTAL_KEY;
        }
    }
    
    if (!lDisplay)
	{
        orDataFlag = DssDataNull;
		return S_OK;
	}
	
	if ((!lbHasSubtotal && lsCGTotal.size() > 0) || lsCG.size() > 0)
	{
		set<Int32>::iterator lIt;
		for (Int32 iCS = 0; iCS < lvCS.size(); iCS++)
		{
			DSSTabularConsolidation* lpCS = lvCS[iCS];
            
            Int32 lChildUnitIndex = -1;
            hr = lpViewDataSet->getTabularData()->FindUnitI(lpCS->getGUID(), DssTemplateCustomGroup, lChildUnitIndex);
            CHECK_HR;
            
			if(lsCG.size() > 0)
			{
				lIt = lsCG.find(lChildUnitIndex);
				if (lIt != lsCG.end())
				{
					lvKeysInViewDataset[*lIt] = DSSXTAB_TOTAL_KEY;
				}
			}
			if (!lbHasSubtotal && lsCGTotal.size() > 0)
			{
				lIt = lsCGTotal.find(lChildUnitIndex);
				if (lIt != lsCGTotal.end())
				{
					lsCGTotal.erase(lIt);
				}
			}
		}
		
		if (lsCGTotal.size() > 0)
			lbHasSubtotal = true;
	}
    
    // 2. Get metric value form XTabView
    
    Int32 lLocalTag = 0;
    
    if (lbHasSubtotal)
	{        
        lLocalTag = 1;  // jimzhang dec/20/2011 This is hard code for passing test. "1" stands for "total", assume that there is only subtotal of total type in FG currently.

        lDummyIndex = DSSXTAB_TOTAL_KEY;
        
        // jimzhang dec/20/2011 Comment out below server's logic for reference
        /*// get di.
         CComPtr<ICDSSDocumentInstance3> licpDI;
         hr = mpDataModel->get_DocumentInstance(&licpDI);
         CHECK_HR_PTR(licpDI);
         
         CComPtr<ICDSSObjectInfo> licpInfo;
         hr = mvpViewTemplate[iReport]->get_Info(&licpInfo);
         CHECK_HR_PTR(licpInfo);
         
         DSS_ID lTemplateID;
         hr = licpInfo->get_ID(&lTemplateID);
         CHECK_HR;
         
         CComQIPtr<ICDSSDocumentInstance7, &IID_ICDSSDocumentInstance7> licpDI7(licpDI);
         if (!licpDI7) return E_NOINTERFACE;
         
         hr = licpDI7->get_TemplateLevelSubtotalIndex(1, &lTemplateID, &lLocalTag);
         CHECK_HR;*/    
	}
    
    DSSXTabView *lpXTabView = lpFGModel->mvpXTabView[iRTTIndex];
    if (!lpXTabView)
        return E_POINTER;
    
    hr = lpXTabView->get_MetricValue2(lvKeysInViewDataset.size(),
                                      &lvKeysInViewDataset[0],
                                      iMetricIndex,
                                      lDummyIndex,
                                      DssOriginalSlice,
                                      lLocalTag,
                                      &orpData,
                                      &orDataSize,
                                      &orDataType,
                                      &orDataFlag);
    return hr;
}

// Sep/18/2012, todo, maybe need to defined as virtual function in the future
int DSSRWFieldGroupModelIterator::getCurrentMetricValueByGUID (GUID *ipReportGUID, GUID *ipMetricGUID, const std::string *ipFieldKey, AERawData *opRawData)
{
    int hr = S_OK;
    
    int lRTTIndex = -1;
    int lMetricIndex = -1;
    
    if (ipFieldKey)
    {
        map<MBase::CString, int>::iterator iter1 = mmFieldToRTTIndex.find(*ipFieldKey);
        map<MBase::CString, int>::iterator iter2 = mmFieldToMetricIndex.find(*ipFieldKey);                            
        if (mmFieldToRTTIndex.end() != iter1 && mmFieldToMetricIndex.end() != iter2)
        {
            lRTTIndex = iter1->second;
            lMetricIndex = iter2->second;
        }
    }
    else
    {
        hr = hGetMetricIndex(*ipReportGUID, *ipMetricGUID, lRTTIndex, lMetricIndex);
        CHECK_HR;
    }
    
    return getMetricValue(mvRows[mCurrent], lRTTIndex, lMetricIndex, *(const char **)(& opRawData->mValue), opRawData->mSizeInByte, opRawData->mDataType, opRawData->mFlag);
}

// Sep/18/2012, todo, maybe need to defined as virtual function in the future
int DSSRWFieldGroupModelIterator::getCurrentFormValueByID(DSSAttribute *ipAttr, GUID *ipFormGUID, AERawData *opRawData)
{
    int hr = S_OK;
    
    GUID lAttrGUID = ipAttr->getID();
    DSSAttributeFormWrappers *lpAttrFormWrappers = ipAttr->getFormWrappers();
    
    int lBaseFormID = -1;
    
    int lnAttrFormWrappers = lpAttrFormWrappers->Count();
    for (int iAttrFormWrappers = 0; iAttrFormWrappers < lnAttrFormWrappers; ++iAttrFormWrappers)
    {
        DSSAttributeFormWrapper *lpAttrFormWrapper = lpAttrFormWrappers->Item(iAttrFormWrappers);
        if (lpAttrFormWrapper->getID() == *ipFormGUID)
        {
            lBaseFormID = lpAttrFormWrapper->getBaseFormID();
            break;
        }
    }
    
    if (lBaseFormID < 0)
        return E_FAIL;
        
    int lUnitIndex = -1;
    hr = FindUnit(lAttrGUID, &lUnitIndex);
    CHECK_HR;
    
    int lGlobalKey = -1;
    hr = getElementIndex(mvRows[mCurrent], lUnitIndex, &lGlobalKey);
    CHECK_HR;
    
    if (DSSXTAB_TOTAL_KEY == lGlobalKey)
    {   
        const vector<MBase::CString> & lvTotalDispForFG = mpDataModel->getTotalDispForFG();
        // jimzhang Jan/10/2012 At current, server side only serializes one total display string for FieldGroup
        _ASSERTE (1 == lvTotalDispForFG.size());
        
        int lStrlen = lvTotalDispForFG[0].size();
        
        //yimliu, 2014/06/20, eliminate the possibility of memory leak.
        /*
         opRawData->mSizeInByte = lStrlen + 1;
         opRawData->mValue = new char [lStrlen + 1];
         CHECK_NEWED_PTR(opRawData->mValue);
         
         memmove(opRawData->mValue, lvTotalDispForFG[0].c_str(), lStrlen);
         opRawData->mValue[lStrlen] = 0;
         
         opRawData->mDataType = DssDataTypeVarChar;
         opRawData->mFlag = DssDataOk;
         */
        char * pValue = new char [lStrlen + 1];
        CHECK_NEWED_PTR(pValue);
        memmove(pValue, lvTotalDispForFG[0].c_str(), lStrlen);
        pValue[lStrlen] = 0;
        AERawData lRawData(pValue, lStrlen + 1, DssDataTypeVarChar, DssDataOk, true);
        opRawData->Assign(lRawData);
        
        return S_OK;
    }
    
    DSSTabularAttribute *lpTabularAttr = mpDataModel->GetTabularAttributeByGUID(lAttrGUID);
    CHECK_PTR(lpTabularAttr);
    
    int lColumnID = 0;
    hr = lpTabularAttr->FindFormWithLocale(0, lBaseFormID, 0, lColumnID);
    CHECK_HR;
    
    DSSDataColumn *lpColumn = NULL;
    hr = lpTabularAttr->Item(lColumnID, &lpColumn);
    CHECK_HR;
    
    // Check if this element is attribute element, DE leaf element or DE branch element
    
    DSSGlobalLookUpTable* lpLookUp = dynamic_cast<DSSGlobalLookUpTable*>(lpTabularAttr);
    CHECK_PTR(lpLookUp);
    
    DSSTabularUnit* lpTabularUnit = NULL;
    int lLocalKey = -1;
    bool lfDE = false;
    hr = lpLookUp->GetGlobalUnit(lGlobalKey, &lpTabularUnit, &lLocalKey, &lfDE);                    
    CHECK_HR;
    
    if (lfDE)
    {
        DSSTabularConsolidation* lpCS = dynamic_cast<DSSTabularConsolidation*>(lpTabularUnit);
        CHECK_PTR(lpCS);
        
        if (lpCS->IsLeaf(lLocalKey) /*&& lpCS->IsDisplayed(lLocalKey)*/) 
        {
            lGlobalKey = lpCS->getChildElementKey(lLocalKey);
        }
        else
        {
            // jimzhang, Nov/23/2012, TQMS 672295 670300, if DE element is a branch element, treat its value is NULL as server side. [todo] However, for IsNotNULL(DE@DESC), its result is alway false for branch element, and I think this behaviour is not proper.
            opRawData->mSizeInByte = 0;
            opRawData->mValue = NULL;
            opRawData->mDataType = DssDataTypeReserved;
            opRawData->mFlag = DssDataRWInvalid;
            
            /*XTabConsolidationElement* lpCSElement = lpCS->getElement(lLocalKey);
           
            int lStrlen = lpCSElement->value.size();
            opRawData->mSizeInByte = lStrlen + 1;
            opRawData->mValue = new char [lStrlen + 1];
            CHECK_NEWED_PTR(opRawData->mValue);
            
            memmove(opRawData->mValue, lpCSElement->value.c_str(), lStrlen);
            opRawData->mValue[lStrlen] = 0;
            
            opRawData->mDataType = DssDataTypeVarChar;
            opRawData->mFlag = DssDataOk;*/
            
            return S_OK;
        }
    }
    
    return lpColumn->getData(lGlobalKey, (const char **)(& opRawData->mValue), opRawData->mSizeInByte, opRawData->mDataType, opRawData->mFlag);
}

// Sep/18/2012, todo, maybe need to defined as virtual function in the future
int DSSRWFieldGroupModelIterator::getCurrentElementProxyByObjInfo(DSSObjectInfo *ipObjInfo, DSSBaseElementProxy **oppCurElementProxy)
{
    int hr = S_OK;
    
    if (!ipObjInfo || !oppCurElementProxy)
        return E_POINTER;
    
    GUID lTargetGUID = ipObjInfo->getID();
    EnumDSSObjectType lTargetType = ipObjInfo->getType();
    
    int lUnitIndex = -1;
    hr = FindUnit(lTargetGUID, &lUnitIndex);
    CHECK_HR;
    
    int lGlobalKey = -1;
    hr = getElementIndex(mvRows[mCurrent], lUnitIndex, &lGlobalKey);
    CHECK_HR;
    
    // jmzhang Jun/4/2012 If a non-detail section defined section formating(specail field group), and contians no field, 
    //      both view dataset and out-of-level view dataset may be empty. In this case, global key of attribute in section formating
    //      expression will be -1.
    if (lGlobalKey >= 0)
    {
        DSSTabularUnit* lpTabularUnit = NULL;
        int lLocalKey = -1;
        
        if (DssTypeAttribute == lTargetType)
        {
            DSSTabularAttribute *lpTabularAttr = mpDataModel->GetTabularAttributeByGUID(lTargetGUID);
            CHECK_PTR(lpTabularAttr);
            
            DSSGlobalLookUpTable* lpLookUp = dynamic_cast<DSSGlobalLookUpTable*>(lpTabularAttr);
            CHECK_PTR(lpLookUp);
            
            bool lfHasDE = false;                    
            hr = lpLookUp->GetGlobalUnit(lGlobalKey, &lpTabularUnit, &lLocalKey, &lfHasDE);             
            if (lfHasDE)
                return E_FAIL;
        }
        else if (DssTypeFilter == lTargetType || DssTypeConsolidation == lTargetType)
        {
            hr = mpDataModel->GetTabularUnit(lTargetGUID, lGlobalKey, &lpTabularUnit, &lLocalKey);
            CHECK_HR;
        }
        else
        {
            return E_FAIL;
        }
        
        CHECK_PTR(lpTabularUnit);
        hr = lpTabularUnit->createElement(lLocalKey, mpDataModel->getObjectContext(), oppCurElementProxy);
        CHECK_PTR(*oppCurElementProxy);
    }
    else
    {
        return S_FALSE;
    }
    
    return hr;
}

int DSSRWFieldGroupModelIterator::getTag(int * opTag)
{
	if (!opTag)
        return E_POINTER;
	
    *opTag = -1;
    
	if (mCurrent >= mnRows)
		return E_UNEXPECTED;
    
    if (!mpIndexTable)
    {
        *opTag = 0;
        return S_OK;
    }
    
    int hr = S_OK;
    
    int iUnit, nUnit = mpIndexTable->mvColumnMap.size();
    // jimzhang Apr/25/2012 583553 For special field group, row count of joined index may be 0. In this case, just return S_OK.
    if ( 0 < nUnit && ! mpIndexTable->mnRow)
        return S_OK;
    
    for(iUnit = 0; iUnit < nUnit; iUnit ++)
    {
        int lGlobal = -1;
        
        //TQMS 728292,728351, hyan, 3/19/2013
        hr = mpIndexTable->GetKey(mvRows[mCurrent], iUnit, &lGlobal);
        CHECK_HR;
        
        if (DSSXTAB_TOTAL_KEY == lGlobal)
        {
            // ToDo hardcode value 1 for now.
            *opTag = 1;
            return S_OK;
        }
    }
    
	return S_OK;
}

int DSSRWFieldGroupModelIterator::FindUnit(GUID &irUnitID, int *opUnit)
{
    if (NULL == opUnit)
        return E_POINTER;
    
    *opUnit = mpDataModel->FindUnit(irUnitID);
    if (*opUnit < 0)
        return E_FAIL;
    
    return S_OK;    
}

// jimzhang May/15/2012 
// Description: 
//      getElementIndexFromIndexTable() don't check if the element is compound NULL, and the caller should do this check.
// @Para2: unit's local index in joined index table
int DSSRWFieldGroupModelIterator::getElementIndexFromIndexTable(int iRow, int iUnitLocalIndex, int *opElementIndex)
{
    if (iUnitLocalIndex < 0)
        return E_INVALIDARG;
    
    if (!mpIndexTable)
        return E_POINTER;
    
    if (! mpIndexTable->mnRow)
        return S_FALSE;
    
    return mpIndexTable->GetKey(iRow, iUnitLocalIndex, opElementIndex);
}

// jimzhang May/15/2012
//      getElementIndexFromBaseIndexTable() don't check if the element is compound NULL, and the caller should do this check.
// @Para2: unit's local index in joined index table
int DSSRWFieldGroupModelIterator::getElementIndexFromBaseIndexTable(int iRow, int iUnitLocalIndex, int *opElementIndex)
{
    if (iUnitLocalIndex < 0)
        return E_INVALIDARG;
    
    JoinedIndexTable *lpIndexTable = NULL;
    int hr = hGetBaseIndexTable(&lpIndexTable);
    CHECK_HR;
    
    return lpIndexTable->GetKey(iRow, iUnitLocalIndex, opElementIndex);
}

int DSSRWFieldGroupModelIterator::getElementIndex(int iRow, int iUnit, int *opElementIndex)
{
	int hr = S_OK;
    
	DSSGUnitContainer *lpContainer = NULL;
	hr = mpDataModel->GetGlobalLookupContainer(&lpContainer);
	CHECK_PTR(lpContainer);
    
    int lGlobalRowCount = -1;        
    hr = lpContainer->GetGlobalRowCount(iUnit, &lGlobalRowCount, false);
    CHECK_HR;
    
	int lLocalID = -1;
    
	if (mpIndexTable)
	{
		hr = getLocalUnitID(iUnit, &lLocalID);
		if (lLocalID > -1)
		{
			hr = getElementIndexFromIndexTable(iRow, lLocalID, opElementIndex);
			CHECK_HR;
            
            // check if this element is compound NULL
			if (lGlobalRowCount == *opElementIndex)
                *opElementIndex = -1;
            
			return S_OK;
		}
	}
	
	JoinedIndexTable* lpIndexTable = NULL;
	hr = hGetBaseIndexTable(&lpIndexTable);
	CHECK_HR;
    
    if (lpIndexTable)
    {
        hr = getLocalUnitID(iUnit, &lLocalID, true);
        CHECK_HR;
        
        if (! lpIndexTable->mnRow)
            return S_FALSE;
        
        hr = lpIndexTable->GetKey(mBaseCurrent, lLocalID, opElementIndex);
        CHECK_HR;
        
        // check if this element is compound NULL
        if (lGlobalRowCount == *opElementIndex)
            *opElementIndex = -1;
    }
    else
    {
        // jimzhang Jun/4/2012 If one section only defined section formating(special field group), and contains no field, 
        //      the view dataset and out-of-level view dataset maybe empty. In this case, both "mpIndexTable" and "mpBaseIndexTable" are NULL.
        *opElementIndex = -1;
    }
    
	return S_OK;
}

int DSSRWFieldGroupModelIterator::getLocalUnitID(int iUnit, Int32 *oplocalUnitID, bool ibUseBaesIndexTable)
{    
    *oplocalUnitID = -1;
	
	JoinedIndexTable *lpIndexTable = ibUseBaesIndexTable ? mpBaseIndexTable : mpIndexTable;
    
    int lnUnits = lpIndexTable->mvColumnMap.size();
	if (lnUnits > 0)
	{
		for (int i = 0; i < lnUnits; ++i)
		{
			if (lpIndexTable->mvColumnMap[i] == iUnit)
			{
				*oplocalUnitID = i;
				break;
			}
		}
	}
    
	return S_OK;
}

// jimzhang nov/24/2011 ToDo
int DSSRWFieldGroupModelIterator::CountFields()
{
    return mnFields;
}

int DSSRWFieldGroupModelIterator::CountRows()
{
    return mnDisplayRows;
}

int DSSRWFieldGroupModelIterator::CountJoinedIndexRows()
{
    //tliao, 818713, we need to check whether this fieldgroup model is under groupby section
    DSSRWSectionModelIterator* lpParentModelIterator = DSSRWNodeModelIterator::mpParentIterator;
    while (NULL != lpParentModelIterator)
    {
        EnumDSSRWSectionType sectionType = (EnumDSSRWSectionType)lpParentModelIterator->getSectionType();
        if (sectionType == DssRWSectionGroupBySection)
        {
            //977369, use CountRows() when pIndexTable is NULL or empty
            int lnRows = (NULL==mpIndexTable ? 0 : mpIndexTable->mnRow);
            return lnRows > 0 ? lnRows : CountRows();
        }
        
        lpParentModelIterator = lpParentModelIterator->DSSRWNodeModelIterator::mpParentIterator;
    }
    
    return CountRows();
}

bool DSSRWFieldGroupModelIterator::FirstRow()
{
    mCurrent = 0;
	return mnDisplayRows > 0;
}

bool DSSRWFieldGroupModelIterator::NextRow()
{
    mCurrent++;
	return mCurrent < mnDisplayRows;
}

FieldGroupCell DSSRWFieldGroupModelIterator::getFieldGroupCell(int iField)
{
    return FieldGroupCell(mCurrent, iField, this);
}

FieldGroupCell DSSRWFieldGroupModelIterator::getFieldGroupCell(int iRow, int iField)
{
    return FieldGroupCell(iRow, iField, this);
}

bool DSSRWFieldGroupModelIterator::getNullChecking()
{ 
    return mpDataModel->getNullChecking(); 
}

bool DSSRWFieldGroupModelIterator::getTrimString()
{
    return mpDataModel->getTrimString();
}

int DSSRWFieldGroupModelIterator::getThresholdID(int iRow, int iField)
{
    return mvThresholdID[iRow * mnFields + iField];
}

int DSSRWFieldGroupModelIterator::getFieldText(int iRow, int iField, MBase::CString &orText)
{
    if (iRow < 0 || iRow >= mnRows)
		return E_INVALIDINDEX;

    if (!mbHasTransactionEnabled)
    {
        orText = mvFeildsText[iRow * mnFields + iField];
    }
    else
    {
        DSSDataModelTransactionRWField *lpTransactionModel = dynamic_cast<DSSDataModelTransactionRWField *>(getTransactionDataModel());
        CHECK_PTR(lpTransactionModel);
        int lBlockOff = lpTransactionModel->getBlockOffByGBElements(mvGroupByElements, mnRows);
        
        MBase::CString *lpNewText = NULL;
        if (!(lpTransactionModel 
              && mvMapToWBColumn[iField] >= 0 
              && (lpNewText = lpTransactionModel->getUpdatedAndSubmittedData(iRow+lBlockOff, iField, mvMapToWBColumn[iField],false))   //640661 check submitted data change as well
              ))
        {
            orText = mvFeildsText[iRow * mnFields + iField];
        }
        else
        {
            //603945, get the changed text from transaction data model
            //return lpTransactionModel->getUpdatedValue(iRow, iField, mvMapToWBColumn[iField], orText);
            if (lpNewText)
                orText = *lpNewText;
        }
    }
	
    return S_OK;
}

int DSSRWFieldGroupModelIterator::hGetBaseIndexTable(JoinedIndexTable **oppIndexTable)
{
    int hr = S_OK;
    
	if (NULL != mpBaseIndexTable)
	{
		*oppIndexTable = mpBaseIndexTable;
		return S_OK;
	}
    
    //ctang: refine the group-by elements according to the join index table
    vector< vector<Int32> > lvvOLGroupByElements;
	hr = hRefineGBElementsForOLView(&lvvOLGroupByElements);
	CHECK_HR;
    
    DSSRWFieldGroupModel *lpFGModel = static_cast<DSSRWFieldGroupModel *>(DSSRWNodeModelIterator::mpNodeModel);
    CHECK_PTR(lpFGModel);
    
    vector<Int32> lvSortUnits;
    hr = lpFGModel->hSliceAndJoin(lpFGModel->mvGroupByUnits, lvvOLGroupByElements, true, &mpBaseIndexTable, false, lvSortUnits);
    CHECK_HR;        
    
	*oppIndexTable = mpBaseIndexTable;
    
	if (NULL == mpBaseIndexTable)
		return S_OK;
    
    // apply sort to mpBaseIndexTable   
    mBaseCurrent = 0;
    Int32 lnRow = mpBaseIndexTable->mnRow;
    if (lnRow > 1)
    {   
        DSSRWFieldGroupCompare lCmp;
        //hr = lCmp.Init(this, mpDataModel->GetDisplayNullOnTop(), mvSortUnits, true);
        hr = lCmp.Init(this, false, mvSortUnits, true); // jimzhang, Feb/6/2013, TQMS 715018, follow server side to set 2nd parameter to false.
		CHECK_HR;
        
        // jimzhang Mar/6/2012 ToDo Implement below logic of server. There is no method to implement GetUserLocale() on iPad currently.
        /*//set user data locale for lCmp
        Int32 lLocale = DssLocaleNeutral;
        hr = mpDataModel->GetUserLocale(&lLocale);
        CHECK_HR;
        
        lCmp.InitLocale(lLocale);
        
        // lwang, 08/14/2007, TQMS 276457: call _xtab_sort_mcm() to use MCM governed buffer 
        MBase::Buffer* lpBuffer = MContractMgr::CreateGovernedBuffer(1, 1, _T("CDSSRWFieldGroupModelIterator"));
        if(!lpBuffer)	// if unsuccessful, we assume it is 2-tier and we create normal buffer
            lpBuffer = MBase::CreateBuffer(1, 1);
        MBase::BufferPtr lBufferMCMPtr(lpBuffer);*/
        
        // jimzhag Mar/6/2012 Below two lines of code is dummy.
        Int32 lLocale = 0;
        lCmp.InitLocale(lLocale);
        
        vector<Int32> lvRow(lnRow);
        for (Int32 i = 0; i < lnRow; i++)
            lvRow[i] = i;
        
        EnumDSSFieldGroupDataLevel lDataLevel = lpFGModel->mDataLevel;
        
        //junwang, 05/08/2012, peformance optimization
        if (lDataLevel == DssFieldGroupDataLevelDefaultReport) 
            hr =  _xtab_sort(&lvRow[0], &lvRow[0] + lnRow, lCmp);
        else
            hr = _xtab_first_n(&lvRow[0], &lvRow[0] + lnRow, lCmp, 1, false);
        CHECK_HR;
        
        mBaseCurrent = lvRow[0];
    }
    
	return S_OK;
}

int DSSRWFieldGroupModelIterator::hRefineGBElementsForOLView(vector< vector<Int32> > *opvvOLGroupByElements)
{
    HRESULT hr = S_OK;
    
    if (! opvvOLGroupByElements)
        return E_POINTER;
    
	if (!mpIndexTable)
	{
        *opvvOLGroupByElements = mvGroupByElements;
		return S_OK;
	}
    
    DSSRWFieldGroupModel *lpFGModel = dynamic_cast<DSSRWFieldGroupModel *>(DSSRWNodeModelIterator::mpNodeModel);
    CHECK_PTR(lpFGModel);
    
	Int32 lRow = mvRows[mCurrent];
	Int32 lLocalUnitID = -1;
	
	for (Int32 iUnit = 0; iUnit < lpFGModel->mvGroupByUnits.size(); iUnit++)
	{
		Int32 lUnit;
		hr = FindUnit(lpFGModel->mvGroupByUnits[iUnit], &lUnit);
		if (hr == S_OK)
		{
            hr = getLocalUnitID(lUnit, &lLocalUnitID, false);
			CHECK_HR;
            
			if (lLocalUnitID >= 0)
			{
				// rzhu, check if the row index is out of the range
				if (lRow >= 0 && lRow < mpIndexTable->mnRow )
                {
                    Int32 lElement;
                    hr = mpIndexTable->GetKey(lRow, lLocalUnitID, &lElement);
                    CHECK_HR;
                    
                    vector<Int32> lvTmp;
                    lvTmp.push_back(lElement);
                    opvvOLGroupByElements->push_back(lvTmp);
                    
                    continue;
                }
			}
		}
		
        opvvOLGroupByElements->push_back(mvGroupByElements[iUnit]);
	}
    
	return S_OK;
}

bool DSSRWFieldGroupModelIterator::HasLinks(int iRow,int iField)
{
    if(mvLinkFieldText[iRow*mnFields+iField]!=NULL)
        return true;
    
    return false;    
}

int DSSRWFieldGroupModelIterator::getLinkText(int iRow,int iField,EnumDSSRWLinkType iType, std::string &orLinkText)
{
    FieldLink *lpFieldLink=mvLinkFieldText[iRow*mnFields+iField];
    
    if(lpFieldLink)
    {
        int lnsize=lpFieldLink->mtype.size();
        
        for(int i=0;i<lnsize;i++)
        {
            if(iType==lpFieldLink->mtype[i])
            {   
                orLinkText=lpFieldLink->mtext[i]->c_str();
                return S_OK;
            }
        }
    }
    
    //921706, lwang, 7/16/2014, return S_FALSE if the link type is not found
    return S_FALSE;
}

// jimzhang Feb/24/2012
// Description
//      This interface is used by DSSRWFieldGroupCompare.
// @Param1: reprot index which is equal to DSSRTTemplateInfo::mReportIndex
// @Param3: metric index in DSSViewDataSet::mvMetricGUID
int DSSRWFieldGroupModelIterator::getMetricPos(int iReport, GUID &irMetricGUID, int &orMetricPos)
{
    DSSRWFieldGroupModel *lpFGModel = static_cast<DSSRWFieldGroupModel *>(DSSRWNodeModelIterator::mpNodeModel);
    CHECK_PTR(lpFGModel);
    
    vector<DSSRTTemplateInfo*> &lvRTT = lpFGModel->mvpRTTemplateInfo; 
    int lnRTT = lvRTT.size();
    for (int iRTT = 0; iRTT < lnRTT; ++iRTT)
    {
        if (iReport == lvRTT[iRTT]->mReportIndex)
        {                        
            DSSViewDataSet *lpViewDataset = lvRTT[iRTT]->mViewPtr;
            DSSTabularData *lpTabularData = lpViewDataset->getTabularData();
            int lnMetric = lpTabularData->getMetricsCount();
            for (int lMetricIndex = 0; lMetricIndex < lnMetric; ++lMetricIndex)
            {
                if (irMetricGUID == lpTabularData->getMetricID(lMetricIndex))
                {
                    orMetricPos = lMetricIndex;
                    return S_OK;
                }
            }
            
            return S_FALSE;
        }
    }
    
    return S_FALSE;
}

// @Param1: reprot index which is equal to DSSRTTemplateInfo::mReportIndex
// @Param2: index of RTTemplateInfo in DSSRWFieldGroupModel::mvpRTTemplateInfo
int DSSRWFieldGroupModelIterator::getRTTIndex(int iReport, int &orRTTIndex)
{
    DSSRWFieldGroupModel *lpFGModel = static_cast<DSSRWFieldGroupModel *>(DSSRWNodeModelIterator::mpNodeModel);
    CHECK_PTR(lpFGModel);
    
    vector<DSSRTTemplateInfo*> &lvRTT = lpFGModel->mvpRTTemplateInfo; 
    int lnRTT = lvRTT.size();
    for (int iRTT = 0; iRTT < lnRTT; ++iRTT)
    {
        if (iReport == lvRTT[iRTT]->mReportIndex)
        {
            orRTTIndex = iRTT;
            return S_OK;
        }
    }
    
    return S_FALSE;
}

int DSSRWFieldGroupModelIterator::hGetMetricIndex(GUID &irSourceGUID, GUID &irMetricGUID, int &orRTTIndex, int &orMetricIndex)
{
    DSSRWFieldGroupModel *lpFGModel = static_cast<DSSRWFieldGroupModel *>(DSSRWNodeModelIterator::mpNodeModel);
    CHECK_PTR(lpFGModel);
    
    vector<DSSRTTemplateInfo*> &lvRTT = lpFGModel->mvpRTTemplateInfo; 
    int lnRTT = lvRTT.size();
    
    if (GUID_NULL == irSourceGUID)
    {
        DSSObjectContext* lpObjContext = mpDataModel->getObjectContext();
        CHECK_PTR(lpObjContext);
    
        DSSDataElements* lpDataElements = lpObjContext->getDataElements();
        CHECK_PTR(lpDataElements);
    
        DSSDataElement *lpDataElement = lpDataElements->getDefaultDataElement();
        CHECK_PTR(lpDataElement);
        
        irSourceGUID = lpDataElement->getDatasetID();
    }
    
    for (int lRTTIndex = 0; lRTTIndex < lnRTT; ++lRTTIndex)
    {
        if (irSourceGUID == lvRTT[lRTTIndex]->mReportID)
        {
            orRTTIndex = lRTTIndex; 
            
            DSSViewDataSet *lpViewDataset = lvRTT[lRTTIndex]->mViewPtr;
            if (! lpViewDataset)
                return S_FALSE;
            
            int lnMetric = lpViewDataset->getTabularData()->getMetricsCount();
            for (int lMetricIndex = 0; lMetricIndex < lnMetric; ++lMetricIndex)
            {
                if (irMetricGUID == lpViewDataset->getTabularData()->getMetricID(lMetricIndex))
                {
                    orMetricIndex = lMetricIndex;
                    return S_OK;
                }
            }
            
            return E_FAIL;
        }
    }
    
    return E_FAIL;
}

DSSDataModelTransaction* DSSRWFieldGroupModelIterator::getTransactionDataModel()
{
    if(!mpNodeModel)
        return S_OK;
	DSSDataModelTransaction *lpTransactionModel = (static_cast<DSSRWFieldGroupModel *>(mpNodeModel))->getTransactionDataModel();
	if (!lpTransactionModel)
	{
		DSSRWFieldGroupNode * lpThisNode = static_cast<DSSRWFieldGroupNode*> (mpNode);
		if (lpThisNode->IsTransactionEnabled())
		{
			lpTransactionModel = new DSSDataModelTransactionRWField();
            
            DSSTransactionReport *lpTargetReport = lpThisNode->getTransactionReport();
            
            int hr = lpTransactionModel->InitWithNodeModel(lpTargetReport, mpNodeModel);
            if (hr == S_OK)
            {
                mpNodeModel->setTransactionDataModel(lpTransactionModel);
            }
            else
            {
                _ASSERT(false);
                delete lpTransactionModel;
            }
		}
	}
	return lpTransactionModel;
}

int DSSRWFieldGroupModelIterator::UpdateData(DSSCellHandle &irHandle, CComVariant &irNewValue)
{
    DSSDataModelTransactionRWField *lpTransactionModel = dynamic_cast<DSSDataModelTransactionRWField *>(getTransactionDataModel());
	if (!lpTransactionModel || (mvMapToWBColumn.empty() && mvOfflineMapToWBColumn.empty()))
		return E_FAIL;
	
	AE_ASSERT(irHandle.type == DssCellHandleField);
	
	int lnFields = mnFields;
	AE_ASSERT(lnFields > 0);
	
	int row = irHandle.v.field / lnFields;
	int col = irHandle.v.field % lnFields;
	
	vector<AERawData> lvOriginalData;
    //DE54514, msun, get the original value of DDIC instead of the current selected new value
    bool lbGetDDICOriginal = true;
	int hr = hGetFieldData(row, lvOriginalData, lbGetDDICOriginal);
	CHECK_HR;
	
	VariantChangeType(&irNewValue, &irNewValue, 0, VT_BSTR);
	std::string lText = MBase::WideCharToUTF8(irNewValue.bstrVal).c_str();
	
    int lBlockOff = lpTransactionModel->getBlockOffByGBElements(mvGroupByElements, mnRows);
    
    //dummy, not used for FG
    vector<AERawData> lvDisplayData(lvOriginalData.size());
    
    int lColumnIndex = mvMapToWBColumn[col];
    if(lColumnIndex < 0 && col < mvOfflineMapToWBColumn.size())
    {
        lColumnIndex = mvOfflineMapToWBColumn[col];
        if(lColumnIndex >= 0)
        {
            lColumnIndex += mvOfflineMapToFields.size();
        }
    }
	hr = lpTransactionModel->AddRecord(row+lBlockOff, col, lColumnIndex, lText, &lvOriginalData,&lvDisplayData);
	CHECK_HR;
	
	hr = lpTransactionModel->UpdateField(row+lBlockOff, col, lColumnIndex, &lText);
	CHECK_HR;
	
	return S_OK;
}

int DSSRWFieldGroupModelIterator::DiscardTransactionChange(DSSCellHandle &irHandle)
{
    int hr = S_OK;
    
    DSSDataModelTransactionRWField *lpTransactionModel = dynamic_cast<DSSDataModelTransactionRWField *>(getTransactionDataModel());
	if (!lpTransactionModel || (mvMapToWBColumn.empty() && mvOfflineMapToWBColumn.empty()))
		return E_FAIL;
	
	AE_ASSERT(irHandle.type == DssCellHandleField);
	
	int lnFields = mnFields;
	AE_ASSERT(lnFields > 0);
	
	int row = irHandle.v.field / lnFields;
	int col = irHandle.v.field % lnFields;
	
    int lBlockOff = lpTransactionModel->getBlockOffByGBElements(mvGroupByElements, mnRows);
    

    
    int lColumnIndex = mvMapToWBColumn[col];
    if(lColumnIndex < 0 && col < mvOfflineMapToWBColumn.size())
    {
        lColumnIndex = mvOfflineMapToWBColumn[col];
        if(lColumnIndex >= 0)
        {
            lColumnIndex += mvOfflineMapToFields.size();
        }
    }
	hr = lpTransactionModel->DiscardTransactionOperation(row+lBlockOff, col, lColumnIndex);
	CHECK_HR;
	
	return S_OK;
}

int DSSRWFieldGroupModelIterator::MarkRows(int iRowOrdinal, EnumDSSRWMarkRowAction iActionType)
{
    DSSDataModelTransactionRWField *lpTransactionModel = dynamic_cast<DSSDataModelTransactionRWField *>(getTransactionDataModel());
	if (!lpTransactionModel || (mvMapToWBColumn.empty() && mvOfflineMapToWBColumn.empty()))
		return E_FAIL;
	
	vector<AERawData> lvOriginalData;
	int hr = hGetFieldData(iRowOrdinal, lvOriginalData);
	CHECK_HR;
    
    int lBlockOff = lpTransactionModel->getBlockOffByGBElements(mvGroupByElements, mnRows);
    
    //dummy, not used for FG
    vector<AERawData> lvDisplayData(lvOriginalData.size());
	if (DssRWMarkRowSelect == iActionType)
		return lpTransactionModel->MarkRecord(iRowOrdinal + lBlockOff, &lvOriginalData,&lvDisplayData, DssTransactionDataInsert);
	else if (DssRWMarkRowUnset == iActionType)
		return lpTransactionModel->MarkRecord(iRowOrdinal + lBlockOff, &lvOriginalData, &lvDisplayData,DssTransactionDataOriginal);
	else
		return E_INVALIDARG;
}

int DSSRWFieldGroupModelIterator::hGetFieldData(int iRow, vector<AERawData> &irvOriginalData, bool ibGetDDICOriginal)
{
	int hr = S_OK;
	
	int lnColumns = mvMapToFields.size() + mvOfflineMapToFields.size();
	irvOriginalData.resize(lnColumns);
	for (int i = 0; i < lnColumns; i++)
	{
        int lField = -1;
        //tliao, 866980, 3/26/2014, handle them one by one
        if (i < mvMapToFields.size())
        {
            lField = mvMapToFields[i];
        }
        else
        {
            lField = mvOfflineMapToFields[i-mvMapToFields.size()];
        }
 
		if (lField >= 0)
		{
			//473405, we need submit the raw data instead of formatted field text
			//hr = mpFieldGroupModel->getFieldText(iRow, mvMapToFields[i], lText);
            //fali, 827602
			hr = this->getRawData(iRow, lField,
											   (const char**)&irvOriginalData[i].mValue,
											   &irvOriginalData[i].mSizeInByte,
											   &irvOriginalData[i].mDataType,
											   &irvOriginalData[i].mFlag, ibGetDDICOriginal);
			CHECK_HR;
		}
	}
    
	return S_OK;
}

int DSSRWFieldGroupModelIterator::getDataChangeType(DSSCellHandle &irHandle, bool ibTrxSubPreset)
{
	_ASSERT(irHandle.type == DssCellHandleField);
	
    DSSDataModelTransactionRWField *lpTransactionModel = dynamic_cast<DSSDataModelTransactionRWField *>(getTransactionDataModel());
    if (! lpTransactionModel)
        return DssTransactionDataReserved;
    
	int lnFields = mnFields;
	
    EnumDSSTransactionDataChangeType lIsColumnRequired = DssTransactionDataReserved;
    EnumDSSTransactionDataChangeType lIsDataUpdated = DssTransactionDataOriginal;
    EnumDSSTransactionDataChangeType lIsToSubmit = DssTransactionDataReserved;
    
	if (lnFields > 0 && lpTransactionModel && !mvMapToWBColumn.empty())
	{
		int row = irHandle.v.field / lnFields;
		int col = irHandle.v.field % lnFields;
        
        int lColumnIndex = mvMapToWBColumn[col];
        if(lColumnIndex < 0 && col < mvOfflineMapToWBColumn.size())
        {
            lColumnIndex = mvOfflineMapToWBColumn[col];
            if(lColumnIndex >= 0)
            {
                lColumnIndex += mvOfflineMapToFields.size();
            }
        }
        
        if(lpTransactionModel->isColumnRequired(lColumnIndex))
            lIsColumnRequired = DssTransactionDataCommit;
        
        bool lbSubmit = true;
        bool* pbSubmit = lIsColumnRequired == DssTransactionDataCommit ? &lbSubmit : NULL;
        bool lbValidateEmpty = false;
        if(ibTrxSubPreset)
        {
            DSSRWNode* lpNode = DSSRWIterator::getRWNode();
            if(lpNode)
            {
                DSSRWFieldGroupNode* lpFGNode = dynamic_cast<DSSRWFieldGroupNode*>(lpNode);
                if(lpFGNode)
                {
                    lbValidateEmpty = lpFGNode->getTransactionRecordNumber() > 0;
                }
            }
        }
        int lBlockOff = lpTransactionModel->getBlockOffByGBElements(mvGroupByElements, mnRows);
		if (lpTransactionModel->IsDataUpdated(row + lBlockOff, col, mvMapToWBColumn[col], pbSubmit, lbValidateEmpty))
        {
            lIsDataUpdated = DssTransactionDataUpdate;
            if(pbSubmit && (*pbSubmit) == false)
                lIsToSubmit = DssTransactionDataOriginal;
        }
	}
	
	return lIsDataUpdated | lIsColumnRequired | lIsToSubmit;
}

void DSSRWFieldGroupModelIterator::ReleaseUnusedMemory()
{
    hCleanUp();
    
    if(mpNodeModel)
    {
        mpNodeModel->ReleaseUnusedMemory();
        mpNodeModel = NULL;
    }
	return;
}

void DSSRWFieldGroupModelIterator::hCleanUp()
{
    if (mpIndexTable)
    {
        delete mpIndexTable;
        mpIndexTable = NULL;
    }
    
    if (mpBaseIndexTable)
    {
        delete mpBaseIndexTable;
        mpBaseIndexTable = NULL;
    }
    
    if (mpFunctionServer)
    {
        delete mpFunctionServer;
        mpFunctionServer = NULL;
    }
    
    //mmCellIndex2RawData.clear();
    
    map<DSSExpNodeOperator *, DSSFunctionObject *>::iterator iter;
    for (iter = mmOperator2FunObj.begin(); iter != mmOperator2FunObj.end(); iter++)
    {
        if (iter->second)
            delete iter->second;
    }
    mmOperator2FunObj.clear();
    
    //zhyang, 856635, memory leak
    int lnLinkFieldText = mvLinkFieldText.size();
    for (int i = 0; i < lnLinkFieldText; ++i)
        delete mvLinkFieldText[i];
    mvLinkFieldText.clear();
    
    //fali, 960685, release allocated memory for AERawData
    if (mvpAllocatedRawData.size() > 0)
    {
        for (int i=0; i<mvpAllocatedRawData.size(); i++)
        {
            delete[] mvpAllocatedRawData[i];
            mvpAllocatedRawData[i] = NULL;
        }
        
    }
}

// jimzhang Jun/1/2012
// Description
//      (1) getRawData() only get raw data from below types of field:
//          <1> DssRWFieldObjectValue (metric & attribute with only one display form)
//          <2> DssRWFieldConcatenation (with only one subfield who is also a metric & attribute with only one display form). 
//      (2) Current logic use "map"(cell index -> raw data) to catch raw data. And this logic is based on below assumption:
//              <1> Only a few cells(belong to different field) needs to call getRawData().
//              <2> getRawData() may be called many times for one cell. 
int DSSRWFieldGroupModelIterator::getRawData(int iRow, int iField, const char **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag, bool ibGetDDICOriginal,bool ibDiscardUnSubmitTrx)
{
    _ASSERTE (oppData && opDataSize && opDataType && opDataFlag);
	
	int lCellIndex = iRow * mnFields + iField;
	if (lCellIndex < 0 || lCellIndex >= mnDisplayRows*mnFields)
		return E_INVALIDINDEX;
	   
    int lTrxColumn = -1;
    if(mvMapToWBColumn.size() > iField && mvMapToWBColumn[iField] >= 0)
    {
        lTrxColumn = mvMapToWBColumn[iField];
    }else if(mvOfflineMapToWBColumn.size() > iField && mvOfflineMapToWBColumn[iField] >= 0)
    {
        lTrxColumn = mvOfflineMapToWBColumn[iField] + mvOfflineMapToFields.size();
    }
    
    DSSRWFieldGroupModel *lpFGModel = dynamic_cast<DSSRWFieldGroupModel *>(DSSRWNodeModelIterator::mpNodeModel);
    CHECK_PTR(lpFGModel);
    
    DSSDataModelTransactionRWField *lpTrxModel = dynamic_cast<DSSDataModelTransactionRWField *>(lpFGModel->getTransactionDataModel());
    
	// Dec/17/2012, TQMS 684909, if this field is editable and has been updated, then convert its updated text to raw data and return
    if (mbHasTransactionEnabled && lpTrxModel && lTrxColumn >= 0)
    {
                    
        int lBlockOff = lpTrxModel->getBlockOffByGBElements(mvGroupByElements, mnRows);
        
        bool lfIsUpdated = lpTrxModel->IsDataUpdated(lBlockOff + iRow, iField, lTrxColumn);
        if (lfIsUpdated)
        {
            // Jun/17/2013, TQMS 759314, if a field is configured with DDIC and DDIC's style is control, we should not use DSSDataModelTransaction::getUpdatedAndSubmittedData() to get the new input sting and then converte the string to raw data, since DDIC of control style has write back form, and DSSDataModelTransaction::getUpdatedAndSubmittedData() will retuen text of display forms.
            if (mmField2WBForm[iField] >= 0)
            {
                DSSField *lpField = NULL;
                int hr = hFindField(iField, &lpField);
                CHECK_HR;
                
                hr = hCalculateRawData(iField, lpField, iRow, oppData, opDataSize, opDataType, opDataFlag, ibGetDDICOriginal);
                CHECK_HR;
                
                // Oct/16/2013, TQMS 801939, do not cache field's raw data, since these raw data may contain pointer which point to string in attribute column, and this string in attribute column may be deleted when editing Grid.
                /*AERawData lRawData;
                lRawData.mValue = (char *)(*oppData);
                lRawData.mSizeInByte = *opDataSize;
                lRawData.mDataType = *opDataType;
                lRawData.mFlag = *opDataFlag;
                mmCellIndex2RawData.insert(pair<int, AERawData>(lCellIndex, lRawData));*/
            }
            else
            {
                MBase::CString *lpNewStr = lpTrxModel->getUpdatedAndSubmittedData(lBlockOff + iRow, iField, lTrxColumn, false, ibDiscardUnSubmitTrx);
                if (lpNewStr)
                {
                    AERawData lRawData(NULL, mmTrxFieldDataSize[iField], mmTrxFieldDataType[iField], DssDataOk, true);
                    int hr = DSSAEHelper::ConvertMetricStr2RawData(*lpNewStr, &lRawData);
                    CHECK_HR;
                    
                    *oppData = lRawData.mValue;
                    *opDataSize = lRawData.mSizeInByte;
                    *opDataType = lRawData.mDataType;
                    *opDataFlag = lRawData.mFlag;
                    lRawData.SetOwnership(false);
                    
                    //fali, 960685, 2014/9/11. keep newly allocated AERawData in vector and release it when destructing. To avoid memory leak
                    mvpAllocatedRawData.push_back((char*)*oppData);
                    // Oct/16/2013, TQMS 801939, do not cache field's raw data, since these raw data may contain pointer which point to string in attribute column, and this string in attribute column may be deleted when editing Grid.
                    //mmCellIndex2RawData[lCellIndex] = lRawData;
                    
                    return hr;
                }
                else
                {
                   // return E_FAIL;
                }
            }
        } // END: if (lfIsUpdated)
        if(!lfIsUpdated) 
        {
            if(mmField2WBForm[iField] >= 0)
            {
                bool lfIsDiscarded = lpTrxModel->IsDataDiscarded(lBlockOff + iRow, iField, lTrxColumn);
                if(lfIsDiscarded)
                {
                    if(lpTrxModel->IsDataUpdatedOrSubmitted(lBlockOff + iRow, iField, lTrxColumn))
                    {
                        MBase::CString *lpNewStr = lpTrxModel->getUpdatedAndSubmittedData(lBlockOff + iRow, iField, lTrxColumn,false);
                        if (lpNewStr)
                        {
                            AERawData lRawData(NULL, mmTrxFieldDataSize[iField], mmTrxFieldDataType[iField], DssDataOk, true);
                            int hr = DSSAEHelper::ConvertMetricStr2RawData(*lpNewStr, &lRawData);
                            CHECK_HR;
                            
                            *oppData = lRawData.mValue;
                            *opDataSize = lRawData.mSizeInByte;
                            *opDataType = lRawData.mDataType;
                            *opDataFlag = lRawData.mFlag;
                            lRawData.SetOwnership(false);
                            
                            //fali, 960685, 2014/9/11. keep newly allocated AERawData in vector and release it when destructing. To avoid memory leak
                            mvpAllocatedRawData.push_back((char*)*oppData);
                            // Oct/16/2013, TQMS 801939, do not cache field's raw data, since these raw data may contain pointer which point to string in attribute column, and this string in attribute column may be deleted when editing Grid.
                            //mmCellIndex2RawData[lCellIndex] = lRawData;
                            
                            return hr;
                        }

                    }
                    
                    DSSField *lpField = NULL;
                    int hr = hFindField(iField, &lpField);
                    CHECK_HR;
                    
                    bool discardDDICElement = true;
                    
                    hr = hCalculateRawData(iField, lpField, iRow, oppData, opDataSize, opDataType, opDataFlag,discardDDICElement);
                    CHECK_HR;
                    
                    // Oct/16/2013, TQMS 801939, do not cache field's raw data, since these raw data may contain pointer which point to string in attribute column, and this string in attribute column may be deleted when editing Grid.
                    /*AERawData lRawData;
                    lRawData.mValue = (char *)(*oppData);
                    lRawData.mSizeInByte = *opDataSize;
                    lRawData.mDataType = *opDataType;
                    lRawData.mFlag = *opDataFlag;
                    mmCellIndex2RawData.insert(pair<int, AERawData>(lCellIndex, lRawData));*/
                }
            }else{
                //770467
                if(lpTrxModel->IsDataUpdatedOrSubmitted(lBlockOff + iRow, iField, lTrxColumn))
                {
                    MBase::CString *lpNewStr = lpTrxModel->getUpdatedAndSubmittedData(lBlockOff + iRow, iField, lTrxColumn,false);
                    if (lpNewStr)
                    {
                        AERawData lRawData(NULL, mmTrxFieldDataSize[iField], mmTrxFieldDataType[iField], DssDataOk, true);
                        int hr = DSSAEHelper::ConvertMetricStr2RawData(*lpNewStr, &lRawData);
                        CHECK_HR;
                        
                        *oppData = lRawData.mValue;
                        *opDataSize = lRawData.mSizeInByte;
                        *opDataType = lRawData.mDataType;
                        *opDataFlag = lRawData.mFlag;
                        lRawData.SetOwnership(false);
                        
                        //fali, 960685, 2014/9/11. keep newly allocated AERawData in vector and release it when destructing. To avoid memory leak
                        mvpAllocatedRawData.push_back((char*)*oppData);
                        // Oct/16/2013, TQMS 801939, do not cache field's raw data, since these raw data may contain pointer which point to string in attribute column, and this string in attribute column may be deleted when editing Grid.
                        //mmCellIndex2RawData[lCellIndex] = lRawData;
                        
                        return hr;
                    }
                }
            }
            
        }
    }

    // Oct/16/2013, TQMS 801939, do not cache field's raw data, since these raw data may contain pointer which point to string in attribute column, and this string in attribute column may be deleted when editing Grid.
    /*map<int, AERawData>::iterator lIter = mmCellIndex2RawData.find(lCellIndex);
    if (mmCellIndex2RawData.end() != lIter)
    {
        AERawData &lrRawData = lIter->second;
        *oppData = lrRawData.mValue;
        *opDataSize = lrRawData.mSizeInByte;
        *opDataType = lrRawData.mDataType;
        *opDataFlag = lrRawData.mFlag;
        
        return S_OK;
	}*/

    
    DSSField *lpField = NULL;
    int hr = hFindField(iField, &lpField);
    CHECK_HR;
    
    hr = hCalculateRawData(iField, lpField, iRow, oppData, opDataSize, opDataType, opDataFlag, ibGetDDICOriginal);
    CHECK_HR;
    
    // Oct/16/2013, TQMS 801939, do not cache field's raw data, since these raw data may contain pointer which point to string in attribute column, and this string in attribute column may be deleted when editing Grid.
    /*AERawData lRawData;
    lRawData.mValue = (char *)(*oppData);
	lRawData.mSizeInByte = *opDataSize;
	lRawData.mDataType = *opDataType;
	lRawData.mFlag = *opDataFlag;
    mmCellIndex2RawData.insert(pair<int, AERawData>(lCellIndex, lRawData));*/
    
    // jimzhang Jun/1/2012 comment out below code for reference
	/*if (*opDataType == DssDataTypeMissing)
	{
		//475653, if the raw data is null, try to find out the data type from other not-null value
		int count = mvRawData.size();
		for (int i = 0; i < mnRows; i++)
		{
			int lIndex2 = i * mnFields + iField;
			if (lIndex2 < count && mvRawData[lIndex2].mDataType != DssDataTypeMissing)
			{
				*opDataType = mvRawData[lIndex2].mDataType;
				break;
			}
		}
	}*/
	
	return hr;
}

int DSSRWFieldGroupModelIterator::hCalculateRawData(int iMainField, DSSField *ipField, int iRow, const char **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag, bool ibDiscardOrOriginalDDICElement)
{
    int hr = S_OK;
    
    std::string lKey = ipField->getKey();
    EnumDSSRWFieldType lType = ipField->getType();
    switch (lType)
    {
        case DssRWFieldObjectValue:
        {
            DSSObjectInfo* lpObjInfo = ipField->getFieldObject(); 
            _ASSERTE(lpObjInfo);
            GUID lObjID = lpObjInfo->getID();
            EnumDSSObjectType lObjType = lpObjInfo->getType();
            switch (lObjType)
            {
                case DssTypeMetric:
                {
                    map<MBase::CString, int>::iterator iter1 = mmFieldToRTTIndex.find(lKey);
                    if (iter1 == mmFieldToRTTIndex.end())
                        return E_FAIL;
                    
                    map<MBase::CString, int>::iterator iter2 = mmFieldToMetricIndex.find(lKey);
                    if (iter2 == mmFieldToMetricIndex.end())
                        return E_FAIL;
                    
                    hr = getMetricValue(mvRows[iRow], iter1->second, iter2->second, *oppData, *opDataSize, *opDataType, *opDataFlag);
                    CHECK_HR;
                    
                }
                    break;
                case DssTypeAttribute:
                {
                    DSSTabularAttribute *lpTabularAttr = NULL;
                    DSSGlobalLookUpTable *lpLookUp = NULL;
                    
                    int lBaseFormID = -1;
                    int lGlobalKey = -1;
                    
                    bool lfIsFromDDICSource = false;
                    if (mmField2WBForm.size() > 0 && mmField2WBForm[iMainField] != -1)
                        lfIsFromDDICSource = true;
                    
                    if (lfIsFromDDICSource)
                    {
                        lBaseFormID = mmField2WBForm[iMainField];
                        
                        DSSRWFieldGroupNode *lpThisNode = dynamic_cast<DSSRWFieldGroupNode *>(mpNode);
                        if (NULL == lpThisNode)
                            return E_FAIL;
                        
                        DSSFields *lpFields = lpThisNode->getFields();
                        CHECK_PTR(lpFields);
                        
                        DSSField *lpField = lpFields->Item(iMainField);
                        DSSRWControl *lpControl = lpField->getInputControl();
                        if(lpControl->getDataSourceID() == GUID_NULL)
                        {
                            lBaseFormID = -1;
                            lfIsFromDDICSource = false;
                        }
                        
                        DSSRWFieldGroupModel *lpFGModel = static_cast<DSSRWFieldGroupModel *>(DSSRWNodeModelIterator::mpNodeModel);
                        CHECK_PTR(lpFGModel);
                        
                        map<string, int>::iterator iter = lpFGModel->mControlKeyToIndexMap.find(lpControl->getKey());
                        if (iter == lpFGModel->mControlKeyToIndexMap.end())
                        {
                            *oppData = NULL;
                            *opDataSize = 0;
                            *opDataType = DssDataTypeReserved;
                            *opDataFlag = DssDataNull;
                            
                            return S_OK;
                        }
                        
                        vector<DSSTabularAttribute *> &lrvSrcAttr = lpFGModel->mvvDDICAttr[iter->second];
                        GUID lSrcAttrGUID = lpControl->getSourceID();
                        int iSrcAttr = 0;
                        for (; iSrcAttr < lrvSrcAttr.size(); ++iSrcAttr)
                        {
                            if (NULL != lrvSrcAttr[iSrcAttr])
                            {
                                GUID lAttrGUID = lrvSrcAttr[iSrcAttr]->getGUID();
                                if (lSrcAttrGUID == lAttrGUID)
                                    break;
                            }
                        }
                        
                        if (lrvSrcAttr.size() == iSrcAttr)
                        {
                            *oppData = NULL;
                            *opDataSize = 0;
                            *opDataType = DssDataTypeReserved;
                            *opDataFlag = DssDataNull;
                            
                            return S_OK;
                        }
                        
                        lpTabularAttr = lrvSrcAttr[iSrcAttr];
                        lpLookUp = dynamic_cast<DSSGlobalLookUpTable*>(lpTabularAttr);

                        DSSBaseElementsProxy *lpCurElements = lpControl->getCurrentElements();
                        //CHECK_PTR(lpCurElements);
                        /*assert(1 == lpCurElements->Count());*/
                        //fali, 827602, if it's getting the original value of DDIC, set lfIsFromDDICSource to false and don't get the current selection of DDIC control, but get the original value before selection
						if(!lpCurElements || lpCurElements->Count() == 0 || ibDiscardOrOriginalDDICElement)//no selection on DDIC yet, we shouldn't crash
						{
                            // jimzhang, TQMS 781535, in this case, "lfIsFromDDICSource" should be true, and below logic
                            //    will set output value to NULL and return S_OK.
							lfIsFromDDICSource = false;
                            //if the source of the DDIC is not the same with the attribute defined on the field,
                            //we should submit the content of the field
                            if(lObjID != lpTabularAttr->getGUID())
                                lBaseFormID = -1;
						}
						else
						{
							DSSBaseElementProxy *lpCurElement = lpCurElements->Item(0);
							CHECK_PTR(lpCurElement);
							lGlobalKey = lpCurElement->getGlobalKey();
                            if(lGlobalKey < 0)
                                lfIsFromDDICSource = false;
						}
                    }
                    
                    if(!lfIsFromDDICSource)
                    {
                        lpTabularAttr = mpDataModel->GetTabularAttributeByGUID(lObjID);
                        _ASSERTE(lpTabularAttr);
                        lpLookUp = dynamic_cast<DSSGlobalLookUpTable*>(lpTabularAttr);
                        _ASSERTE(lpLookUp);
                        
                        if(lBaseFormID == -1)
                        {
                            bool lbHasFormOnField = false;
                            vector<int> lvUnionForms;
                            
                            DSSAttributeFormWrappers *lpDispFormWrappers = ipField->getAttributeForms();
                            int lnDispForms = lpDispFormWrappers->Count();
                            if (lnDispForms > 0)
                            {
                                lbHasFormOnField = true;
                            }
                            else
                            {
                                hr = lpLookUp->GetDisplayFormUnionForControl(lvUnionForms);
                                CHECK_HR;
                                lnDispForms = lvUnionForms.size();
                            }
                            
                            if (1 != lnDispForms)
                                break;
                            
                            if (lbHasFormOnField)
                            {
                                DSSAttributeFormWrapper *lpAttrFormWrapper = lpDispFormWrappers->Item(0);
                                _ASSERTE(lpAttrFormWrapper);
                                
                                lBaseFormID = lpAttrFormWrapper->getBaseFormID();
                            }
                            else
                            {
                                lBaseFormID = lvUnionForms[0];
                            }
                        }
                    }
                    
                    int lColumnID = 0;
                    hr = lpTabularAttr->FindFormWithLocale(0, lBaseFormID, 0, lColumnID);
                    CHECK_HR;
                    
                    DSSDataColumn *lpColumn = NULL;
                    hr = lpTabularAttr->Item(lColumnID, &lpColumn);
                    CHECK_HR;
                    
                    if(!lpColumn)
                    {
                        *oppData = NULL;
                        *opDataSize = 0;
                        *opDataType = DssDataTypeReserved;
                        *opDataFlag = DssDataNull;
                        
                        return S_OK;
                    }
                    if (lfIsFromDDICSource)
                    {
                        if (lGlobalKey < 0)
                        {
                            *oppData = NULL;
                            *opDataSize = lpColumn->getSizeOfData();
                            *opDataType = lpColumn->getDataType();
                            *opDataFlag = DssDataNull;
                            
                            return S_OK;
                        }
                    }
                    else
                    {
                        int lUnitIndex = -1;
                        hr = FindUnit(lObjID, &lUnitIndex);
                        CHECK_HR;
                        
                        hr = getElementIndex(mvRows[iRow], lUnitIndex, &lGlobalKey);
                        // CHECK_HR;
                        // #734994 tehe: we may not find the key if the correspond value can't find in database
                        if(hr != S_OK && hr != S_FALSE)
                            return hr ;
                        if(hr == S_FALSE || lGlobalKey == -1/*compound Null, 750882*/)
                        {
                            *oppData = NULL;
                            *opDataSize = lpColumn->getSizeOfData();
                            *opDataType = lpColumn->getDataType();
                            *opDataFlag = DssDataNull;
                            
                            return S_OK;
                        }
                    }
                    
                    if (DSSXTAB_TOTAL_KEY == lGlobalKey)
                        break;
                    
                    if (lfIsFromDDICSource)
                    {
                        hr = lpColumn->getData(lGlobalKey, oppData, *opDataSize, *opDataType, *opDataFlag);
                        CHECK_HR;
                    }
                    else
                    {
                        DSSTabularUnit* lpTabularUnit = NULL;
                        bool lfDE = false;
                        int lLocalKey = -1;
                        if(lpLookUp)
                        {
                            hr = lpLookUp->GetGlobalUnit(lGlobalKey, &lpTabularUnit, &lLocalKey, &lfDE);
                            CHECK_HR;
                        }
                        
                        if (lfDE)
                        {
                            DSSTabularConsolidation* lpCS = dynamic_cast<DSSTabularConsolidation*>(lpTabularUnit);
                            _ASSERTE(lpCS);
                            if(lpCS->IsLeaf(lLocalKey) && lpCS->IsDisplayed(lLocalKey))
                            {
                                int lChildKey = lpCS->getChildElementKey(lLocalKey);
                                _ASSERTE(lChildKey >= 0);
                                
                                hr = lpColumn->getData(lChildKey, oppData, *opDataSize, *opDataType, *opDataFlag);
                                CHECK_HR;
                            }
                        }
                        else
                        {
                            hr = lpColumn->getData(lGlobalKey, oppData, *opDataSize, *opDataType, *opDataFlag);
                            CHECK_HR;
                        }
                    }
                }
                    break;
                default:
                    break;
            }
        }
            break;
            
        case DssRWFieldConcatenation:
        {
            DSSFields *lpChildren = ipField->getChildFields();
            if (lpChildren)
            {
                int lnChildren = lpChildren->Count();
                if (1 != lnChildren)
                    break;
                
                DSSField* lpChild = lpChildren->Item(0);
                CHECK_PTR(lpChild);
                
                hr = hCalculateRawData(iMainField, lpChild, iRow, oppData, opDataSize, opDataType, opDataFlag, ibDiscardOrOriginalDDICElement);
                CHECK_HR;
            }
            else//837594
            {
                *opDataFlag = DssDataNull;
            }
        }
            break;
            
        case DssRWFieldLabel:
        {
            //in case of small string optimization (SSO), should not return local variable as output
            //De37205, msun,  also should not 'new' here to avoid memory leak, because the caller of this function won't delete it. 
            std::string& lFieldText = ipField->getTextRef();
            *opDataSize = lFieldText.size() + 1;
            *oppData = lFieldText.c_str();
            *opDataType = DssDataTypeVarChar;
            *opDataFlag = DssDataOk;            
        }
            break;
            
        case DssRWFieldAutoText:
        {
            std::string lAutoTextValue;
            hr = mpDataModel->getAutoText(lKey,lAutoTextValue);
            CHECK_HR;
            
            if (! lAutoTextValue.length())
            {
                GUID lDataSourceID=ipField->getDataSourceObjectID();
                if(MBase::IsGUIDNULL(lDataSourceID))
                {
                    DSSDocumentInstance *lpDocumentInstance=mpDataModel->getDocumentInstance();
                    lDataSourceID=lpDocumentInstance->getDefaultDataSetID();
                }
                
                EnumDSSAutoText lAutoTextType=ipField->getAutoTextType();
                
                if (DssAutoTextDocument == lAutoTextType)
                {
                    DSSDocumentDefinition *lpDocumentDefinition=DSSRWIterator::mpObjectContext->getDocumentDefinition();
                    lAutoTextValue = lpDocumentDefinition->getName();
                }
                else if (DssAutoTextProject == lAutoTextType)
                    hr = mpDataModel->getAutoText(lDataSourceID, "PROJECT", lAutoTextValue);
                else if (DssAutoTextDateTime == lAutoTextType)
                    ;// ToDo we need get the current time of Cilent Side
                else if (DssAutoTextUser == lAutoTextType)
                    hr=mpDataModel->getAutoText(lDataSourceID, "USER", lAutoTextValue);
                else if (DssAutoTextReport == lAutoTextType)
                    hr=mpDataModel->getAutoText(lDataSourceID, "REPORTNAME", lAutoTextValue);
                else if (DssAutoTextReportDescription == lAutoTextType)
                    hr=mpDataModel->getAutoText(lDataSourceID, "REPORTDESCRIPTION", lAutoTextValue);
                else if (DssAutoTextReportExecutionTime == lAutoTextType)
                    hr=mpDataModel->getAutoText(lDataSourceID, "EXECUTIONTIME", lAutoTextValue);
                else if (DssAutoTextReportFilterDetails == lAutoTextType)
                    hr=mpDataModel->getAutoText(lDataSourceID, "FILTERDETAILS", lAutoTextValue);
                else if (DssAutoTextReportPromptDetails == lAutoTextType)
                    hr=mpDataModel->getAutoText(lDataSourceID, "PROMPTDETAILS", lAutoTextValue);
                else if (DssAutoTextAttributeElementID==lAutoTextType||DssAutoTextAttributeLongElementID==lAutoTextType)
                {
                    DSSAttribute *lpAttribute=dynamic_cast<DSSAttribute*>(ipField->getFieldObject());
                    if(lpAttribute)
                    {
                        GUID lObjID=lpAttribute->getID();
                        
                        DSSTabularAttribute *lpTabularAttribute=mpDataModel->GetTabularAttributeByGUID(lObjID);
                        DSSGlobalLookUpTable* lpLookUp = dynamic_cast<DSSGlobalLookUpTable*>(lpTabularAttribute);
                        CHECK_PTR(lpLookUp);
                        
                        int lUnitIndex = -1;
                        hr = FindUnit(lObjID, &lUnitIndex);
                        CHECK_HR;
                        
                        int lGlobalKey = -1;
                        hr = getElementIndex(mvRows[0], lUnitIndex, &lGlobalKey);
                        CHECK_HR;
                        
                        bool lfHasDE = false;
                        int lLocalKey = -1;
                        DSSTabularUnit* lpTabularUnit = NULL;
                        hr = lpLookUp->GetGlobalUnit(lGlobalKey, &lpTabularUnit, &lLocalKey, &lfHasDE);
                        CHECK_HR_PTR(lpTabularUnit);
                        
                        DSSBaseElementProxy *lpElement=NULL;
                        hr = lpTabularUnit->createElement(lLocalKey, mpDataModel->getObjectContext(), &lpElement);
                        CHECK_HR;
                        
                        if(lpElement)
                        {
                            // TQMS 763290,751433,619843, convert long element ID to short one.
                            //lAutoTextValue=lpElement->getElementID();
                            lAutoTextValue = DSSBaseElementsProxy::ConvertFromLongToShort(lpElement->getElementID());
                        }
                    }
                }
                else
                {
                    lAutoTextValue="";
                }
            } // END: if (! lAutoTextValue.length())
            //in case of small string optimization (SSO), should not return local variable as output
            //De37205, msun,  also should not 'new' here to avoid memory leak, because the caller of this function won't delete it.
            mResolvedAutoText = lAutoTextValue;
            *oppData = mResolvedAutoText.c_str();
            *opDataSize = lAutoTextValue.size() + 1;
            
            *opDataType = DssDataTypeVarChar;
            *opDataFlag = DssDataOk;
        }
            break;
            
        case DssRWFieldCalculation:
        {
            int lOff = iRow * mnFields + iMainField;
            *oppData = mvFeildsText[lOff].c_str();
            *opDataSize = mvFeildsText[lOff].size() + 1;
            
            *opDataType = DssDataTypeVarChar;
            *opDataFlag = DssDataOk;
        }
            break;

            
        default:
        {
            hr = E_NOTIMPL;
        }
            break;
    }
    
    return hr;
}

int DSSRWFieldGroupModelIterator::hReInitMetricField(int iMainFieldIndex, int iLinkLocalIndex, DSSField *ipField, DSSFormat *ipFormat,
                                                     int iRTTIndex, int iMetricIndex)
{
    if (!mpIndexTable)
		return E_FAIL;
    
    DSSRWFieldGroupModel *lpFGModel = static_cast<DSSRWFieldGroupModel *>(DSSRWNodeModelIterator::mpNodeModel);
    CHECK_PTR(lpFGModel);
    
    DSSViewDataSet *lpViewDataSet = lpFGModel->mvpRTTemplateInfo[iRTTIndex]->mViewPtr;
    CHECK_PTR(lpViewDataSet);
    
    DSSXTabView *lpXTabView = lpFGModel->mvpXTabView[iRTTIndex];
    if (!lpXTabView)
        return E_POINTER;
    
    GUID lReportGUID;
    lpFGModel->mvpRTTemplateInfo[iRTTIndex]->GetReportID(&lReportGUID);    
    //GUID lMetricGUID = ipField->getFieldObject()->getID();
    
    const vector<MBase::CString> & lvNullDispForFG = mpDataModel->getNullDispForFG();
    
	int lnColumns = mpIndexTable->mvColumnMap.size();
    // jimzhang Apr/25/2012 TQMS 583553 For special field group, row count of joined index may be 0. In this case, 
	//		flag of metric value is DssDataInvalid, and set the corresponding null replacement as metric text.
    if (0 < lnColumns && ! mpIndexTable->mnRow)
    {   
        mCurrent = 0;
        for (int lOff = iMainFieldIndex; mCurrent < mnDisplayRows; ++mCurrent, lOff += mnFields)
        {
            if (-1 == iLinkLocalIndex)
                mvFeildsText[lOff].append(lvNullDispForFG[3]);
            else
            {
                MBase::CString lText = lvNullDispForFG[3];
                hGetEncodedLinkText(lText, mvLinkFieldText[lOff]->mtext[iLinkLocalIndex], DssRWFieldObjectValue, mvLinkFieldText[lOff]->mtype[iLinkLocalIndex], DssAutoTextReserved);
            }
        }
        
        return S_OK;
    }
    
    int hr = S_OK;
    bool lbHasError = false;
    
    vector<int> lvKeysInJoinedIndex(lnColumns);
    int lDummyIndex = 0;
    int lDummyUnitIndex = 0;
    hr = lpViewDataSet->getTabularData()->getDummyUnitIndex(&lDummyUnitIndex);
    CHECK_HR;
    
    int lnUnits = lpViewDataSet->Count();    
    vector<int> lvKeysInViewDataset;
    lvKeysInViewDataset.resize(lnUnits);
    
	set<Int32> lsCG;
	set<Int32> lsCGTotal;
	vector<DSSTabularConsolidation *> lvCS;
    
    // jimzhang Jun/8/2012 begin Cache unit local index, unit type. If unit type is DssTemplateConsolidation, catch the Consolidation object poonter and row count of this unit.
    vector<int> lvLocalIndex(lnUnits);
    vector<EnumDSSTemplateUnitType> lvUnitType(lnUnits);
    vector<DSSTabularConsolidation *> lvCSCache(lnUnits);
    vector<int> lvCompoundNull(lnUnits);
    
    for (int i = 0; i < lnUnits; ++i)
    {
        DSSTabularUnit *lpUnit = NULL;
        hr = lpViewDataSet->getTabularData()->Item(i, &lpUnit);
        CHECK_HR;
        
        GUID lGUID = GUID_NULL;
        hr = lpUnit->getGUID(lGUID);
        CHECK_HR;
        
        lvUnitType[i] = lpUnit->getUnitType();
        if (DssTemplateMetrics != lvUnitType[i])
        {
            int lGlobalIndex = -1;
            if (lDummyUnitIndex == i)
            {
                DSSGUnitContainer *lpGUnitContainer = NULL;
                hr = mpDataModel->GetGlobalLookupContainer(&lpGUnitContainer);
                CHECK_PTR(lpGUnitContainer);
                
                lGlobalIndex = lpGUnitContainer->count();
            }
            else
            {
                lGlobalIndex = mpDataModel->FindUnit(lGUID);
            }
            
            if (lGlobalIndex < 0)
                return E_FAIL;
            
            lvLocalIndex[i] = -1;
            for (int j = 0; j < lnColumns; ++j)
            {
                if (mpIndexTable->mvColumnMap[j] == lGlobalIndex)
                {
                    lvLocalIndex[i] = j;
                    break;
                }
            }
            
            if (DssTemplateConsolidation == lvUnitType[i])
            {
                DSSTabularConsolidation *lpCS = dynamic_cast<DSSTabularConsolidation *>(lpUnit);
                CHECK_PTR(lpCS);
                lvCS.push_back(lpCS);
                lvCSCache[i] = lpCS;
                
                DSSGUnitContainer *lpContainer = NULL;
                hr = mpDataModel->GetGlobalLookupContainer(&lpContainer);
                CHECK_PTR(lpContainer);
                
                lvCompoundNull[i] = -1;
                hr = lpContainer->GetGlobalRowCount(lGlobalIndex, &lvCompoundNull[i], false);
                CHECK_HR;
            }
        }
    } // END: for (int i = 0; i < lnUnits; ++i)
    // jimzhang Jun/8/2012 end
    
    // jimzhang May/23/2012 begin Cache format objects when threshold ID is -1, 0, 1, 2, ... <lnThresholds-1>
    DSSField *lpMainField = NULL;
    hFindField(iMainFieldIndex, &lpMainField);
    DSSThresholds *lpThresholds = lpMainField->getThresholds();
    int lnThresholds = lpThresholds->Count();            
    vector<DSSFormat *> lvFormat;
    if (lnThresholds)
    {
        lvFormat.resize(lnThresholds);
        for (int i = 0; i < lnThresholds; ++i)
        {
            DSSFormat *lpFormat = getFieldFormat(ipField, EnumFormattingFieldTypeMetric, ipFormat, lpThresholds->Item(i));
            lvFormat[i] = lpFormat;
        }
    }
    DSSFormat *lpFieldFormat = getFieldFormat(ipField, EnumFormattingFieldTypeMetric, ipFormat, NULL);

    // jimzhang May/23/2012 end
    
    // jimzhang Jun/28/2012 TQMS 614371 begin 
    //      (1) When formatting a metric cell, we should check if there is an Consolidation unit in keys of this metric cell. 
    //      For metric field not in detail header, the Consolidation unit may exits in out-of-level view dataset. 
    //      Below logic is just to check if there is such consolidation unit in out-of-level view dataset, if there is,
    //      get its unit index in "BaseIndexTable".
    //      (2) To follow server's logic, we only find the first Consolidation unit. However, serer's search order is by Template's Axis.
    //      On iPad, there is no Template, and only find first Consolidation unit in view dataset. This is inconsistent with server side, 
    //      and may cause issue when there are multipul Consolidation unit in (out-of-level) view dataset.
    int lCSUnitIndexInBaseIndexTable = -1;
    DSSTabularConsolidation *lpCSInOLView = NULL;
    if (DssFieldGroupDataLevelDefaultReport != lpFGModel->mDataLevel)
    {
        DSSViewDataSet *lpOLViewDataSet = lpFGModel->mvpRTTemplateInfo[iRTTIndex]->mOLViewPtr;
        if (lpOLViewDataSet)
        {
            int lnOLUnits = lpOLViewDataSet->Count();
            for (int i = 0; i < lnOLUnits; ++i)
            {
                DSSTabularUnit *lpOLUnit = NULL;
                hr = lpOLViewDataSet->getTabularData()->Item(i, &lpOLUnit);
                CHECK_HR;
                
                if (DssTemplateConsolidation == lpOLUnit->getUnitType())
                {
                    lpCSInOLView = dynamic_cast<DSSTabularConsolidation *>(lpOLUnit);
                    CHECK_PTR(lpCSInOLView);
                    
                    GUID lGUID = GUID_NULL;
                    hr = lpCSInOLView->getGUID(lGUID);
                    CHECK_HR;
                    
                    int lOLUnitIndex = -1;
                    hr = FindUnit(lGUID, &lOLUnitIndex);
                    CHECK_HR;
                    
                    if (! mpBaseIndexTable)
                    {
                        JoinedIndexTable *lpBaseIndexTable = NULL;
                        hGetBaseIndexTable(&lpBaseIndexTable);
                    }
                    
                    // find the Consolidation unit's index in BaseIndexTable
                    if (mpBaseIndexTable)
                    {
                        int lnOLColumns = mpBaseIndexTable->mvColumnMap.size();
                        for (int j = 0; j < lnOLColumns; ++j)
                        {
                            if (mpBaseIndexTable->mvColumnMap[j] == lOLUnitIndex)
                            {
                                lCSUnitIndexInBaseIndexTable = j;
                                break;
                            }
                        }
                    }
                    
                    break;  // only find the first Consolidation unit in out-of-level view dataset
                }
            }
        }
    }
    // jimzhang Jun/28/2012 TQMS 614371 end
    
    DSSCubeDataSlice *lpReportLevelSlice = NULL; // jimzhang Jun/7/2012 Catch the data slice which is not at total level
    
    bool lfTrimEmptySections = mpDataModel->GetTrimEmptySections();
    EnumDSSFieldGroupDataLevel lDataLevel = lpFGModel->mDataLevel;
    
    mCurrent = 0;
    for (int lOff = iMainFieldIndex; mCurrent < mnDisplayRows; ++mCurrent, lOff += mnFields)
    {
        //TQMS672319,  hyan, 2012/10/31
        //we must set to NULL at every begining
        DSSFormat* lpConsolidationFormat = NULL; // jimzhang Jun/27/2012 If there is a Consolidation unit in dataset which this metric belongs to, we should use format of this consolidation element to format this metric cell.
        
        // 1. Get keys of currnet row from JoinedIndexTable
        int iColumn;
        for (iColumn = 0; iColumn < lnColumns; ++iColumn)
        {
            lvKeysInJoinedIndex[iColumn] = -1; 
            hr = mpIndexTable->GetKey(mvRows[mCurrent], iColumn, &lvKeysInJoinedIndex[iColumn]);
            if (S_OK != hr)
                break;
        }
        if (iColumn < lnColumns)
        {
            lbHasError = true;
            continue;
        }
        
        // 2. Convert keys in JoinedIndexTable to keys in view dataset 
        bool lDisplay = true;
        bool lbHasSubtotal = false;
        lsCG.clear();
        lsCGTotal.clear();
        bool lbFirstCS = false; // jimzhang Jun/28/2012 At current, we only use element of first Consolidation unit in dataset to format metric cell.
        for (int i = 0; i < lnUnits; ++i)
        {
            if (DssTemplateMetrics != lvUnitType[i])
            {
                if (lvLocalIndex[i] >= 0)
                    lvKeysInViewDataset[i] = lvKeysInJoinedIndex[lvLocalIndex[i]];
                else
                    lvKeysInViewDataset[i] = DSSXTAB_TOTAL_KEY;
                
                if (lDummyUnitIndex == i)
                    lDummyIndex = lvKeysInViewDataset[i];
                
                if (DssTemplateCustomGroup == lvUnitType[i])
                {
                    if (DSSXTAB_TOTAL_KEY == lvKeysInViewDataset[i])
                        lsCGTotal.insert(i); //since in Consolidation and DE case, CS unit has a chid CG, so this may not mean subtotal
                    else
                        lsCG.insert(i);
                }
                else if (DssTemplateConsolidation == lvUnitType[i])
                {   
                    if (lvKeysInViewDataset[i] >= 0 && lvCompoundNull[i] != lvKeysInViewDataset[i])
                    {
                        int lOrdinal = -1;
                        XTabConsolidationElement *lpXTabCSElement = lvCSCache[i]->getElement(lvKeysInViewDataset[i]);
                        
                        //TQMS 642949,642968, hyan, 13/8/2012
                        //lpXTabCSElement maybe NULL
                        if (lpXTabCSElement)
                        { 
                            lOrdinal = lpXTabCSElement->ordinal;
                            
                            if (! lbFirstCS)
                            {
                                //hyan,2012/06/18
                                //issue614371, for consolidation element, we must use the GridFormat when the GridFormat is useful
                                DSSConsolidationElement *lpConsolidationElement=(DSSConsolidationElement*)(DSSRWIterator::mpObjectContext->FindObjectH(lpXTabCSElement->guid));
                                if(lpConsolidationElement)
                                {
                                    lpConsolidationFormat=lpConsolidationElement->getGridFormat();
                                    if(lpConsolidationFormat)
                                    {
                                        CComVariant* lpVal = NULL;
                                        lpVal = lpConsolidationFormat->getPropertyValue("FormattingNumber", "Category");
                                        if (!lpVal || 9 == lpVal->iVal) // check if this format is valid
                                            lpConsolidationFormat = NULL;
                                    }
                                }
                                
                                lbFirstCS = true;
                            }
                        }
                        
                        if (lOrdinal < 0)
                        {
                            lDisplay = false;
                            break;
                        }
                    }
                    
                    if (DSSXTAB_TOTAL_KEY == lvKeysInViewDataset[i])
                        lbHasSubtotal = true;
                }
                else if (DSSXTAB_TOTAL_KEY == lvKeysInViewDataset[i])
                {
                    lbHasSubtotal = true;
                }
            }
            else
            {
                lvKeysInViewDataset[i] = DSSXTAB_TOTAL_KEY;
            }
        } // END: for (int i = 0; i < lnUnits; ++i)
        
        if (!lDisplay)
        {
            // jimzhang Jun/8/2012 If this metriv value is not displayable, flag of this metric value is DssDataInvalidCell. So set corresponding null replacement to "Field Text" directly.
            if (-1 == iLinkLocalIndex)
                mvFeildsText[lOff].append(lvNullDispForFG[3]);
            else
            {
                MBase::CString lText = lvNullDispForFG[3];
                hGetEncodedLinkText(lText, mvLinkFieldText[lOff]->mtext[iLinkLocalIndex], DssRWFieldObjectValue, mvLinkFieldText[lOff]->mtype[iLinkLocalIndex], DssAutoTextReserved);
            }            
            continue;
        }
        
        // jimzhang Jun/28/2012 TQMS 614371 If the metric field is not in detail section, and there is no Consolidation unit in view dataset,
        //      we should use the first Consolidation unit in out-of-level view dataset to format this metric cell.
        if (DssFieldGroupDataLevelDefaultReport != lpFGModel->mDataLevel && !lpConsolidationFormat && lpCSInOLView)
        {
            if (! mpBaseIndexTable)
            {
                JoinedIndexTable *lpBaseIndexTable = NULL;
                hGetBaseIndexTable(&lpBaseIndexTable);
            }
            
            if (mpBaseIndexTable)
            {
                int lKey = -1;
                hr = mpBaseIndexTable->GetKey(mvRows[mCurrent], lCSUnitIndexInBaseIndexTable, &lKey);
                CHECK_HR;
                
                if (lKey >= 0) // jimzhang Jul/11/2012 ToDo. Add code to check if "lKey" is compound NULL.
                {
                    XTabConsolidationElement *lpXTabCSElement = lpCSInOLView->getElement(lKey);
                    //tliao, #689402
                    if (lpXTabCSElement)
                    {
                        DSSConsolidationElement *lpConsolidationElement=(DSSConsolidationElement*)(DSSRWIterator::mpObjectContext->FindObjectH(lpXTabCSElement->guid));           
                        if(lpConsolidationElement)
                        {
                            lpConsolidationFormat=lpConsolidationElement->getGridFormat();
                            if(lpConsolidationFormat)
                            {
                                CComVariant* lpVal = NULL;
                                lpVal = lpConsolidationFormat->getPropertyValue("FormattingNumber", "Category");
                                if (!lpVal || 9 == lpVal->iVal) // check if this format is valid
                                    lpConsolidationFormat = NULL;
                            }
                        }
                    }
                }
            }
        }
         
        if ((!lbHasSubtotal && lsCGTotal.size() > 0) || lsCG.size() > 0)
        {
            set<Int32>::iterator lIt;
            for (Int32 iCS = 0; iCS < lvCS.size(); iCS++)
            {
                DSSTabularConsolidation* lpCS = lvCS[iCS];
                Int32 lChildUnitIndex = -1;
                hr = lpViewDataSet->getTabularData()->FindUnitI(lpCS->getGUID(), DssTemplateCustomGroup, lChildUnitIndex);
                if (S_OK != hr)
                {
                    lbHasError = true;
                    continue;
                }
                
                if(lsCG.size() > 0)
                {
                    lIt = lsCG.find(lChildUnitIndex);
                    if (lIt != lsCG.end())
                        lvKeysInViewDataset[*lIt] = DSSXTAB_TOTAL_KEY;
                }
                if (!lbHasSubtotal && lsCGTotal.size() > 0)
                {
                    lIt = lsCGTotal.find(lChildUnitIndex);
                    if (lIt != lsCGTotal.end())
                        lsCGTotal.erase(lIt);
                }
            }
            
            if (lsCGTotal.size() > 0)
                lbHasSubtotal = true;
        }
        
        EnumDSSMetricType lSubtotalType = DssMetricReserved;
        Int32 lLocalTag = 0;
        if (lbHasSubtotal)
        {        
            lLocalTag = 1;  // jimzhang dec/20/2011 This is hard code.
            
            lDummyIndex = DSSXTAB_TOTAL_KEY;
            
            // jimzhang dec/20/2011 Comment out below server's logic for reference
            /*// get di.
             CComPtr<ICDSSDocumentInstance3> licpDI;
             hr = mpDataModel->get_DocumentInstance(&licpDI);
             CHECK_HR_PTR(licpDI);
             
             CComPtr<ICDSSObjectInfo> licpInfo;
             hr = mvpViewTemplate[iReport]->get_Info(&licpInfo);
             CHECK_HR_PTR(licpInfo);
             
             DSS_ID lTemplateID;
             hr = licpInfo->get_ID(&lTemplateID);
             CHECK_HR;
             
             CComQIPtr<ICDSSDocumentInstance7, &IID_ICDSSDocumentInstance7> licpDI7(licpDI);
             if (!licpDI7) return E_NOINTERFACE;
             
             hr = licpDI7->get_TemplateLevelSubtotalIndex(1, &lTemplateID, &lLocalTag);
             CHECK_HR;*/    
            
            //lSubtotalType = lpViewDataSet->getTabularData()->getSubtotalType(iMetricIndex);
            DSSTabularMetrics *lpMetrics = lpViewDataSet->getTabularData()->getTabularMetircs();
            if (lpMetrics)
            {
                lSubtotalType = lpMetrics->getTotalImplementation(iMetricIndex);
            }
        }
        
        // 3. Get slice from XTabView, then get metric value form slice
        
        const char *lpData = NULL;
        int lDataSize = 0;
        EnumDSSDataType lDataType = DssDataTypeReserved;
        DSSDataFlag lDataFlag = DssDataNull;
        
        // jimzhang Jun/8/2012 At current, only cache the slice which is at non-total level. The reason is:
        //      (1) Most metric value is at non-total level.
        //      (2) If cache all slices, then each time we get a slice, we should use level as the key to search the slice from cached slices.
        //          As slice level is a vector of bool, this search will cost some CPU time.
        DSSCubeDataSlice *lpSlice = NULL;
        if (! lbHasSubtotal)
        {
            if (! lpReportLevelSlice)
            {
                hr = lpXTabView->FindDataLocFromIndex(&lvKeysInViewDataset[0], iMetricIndex, lLocalTag, DssOriginalSlice, &lpReportLevelSlice, lDataFlag);
                if (S_OK != hr)
                {
                    lbHasError = true;
                    continue;
                }
            }
            
            lpSlice = lpReportLevelSlice;
        }
        else
        {
            hr = lpXTabView->FindDataLocFromIndex(&lvKeysInViewDataset[0], iMetricIndex, lLocalTag, DssOriginalSlice, &lpSlice, lDataFlag);
            if (S_OK != hr)
            {
                lbHasError = true;
                continue;
            }
        }
        
        if (lpSlice)
        {   
            hr = lpSlice->getVal(lnUnits, &lvKeysInViewDataset[0], &lpData, lDataSize, lDataType, lDataFlag);
            if (hr != S_FALSE && hr != S_OK)
                return hr;
            else if (S_FALSE == hr)
                lDataFlag = DssDataInvalidCell;
        }
        
        // 4. Format the metric value and save formatted text
        if (DssDataOk != lDataFlag)
        {   
            if (-1 == iLinkLocalIndex)
            {
                if (DssDataNull == lDataFlag)
                    mvFeildsText[lOff].append(lvNullDispForFG[0]);
                else if (DssDataUnCalculated == lDataFlag)
                    mvFeildsText[lOff].append(lvNullDispForFG[1]);
                else if (DssDataMissing == lDataFlag) ////fali, 824676, handle missingType slice
                    mvFeildsText[lOff].append(lvNullDispForFG[2]);
                else
                    mvFeildsText[lOff].append(lvNullDispForFG[3]);
            }
            else
            {
                MBase::CString lText;
                if (DssDataNull == lDataFlag)
                    lText = lvNullDispForFG[0];
                else if (DssDataUnCalculated == lDataFlag)
                    lText = lvNullDispForFG[1];
                else
                    lText = lvNullDispForFG[3];
                hGetEncodedLinkText(lText, mvLinkFieldText[lOff]->mtext[iLinkLocalIndex], DssRWFieldObjectValue, mvLinkFieldText[lOff]->mtype[iLinkLocalIndex], DssAutoTextReserved);
            }
        }
        else
        {
            if (lfTrimEmptySections && mpIndexTable && DssFieldGroupDataLevelDefaultReport == lDataLevel)
                mvIsTrimed[mCurrent] = false;
            
            DSSFormat *lpFormat=NULL;
            int lThresholdID = mvThresholdID[lOff];
            if(lThresholdID==-1)
            {
                if (DssMetricSubtotalCount == lSubtotalType) // If this metric value is count subtotal, do not apply metric format on it.
                    lpFormat = NULL;
                else if (lpConsolidationFormat) // If there is format in Consolidation element, then use it, otherwise, use format of metric field.
                    lpFormat = lpConsolidationFormat;
                else
                    lpFormat = lpFieldFormat;
                
                //hyan,2012/05/31
                //support cell format
                if(lDataType==DssDataTypeCellFormatData)
                {
                    CComVariant* lpVal = NULL;
                    if(ipFormat)                        
                        lpVal = ipFormat->getPropertyValue("FormattingNumber", "Category");
                    
                    //hyan, 2012/05/31, 
                    //we only use cell format when there is no format define on the column
                    if(lpVal==NULL||lpVal->iVal == 9)
                    {
                        GUID lFormatGUID=((MDataType::DSSCellFmtData *)lpData)->GetFormatGUID();
                        lpFormat=dynamic_cast<DSSFormat*>(DSSRWIterator::mpObjectContext->FindObjectH(lFormatGUID));
                    }
                }
            }
            else
            {
                lpFormat=lvFormat[lThresholdID];
            }
            
            int lFormType=0;
            int lColor=0;
            MBase::CString lValStr;
            hr = DSSFormatHelper::getFormatValue(lpData, lDataSize, lDataType, lDataFlag, lFormType, lpFormat, &lColor, lValStr);
            if (S_OK != hr)
                lbHasError = true;
            
            if (-1 == iLinkLocalIndex)
                mvFeildsText[lOff].append(lValStr);
            else
                hGetEncodedLinkText(lValStr, mvLinkFieldText[lOff]->mtext[iLinkLocalIndex], DssRWFieldObjectValue, mvLinkFieldText[lOff]->mtype[iLinkLocalIndex], DssAutoTextReserved);
        }
    } // END: for (int lOff = iMainFieldIndex; mCurrent < mnDisplayRows; ++mCurrent, lOff += mnFields)
    
    return true == lbHasError ? S_FALSE : S_OK;
}

int DSSRWFieldGroupModelIterator::hReInitAttrField(int iMainFieldIndex, int iLinkLocalIndex, DSSField *ipField, DSSFormat *ipFormat,
                                                   string *iParentKey,GUID &irAttrGUID)
{
    int hr = S_OK;
    bool lbHasError = false;
    
    DSSTabularAttribute *lpTabularAttr = mpDataModel->GetTabularAttributeByGUID(irAttrGUID);
    CHECK_PTR(lpTabularAttr);
    
    //hyan,2012/03/15
    //we need to get form from global look up
    DSSGlobalLookUpTable* lpLookUp = dynamic_cast<DSSGlobalLookUpTable*>(lpTabularAttr);
    CHECK_PTR(lpLookUp);
    
    vector<int> lvUnionForms;                    
    hr=lpLookUp->GetDisplayFormUnionForControl(lvUnionForms);                    
    CHECK_HR;
    
    //hyan,2012/03/15
    bool lbHasDispFormWrappersOnField=false;
    
    DSSAttributeFormWrappers * lpDispFormWrappers = ipField->getAttributeForms();
    int lnDispFormWrappers = lpDispFormWrappers->Count();                     
    //hyan,2012/03/15
    //for example:Customer@[Last Name], in this case the lpDispFormWrappers will be hold the Display Forms
    if (lnDispFormWrappers > 0)
        lbHasDispFormWrappersOnField = true;
    else
        lnDispFormWrappers = lvUnionForms.size();
    
    // jimzhang May/23/2012 begin To avoid calculating form type, color and data slice each time when calculating an element, cache these information here.
    vector<int> lvFormType(lnDispFormWrappers);
    vector<int> lvColor(lnDispFormWrappers);                    
    vector<DSSDataColumn *> lvColumn(lnDispFormWrappers);
    
    for (int i = 0; i < lnDispFormWrappers; ++i)
    {
        if(lbHasDispFormWrappersOnField)
        {
            DSSAttributeFormWrapper *lpAttrFormWrapper = lpDispFormWrappers->Item(i);
            CHECK_PTR(lpAttrFormWrapper);
            
            // DE20332; jzeng; 11/05/15; compound form, if the baseFormIDs is missing loading, regards as normal form
            if ((lpAttrFormWrapper->getBaseFormType() == DssBaseFormReserved) && (lpAttrFormWrapper->getBaseFormIDs().size() > 0))
            {
                vector<int>& lBaseFormIDs = lpAttrFormWrapper->getBaseFormIDs();
                vector<int>& lBaseFormTypes = lpAttrFormWrapper->getBaseFormTypes();
                for (int compoundIndex = 0; compoundIndex < lBaseFormIDs.size(); compoundIndex++)
                {
                    lvFormType[i + compoundIndex] = lBaseFormTypes[compoundIndex];
                    int lBaseFormID = lBaseFormIDs[compoundIndex];
                    int lColumnID = 0;
                    hr = lpTabularAttr->FindFormWithLocale(0, lBaseFormID, 0, lColumnID);
                    CHECK_HR;
                    
                    hr = lpTabularAttr->Item(lColumnID, &lvColumn[i + compoundIndex]);
                    CHECK_HR;
                }
                
                // set the offset based on compound id count
                lnDispFormWrappers = lnDispFormWrappers + (lBaseFormIDs.size() - 1);
                i = i + (lBaseFormIDs.size() - 1);
            }
            else
            {
                //hyan,number formatting
                lvFormType[i] = lpAttrFormWrapper->getBaseFormType();
                
                int lBaseFormID = lpAttrFormWrapper->getBaseFormID();
                int lColumnID = 0;
                hr = lpTabularAttr->FindFormWithLocale(0, lBaseFormID, 0, lColumnID);
                CHECK_HR;
                
                hr = lpTabularAttr->Item(lColumnID, &lvColumn[i]);
                CHECK_HR;
            }
        }
        else
        {
            int lBaseFormID=lvUnionForms[i];
            
            //TODO[opt]: cache data form
            int lColumnID = 0;
            int hr = lpLookUp->FindFormWithLocale(0, lBaseFormID, 0, lColumnID);
            if(hr!=S_OK)
                continue;//return S_OK;
            
            //we can use the ColumnID as the index to get FormType
            lvFormType[i]=lpLookUp->getBaseFormType(lColumnID);
            
            hr = lpLookUp->Item(lColumnID, &lvColumn[i]);
            CHECK_HR;
        }
        
        CHECK_PTR(lvColumn[i]);
    }
    // jimzhang May/23/2012 end
    
    int lUnitIndex = -1;
    hr = FindUnit(irAttrGUID, &lUnitIndex);
    CHECK_HR;
    
    // jimzhang Jul/26/2012 Get offset of report which contains this attribute, and attribute element null replacement
    //      of this report maybe used in following logic. If one attribute exits in multiputl reports,
    //      server logic can identify which report the null element belongs to, but iPad side has no this mapping info,
    //      and only get the first report which contains this attribute. This inconsistency will cause issues.
    
    // jimzhang, Jun/21/2013, TQMS 731888, do not use null replacement of dataset level, and only use null replacement of project level
    // tliao, 06/28/2013, 763713, roll back change for 731888
    int lReportIndex = -1;
    DSSDataElements *lpDataElements = mpDataModel->getObjectContext()->getDataElements();
    int lnDataElement = lpDataElements->Count();
    for (int iDataElement = 0; iDataElement < lnDataElement; ++iDataElement)
    {
        DSSDataElement *lpElement = lpDataElements->Item(iDataElement);
        DSSUnitObjects *lpObjects = lpElement->getUnitObjects();
        int lnObject = lpObjects->Count();
        int iObj = 0;
        for(iObj = 0; iObj < lnObject; ++iObj)
        {
            DSSUnitObject *lpObject = lpObjects->Item(iObj);
            GUID lID = lpObject->getID();
            if(ATL::InlineIsEqualGUID(lID, irAttrGUID))
                break;
        }
        
        if (iObj < lnObject)
        {
            lReportIndex = iDataElement;
            break;
        }
    }

    // jimzhang May/23/2012 begin Cache format objects when threshold ID is -1, 0, 1, 2, ... <lnThresholds-1>
    DSSField *lpMainField = NULL;
    hFindField(iMainFieldIndex, &lpMainField);
    DSSThresholds *lpThresholds = lpMainField->getThresholds();
    int lnThresholds = lpThresholds->Count();            
    vector<DSSFormat *> lvFormat;
    if (lnThresholds)
    {
        lvFormat.resize(lnThresholds);
        for (int i = 0; i < lnThresholds; ++i)
        {
            DSSFormat *lpFormat = getFieldFormat(ipField, EnumFormattingFieldTypeAttribute, ipFormat, lpThresholds->Item(i));
            lvFormat[i] = lpFormat;
        }
    }
    DSSFormat *lpFieldFormat = getFieldFormat(ipField, EnumFormattingFieldTypeAttribute, ipFormat, NULL);
    // jimzhang May/23/2012 end 
    
    // jimzhang Jul/12/2012 begin. This is an optimization for generating element text. 
    //      If this field meets below condition, use 1D arrary to cache element text.
    //          (1) There is no threshold defined on this field.
    //          (2) The repetition rate of elements text is not less that 2.
    vector<MBase::CString> lvUniqueElemText;
    vector<int> lvGlobalKey2Text;
    vector<int> lvRow2Text;
    int lGlobalRowCount = -1;
    
    int lfCacheElemText = false;
    
    if (! lnThresholds)
    {
        DSSGUnitContainer *lpContainer = NULL;
        hr = mpDataModel->GetGlobalLookupContainer(&lpContainer);
        CHECK_PTR(lpContainer);
        
        hr = lpContainer->GetGlobalRowCount(lUnitIndex, &lGlobalRowCount, true);
        CHECK_HR;
        
        // jimzhang Jul/19/2012 ToDo. Use count of DE display elements instead of "lGlobalRowCount"
        //      when calculating repetition rate of element
        double lRepetitionRate = mnDisplayRows*1.0/lGlobalRowCount;
        if (lRepetitionRate >= 2)
            lfCacheElemText = true;
    }
    
    if (lfCacheElemText)
    {
        lvGlobalKey2Text.resize(lGlobalRowCount + 1); // Allocate a more element for "Total" key.
        lvRow2Text.resize(mnDisplayRows);
        if ((lGlobalRowCount+1) != lvGlobalKey2Text.size() || mnDisplayRows != lvRow2Text.size())
        {
            // Fail of vector::resize() means there is not enough memory, in this case, do not cache element text.
            lfCacheElemText = false;
        }
        else
        {
            memset(&lvGlobalKey2Text[0], 0xff, lvGlobalKey2Text.size() * sizeof(int));
            memset(&lvRow2Text[0], 0xff, lvRow2Text.size() * sizeof(int));
        }
    }
    // jimzhang Jul/12/2012 end.
    
    // jimzhang May/23/2012 begin Move the definition of local variables out of below for-loop to avoid assignment/function calling  in the loop.
    char *lpData = NULL;
    int lDataSize = 0;
    EnumDSSDataType lDataType = DssDataTypeUnknown;
    DSSDataFlag lDataFlag = DssDataUnknown;
    
    MBase::CString lDataString;
    // jimzhang May/23/2012 end
    
    string lAttributeFormSeperator = ":";
    //hyan,2012/02/08, get seperator of multi attributeform of FG
    if (1 < lnDispFormWrappers)
    {
        DSSRWFieldGroupNode *lpFieldGroupNode=dynamic_cast<DSSRWFieldGroupNode*>(mpNode);
        if(lpFieldGroupNode)
        {
            string lSeperator=lpFieldGroupNode->getSeperator();
            if(lSeperator.length()>0)
                lAttributeFormSeperator=lSeperator;
        }
    }
    
    int lElemTextIndex = -1; // offset in "lvUniqueElemText"
    mCurrent = 0;
    for (int lOff = iMainFieldIndex; mCurrent < mnDisplayRows; ++mCurrent, lOff += mnFields)
    {
        int lGlobalKey = -1;
        hr = getElementIndex(mvRows[mCurrent], lUnitIndex, &lGlobalKey);
        if (S_OK != hr)
        {
            if (FAILED(hr))
                lbHasError = true;
            continue;
        }
        
        // Global key equals "-1" means this element is NULL. As null replacement of attribute element 
        //      has not been serialized to client side, just display empty in this case.
        if (-1 == lGlobalKey) 
            continue;
        
        if (lfCacheElemText)
        {
            int lKeyIndex = DSSXTAB_TOTAL_KEY == lGlobalKey ? lGlobalRowCount : lGlobalKey;
            if (0xffffffff != lvGlobalKey2Text[lKeyIndex])
            {   // cache is hit
                lvRow2Text[mCurrent] = lvGlobalKey2Text[lKeyIndex];
                continue;
            }
            
            // Cache is not hit, and will be updated here
            ++lElemTextIndex;
            if (lElemTextIndex >= lvUniqueElemText.size())
                lvUniqueElemText.resize(lvUniqueElemText.size() + 100);
            
            lvGlobalKey2Text[lKeyIndex] = lElemTextIndex;
            lvRow2Text[mCurrent] = lElemTextIndex;
        }
        
        if(iParentKey)
        {
            MBase::CString lAttributeElementID;
            hr=lpTabularAttr->getAttributeElementIDForAutoText(lGlobalKey, DSSRWIterator::mpObjectContext, lAttributeElementID);
            
            map<string,MBase::CString>::iterator iter=mvAttributeElementID.find(*iParentKey);
            if(iter!=mvAttributeElementID.end())
                mvAttributeElementID.erase(iter);
            
            mvAttributeElementID.insert(map<string,MBase::CString>::value_type(*iParentKey,lAttributeElementID));
        }
        
        if (DSSXTAB_TOTAL_KEY == lGlobalKey)
        {                        
            const vector<MBase::CString> & lvTotalDispForFG = mpDataModel->getTotalDispForFG();                        
            // jimzhang Jan/10/2012 At current, server side only serializes one total display string for FieldGroup
            if (1 != lvTotalDispForFG.size())
                return E_FAIL;
            
            std::string lTotalStr;
            for (int iForm = 0; iForm < lnDispFormWrappers; ++iForm)
            {
                lTotalStr.append(lvTotalDispForFG[0]);
                if (iForm != lnDispFormWrappers-1)
                    lTotalStr.append(lAttributeFormSeperator);
            }
            
            if (! lfCacheElemText)
            {
                if (-1 == iLinkLocalIndex)
                    mvFeildsText[lOff].append(lTotalStr);
                else
                    hGetEncodedLinkText(lTotalStr, mvLinkFieldText[lOff]->mtext[iLinkLocalIndex], DssRWFieldObjectValue, mvLinkFieldText[lOff]->mtype[iLinkLocalIndex], DssAutoTextReserved);                               
            }
            else
            {
                lvUniqueElemText[lElemTextIndex].append(lTotalStr);
            }
            
            continue;
        }
        
        int lThresholdID = mvThresholdID[lOff];
        DSSFormat *lpFormat = -1 == lThresholdID ? lpFieldFormat : lvFormat[lThresholdID];
        
        DSSTabularUnit* lpTabularUnit = NULL;
        int lLocalKey = -1;
        bool lfDE = false;
        hr = lpLookUp->GetGlobalUnit(lGlobalKey, &lpTabularUnit, &lLocalKey, &lfDE);                    
        if (S_OK != hr)
        {
            lbHasError = true;
            continue;
        }
        
        std::string lElemText;
        for (int iForm = 0; iForm < lnDispFormWrappers; ++iForm)
        {
            if (lfDE)
            {
                DSSTabularConsolidation* lpCS = dynamic_cast<DSSTabularConsolidation*>(lpTabularUnit);
                if (! lpCS)
                {
                    lbHasError = true;
                    continue;
                }                                
                
                // # tehe 2/6/2012 we need do special handling for leaf nodes
                // jimzhang Jul/23/2012 TQMS 630589. Undisplayable DE element has been filtered out at slicing stage, 
                //      so I think displayble checking here is not necessary (When getting dada from joined index,
                //      server side does not check if DE element is displayable , too).
                //      However, I am not sure if ignoring displayble checking will caues other issues. And this is 
                //      is a temporayr fix.
                if(lpCS->IsLeaf(lLocalKey) /*&& lpCS->IsDisplayed(lLocalKey)*/) 
                {
                    int lChildKey = lpCS->getChildElementKey(lLocalKey);
                    if (lChildKey < 0)
                    {
                        lbHasError = false;
                        continue;
                    }
                    
                    lvColumn[iForm]->getData(lChildKey, (const char**)(&lpData), lDataSize, lDataType,lDataFlag);
                    
                    hr = DSSFormatHelper::getFormatValue(lpData, lDataSize, lDataType, lDataFlag, lvFormType[iForm], lpFormat, &lvColor[iForm],lDataString);
                    if (S_OK != hr)
                    {
                        lbHasError = true;
                        continue;
                    }
                    
                    if(! lDataString.length() )
                        DSSAEHelper::ConvertToString(lpData, lDataSize, lDataType, lDataFlag, lDataString);
                    
                    if (! lfCacheElemText)
                    {
                        if (lElemText.length() > 0)
                            lElemText.append(lAttributeFormSeperator);
                        lElemText.append(lDataString);
                    }
                    else
                    {
                        if (lvUniqueElemText[lElemTextIndex].length())
                            lvUniqueElemText[lElemTextIndex].append(lAttributeFormSeperator);
                        lvUniqueElemText[lElemTextIndex].append(lDataString);
                    }
                }
                else
                {
                    XTabConsolidationElement* lpCSElement = lpCS->getElement(lLocalKey);
                    
                    if (! lfCacheElemText)                                        
                        lElemText.append(lpCSElement->value);
                    else
                        lvUniqueElemText[lElemTextIndex].append(lpCSElement->value);
                    
                    break;
                }
            }
            else
            {
                hr = lvColumn[iForm]->getData(lGlobalKey, (const char**)&lpData, lDataSize, lDataType,lDataFlag);
                if (S_OK != hr)
                {
                    lbHasError = true;
                    continue;
                }
                
                if (DssDataOk == lDataFlag)
                {
                    hr=DSSFormatHelper::getFormatValue(lpData, lDataSize, lDataType, lDataFlag, lvFormType[iForm], lpFormat, &lvColor[iForm],lDataString);                        
                    if (S_OK != hr)
                    {
                        lbHasError = true;
                        continue;
                    }
                }
                else
                {
                    // jimzhang, Jun/21/2013, TQMS 731888, do not use null replacement of dataset level, and only use null replacement of project level
                    // tliao, 06/28/2013, 763713, roll back change for 731888
                    const vector<vector<string> > & lvvElemNullReplacement = mpDataModel->getNullDisplay();
                    int lNullIndex = DSSAEHelper::ConvertDataFlag(lDataFlag);
                    if (-1 == lReportIndex || lReportIndex >= lvvElemNullReplacement.size() ||
                        lNullIndex >= lvvElemNullReplacement[lReportIndex].size())
                    {
                        lbHasError = true;
                        break;
                    }
                    
                    lDataString.assign(lvvElemNullReplacement[lReportIndex][lNullIndex].c_str());
                }
                
                if (! lDataString.length())
                    DSSAEHelper::ConvertToString(lpData, lDataSize, lDataType, lDataFlag, lDataString);
                
                if (! lfCacheElemText)
                {
                    if (lElemText.length())
                        lElemText.append(lAttributeFormSeperator);
                    lElemText.append(lDataString);
                }
                else
                {
                    if (lvUniqueElemText[lElemTextIndex].length() )
                        lvUniqueElemText[lElemTextIndex].append(lAttributeFormSeperator);
                    lvUniqueElemText[lElemTextIndex].append(lDataString);
                }
            }
        } // END: for (int iForm = 0; iForm < lnDispFormWrappers; ++iForm) 
        
        if (! lfCacheElemText)
        {
            if (-1 == iLinkLocalIndex)
                mvFeildsText[lOff].append(lElemText);
            else
                hGetEncodedLinkText(lElemText, mvLinkFieldText[lOff]->mtext[iLinkLocalIndex], DssRWFieldObjectValue, mvLinkFieldText[lOff]->mtype[iLinkLocalIndex], DssAutoTextReserved);
                
        }                        
    } // END: for (int lOff = iMainFieldIndex; mCurrent < mnDisplayRows; ++mCurrent, lOff += mnFields)
    
    if (lfCacheElemText)
    {
        mCurrent = 0;
        for (int lOff = iMainFieldIndex; mCurrent < mnDisplayRows; ++mCurrent, lOff += mnFields)
        {
            lElemTextIndex = lvRow2Text[mCurrent];
            if (0xffffffff == lElemTextIndex)
                // Pervious logic failed to get global key of element in current row.
                continue;
            
            if (-1 == iLinkLocalIndex)
                mvFeildsText[lOff].append(lvUniqueElemText[lElemTextIndex]);
            else
                hGetEncodedLinkText(lvUniqueElemText[lElemTextIndex], mvLinkFieldText[lOff]->mtext[iLinkLocalIndex], DssRWFieldObjectValue, mvLinkFieldText[lOff]->mtype[iLinkLocalIndex], DssAutoTextReserved);
                
        }
    }
    
    return true == lbHasError ? S_FALSE : S_OK;
}

int DSSRWFieldGroupModelIterator::getFieldIndexFromHandle(DSSCellHandle &irHandle)
{
	if (irHandle.type == DssCellHandleField && mnFields > 0)
    {
        return irHandle.v.field % mnFields;
    }
	return -1;
}

int DSSRWFieldGroupModelIterator::getRowIndexFromHandle(DSSCellHandle &irHandle)
{
	if (irHandle.type == DssCellHandleField && mnFields > 0)
    {
        return irHandle.v.field / mnFields;
    }
	return -1;
}

int DSSRWFieldGroupModelIterator::CreateInputControlIterator(std::string iKey, DSSRWInputControlIterator** oppControlIterator)
{
    int hr = S_OK;
    if(!mpDataModel)
        return E_FAIL;
    
    if(mapKey2InputControl.find(iKey) != mapKey2InputControl.end() && mapKey2InputControl[iKey] != NULL)
    {
        if(oppControlIterator)
            (*oppControlIterator) = mapKey2InputControl[iKey];
        // jzeng 9/22/15, For performance, don't reinit if hit cache; do reinit while changing such as selection from ddic
        //(*oppControlIterator)->ReInit();
        return S_OK;
    }
    
    if(oppControlIterator)
    {
        DSSRWDDICInputControlIterator* lpIterator = new DSSRWDDICInputControlIterator();
        hr = lpIterator->Init(iKey, DSSRWIterator::mpObjectContext, mpNodeModel, mpDataModel);
        CHECK_HR;
        
        (*oppControlIterator) = lpIterator;
        
        if(mapKey2InputControl[iKey])
            mvObsoleteInputControl.push_back(mapKey2InputControl[iKey]);
        mapKey2InputControl[iKey] = lpIterator;
    }
    return S_OK;
}

int DSSRWFieldGroupModelIterator::GenerateTransactionRecord(DSSCellHandle &irHandle, CComVariant &irNewValue)
{
    DSSDataModelTransactionRWField *lpTransactionModel = dynamic_cast<DSSDataModelTransactionRWField *>(getTransactionDataModel());
	if (!lpTransactionModel || mvMapToWBColumn.empty())
		return E_FAIL;
	
	AE_ASSERT(irHandle.type == DssCellHandleField);
	
	int lnFields = mnFields;
	AE_ASSERT(lnFields > 0);
	
	int row = irHandle.v.field / lnFields;
	int col = irHandle.v.field % lnFields;
	
	vector<AERawData> lvOriginalData;
    //fali, 827602, this method is only called when there's DDIC control, get the original value of DDIC instead of the current selected new value
    bool lbGetDDICOriginal = true;
	int hr = hGetFieldData(row, lvOriginalData, lbGetDDICOriginal);
	CHECK_HR;
	
	VariantChangeType(&irNewValue, &irNewValue, 0, VT_BSTR);
	std::string lText = MBase::WideCharToUTF8(irNewValue.bstrVal).c_str();
	
    int lBlockOff = lpTransactionModel->getBlockOffByGBElements(mvGroupByElements, mnRows);
    
    //dummy, not used for FG
    vector<AERawData> lvDisplayData(lvOriginalData.size());
    int lColumnIndex = mvMapToWBColumn[col];
    if(lColumnIndex < 0 && col < mvOfflineMapToWBColumn.size())
    {
        lColumnIndex = mvOfflineMapToWBColumn[col];
        if(lColumnIndex >= 0)
        {
            lColumnIndex += mvOfflineMapToFields.size();
        }
    }
	hr = lpTransactionModel->AddRecord(row+lBlockOff, col, lColumnIndex, lText, &lvOriginalData,&lvDisplayData);
	CHECK_HR;
	
	return S_OK;
}

int DSSRWFieldGroupModelIterator::UpdateFieldCell(DSSCellHandle &irHandle, CComVariant &irNewValue)
{
    DSSDataModelTransactionRWField *lpTransactionModel = dynamic_cast<DSSDataModelTransactionRWField *>(getTransactionDataModel());
	if (!lpTransactionModel || mvMapToWBColumn.empty())
		return E_FAIL;
	
	AE_ASSERT(irHandle.type == DssCellHandleField);
	
	int lnFields = mnFields;
	AE_ASSERT(lnFields > 0);
	
	int row = irHandle.v.field / lnFields;
	int col = irHandle.v.field % lnFields;
	
    int hr = S_OK;
//	vector<AERawData> lvOriginalData;
//	int hr = hGetFieldData(row, lvOriginalData);
//	CHECK_HR;
	
	VariantChangeType(&irNewValue, &irNewValue, 0, VT_BSTR);
	std::string lText = MBase::WideCharToUTF8(irNewValue.bstrVal).c_str();
	
    int lBlockOff = lpTransactionModel->getBlockOffByGBElements(mvGroupByElements, mnRows);
    int lColumnIndex = mvMapToWBColumn[col];
    if(lColumnIndex < 0 && col < mvOfflineMapToWBColumn.size())
    {
        lColumnIndex = mvOfflineMapToWBColumn[col];
        if(lColumnIndex >= 0)
        {
            lColumnIndex += mvOfflineMapToFields.size();
        }
    }
	hr = lpTransactionModel->UpdateField(row+lBlockOff, col, lColumnIndex, &lText, true);
	CHECK_HR;
	
	return S_OK;
}

//tliao, 2013-1-15, port server side logic (encode link text) to client side
int DSSRWFieldGroupModelIterator::hGetEncodedLinkText(MBase::CString &irText, MBase::CString* iCurrentText, EnumDSSRWFieldType iFieldType, EnumDSSRWLinkType iLinkType, EnumDSSAutoText iAutoTextType) 
{
    typedef enum {NoEncode = 0, URLEncode, XMLEncode} EncodeFlags;
    EncodeFlags lEncode = URLEncode;
    
    if (iLinkType == DssRWLinkToolTip || iLinkType == DssRWLinkHTML || iLinkType == DssRWLinkImage)
        lEncode = NoEncode;
    else if (iLinkType == DssRWLinkResolvedHyperLinks)
        lEncode = XMLEncode;
    
    if (iLinkType == DssRWLinkHyperLink) 
    {
        if (iCurrentText->find('?', 0) == MBase::CString::npos) {
            lEncode = NoEncode;
        }
    }
    
    if (iFieldType == DssRWFieldLabel)
    {
        lEncode = NoEncode;
    }
    
    if (iFieldType == DssRWFieldAutoText)
    {
        if (iAutoTextType == DssAutoTextWebServer)
            lEncode = NoEncode;
    }
    
    if (lEncode == URLEncode)
    {
        hEncodeURL(irText);
    }
    else if (lEncode == XMLEncode)
    {
        hEncodeXML(irText);
    }
    
    if (irText.length() > 0)
        iCurrentText->append(irText);
    
    return S_OK;
}

bool DSSRWFieldGroupModelIterator::isTransactionModelValidated()
{
    DSSRWFieldGroupModel *lpFGModel = dynamic_cast<DSSRWFieldGroupModel *>(DSSRWNodeModelIterator::mpNodeModel);
    CHECK_PTR(lpFGModel);
    DSSDataModelTransaction* lpTransactionModel = lpFGModel->getTransactionDataModel();
    
    int lBlockOff = 0;
    DSSDataModelTransactionRWField* lpFGTransactionModel = dynamic_cast<DSSDataModelTransactionRWField*>(lpTransactionModel);
    if(lpFGTransactionModel)
        lBlockOff = lpFGTransactionModel->getBlockOffByGBElements(mvGroupByElements, mnRows);
    
    if(lpTransactionModel)
    {
        for(int i = 0; i < mvRows.size(); i++)
        {
            if(lpTransactionModel->isModelValidated(lBlockOff + mvRows[i]))
                return true;
        }
    }
    return false;
}

EnumDSSDataType DSSRWFieldGroupModelIterator::getTransactionInputDataType(DSSCellHandle& irHandle)
{
    _ASSERT(irHandle.type == DssCellHandleField);
	
    DSSDataModelTransactionRWField *lpTransactionModel = dynamic_cast<DSSDataModelTransactionRWField *>(getTransactionDataModel());
    if (! lpTransactionModel)
        return DssDataTypeMissing;
    
	int lnFields = mnFields;
	
    //EnumDSSTransactionDataChangeType lIsColumnRequired = DssTransactionDataReserved;
    //EnumDSSTransactionDataChangeType lIsDataUpdated = DssTransactionDataOriginal;
    //EnumDSSTransactionDataChangeType lIsToSubmit = DssTransactionDataReserved;
    
	if (lnFields > 0 && lpTransactionModel && (!mvMapToWBColumn.empty() || !mvOfflineMapToWBColumn.empty()))
	{
		//int row = irHandle.v.field / lnFields;
		int col = irHandle.v.field % lnFields;
        int lColumnIndex = mvMapToWBColumn[col];
        if(lColumnIndex < 0 && col < mvOfflineMapToWBColumn.size())
        {
            lColumnIndex = mvOfflineMapToWBColumn[col];
            if(lColumnIndex >= 0)
            {
                lColumnIndex += mvOfflineMapToFields.size();
            }
        }
        return lpTransactionModel->getColumnDataType(lColumnIndex);
	}
    return DssDataTypeMissing;
}

int DSSRWFieldGroupModelIterator::getCurrentBlockOffset()
{
	DSSDataModelTransactionRWField *lpTransactionModel = dynamic_cast<DSSDataModelTransactionRWField *>(getTransactionDataModel());
	if (!lpTransactionModel || (mvOfflineMapToWBColumn.empty() && mvMapToWBColumn.empty()))
		return 0;

	return lpTransactionModel->getBlockOffByGBElements(mvGroupByElements, mnRows);
}

int DSSRWFieldGroupModelIterator::AnalyzeTransactionThreshold(DSSThreshold* ipThreshold, bool* opResult)
{
    int hr = S_OK;
    if(ipThreshold && mpDataModel)
    {
        hr = mpDataModel->AnalyzeTransactionThreshold(ipThreshold, opResult);
        CHECK_HR;
    }
    return hr;
}

int DSSRWFieldGroupModelIterator::getRawDataForInitialRendering(int iRow, int iField, int iDataSize, EnumDSSDataType iDataType, const char **oppData, DSSDataFlag *opDataFlag)
{
    if(iRow * mnFields + iField >= mvFeildsText.size() || iRow * mnFields + iField < 0)
        return E_FAIL;
    AERawData lRawData(NULL, iDataSize, iDataType, DssDataOk, true);
    int hr = DSSAEHelper::ConvertMetricStr2RawData(mvFeildsText[iRow * mnFields + iField], &lRawData);
    CHECK_HR;
    lRawData.SetOwnership(false);
    *oppData = lRawData.mValue;
    *opDataFlag = lRawData.mFlag;
    
    //fali, 960685, avoid memory leak
    mvpAllocatedRawData.push_back((char*)*oppData);
    return hr;
}

int DSSRWFieldGroupModelIterator::UpdateRequiredColumnStatus()
{
    int hr = S_OK;
    DSSRWFieldGroupModel *lpFGModel = dynamic_cast<DSSRWFieldGroupModel *>(DSSRWNodeModelIterator::mpNodeModel);
    CHECK_PTR(lpFGModel);

    DSSDataModelTransaction *lpTransactionModel = lpFGModel->getTransactionDataModel();
    if(lpTransactionModel)
    {
        hr = lpTransactionModel->UpdateRequiredColumnStatus();
        CHECK_HR;
    }
    return S_OK;
}

int DSSRWFieldGroupModelIterator::hBuildOfflineTransactionColumnMap(DSSRWNode * ipNode)
{
    //tliao, 866980, 3/26/2014, use the right map
	if (!mvOfflineMapToFields.empty())		//map has been built
		return S_OK;
	
	if (!ipNode)
		return E_POINTER;
	
    DSSOfflineTransactionCSIInfo *lpCSIInfo = static_cast<DSSRWFieldGroupNode*>(ipNode)->getOfflineTransactionCSIInfo();
	// CHECK_PTR(lpTargetReport);
    if(!lpCSIInfo)  // 6/28/2012 tehe
        return S_OK;
	
    DFCCSITransactionBase* lpCSI = lpCSIInfo->getTransactionCSI();
    if(!lpCSI)
        return S_OK;
    
    set<Int32, less<Int32>, MBase::Allocator<Int32> >* lpPlaceHolders = lpCSI->GetPlaceHolderSet();
    if(!lpPlaceHolders)
    {
        return S_OK;
    }
    
    int lnTargetColumns = lpPlaceHolders->size();
    if(lnTargetColumns <= 0)
    {
        return S_OK;
    }
    
    int hr = S_OK;    //782890
    
	mvOfflineMapToFields.resize(lnTargetColumns);
	
	int i = 0;
	for (i = 0; i < lnTargetColumns; i++)
		mvOfflineMapToFields[i] = -1;
	
	DSSFields *lpFields = static_cast<DSSRWFieldGroupNode*>(ipNode)->getFields();
	int lnFields = lpFields->Count();
	
	//1,check document level mapping first
	for (i = 0; i < lnFields; i ++)
	{
		DSSField *lpField = lpFields->Item(i);
		if (lpField->IsMappedForOfflineTransaction())
		{
			int lColumnIndex = lpField->getOfflineColumnIndex();
			lColumnIndex --;	//it's 1-based
            assert(lColumnIndex >= 0 && lColumnIndex < lnTargetColumns);
			if(lColumnIndex >= 0 && lColumnIndex < lnTargetColumns)
            {
                mvOfflineMapToFields[lColumnIndex] = i;
            }
		}
	}
	
	//2, build the map from source field to transaction report's template
	for (i = 0; i < lnTargetColumns; i++)
	{
		if (mvOfflineMapToFields[i] != -1)
			continue;
        
        //suppose offline columns must be mapped
        mvOfflineMapToFields.clear();
        return S_OK;
	}
	
    DSSDataModelTransaction *lpTransactionModel = getTransactionDataModel();
	if (!lpTransactionModel)
    {
        if(!mpNodeModel)
            
            return S_OK;
        DSSDataModelTransaction *lpTransactionModel = (static_cast<DSSRWFieldGroupModel *>(mpNodeModel))->getTransactionDataModel();
        if (!lpTransactionModel)
        {
            DSSRWFieldGroupNode * lpThisNode = static_cast<DSSRWFieldGroupNode*> (ipNode);
            if (lpThisNode->IsTransactionEnabled())
            {
                lpTransactionModel = new DSSDataModelTransactionRWField();
                
                DSSTransactionReport *lpTargetReport = lpThisNode->getTransactionReport();
                
                int hr = lpTransactionModel->InitWithNodeModel(lpTargetReport, mpNodeModel);
                if (hr == S_OK)
                {
                    mpNodeModel->setTransactionDataModel(lpTransactionModel);
                }
                else
                {
                    _ASSERT(false);
                    delete lpTransactionModel;
                }
            }
        }
    }
    lpTransactionModel->setCSI(lpCSI);
    
	mvOfflineMapToWBColumn.resize(lnFields);
	for (i = 0; i < lnFields; i++)
		mvOfflineMapToWBColumn[i] = -1;
	
	for (i = 0; i < lnTargetColumns; i++)
	{
		if (mvOfflineMapToFields[i] >= 0)
			mvOfflineMapToWBColumn[mvMapToFields[i]] = i;
	}
	
	return hr;
}
int DSSRWFieldGroupModelIterator::hSetOfflineTransactionMapping()
{
    DSSDataModelTransaction *lpTransactionModel = getTransactionDataModel();
	if (!lpTransactionModel)
        return S_OK;
    
    if(mvOfflineMapToFields.empty())
        return S_OK;
    
    int lnTargetColumns = mvOfflineMapToFields.size();
    
    vector<int> lvDummy(lnTargetColumns);
    mvOfflineDataType.resize(lnTargetColumns);
    mapOfflineToTrxColumn.resize(lnTargetColumns);
    
    for(int i = 0; i < lnTargetColumns; i++)
    {
        mvOfflineDataType[i] = DssDataTypeReserved;
        mapOfflineToTrxColumn[i] = -1;
        
        if(mvOfflineMapToFields[i] >= 0 && mmTrxFieldDataType.find(mvOfflineMapToFields[i]) != mmTrxFieldDataType.end())
        {
            mvOfflineDataType[i] = mmTrxFieldDataType[mvOfflineMapToFields[i]];
            
            if(mvMapToWBColumn.size() > mvOfflineMapToFields[i] && mvMapToWBColumn[mvOfflineMapToFields[i]] >= 0)
            {
                mapOfflineToTrxColumn[i] = mvMapToWBColumn[mvOfflineMapToFields[i]];
            }
        }
    }
	lpTransactionModel->setOfflineTransaction(lvDummy, mvOfflineMapToFields,lvDummy, mvOfflineDataType, mapOfflineToTrxColumn);
    return S_OK;
}

//To be completed.
/*
int DSSRWFieldGroupModelIterator::hReInitConsolidationField(int iMainFieldIndex, int iLinkLocalIndex, DSSField *ipField, DSSFormat * ipFormat, string *iParentKey, GUID &irCsGUID)
{
    int hr = S_OK;
    bool lbHasError = false;
    int lUnitIndex = -1;
    hr = FindUnit(irCsGUID, &lUnitIndex);
    CHECK_HR;
    bool lbHasChild = false;
    
    ////Begin: For attribute display.
    DSSTabularUnit *lpTabularAttr = NULL;
    DSSGlobalLookUpTable * lpLookUpAttr = NULL;
    vector<int> lvUnionForms;
    bool lbHasDispFormWrappersOnField = false;
    DSSAttributeFormWrappers * lpDispFormWrappers = NULL;
    int lnDisFormWrappers = 0;
    DSSGUnitContainer * lpGlobalLookupContainer = NULL;
    mpDataModel->GetGlobalLookupContainer(&lpGlobalLookupContainer);
    string lstrAttributeFormSeperator = NULL;
    
    vector<int> lvFormType;
    vector<int> lvColor;
    vector<DSSDataColumn *> lvColumn;
    ////End: For attribute display.
    mCurrent = 0;
    for (int lOff = iMainFieldIndex; mCurrent < mnDisplayRows; ++mCurrent, lOff += mnFields)
    {
        int lGlobalKey = -1;
        hr = getElementIndex(mvRows[mCurrent], lUnitIndex, &lGlobalKey);
        if (S_OK != hr)
        {
            lbHasError = true;
            continue;
        }
        
        if (DSSXTAB_TOTAL_KEY == lGlobalKey)
        {
            // jimzhang dec/30/2011 ToDo "Total" is hard code
            if (-1 == iLinkLocalIndex)
                mvFeildsText[lOff].append("Total");
            else
                mvLinkFieldText[lOff]->mtext[iLinkLocalIndex]->append("Total");
            
            continue;
        }
        else if (-1 == lGlobalKey)
        {
            // jimzhang Mar/28/2012 Global key equals -1 means that the type of this element is DssElementNULL.
            //      In this case, for Consolidation, server's logic only assign empty string to this element's value.
            if (-1 == iLinkLocalIndex)
                mvFeildsText[lOff].append("");
            else
                mvLinkFieldText[lOff]->mtext[iLinkLocalIndex]->append("");
            
            continue;
        }
        
        DSSTabularUnit* lpTabularUnit = NULL;
        int lLocalKey = -1;
        mpDataModel->GetTabularUnit( irCsGUID, lGlobalKey, &lpTabularUnit, &lLocalKey);
        if (! lpTabularUnit)
        {
            lbHasError = true;
            continue;
        }
        
        DSSTabularConsolidation* lpTCS = dynamic_cast<DSSTabularConsolidation*>(lpTabularUnit);
        if (! lpTCS)
        {
            lbHasError = true;
            continue;
        }
        
        XTabConsolidationElement* lpCSElement = lpTCS->getElement(lLocalKey);
        if (! lpCSElement)
        {
            lbHasError = true;
            continue;
        }
        
        if (lpGlobalLookupContainer->isDerivedUnit(irCsGUID) && lpTCS->IsLeaf(lLocalKey))
        {
            if (lbHasChild == false) {
                ////Initialize data for attribute display.
                lbHasChild = true;
                lpTabularAttr = lpTCS->getChildUnit();
                
                lpLookUpAttr = dynamic_cast<DSSGlobalLookUpTable*>(lpTabularAttr);
                hr = lpLookUpAttr->GetDisplayFormUnionForControl(lvUnionForms);
                if (hr != S_OK) {
                    lbHasError = true;
                    continue;
                }
                lpDispFormWrappers = ipField->getAttributeForms();
                lnDisFormWrappers = lpDispFormWrappers->Count();
                
                if (lnDisFormWrappers > 0) {
                    lbHasDispFormWrappersOnField = true;
                }
                else
                    lnDisFormWrappers = lvUnionForms.size();
                
                if (lnDisFormWrappers > 1) {
                    DSSRWFieldGroupNode *lpFieldGroupNode=dynamic_cast<DSSRWFieldGroupNode*>(mpNode);
                    if(lpFieldGroupNode)
                    {
                        string lSeperator=lpFieldGroupNode->getSeperator();
                        if(lSeperator.length()>0)
                            lstrAttributeFormSeperator = lSeperator;
                        else
                            lstrAttributeFormSeperator = ":";
                    }
                }
                
                lvFormType.resize(lnDisFormWrappers);
                lvColor.resize(lnDisFormWrappers);
                lvColumn.resize(lnDisFormWrappers);
                
                for (int i = 0; i < lnDisFormWrappers; ++i)
                {
                    if(lbHasDispFormWrappersOnField)
                    {
                        DSSAttributeFormWrapper *lpAttrFormWrapper = lpDispFormWrappers->Item(i);
                        CHECK_PTR(lpAttrFormWrapper);
                        
                        //hyan,number formatting
                        lvFormType[i] = lpAttrFormWrapper->getBaseFormType();
                        
                        int lBaseFormID = lpAttrFormWrapper->getBaseFormID();
                        int lColumnID = 0;
                        hr = lpLookUpAttr->FindFormWithLocale(0, lBaseFormID, 0, lColumnID);
                        CHECK_HR;
                        
                        hr = lpLookUpAttr->Item(lColumnID, &lvColumn[i]);
                        CHECK_HR;
                    }
                    else
                    {
                        int lBaseFormID=lvUnionForms[i];
                        
                        //TODO[opt]: cache data form
                        int lColumnID = 0;
                        int hr = lpLookUpAttr->FindFormWithLocale(0, lBaseFormID, 0, lColumnID);
                        if(hr!=S_OK)
                            continue;//return S_OK;
                        
                        //we can use the ColumnID as the index to get FormType
                        lvFormType[i]=lpLookUpAttr->getBaseFormType(lColumnID);
                        
                        hr = lpLookUpAttr->Item(lColumnID, &lvColumn[i]);
                        CHECK_HR;
                    }
                    
                    CHECK_PTR(lvColumn[i]);
                }////End: for (int i = 0; i < lnDisFormWrappers; ++i)
                lUnitIndex = lpTabularAttr->getUnitIndex();/////???????
                ///need to
                
            }/////End: if (lbHasChild == false)
            else
            {
                
            }
        }/////End: if (lpGlobalLookupContainer->isDerivedUnit(irCsGUID) && lpTCS->IsLeaf(lLocalKey))
        else
        {
            if (-1 == iLinkLocalIndex)
                mvFeildsText[lOff].append(lpCSElement->value);
            else
                hGetEncodedLinkText(lpCSElement->value, mvLinkFieldText[lOff]->mtext[iLinkLocalIndex], DssRWFieldObjectValue, mvLinkFieldText[lOff]->mtype[iLinkLocalIndex], DssAutoTextReserved);
        }
 
    }
    return hr;
}
*/

//yimliu, 2014/07/16, 920357, 919585.
int DSSRWFieldGroupModelIterator::hReInitConsolidationFieldSimple(int iMainFieldIndex, int iLinkLocalIndex, DSSField *ipField, DSSFormat * ipFormat, string *iParentKey, GUID &irCsGUID)
{
    int hr = S_OK;
    bool lbHasError = false;
    int lUnitIndex = -1;
    hr = FindUnit(irCsGUID, &lUnitIndex);
    CHECK_HR;
    
    ////Begin: For attribute display.
    
    
    
    vector<int> lvUnionForms;
    bool lbHasDispFormWrappersOnField = false;
    int lnDisFormWrappers = 0;
    DSSGUnitContainer * lpGlobalLookupContainer = NULL;
    mpDataModel->GetGlobalLookupContainer(&lpGlobalLookupContainer);
    string lstrAttributeFormSeperator = ":";
    vector<int> lvFormType;
    vector<int> lvColor;
    vector<DSSDataColumn *> lvColumn;
    ////End: For attribute display.
    mCurrent = 0;
    for (int lOff = iMainFieldIndex; mCurrent < mnDisplayRows; ++mCurrent, lOff += mnFields)
    {
        int lGlobalKey = -1;
        hr = getElementIndex(mvRows[mCurrent], lUnitIndex, &lGlobalKey);
        if (S_OK != hr)
        {
            lbHasError = true;
            continue;
        }
        
        if (DSSXTAB_TOTAL_KEY == lGlobalKey)
        {
            // jimzhang dec/30/2011 ToDo "Total" is hard code
            if (-1 == iLinkLocalIndex)
                mvFeildsText[lOff].append("Total");
            else
                mvLinkFieldText[lOff]->mtext[iLinkLocalIndex]->append("Total");
            
            continue;
        }
        else if (-1 == lGlobalKey)
        {
            // jimzhang Mar/28/2012 Global key equals -1 means that the type of this element is DssElementNULL.
            //      In this case, for Consolidation, server's logic only assign empty string to this element's value.
            if (-1 == iLinkLocalIndex)
                mvFeildsText[lOff].append("");
            else
                mvLinkFieldText[lOff]->mtext[iLinkLocalIndex]->append("");
            
            continue;
        }
        
        DSSTabularUnit* lpTabularUnit = NULL;
        int lLocalKey = -1;
        mpDataModel->GetTabularUnit( irCsGUID, lGlobalKey, &lpTabularUnit, &lLocalKey);
        if (! lpTabularUnit)
        {
            lbHasError = true;
            continue;
        }
        
        DSSTabularConsolidation* lpTCS = dynamic_cast<DSSTabularConsolidation*>(lpTabularUnit);
        if (! lpTCS)
        {
            lbHasError = true;
            continue;
        }
        
        XTabConsolidationElement* lpCSElement = lpTCS->getElement(lLocalKey);
        if (! lpCSElement)
        {
            lbHasError = true;
            continue;
        }
        
        if (lpGlobalLookupContainer->isDerivedUnit(irCsGUID) && lpTCS->IsLeaf(lLocalKey))
        {
            int lChild = lpCSElement->childElementID;
            DSSTabularAttribute *lpTabularAttr = dynamic_cast<DSSTabularAttribute*>(lpTCS->getChildUnit());
            DSSGlobalLookUpTable * lpLookUpAttr = dynamic_cast<DSSGlobalLookUpTable*>(lpTabularAttr);
            
            DSSAttributeFormWrappers * lpDispFormWrappers = ipField->getAttributeForms();
            hr = lpLookUpAttr->GetDisplayFormUnionForControl(lvUnionForms);
            CHECK_HR;
            
            if (lpDispFormWrappers == NULL || (lnDisFormWrappers = lpDispFormWrappers->Count()) ==0) {
                lnDisFormWrappers = lvUnionForms.size();
                lbHasDispFormWrappersOnField = false;
            }
            else
            {
                lbHasDispFormWrappersOnField = true;
                DSSRWFieldGroupNode *lpFieldGroupNode=dynamic_cast<DSSRWFieldGroupNode*>(mpNode);
                if(lpFieldGroupNode != NULL)
                {
                    string lSeperator=lpFieldGroupNode->getSeperator();
                    if(lSeperator.length()>0)
                        lstrAttributeFormSeperator = lSeperator;
                }
            }
            std::string lstrAppendValue;
            for (int i = 0; i < lnDisFormWrappers; ++i) {
                int lColumnID = 0;
                DSSDataColumn *lpColumn = NULL;
                int lFormType = -1;
                std::string sValue;
                if (lbHasDispFormWrappersOnField) {
                    DSSAttributeFormWrapper *lpAttrFormWrapper = lpDispFormWrappers->Item(i);
                    CHECK_PTR(lpAttrFormWrapper);
                    
                    int lBaseFormID = lpAttrFormWrapper->getBaseFormID();
                    hr = lpTabularAttr->FindFormWithLocale(0, lBaseFormID, 0, lColumnID);
                    CHECK_HR;
                    lFormType = lpAttrFormWrapper->getBaseFormType();
                    hr = lpTabularAttr->Item(lColumnID, &lpColumn);
                }
                else
                {
                    int lBaseFormID = lvUnionForms[i];
                    hr = lpLookUpAttr->FindFormWithLocale(0, lBaseFormID, 0, lColumnID);
                    CHECK_HR;
                    lFormType = lpLookUpAttr->getBaseFormType(lColumnID);
                    hr = lpLookUpAttr->Item(lColumnID, &lpColumn);
                }
                int lOffset = lChild;
                const void * lCellData = NULL;
                int lCellDataSize = 0;
                EnumDSSDataType lCellDataType;
                DSSData_Flags lCellDataFlag;
                int lColor = 0;
                lpColumn->getData(lOffset, (const char**)(&lCellData), lCellDataSize, lCellDataType,lCellDataFlag);
                if (lCellDataFlag != 0) {
                    if (mpDataModel)
                    {
                        hr = mpDataModel->getNullDisplay(irCsGUID, lCellDataFlag, sValue);
                    }
                }////End: if (lCellDataFlag != 0)
                if(sValue.length()==0)
                {
                    const MDataType::DSSDateTime* aeDateTime = static_cast<const MDataType::DSSDateTime*>(lCellData);
                    
                    
                    
                    //hyan,2012/02/09
                    //TQMS547672,we need special handle for TimeStampType in NGB and Selector
                    
                    //hyan,2012/03/26
                    //TQMS570104,we need to use aeDateTime->IsDateOnly() to check whether the original data only
                    //contains date
                    if(lCellDataType==DssDataTypeTimeStamp && aeDateTime->IsDateOnly())
                    {
                        lCellDataType=DssDataTypeDate;
                        DSSAEHelper::ConvertToString(lCellData, lCellDataSize, lCellDataType, lCellDataFlag, sValue);
                    }
                    else
                    {
                        //hyan,2012/05/23
                        //for string data type, we donot need format
                        if(lCellDataType==DssDataTypeVarChar || lCellDataType==DssDataTypeUTF8Char)
                        {
                            //hyan,2012/06/12
                            //TQMS611487, lCellData maybe NULL
                            if(lCellData)
                                sValue=(char *)lCellData;
                        }
                        else
                        {
                            MBase::CString lDataString = "";
                            
                            //MBase::CString lFormatValue;
#ifndef __ANDROID__
                            hr=DSSFormatHelper::getFormatValue(lCellData, lCellDataSize, lCellDataType, lCellDataFlag, lFormType, ipFormat, &lColor,lDataString);
                            
                            CHECK_HR;
#endif
                            
                            sValue=lDataString;
                            
                            if(sValue.length()==0)
                                DSSAEHelper::ConvertToString(lCellData, lCellDataSize, lCellDataType, lCellDataFlag, sValue);
                        }////End: if(lCellDataType==DssDataTypeVarChar || lCellDataType==DssDataTypeUTF8Char)
                    }////End: if(lCellDataType==DssDataTypeTimeStamp && aeDateTime->IsDateOnly())
                }////End: if(sValue.length()==0)
                if (lstrAppendValue.length()>0) {
                    lstrAppendValue.append(lstrAttributeFormSeperator);
                }
                lstrAppendValue.append(sValue);
            }////End: for (int i = 0; i < lnDisFormWrappers; ++i)
            if (-1 == iLinkLocalIndex)
            {
                mvFeildsText[lOff].append(lstrAppendValue);
                //debug, yimliu
                std::cout<<lstrAppendValue<<std::endl;
            }
            else
                hGetEncodedLinkText(lstrAppendValue, mvLinkFieldText[lOff]->mtext[iLinkLocalIndex], DssRWFieldObjectValue, mvLinkFieldText[lOff]->mtype[iLinkLocalIndex], DssAutoTextReserved);
        }/////End: if (lpGlobalLookupContainer->isDerivedUnit(irCsGUID) && lpTCS->IsLeaf(lLocalKey))
        else
        {
            if (-1 == iLinkLocalIndex)//debug, yimliu
                mvFeildsText[lOff].append(lpCSElement->value), std::cout<<lpCSElement->value<<std::endl;
            else
                hGetEncodedLinkText(lpCSElement->value, mvLinkFieldText[lOff]->mtext[iLinkLocalIndex], DssRWFieldObjectValue, mvLinkFieldText[lOff]->mtype[iLinkLocalIndex], DssAutoTextReserved);
        }
        /*
         ///////////////yimliu
         //Deal with leaf node.
         DSSGUnitContainer * lpGlobalLookupContainer;
         mpDataModel->GetGlobalLookupContainer(&lpGlobalLookupContainer);
         
         ///////////////end, yimliu
         */
        
    }
    return hr;
}

int DSSRWFieldGroupModelIterator::getFieldDisplayText(int iRow, int iField, MBase::CString &orText)
{
    int idxField = iRow * mnFields + iField;
    assert (idxField >= 0 && idxField < mvFeildsText.size());
    
    orText = mvFeildsText[idxField];
    
    return S_OK;
}

bool DSSRWFieldGroupModelIterator::isDDICOnField(int iField)
{
    if(mmField2WBForm.size() == 0)
        return false;
    assert (iField >= 0);
    
    bool lfIsFromDDICSource = false;
    if (mmField2WBForm.size() > 0 && mmField2WBForm[iField] != -1)
		lfIsFromDDICSource = true;
    
    return lfIsFromDDICSource;
}
