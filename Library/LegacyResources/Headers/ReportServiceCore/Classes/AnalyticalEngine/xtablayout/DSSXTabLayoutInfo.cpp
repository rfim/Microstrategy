//
//  DSSXTabLayoutInfo.cpp
//  ReportServiceCore
//
//  Created by He Tengfei on 11/14/11.
//  Copyright 2011 MicroStrategy, Inc.. All rights reserved.
//

#include "DSSXTabLayoutInfo.h"
#include "DSSXTabLayoutUnitInfo.h"
#include "DSSTemplate.h"
#include "DSSAxis.h"
#include "DSSXTabLayoutUnitInfo.h"
#include "DSSTemplateUnit.h"
#include "DSSTemplateDimension.h"
#include "DSSrwdconstants.h"
#include "DSSAEHelper.h"
#include "DSSTabularData.h"
#include "DSSTabularMetrics.h"
#include "DSSTemplateUnit.h"
#include "DSSViewDataSet.h"
#include "DSSRWGridModel.h"

DSSXTabLayoutInfo::DSSXTabLayoutInfo()
: mNumAxis(-1),  mTotalTag(0), mfTotalTagFinal(false)
//, mvNumUnit(NULL), mvvUnit(0)
{
    mvRunTimeUnits[0] = 0;
    mvRunTimeUnits[1] = 0;
}


DSSXTabLayoutInfo::~DSSXTabLayoutInfo()
{
	int iAxis = 0,  iUnit = 0 ;
	// release each LayoutUnit once
	for(iAxis = 0; iAxis < mNumAxis; iAxis ++)
	{
		if(mvvUnit[iAxis].size() > 0)
		{
			for(iUnit = 0; iUnit < mvNumUnit[iAxis]; iUnit ++)
			{
				//when initialize the array, should always set to NULL
				if(mvvUnit[iAxis][iUnit])
					delete mvvUnit[iAxis][iUnit];
			}
            mvvUnit[iAxis].clear();
			// delete [] mvvUnit[iAxis];
		}
        
	}
    
    
	// free the arrays
//	if (mvvUnit) delete [] mvvUnit;
//	if (mvNumUnit) delete [] mvNumUnit;
}


int DSSXTabLayoutInfo::get_UnitInfo(int UnitID, DSSXTabLayoutUnitInfo **ppUnitInfo)
{
	return E_NOTIMPL;
}

int DSSXTabLayoutInfo::get_AxisSize(int Axis, int *pSize)
{
	// Standard pointer check
	if (pSize == NULL) return E_POINTER;
    
	// Check the range of Axis
	if (Axis >= 0 && Axis < mNumAxis)
		*pSize = mvNumUnit[Axis];
	else
		*pSize = 0;
    
	return S_OK;
}

int DSSXTabLayoutInfo::get_UnitInfoByAxis(int Axis, int AxisOffset, DSSXTabLayoutUnitInfo **ppUnitInfo)
{    
	// Standard pointer check
	if (ppUnitInfo == NULL) return E_POINTER;
	*ppUnitInfo = NULL;
    
	// Determine whether the Axis is in range
	if (Axis < 0 || Axis >= mNumAxis) return E_INVALIDARG;
    
	// Determine that the Offset is in range
	if (AxisOffset < 0 || AxisOffset >= mvNumUnit[Axis]) return E_INVALIDARG;
    
	// Return the requested UnitInfo
	*ppUnitInfo = mvvUnit[Axis][AxisOffset];
	
	return S_OK;
}

int DSSXTabLayoutInfo::Init(DSSTemplate *pTemplate, DSSTabularData *pTabularData, DSSRWGridModel *ipGridModel)
{
	// Check whether there is a template
	if (pTemplate == NULL) return E_FAIL;
    
	HRESULT hr = S_OK;
	//-----------------------------------------
	// Get the # of Axes in the Template
    
	// mpTabularData = pTabularData;
    
    int lnAxis = 0;
    DSSAxis *lpAxis = pTemplate->getRow();
    if(lpAxis)
        lnAxis ++;
    lpAxis = pTemplate->getColumn();
    if(lpAxis)
        lnAxis ++;
    mNumAxis = lnAxis;

	if(hr != S_OK) return hr;
    
    // 3/26/2012 tehe: fix memory leak
	// mvvUnit = new DSSXTabLayoutUnitInfo**[mNumAxis];
	// mvNumUnit = new int[mNumAxis];
    mvNumUnit.clear();
    mvNumUnit.resize(mNumAxis);
    
    mvvUnit.clear();
    mvvUnit.resize(mNumAxis);
    
	int iUnit = 0,  iAxis = 0 ,  lSequenceNumber = 0 ;
//	for(iAxis = 0; iAxis < mNumAxis; iAxis ++)
//	{
//		//necessary, in case something went wrong, so distructor
//		//will be able to avoid deleting it.
//		mvvUnit[iAxis] = NULL;
//		mvNumUnit[iAxis] = 0;
//	}
    
	for(iAxis = 0; iAxis < mNumAxis; iAxis ++)
	{
		DSSAxis *lpAxis;
		lpAxis = pTemplate -> getAxis(iAxis + 1);
		if(hr != S_OK) return hr;
        
		int lnUnit = 0,  lnUnit2 = 0 ;
		lnUnit2 = lpAxis -> Count();
		if(hr != S_OK) return hr;
        
		//lnUnit2 does not taken into consideration of Dimension
		lnUnit = 0;
		for(iUnit = 0; iUnit < lnUnit2; iUnit ++)
		{
			DSSTemplateUnit *lpTemplateUnit;
			//lpTemplateUnit = lpAxis -> Item(iUnit + 1);
            lpTemplateUnit = lpAxis -> Item(iUnit);
			if(lpTemplateUnit == NULL) return E_FAIL;
            
			EnumDSSTemplateUnitType lType = DssTemplateReserved;
			lType = lpTemplateUnit -> getType();
			           
			switch (lType)
			{
                case DssTemplateAttribute:
                case DssTemplateConsolidation:
                case DssTemplateCustomGroup:
                case DssTemplateMetrics:
                    lnUnit ++;
                    break;
                case DssTemplateDimension:
				{
					int lnUnit3 = 0;
					DSSTemplateDimension *lpTemplateDimension = static_cast<DSSTemplateDimension*>(lpTemplateUnit);
					if(!lpTemplateDimension) return E_FAIL;
                    
					lnUnit3 = lpTemplateDimension -> Count();
                    
                    lnUnit += lnUnit3;
				}
                    break;
                default:
                    return E_FAIL;
			}
		}
        
        
		mvNumUnit[iAxis] = lnUnit;
		// mvvUnit[iAxis] = new DSSXTabLayoutUnitInfo*[lnUnit];
        mvvUnit[iAxis].clear();
        mvvUnit[iAxis].resize(lnUnit);
        
		for(iUnit = 0; iUnit < lnUnit; iUnit ++)
			mvvUnit[iAxis][iUnit] = NULL;
        
		iUnit = 0;
		int iUnit3 = 0;
        
		//reset
		mTotalTag = 0;
		mfTotalTagFinal = false;
		while(iUnit < lnUnit2)
		{
			DSSTemplateUnit *lpTemplateUnit;
			lpTemplateUnit = lpAxis -> Item(iUnit);
			if(lpTemplateUnit == NULL) return E_FAIL;
            
			EnumDSSTemplateUnitType lType = DssTemplateReserved;
			lType = lpTemplateUnit -> getType();
			            
			// int lElementID = DSSXTabKeyUnset;
			// bool lfElementFinal = true;
            
			switch (lType)
			{
                case DssTemplateDimension:
				{
                    DSSTemplateDimension *lpTemplateDimension = static_cast<DSSTemplateDimension*>(lpTemplateUnit);
					int lnUnit3 = 0;
					lnUnit3 = lpTemplateDimension -> Count();
					
                    int iUnit2 = 0;
					for(iUnit2 = 0; iUnit2 < lnUnit3; iUnit2 ++)
					{
						// lElementID = DSSXTabKeyUnset;
						DSSTemplateAttribute *lpTemplateAttribute = lpTemplateDimension -> Item(iUnit2);
						
                        hr = pTabularData->FindUnitI(lpTemplateAttribute->getID(), DssTemplateAttribute, lSequenceNumber);
						// hr = ConvertTemplateAttribute(lpTemplateAttribute, &lSequenceNumber, &lElementID, &lfElementFinal);
						CHECK_HR;
                        
						DSSXTabLayoutUnitInfo * lpUnitInfo = new DSSXTabLayoutUnitInfo();
						if (lpUnitInfo == NULL) hr = E_OUTOFMEMORY;
						else
						{
							// Initialize the new unit
							lpUnitInfo -> Init(lSequenceNumber, false);
                            
							mvvUnit[iAxis][iUnit3] = lpUnitInfo;
                            
							// Give it its initial reference
							// lpUnitInfo -> put_IsCurrentElementFinal(lfElementFinal);
						}
						iUnit3 ++;
                        
					}
					iUnit ++;
				}
                    break;
                case DssTemplateAttribute:
                {
                    
					// hr = ConvertTemplateAttribute(lpTemplateUnit, &lSequenceNumber, &lElementID, &lfElementFinal);
                    hr = pTabularData->FindUnitI(lpTemplateUnit->getID(), DssTemplateAttribute, lSequenceNumber, true);
					if(hr != S_OK)
                    {
                        //it may be a derived element
                        hr = pTabularData->FindUnitI(lpTemplateUnit->getID(), DssTemplateConsolidation, lSequenceNumber, true);
                        CHECK_HR;
                    }
                    
                    
					DSSXTabLayoutUnitInfo * lpUnitInfo = new DSSXTabLayoutUnitInfo();
					if (lpUnitInfo == NULL) hr = E_OUTOFMEMORY;
					else
					{
						// Initialize the new unit
						lpUnitInfo -> Init(lSequenceNumber,false);
                        
						mvvUnit[iAxis][iUnit3] = lpUnitInfo;
                        
						// Give it its initial reference
						
						// lpUnitInfo -> put_IsCurrentElementFinal(lfElementFinal);
                        
					}
					iUnit3 ++;
					iUnit ++;
                    
				}
                    break;
                case DssTemplateConsolidation:
                {
                    
					// hr = ConvertTemplateAttribute(lpTemplateUnit, &lSequenceNumber, &lElementID, &lfElementFinal);
                    hr = pTabularData->FindUnitI(lpTemplateUnit->getID(), DssTemplateConsolidation, lSequenceNumber, true);
					if(hr != S_OK) return hr;
                    
                    
					DSSXTabLayoutUnitInfo * lpUnitInfo = new DSSXTabLayoutUnitInfo();
					if (lpUnitInfo == NULL) hr = E_OUTOFMEMORY;
					else
					{
						// Initialize the new unit
						lpUnitInfo -> Init(lSequenceNumber,false);
                        
						mvvUnit[iAxis][iUnit3] = lpUnitInfo;
                        
						// Give it its initial reference
						
						// lpUnitInfo -> put_IsCurrentElementFinal(lfElementFinal);
                        
					}
					iUnit3 ++;
					iUnit ++;
                    
				}
                    break;
                case DssTemplateCustomGroup:
				{
                    
					// hr = ConvertTemplateAttribute(lpTemplateUnit, &lSequenceNumber, &lElementID, &lfElementFinal);
                    hr = pTabularData->FindUnitI(lpTemplateUnit->getID(), DssTemplateCustomGroup, lSequenceNumber);

					if(hr != S_OK) return hr;
                    
                    
					DSSXTabLayoutUnitInfo * lpUnitInfo = new DSSXTabLayoutUnitInfo();
					if (lpUnitInfo == NULL) hr = E_OUTOFMEMORY;
					else
					{
						// Initialize the new unit
						lpUnitInfo -> Init(lSequenceNumber,false);
                        
						mvvUnit[iAxis][iUnit3] = lpUnitInfo;
                        
						// Give it its initial reference
						
						// lpUnitInfo -> put_IsCurrentElementFinal(lfElementFinal);
                        
					}
					iUnit3 ++;
					iUnit ++;
                    
				}
                    break;
                case DssTemplateMetrics:
				{
                    
					//only convert for page-by
					// hr = ConvertTemplateMetrics(lpTemplateUnit, &lSequenceNumber, &lElementID);
                    //hr = pTabularData->FindUnitI(lpTemplateUnit->getID(), DssTemplateMetrics, lSequenceNumber);
					//DSSViewDataSet *lpViewDataSet;
                    //hr = pTabularData->getViewDataSet(&lpViewDataSet);
                    lSequenceNumber = pTabularData->getMetircPos();
                    if(hr != S_OK) return hr;
                    
					DSSXTabLayoutUnitInfo * lpUnitInfo = new DSSXTabLayoutUnitInfo();
					if (lpUnitInfo == NULL) hr = E_OUTOFMEMORY;
					else
					{
						// Initialize the new unit
						lpUnitInfo -> Init(lSequenceNumber,true);
                        
						mvvUnit[iAxis][iUnit3] = lpUnitInfo;
                        
						// Give it its initial reference
						
					}
					iUnit ++;
					iUnit3 ++;
				}
                    break;
                default:
                    return E_FAIL;
			}
            
		}
	}
    
    //zhyang, 781580, use new rule to push down GB units.
    // tehe: push down gb units
    if (ipGridModel)
    {
        bool lbMetricUnitOnRow = false;
        DSSAxis *lpRow = pTemplate->getRow();
        if(lpRow && lpRow->getTemplateMetrics() != NULL)
        {
            lbMetricUnitOnRow = true;
        }
        vector<int> lvRuntimeUnits;
        ipGridModel->getRunTimeUnits(lvRuntimeUnits);
        
        int lGBPos = 0;
        
        DSSTemplate *lpTemplate;
        hr = ipGridModel->GetViewDataSet()->getTemplate(&lpTemplate);
        CHECK_HR_PTR(lpTemplate);
        
        bool lHasMetricSort[2];
        
        for(int i = 0; i < 2; ++i)
        {
            lHasMetricSort[i] = false;
            DSSAxis *axis = lpTemplate->getAxis(i+1);
            DSSSorts* sorts = NULL;
            // DE43292 weilin 2016/09/12 add defensive code to avoid crash
            if (axis)
            {
                sorts=axis->getSorts();
            }

            if(sorts==NULL)
                continue;
            for(int j=0;j<sorts->Count();++j)
            {
                DSSSort* sort=sorts->Item(j);
                if(sort->getSortType() == DssSortMetric)
                {
                    lHasMetricSort[i] = true;
                }
            }
        }
        
        if(lHasMetricSort[0])
        {
            lGBPos = 0;
        }
        else if(lHasMetricSort[1])
        {
            lGBPos = 1;
        }
        else
        {
            lGBPos = lbMetricUnitOnRow ? 1 : 0;
        }
        
        if(lvRuntimeUnits.size() > 0)
        {
            for(int i=0;i<lvRuntimeUnits.size();i++)
            {
                ipGridModel->setIsRunTimeUnit(lvRuntimeUnits[i],true);
            }
            
            UpdateUnitInfo(lGBPos, lvRuntimeUnits);
            /*
             if(!lbMetricUnitOnRow)
             {
             UpdateUnitInfo(0,lvRuntimeUnits);
             }
             else
             {
             UpdateUnitInfo(1,lvRuntimeUnits);
             }*/
        }
    }

	return S_OK;
}

int DSSXTabLayoutInfo::UpdateUnitInfo(int iAxis, const vector<int>& lvRuntimeUnits)
{
    // 3/26/2012 tehe: use vector of vector instead of mppUnit to fix memory leak
    mvvUnit[iAxis].resize(mvNumUnit[iAxis] + lvRuntimeUnits.size());
    
    for(int j=0;j<lvRuntimeUnits.size();j++)
    {
        mvvUnit[iAxis][mvNumUnit[iAxis]+j] = new DSSXTabLayoutUnitInfo();
        mvvUnit[iAxis][mvNumUnit[iAxis]+j]->Init(lvRuntimeUnits[j],false);
    }

    mvNumUnit[iAxis] = mvNumUnit[iAxis] + lvRuntimeUnits.size();
    mvRunTimeUnits[iAxis] = lvRuntimeUnits.size();
    return S_OK;
}

int DSSXTabLayoutInfo::get_Tag(int *pTag)
{
	if(pTag == NULL)
		return E_POINTER;
    
	*pTag = mTotalTag;
	return S_OK;
}

int DSSXTabLayoutInfo::put_Tag(int iTag)
{
	mTotalTag = iTag;
    
	mfTotalTagFinal = true;
    
	return S_OK;
}
