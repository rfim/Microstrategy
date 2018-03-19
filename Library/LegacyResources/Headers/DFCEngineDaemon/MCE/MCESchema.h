//==============================================================================================
//	FILENAME	:	MCESchema.h
//  DESCRIPTION	:	This class creates a new schema object which is much smaller than ProjectSchema.
//					MCESchema only contains attributes from cubes, attributes outside cubes are not
//					included. MCESchema is mainly designed for relation generation.
//	AUTHOR		:	zhwang
//	CREATION	:	19/04/10
//	Copyright (C) MicroStrategy Incorporated 2010
//	All Rights Reserved
//==============================================================================================

#ifndef MCESchema_h
#define MCESchema_h 1

#include "MCE/MCEDataType.h"
#include "MCE/MCEAttr.h"
#include "Includes/DFCSchemaInterface.h"
class DFCAttr;
//class SchemaLoadIndex;

typedef vector<DFCBitVectorBase *, MBase::Allocator<DFCBitVectorBase *> >  VecBitVec;
typedef enum {
	MCEDownward	=	0,				//from ancestor to descendant 
	MCEUpward		=	1,			//from descendant to ancestor	
	MCEDownwardThenUpward	=	2,	//first from ancestor to descendant, then from descendant to ancestors
	MCENoDirection	=	3			//no join path, not suitable for join
} EnumMCEJoinDirection;

typedef enum {
	MCENoRelation = 0,				//two attributes in different hierarchies
	MCEAncestor = 1,				//attr1 is the 1:M ancestor of attr2
	MCEDescendant = 2,				//attr1 is the M:1 descendant of attr2
	MCEM2M = 3,						//two attributes of M:M relation
	MCESibling = 4,					//two attributes share a same descendant
} EnumMCEAttrRelation;

#define XMCEAncestor 0x00000001     //     000001
#define XMCEDescendant 0x00000002   //     000010
#define XMCEM2M 0x00000004          //     000100  
#define XMCESibling 0x0000008		//     001000
#define XMCENoRelation 0x00000010   //     010000

class MCESchema : public DFCSchemaInterface
{
public:
    MCESchema(MBase::Buffer* ipBuffer);
    ~MCESchema();


	//init the member variables
	HRESULT InitSchema();

	//hxiao 2012-10-19: init the local or project relation vector
	HRESULT InitLocalOrProjRelVector();
	//hxiao 2012-10-20: handle the no_relation local relationship
	HRESULT HandleNoLocalRel(const DSS_ID &iChildID, const DSS_ID &iParentID);
	//hxiao 2012-09-25: handle the MM local relationship
	HRESULT HandleMMLocalRel(const DSS_ID &iChildID, const DSS_ID &iParentID);
	//hxiao 2012-09-25: handle the 1M OR 11 local relationship
	HRESULT Handle1MOr11LocalRel(const DSS_ID &iChildID, const DSS_ID &iParentID);
	//hxiao 2012-10-20: check whether iParent is the direct parent of Child;
	void IsDirectRel(Int32 iChildIndex, Int32 iParentIndex, DFCVecIntVec * iRefChildrenVec,bool &oIsDirectRel);
	//hxiao 2012-11-07: remove the redundant ref edge in the MCESchema which may get from the project schema
	HRESULT RemoveRedundantRefEdge();

	//zhwang@2011-02-16
	HRESULT GetClosestChild(const DSS_ID iAttrID, MCEDSSIDVec* iAttrs, DSS_ID &oAttrID, bool iOnlyOne2MRelationFlag = false);
	HRESULT GetClosestChild(const DSS_ID iAttrID, const int iAttrNum, const DSS_ID *ipAttrArr, DSS_ID &oAttrID, bool iOnlyOne2MRelationFlag = false);
	HRESULT GetClosestParent(const DSS_ID iAttrID, MCEDSSIDVec* iAttrs, DSS_ID &oAttrID,  bool iOnlyOne2MRelationFlag = false);
	HRESULT GetClosestParent(const DSS_ID iAttrID, const int iAttrNum, const DSS_ID *ipAttrArr, DSS_ID &oAttrID, bool iOnlyOne2MRelationFlag = false);
	HRESULT GetClosestAttr(const DSS_ID iAttrID, const int iAttrNum, const DSS_ID *ipAttrArr, DSS_ID &oAttrID, bool iOnlyOne2MRelationFlag = false, bool iParent2Child = true);
	HRESULT GetClosestSibling(const DSS_ID iAttrID, MCEDSSIDVec *iAttrs, DSS_ID &oAttrID);
	//zhwang@2011-11-15
	HRESULT GetClosestAttrs(const DSS_ID iAttrID, const int iAttrNum, const DSS_ID *ipAttrArr, MCEDSSIDVec &oAttrIDs, bool iOnlyOne2MRelationFlag = false, bool iParent2Child = true);

	// get all the template attributes for one specific cube or report instance. 
	// no custom groups and consolidations
	HRESULT GetTemplateAttrs(Int32 iCubeIndex, DFCAttrGroup &oAG);

	HRESULT AddAttributesFromOneCube(MCEAttrVec *ipAttrSet, DSS_ID &iID, DFCString *ipCubeName);

	//determine the relationship between two attributes. Dest is the child? parent ? or ? of Src
	EnumMCEAttrRelation DetermineRelationship(MCEAttr *ipSrc, MCEAttr *ipDest);
	//the index here is the MCE index
	EnumMCEAttrRelation DetermineRelationship(Int32 iSrcIndex, Int32 iDestIndex,bool isIgnoreSibling = false);
	EnumMCEAttrRelation DetermineRelationship(DSS_ID &iSrc, DSS_ID &iDest);

	//optimize to reduce the size of graph, this is useful only for Siblings and 1:M relationship
	HRESULT OptimizationForGraph(MCEAttr * ipSrc, MCEAttr * ipDest, DFCBitVector *ipOutOfRangeAttrBV);

	//Assume A - S1 - B, we must find whether there exists C such that A - S1 - C, B - S1 - C
	//if so, return true, else return false
	bool IsStrongSibling(MCEAttr *ipAttr1, MCEAttr *ipAttr2, MCEIntSet *ipDataSets);
	bool IsStrongSibling(Int32 iAttrNbr1, Int32 iAttrNbr2, MCEIntSet *ipDataSets);

	// if two Attributes in the same cube, return true, else return false;
	bool IsTwoAttrsInSameCube(MCEAttr *ipAttr1, MCEAttr *ipAttr2);
	// find all the cubes containing the two attributes at the same time;
	HRESULT CubesContainingTwoAttrs(MCEAttr *ipAttr1, MCEAttr *ipAttr2, MCEIntSet *iopResultSet);

	DFCBitVector *GetReducibleAttrBV();

	MCEAttrVec *GetAttributes();

	//get the attribute via its MCE index.
	MCEAttr *GetAttr(Int32 iIndex);
	//get the attribute via DFCAttr
	MCEAttr *GetAttr(DFCAttr *ipAttr);
	//get MCE attribute via DSS_ID
	MCEAttr *GetAttr(const DSS_ID &iID);
	// get DFCAttr via DSS_ID
	DFCAttr *GetDFCAttr(const DSS_ID &iID);
	// for a attribute with DSS_ID iID, to see whether it is a normal attribute
	// or custom group or consolidation element.
	bool IsConORCG(const DSS_ID &iID);

	MBase::ReturnWString GetAttrName(const DSS_ID &iID);

	// get the Cube name via its index (starting from 0)
	DFCString *GetCubeName(Int32 iIndex);
	DSS_ID GetCubeID(Int32 iIndex);

	//one input is DSS_ID of DFCAttr, and map the DSS_ID to ipMCEAttr.
	HRESULT SetDFC2MCEMap(DSS_ID &iID, MCEAttr *ipMCEAttr);
	HRESULT SetDSSID2DFCAttrMap(DSS_ID &iID, DFCAttr *ipDFCAttr);
	HRESULT AddMCEAttr(MCEAttr *ipMCEAttr);

	// This function returns the index of MCEAttr which contains DFCAttr *ipAttr.
	Int32 FindMCEIndex(const DFCAttr *ipAttr);

	//read the attributes from all the cubes and find all the reducible attributes.
	HRESULT CalculateReducibleAttrs();

	// Called by JoinPathGenerator class
	MCEAttrVecVec *GetMCEAttrVecVec();

	//only for test
	void Print1MDescendant(ostream& iOstream);
	void PrintM1Ancestor(ostream& iOstream);

	//only for test, should be commented out later
	void SetDescendantBV(VecBitVec *ipVecBV);
	void SetAncestorBV(VecBitVec *ipVecBV);
	void SetMMDescendantBV(VecBitVec *ipVecBV);
	void SetMMAncestorBV(VecBitVec *ipVecBV);
	void SetMCEAttrVecVec(MCEAttrVecVec *ipMCEAttrVecVec); 
	void SetMCEAttrVec(MCEAttrVec *ipMCEAttrVec);

	// ywu, 2011.1.27. find out lowest level attributes in iAttrs, and put them in ioLowestAttrs
	void FindLowestAttrs(MCEDSSIDVec* iAttrs, GUIDSet& ioLowestAttrs);
	void FindLowestAttrs(GUIDSet* iAttrs, GUIDSet& ioLowestAttrs);

	void FindLowerAttrs(DSS_ID iAttrID, MCEDSSIDVec* iAttrs, GUIDSet& ioLowerAttrs);

	// ywu, 2011.1.27. find out all attributes in iAttrs which are ancestors of attributes in ioLowestAttrs, 
	// and store them in ioLowestAttrs
	void FindAllAncestors(GUIDSet& iAttrs, GUIDSet& ioLowestAttrs, GUIDSet& oAncestors);

	HRESULT DimGroupingCalculation();

    DFCFunc* GetFuncSUM() { return NULL; }
    DFCFunc* GetFuncIN() { return NULL; }
	DFCFunc* GetFuncAND() { return NULL; }
	DFCFunc* GetFuncOR() { return NULL; }
	DFCFunc* GetFuncNot() { return NULL; }
	DFCFunc* GetFuncEQUAL() { return NULL; }

	DFCForm* GetAttrForm(DSS_ID& iFormID) { return NULL; }
	DFCFunc* GetFunc(DSS_ID& iFuncID) { return NULL; }
	DFCFact* GetFact(DSS_ID& iFactID) { return NULL; }



	//zhwang@2011-07-27
	/*
	 * Assume the hierarchy is as follows:
	 *	A
	 *  | \
	 *	B  C
	 *  |  |
	 *	|  D
	 *	|  |
	 *	| /
	 *	E
	 *	The output is A, B, C, D, E
	 */
	HRESULT TopologicalSort(MCEDSSIDVec &ioVector, bool iFromParent2Child = true);
	HRESULT TopologicalSort(MCEIntVec &ioVector, bool iFromParent2Child = true);

	// transform the attribute group 2 bit vector
	HRESULT AttrGroup2DFCBitVector(MBase::Buffer* ipBuffer, DFCAttrGroup &iAG, DFCBitVector **oppAttrsBV);
	HRESULT MCEDSSIDVec2DFCBitVector(MBase::Buffer *ipBuffer, MCEDSSIDVec *ipAttrs, DFCBitVector **oppAttrsBV);
	HRESULT GUIDSet2DFCBitVector(MBase::Buffer *ipBuffer, GUIDSet &ipAttrs, DFCBitVector **oppAttrsBV);
	HRESULT GUIDSet2MCEIntSet(GUIDSet &ipAttrs, MCEIntSet &oAttrSet);

	HRESULT UnionAllDescendants(MBase::Buffer *ipBuffer, DFCAttrGroup &iAG, DFCBitVector **oppAttrsBV);
	HRESULT GetAllCommonDescendants(MBase::Buffer *ipBuffer, MCEIntVec &iAttrs, DFCBitVector **oppAttrsBV);
	HRESULT UnionAllAncestors(MBase::Buffer *ipBuffer, DFCAttrGroup &iAG, DFCBitVector **oppAttrsBV);

	HRESULT NewEmptyBVs(MBase::Buffer *ipBuffer, DFCBitVector **oppBV);



	/*
	 * zhwang@2012-6-16, helper function for join of two cubes
	 * It mainly solves the problems as follows:
	 * for hierechy like:
	 *      D
	 *      |
	 *      A
	 *    /  \
	 *   B    C
	 * If B and C are known lowest attributes, 
	 * try to find A in order to generate big relation [B, A, C]
	 * instead of big relation [A, B, C, D]
	 *
	 * Another example:
	 *      E
	 *    /   \
	 *  F      D
	 *  |      |
	 *  H      A
	 *        /  \
	 *       B    C
	 * C0: H, F
	 * C1: E, D, B, C
	 * big relation: [H, E, D, B, C] will be generated.
	 * 
	 * The function returns the lowest parents so that
	 * the big relation could be successfully generated and the big relation is smallest
	 * among all possible choices.
	 */
	HRESULT CalculateJoinClosure(DFCBitVector *ipLowestAttrsBV1, DFCBitVector *ipLowestAttrsBV2, MCEDSSIDVec *ipAttrsUnion, MBase::Buffer *ipBuffer, GUIDSet *opAttrSet);

	/*
	 * If there exists attribute in iAttrs such that iID is the parent of it
	 * return true;
	 */
	HRESULT ExistsChild(DSS_ID &iID, MCEDSSIDVec &iAttrs, bool &oExists);

	/* thuang@2012-11-28
	 * If there exists attribute in iAttrs such that iID is the child of it
	 * return true;
	 */
	HRESULT ExistsParent(DSS_ID &iID, GUIDSet &iAttrs, bool &oExists, bool iOnlyOne2MRelationFlag = true);

	/*
	 * Test whether the attribute is M2M, M:1, 1:M, sibling related to some attribute in the set
	 */
	HRESULT IsRelated(DSS_ID &iID, MCEDSSIDVec &iAttrs, bool &oRelated);
	HRESULT IsRelated(DSS_ID &iID, GUIDSet &iAttrs, bool &oRelated);
	// only calculate the relation status between attributes in the mask
	HRESULT GetRelations(Int32 iIndexA, MCEIntSet &iAttrs, Int32 &oRelatedStatus, DFCBitVector *ipMask = NULL);

	Int64 EstimateSize();
    HRESULT UnserializeForSmartClient(ICDSSReadBlockStream* ipReadBlockStream);
    static MCESchema* UnserializeForSmartClient(ICDSSReadBlockStream* ipReadBlockStream, MBase::Buffer* ipBuffer);
private:

	HRESULT IsAllAttrsShareACommonChild(MCEIntSet *ipAttrs, MBase::Buffer *ipBuffer, bool &oShareCommonChild);
	// Currently, we just support union 1:M(ancestor) or M:1(descendants) bitvectors.
	HRESULT UnionAllBVs(MBase::Buffer *ipBuffer, DFCAttrGroup &iAG, EnumMCEAttrRelation iType, DFCBitVector **oppAttrsBV);

	// if iParent2Child is set to true, then we calculate the length of the path from Parent to Child via BFS search of the hierarchy
	// if it is false, we calculate the case when the length of path is from child to parent.
	// if iOnlyOne2MRelationFlag is set to true, we only care about 1:M relation (strong relation)
	// if iOnlyOne2MRelationFlag is set to false, we care about 1:M relation && M:M relation
	void CalculateShortestPath(Int32 iIndexParent, Int2IntMap &iMCEIndex2DistanceFromAttr, bool iOnlyOne2MRelationFlag, bool iParent2Child = true, DFCBitVector *ipAttrsBV = NULL);

    HRESULT FindDimClosure(Int32 iDimIndex, Int32 iIndex, DFCBitVector &ioVisited);

	//thuang@2012-11-11 only for vector<DFCBitVector*>
	Int64 EstimateSize4VecBitVec(VecBitVec *ipVBV);

	Int64 EstimateSize4DFCVecIntVec(DFCVecIntVec *ipVBV);

	//hxiao 2012-10-22: add for merging local relationship 
	void  hDeleteDirect1MRel(Int32 iChildIndex, Int32 iParentIndex);
	void  hDeleteDirectMMRel(Int32 iChildIndex, Int32 iParentIndex);
	void  hSetDirect1MRel(Int32 iChildIndex, Int32 iParentIndex);
	void  hSetDirectMMRel(Int32 iChildIndex, Int32 iParentIndex);
	//hxiao 2012-09-25: populate the local relationship edge
	void  AddEdge(Int32 iChildIndex, Int32 iParentIndex, VecBitVec * ioDescendentsBVs, VecBitVec * ioAncestorsBVs);
	//hxiao 2012-09-25: populate the local relationship immediate edge
	void  AddRefEdge(Int32 iChildIndex, Int32 iParentIndex,DFCVecIntVec * ioRefChildrenVec, DFCVecIntVec * ioRefParentsVec, VecBitVec * iDescendentsBVs, VecBitVec * iAncestorsBVs);
	void  DeleteEdge(Int32 iChildIndex, Int32 iParentIndex, DFCVecIntVec * iRefChildrenVec, DFCVecIntVec * iRefParentsVec, VecBitVec * ioDescendentsBVs, VecBitVec * ioAncestorsBVs);
	//hxiao 2012-10-22: populate the local relationship immediate edge
	void  DeleteRefEdge(Int32 iChildIndex, Int32 iParentIndex, DFCVecIntVec * ioRefChildrenVec, DFCVecIntVec * ioRefParentsVec);
	//hxiao 2012-11-07: remove the redundant ref edge in the MCESchema which may get from the project schema
	void hRemoveRedundantRefEdge(VecBitVec * iDescendentsBVs, VecBitVec * iAncestorsBVs,DFCVecIntVec * ioRefChildrenVec, DFCVecIntVec * ioRefParentsVec);
	//hxiao 2012-11-26:re-compute the descendants info of B and B's ancestors(union of its direct child)
	void hReComputeBVByRef(Int32 iIndex, DFCVecIntVec *iRefChildrenVec, DFCVecIntVec * iRefParentsVec, VecBitVec * ioDescendentsBVs);
	//hxiao 2012-11-26: check the indirect path for the circle case when removing the redundant ref edge
	bool hHasIndirectPath(Int32 iChildIndex, Int32 iParentIndex, DFCVecIntVec * iRefChildrenVec);



private:
	//Set of attributes each of which only shows once in all datasets, this is for graph generation later.
	DFCBitVector *mpReducibleAttrBV;

	//vectors of attribute vectors, and attributes from one vector are read from the same cube.
	//there is a simple mapping: mpMCEAttrVecVec[i] refers to the attributes in cube[i].
	MCEAttrVecVec *mpMCEAttrVecVec;	

	//map the DFCAttr DSSID to MCEAttr *, this variable contains all the MCEAttrs from all cubes
	//zhwang@2010-12-08: map the Consolidation or Custom Group DSSID to MCEAttr *(fake MCE Attribute)
	DSSID2MCEAttrMap *mpDFCAttr2MCEAttrMap;
	// this map only contains CG & CON
	DSSID2DFCAttrMap *mpDSSID2DFCAttrMap;

	//vector of MCEAttrs, with MCE Index from 0 to N in order. So no map is needed!
	MCEAttrVec *mpMCEAttrVec;

	//vector of Cube IDs in order, the ith Cube's id is mpCubeIDVec[i]
	MCEDSSIDVec * mpCubeIDVec;
	//vector of Cube Names in order, the ith Cube's id is mpCubeNameVec[i]
	MCEDFCStringVec *mpCubeNameVec;


    //MBase::Buffer *mpBuffer;


public:
	DFCAttr* FindAttr (Int32 iNbr);
	DFCAttr* FindAttr (DFCString& iName);
    // Additional Public Declarations
	DFCBitVectorBase * GetDescendentsBVOfAttr (const DFCAttr *ipAttr);
	DFCBitVectorBase * GetDescendentsBVOfAttr (const Int32 iMCEIndex);
	DFCBitVectorBase * GetAncestorsBVOfAttr (const DFCAttr *ipAttr);
	DFCBitVectorBase * GetAncestorsBVOfAttr (const Int32 iMCEIndex);
	DFCBitVectorBase * GetPartnersBVOfAttr (const DFCAttr *ipAttr);
	DFCBitVectorBase * GetParDescendentsBVOfAttr (const DFCAttr *ipAttr);
	DFCBitVectorBase * GetParAncestorsBVOfAttr (const DFCAttr *ipAttr);
	DFCBitVectorBase * GetParPartnersBVOfAttr (const DFCAttr *ipAttr);
	DFCBitVectorBase * GetMMDescendentsBVOfAttr (const DFCAttr *ipAttr);
	DFCBitVectorBase * GetMMAncestorsBVOfAttr (const DFCAttr *ipAttr);
	DFCBitVectorBase * GetMMPartnersBVOfAttr (const DFCAttr *ipAttr);

	DFCBitVectorBase * GetMMDescendentsBVOfAttr (const Int32 iMCEIndex);
	DFCBitVectorBase * GetMMAncestorsBVOfAttr (const Int32 iMCEIndex);

	DFCVecIntVec * GetRefChildrenVec();

	//hxiao 2012-10-25 begin :help function
	DFCIntVec * GetRefChildrenVecOfAttr (const DFCAttr *ipAttr);
	DFCIntVec * GetRefChildrenVecOfAttr (const Int32 iMCEIndex);
	DFCIntVec * GetRefParentsVecOfAttr (const DFCAttr *ipAttr);
	DFCIntVec * GetRefParentsVecOfAttr (const Int32 iMCEIndex);

	DFCIntVec * GetRefMMChildrenVecOfAttr (const DFCAttr *ipAttr);
	DFCIntVec * GetRefMMChildrenVecOfAttr (const Int32 iMCEIndex);
	DFCIntVec * GetRefMMParentsVecOfAttr (const DFCAttr *ipAttr);
	DFCIntVec * GetRefMMParentsVecOfAttr (const Int32 iMCEIndex);
    
    //creates an empty BitVector of the appropriate size and type for this schema
    DFCBitVectorBase * NewEmptyBV(MBase::Buffer* iBuffer);
    DFCBitVectorBase * NewEmptyBV(Int32 iLength, MBase::Buffer* iBuffer);
    DFCBitVectorBase * NewBV(const DFCBitVectorBase * iInput, MBase::Buffer* iBuffer) { return NULL; };
    
	//print function for debugging
	void PrintAllBitVectors(ostream& iOstream);
	//hxiao 2012-10-25 end :help function

	Int32 GetNumAttr ();
public:
    virtual void AddLocale() {}
    virtual void CheckLocaleInSchema()              {}
    virtual bool ReplaceAttrIfExisted(DFCAttr* ipAttr)          { return NULL; }
    virtual DFCCol* FindCol(DFCString&)             { return NULL; }
    virtual DFCCol* FindCol(Int32)                  { return NULL; }
    virtual DFCDBRole* FindDBRole (Int32 iNbr)      { return NULL; }
    virtual DFCDBRole* FindDBRole (DFCString& iName){ return NULL; }
    virtual DFCDBRole* FindDBRole(DSS_ID* iID)      { return NULL; }
    virtual DFCDatabase* FindDatabase (Int32 iNbr)  { return NULL; }
    virtual DFCDatabase* FindDatabase (DFCString& iName)        { return NULL; }
    virtual DFCDim* FindDim (DFCString& iName)      { return NULL; }
    virtual DFCDim* FindDim (Int32 iNbr)            { return NULL; }
    virtual DFCFact* FindFact (DFCString& iName)    { return NULL; }
    virtual DFCFact* FindFact (Int32 iNbr)          { return NULL; }
    virtual DFCFactGroup* FindFactGroup (DFCString& iName)      { return NULL; }
    virtual DFCFactGroup* FindFactGroup (Int32 iNbr){ return NULL; }
    virtual DFCFunc* FindFunc (DFCString& iName)    { return NULL; }
    virtual DFCFunc* FindFunc (Int32 iId)           { return NULL; }
    virtual DFCRole* FindRole (DFCString& iName)    { return NULL; }
    virtual DFCRole* FindRole (Int32 iNbr)          { return NULL; }
    virtual DFCTable* FindTable (DFCString& iName)  { return NULL; }
    virtual DFCTable* FindTable (Int32 iNbr)        { return NULL; }
    virtual DFCTableSource* FindTableSource (DFCString& iName)  { return NULL; }
    virtual DFCTableSource* FindTableSource (Int32 iNbr)        { return NULL; }
        
    
    virtual DFCFunc* GetFuncBETWEEN ()              { return NULL; }
    virtual DFCFunc* GetFuncCOUNT ()                { return NULL; }
    virtual DFCFunc* GetFuncGREATEQUAL ()           { return NULL; }
    virtual DFCFunc* GetFuncGREATER ()              { return NULL; }
    virtual DFCFunc* GetFuncGREATEREQUAL ()         { return NULL; }
    virtual DFCFunc* GetFuncINTERSECT ()            { return NULL; }
    virtual DFCFunc* GetFuncINTERSECTIN ()          { return NULL; }
    virtual DFCFunc* GetFuncISNOTNULL ()            { return NULL; }
    virtual DFCFunc* GetFuncISNULL (bool iTest=false) { return NULL; }
    virtual DFCFunc* GetFuncLESS ()                 { return NULL; }
    virtual DFCFunc* GetFuncLESSEQUAL ()            { return NULL; }
    virtual DFCFunc* GetFuncLIKE ()                 { return NULL; }
    virtual DFCFunc* GetFuncMAX ()                  { return NULL; }
    virtual DFCFunc* GetFuncMIN ()                  { return NULL; }
    virtual DFCFunc* GetFuncNOT ()                  { return NULL; }
    virtual DFCFunc* GetFuncNOTBETWEEN ()           { return NULL; }
    virtual DFCFunc* GetFuncNOTEQUAL ()             { return NULL; }
    virtual DFCFunc* GetFuncNOTIN ()                { return NULL; }
    virtual DFCFunc* GetFuncNOTLIKE ()              { return NULL; }
    virtual DFCFunc* GetFuncNULLTOZERO ()           { return NULL; }
    virtual DFCFunc* GetFuncRANK ()                 { return NULL; }
    virtual DFCFunc* GetFuncZEROTONULL ()           { return NULL; }
    virtual DFCFunc* GetFuncPLUS ()                 { return NULL; }
    virtual DFCFunc* GetFuncMINUS ()                { return NULL; }
    virtual DFCFunc* GetFuncTIMES ()                { return NULL; }
    virtual DFCFunc* GetFuncDIVIDE ()               { return NULL; }
    virtual DFCFunc* GetFuncTUPLE ()                { return NULL; }
    virtual DFCFunc* GetFuncLAST ()                 { return NULL; }
    virtual DFCFunc* GetFuncFIRST ()                { return NULL; }
    virtual DFCFunc* GetFuncIF (bool iTest=false)   { return NULL; }
    virtual DFCFunc* GetFuncUNION ()                { return NULL; }
    virtual DFCFunc* GetFuncEXCEPT ()               { return NULL; }
    
    virtual void PrintAttr (ostream& iOstream)      { }
    virtual void PrintCol (ostream& iOstream)       { }
    virtual void PrintDBRole (ostream& iOstream)    { }
    virtual void PrintDatabase (ostream& iOstream)  { }
    virtual void PrintDim (ostream& iOstream)       { }
    virtual void PrintFact (ostream& iOstream)      { }
    virtual void PrintFactGroup (ostream& iOstream) { }
    virtual void PrintFunc (ostream& iOstream)      { }
    virtual void PrintRole (ostream& iOstream)      { }
    virtual void PrintTable (ostream& iOstream)     { }
    virtual void PrintTableSource (ostream& iOstream) { }
    virtual void PrintAll (ostream& iOstream)       { }
    

    virtual bool HasSecAggFunc ()                   { return NULL; }
    virtual void SetHasSecAggFunc(bool iHasSecAggFunc) { }
    
    virtual Int32 GetNumCol ()                      { return NULL; }
    virtual Int32 GetNumDatabase ()                 { return NULL; }
    virtual Int32 GetNumDBRole ()                   { return NULL; }
    virtual Int32 GetNumDim ()                      { return NULL; }
    virtual Int32 GetNumFact ()                     { return NULL; }
    virtual Int32 GetNumFunc ()                     { return NULL; }
    virtual Int32 GetNumFactGroup ()                { return NULL; }
    virtual Int32 GetNumRole ()                     { return NULL; }
    virtual Int32 GetNumTable ()                    { return NULL; }
    virtual Int32 GetNumTableSource ()              { return NULL; }
    
    virtual EnumDSSDatabaseType GetDatabaseType()   { return EnumDSSDatabaseType(0); }
    
    virtual void SetDatabaseType(EnumDSSDatabaseType iDbType)   { return; };
    virtual bool IsProjectSchema()                              { return false; }
    
    virtual void SetAttrBitVectors()                            { }
    virtual void SetLocalAttrBitVectors()                       { }
    
    virtual DFCProjectSchema* GetProjectSchema()                { return NULL; }
    
    virtual void AddLocale(Int32 iLocale)                       { }
    virtual bool CheckLocaleInSchema(Int32 iLocale)             { return NULL; }
    virtual bool UseSQLDI()                                     { return NULL; }
    virtual void SetUseSQLDI(bool input)                        { }
    virtual bool GetProjectDIEnabled()                          { return NULL; }
    virtual void SetProjectDIEnabled(bool input)                { }
    virtual const set<Int32, less<Int32>, MBase::Allocator<Int32> >* GetLocales()   { return NULL; }
    virtual void UpdateDatabase ()                              { }
private:

//	VecBitVec * mpbvDescendentsBVs;
//	VecBitVec * mpbvAncestorsBVs;
//	VecBitVec * mpbvPartnersBVs;
//	VecBitVec * mpbvParDescendentsBVs;
//	VecBitVec * mpbvParAncestorsBVs;
//	VecBitVec * mpbvParPartnersBVs;
//	VecBitVec * mpbvMMDescendentsBVs;
//	VecBitVec * mpbvMMAncestorsBVs;
//	VecBitVec * mpbvMMPartnersBVs;
//
//	// to record immediate relationships
//	DFCVecIntVec * mpvRefChildrenVec;
//	DFCVecIntVec * mpvRefParentsVec;
//	DFCVecIntVec * mpvRefParChildrenVec;
//	DFCVecIntVec * mpvRefParParentsVec;
//	DFCVecIntVec * mpvRefMMChildrenVec;
//	DFCVecIntVec * mpvRefMMParentsVec;
//
//	DFCVecVecIntVec * mpvJointChildrenGroupVec; 

	//hxiao 2012-10-19: merge the local relations into project bitvectors
	//to record current relationship is project or local relation
	//0 means project relation while 1 means local relation
	VecBitVec *mpvLocalOrProjRelVec;//to record current relationship is project or local relation , 0 means project while 1 means local

private:
    ////SchemaLoadIndex * mSmIndex;
	////DFCSchemaInterface *mpSchema;
public:
    
	//process to initialize the bit vectors in schema
	////HRESULT Init();
    ////DFCSchemaInterface* GetProjectSchema(){return mpSchema;};
	////SchemaLoadIndex * GetSchemaLoadIndex(){return mSmIndex;};
	//Currently we populate the BVs of Descendant & Ancestor && MMDescendant &&MMAncestor relationship bit vectors
	////HRESULT PopulateBVs(DFCSchemaInterface *ipSchema);
    
	/* !!!!!this function must be called after PopulateBVs is called!!!!!
     Goal: populate mpvRefChildrenVec, mpvRefParentsVec;
     General process:
     1. loop through all the MCE Attributes
     2. for each MCE Attribute, get its corresponding attribute
     3. read the corresponding vector containing all its direct children (parents)
     3.1 for every child (parent) attribute, if it also exists in corresponding the descendant bitVector
     then this attribute is the direct child(parent) of the MCE Attribute.
     !!!!!!!
     */
	///HRESULT PopulateRefVec(DFCSchemaInterface *ipSchema);
    
	
	////HRESULT FindDirectChildren(DFCSchemaInterface *ipSchema, MCEAttr *ipAttr, bool iIsMMRelConsidered = false);
    
	// new implementation, reading the direct relationship from DFCAttr instead of RefChildrenVec in schema!
	////HRESULT FindDirectChildren(MCEAttr *ipAttr, DFCSchemaInterface *ipSchema, bool iIsMMRelConsidered = false);
    
	////HRESULT FindDirectParents(DFCSchemaInterface *ipSchema, MCEAttr *ipAttr, bool iIsMMRelConsidered = false);
	////bool static CheckIfTwoAttrsProjectSchemaRelated(DFCAttr *ipAttr1, DFCAttr *ipAttr2, DFCSchemaInterface *ipProjectSchema);

};


#endif