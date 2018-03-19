/*
 *  DSSCMacroManipulation.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 11-3-12.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CMACROMANIPULATION_H
#define CMACROMANIPULATION_H

#include "DSSCManipulation.h"

class DSSCMacroManipulation : public DSSCManipulation
{
private:
	std::vector<DSSCManipulation*> mManipulations;
	bool mIsForChangeVizType;
public:
	DSSCMacroManipulation();
	virtual ~DSSCMacroManipulation();
	int Execute();
	int size();
	DSSCManipulation* getManipulationAtIndex(int index);
	void AddManipulation(DSSCManipulation* ipManipulation);
    std::vector<DSSCManipulation*>* getManipulations();
	DSSRWNode* getSelectionNode();
    std::vector<DSSRWNode*>* getNodesDataChanged();
    bool IsNeedServer();
    
    bool getIsForChangeVizType();
    void setIsForChangeVizType(bool iIsChangeDisplayMode);
    virtual std::vector<std::string>* getNodeKeysExclusive();
};

inline int DSSCMacroManipulation::size()
{
    return (int)mManipulations.size();
}

inline DSSCManipulation* DSSCMacroManipulation::getManipulationAtIndex(int iIndex)
{
    if (iIndex<0 || iIndex>=mManipulations.size())
        return NULL;
    return mManipulations[iIndex];
}

#endif
