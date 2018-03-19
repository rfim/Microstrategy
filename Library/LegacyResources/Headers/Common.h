//========================================================
//	Filename:		Kernel/SourceCode/IPhoneChart/Common.h
//	Date:			2008-05-20
//	Authors:		Bing Huang

//	Copyright (C) 1997-2008 MicroStrategy Incorporated
//	All rights reserved
//========================================================

#ifndef MsiChart_Common_h
#define MsiChart_Common_h

#include <vector>

#include "Base/Defines/BasicTypes.h"

// #include "Chart/Chart/ImageMap.h"

/////////////////GridChartSupport///////////////
const int DssGraphTypeGridChart = 20000;
const int DssGraphRangeLegendFrame = 10001;

#ifdef _VE_CE
typedef char JSONChar;
#define JSONString MBase::String;
#define JSONscanf sscanf
#define ToDecimalJSONString ToDecimalString
#define JSONStringToUTF8(x) x
#define ToJSONString(x) x
#define ToWideString(x) x
#endif

//Bayer hiChart, defined for CE internal use.
typedef enum
{
    DssGraphTypeBudgetLipStick_Veritical = 207, //DssGraphTypeVerticalHichertLipStick,
    DssGraphTypeBudgetLipStick_Horizontal,// = DssGraphTypeHorizontalHichertLipStick,
    DssGraphTypeBudgetOverlapBar_Veritical,// = DssGraphTypeVerticalHichertOverlappingBars,
    DssGraphTypeBudgetOverlapBar_Horizontal,// = DssGraphTypeHorizontalHichertOverlappingBars,
}DssGraphTypeHiChecrt;
const int DssPlotWrapper = 30000;
const int DssIdSpan = 800;
// TQMS: 635507, by zshe
typedef enum
{
	DssGraphNumberFormatAsDefaultAxis,
	DssGraphNumberFormatAsY1Axis,
	DssGraphNumberFormatAsY2Axis,
	DssGraphNumberFormatAsZAxis
} EnumDssGraphNumberFormatAsAxis;

#ifndef _VE_CE
// The following code comes from a later version of GraphStyle.idl. Will be removed in the next rebase.	
// typedef enum
// {
// 	 DssGraphOffScaleHide	= 0,
//	 DssGraphOffScaleClip	= 1,
//	 DssGraphOffScaleShow	= 2,
// } EnumDSSGraphOffScaleType;
#endif
typedef enum
{
	UCM_BYX = 0,
	UCM_BYY = 1,
	UCM_BYBOTH = 2
}EnumDssGraphGridChartUniformColorMode;

typedef enum
{
	GMCM_BYX = 0,
	GMCM_BYY = 1,
	GMCM_BYBOTH = 2,
	GMCM_SPECIFIC = 3,	//< color by a specific metric
	GMCM_COLORBY = 4,	//< combination of color-by unit
}EnumDssGraphMatrixColorMode;

typedef enum
{
	GMSM_AUTOFIT = 0,
	GMSM_PROPORTIONAL = 1,
}EnumDssGraphMatrixSizeMode;

typedef enum
{
	GMESM_AUTOMATIC = 0,
	GMESM_PROPORTIONAL = 1,
	GMESM_MANUAL = 2,
}EnumDssGraphMatrixElementSizeMode;

typedef enum
{
	DssGraphGridFitToGraph = 100,
}EnumDssGraphMatrixGridStyle;

typedef enum
{
    GAO_CENTER_EXPAND = 0,
    GAO_LOW_HIGH,  
    GAO_HIGH_LOW,
}EnumDssGraphAnimationOrientation;

typedef enum
{
	GMNCDM_ALL = 0,		//< all the value under all the metric will be condensed
	GMNCDM_PART,
	GMNCDM_NONE,
}EnumDSSGraphMatrixNumberCondenseMode;

typedef enum
{
	GMEPP_TOP = 0,
	GMEPP_BOTTOM,
	GMEPP_LEFT,
	GMEPP_RITHT, 
}EnumDssGraphMatrixExtraPaddingPosition;

typedef enum
{
	GMMC_UNDEF = 0,
	GMMC_DATA_RENDER = 1,
	GMMC_SIZE_RENDER = 2,
	GMMC_COLOR_RENDER = 4,
}EnumDssGraphMatrixMetricCategory;

typedef enum
{
    GMDLP_BOTTOM = 0,
    GMDLP_TOP,
    GMDLP_LEFT,
    GMDLP_RIGHT,
    GMDLP_CENTER,
}EnumDssGraphMatrixDataLabelPosition;

typedef enum
{
    DssGraphMarkerShapeGMTick = 200,
    DssGraphMarkerShape2DPie,
    DssGraphMarkerShape2DRingPie,
    DssGraphMarkerShapeCombinaed,
}EnumDssGraphMatrixExtraMarkerShape;

typedef enum {
    DssTimeSeriesNone = 0,
    DssTimeSeriesLine = 0x0001,
    DssTimeSeriesAxis = 0x0002,
    DssTimeSeriesMarker = 0x0004,
    DssTimeSeriesAll = 0x0007,
} DssTimeSeriesItems;

// clark, move EnumDSSExtraGraphProperty to DSSiphoneenums.h

typedef enum 
{	
	// Interlaced grids
	DssGraphO1Interlaced = 20000,
	DssGraphO2Interlaced,
	DssGraphX1Interlaced,
	DssGraphY1Interlaced,
	DssGraphY2Interlaced,
	DssGraphY3Interlaced,
	DssGraphY4Interlaced,
	DssGraphZInterlaced,

	// Quality Bands
	DssGraphQualityBand,
	DssGraphQualityBandTitle,
	
	// iPad charting start from 21000 	
	// Support second user line for Graph Matrix.
	DssGraphY1SecondUserLine = 21000,	
	
	// Warning message for non-supported chart type.
	DssGraphWarningMessage = 21001,
	
	// 412906, bracket line of O1 axis.
	DssGraphO1BracketLine = 21002,

	// the highlight layerobject triple id
	DssGraphHighlightLayer = 21003,
	
	// Base layer
	DssGraphBaseLayer = 21004,
	
	// Middle wall layer in BiPolar chart with depth.
	DssGraphMiddleWallLayer = 21005,
	
	// Plot layer
	DssGraphPlotLayer = 21006,	
	
	// Renderer layer
	DssGraphRendererLayer = 21007,

    // Graph container
    DssGraphContainer = 21500,
    
	//GraphMatrix widget start from 22000
	DssGraphMatrixRowSliceUnit = 22000,
	DssGraphMatrixColSliceUnit,
	DssGraphMatrixXDropZone, 
	DssGraphMatrixYDropZone,
	DssGraphMatrixBubbleSizeMetrix,
    DssGraphMatrixUniformXAxis,
    DssGraphMatrixUniformYAxis,
    DssGraphMatrixUniformX1Axis,
    DssGraphMatrixUniformY1Axis,
    DssGraphMatrixTickObject,  //< For tick mode.
    
    
    
}EnumDSSExtraGraphObject;

typedef enum
{
	DssGraphTypeGridReserved = 0,
	DssGraphTypeGrid = 1,
	DssGraphTypeGraphMatrix,
}EnumDSSGraphGridType;

namespace MsiChart
{
	const Int32 gVirtualCoordinateWidth = 700;
	const Int32 gVirtualCoordinateHeight = 400;
	const Int32 gNullObjectId = -3;
	const Int32 gNullSeriesId = -3;
	const Int32 gNullGroupId = -3;
    const Int32 gNullAngleId = -3;
    const Int32 gNullSlcieId = -3;
	const Int32 gMaxSeries = 20;
	
	enum EnumGraphMajorType
	{
		GMT_INVALID = 0,
		GMT_CATEGORY,
		GMT_PIE,
		GMT_SCATTER,
		GMT_POLAR,
		GMT_RADAR,
		GMT_BUBBLE,
		GMT_STOCK,
		GMT_HISTOGRAM,
		GMT_GAUGE,
		GMT_FUNNEL,
		GMT_BOX,
		GMT_GANTT,
		GMT_SPECTRAL,
		GMT_TABLE,
		GMT_3D_CATEGORY,
		GMT_3D_SCATTER,
		GMT_GRID		//GridChart used in GraphMatrix
	};

	typedef enum
	{	
		// Make this list consistent with EnumFileType in ChartCtrl.h.
		PFT_JPEG, 
		PFT_PNG, 
		PFT_BMP = 7,
		PFT_GIF = 100, 		
		PFT_PDF = 102,
		PFT_SVG,
		PFT_WIN32
	} EnumPictureFormatType;

	// reference: pgsdk.h
	// This emumeration is not continuous since we need to maintain compatibility 
	// with web GUI code. 
	typedef enum  
	{
		OS_ELLIPSE = 2,
		OS_LINE = 5,
		OS_POLYGON,
		OS_RECTANGLE,	
		OS_WEDGE = 12,
		OS_WEDGECRUST,
		OS_POLYLINE = 18,	
		OS_PATH  =24,
	} EnumObjectShape;

	enum GraphInfoProperty {
		GIPBinaryDefinition = 1,
		GIPResizingMode = 2,
		GIPMaxSeries = 3,
		GIPMaxCategories = 4,
		GIPSaveDimensionsMode = 5,
		GIPIsChartAvailable = 6,
		GIPGraphTypesAvailable = 7,
		GIPHeight = 8,
		GIPWidth = 9,
		GIPGraphType = 10,
		GIPSeriesByCols = 11,
		GIPSuppressLastLevel = 14,
		GIPGraphFormat = 15,
		GIPIncludeSubtotal = 17,
		GIPDepthEffect = 23,
		GIPChartBackColor = 24,
		GIPTypesApplyTo = 25,
		GIPRecalculateChartFrame = 26,
		GIPRecalculateLegend = 27,
		GIPConcatenateLables = 32,
		GIPAutosizeHorizontal = 33,
		GIPAutosizeVertical = 34,
		GIPUseDefaultFont	= 35,
		GIPDefaultFontName	= 36,
		GIPDefaultFontCharset	= 37,
		GIPNestedLabel = 38,
		GIPNestedLabelBracket = 39,
		GIPManualLayoutMode = 40,
		GIPEnableTooltips = 41,
		GIPUseCurvedLines = 42,
		GIPUseFontAntiAliasing = 43,
		GIPEnhancedSeriesEffect = 44,
		GIPChartEngine = 46
	};
	
	//This enum is used in time series chart to distinguish different usage of image output. Different items can be combined if necessary.
	typedef enum
	{
		IU_GENERAL = 0,						///< General image usage.
		IU_FIRST_IMAGE_OF_SCROLL_VIEW = 1,	///< Use current image as the fisrt image of scroll view.
		IU_SECOND_IMAGE_OF_SCROLL_VIEW = 2,	///< Use current image as the second image of scroll view.
		IU_IMAGE_OF_SCROLL_VIEW = 4,		///< Use current image as a piece of scroll view.
		IU_EMPTY_IMAGE = 8,					///< Use current image as an empty image for elastic effect.
	}EnumImageUsage;  //Added by lliu 
#ifdef _VE_CE
	typedef enum 
	{
		LJT_MITER = 0,
		LJT_BEVEL = 1,
		LJT_ROUND = 2,
	}EnumLineJoinType;
#endif
	/// Helper enum for gradient properties.
	typedef enum
	{
		GType,
		GStartColor,
		GEndColor,
		GAngle,
		GHOffset,
		GVOffset,
		GLastProp
	}EnumGradientProps;
	
	/// Helper enum for bevel properties.
	typedef enum
	{
		BType,
		BAngle,
		BIntensity,
		BDepth,
		BLastProp
	}EnumBevelProps;
	
	typedef enum
	{
		BoxLeft,
		BoxTop,
		BoxRight,
		BoxBottom
	}EnumBox;
    
    //Struct InfoId is added for the object needs more infoindex than TripleId.
    const Int32 EnumInfoIndexNumber = 2;
    typedef enum
    {
        EnumGraphMatrixPieAngleIndex = 0,
        EnumGraphMatrixPieSliceIndex = 1
    }EnumPieSliceIndex;
    
	
    struct InfoId   //For some objects, such as Pie slice, TripleID is not enough to indentify a object, need more info, such as AngleId, SliceId.
    {
        InfoId(const std::vector<int>& iInfoIndexSet, Int32 iInfoIndexNum)
        {
            mInfoNumber = iInfoIndexNum;
            mInfoIndexSet.resize(iInfoIndexNum);
            mInfoIndexSet.assign(iInfoIndexSet.begin(), iInfoIndexSet.end());
        }
        
        InfoId()
        {
            mInfoNumber = EnumInfoIndexNumber;  //Note, when we add other enumtype we add to modify the EnumInfoIndexNumber, currently it's 2(AngleIndex, SliceIndex)
            mInfoIndexSet.resize(mInfoNumber, -3);
        }
        
        std::vector<int> mInfoIndexSet;
        Int32 mInfoNumber;
    };
    
	struct TripleId : public InfoId
	{
        
		TripleId(Int32 iObjectId = gNullObjectId, Int32 iSeriesId = gNullSeriesId, Int32 iGroupId = gNullGroupId)
        :InfoId(),mObjectId(iObjectId), mSeriesId(iSeriesId), mGroupId(iGroupId){}

		TripleId(const TripleId& irTripleId)
			:InfoId(irTripleId.mInfoIndexSet, irTripleId.mInfoNumber), mObjectId(irTripleId.mObjectId), mSeriesId(irTripleId.mSeriesId), mGroupId(irTripleId.mGroupId){}

		Int32 mObjectId;
		Int32 mSeriesId; 
		Int32 mGroupId;
		bool operator < (const TripleId& iTripleId) const
		{
			if (mObjectId < iTripleId.mObjectId)
			{
				return true;
			}
			else if (mObjectId == iTripleId.mObjectId)
			{
				if (mSeriesId < iTripleId.mSeriesId)
				{
					return true;
				}
				else if (mSeriesId == iTripleId.mSeriesId)
				{
                    if (mGroupId  < iTripleId.mGroupId)
                    {
                        return true;
                    }
                    else if (mGroupId == iTripleId.mGroupId)
                    {
                        for (int i = 0; i < iTripleId.mInfoNumber; i ++)
                        {
                            if (mInfoIndexSet[i] == iTripleId.mInfoIndexSet[i])
                                continue;
                            return mInfoIndexSet[i] < iTripleId.mInfoIndexSet[i];
                        }
                        return false;
                    }
                    else
                        return false;
				}
                else
                    return false;
			}
			else
				return false;
		}
                            
		bool operator == (const TripleId& iTripleId) const
		{
            bool lIsTripleIdEquals = (mObjectId == iTripleId.mObjectId && mSeriesId == iTripleId.mSeriesId && mGroupId == iTripleId.mGroupId);
            if (!lIsTripleIdEquals)
                return false;
			for (int i = 0; i < iTripleId.mInfoNumber; i ++)
            {
                if (mInfoIndexSet[i] == iTripleId.mInfoIndexSet[i])
                    continue;
                else
                    return false;
            }
            
            return true;
		}
	};
}


#endif
