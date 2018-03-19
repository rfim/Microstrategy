//
//  StdAfx.h
//  DFCEngineDaemon
//
//  Created by ydong on 12/18/12.
//  Copyright (c) 2012 MicroStrategy, Inc. All rights reserved.
//

#ifndef DFCEngineDaemon_PCH
#define DFCEngineDaemon_PCH



//#pragma warning(disable:4275 4146)
//#pragma warning(disable:4284)			// infix notation for std::list
//#pragma warning(disable:4018)			// signed / unsigned mismatch
//#pragma warning(disable:4244)			// conversion from double to int (loss of data)

// Define COM interface headers from a type library
#include "DSSCOMMaster/DSSCOM_TLB_INC.H"

//PDCheader
#include "PDCHeader/PDCwindows.h"
#include "PDCHeader/PDCstring"
#include "PDCHeader/PDCalgorithm"

// from Base
#include "Base/Defines/BasicTypes.h"

#include "Base/Base/Buffer.h"
#include "Base/Base/String.h"
#include "Base/Base/Allocator.h"
#include "Base/Base/NewOperator.h"

// for using STL
#include "DataStructure/map.h"
#include "DataStructure/vector.h"

// for error messages
#include "DssSrvrMsgs/DSSSrvrMsgs.h"


// from Includes
#include "Includes/DFCComutil_Includes.h"
#include "Includes/Globals.h"
#include "Includes/DFCLog.h"
#include "Includes/DFCStream.h"

// class DFCParseData;

//// Attribute
//// Note: Attribute must be included in front of AttrFormSet
//#include "Attribute/DFCRoleType.h"
//#include "Attribute/DFCAttr.h"
//#include "Attribute/DFCRoleType.h"
//#include "Attribute/DFCAttrGroup.h"
//#include "Attribute/DFCAttrDefUnit.h"
//#include "Attribute/DFCAttrDef.h"
//#include "Attribute/DFCDynamicAttr.h"
//#include "Attribute/DFCTotalDimAttr.h"
//#include "Attribute/DFCMultiUseAttr.h"
//#include "Attribute/DFCNormAttr.h"
//#include "Attribute/DFCTransAttr.h"
//#include "Attribute/DFCRoleAttr.h"
//#include "Attribute/DFCIndepAttr.h"
//#include "Attribute/DFCAttrInfo.h"
//#include "Attribute/DFCRole.h"
//
//// AttrFormSet
//#include "AttrFormSet/DFCTemplateUnitDisp.h"
//#include "AttrFormSet/DFCAttrCol.h"
//#include "AttrFormSet/DFCTemplateUnitDisp.h"
//#include "AttrFormSet/DFCAttrDisp.h"
//#include "AttrFormSet/DFCAttrFormSetExt.h"
//#include "AttrFormSet/DFCForm.h"
//#include "AttrFormSet/DFCFormMap.h"
//#include "AttrFormSet/DFCAttrFormSet.h"
//#include "AttrFormSet/DFCAttrInfoBase.h"
//
//
//
//// Column
#include "Column/DFCDataType.h"
//#include "Column/DFCCol.h"

// DataStructur
#include "DataStructure/DFCBitVector.h"
#include "DataStructure/DFCBitVectorBase.h"
#include "DataStructure/DFCBitMatrix.h"
#include "DataStructure/DFCCompoundBitVector.h"
//#include "DataStructure/DFCSQLControl.h"
#include "DataStructure/DFCString.h"
#include "DataStructure/DFCStringRes.h"
#include "DataStructure/DFCStringResVec.h"
#include "DataStructure/DFCStringResInternal.h"
#include "DataStructure/DFCStringResRotate.h"
#include "DataStructure/DFCStringResString.h"
#include "DataStructure/DFCStringResStringVec.h"
#include "DataStructure/DFCStringMapSet.h"
//#include "DataStructure/DFCToken.h"


//// Dimension
//#include "Dimension/DFCDim.h"
//#include "Dimension/DFCDim.h"
//#include "Dimension/DFCDimSystem.h"
//
//// EngineMsg
//#include "EngineMsg/EngineMsg.h"
//
//// Fact
//#include "Fact/DFCDimtyExt.h"
//#include "Fact/DFCDimtyExtUnit.h"
//#include "Fact/DFCFact.h"
//#include "Fact/DFCFactGroup.h"
//#include "Fact/DFCFactInfo.h"
//#include "Fact/DFCVirtualFact.h"

// Function
#include "Function/DFCFunc.h"
#include "Function/DFCAggFunc.h"
#include "Function/DFCFuncSimple.h"
#include "Function/DFCSimpleRelativityFunc.h"

// Misc
#include "Misc/AutoPointers.h"
//#include "Misc/CommonFunc.h"

//// ObjInfo
//#include "ObjInfo/DFCObjInfo.h"
//
//// Project
//#include "Project/FuncContainer.h"
#include "Includes/DFCSchemaInterface.h"
//#include "Project/DFCProjectSchema.h"
//#include "Project/DFCSchema.h"
//#include "Project/DFCParseData.h"

// Relation
//#include "Relation/DFCCorrAttrAssoc.h"
//#include "Relation/DFCCorrAttrAssocVec.h"
//#include "Relation/DFCRel.h"

// ReportDisp
#include "ReportDisp/DFCHeadUnitInfo.h"
#include "ReportDisp/DFCSelectUnitInfo.h"
#include "ReportDisp/DFCHeadInfo.h"

//// SchemaExpr
//#include "SchemaExpr/DFCExprPos.h"
//#include "SchemaExpr/DFCExprRange.h"
//#include "SchemaExpr/DFCTableLkup.h"
//#include "SchemaExpr/DFCFactLkup.h"
//#include "SchemaExpr/DFCExprTrueCol.h"
//#include "SchemaExpr/DFCSchemaExpr.h"
//#include "SchemaExpr/DFCSchemaExprFact.h"
//#include "SchemaExpr/DFCSchemaExprFunc.h"
//#include "SchemaExpr/DFCSchemaExprString.h"
//
//#include "SEH/DFCComUtilException.h"
//#include "SEH/DFCPopuException.h"
//#include "SEH/DFCComUtilException.h"
//#include "SEH/DFCErrParameter.h"
//#include "SEH/DFCException.h"
//#include "SEH/DFCExceptionObj.h"
//#include "SEH/DFCSchemaLogicException.h"
//#include "SEH/DFCSchemaException.h"

// SimpleExpr
#include "SimpleExpr/DFCSimpleExpr.h"
#include "SimpleExpr/DFCSimpleExprDatasetID.h"
#include "SimpleExpr/DFCSimpleExprBranch.h"
#include "SimpleExpr/DFCSimpleExprConst.h"
#include "SimpleExpr/DFCSimpleExprPos.h"
#include "SimpleExpr/DFCSimpleExprUnit.h"

// StringSubst
#include "StringSubst/DFCStringSubst.h"

//// Table
//#include "Table/DFCTable.h"
//#include "Table/DFCPseudoTable.h"
//#include "Table/DFCExtTable.h"
//#include "Table/DFCTableI.h"
//#include "Table/DFCTableSTLSet.h"
//#include "Table/DFCVFTable.h"

// VLDB Related
#include "VLDBManager/VLDBInfo.h"
#include "VLDBServer/DataTypeMapping/DSSDataTypeConst.h"
#include "GlobalOptimization/DFCGOHelper.h"

#define CHECK_NULL_ABORT(x)	{                               \
    if(x==NULL)                                             \
    {                                                       \
        _ASSERT(x);                                         \
        throw DFCSchemaException(                           \
            LOGIC_EXCEPTION,                                \
            GetMsgFromID(DFC_ENG_NULL_POINTER),             \
            SCHEMA_OTHER_OBJ,                               \
            (void *) this                                   \
        );                                                  \
    }                                                       \
}


#define ATTR_BLOCK_HEADER short(0x0000)
#define FORM_BLOCK_HEADER short(0x0001)
#define TABLE_BLOCK_HEADER short(0x0002)
#define DIM_BLOCK_HEADER short(0x0003)
#define DBROLE_BLOCK_HEADER short(0x0004)
#define DATABASE_BLOCK_HEADER short(0x0005)
#define COL_BLOCK_HEADER short(0x0006)
//7 is reserved for function
#define ROLE_BLOCK_HEADER short(0x0008)


#endif
