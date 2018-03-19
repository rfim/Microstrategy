//
//  GridMap.h
//  IPhoneChart
//
//  Created by Li, En on 1/4/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef IPhoneChart_GridMap_h
#define IPhoneChart_GridMap_h


#include "Point2D.h"
#include "Rect2D.h"
#include "PolygonObject.h"
#include "PolyLineObject.h"
#include "RectangleObject.h"
#include "FormatFont.h"

namespace MsiChart {
    
    struct Circle
    {
        Circle() : mCenter(Point2D(0,0)), mRadius(0)
        {}
        
        Circle(Point2D iCenter, Int32 iRadius) : mCenter(iCenter), mRadius(iRadius)
        {}
    
        Point2D mCenter;
        Int32 mRadius;
    };
    
    typedef std::pair<Int32, Int32> OccupyIndex;    //0: Host marker, 1: Data label
    typedef std::vector<std::vector<Int32> > GridMatrix;
    typedef std::vector<std::vector<std::vector<OccupyIndex> > > HMDLMatrix;
    typedef std::vector<std::vector<std::vector<Rect2D> > > RectMatrix;
    typedef std::vector<std::vector<std::vector<Circle> > > CircleMatrix;
    struct GridMap
    {
        bool mIsValid;
        Int32 mMetric;
        Int32 mXUnit;
        Int32 mYUnit;
        Int32 mXCount;
        Int32 mYCount;
        Point2D mOrigin;
        
        GridMatrix mMatrix;
        HMDLMatrix mHMDLMatrix;
        RectMatrix mRectMatrix;
        CircleMatrix mCircleMatrix;
        
        GridMap();
        GridMap(const Rect2D& irRect, const Rect2D& irUnit);
        ~GridMap();
        Int32 ToMetric(); //to Get the metric
        bool UpdataMatrix(const Point2D irCenter, const Int32 irRadius, bool iIsAdd);  //Circle
        bool UpdateMatrix(const Rect2D& irOccupy, bool iIsAdd = true, EnumDSSGraphFontRotation iRotation = DssGraphFontRotateNormal);
        bool UpdateMatrix(bool iIsDataLabel, const Rect2D& irOccupy);  //Rect, label or data marker bound
        bool UpdateMatrix(const PolygonObject* irOccupy, bool iIsAdd = true); //Polygon
        bool UpdateMatrix(const PolyLineObject* irOccupy, bool iIsAdd = true); //Polyline
        bool UpdateMatrix(const Point2D& irSegmentStart, const Point2D& irSegmentEnd, bool iIsAdd = true);  //segment
        bool RestoreMatrix(const Rect2D& irOccupy);
        bool IsAccomdatable(const Rect2D& irHostMarker, const Rect2D& irOccupy, Point2D& orPosition, EnumDSSGraphFontRotation iRotation = DssGraphFontRotateNormal);
        bool IsAccomdatable(const Rect2D& irHostMarker, const Rect2D& irOccupy);
        bool IsAccomdatable(const Rect2D& irOccupy, const EnumDssGraphMatrixDataLabelPosition& irLocation);
        bool IsAccomdatable(const Rect2D& irOccupy);
        bool IsAccomdatable(const Rect2D& irOccupy, bool iIsExceptDataLabel);
        bool IsAccomdatable(const Point2D& irCenter, const Int32 irRadius, const Rect2D& irOccupy);
        bool IsAccomdatableConsiderAccurateDistanceInThePadding(const Point2D& irHostCenter, const Int32 irHostRadius, const Rect2D &irOccupy);
        bool IsAccomdatableConsiderAccurateDistanceInThePadding(const Rect2D& irOccupy);
        bool IsFitPddingWithBorder(const Rect2D& irOccupy, const Rect2D& irFrame);
        bool IsTwoObjectFitPadding(const Point2D& irCenter, const Int32& irRadius, const Rect2D& irDLRect, const Int32 &irPadding);
        bool IsTwoRectFitPadding(const Rect2D &irDLRect, const Rect2D &irRect, const Int32 &irPadding);
        //record the occupy object index(index in data label vector)
        bool UpdataMatrixOccupy(bool iIsDataLabel, const Rect2D& irOccupy, Int32 iDataLabelIndex);
        //record the occupy object(bounding box)
        bool UpdataMatrixOccupyObject(const Rect2D& irOccupy, const Rect2D& irOccupiedObject);
        bool UpdataMatrixOccupyCircleObject(const Rect2D& irOccupy, const Point2D& irCenter, const Int32 irRadius);
        
        // GM Bubble Partial Rendering Support, Overlap detection
        bool IsAccomdatableWithOverlapNumber(Int32 iOverlapNumber, const Int32& iMarkerShape, const Point2D& irMarkerCenter, const Int32& irMarkerSize);
        
        std::vector<PolyLineObject::Ptr> ToDraw(GraphObjectManager* ipManager);
        
    private:
        
        bool hIsOccupiedByHostMarker(const Rect2D& irHostMarker, int irX,int irY);
        bool hIsOccupiedByHostMarker(const Point2D irCenter, const Int32 irRadius, int irX,int irY);
        
        Point2D hGetApproximateCenterFromTopLeft(Point2D& irTopLeftPoint, Int32 iXOffset, Int32 iYOffset);
        
        Int32 hCountOverlapsInRect(Int32 iStartX, Int32 iStartY, Int32 iXCover, Int32 iYCover, Point2D& orPoint, Int32 iLimit = 1);
        bool hTreatTiltOccupation(const Rect2D& irOccupy, bool iIsAdd = true, bool iIs45 = true);
        
        //@Return false if the point to be mapped falls out of the Grid Map.
        bool hPointToIndex(const Point2D& irPoint, Int32& orX, Int32& orY, bool iIsUseFloorValue = true);
        
        //@Return false if the target Rect can not be enclosed by the Grid Map
        bool hGetRectPositon(const Rect2D& irPoint, Int32& orX, Int32& orY, Int32& orXCover, Int32& orYCover);
        //void hGetApproximateCenter(Point2D& orPoint, const Point2D& irVertex, Int32
        bool hSearchInAreas(Point2D& orPoint, const Point2D& irMarkerCenter, Int32 iOriginX, Int32 iOriginY, Int32 iWidth, Int32 iHeight);
        bool hSearchAlongAxes(Point2D& orPoint, const Point2D& irMarkerCenter, const Point2D& irOriginIndex,
                              const Point2D& irXYOffset, const Point2D irRectCover, bool& iorAlreadyHasOnePlacement, /*double& iorPreDistance,*/ bool iIsYFirst = true);
        
        void hIndexToPoint(Int32 iX, Int32 iY, Point2D& iorPoint2D);
        
        //judge whether two segments intersected
        bool hSegmentsIntersection(const Point2D& irPoint1, const Point2D& irPoint2, const Point2D& irPoint3, const Point2D& irPoint4);
        int hDirection(const Point2D& irPoint1, const Point2D& irPoint2, const Point2D& irPoint3);
        bool hOnSegment(const Point2D& irPoint1, const Point2D& irPoint2, const Point2D& irPoint3);
    };
    
}
#endif
