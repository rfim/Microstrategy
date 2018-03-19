//==============================================================================================
//	FILENAME	:	GraphCollectionObject.h
//	AUTHOR		:	Liang Liu
//	CREATION	:	2008-05-22
//	Copyright (C) MicroStrategy Incorporated 2008
//	All Rights Reserved
//==============================================================================================
#ifndef MsiChart_GraphCollectionObject_h
#define MsiChart_GraphCollectionObject_h

#include "PDCHeader/PDCvector"
#include "GraphObject.h"

namespace MsiChart
{	
	/**
	Maintains a list of GraphObjects which can be selected and highlighted together.
	*/
	class GraphCollectionObject: public GraphObject
	{
	public:
		typedef MSynch::SmartPtr<GraphCollectionObject> Ptr;

		/**	Constructor.
		@param irTripleId This is used by base class.(GraphObject)
		@param ipManager This is used by base class.
		@param iCollectionType Indicate the type of this GraphCollectionObject. See EnumCollectionType.
		*/
		GraphCollectionObject(const TripleId& irTripleId, GraphObjectManager* ipManager, EnumCollectionType iCollectionType); 
		virtual ~GraphCollectionObject();

		/** This function will add a GraphObject into current GraphCollectionObject. 
		@param iGraphObjectPtr Pointer to the GraphObject which is ready to be added into current GraphCollectionObject.
		Current GraphCollectionObject will be the parent object of iGraphObjectPtr. 		
		@param iShareFormat Bool value indicate whether child object will share the format information from parent.		
		*/
		void AddGraphObject(GraphObject::Ptr iGraphObjectPtr, bool iShareFormat = true);

		/**	Clear mGraphObjectCollection.		
		*/
		void ClearCollection();

		/**	This function is to set the mFormatFillPtr data member. 
		@param iFormatFillPtr The input FormatFill object pointer.
		*/
		void SetFormatFill(FormatFill::Ptr iFormatFillPtr); 

		/** This function is to set the mFormatFontPtr data member. 
		@param iFormatFontPtr The input FormatFill object pointer.
		*/
		void SetFormatFont(FormatFont::Ptr iFormatFontPtr); 

		/** This function returns mFormatFillPtr data member. 
		@return mFormatFillPtr
		*/
		FormatFill::Ptr GetFormatFill() const;
		/**	This function returns mFormatFontPtr data member. 
		@return mFormatFontPtr
		*/
		FormatFont::Ptr GetFormatFont() const;

		//---------Override functions-------------------------------------		
		virtual void Draw() const;
		virtual void GenerateImageMap(ImageMap& iorImageMap);
		virtual void GetOptions(Int32 iPropertyId, CComVariant* opValue) const;
		virtual void DrawOutLine();
		virtual bool PointInObject(const Point2D& irPoint);
		virtual bool ObjectInRectangle(const Rect2D& irRect);
		virtual void GetClockwisePolygon(std::vector<Point2D>& orPolygon) const; 

		/**	This function returns the size of current collection. 
		@return Size of current collection.
		*/
		unsigned Int32 GetCollectionSize();
		/**	This function returns a graph object in current collection by index. 
		@param iIndex The index of desired graph object.
		@return mGraphObjectCollection[iIndex] 
		*/
		GraphObject::Ptr GetObjectByIndex(Int32 iIndex);

		/**	
		Get the collection type of current collection.
		@return mCollectionType. 
		*/
		EnumCollectionType GetCollectionType() const;

	protected:
		/// A collection of GraphObjects which share the same format properties. 
		std::vector<GraphObject::Ptr> mGraphObjectCollection;
		/// A pointer to FormatFill object.
		FormatFill::Ptr mFormatFillPtr;
		/// A pointer to FormatFont object.
		FormatFont::Ptr mFormatFontPtr;
		/// The type of current GraphCollectionObject.
		EnumCollectionType mCollectionType;
	};	
	

	inline FormatFill::Ptr GraphCollectionObject::GetFormatFill() const
	{
		return mFormatFillPtr;
	}

	inline FormatFont::Ptr GraphCollectionObject::GetFormatFont() const
	{
		return mFormatFontPtr;
	}

	inline EnumCollectionType GraphCollectionObject::GetCollectionType() const
	{
		return mCollectionType;
	}
}
#endif
