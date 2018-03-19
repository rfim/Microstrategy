//==============================================================================================
//	FILENAME	:	FancyBoxObject.h
//	AUTHOR		:	Fangyao Ma
//	CREATION	:	2008-08-25
//	Copyright (C) MicroStrategy Incorporated 2008
//	All Rights Reserved
//==============================================================================================

#ifndef MsiChart_FancyBoxObject_h
#define MsiChart_FancyBoxObject_h 1

#include "RectangleObject.h"
#include "../Common.h"


namespace MsiChart
{
	/**
	 *	Fancy box object, add "Fancy" style to one rectangle area
	 */
	class FancyBoxObject: public RectangleObject
	{
	public:
		typedef MSynch::SmartPtr<FancyBoxObject> Ptr;

		FancyBoxObject(const TripleId& irTripleId, 
						GraphObjectManager* ipManager,		// manager who create the object						
						EnumBoxStyle	iBoxStyle,
						const Rect2D& irRect,
						GraphCollectionObject* ipParentObject = NULL);

		virtual ~FancyBoxObject();

		//pure virtual functions from GraphObject
		virtual void Draw() const;

		virtual void GetOptions(Int32 iPropertyId, CComVariant* opValue) const;
		
	private:
		void hDrawFancyBox() const;

		EnumBoxStyle	mBoxStyle;	///< Enumeration of Box Style, as in FormatFont
	};


}
#endif
