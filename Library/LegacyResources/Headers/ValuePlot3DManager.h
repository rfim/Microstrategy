//==============================================================================================
//	FILENAME	:	ValuePlot3DManager.h
//	AUTHOR		:	Dong Shi
//	CREATION	:	2009-06-04
//	Copyright (C) MicroStrategy Incorporated 2008
//	All Rights Reserved
//==============================================================================================
#ifndef MsiChart_ValuePlot3DManager_h
#define MsiChart_ValuePlot3DManager_h

#include "PlotManager.h"
#include "ValueAxis.h"
#include "Transformer.h"
#include "WallManager.h"

namespace MsiChart
{
	typedef enum
	{
		TDS_VAT_Z = 0,
		TDS_VAT_X,
		TDS_VAT_Y
	}TDS_VATYPE;

	typedef enum{
		TDS_3D_BALL,
		TDS_3D_RECTANGE,
		TDS_3D_SQUARE,
		TDS_3D_TRIANGLE,
		TDS_3D_DIAMOND,
		TDS_3D_FATPLUS,
		TDS_3D_THINPLUS,
		TDS_3D_SPIDEX,
		TDS_3D_STAR,
		TDS_3D_CYLINDER
	}TDS_3DMAKERSHAPE;

	struct MarkerInfo
	{
		Int32 mSeriesId;
		Int32 mGroupId;
		DPoint3D mCoordination;

		MarkerInfo() : mSeriesId(0), mGroupId(0)
		{
		}

		MarkerInfo(Int32 i, Int32 j, DPoint3D& irPoint) : mSeriesId(i), mGroupId(j),
			mCoordination(irPoint)
		{
		}

		MarkerInfo(const MarkerInfo& irMarkerInfo) : mSeriesId(irMarkerInfo.mSeriesId),
			mGroupId(irMarkerInfo.mGroupId), mCoordination(irMarkerInfo.mCoordination)
		{
		}

		//bool operator<(const MarkerInfo& irMarkerInfo)
		//{
		//	if(mSeriesId == irMarkerInfo.mSeriesId)
		//		return mGroupId < irMarkerInfo.mGroupId;
		//	else
		//		return mSeriesId < irMarkerInfo.mSeriesId;
		//}
	};

	//used by std::sort to decide the drawing order of the markers
	inline bool ComparisionOfTDSMakerInfo(const std::pair<double, MarkerInfo>& iFirst, const std::pair<double, MarkerInfo>& iSencond)
	{
		 if(iFirst.first == iSencond.first)
		 {
			 if(iFirst.second.mSeriesId == iSencond.second.mSeriesId)
				 return iFirst.second.mGroupId < iSencond.second.mGroupId;
			 else
				 return iFirst.second.mSeriesId < iSencond.second.mSeriesId;
		 }
		 else
			 return iFirst.first < iSencond.first;
	}

	struct TDScatterSeriesOptions
	{
		typedef MSynch::SmartPtr<TDScatterSeriesOptions> Ptr;
		Int32 mMarkerSize;
		unsigned char mMarkerShape;
		MsiChart::ChartAdvFormat	mGraphNumberFormat;
		Int32 mSeriesId;
		bool mIsShowDataLabels;
		TDScatterSeriesOptions() :
			mMarkerSize(31), mMarkerShape(1), mSeriesId(0), mIsShowDataLabels(false)
		{
		}
	};

	struct TDScatterOptions
	{
		bool mIsLineToLeftWall;
		bool mIsLineToRightWall;
		bool mIsLineToFloor;
		bool mIsConnectingNeighbors;
		bool mIsShowDataLabels;
		bool mIsUse3DMarker;

		std::vector<TDScatterSeriesOptions::Ptr> mTDSSeriesOptions;

		TDScatterOptions() : mIsLineToLeftWall(false),
			mIsLineToRightWall(false), mIsLineToFloor(false),
			mIsConnectingNeighbors(false),
			//mIsShowDataLabels(false),
			mIsUse3DMarker(true)//By default, we use 3D markers
		{								   	
		}
	};



	//This class is for 3D-Scatter chart.	
	class ValuePlot3DManager : public PlotManager
	{
	public:
		typedef MSynch::SmartPtr<ValuePlot3DManager> Ptr;

		//ctor & dtor
		ValuePlot3DManager(const TripleId& irTripleId, GraphObjectManager* ipManager, const Rect2D& irPlotArea);
		virtual ~ValuePlot3DManager();

		//from PlotManager
		virtual void GenerateMapAndList(); 	
		virtual void GetOptions(Int32 iSeriesId, Int32 iPropertyId, VARIANT* opValue) const;
	protected:
		//sanity 
		/**
		This method is to check if the dataset has enough data for drawing 
		@return ture if the data set if valid for drawing and false otherwise
		*/		
		bool hSanitySetCheck();		
		
		//property population
		void hLoadProperties();
		void hLoadSeriesProperties(Int32 iSeriesID);
		
		//axes & walls
		/**
		This method is to get the max and min value of one specific axis for deciding its scale
		@param iorMin the minimual value for the target axis
		@param iorMax the maximual value for the target axis
		@param iVAType the target value axis type
		*/		
		void hGetMinMaxValueForAxes(double& iorMin, double& iorMax, TDS_VATYPE iVAType);

		/**
		This method is to populate the wall manager for later use
		*/		
		void hPopulateWallManager();

		/**
		This method is to populate the XYZ axes for later use
		*/		
		void hPopulateAxes();

		/**
		This method is to get value for a scatter marker
		@param iSeriesIndex the series id(index) for this scatter marker
		@param iGroupIndex the group id(index) for this scatter marker
		@param iorValue the result value is stored in a 3D point
		@return true if we get the value successfully, and false otherwise
		*/		
		bool hGetScatterValue(Int32 iSeriesIndex, Int32 iGroupIndex, DPoint3D& iorValue);

		/**
		This method is to compute the drawing order	of all the markers
		@param iorDrawingOrder the result order is stored in a vector of MarkerInfo 
		*/		
		void hComputeDrawingOrder(std::vector<MarkerInfo>& iorPreWallOrder, std::vector<MarkerInfo>& iorDrawingOrder,
			std::vector<MarkerInfo>& iorPostWallOrder);

		//for link to wall & floor & neighbor : we have to decide the drawing sequence first.
		/**
		This method is to draw the linked lines, including series lines
		@param irDrawingOrder the drawing order of all the markeres 
		*/		
		void hGenerateLinkedLines(const std::vector<MarkerInfo>& irDrawOrder);	 

		/**
		This method is to draw the linked lines to side walls
		@param irDrawingOrder the drawing order of all the markeres 
		@param iTargetFace, the type of the axis which is perpendicular to the side face we want to link to
		*/		
		void hGenerateLineToSideFace(const std::vector<MarkerInfo>& irDrawOrder, TDS_VATYPE iTargetFace);

		/**
		This method is to draw the series lines
		*/		
		void hGenerateSeriesLines();

		//for marker & data labels
		/**
		This method is to draw all the data markers 
		@param irDrawingOrder the drawing order of all the markeres 
		*/		
		void hGenerateMarkers(const std::vector<MarkerInfo>& irDrawOrder);

		/**
		This method is to draw all the 2d data markers 
		@param irDrawingOrder the drawing order of all the markeres 
		@param irMarkerCollections collections to hold the markeres
		*/		
		void hGenerate2DMarkers(const std::vector<MarkerInfo>& irDrawOrder, std::vector<GraphCollectionObject::Ptr>& irMarkerCollections);

		/**
		This method is to draw all the 3d data markers 
		@param irDrawingOrder the drawing order of all the markeres 
		@param irMarkerCollections collections to hold the markeres
		*/		
		void hGenerate3DMarkers(const std::vector<MarkerInfo>& irDrawOrder, std::vector<GraphCollectionObject::Ptr>& irMarkerCollections);

		/**
		This method is to set the size for the 3D marker
		@param iMarkerSize the marker size value user set in the dialog
		*/		
		Int32 hGetMarkerCubicSize(Int32 iMarkerSize);
		Int32 hGetMarkerSize(Int32 iMarkerSize);
		/**
		This method is to comupte the edge length of cubic which encapusulate the marker most tightly 
		*/		
		void hSetMaxMarkerCubicSize();

		/**
		This method is to get the light angle for the fill effect of 3D ball
		@param irGradientVector the vector indicating the graidient direction 
		*/
		Int32 hGetLightAngle(const Vector2D& irGradientVector);
		
		/**
		This method is to map the gradient direction from 3d space to 2D space
		@param irCenter the central point of the ball.
		@param iorGradientVector the vector indicating the graidient direction 
		*/
		void hGet2DGradientVector(const DPoint3D& irCenter, Vector2D& iorGradientVector);

		/**
		This method is to draw the data labels of all the markers
		@param irDrawingOrder the drawing order of all the markeres 
		*/		
		void hGenerateDatalabels(const std::vector<MarkerInfo>& irDrawOrder);
		
		/**
		This method is to get the datalabel content for one specific marker
		@param iSeriesId the series id of the hosting marker
		@param iGroupId the group id of the hosting marker
		@param ipCollection the collection to hold all the data labels in the same series.
		*/		
		TextObject::Ptr hGetDatalabel(Int32 iSeriesId, Int32 iGroupId, GraphCollectionObject* ipCollection);
			

	private:
		/**
		This method is used to 1) decided if or not the data marker will be shown 2) if shown, then its absolute position
		@param irLogicalPoint Specifies a logical coordinate.
		@param iorAbsPoint Output 3D absolute coordinate 
		@return Return whether this value should be shown or not. If the return value is false, orPosition is not valid.
		*/
		bool hMapToAbsPosition(const DPoint3D& irLogicalPoint, DPoint3D& iorAbsPoint) const;

		//specail handling of add legned marker to marker collection
		/**
		This method is to adjust the data label position acccording to the position of its hosting marker
		@param ipText pointer to the data label
		@param irRefRect the bounding rectangle of the hosting marker
		@param iRotationInfo the rotation information of the data label
		*/		
		void hMoveDataLabel(TextObject* ipText, const Rect2D& irRefRect, EnumDSSGraphFontRotation iRotationInfo);

		/**
		This method is to adjust the data label position acccording to the position of its hosting marker
		@param iSeriesId the series id of the marker collection
		@param ipRiserCollection the collection to hold all the markers in the same series.
		@param iMarkerShape the marker shape for the markers
		*/		
		void hAddLegendMarkerToCollection(Int32 iSeriesId,
			GraphCollectionObject* ipRiserCollection, 
			 unsigned char iMarkerShape = DssGraphMarkerShapeRectangle) const;
		
		TDS_3DMAKERSHAPE hMapMarkerShape(Int32 iMarkerShape);
		GraphObject::Ptr hCreate3DMarkerByShape(const MarkerInfo& irMarkerInfo, TDS_3DMAKERSHAPE iMarkerShape, std::vector<GraphCollectionObject::Ptr>& irMarkerCollections);

		//options
		TDScatterOptions mTDSOptions; ///< Three D Scatter options
		
		//x-y-z axes	
		ValueAxis::Ptr mXValueAxisPtr; ///< X Axis
		ValueAxis::Ptr mYValueAxisPtr; ///< Y Axis
		ValueAxis::Ptr mZValueAxisPtr; ///< Z Axis

		//wall & length with no margin
		WallManager::Ptr mWallManagerPtr; ///< Wall Manager
		Int32 mMaxMarkerCubicLength;

		//data set related
		Int32 mGroupCount; ///< Number of group
		Int32 mSeriesCount; ///< Number of series
		Transformer* mpTransformer; ///< Transformer		


		//collections
		std::vector<GraphCollectionObject::Ptr> mMarkerCollections; //for markers

	};
}

#endif