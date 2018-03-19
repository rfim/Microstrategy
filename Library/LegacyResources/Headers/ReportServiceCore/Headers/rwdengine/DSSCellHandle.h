/*
 *  DSSCellHandle.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 3/26/10.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */
#ifndef CELL_HANDLE_H
#define CELL_HANDLE_H
#import "DSSrwdconstants.h"
class DSSTemplateUnit;
class DSSTabularUnit;
class DSSBaseElementProxy;

class DSSCellHandle
{
public:
	DSSCellHandle();
	
	DSSCellHandle(EnumDSSCellHandleType iType, int val);
	
	DSSCellHandle(EnumDSSCellHandleType iType, DSSTabularUnit *ipXTabUnit, int iOrdinal, int iKey, int iFormId = 0);
	
	DSSCellHandle(EnumDSSCellHandleType iType, DSSTemplateUnit *ipTemplateUnit, int val);
	
	/*
	 compare if two cell handle are equal. The difference between "IsStrictEqual" is:
	 for DssCellHandleXTabHeaderElement: it only checks element key, ignores the oridnal number
	 */
	bool IsEqual(const DSSCellHandle &irObj);	
	bool IsStrictEqual(const DSSCellHandle &irObj);
	
	EnumDSSCellHandleType type;
	union value{
		int ordinal;					//DssCellHandleXTabHeaderElement
		int cell;						//DssCellHandleXTabData
		//int unitDummyID;				//DssCellHandleTemplateUnit
		int form;						//DssCellHandleTemplateUnitAndForm
		int depth;						//DssCellHandleCustomGroupAndDepth
		int field;						//DssCellHandleField
		int controlEle;					//DssCellHandleControl
		//int groupbyDummy;				//DssCellHandleGroupByElement
		int paddingL;					//DssCellHandlePaddingLeft
		int paddingU;					//DssCellHandlePaddingUp
	} v;
	
	union pointer{
		DSSTabularUnit *xtabUnit;			//DssCellHandleXTabHeaderElement
		//void*	pCellDummy;				//DssCellHandleXTabData
		DSSTemplateUnit *unit;				//DssCellHandleTemplateUnit
		//DssCellHandleTemplateUnitAndForm
		//DssCellHandleCustomGroupAndDepth
		//void *pFiledDummy;			//DssCellHandleField
		//DSSBaseElementProxy *element;	//DssCellHandleControl
		//DssCellHandleGroupByElement
		//void *pPaddingDummy;			//DssCellHandlePaddingLeft
		//void *pPaddingDummy;			//DssCellHandlePaddingUp
	} p;
	
	union extend{
		int key;						//DssCellHandleXTabHeaderElement
	} x;
	
	int formId;							//DssCellHandleXTabHeaderElement, baseformId
};
#endif