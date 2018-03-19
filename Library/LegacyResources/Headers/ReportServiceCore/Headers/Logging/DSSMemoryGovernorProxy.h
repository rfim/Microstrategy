//
//  DSSMemoryGovernorProxy.h
//  ReportServiceCore
//
//  Created by Zhenhua Sheng on 4/25/11.
//  Copyright 2011 MicroStrategy. All rights reserved.
//

#ifndef __DSSMEMORYGOVERNORPROXY_H__
#define __DSSMEMORYGOVERNORPROXY_H__

#define DSS_MEMORY_GOVERNOR_ENABLE YES

#include <string>

using namespace std;

class DSSMemoryGovernorProxy {
public:
	static DSSMemoryGovernorProxy* GetInstance();
	virtual ~DSSMemoryGovernorProxy();
	void SetIPadMGInstance(void* ipIPadMGInstance);
	void addTraceLog(string iTraceLog);

private:
	DSSMemoryGovernorProxy();
	static DSSMemoryGovernorProxy* mpMGInstance;
	void* mpIPadMGInstance;
};

#endif