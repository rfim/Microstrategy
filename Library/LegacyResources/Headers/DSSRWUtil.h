#ifndef _RWUtil_H
#define _RWUtil_H

#import <map>
#import "GUID.h"

class DSSTabularConsolidation;
class DSSTabularAttribute;
class DSSTabularCustomGroup;
class DSSTabularData;
class DSSXTabBitVector;
class DSSTabularIndex;

// awu, 8/30/2007, Declare AttributeIdArityMapType to associate MSIAttribute ID to its Arity
typedef std::map<GUID, int, std::less<GUID> > AttributeIdArityMapType;
typedef AttributeIdArityMapType::const_iterator MapIteratorType;

typedef struct RWUtilContext
{
    AttributeIdArityMapType* mpIdArityMap;
    int mLocale;
    int mForm;
    bool mfDisplayNullOnTop;
    bool mfIsDefaultSort;
    bool mfIncludeIDForms;
	
	RWUtilContext(): mpIdArityMap(NULL), mLocale(0), mForm(-1), mfDisplayNullOnTop(true), mfIsDefaultSort(true), mfIncludeIDForms(true) {};
	
    RWUtilContext(int iForm,
                  AttributeIdArityMapType* ipIdArityMap = NULL,
                  int iLocale = 0,  ////DssLocaleNeutral
                  bool fDisplayNullOnTop = true,
                  bool fIsDefaultSort = true,
                  bool fIncludeIDForms = true
                  ):mpIdArityMap(ipIdArityMap), mLocale(iLocale), mForm(iForm), mfDisplayNullOnTop(fDisplayNullOnTop), mfIsDefaultSort(fIsDefaultSort), mfIncludeIDForms(fIncludeIDForms) {};
	
	~RWUtilContext(){}
	
}RWUtilContext;

// awu, 8/30/2007, a new parameter of type AttributeIdArityMapType* is added for functions which 
// directly or indirectly need Arity of Attibute objects of related Elements.
int CompareCSElements4Display(DSSTabularConsolidation *ipConsolidation1, int iEleIndex1, void *ipTabularData1,
                              DSSTabularConsolidation *ipConsolidation2, int iEleIndex2, void *ipTabularData2,
                              RWUtilContext iContext,
                              //ctang
                              int iViewIndex1, int iViewIndex2,
                              int & oResult);

int CompareLkUpTableElements(DSSTabularAttribute *ipLookUpTable1, int iEleIndex1,
                             DSSTabularAttribute *ipLookUpTable2, int iEleIndex2,
                             RWUtilContext iContext,
                             int & oResult);

int CompareCGElements(DSSTabularCustomGroup *ipCustomGroup1, int iEleIndex1,
                      DSSTabularCustomGroup *ipCustomGroup2, int iEleIndex2,
                      RWUtilContext iContext, bool ifIsParentFirst,
                      int& oResult);


int CompareDEWithAttr4Display(DSSTabularConsolidation *ipConsolidation, int iEleIndex1, 
                              DSSTabularAttribute *ipLookUpTable, int iEleIndex2,
                              RWUtilContext iContext,
                              bool fIsReversed,
                              int & oResult);

int hCompareDEWithViewIndex(int iViewIndex1, int iViewIndex2, int* opResult);

int hCheckDisplayable(DSSTabularData *ipTabularData, DSSTabularIndex *ipIndices, DSSXTabBitVector *iopMask);

/*int CompareCSElements4Display(DSSTabularConsolidation *ipConsolidation1, int iEleIndex1, ICDSSTabularData *ipTabularData1,
									 DSSTabularConsolidation *ipConsolidation2, int iEleIndex2, ICDSSTabularData *ipTabularData2,
									 ICDSSDocumentInstance3 *ipDocInstance,
			 						  bool fDisplayNullOnTop,
									  int iForm,
									  bool fIsDefaultSort, AttributeIdArityMapType* ipIdArityMap,
									  int iLocale,
									 int & oResult);

int CompareCSElements4NonDisplay(DSSTabularConsolidation *ipConsolidation1, int iEleIndex1, ICDSSTabularData *ipTabularData1,
						  DSSTabularConsolidation *ipConsolidation2, int iEleIndex2, ICDSSTabularData *ipTabularData2,
						  ICDSSDocumentInstance3 *ipDocInstance,
						  bool fDisplayNullOnTop, AttributeIdArityMapType* ipIdArityMap,
						  int & oResult);

int CompareCGElements(ICDSSXTabCustomGroup *ipCustomGroup1, int iEleIndex1,
						  ICDSSXTabCustomGroup *ipCustomGroup2, int iEleIndex2,
						  bool fIsParentFirst,
						  ICDSSDocumentInstance3 *ipDocInstance,
  						  bool fDisplayNullOnTop, AttributeIdArityMapType* ipIdArityMap,
						  int & oResult);

int hCompareTableElements(ICDSSXTabCustomGroup *ipCustomGroup1, 
							ICDSSXTabCustomGroup *ipCustomGroup2,
							int iTableIndex1, int iIndex1,
							int iTableIndex2, int iIndex2, 
							ICDSSDocumentInstance3 *ipDocInstance,
							bool fDisplayNullOnTop, AttributeIdArityMapType* ipIdArityMap,
							int &oResult);

int CompareVariant(VARIANT *pVar1, VARIANT * pVar2,
		int & oResult);

int gGetTabularDataThroughTemplate(ICDSSReportInstance *ipReportInstance, 
									ICDSSTemplate *ipTemplate,
									ICDSSTabularData **oppTabluarData);

void Merge(vector<int> & Range1, vector<int> & Range2, vector<int> & CombineRange);

int CompareLkUpTableElements(ICDSSXTabLookUpTable *ipLookUpTable1, int iEleIndex1,
									 ICDSSXTabLookUpTable *ipLookUpTable2, int iEleIndex2,
									 ICDSSDocumentInstance3 *ipDocInstance,
			 						bool fDisplayNullOnTop, AttributeIdArityMapType* ipIdArityMap,
									 int & oResult,
									 int Form = -1,
									 bool fIncludeIDForms = true,
									 int iLocale = DssLocaleNeutral);

int CompareDEWithAttr4NonDisplay(DSSTabularConsolidation *ipConsolidation, int iEleIndex1, ICDSSTabularData *ipTabularData1,
						  ICDSSXTabLookUpTable *ipLookUpTable, int iEleIndex2,
						  ICDSSDocumentInstance3 *ipDocInstance,
						  bool fDisplayNullOnTop, AttributeIdArityMapType* ipIdArityMap,
						  int & oResult);

int CompareDEWithAttr4Display(DSSTabularConsolidation *ipConsolidation, int iEleIndex1, ICDSSTabularData *ipTabularData1,
						  ICDSSXTabLookUpTable *ipLookUpTable, int iEleIndex2,
						  ICDSSDocumentInstance3 *ipDocInstance,
						  bool fDisplayNullOnTop, 
						  int iForm,
						  bool fIsDefaultSort, 
						  AttributeIdArityMapType* ipIdArityMap,
						  int iLocale,
						  bool fIsReversed,
						  int & oResult);
 */

/*	awu, 8/30/2007, Add helper function to retrieve MSIAttribute Arity.
	Helper function hGetAttributeArity retrieves the Arity of an MSIAttribute given its ObjectID

	@param	ipDocInstance		pointer to the DocumentInstance
	@param	iID					Object ID of the MSIAttribute object.
	@param	ipIdArityMap		the Map of MSIAttribute ID to its Arity.
	@param	oResult				the Arity of the given MSIAttribute
	@return COM error code
*/

/*
int hGetAttributeArity(ICDSSDocumentInstance3* ipDocInstance, DSS_ID& iID,
						AttributeIdArityMapType* ipIdArityMap, int &oResult);

int hCompareOrdinal(DSSTabularConsolidation *ipConsolidation1, int iEleIndex1,
						DSSTabularConsolidation *ipConsolidation2, int iEleIndex2,
						int & oResult);

int hCompareDEDefault(DSSTabularConsolidation *ipConsolidation1, int iEleIndex1, ICDSSTabularData *ipTabularData1,
									 DSSTabularConsolidation *ipConsolidation2, int iEleIndex2, ICDSSTabularData *ipTabularData2, 
									 ICDSSDocumentInstance3 *ipDocInstance,
			 						  bool fDisplayNullOnTop, AttributeIdArityMapType* ipIdArityMap,
									  int iForm,
									 int & oResult);

int hCompareDE(DSSTabularConsolidation *ipConsolidation1, int iEleIndex1, ICDSSTabularData *ipTabularData1,
									 DSSTabularConsolidation *ipConsolidation2, int iEleIndex2, ICDSSTabularData *ipTabularData2,
									 ICDSSDocumentInstance3 *ipDocInstance,
			 						  bool fDisplayNullOnTop, AttributeIdArityMapType* ipIdArityMap,
									  int iForm,
									  int iLocale,
									 int & oResult);

int hGetTable( DSSTabularConsolidation *ipConsolidation, ICDSSTabularData * ipTabularData, ICDSSXTabLookUpTable** opXTabLookupTable, ICDSSXTabTable** opXTabTable);

int hCompareValue(DSSTabularConsolidation *ipConsolidation, int iElementIndex, ICDSSTabularData *ipTabularData, unsigned char* ipData, ICDSSDocumentInstance3 *ipDocInstance, int iForm, int iLocale, bool fDisplayNullOnTop, int& oResult);

int GetDataMethod(ICDSSDocumentInstance3 *ipDI, EnumDSSDataMethod & orDataMethod);

int GetXTabColumns(ICDSSTabularData *ipTabularData, int iUnit, EnumDSSTemplateUnitType iType, 
					   int iForm, EnumDSSDataMethod iDataMethod, int iLocale, 
					   ICDSSXTabColumn** oppXTabColumns);

//get the XTabLookup for attribute and DE
int getXTabLookupTable(ICDSSTabularData *ipTabularData, int iUnit, bool &orIsDE, ICDSSXTabLookUpTable ** oppLookUpTable, ICDSSXTabTable ** oppXTabTable, DSSTabularConsolidation ** oppCS = NULL);

int getXTabLookupTable(ICDSSTabularData *ipTabularData, DSS_ID &iUnitID, bool &orIsDE, ICDSSXTabLookUpTable ** oppLookUpTable, ICDSSXTabTable ** oppXTabTable, DSSTabularConsolidation ** oppCS = NULL);
*/
#endif