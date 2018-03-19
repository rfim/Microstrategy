//
//  DSSCManipulationSetCurrentControlElementStatus.h
//  ReportServiceCore
//
//  Created by Zhu, Wei on 12/21/12.
//
//

#ifndef ReportServiceCore_DSSCManipulationSetCurrentControlElementStatus_h
#define ReportServiceCore_DSSCManipulationSetCurrentControlElementStatus_h

#include "DSSRWControl.h"
#include "DSSCManipulation.h"
#include "DSSRWTemplateNode.h"

class DSSCManipulationSetCurrentControlElementStatus : public DSSCManipulation
{
private:
	EnumDSSCurrentElementStatus mStatus;
    DSSRWTemplateNode* mpTemplateNode;
    DSSRWControl* mpControl;
    std::string mControlKey;
    
    void GenerateAODeltaXML();
    
#ifndef __ANDROID__
	void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
	
public:
	DSSCManipulationSetCurrentControlElementStatus(DSSRWTemplateNode* iNode, std::string& iControlKey, EnumDSSCurrentElementStatus iStatus);
	DSSCManipulationSetCurrentControlElementStatus(DSSActionObject* ipAO, DSSObjectContext* ipContext);
	virtual ~DSSCManipulationSetCurrentControlElementStatus();
	int Execute();
};


#endif
