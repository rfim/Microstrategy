//==============================================================================================
//	FILENAME	:	MSIGridContext.h
//	AUTHOR		:	Bing Huang
//	CREATION	:	2010-03-28
//	Copyright (C) MicroStrategy Incorporated 2008
//	All Rights Reserved
//==============================================================================================
#ifndef MsiChart_MSIGridContext_h
#define MsiChart_MSIGridContext_h

#include "ChartContext.h"
//#include "FormatManager.h"

namespace MsiChart
{
	/**
	 This class is used to provide chart properties when GraphStyle does not exist.
	 */
	class MSIGridContext: public ChartContext
	{
	public:
		MSIGridContext(Dataset::Ptr iDatasetPtr, FormatManager::Ptr iFormatManagerPtr, const char* ipGDFFileName);
		virtual ~MSIGridContext();
		
		//Factory methods for FormatLine/FormatFill/FormatFont
		virtual FormatLine::Ptr GetFormatLine(const TripleId& irTripleId, Int32 iCollectionType, EnumShapeType iShape = _GENERIC) const;
		virtual FormatFill::Ptr GetFormatFill(const TripleId& irTripleId, Int32 iCollectionType, EnumShapeType iShape = _GENERIC) const;
		virtual FormatFont::Ptr GetFormatFont(const TripleId& irTripleId) const;			
		virtual bool GetProperty(Int32 iPropertyId, const TripleId& irTripleId, CComVariant* opValue) const;	
		virtual bool IsGenericContext() const;
	private:
		void hLoadGDF(const char* ipGDFFileName);
		
	};
	
	inline bool MSIGridContext::IsGenericContext() const
	{
		return true;
	}
}

#endif
