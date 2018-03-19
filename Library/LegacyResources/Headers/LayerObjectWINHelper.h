//==============================================================================================
//	FILENAME	:	LayerObjectWinHelper.h
//	AUTHOR		:	Joseph Adamski-Smith
//	CREATION	:	2015-01-19
//	Copyright (C) MicroStrategy Incorporated 2015
//	All Rights Reserved
//==============================================================================================
#ifndef MsiChart_LayerObjectWinHelper_h
#define MsiChart_LayerObjectWinHelper_h

#include "LayerObject.h"
#include "LayerObjectHelper.h"

namespace MsiChart
{
	class LayerObjectWINHelper: public LayerObjectHelper
	{
	public:
		LayerObjectWINHelper(LayerObject* iLayerObject):LayerObjectHelper(iLayerObject){};
		~LayerObjectWINHelper(){};
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
