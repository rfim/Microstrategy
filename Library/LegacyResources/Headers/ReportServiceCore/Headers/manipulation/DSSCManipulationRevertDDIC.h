//
//  DSSCManipulationRevertDDIC.h
//  ReportServiceCore
//
//  Created by Zeng Jinliang on 6/25/14.
//  Copyright (c) 2014 MicroStrategy, Inc. All rights reserved.
//
//  Description: This Manipulation is used to revert DDIC recursive. Only clear the same fieldgroup node's control.

#ifndef CManipulation_Revert_DDIC_H
#define CManipulation_Revert_DDIC_H

#include "DSSCManipulation.h"

class DSSActionObject;
class DSSRWNode;

class DSSCManipulationRevertDDIC : public DSSCManipulation
{
public:
    DSSCManipulationRevertDDIC(DSSRWNode* ipNode, DSSRWControl* ipControl, const std::vector<std::string>& iRevertFieldVector);
    DSSCManipulationRevertDDIC(DSSActionObject* ipAO, DSSObjectContext* ipContext);
	virtual ~DSSCManipulationRevertDDIC();
	
    int Execute();
    
    DSSRWNode* getNode() { return mpNode; }
    bool isFieldGroupNode() { return mIsFGNode; }
    const std::vector<std::string>& getDiscardTransactionFields() { return mDiscardTransactionFields; }
    
    // Get field key vector by control link; original output must initialize by the caller.
    static int getRevertFieldVector(DSSRWNode* ipNode, DSSRWControl* ipControl, std::vector<std::string>& oRevertFieldVector);

private:
    int hRevertFieldDDIC();
    int hRevertDDIC(DSSRWControl* ipControl);
    int hRevertDDICTargetControl(DSSRWControl* ipControl);
    std::string hBuildClearElementsXML();

private:
	DSSRWNode* mpNode;
    std::vector<std::string> mProcessedDDICKeys;
    bool mHasUC;
    bool mIsFGNode;
    std::vector<std::string> mDiscardTransactionFields;
};

#endif /* defined(CManipulation_Revert_DDIC_H) */
