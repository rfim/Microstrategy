//
//  DataLabelManager.h
//  IPhoneChart
//
//  Created by Li, En on 7/26/13.
//
//

#ifndef IPhoneChart_DataLabelManager_h
#define IPhoneChart_DataLabelManager_h


#include "GridMap.h"
#include "TextObject.h"
#include "ChartContext.h"

#include "GraphObjectManager.h"
namespace MsiChart {
    
    // RefLineDataLabel support
    struct refLableCandidate
    {
        refLableCandidate()
        {
            mPositionIndex = 0;
            mCannotFitHorizontally = false;
            mCannotFitVertically = false;
            mOverlapDataLabelCount = 0;
            mOverlapRefLabelCount = 0;
            mOverlapedDataLablesIndexs.clear();
        }
        
        Int32 mPositionIndex;
        bool mCannotFitVertically;
        bool mCannotFitHorizontally;
        Int32 mOverlapRefLabelCount;
        Int32 mOverlapDataLabelCount;
        std::vector<Int32> mOverlapedDataLablesIndexs;
        Rect2D mPosition;
        Int32 mXOffset;
        Int32 mYOffset;
    };
    
    typedef enum
    {
        GMDLHMS_AREA = 0,
        GMDLHMS_LINE,
        GMDLHMS_BAR,
        GMDLHMS_CIRCLE,
        GMDLHMS_SQUARE
    }EnumDssGraphMatrixDataLabelHostMarkerShape;

    //Data Label Info for all chart types
    struct GMDataLabelInfo
    {
        bool mIsDisPlayed;  //<Whether this label is already been located
        bool mIsValid;      //Is Host marker valid ?
        
        Int32 mSeriesId;
        Int32 mGroupId;
        
        Rect2D mHostMarkerBound;
        
        EnumDssGraphMatrixDataLabelPosition mDataLabelLocation;
        TextObject::Ptr mLabelPtr;
        
        GMDataLabelInfo(): mSeriesId(0),mGroupId(0), mIsDisPlayed(false), mIsValid(false), mDataLabelLocation(GMDLP_BOTTOM), mHostMarkerShape(GMDLHMS_SQUARE)
        {}
        
        Rect2D ToRectangle()
        {
            Rect2D lRect;
            mLabelPtr->GetBoundingRect(lRect);
            return lRect;
        }
        
        std::vector<Point2D> mDLCenterPoint;
        Int32 mCenterXOffset; //used for Line with sizeby, area..., the offset from Base Point.
        Int32 mCenterYOffset;
        std::vector<EnumDssGraphMatrixDataLabelPosition> mDLPosition;
        
        //For Grid Chart
        int mXIndex;
        int mYIndex;
        
        //For Bar/Line/Area Chart
        int mSeriesInfo;   //seriesInfo index
        Point2D mHostMarkerPoint;  //the host marker's position of DL.
        
        EnumDssGraphMatrixDataLabelHostMarkerShape mHostMarkerShape;
    };
    
    typedef std::vector<GMDataLabelInfo> GMDataLabelVecs;
    
    struct GMRefLabelInfo
    {
        bool mIsDisPlayed;  //<Whether this label is already been located
        Int32 mRefLinePos;
        bool mIsVertical;
        
        TextObject::Ptr mLabelPtr;
        
        GMRefLabelInfo(): mIsDisPlayed(false)
        {}
        
        Rect2D ToRectangle()
        {
            Rect2D lRect;
            mLabelPtr->GetBoundingRect(lRect);
            return lRect;
        }
        
    };
    struct AxisOriginLabelInfo
    {
        Int32 mAxisIndex;   //Y1 ,Y2,Y3 or Y4.
        Point2D mRefPoint;
        TextObject::Ptr mLabelPtr;
        bool mIsDisPlayed;  //<Whether this label is already been located
        bool mIsVertical;   //Whther the chart is vertical or horizontal
        bool mIsAttributeMetrixLable;
        
        AxisOriginLabelInfo(bool iIsVertical,Int32 iAxisIndex,Point2D iRefPoint,bool iIsAttributeMetrixLable):
        mIsVertical(iIsVertical),
        mAxisIndex(iAxisIndex),
        mRefPoint(iRefPoint),
        mIsAttributeMetrixLable(iIsAttributeMetrixLable),
        mIsDisPlayed(false)
        {}
        
        Rect2D ToRectangle()
        {
            Rect2D lRect;
            mLabelPtr->GetBoundingRect(lRect);
            return lRect;
        }
        
    };
    
    typedef std::vector<GMRefLabelInfo> GMRefLabelVecs;
    typedef std::vector<AxisOriginLabelInfo> AxisOriginLabelVecs;
    class DataLabelManager
    {
    public:
        
        typedef MSynch::SmartPtr<DataLabelManager> Ptr;
        
        DataLabelManager(const ChartContext::Ptr irChartContextPtr, GraphObjectManager* ipManager = NULL);
        ~DataLabelManager();
        
        //void SetChartContext(const ChartContext::Ptr irChartContextPtr);
        
        //Add Occupied Object in initial
        void AddSinglePointObject(const std::vector<GraphObject::Ptr >& irSinglePointObjects);
        void AddPolygonObject(const std::vector<GraphObject::Ptr >& irPolygonObjects);
        void AddPolyLinePointObject(const std::vector<GraphObject::Ptr >& irPolyLineObjects);
        void AddRectangleObject(const std::vector<Rect2D >& irRectangleObject);
        void AddCircleObject(const std::vector<Circle>& irCirclePieObject);
        
        //Add Data Labels to be detected
        void AddDataLabels(const GMDataLabelVecs& irDataLabels);
        
        //Place Data Labels restore in local list
        void PlaceDataLabels();
        
        //Place Data Labels in the input list
        bool PlaceDataLabels(GMDataLabelVecs& iorDataLabelVec);
        
        //place data labels in combo chart
        bool PlaceDataLabelsForCombo(GMDataLabelVecs &iorDataLabelVec, bool isLastProcess);
        
        //RefLine Labels support
        void AddRefLineLabel(const GMRefLabelInfo& irRefLabel);
        void PlaceRefLineLabels();
        
        //AxisOrigin Label support
        void AddAxisOriginLabel(const AxisOriginLabelInfo& irAxisOrigin);
        void PlaceAxisOriginLabels();
        GMDataLabelVecs mDataLabels;
    protected:
        GridMap mGridMap;
        Rect2D mDataAreaFrame;
        ChartContext::Ptr mChartContextPtr;
        GraphObjectManager* mpManager;

        //Occupied Object
        std::vector<GraphObject::Ptr > mSinglePointObjects;
        std::vector<GraphObject::Ptr > mPolygonObjects;
        std::vector<GraphObject::Ptr > mPolyLineObjects;
        std::vector<Rect2D> mRectangleObject;
        std::vector<Circle> mCirclePieObject;
        
        //Init Grid Map by Occupied Objects
        void hInitGridMapOccupationByDataObjects();
        
        EnumDssGraphMatrixDataLabelPosition hGetCorrectLocationByOrder(int iOrder, GMDataLabelInfo irDataLabel);
        EnumDssGraphMatrixDataLabelPosition hGetCorrectLocationByOrderForCombo(int iOrder, GMDataLabelInfo irDataLabel);
        void hPutDataLabelsInLocation(GMDataLabelInfo &iorDataLabel, EnumDssGraphMatrixDataLabelPosition iLocation);
        
        
        
        //RefLine Labels support
        void hGetRefLabelPosition(TextObject::Ptr iTextLable, Int32 iRefLableIndex, Int32 iRefLinePos, bool iIsVertical, Int32 &iorXOffset, Int32 &iorYOffset, Rect2D &iorRect);
        void hCheckRefLabelCandiate(refLableCandidate& iorRefLableCandidate, Int32 iCurIndex);
        GMRefLabelVecs mRefLineLabels;
        
        //AxisOrigin Labels support
        AxisOriginLabelVecs mAxisOriginLables;
        bool hGetAxisOriginLabelPosition(Point2D iRefPos ,Int32 iIndex,Rect2D& orRect, bool iIsAtriMetri);
    };
}




#endif
