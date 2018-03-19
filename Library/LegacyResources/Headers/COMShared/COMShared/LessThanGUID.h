//==============================================================================================
//	FILENAME	:	LessThanGUID.h
//  PROJECT		:	COMShared
//	AUTHOR		:	Cezary Raczko
//	CREATION	:	2003-06-30
//	Copyright (C) MicroStrategy, Inc. 2003-2006
//==============================================================================================

#ifndef MCOMShared_LessThanGUID_h
#define MCOMShared_LessThanGUID_h

#include "COMShared/COMShared/COMShared.h"
#include "PDCHeader/PDCfunctional"
#include "PDCHeader/PDCvector"
#include "Base/Base/GUID.h"
#include "Base/Base/String.h"

namespace MCOMShared
{

/**
	Function to compare two GUIDs
	@return
		0 - irID1 == irID2
		1 - irID1 > irID2
		-1 - irID1 < irID2
*/
static inline Int32 InlineCompareGUID(const GUID& irID1, const GUID& irID2)
{
	const unsigned Int32* p1 = (const unsigned Int32*)&irID1;
	const unsigned Int32* p2 = (const unsigned Int32*)&irID2;

	for (Int32 i = 0; i < 4; ++i, ++p1, ++p2)
	{
		if (*p1 < *p2) return -1;
		else if (*p1 > *p2) return 1;
	}

	return 0;
}

};

typedef std::pair<GUID, GUID> GUIDPair;

typedef std::pair<MBase::GUID, MBase::GUID> GuidPair;
typedef std::pair<MBase::GUID, std::wstring> GuidStrPair;

namespace std
{

/**
	Template specialization to allow ids to be used in stl data structures.
*/
template <>
struct less<GUID>
{
	inline bool operator()(const GUID& irID1, const GUID& irID2) const
	{
		return MCOMShared::InlineCompareGUID(irID1, irID2) < 0;
	}
};

/**
	Template specialization to allow id pairs to be used in stl data structures.
*/
template <>
struct less<GUIDPair>
{
	inline bool operator()(const GUIDPair& irIDPair1, const GUIDPair& irIDPair2) const
	{
		Int32 lResult = MCOMShared::InlineCompareGUID(irIDPair1.first, irIDPair2.first);
		if (lResult != 0)
		{
			return lResult < 0;
		}
		else
		{
			return MCOMShared::InlineCompareGUID(irIDPair1.second, irIDPair2.second) < 0;
		}
	}
};

/**
	Template specialization to allow id vectors to be used in stl data structures.
*/
template <>
struct less<vector<GUID> >
{
	inline bool operator()(const vector<GUID>& irIDVector1, const vector<GUID>& irIDVector2) const
	{
		size_t lSize = min(irIDVector1.size(), irIDVector2.size());

		for (size_t lIndex = 0; lIndex < lSize; ++lIndex)
		{
			Int32 lResult = MCOMShared::InlineCompareGUID(irIDVector1[lIndex], irIDVector2[lIndex]);
			if (lResult != 0)
				return lResult < 0;
		}

		return irIDVector1.size() < irIDVector2.size();
	}
};

/**
	Template specialization to allow GUIDStringPair to be used in stl data structures.
*/
template <>
struct less<GuidStrPair >
{
	inline bool operator()(const GuidStrPair& irIDPair1, const GuidStrPair& irIDPair2) const
	{
		Int32 lResult = MCOMShared::InlineCompareGUID(irIDPair1.first, irIDPair2.first);
		if (lResult != 0)
		{
			return lResult < 0;
		}
		else
		{
			return irIDPair1.second.compare(irIDPair2.second) < 0;
		}
	}
};

template <>
struct less<std::pair<MBase::String, MBase::String> >
{
	inline bool operator()(const std::pair<MBase::String, MBase::String> & irStringPair1,
		const std::pair<MBase::String, MBase::String>& irStringPair2) const
	{
		Int32 lResult = irStringPair1.first.compare(irStringPair2.first);
		if (lResult != 0)
		{
			return lResult < 0;
		}
		else
		{
			return irStringPair1.second.compare(irStringPair2.second) < 0;
		}
	}

};

};

#endif
