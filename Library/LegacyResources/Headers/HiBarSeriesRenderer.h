//
//  HiBarSeriesRenderer.h
//  IPhoneChart
//
//  Created by Shi, Dong on 12/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef MsiChart_HiBarSeriesRenderer_h
#define MsiChart_HiBarSeriesRenderer_h

#include "GraphConsolidationObject.h"
#include "BarSeriesRenderer.h"


namespace MsiChart
{
    typedef enum
    {
        HRT_LIPSTIK = 0,
        HRT_OVERLAP = 1,    
    }EnumBudgetRenerType;
    
    typedef enum
    {
        HDI_ACT = 0,
        HDI_TGT = 1,
    }EnumBudgetDataIndex;
    
    typedef enum
    {
        HBSR_ACT = 1,
        HBSR_TGT = 2,
        HBSR_BOTH = 3,
    }EnumHiBarSearchRule;
    
    struct HiLabelInfo
    {
        Int32 mGroupIndex;
        bool mIsShown;          //< decided by bar position, like in overlap bar mode.
        bool mIsAtopBar;        //< if the label is on top of the bar.
        Point2D mPosition;
        double mDataValue;      //< value for the data label.
        HiLabelInfo()
        {
            mIsAtopBar = true;
            mIsShown = false;
            mGroupIndex = -1;
            mPosition = Point2D(-1, -1);
            mDataValue = 0;
        }
    };
    
    class HiBarSeriesRenderer: public BarSeriesRenderer
    {
    public:      
        
        ///	Type definition
		typedef MSynch::SmartPtr<HiBarSeriesRenderer> Ptr;
        
        HiBarSeriesRenderer(const TripleId& irTripleId, GraphObjectManager* ipManager,
                            Dataset::Ptr iDatasetPtr, Legend::Ptr iLegendPtr);
        
		virtual ~HiBarSeriesRenderer()
		{
		}
        
        virtual void GenerateByValueAxis(EnumValueAxis iValueAxis, std::vector<GraphObject::Ptr>& iorGraphObjectList);

		///	Load default bar graph related properties from ChartContext
		virtual void LoadProperties();

        
        //override from GraphObjectManager
		virtual GraphObject::Ptr OnPointDetectionEx(const Point2D& irCursor, 
													const std::vector<Int32>& irObjectIds, Int32& iorDistance, Int32 iRadius,
													std::vector<GraphObject::Ptr>& iorGraphObjectsAround, EnumGraphObjectUsage iUsage = GOU_NORMAL_HIGHLIGHT); 
        virtual GraphObject::Ptr GetGraphObject(const TripleId& irTripleId, bool iRecursive = true) const;

        
    protected: 
        virtual void hRenderItems(EnumValueAxis iVA, BOOL iIsOnlyForDataLabels);
        virtual void hCalBarWidth();
		virtual void hReserveAreaForDataLabel(Rect2D &iorRect);
        virtual void hClearUp();
        virtual bool hIsLabelShown();

    private://data members
        
        void hRenderItemsPerGroup(const RiserInfo& irRiserInfo, std::vector<HiLabelInfo>& orPrimaryLabelCollection, std::vector<HiLabelInfo>& orSecondaryLabelCollection);

        void hPopulate(); //hard coded logic and also initialization of the data members.
        
        void hUpdateLabelWithRects(Rect2D ipRectArray[], const std::vector<double>& irValues, HiLabelInfo& orACTLabelInfo, HiLabelInfo& orTGTLabelInfo);
        //Return the label to be shown.
        Int32 hSmartLabelAtGroup(Int32 iGroupIndex);
        Int32 hSmartLabels();
        Int32 hCheckOneLabel(const TripleId& irLabelId);
        
        //layout
        bool mIsVertical;
        EnumBudgetRenerType mRenderType;
        
        
        //Additional lines & dot
        bool mShowAddtionalLines;
        Int32 mLineCollectionsCount;

        std::vector<GraphCollectionObject::Ptr> mAddtionalLineCollections;
        std::vector<GraphCollectionObject::Ptr> mAddtionalDotCollections;

        GraphCollectionObject::Ptr mAddtionalDotCollection;
        GraphCollectionObject::Ptr mVirtualMarkerCollection;
        void hGenerateAddtionalLinesInSeries(Int32 iSeriesIndex, std::vector<Int32>& irNullGroupIndexSet, std::vector<std::vector<double> >& irLinePointValues);
        GraphObject::Ptr hGenerateOneDot(const TripleId& irId, bool iIsVirtualMarker, const Point2D& irSuggestedPosition);
        GraphObject::Ptr hGenerateOneAdditionalLineSection(const TripleId& irId, const std::vector<double>& irLineSectionPointValues, std::vector<Point2D>& orLinePoints);
        bool hComputeExtendedPoint(Point2D& iorPoint, Int32 iGroupIndex, CategoryAxis::Ptr iCategoryAxisPtr, ValueAxis::Ptr iValueAxisPtr, const std::vector<double>& irLineSectionPointValues);
        
        
        bool hShouldDrawZeroCustomerLine();
        void hDrawCustomerLine();
        
        //Bars
        GraphCollectionObject::Ptr mIncrementBarCollection;   //IncrementBar
        GraphCollectionObject::Ptr mDecrementBarCollection;   //DecrementBar
        GraphCollectionObject::Ptr mActualBarCollection;
        GraphCollectionObject::Ptr mTargetBarCollection;
        
        Rect2D hLayoutOneBar(const RiserInfo& irRiserInfo, const std::vector<bool>& irIsEmptyValue, const std::vector<double>& irValues, bool& orDoesTargetBarPrevail);
        virtual void hGetRiserStartPosAndSize(const RiserInfo& irRiserInfo, Int32& orStart, Int32& orSize, const std::vector<bool>& irIsEmptyValue, const std::vector<double>& irValues);
        GraphObject::Ptr hFindBarRiserInGroup(Int32 iGroupIndex, EnumHiBarSearchRule iRule, bool iIsACTBarPrevail);
		virtual Int32 hGetIndexInColumn(const RiserInfo& irRiserInfo);

        //Labels.
        bool mShowLabels;
        bool mShouldLabelRotate;
        std::vector<bool> mShowSeriesLabels;
        std::vector<GraphCollectionObject::Ptr> mDataLabelCollections;
        std::map<TripleId, GraphObject::Ptr> mLabelMap;
        Int32 hPlaceLabels(std::vector<TextObject::Ptr>& orLabels, bool iIsRotated, const std::vector<HiLabelInfo>& irActLabelCollection, const std::vector<HiLabelInfo>& irTargetLabelCollection);
        void hAddLabelsToMapCollectionAndList(const std::vector<TextObject::Ptr>& irLabels);
        void hGenerateLabelsInSeries(Int32 iSeriesIndex, bool iIsRotated, const std::vector<HiLabelInfo>& irLabelCollection, std::vector<TextObject::Ptr>& orLabels);
        void hSetRotationForLabelCollections(bool iIsRotated);
        void hGetExtremeLabels(std::vector<TripleId>& orExtremeLabels);
        GraphObject::Ptr GetLabelFromMap(const TripleId& irLabelId);
        

        GraphCollectionObject* hFindRiserCollection(const RiserInfo& irInfo, bool iTargetBarPrevail);
        GraphCollectionObject* hGetSimpleCollection(Int32 iSeriesIndex);
        
        //helper functions
        bool hGetValues(const RiserInfo& irRiserInfo, std::vector<bool>& orIsEmptyValue, std::vector<double>& orValues);
        bool hGetValues(Int32 iSeriesIndex, std::vector<Int32>& orLineSectionStartIndexSet, std::vector<std::vector<double> >& orLinePointValues);
        
        //tooltip & hover
        /// Provide position information for tooltip display on iPad.
		virtual bool GetDataPoint(Int32 iSeriesId, Int32 iGroupId, Point2D& orDataPoint);
        void hCreateconsolidationObjects();
        GraphConsolidationObject::Ptr hFindConsolidationObject(Int32 iGroupIndex);
        GraphConsolidationObject::Ptr hGetConsolidationObject(Int32 iGroupIndex) const;
        std::vector<GraphConsolidationObject::Ptr> mConsolidationObjects;
        
        //legend marker
        void hAddLegendMarkersToCollections();
        
        //Insufficient data.
        bool mIsInsufficientDataAllowed;
        bool mIsInsufficientData;
        
        //extreme label ids. postive side and negative side
        std::vector<TripleId> mExtremeLabelIds;
        
    };
}

#endif

