//========================================================
//	Filename:		Common.h
//	Date:			2008-05-20
//	Authors:		Bing Huang

//	Copyright (C) 1997-2008 MicroStrategy Incorporated
//	All rights reserved
//========================================================

#ifndef MsiChart_ImageMap_h
#define MsiChart_ImageMap_h

#include "PDCHeader/PDCvector"
#include "Base/Base/BaseString.h"
#include "Synch/Defines/SmartPtr.h"

	// copy from Z:/Kernel/Chart/Chart/ImageMap.h
	// Use smart pointer for AreaRec.
#ifndef IMAGEMAP_H
	class AreaRec
	{
	public:
		// typedef MSynch::SmartPtr<AreaRec> Ptr;

		AreaRec(Int32 iShape, Int32 iObjectID, Int32 iSeriesID, Int32 iGroupID,
			const wchar_t* iCoordinates) :
		mShape(iShape),
			mObjectID(iObjectID),
			mSeriesID(iSeriesID),
			mGroupID(iGroupID),
			mCoordinates(iCoordinates)
		{
		}

		~AreaRec() throw()
		{
		}

		AreaRec(const AreaRec& iAreaRec) :
		mShape(iAreaRec.mShape),
			mObjectID(iAreaRec.mObjectID),
			mSeriesID(iAreaRec.mSeriesID),
			mGroupID(iAreaRec.mGroupID),
			mCoordinates(iAreaRec.mCoordinates),
			mToolTip(iAreaRec.mToolTip)
		{
		}

		const AreaRec& operator =(const AreaRec& iAreaRec)
		{
			if(this == &iAreaRec)
				return *this;

			mShape = iAreaRec.mShape;
			mObjectID = iAreaRec.mObjectID;
			mSeriesID = iAreaRec.mSeriesID;
			mGroupID = iAreaRec.mGroupID;
			mCoordinates = iAreaRec.mCoordinates;
			mToolTip = iAreaRec.mToolTip;

			return *this;
		}

		Int32 GetShape()
		{
			return mShape;
		}

		Int32 GetObjectID()
		{
			return mObjectID;
		}

		Int32 GetSeriesID()
		{
			return mSeriesID;
		}

		Int32 GetGroupID()
		{
			return mGroupID;
		}

		const wchar_t* GetCoordinates()
		{
			return mCoordinates.c_str();
		}


	private:
		Int32 mShape;
		Int32 mObjectID;
		Int32 mSeriesID;
		Int32 mGroupID;
		MBase::String mCoordinates;
		MBase::String mToolTip;
	};

	typedef std::vector<AreaRec*> ImageMap;
	#define IMAGEMAP_H
	
#endif


#endif
