/*
 *  DSSTimeLogger.h
 *  ReportServiceCore
 *
 *  Created by Sun Han on 3/4/11.
 *  Copyright 2011 MicroStrategy. All rights reserved.
 *
 */

#ifndef TIMELOGGER_H
#define TIMELOGGER_H


#include <CoreFoundation/CoreFoundation.h>
#include <string>
#include <stack>
#include <vector>
#include <iostream>
#include <DSSAEHelper.h>


using namespace std;

enum EnumTimeLoggerLevel
{
	TLL_0 = 0,
	TLL_1 = 1,
	TLL_2 = 2,
	TLL_3 = 3,
	TLL_4 = 4,
	TLL_5 = 5,
	TLL_6 = 6,
	TLL_7 = 7,
};

struct DSSTimeLoggerEntry {
	CFDateRef mStart;
	CFDateRef mEnd;
	CFTimeInterval mDuration;
	CFTimeInterval mSubDurations;
	string mDesc;
	EnumTimeLoggerLevel mLevel; 
	DSSTimeLoggerEntry(EnumTimeLoggerLevel iLevel, string iDesc);
	virtual ~DSSTimeLoggerEntry();
};

class DSSTimeLogger{
private:
	DSSTimeLogger();
	stack<DSSTimeLoggerEntry*> mLogStack;
	vector<DSSTimeLoggerEntry*> mCompletedLogs;
	static DSSTimeLogger* _mInstance;
	void FinishTop();
public:
	static DSSTimeLogger* getInstance();
	virtual ~DSSTimeLogger();
	void EventWillBegin(EnumTimeLoggerLevel bLevel, string iDesc);
	void EventDidFinish(EnumTimeLoggerLevel fLevel);
	void Clear();
	void PrintAllCompletedRecords();
};

#endif