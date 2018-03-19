//
//  DateTimeHelper.h
//  ReportServiceCore
//
//  Created by He Tengfei on 2/23/12.
//  Copyright 2012 MicroStrategy, Inc. All rights reserved.
//
#ifndef DATETIMEHELPER_H
#define DATETIMEHELPER_H 1

#include "DSSDateTime.h"

typedef enum {eBasis30360 = 0, eBasisActualActual, eBasisActual360, eBasisActual365,
    eBasisEurop30360, eBasis30360US} Basis;

bool isLegalDate(const MDataType::DSSDateTime& iTime);
MDataType::DSSDateTime shiftByMonth(MDataType::DSSDateTime iTime, int month);
unsigned int difference(const MDataType::DSSDateTime& iThis, const MDataType::DSSDateTime& date, Basis basis);
unsigned int dateToNumber(const MDataType::DSSDateTime& iThis, Basis basis, int day1);
int daysPerYear(Basis basis); 

#endif
