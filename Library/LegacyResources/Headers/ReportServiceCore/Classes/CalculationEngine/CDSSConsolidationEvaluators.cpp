//
//  CDSSConsolidationEvaluators.cpp
//  ReportServiceCore
//
//  Created by Liao Taimin on 4/17/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#include "CDSSConsolidationEvaluators.h"

#import "DSSFunctionObject.h"
#import "DSSObjectContext.h"
#import "DSSFunctionServer.h"
#import "DSSTabularConsolidation.h"
#import "CDSSConsolidationEvaluator.h"
#import "DSSConsolidationElement.h"

CDSSConsolidationEvaluators::CDSSConsolidationEvaluators():mpXTabConsolidation(NULL), mpFunctionServer(NULL)
{}

CDSSConsolidationEvaluators::~CDSSConsolidationEvaluators()
{
	Int32 iEvaluator, lnEvaluator = mvEvaluators.size();
    
	for(iEvaluator = 0; iEvaluator < lnEvaluator; iEvaluator ++)
    {
		if(mvEvaluators[iEvaluator])
        {
            delete mvEvaluators[iEvaluator];
            mvEvaluators[iEvaluator] = NULL;
        }
    }
    
	if(mpXTabConsolidation)
    {
        delete mpXTabConsolidation;
        mpXTabConsolidation = NULL;
    }
    
	if(mpFunctionServer)
    {
        delete mpFunctionServer;
        mpFunctionServer = NULL;
    }
}

//## Other Operations (implementation)
//int CDSSConsolidationEvaluators::Item(Int32 index, DSSFunctionObject **ppFunctionObject)
DSSFunctionObject* CDSSConsolidationEvaluators::Item (int iIndex)
{    
	//ctang 2010-11-9 translate global to local
	Int32 lIndex = mpXTabConsolidation->TranslateGlobalToLocal(iIndex);
    
	if(lIndex < 0 ||
       lIndex >= mvEvaluators.size())
        return NULL;
    
	if(mvEvaluators[lIndex] == NULL)
		return NULL;

	return mvEvaluators[lIndex];
}

int CDSSConsolidationEvaluators::Count(Int32 *pCount)
{
	CHECK_PTR(pCount);
    
	*pCount = mvEvaluators.size();
    
	return S_OK;
}

int CDSSConsolidationEvaluators::getCLSID(GUID *pCLSID)
{
	return E_FAIL;		//should be internal error
}

int CDSSConsolidationEvaluators::getDescription(char **pDescription)
{
	return E_FAIL;		//should be internal error
}

int CDSSConsolidationEvaluators::getName(char **pName)
{
	return E_FAIL;		//should be internal error
}

int CDSSConsolidationEvaluators::getVersion(char **pVersion)
{
	return E_FAIL;		//should be internal error
}

int CDSSConsolidationEvaluators::init(DSSObjectContext* ipObjContext, DSSFunctionServer *pFunctionServer,DSSTabularConsolidation *pXTabConsolidation)
{
    int hr = S_OK;
    
	AE_ASSERT(pFunctionServer);
	mpFunctionServer = pFunctionServer;
    
	AE_ASSERT(pXTabConsolidation);
	mpXTabConsolidation= pXTabConsolidation;
    
	//not very efficient
	Int32 lnElement = 0,  iElement = 0 ;
	//ctang 2010-11-9 handle for global de case
	lnElement = mpXTabConsolidation -> getNumAllElements();
    
	mvEvaluators.resize(lnElement);
    
	DSS_ID lID;
	for(iElement = 0; iElement < lnElement; iElement ++)
	{
		//ctang 2010-11-9 translate local to global
		Int32 lGlobalElement = mpXTabConsolidation->TranslateLocalToGlobal(iElement);
		hr = mpXTabConsolidation -> getChildElementID(lGlobalElement, &lID);
		if(hr != S_OK && hr != S_FALSE)
			return hr;
        //tliao, 894371, it is possible that lID is GUID_NULL and hr = S_OK, don't know why, maybe there is some change on server side
		if(hr == S_FALSE || lID == GUID_NULL)
			//that would be a leaf element
			mvEvaluators[iElement] = NULL;
		else
		{
			AE_ASSERT(ipObjContext);
			// JLI 9/23/2009 Support UC on DE. Look up from the OC using DssContextFlagFindBySingleIDAllContexts to find cross refered DE
			DSSObjectInfo *lpInfo = NULL;
			lpInfo = ipObjContext->FindObjectH(lID);
			CHECK_HR_PTR(lpInfo);
            
            
			DSSConsolidationElement *lpConsolidationElement = dynamic_cast<DSSConsolidationElement*>(lpInfo);
			CHECK_PTR(lpConsolidationElement);
            
			DSSCExpression *lpExpression = lpConsolidationElement -> getExpression();
			CHECK_PTR(lpExpression);
            
			//can not allocate on buffer
			//if allocate on buffer, then can not call the destructor
			//then the Expression will not get released
			mvEvaluators[iElement] = new CDSSConsolidationEvaluator();
			CHECK_NEWED_PTR(mvEvaluators[iElement]);
            
			hr = mvEvaluators[iElement] -> init(lpExpression, mpFunctionServer);
			CHECK_HR;
            
		}
        
	}
    
	return hr = S_OK;
}


int CDSSConsolidationEvaluators::put_DynamicAggFunc(Int32 iFuncIndex)
{
    int hr = S_OK;
    
	Int32 iEvaluator, lnEvaluator = mvEvaluators.size();
    
	for(iEvaluator = 0; iEvaluator < lnEvaluator; iEvaluator ++)
	{
		if(mvEvaluators[iEvaluator])
		{
			hr = mvEvaluators[iEvaluator] -> put_DynamicAggFunc(iFuncIndex);
			CHECK_HR;
		}
	}
    
	return hr;
}

int CDSSConsolidationEvaluators::put_NonAggFunc(Int32 iFuncIndex)
{
    int hr = S_OK;
    
	Int32 iEvaluator, lnEvaluator = mvEvaluators.size();
    
	for(iEvaluator = 0; iEvaluator < lnEvaluator; iEvaluator ++)
	{
		if(mvEvaluators[iEvaluator])
		{
			hr = mvEvaluators[iEvaluator] -> put_NonAggFunc(iFuncIndex);
			CHECK_HR;
		}
	}
    
	return hr;
}