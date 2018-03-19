#import "BaseFormatManagerHelper.h"
#import "BaseFormatManager.h"
#import "../../../../../iOS/MicroStrategyMobileSDK/DashboardViewer/Properties.h"
#import "../../../../../iOS/MicroStrategyMobileSDK/DashboardViewer/GridIterator/GridCell.h"
#import "../../../../../iOS/MicroStrategyMobileSDK/DashboardViewer/GridIterator/GridIterator.h"
#import "DSSRWTemplateNode.h"
#import "GridFormatting.h"
#import "DSSRWGridIterator.h"

#ifndef ARRAY_KEY
#define ARRAY_KEY
#endif


namespace MsiFormat{
    BaseFormatManager::Ptr CreateBaseFormatManager(DSSRWGridIterator* ipIterator)
	{
		DSSRWTemplateNode* lpTemplateNode = static_cast<DSSRWTemplateNode*>(ipIterator->getRWNode());
		return new BaseFormatManager(lpTemplateNode->getTemplate(), ipIterator);
	}
	
	BaseFormatManager::BaseFormatManager(DSSTemplate* ipTemplate, DSSRWGridIterator* ipIterator): mFormatInfoPtr(NULL), mpCustomGroupInfo(NULL), mpInfo(NULL)
    ,mpBaseFormatManagerHelper(NULL)
	{
		hInit(ipTemplate, ipIterator);
	}
	
	void BaseFormatManager::hInit(DSSTemplate* ipTemplate, DSSRWGridIterator* ipIterator)
	{
		//note we now allow one tree to be empty
		mIsLayersInited = false;
		mIsLinkedDrillableUnitSetReady = false;
		mIsRegularDrillableUnitSetReady = false;
        mParseThresholds = false;
        mIsValid = (ipTemplate != NULL);
        mThemeType = THEMETYPE_UNDEFINED;
		mCurrentCellPosition = GA_INVALID;
        mAbsoluteCoordinates = false;
		if (mIsValid)
		{
			bool lIsRowHeaderEmpty = true;
            // weilin DE43292 2016/09/12 add defensive code to avoid crash
            if (ipTemplate && ipTemplate->getRow())
            {
                lIsRowHeaderEmpty = (ipTemplate->getRow()->Count() == 0);
            }
			bool lIsColHeaderEmpty = true;
            // weilin DE43292 2016/09/12 add defensive code to avoid crash
            if (ipTemplate && ipTemplate->getColumn())
            {
                lIsColHeaderEmpty = (ipTemplate->getColumn()->Count() == 0);
            }
			//bool lIsBothEmpty = lIsRowHeaderEmpty && lIsColHeaderEmpty;
			mpInfo = NULL;
			mIsValid = true;
			mpCurrentGridCell = NULL;
			mHasCellLevelFormat = false; //Should Get from Ji's API or another approach.
            mAbsoluteCoordinates = false;
			if (mIsValid)
			{
				mpTemplate = ipTemplate;
				mpIterator = ipIterator;
				if (mpIterator->CountRows() == 0 && mpIterator->CountColumns() == 0)
				{
					mIsValid = false;
					return;
				}
				
                mAbsColDataGridOrigin = false;
				mKey = mpIterator->getRWNode()->getKey();
				mHasCellLevelFormat = mpIterator->HasCellFormatData();
				mpCurrentCellData = NULL;
				
				mCurrentRowIndex = mCurrentColIndex = -1;
				mLayerToParse = FLI_RESERVED;
				mFormatInfoPtr = new FormatInfo();
				mDssFormatTotal = 0;
                mUniformElementsSpan = -1;
#ifdef ARRAY_KEY
				mDssFormatTotal = mpTemplate->getObjectContext()->getFormatCount();
				mFormatInfoPtr->InitKey(mDssFormatTotal);
#endif
				//only fetch numberformatting or not.
				mIsNumberformatExclusive = false;
				//with or without metrics?
				mHasMetrics = (mpTemplate->getTemplateMetrics() != NULL);
				//empty row axis?
				mIsRowHeaderEmpty = lIsRowHeaderEmpty;
				//empty col axis?
				mIsColHeaderEmpty = lIsColHeaderEmpty;
				//MetricInfo instance
				mpInfo = new MetricUnitInfo(mpTemplate);
				//extra column -> mHasExtralColumn;
                mpBaseFormatManagerHelper = new BaseFormatManagerHelper(this , mpTemplate, mpInfo);
				mHasExtraColumn = hSetExtraCloumnFlag();
				mHasConsolidationOnRow = false;
				mHasConsolidationOnColumn = false;
				mHasDerivedElementOnRow = false;
				mHasDerivedElementOnCol = false;
				mIsValid = mpInfo->mIsValid;
				if (!mIsValid)
					return;
				_ASSERTE(mpInfo->mHasMetrics == mHasMetrics);
				//Use Graph Properties
				mIsUseGraphProperties = true;
				//CustomGroupInfo instance
				mHasCustomGroup = false;
				mpCustomGroupInfo = new CustomGroupUnitInfo(mpIterator);
				mIsValid = mpCustomGroupInfo->mIsValid;
				if (!mIsValid)
					return;
				if(!mpCustomGroupInfo->hasCustomGroupInRow() && !mpCustomGroupInfo->hasCustomGroupInColumn())
					mHasCustomGroup = false;
				else
					mHasCustomGroup = true;
                
				//layout of BaseFormatManager
				mRowHeaderWidth = mColHeaderHeight = mRowHeaderStartIndex = mColHeaderStartIndex = 0;
				if (!mIsColHeaderEmpty)
				{
                    hGetSubtotalMatrix(false, mColSubtotalMatrix);
                    hGetIndexMap(false);
                    mColHeaderHeight = (Int32)mColIndexToUnitIndex.size();
                    if (!mIsRowHeaderEmpty)
                        mRowHeaderStartIndex = mColHeaderHeight - 1;
				}
                else
                {
                    hCheckLayerSignals(false);
                }
//                else if (mIsColHeaderEmpty && (mIsShowColHeader || mIsShowRowHeader))
//                {
//                    mColHeaderHeight = 1;
//                }
                
				if (!mIsRowHeaderEmpty)
				{
					hGetSubtotalMatrix(true, mRowSubtotalMatrix);
					hGetIndexMap(true);
					mRowHeaderWidth = (Int32)mRowIndexToUnitIndex.size();
					mColHeaderStartIndex = mRowHeaderWidth;
				}
                else
                {
                    hCheckLayerSignals(false);
                }
                
				if (mHasExtraColumn)
					mColHeaderStartIndex ++;
				
				hSetDataGridOrigin();
			}
		}
	}
	
    BaseFormatManagerHelper::~BaseFormatManagerHelper()
    {
        mpBaseFormatManager = NULL;
        mpTemplate = NULL;
        mpInfo = NULL;
    }

    bool BaseFormatManagerHelper::SetExtraCloumnFlag()
	{
        
		bool lIsShowRowHeader = true;
		bool lIsShowColHeader = true;
		bool lIsTitleOverlap = true;
		CComVariant* lpVariant = NULL;
        
		lpVariant = [Properties templateShowRowHeaders:mpTemplate];  _ASSERTE(lpVariant != NULL && lpVariant->vt == VT_BOOL);
        lIsShowRowHeader = (lpVariant->boolVal == VARIANT_TRUE);
        mpBaseFormatManager->SetShowRowHeader(lIsShowRowHeader);
        
		lpVariant = [Properties templateShowColHeaders:mpTemplate];	_ASSERTE(lpVariant != NULL && lpVariant->vt == VT_BOOL);
        lIsShowColHeader = (lpVariant->boolVal == VARIANT_TRUE);
        mpBaseFormatManager->SetShowColHeader(lIsShowColHeader);
        
		lpVariant = [Properties titleOverlapViewMode:mpTemplate]; _ASSERTE(lpVariant != NULL && lpVariant->vt == VT_I4);
		lIsTitleOverlap = [Properties ToBool: lpVariant];
        
		if (!lIsShowRowHeader || !lIsShowColHeader)
			return false;
		if (mpBaseFormatManager->IsColHeaderEmpty())							//changed to adapt new logic
			return false;
		
		//# 458390
		if (mpBaseFormatManager->IsRowHeaderEmpty() && !mpBaseFormatManager->IsColHeaderEmpty())
		{
			Int32 lMetricNumberOnCol = 0;
			if (mpInfo->mHasMetrics && !mpInfo->mIsInRowAxis)
				lMetricNumberOnCol = 1;
			DSSAxis* lpColumn = mpTemplate->getColumn();
			if (mpBaseFormatManager->IsShowColHeader() && lpColumn->Count() > lMetricNumberOnCol)
			{
				return true;
			}
		}
        
		//439577
		//if (mIsRowHeaderEmpty && !mIsColHeaderEmpty)	//changed to adapt new logic
		//	return true;
		return !lIsTitleOverlap;
	}
    
    //Exposed APIs to get format
	CComVariant* BaseFormatManagerHelper::GetFormat(Int32 iPropertyID)
	{
		//template level property
		switch (iPropertyID)
		{
			case DssGridGraphDisplayMode:
			{
                DSSRWGridIterator* lpIterator = mpBaseFormatManager->GetIterator();
				DSSRWTemplateNode* lpNode = static_cast<DSSRWTemplateNode*>(lpIterator->getRWNode());
				if (lpNode->getDisplayMode() == DssDisplayModeGrid)
				{ //Grid
                    BaseFormatManager::sVariant.vt = VT_I4;
					BaseFormatManager::sVariant.lVal = 1;
				}
				else
				{//Graph
					BaseFormatManager::sVariant.vt = VT_I4;
					BaseFormatManager::sVariant.lVal = 2;
				}
				return &BaseFormatManager::sVariant;
			}
				break;
			case DssGridColumnScenario:
				return [Properties columnScenario:mpTemplate];
				break;
			case DssGridLockRowHeaders:
				return [Properties lockRowHeaders:mpTemplate];
				break;
			case DssGridMergeCells:
				return [Properties mergeCells:mpTemplate];
				break;
			case DssGridMergeColumnCells:
				return [Properties mergeColumnCells:mpTemplate];
				break;
			case DssGridLongNames:
				return [Properties longNames:mpTemplate];
				break;
			case DssGridTemplateShowRowHeaders:
				return [Properties templateShowRowHeaders:mpTemplate];
				break;
			case DssGridTemplateShowColHeaders:
				return [Properties templateShowColHeaders:mpTemplate];
				break;
			case DssGridLockColumnHeaders:
				return [Properties lockColumnHeaders:mpTemplate];
				break;
			case DssGridTitleOverlapViewMode:
				return [Properties titleOverlapViewMode:mpTemplate];
				break;
			case DssGridShowExtraColumn:
				BaseFormatManager::sVariant.vt = VT_BOOL;
				BaseFormatManager::sVariant.boolVal = (mpBaseFormatManager->HasExtraColumn()) ? VARIANT_TRUE : VARIANT_FALSE;
				return &BaseFormatManager::sVariant;
			case DssGridRowScenario:
				return [Properties rowScenario:mpTemplate];

			case _templateBanding:
				return [Properties templateBanding:mpTemplate];
			default:
				_ASSERTE(false);
				return NULL;//to purge warnings.
		}
		_ASSERTE(false); //to purge warnings.
		return NULL;
	}
    
    FormatInfo::Ptr BaseFormatManagerHelper::GetFormat(void* ipGridCell, bool iIsPassToGridCell)
	{
        mpBaseFormatManager->SetCurrentGridCell(ipGridCell);
		GridCell* lpGridCell = (__bridge GridCell*) (ipGridCell);
		Int32 lRow = lpGridCell.row - 1;
		Int32 lCol = lpGridCell.col - 1;
		FormatInfo::Ptr lFormatPtr = mpBaseFormatManager->GetFormat(lRow, lCol);
		SetDrillable(ipGridCell, lFormatPtr);
        
		if (iIsPassToGridCell) //pass to grid cell
		{
#ifdef FORMAT_DEBUG
			if (lFormatPtr->Find(_numberFormat))
			{
				CComVariant* lpVariant = lFormatPtr->GetProperty(_numberFormat);
				MBase::String lStr(lpVariant->bstrVal);
				int i = 0;
			}
#endif
			[lpGridCell	setCellFormat:lFormatPtr];
		}
		return lFormatPtr;
	}
    
    void BaseFormatManagerHelper::SetDrillable(void* ipGridCell, FormatInfo::Ptr iFormatPtr)
	{
		GridCell* lpGridCell = (__bridge GridCell*) (ipGridCell);
		if (lpGridCell.cellType != CellCategoryHeader)
			return;
        
		mpBaseFormatManager->GetLinkedDrillableUnitSet();
		mpBaseFormatManager->GetRegularDrillableUnitSet();
        
		/*if (mDrillableColumns.find(lpGridCell.col) != mDrillableColumns.end())
         {
         iFormatPtr->SetDrillable();
         return;
         }*/
		std::set<std::pair<Int32, Int32> > lRegularDrillableUnits;
        std::set<std::pair<Int32, Int32> > lLinkedDrillableUnits;
        mpBaseFormatManager->GetRegularDrillableUnit(lRegularDrillableUnits);
        mpBaseFormatManager->GetLinkedDrillableUnit(lLinkedDrillableUnits);
        
		if (lRegularDrillableUnits.find(make_pair([lpGridCell axis], [lpGridCell unitIndex])) != lRegularDrillableUnits.end())
		{
			iFormatPtr->SetDrillable();
		}
		else if (lLinkedDrillableUnits.find(make_pair([lpGridCell axis], [lpGridCell unitIndex])) != lLinkedDrillableUnits.end())
		{
			iFormatPtr->SetDrillable();
		}
        else
		{
			iFormatPtr->SetDrillable(false);
		}
		return;
        
		/*
         DSSTemplateUnit* lpUnit = NULL;
         DSSAxis* lpAxis = NULL;
         Int32 lUnitIndex = [lpGridCell unitIndex];
         
         if ([lpGridCell axis] == 0)
         lpAxis = mpTemplate->getRow();
         else
         lpAxis = mpTemplate->getColumn();
         
         if (lpAxis && lUnitIndex < lpAxis->Count())
         lpUnit = lpAxis->Item(lUnitIndex);
         
         if (!lpUnit)
         return;
         
         //TQMS 460017, need to check whether the unit has linked drill or not. By xliu, 4/27/2011.
         std::string lUnitData = lpUnit->getUnitData();
         if (hHasLinkedDrill(lUnitData))
         {
         iFormatPtr->SetDrillable();
         return;
         }
         
         //TQMS 473242, need to check whehter there is DE along the path. By xliu, 4/27/2011.
         DSSCellHandle lCellHandle = lpGridCell.cellHandle;
         if (mpIterator->HasDerivedElementAlongPath(lCellHandle))
         {
         return;
         }
         
         if (lpUnit)
         {
         DSSDrillPathRefs* lpDrillPathRefs = lpUnit->getDrillPaths();
         int lDrillCount = lpUnit->getDrillPaths()->Count();
         for (int i = 0; i <lDrillCount; i++)
         {
         DSSDrillPath* lpDrillPath = lpDrillPathRefs->Item(i);
         if (lpDrillPath->getDrillImportance() == DssDrillImportanceHigh)
         iFormatPtr->SetDrillable();
         }
         }
         //mDrillableColumns.insert(lpGridCell.col);
         */
	}
    
    bool BaseFormatManagerHelper::HasLinkedDrill(DSSTemplateUnit* ipTemplateUnit)
	{
//		std::string lUnitData = ipTemplateUnit->getUnitData();
//		
//		NSString* linkDrillDef = [ NSString stringWithUTF8String:lUnitData.c_str()];
//		linkDrillDef = [linkDrillDef  stringByTrimmingCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@" "]];
//		CFStringRef stringRef = CFURLCreateStringByReplacingPercentEscapesUsingEncoding(kCFAllocatorDefault, (CFStringRef)linkDrillDef, CFSTR(""), kCFStringEncodingUTF8);
//		NSString* realLinkDrilling = [NSString stringWithFormat:@"%@", stringRef];
//		CFRelease(stringRef);
//		
//		if (!realLinkDrilling || [realLinkDrilling length] == 0) {
//			return false;
//		}
//		LinkDrillingXMLHandler* linkDrillingHandler = [[LinkDrillingXMLHandler alloc] init];
//		[linkDrillingHandler loadXML:realLinkDrilling];
//		Link* _lnk = [linkDrillingHandler.lnks defaultLink];
//		if (_lnk) {
//			return true;
//		}
//		else {
//			return false;
//		}
        return false;
	}
    
    Int32 BaseFormatManagerHelper::GetRowHeight()  //for fix mode only
	{
		CComVariant* lpVal = NULL;
		lpVal = [Properties rowHeight:mpTemplate];
		_ASSERTE(lpVal != NULL);
		return [Properties ToInt:lpVal];
	}
    
    Int32 BaseFormatManagerHelper::GetColumnWidth(Int32 iColumnIndex)
	{
		CComVariant* lpVal = [Properties columnScenario:mpTemplate];
		_ASSERTE(lpVal != NULL && lpVal->vt == VT_I4);
		bool lIsFixedMode = (lpVal->lVal == 3);
		Int32 lRetWidth = -1;
		
		
		if (lIsFixedMode)
		{
			Int32 lX = iColumnIndex;
			Int32 lY = 0;

			if (!mpBaseFormatManager->IsShowRowHeader())						//hiden header case. Map to normal tempalte.
				lX += mpBaseFormatManager->GetAbsColDataGridOrigin();
			if (!mpBaseFormatManager->IsShowColHeader())
                lY += mpBaseFormatManager->GetAbsRowDataGridOrigin();
			
			GRID_AREA lArea	= mpBaseFormatManager->MapToDataCellIndex(lX, lY, false);
			
			if (lArea == GA_EMPTY_ZONE)
				lArea = GA_ROW;
			
			if (lArea == GA_EXTRACOLUMN)
			{
				_ASSERTE(mpBaseFormatManager->HasExtraColumn());
				if (mpBaseFormatManager->IsColHeaderEmpty())
					lRetWidth = 0;
				else
				{
					lpVal = [Properties extraWidth:mpTemplate];
					_ASSERTE(lpVal != NULL && lpVal->vt == VT_I4);
					lRetWidth = (Int32)lpVal->lVal;
				}
				if (lRetWidth < 0)
					lRetWidth = 100;
				return lRetWidth;
			}
			
			if (lArea == GA_ROW)
			{
				Int32 lDepth = 1;
				Int32 lCount = 0; //to purge warnings.
				lCount = mpTemplate->getRow()->Count();
                
                std::map<Int32, Int32> lRowIndexToUnitIndex;
                mpBaseFormatManager->GetRowIndexToUnitIndex(lRowIndexToUnitIndex);
				_ASSERTE(lRowIndexToUnitIndex.size() > 0);
				Int32 lUnitIndex = lRowIndexToUnitIndex[iColumnIndex];
                
                std::map<Int32, Int32> lUnitIndexToRowStartOrdinal;
                mpBaseFormatManager->GetUnitIndexToRowStartOrdinal(lUnitIndexToRowStartOrdinal);
				_ASSERTE(lUnitIndex < lCount);
				Int32 lStartOrdinal = lUnitIndexToRowStartOrdinal[lUnitIndex];
				DSSTemplateUnit* lpUnit = mpTemplate->getRow()->Item(lUnitIndex);
				EnumDSSTemplateUnitType lType = lpUnit->getType();
				if  (lType == DssTemplateAttribute) //For attribute, we count its forms.For others we use depth = 1.
				{
					lDepth = iColumnIndex - lStartOrdinal + 1; //1-based, in Ming's code.
				}
				
				lRetWidth = lpUnit->getColumnWidth(lDepth);
				
				if (lRetWidth < 0)
					lRetWidth = 100;
				return lRetWidth;
			}
			
			if( (lArea == GA_COLUMN || lArea == GA_DATA) && mpBaseFormatManager->HasMetrics())
			{
				MetricUnitInfo* lpInfo = mpInfo;
				_ASSERTE(lpInfo != NULL);
				DSSTemplateMetrics* lpMetrics = mpTemplate->getTemplateMetrics();
				_ASSERTE(lpMetrics != NULL);
				
				if (lpInfo->mIsInRowAxis)
				{
					//use width of the first template metric
					lRetWidth = lpMetrics->Item(0)->getMetricWidth();
				}
				else
				{
                    Int32 lActualColIndex = iColumnIndex; //index mapped to a normal template.
                    if (!mpBaseFormatManager->IsShowRowHeader())//hiden header case. Map to normal tempalte.
                        lActualColIndex += mpBaseFormatManager->GetAbsColDataGridOrigin();
                    
					GUID lMetricID = mpBaseFormatManager->GetMetricIDFromColRowIndex(0, lActualColIndex - mpBaseFormatManager->GetColHeaderStartIndex());
					Int32 lCount = lpMetrics->Count();
					for (Int32 i = 0; i < lCount; i++)
					{
						if (lpMetrics->Item(i)->getMetric()->getID() == lMetricID)
						{
							lRetWidth = lpMetrics->Item(i)->getMetricWidth();
							break;
						}
					}
				}
                
				if (lRetWidth < 0)
					lRetWidth = 100;
				return lRetWidth;
			}
			
			if (lArea == GA_DATA_NOMETRIC || !mpBaseFormatManager->HasMetrics())
			{
				_ASSERTE(mpBaseFormatManager->HasMetrics() == false);
				//use template default
				lpVal = [Properties gridWidth:mpTemplate];
				_ASSERTE(lpVal != NULL && lpVal->vt == VT_I4);
				lRetWidth = (Int32)lpVal->lVal;
                
				if (lRetWidth < 0)
					lRetWidth = 100;
				return lRetWidth;
			}
		}
		
		if (lRetWidth < 0)
			lRetWidth = 100;
		return lRetWidth;
	}
    void BaseFormatManagerHelper::RegisterOneLayer(FORMAT_LAYER_INDEX iLayerIndex, GUID& irID, FORMAT_CATEGORY& irCellType)			  //Push one layer into the layer stack.
	{
		BaseFormatting* lpFormatting = NULL;
		switch (iLayerIndex)
		{
			case FLI_THRESHOLD:
                if (mpTemplate->hasThreshold())
                    lpFormatting = new ThresholdFormatting(irID, irCellType,mpBaseFormatManager);
				break;
			case FLI_SUBTOTAL:
            {
				if (mpBaseFormatManager->GetColSubtotalMatrixSize() > 0 || mpBaseFormatManager->GetRowSubtotalMatrixSize() > 0)
					lpFormatting = new SubTotalFormatting(irID, irCellType, mpBaseFormatManager);
				break;
            }
			case FLI_BANDING:
            {
                bool lIsUseBanding = ([Properties bandingOptions:mpTemplate])->lVal != 0;
                if (!mpTemplate->IsBandingHidden() && lIsUseBanding)
                    lpFormatting = new BandingFormatting(irID, irCellType, mpBaseFormatManager);
            }
				break;
			case FLI_TEMPLATE:
				lpFormatting = new TemplateFormatting(irID, irCellType, mpBaseFormatManager);
				break;
			case FLI_DERIVEDELEMENT:
				if (mpBaseFormatManager->HasDerivedElementOnRow() || mpBaseFormatManager->HasDerivedElementOnCol())
					lpFormatting = new DerivedElementFormatting(irID, irCellType, mpBaseFormatManager);
				break;
			case FLI_COMPOSITE_CG_CONSO:
				if (mpBaseFormatManager->HasCustomGroup() || mpBaseFormatManager->HasConsolidationOnRow() || mpBaseFormatManager->HasConsolidationOnColumn())
				{
					lpFormatting = new CGAndConsolidationFormatting(irID, irCellType, mpBaseFormatManager);
				}
				break;
			case FLI_CELL:
				if (mpBaseFormatManager->HasCellLevelFormat())
				{
					lpFormatting = new CellLevelFormatting(irID, irCellType, mpBaseFormatManager);
				}
				break;
			default:
				_ASSERTE(false);
		}
        
		if (lpFormatting != NULL)
            mpBaseFormatManager->AddFormattingLayer(lpFormatting);
	}
    /********************************************************************************
	 *																				*
	 *		Following are helper functions for debugging, private to this class		*
	 *																				*
	 *********************************************************************************/
	
	
	void BaseFormatManagerHelper::DumpTotalNumberFormat()    //this is for [testing].
	{
		NSLog(@"\r\rDump Total Formatting Begins\r");
		DSSAxis* lpAxis = NULL;
		DSSFormat* lpFormat = NULL;
		CComVariant* lpNumberFormat = NULL;
		for (Int32 j = 1; j < 3; j++)
		{
			NSLog(@"Subtotal Header from Axis %d :\r", j);
			lpAxis = mpTemplate->getAxis(j);
			
			lpFormat = lpAxis->getSubtotalHeaderFormat();
			lpNumberFormat = [Properties numberFormat:lpFormat];
			_ASSERTE(lpNumberFormat != NULL && lpNumberFormat->vt == VT_BSTR);
			//NSLog(@"axis level with subtotal header: %s\r", lNumberFormat.c_str());
			
			lpFormat = lpAxis->getSubtotalGridFormat();
			lpNumberFormat = [Properties numberFormat:lpFormat];
			_ASSERTE(lpNumberFormat != NULL && lpNumberFormat->vt == VT_BSTR);
			//NSLog(@"axis level with subtotal grid: %s\r", lNumberFormat.c_str());
			
			NSLog(@"Unit on this axis: \r");
			Int32 lCount = lpAxis->Count();
			for (Int32 i = 0; i < lCount; i++)
			{
				DSSTemplateUnit* lpUnit = lpAxis->Item(i);
				lpFormat = lpUnit->getSubtotalHeaderFormat();
				lpNumberFormat = [Properties numberFormat:lpFormat];
				_ASSERTE(lpNumberFormat != NULL && lpNumberFormat->vt == VT_BSTR);
				//	NSLog(@"	Unit %d with total header: %s \r", i, lpNumberFormat->bstrVal.c_str());
				
				lpFormat = lpUnit->getSubtotalGridFormat();
				lpNumberFormat = [Properties numberFormat:lpFormat];
				_ASSERTE(lpNumberFormat != NULL && lpNumberFormat->vt == VT_BSTR);
				//	NSLog(@"	Unit %d with total grid: %s \r", i, lNumberFormat.c_str());
			}
			
			NSLog(@"\r\r\r");
		}
		NSLog(@"Dump Total Formatting Ends\r");
	}
    
    
    /*	void FormatInfo::hTraceVariant(CComVariant* ipVariant)
     {
     printf(" <<<< Grid Property Debugging : PropId = %d, SeriesId = %d, ObjId = %d  >>>>\n", mKey.c_str(), iPropertyId, iSeriesId, iObjectId);
     printf("Result: Prop Type = %d Prop Value = ", opValue->vt);
     switch (opValue->vt)
     {
     case VT_I4:
     printf("%l\n", opValue->lVal);
     break;
     case VT_I2:
     printf("%d\n", opValue->iVal);
     break;
     case VT_UI1:
     printf("%d\n", opValue->bVal);
     break;
     case VT_BOOL:
     printf("%s\n", opValue->boolVal == VARIANT_TRUE ? "true" : "false");
     break;
     case VT_BSTR:
     {
     MBase::String lStrValue(opValue->bstrVal);
     printf("%s\n", lStrValue.c_str());
     break;
     }
     }
     }
     */
    
    
    bool BaseFormatManagerHelper::HasBanding()const
    {
        bool lIsUseBanding = ([Properties bandingOptions:mpTemplate])->lVal != 0;
        if (!mpTemplate->IsBandingHidden() && lIsUseBanding)
            return true;
        return false;
    }

}