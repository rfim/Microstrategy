//==============================================================================================
//	FILENAME	:	LayerObjectHelper.h
//	AUTHOR		:	Joseph Adamski-Smith
//	CREATION	:	2015-01-19
//	Copyright (C) MicroStrategy Incorporated 2015
//	All Rights Reserved
//==============================================================================================
#ifndef MsiChart_LayerObjectHelper_h
#define MsiChart_LayerObjectHelper_h

namespace MsiChart
{
	class LayerObject;

	class LayerObjectHelper
	{
	public:
		LayerObjectHelper(LayerObject* iLayerObject);
		~LayerObjectHelper();
		virtual void Draw() const;
		virtual void GenerateView();
		virtual void GenerateLayerObjectView() const;
		virtual void ResetLayerView();

		// Draw the objects in mGraphObjectList;
		virtual void DoDraw() const;			

	protected:
		LayerObject* mpLayerObject;
	};	
	
}
#endif
