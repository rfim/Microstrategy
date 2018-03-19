//==============================================================================================
//	FILENAME	:	LayerObjectIOSHelper.h
//	AUTHOR		:	Joseph Adamski-Smith
//	CREATION	:	2015-01-19
//	Copyright (C) MicroStrategy Incorporated 2015
//	All Rights Reserved
//==============================================================================================
#ifndef MsiChart_LayerObjectIOSHelper_h
#define MsiChart_LayerObjectIOSHelper_h

#include "LayerObject.h"
#include "LayerObjectHelper.h"

namespace MsiChart
{
	class LayerObjectIOSHelper: public LayerObjectHelper
	{
	public:
		LayerObjectIOSHelper(LayerObject* iLayerObject):LayerObjectHelper(iLayerObject){};
		~LayerObjectIOSHelper(){};
		//---------Override functions-------------------------------------		
		void Draw() const;
		void GenerateView();
		void GenerateLayerObjectView() const;
		void ResetLayerView();
		// Draw the objects in mGraphObjectList;
		void DoDraw() const;			
	};	
}
#endif
