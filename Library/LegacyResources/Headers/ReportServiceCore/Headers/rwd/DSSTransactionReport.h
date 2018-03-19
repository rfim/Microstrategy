/*
 *  DSSTransactionReport.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-11-16.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef TRANSACTION_REPORT_H
#define TRANSACTION_REPORT_H

#include "GUID.h"
#include "DSSrwdconstants.h"
#include <vector>
#include <map>
#include "DSSColumnMappingInfo.h"

class DFCCSITransactionBase;

class DSSTransactionReport
{
private:
	GUID mID;
	EnumDSSReportType mType;
	std::map<int, DSSColumnMappingInfo*> mColumnMap;
    DFCCSITransactionBase* mpTransactionCSI;
public:
	DSSTransactionReport(GUID& iID, EnumDSSReportType iType);
	virtual ~DSSTransactionReport();
	GUID getID();
	EnumDSSReportType getType();
	DSSColumnMappingInfo* getColumn(int iIndex);
	std::map<int, DSSColumnMappingInfo*>* getColumnMap();
	void AddColumn(DSSColumnMappingInfo* iColumn);
    void setTransactionCSI(DFCCSITransactionBase* ipCSI);
    DFCCSITransactionBase* getTransactionCSI();
};

#endif
