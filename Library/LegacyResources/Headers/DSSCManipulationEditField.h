//
//  DSSCManipulationEditField.h
//  ReportServiceCore
//
//  Created by Zhu, Wei on 12/26/12.
//
//

#ifndef ReportServiceCore_DSSCManipulationEditField_h
#define ReportServiceCore_DSSCManipulationEditField_h

#include "DSSCManipulationAddRWNode.h"
#include "DSSRWFieldGroupNode.h"

class DSSCManipulationEditField: public DSSCManipulationAddRWNode
{
public:
    DSSCManipulationEditField(DSSRWNode* ipNode, std::string iFieldName, EnumDSSRWFieldType iFieldType, std::string& iFieldKey, std::string& iText);
    DSSCManipulationEditField(DSSActionObject* ipAO, DSSObjectContext* ipContext);
    ~DSSCManipulationEditField();
    
    bool EditField();
    int Execute();
    void GenerateAODeltaXML();
#ifndef __ANDROID__
    void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
    
private:
    // DSSRWNode* mpNode;
    DSSFields* mpFields;
    DSSField* mpField;
    std::string mFieldKey;
    EnumDSSRWFieldType mFieldType;
    std::string mFieldName;
    std::string mText;
};

#endif
