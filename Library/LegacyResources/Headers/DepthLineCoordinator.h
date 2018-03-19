//==============================================================================================
//	FILENAME	:	DepthLineCoordinator.h
//	AUTHOR		:	Liang Liu
//	CREATION	:	2008-12-17
//	Copyright (C) MicroStrategy Incorporated 2008
//	All Rights Reserved
//==============================================================================================

#ifndef MsiChart_DepthLineCoordinator_h
#define MsiChart_DepthLineCoordinator_h

#include "DepthLineObject.h"

namespace MsiChart
{	
	/**
	This class is mainly used to sort depth lines. The sorted depth lines is expected to be in correct 
	drawing order, so that the pseudo 3D effect of the whole set of depth lines will look realistic.
	*/
	class DepthLineCoordinator
	{
	public:
		/** constructor
		@iIsCurveLine Indicates whether current DepthLineCoordinator need to handle curve depth lines or 
		straight depth lines.
		*/
		DepthLineCoordinator(bool iIsCurveLine);
		/**
		Add a DepthLineObject into current DepthLineCoordinator.
		@param iDepthLinePtr Specifies a DepthLineObject.
		*/
		void AddDepthLine(DepthLineObject::Ptr iDepthLinePtr);
		/**
		Get sorted depth lines.
		@param orSortedDepthLines Output the sorted depth lines.
		*/
		void GetSortedDepthLines(std::vector<DepthLineObject::Ptr>& orSortedDepthLines);
	
	private:
		/**
		This is a helper method of GetSortedDepthLines().
		This method solves the topological sorting problem using DFS algorithm.
		@param iIndex Specifies an index for mDepthLines.
		@param orSortedDepthLines Output the sorted depth lines.
		*/
		void hDFS(Int32 iIndex, std::vector<DepthLineObject::Ptr>& orSortedDepthLines);		

		/// Indicates the type of current DepthLineCoordinator.
		bool mIsCurveLine;	
		
		/// Maintains a vector of DepthLineObject pointers. 
		std::vector<DepthLineObject::Ptr> mDepthLines;	 
	
		/// This is a helper flag array for DFS algorithm.
		std::vector<bool> mVisited;
	};
}

#endif
