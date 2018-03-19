//
//  DSSPerformanceProxy.h
//  ReportServiceCore
//
//  Created by Sun Han on 3/8/11.
//  Copyright 2011 MicroStrategy. All rights reserved.
//

#ifndef __DSSPERFORMANCEPROXY_H__
#define __DSSPERFORMANCEPROXY_H__

#ifndef __ANDROID__
	#define DSS_TIME_LOGGER_ENABLE YES
#endif

#include <string>

using namespace std;

class DSSPerformanceProxy {
public:
	static DSSPerformanceProxy* GetInstance();
	virtual ~DSSPerformanceProxy();
	void SetIPadPDInstance(void* ipIPadPDInstance);
	void performanceEventWillBegin();
	void performanceEventDidFinished(string iDescription);

private:
	DSSPerformanceProxy();
	static DSSPerformanceProxy* mPPInstance;
	void* mpIPadPDInstance;
};

#endif

