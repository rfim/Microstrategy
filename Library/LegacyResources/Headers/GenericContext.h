//==============================================================================================
//	FILENAME	:	GenericContext.h
//	AUTHOR		:	Liang Liu
//	CREATION	:	2008-10-31
//	Copyright (C) MicroStrategy Incorporated 2008
//	All Rights Reserved
//==============================================================================================
#ifndef MsiChart_GenericContext_h
#define MsiChart_GenericContext_h

#include "ChartContext.h"
//#include "FormatManager.h"

namespace MsiChart
{
	/**
	This class is used to provide chart properties when GraphStyle does not exist.
	*/
	class GenericContext: public ChartContext
	{
	public:
		GenericContext(Dataset::Ptr iDatasetPtr);
		GenericContext(Dataset::Ptr iDatasetPtr, const char* ipGDFFileName);
		GenericContext(Dataset::Ptr iDatasetPtr, FormatManager::Ptr iFormatManagerPtr, const char* ipGDFFileName); 
		virtual ~GenericContext();	

		//Factory methods for FormatLine/FormatFill/FormatFont
		virtual FormatLine::Ptr GetFormatLine(const TripleId& irTripleId, Int32 iCollectionType,EnumShapeType iShape = _GENERIC) const;
		virtual FormatFill::Ptr GetFormatFill(const TripleId& irTripleId, Int32 iCollectionType,EnumShapeType iShape = _GENERIC) const;
		virtual FormatFont::Ptr GetFormatFont(const TripleId& irTripleId) const;
		virtual bool GetProperty(Int32 iPropertyId, const TripleId& irTripleId, CComVariant* opValue) const;	
		virtual void PutProperty(Int32 iPropertyId, const TripleId& irTripleId, CComVariant* ipValue);
		virtual bool IsGenericContext() const;

	private:
		/**
		Check whether current graph type is a Vertical type.
		@return Return true if current graph type is Vertical. Otherwise, return false.
		*/
		bool hIsVertical() const;

		/**
		Load the GDF content to the mProperties.
		@param iGDFFileName The input GDF file name.
		*/
		void hLoadGDF(const char* ipGDFFileName);		
		
		bool hIsAutoAxis(Int32 iObjectId) const;
		
		//FormatManager::Ptr mFormatManagerPtr;
		Int32 hGetColorIndexForFill(const TripleId& irTripleId, FormatFill::Ptr iFormatFillPtr) const;

		virtual Int32 hGetGMColorIndex(const TripleId& irTripleId, Int32 iDefaultIndex, Int32 iColColorIndexOffset = -1) const; //Get Color Index according to various color option in Graph matrix.
		
        FormatLine::Ptr hSpecialLineEffect(FormatLine::Ptr iFormatLinePtr, const TripleId& irTripleId, Int32 iCollectionType) const;  //< Hard coded, special handling cases...
        void hResolveFontFormat(FormatFont* ioFormatFont,const TripleId& irTripleId) const;
        void hResolveLineFormat(FormatLine* ioFormatLine,const TripleId& irTripleId,EnumShapeType iShape = _GENERIC) const;
        void hResolveFillFormat(FormatFill* ioFormatFill,const TripleId& irTripleId,EnumShapeType iShape = _GENERIC) const;
	
    };

	inline bool GenericContext::IsGenericContext() const
	{
		return true;
	}
}

#endif
