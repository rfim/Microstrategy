/*
 *  DSSCellFmtData.h
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 12/29/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CELLFMTDATA_H
#define CELLFMTDATA_H

#include "GUID.h"

namespace MDataType
{
	class DSSCellFmtData
	{
	public:
		DSSCellFmtData();
		DSSCellFmtData(double lValue, int lType, int lFormatID);
        DSSCellFmtData(double lValue, int lType, GUID &lFormatGUID);
		DSSCellFmtData(const DSSCellFmtData& irSrc);
		
		DSSCellFmtData& operator = (const DSSCellFmtData& irSrc);
		
		virtual ~DSSCellFmtData() throw();
		bool operator < (const DSSCellFmtData& data2) const;
		bool operator == (const DSSCellFmtData& data2) const;
		double GetValue() const {return mValue;}
		int GetType(){return mType;}
		int GetFormatID(){_ASSERT(false);  return mFormatID;}
        void setValue(double iVal){mValue = iVal;}
        GUID GetFormatGUID();
	private:
		int CellFmtDataCompare(const DSSCellFmtData & data2) const;
		double mValue;
		int mType;
		int mFormatID;
        GUID mFormatGUID;
	};
}

#endif
