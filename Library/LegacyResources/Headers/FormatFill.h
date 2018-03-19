//==============================================================================================
//	FILENAME	:	FormatFill.h
//	AUTHOR		:	Liang Liu, Bing Huang
//	CREATION	:	2008-09-25
//	Copyright (C) MicroStrategy Incorporated 2008
//	All Rights Reserved
//==============================================================================================
#ifndef MsiChart_FormatFill_h
#define MsiChart_FormatFill_h

#include "PDCHeader/PDCvector"
#include "Base/Base/BaseString.h"
#include "Synch/Defines/SmartPtr.h"
#include "RGBColor.h"
#include "Rect2D.h"
#include "../Common.h"
#include <map>

struct _cairo;
typedef struct _cairo cairo_t;

struct _cairo_surface;
typedef struct _cairo_surface cairo_surface_t;

struct tagVARIANT;
typedef struct tagVARIANT VARIANT;

namespace MsiChart
{
	extern const RGBColor gBlack;
	extern const RGBColor gWhite;

	/// Enumeration for radient gradient usage
	typedef enum
	{
		RG_PIESLICE,
		RG_RING,
		RG_BORDERRING
	} EnumRadientGradientUsage;

	/// Enumeration of FormatFill type.
	typedef enum
	{
		// Make the values consistent with the corresponding enum in DSSCOMMater_i.h
		FT_SIMPLE = 0,		
		FT_TEXTURE = 4,
		FT_PICTURE = 6,
		FT_GRADIENT = 14,
		FT_PATTERN = 101
	}EnumFillType;		// Need not include Bevel.


	/// Enumeration of fliping type.
	typedef enum
	{
		// FT_Texture and FT_Picture
		FF_NO_FLIPPING,
		FF_FLIP_HORIZONTAL,
		FF_FLIP_VERTICAL,		
		FF_FLIP_BOTH,

		// FT_TEXTURE ONLY
		FF_BRICKS_HORIZONTAL,
		FF_BRICKS_VERTICAL
	}EnumFillFlip;

    typedef enum{
        EnumClassMemberFillType = 0,
        EnumClassMemberFillAlpha,
        EnumClassMemberFillSimpleColor,
        EnumClassMemberFillGrandient,
        EnumClassMemberFillPattern,
        EnumClassMemberFillPicture,
        EnumClassMemberFillBevel,
        EnumClassMemberFillDarkerColor
    }EnumFormatFillMembers;

	/// keep gradient options.
	struct GraphFillGradient
	{
		GraphFillGradient()
			: mStartGraphColor(gBlack)
			, mEndGraphColor(gWhite)
			, mAngle(0)
			, mHorizontalOffset(50)
			, mVerticalOffset(50)
			, mGradientType(DssGraphGradientCircular)
			, mEndAlpha(0){}			
		RGBColor	mStartGraphColor;
		RGBColor	mEndGraphColor;
        std::vector<unsigned char> mStepAlpha;
        std::vector<RGBColor> mStepGraphColor;
        std::vector<float> mStepLocation;
		Int32	mAngle;						// 0~360
		unsigned char mHorizontalOffset;	// 0~100
		unsigned char mVerticalOffset;		// 0~100
		unsigned char mGradientType;		// EnumGradientType
		unsigned char mEndAlpha;
	};

	/// Keep pattern options.
	struct GraphFillPattern
	{
		GraphFillPattern()
			: mForegroundColor(gWhite)	
			, mBackgroundColor(gBlack)
			, mPatternId(28){}			
		RGBColor	mForegroundColor;
		RGBColor	mBackgroundColor;
		unsigned char mPatternId;				
	};

	/// Keep texture options.
	/*struct GraphFillTexture
	{
		GraphFillTexture()
		: mFillFlip(FF_NO_FLIPPING){}
		unsigned char mFillFlip;	// EnumFillFlip					
		MBase::String mTextureFile;									
	};*/

	/// Keep picture options.
	struct GraphFillPicture
	{
		GraphFillPicture()
		: mFillFlip(FF_NO_FLIPPING)
		, mScaleType(DssGraphPictureScaleNone)
		, mPictureFormat(DssGraphPicturePNG)
		, mNameLength(0){}
		unsigned char mFillFlip;	// EnumFillFlip		
		unsigned char mScaleType;
		unsigned char mPictureFormat;
		unsigned char mNameLength;
		MBase::StrongArrayPtr<char> mPictureFilePtr;		
		TripleId mTripleId;
	};

	/// Keep bevel options.
	struct GraphFillBevel
	{
		GraphFillBevel()
			: mFillBevelType(DssGraphFillBevelNone)
			, mDepth(0)
			, mLightAngle(270)
			, mIntensity(10) {}
		unsigned char mFillBevelType;
		unsigned char mDepth;
		Int32 mLightAngle;
		unsigned char mIntensity;
	};

	class ChartContext;

	/**
	This class manages all the fill format info of a ShapeObject.
	There are six fill types: Simple, Gradient, Texture, Picture, Pattern and Bevel.
	There are four subtypes of Bevel: Smooth edge, Chisel edge, Donut and Bevel.	
	See http://was-ryuan-23/wiki/images/9/9f/Graph_Format_Fill.doc for reference.
	*/
	class FormatFill
	{	
	public:
		typedef MSynch::SmartPtr<FormatFill> Ptr;
		/// Constructor.
		FormatFill(const RGBColor& irSimpleColor = gWhite, unsigned char iAlpha = 255);

		/**
		Get options which are related to fill format.
		@param iPropertyId Specifies a property ID which is related to fill format.
		@param opValue Stores the output value of the specified property.
		@return True means the output value is valid while false means the opposite.
		*/
		bool GetOptions(Int32 iPropertyId, CComVariant* opValue) const;		

		/**
		Make current FormatFill darker. 
		This method is used in pseudo 3D effect. mDarkerColor will be true after this method is called.
		@param iFactor Should be a value in [0, 1]. 
		Each color component in current FormatFill will be multiplied by iFactor.
		*/
		void MakeDarker(float iFactor);
		/**
		This is a reversed method of MakeDarker(). This method will restore current color from darker color to original color.
		mDarkerColor will be false after this method is called.
		*/
		void RestoreColor();		
		/**
		This method fills the current path with a solid color.
		@param ipChartContext Pointer to ChartContext.
		*/
		void FillSimple(ChartContext* ipChartContext) const;	
		/**
		This method fill the current path with gradient effect.
		@param ipChartContext Pointer to ChartContext.
		@param irBoundingRect Indicates the bounding rectangle of current path.
		*/
		void FillGradient(ChartContext* ipChartContext, const Rect2D& irBoundingRect) const;		
		/**
		This method fill the current path with pattern effect.
		@param ipChartContext Pointer to ChartContext.
		*/
		void FillPattern(ChartContext* ipChartContext) const;	
		/**
		 This method deals with smooth edge and chisel edge type of bevel.
		 @param ipChartContext Pointer to ChartContext.
		 @param irPolygon Indicates a clockwise polygon corresponding to current path.		 
		 */
		void FillEdgeType(ChartContext* ipChartContext, const std::vector<Point2D>& irPolygon) const;
		/**
		This method deals with donut and sphere type of bevel.
		@param ipChartContext Pointer to ChartContext.
		@param irBoundingRect Indicates the bounding rectangle of current path.
		@param iIsCircularShape Indicates whether current object is a circular ShapeObject or not.
		*/
		void FillDonutOrSphere(ChartContext* ipChartContext, const Rect2D& irBoundingRect, bool iIsCircularShape) const;
		
		/**
		This is a helper method for ChartContext::hApplyEnhancedSeriesEffect().
		@param iTransparency New transparency.
		@param iBevelType New bevel type.
		@param iDepth New bevel depth.
		@param iBevelAngle New bevel angle.
		@param iIntensity New Bevel intensity.
		*/
		void EnhancedSeriesEffect(Int32 iTransparency, Int32 iBevelType, Int32 iDepth, Int32 iBevelAngle, Int32 iIntensity);
        //lizhang, de41340, 20160908, add this method to check whether two formatfill is equal, specially for legend marker with threshold color
        bool IsFormatFillEqual(FormatFill::Ptr iComparedFormat);
        inline void SetResolvedInDataSet(const bool iFlag ,const EnumFormatFillMembers iMember);
        inline bool IsResolvedInDataSet(const EnumFormatFillMembers iMember) const;
#ifndef _VE_CE
		/**
		 Apply radial gradient effect on the input surface.
		 This method is useful for the combining effect of bevel sphere and picture/texture/gradient/pattern.
		 */
		void ApplyRadialGradientForSurface(CGContextRef iCGContextRef) const;
#endif
		
	//private:		
		static float hRatioInRectangle(const Rect2D& irRect, Int32 iAngle, const FPoint2D& irAnchorPoint, FPoint2D& orStartPoint, FPoint2D& orEndPoint);

	private:
		/**
		 This is a helper method for Bevel effect. 		
		 Get the maximum reasonable depth of a polygon specified by irPolygon.
		 Here, "maximum reasonable depth" means when shrinking the input polygon using this depth, at least one edge
		 or irPolygon becomes a point. When shrinking the input polygon using a depth less than "maximum reasonable depth",
		 no edge will become a point.
		 This function is called by FillEdgeType()	
		 @param irPolygon Specifies a polygon.
		 @param orMaxDepth The maximum reasonable depth for irPolygon.
		 @return Whether the depth is valid or not.
		 */
		bool hGetAreaDepth(const std::vector<FPoint2D>& irPolygon, float& orMaxDepth) const;
		
		/**
		 This is a helper method for hGetAreaDepth().
		 This function deal with the case that when computing depth of an area, the current angle is greater than 180 degree.
		 @param irPolygon Specifies a concave polgyon.
		 @param iCurrent Specifies a vetex in irPolygon which has an angle greater than 180 degree.
		 @param orPolygon Output a new polygon which has less concave angles than irPolygon.
		 */
		void hGetNewPolygon(const std::vector<FPoint2D>& irPolygon, Int32 iCurrent, std::vector<FPoint2D>& orPolygon) const;		
		/**
		 This is a helper method for hGetAreaDepth().
		 This function deal with the case that when computing depth of an area, the current angle is greater than 180 degree.
		 @param irPolygon Specifies a polygon got from hGetNewPolygon().
		 @param orMaxDepth The helper depth for irPolygon.
		 @return Whether the depth is valid or not.
		 Here, "helper depth" means a intermidiate depth which is useful for hGetAreaDepth().
		 */
		bool hGetNewAreaDepth(const std::vector<FPoint2D>& irPolygon, float& orMaxDepth) const;
		
		/**
		 This is a helper method for FillEdgeType().
		 Given the depth and the angle, get the point on the angular bisector and the point in the outbound of the polygon.
		 @param irA,irB,irC Specifies an angle ABC.
		 @param irDetph Specifies a depth.
		 @param orPointStart A point on the bisector of angle ABC. The distance between orPointStart and line BC is irDepth.
		 @param orPointEnd The projection of orPointStart on line BC.
		 */
		void hGetAngularBisectorIntersectionPoint(const FPoint2D& irA, const FPoint2D& irB, const FPoint2D& irC, float irDepth, FPoint2D& orPointStart, FPoint2D& orPointEnd) const;
		
		/** Check whether a point is in the range of two angles' bisectors.
		 @param irPoint Specifies a point.
		 @param irA,irB,irC,irD Specifis a polyline ABCD.
		 @return Suppose E is the intersection of the bisectors of angle ABC and angle BCD. 
		 Return true if irPoint is inside triangle BCE. Otherwise, return false.
		 */
		bool hPointInRange(const FPoint2D& irPoint, const FPoint2D& irA, const FPoint2D& irB, const FPoint2D& irC, const FPoint2D& irD) const;
		
		/**
		 Get the intersection of the bisectors of angle ABC and angle BCD. Return the distance between the intersecton and line BC.
		 @param irA,irB,irC,irD Specifies a polyline. Should be in clockwise order. 
		 Both of ABC and BCD should be less than or equal to PI. 
		 @param orBisectorPoint Output the intersection of the bisectors of angle ABC and angle BCD.
		 @param orDistance the distance between the intersecton and line BC
		 @return If A,B,C,D are in the same line we return false, else true.
		 */
		bool hGetBisectorPointAndDepth(const FPoint2D& irA, const FPoint2D& irB, const FPoint2D& irC, const FPoint2D& irD, FPoint2D& orBisectorPoint, float& orDistance) const;
		
		/**
		 Get the twice of a triangle area.		
		 @param irA,irB,irC These three points specifies a triangle.
		 @return Return 2*Area(ABC) if A, B, C are in clockwise order
		 Return -2*Area(ABC) if A, B, C are in counterclockwise order
		 */
		float hGetTriangleAreaDouble(const FPoint2D& irA, const FPoint2D& irB, const FPoint2D& irC) const;
			
		/**
		 Check whether two float points are equal.
		 @param irA,irB Two points to be checked.
		 @return true if euqal.
		 */
		bool hIsFPointsEqual(const FPoint2D& irA, const FPoint2D& irB) const;
		
	// data members
	public:
		unsigned char mFillType;		///< Fill type should be a value in EnumFillType.
		unsigned char mAlpha;	///< Alpha should be an integer in range 0~255.
		RGBColor mSimpleColor;			///< This color is used when mFillType is FT_SIMPLE.
		GraphFillGradient	mFillGradient;	///< Stores gradient options which is used when mFillType is FT_GRADIENT.
		GraphFillPattern	mFillPattern;	///< Stores pattern options which is used when mFillType is FT_PATTERN.
		//GraphFillTexture	mFillTexture;	///< Stores texture options which is used when mFillType is FT_TEXTURE.
		GraphFillPicture	mFillPicture;	///< Stores picture options which is used when mFillType is FT_PICTURE.
		GraphFillBevel	mFillBevel;		///< Stores gradient options which is is used for Bevel
		bool mDarkerColor;

		Int32               mClassMemberMap;
	};	
    inline void FormatFill::SetResolvedInDataSet(const bool iFlag ,const EnumFormatFillMembers iMember)
    {
        mClassMemberMap ^= iFlag << iMember;
    }
    inline bool FormatFill::IsResolvedInDataSet(const EnumFormatFillMembers iMember) const
    {
        return (mClassMemberMap & 1 << iMember) != 0;
    }
}
#endif
