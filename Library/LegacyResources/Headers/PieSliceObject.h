//=====================================================================================
//	Filename:		Kernel/SourceCode/ChartEngine/PrivateSource/PieSliceObject.h
//	Date:			2008-05-22
//	Authors:		Bing Huang

//	Copyright (C) 1997-2008 MicroStrategy Incorporated
//	All rights reserved
//=====================================================================================

#ifndef MsiChart_PieSliceObject_h
#define MsiChart_PieSliceObject_h

#include "ShapeObject.h"

namespace MsiChart
{
	//related structs and enums
	typedef enum
	{
		BRT_TOP,
		BRT_BOTTOM,
		BRT_CYLINDER,
		BRT_RING_CYLINDER,
		BRT_START_RECT,
		BRT_END_RECT,
	}BoundingRectType;
	
	typedef enum
	{
		RA_START,
		RA_END
	}RectangleAngle;

	struct Angle
	{
		Angle(Int32 iStartAngle = 0, Int32 iEndAngle = 0)
			: mStartAngle(iStartAngle), mEndAngle(iEndAngle)
		{}

		Int32 mStartAngle;
		Int32 mEndAngle;
	};

	struct SliceInfo
	{
		Int32	mStartAngle;
		Int32	mEndAngle;
		float	mRingPieFactor;
		bool	mIsRingPie;
		bool	mIs2DCircular;
		double	mScale;

		Point2D mCenter;
		float	mRadius;
		Int32	mMaxRadius;//no proportional
		Int32	mDepth;

		SliceInfo(Int32 iStartAngle, Int32 iEndAngle, float iRingPieFactor, 
			bool iIsRingPie, bool iIs2DCircular, double iScale, const Point2D& irCenter,
			float iRadius, Int32 iMaxRadius, Int32 iDepth, double iPercentContribution = 0)
			: mStartAngle(iStartAngle),
			mEndAngle(iEndAngle),
			mRingPieFactor(iRingPieFactor),
			mIsRingPie(iIsRingPie),
			mIs2DCircular(iIs2DCircular),
			mScale(iScale),
			mCenter(irCenter), 
			mRadius(iRadius),
			mMaxRadius(iMaxRadius),
			mDepth(iDepth)
		{}
	};

	class PieSliceObject : public ShapeObject
	{
	public:
		typedef MSynch::SmartPtr<PieSliceObject> Ptr;

		PieSliceObject(const TripleId& irTripleId, GraphObjectManager* ipManager, 
			Int32 iExplode, Int32 iStartAngle, Int32 iEndAngle, float iRingPieFactor, 
			bool iIsRingPie, bool iIs2DCircular, double iScale, const Point2D& irCenter,
			float iRadius, Int32 iMaxRadius, Int32 iDepth, bool iOneSeries,
			GraphCollectionObject* ipParentObject = NULL, bool iIsShown = true);
        
        //Constructor method 2 for new pie chart.
        PieSliceObject(const TripleId& irTripleId, Int32 iAngleIndex, Int32 iSliceId, GraphObjectManager* ipManager,
                       Int32 iExplode, Int32 iStartAngle, Int32 iEndAngle, float iRingPieFactor, 
                       bool iIsRingPie, bool iIs2DCircular, double iScale, const Point2D& irCenter,
                       float iRadius, Int32 iMaxRadius, Int32 iDepth, bool iOneSeries,
                       GraphCollectionObject* ipParentObject = NULL, bool iIsShown = true);
				
		

		//virtual functions from GraphObject
		virtual void Draw() const;
        virtual void DrawInnerOutLine();
		virtual void DrawOutLine();
		virtual bool PointInObject(const Point2D& irPoint);
		virtual bool ObjectInRectangle(const Rect2D& irRect);
		virtual void GenerateImageMap(ImageMap& iorImageMap);
		virtual void GetOptions(Int32 iPropertyId, CComVariant* opValue) const;
		virtual void GetBoundingRect(Rect2D& orBoundingRect) const;

		/**
		Get slice information struct
		@return The pointer to mSliceInfo member variable.
		*/
		const SliceInfo* GetSliceInfo() const;

		/**
		Set the bonding rectangle type so that in GetBoundingRect() function we can get
		the correct information.
		@param iBoundingRectType The input type. 
		*/
		void SetBoundingRectType(BoundingRectType iBoundingRectType) const;

		/**
		Set the start angle for pie slice cylinder.
		The angle will be used in hGetCylinderBoundingRect() function.
		@param iCylinderStart The input angle. 
		*/
		void SetCylinderStart(Int32 iCylinderStart) const;

		/**
		Set the end angle for pie slice cylinder.
		The angle will be used in hGetCylinderBoundingRect() function.
		@param iCylinderEnd The input angle. 
		*/
		void SetCylinderEnd(Int32 iCylinderEnd) const;

		/**
		Set the flag for drawing process.
		@param iIsOutline true indicates draw outline, false indicates draw slice. 
		*/
		void SetDrawOutline(bool iIsOutline);
        
        void SetWhitenOuterBorder(bool iIsWhiten);
        
        void SetOutLineInfo(bool iIsConsectiveOutline, bool iIsHighLightStartAngelEdge, bool iIsHighLightEndAngleEdge);

        void SetIsNeedInnerStrokeForConsectiveSlcie(bool iStroke);
        
        float GetStartAngelRadian();
        float GetEndAngelRadian();
        
		/**
		Round the two angles in the angle struct to 0 - 359 degree.
		@param iorAngle The input and output angle struct. 
		*/
		void ConvertAngle(Angle& iorAngle) const;

		/**
		This function is used to check whether in our algorithm the included angles have intersection.
		It will also return the intersection angle if exists.
		There should only be one inersection angle in our algorithm.
		All angle in iAngle1 and iAngle2 should be convert to 0 - 359 using ConvertAngle()
		@param irAngle1 The input angle 1. 
		@param irAngle2 The input angle 2.
		@param orAngle The intersected angle if existed.
		@return true if intersected, false if no intersection occured.
		*/
		bool GetIntersectionConvertedAngles(const Angle& irAngle1, const Angle& irAngle2, Angle& orAngle) const;

		/**
		Check whether two included angles intersected.
		The angles have been converted to 0 - 359.
		@param irAngle1 The input angle 1. 
		@param irAngle2 The input angle 2.
		@return true if intersected, false if no intersection occured.
		*/
		bool GetIntersectionAngle(const Angle& irAngle1, const Angle& irAngle2) const;

		/**
		Check whether two included angles intersected, if intersected, return the intersected angle.
		The angles have been converted to 0 - 359.
		@param irAngle1 The input angle 1. 
		@param irAngle2 The input angle 2.
		@param orAngle The intersected angle if existed.
		@return true if intersected, false if no intersection occured.
		*/
		bool GetIntersectionAngle(const Angle& irAngle1, const Angle& irAngle2, Angle& orAngle) const;

		/**
		Call hDrawRingPieTop() method, called by PieSliceElementObject instances.
		*/
		void ElementDrawRingPieTop();

		/**
		Call hDrawNormalPieTop() method, called by PieSliceElementObject instances.
		*/
		void ElementDrawNormalPieTop();

		/**
		Call hFillSide() method, called by PieSliceElementObject instances.
		*/
		void ElementFillSide();

		/**
		Computer the leader line start point for this slice.
		@param orPoint The start point of leader line.
		*/
		void GetLeaderLineStartPoint(Point2D& orPoint) const;

		/**
		Get the sector image map.
		@param irCenter The input center point. 
		@param iRadius The input radius. 
		@param iStartAngle The input start angle of the sector. 
		@param iEndAngle The input end angle of the sector. 
		@param iorImageMap The output image map.
		*/
		void GetSectorImageMap(const Point2D& irCenter, const Int32 iRadius, const Int32 iStartAngle, const Int32 iEndAngle, ImageMap& iorImageMap) const;

		/**
		Get the side rectangle image map.
		@param irCenter The input center point. 
		@param iRadius The input radius. 
		@param iDepth The input depth.
		@param iAngle The input angle of the side rectangle. 
		@param iorImageMap The output image map.
		*/
		void GetSideRectangleImageMap(const Point2D& irCenter, const Int32 iRadius, const Int32 iDepth, const Int32 iAngle, ImageMap& iorImageMap) const;

		/**
		Get the cylinder image map.
		@param irCenter The input center point. 
		@param iRadius The input radius.
		@param iDepth The input depth.
		@param iStartAngle The input start angle of the sector. 
		@param iEndAngle The input end angle of the sector. 
		@param iorImageMap The output image map.
		*/
		void GetCylinderImageMap(const Point2D& irCenter, const Int32 iRadius, const Int32 iDepth, const Int32 iStartAngle, const Int32 iEndAngle, ImageMap& iorImageMap) const;

		void UpdateRadius(Int32 iRadius, Int32 iMaxRadius);

		Point2D GetPointByRatio(double iRadiusRatio, bool iSmartLabels = false) const;

		Int32 GetMiddleAngleQuadrant() const;

		Int32 GetMiddleAngle() const;
        
        Int32 GetAngleIndex() const;
        
        Int32 GetSliceIndex() const;

		float GetMiddleAngleRadian() const;

	protected:
		//pure virtual functions from ShapeObject
		virtual void GetClockwisePolygon(std::vector<Point2D>& orPolygon) const; 
		virtual void FillBevel() const;
		virtual bool IsCircularShape() const;
		
		friend class PieSliceElementObject;

	private:	
		/**
		Draw the glow effect for slices when highlighted
		*/
		void hDrawGlowEffect() const;
			
		/**
		Draw the fill and line.
		*/
		void hApplyFillAndLine() const;

		/**
		Fill the side of the slice.
		*/
		void hFillSide() const;

		/**
		Draw ring pie slice.
		*/
		void hDrawRingPieSlice() const;

        void hDrawRingPieInnerStroke() const;
        
		/**
		Draw ring pie slice top sector, only handle the one seires case.
		*/
		void hDrawRingPieOneSeriesTop() const;

		/**
		Draw ring pie slice bottom sector, only handle the one seires case.
		*/
		void hDrawRingPieOneSeriesBottom() const;

		/**
		Draw ring pie middle part, including cylinder and side rectangle.
		*/
		void hDrawRingPieMiddle() const;

		/**
		Draw ring pie top sector.
		*/
		void hDrawRingPieTop() const;

		/**
		Draw ring pie bottom sector.
		*/
		void hDrawRingPieBottom() const;

		/**
		Draw ring pie side rectangle.
		@param iRectangleAngle The input anlge.
		*/
		void hDrawRingPieMiddleRectangle(RectangleAngle iRectangleAngle) const;

		/**
		Draw ring pie cylinder, only draw the part that can be seen.
		*/
		void hDrawRingPieMiddleCylinder() const;

		/**
		Draw cylinder according to the angle information.
		@param iIsRing Indicates whether it's a ring pie.
		@param iStartAngle The start angle of the cylinder.
		@param iEndAngle The end angle of the cylinder.
		*/
		void hDrawCylinder(bool iIsRing, Int32 iStartAngle, Int32 iEndAngle) const;

		/**
		Draw normal pie slice.
		*/	
		void hDrawNormalPieSlice() const;

        void hDrawNormalPieInnerStroke() const;
        
		/**
		Draw normal pie slice top sector, only handle the one seires case.
		*/
		void hDrawNormalPieOneSeriesTop() const;

		/**
		Draw normal pie slice bottom sector, only handle the one seires case.
		*/
		void hDrawNormalPieOneSeriesBottom() const;

		/**
		Draw normal pie middle part, including cylinder and side rectangle.
		*/
		void hDrawNormalPieMiddle() const;

		/**
		Draw normal pie top sector.
		*/
		void hDrawNormalPieTop() const;

		/**
		Draw normal pie bottom sector.
		*/
		void hDrawNormalPieBottom() const;

		/**
		Draw normal pie side rectangle.
		@param iRectangleAngle The input anlge.
		*/
		void hDrawNormalPieMiddleRectangle(RectangleAngle iRectangleAngle) const;

		/**
		Draw normal pie cylinder, only draw the part that can be seen.
		*/
		void hDrawNormalPieMiddleCylinder() const;

		/**
		Judge whether a point hit the ring pie slice.
		@param irPoint The input point.
		@return true if hit, false if out of slice.
		*/
		bool hPointInRingPieSlice(const Point2D& irPoint) const;

		/**
		Judge whether a point hit the normal pie slice.
		@param irPoint The input point.
		@return true if hit, false if out of slice.
		*/
		bool hPointInNormalPieSlice(const Point2D& irPoint) const;

		/**
		Check whether an included angle has an intersection with the slice angle.
		@param irAngle The input included angle.
		@return true if intersect, false if not.
		*/
		bool hGetIntersectionConvertedAngleToSliceAngle(const Angle& irAngle) const;

		/**
		Get the bounding rectangle for top or bottom sector.
		@param orBoundingRect The output rectangle.
		*/
		void hGetTopBottomBoundingRect(Rect2D& orBoundingRect) const;

		/**
		Get the bounding rectangle for cylinder.
		@param orBoundingRect The output rectangle.
		*/
		void hGetCylinderBoundingRect(Rect2D& orBoundingRect) const;

		/**
		Get the bounding rectangle for side rectangle.
		@param orBoundingRect The output rectangle.
		*/
		void hGetRectangleBoundingRect(Rect2D& orBoundingRect) const;

		void hWhitenOuterBorder() const;
        
		//SliceInfo mSliceInfo;
		SliceInfo mSliceInfo;		
		
		//used by "GetBoudingRect()"
		mutable BoundingRectType mBoundingRectType;
		mutable Int32 mCylinderStart;
		mutable Int32 mCylinderEnd;
		//deal with the case that there is only one series
		bool mOneSeries;
		mutable bool mIsOutline;

		Point2D mOriginalCenter;
		float mStartAngleRadian;
		float mEndAngleRadian;
		float mMiddleAngleRadian;

		Int32 mMiddleAngle;
		Int32 mMiddleAngleQuadrant;
        
        Int32 mAngleIndex;
        Int32 mSliceIndex;

		bool mIsShown;
        bool mIsWhitenOuterBorder;
        
        // Enhacement of Highline, for multiple slice connected, highlight the outer border as a group as web did.
        bool mIsConsecutiveHighLihgt;
        bool mIsConsecutiveSlcieNeedInnerStroke;
        bool mIsHighLightStartAngleEdge;
        bool mIsHighLightEndAngleEdge;
	};

	inline void PieSliceObject::SetCylinderStart(Int32 iCylinderStart) const
	{
		mCylinderStart = iCylinderStart;
	}

	inline void PieSliceObject::SetCylinderEnd(Int32 iCylinderEnd) const
	{
		mCylinderEnd = iCylinderEnd;
	}
	
	inline void PieSliceObject::SetBoundingRectType(BoundingRectType iBoundingRectType) const
	{
		mBoundingRectType = iBoundingRectType;
	}

	inline void PieSliceObject::SetDrawOutline(bool iIsOutline)
	{
		mIsOutline = iIsOutline;
	}

	inline const SliceInfo* PieSliceObject::GetSliceInfo() const
	{
		return &mSliceInfo;
	}

	inline Int32 PieSliceObject::GetMiddleAngleQuadrant() const
	{
		return mMiddleAngleQuadrant;
	}

	inline Int32 PieSliceObject::GetMiddleAngle() const
	{
		return mMiddleAngle;
	}

	inline float PieSliceObject::GetMiddleAngleRadian() const
	{
		return mMiddleAngleRadian;
	}
    
    inline Int32 PieSliceObject::GetAngleIndex() const
	{
		return mAngleIndex;
	}
    
    inline Int32 PieSliceObject::GetSliceIndex() const
	{
		return mSliceIndex;
	}
    
    inline void PieSliceObject::SetIsNeedInnerStrokeForConsectiveSlcie(bool iStroke)
    {
        mIsConsecutiveSlcieNeedInnerStroke = iStroke;
    }
    
    inline float PieSliceObject::GetStartAngelRadian()
    {
        return mStartAngleRadian;
    }
    
    inline float PieSliceObject::GetEndAngelRadian()
    {
        return mEndAngleRadian;
    }
    
    inline void PieSliceObject::SetOutLineInfo(bool iIsConsectiveOutline, bool iIsHighLightStartAngelEdge, bool iIsHighLightEndAngleEdge)
    {
        mIsConsecutiveHighLihgt = iIsConsectiveOutline;
        mIsHighLightStartAngleEdge = iIsHighLightStartAngelEdge;
        mIsHighLightEndAngleEdge = iIsHighLightEndAngleEdge;
    }
    
    inline void PieSliceObject::SetWhitenOuterBorder(bool iIsWhiten)
	{
		mIsWhitenOuterBorder = iIsWhiten;
	}

}

#endif
