#ifndef _DFCENGINELITE_H
#define _DFCENGINELITE_H
class MCESchema;
class CDSSEngineLite
{
public:
    CDSSEngineLite(MCESchema* ipMCESchema, MBase::Buffer* ipBuffer);
    ~CDSSEngineLite();
    int ResolveAggLevel(Int32 iLength, DSS_ID * iSet, Int32 * iLevel, Int32 * iWanted, Int32 * oResult);
    int ResolveDownwardLevel(Int32 iLength, DSS_ID * iSet, Int32 * iMetricLevel, Int32 * iTargetAttrs, Int32 * oResult);
    int ResolveTrueLevel(Int32 iLength, DSS_ID * iSet, Int32 * input, Int32 * oResult);
    int IsUpperLevel(Int32 iLength, DSS_ID * iSet, Int32 * iLevel1, Int32 * iLevel2, Int32 * oResult);
    int ResolveAllLevel(Int32 iLength, DSS_ID * iSet, Int32 * input, Int32 * oResult);
    // wyuan; 01/06/2014; Port ResolveAggLevelEx3 to Mobile Cube.
    int ResolveAggLevelEx3(
                                           Int32 iLength,		// the number of attributes
                                           DSS_ID* iSet,		// the ID of each attribute
                                           Int32 * iOrgLevel, // whether each attribute appears in the original level of the metric
                                            Int32* iWanted,	// whether each attribute is wanted in the final aggregation
                                            Int32 iNumNonAggAttrs,	// the number of non-aggregatable attributes
                                            DSS_ID* iNonAggAttrIDs, // the ID of each non-aggregatable attribute
                                            EnumDSSAggregation* iNonAgg,	// the aggregation property of each non-aggregatable attribute
                                            void * ipLocalSchema, // local schema for MDX or DI Attr
                                            Int32* oResult,	// whether each attribute is in the final aggregation level
                                            EnumDSSAggregation* oIntermediateAgg);
    
private:
    int ResolveAggLevelHelper(Int32 iLength, DSS_ID * iSet, Int32 * iOrgLevel, Int32 * iWanted, Int32 * oResult, bool iHighLevel);
    int ResolveDownwardLevelHelper(Int32 iLength, DSS_ID * iSet, Int32 * iMetricLevel, Int32 * iTargetAttrs, Int32 * oResult);
    int ResolveTrueLevelHelper(Int32 iLength, DSS_ID * iSet, Int32 * input, Int32 * oResult, bool iHighLevel);
    int IsUpperLevelHelper(Int32 iLength, DSS_ID * iSet, Int32 * iLevel1, Int32 * iLevel2, Int32 * oResult);
    int ResolveAllLevelHelper(Int32 iLength, DSS_ID * iSet, Int32 * input, Int32 * oResult);
    //wyuan; 01/06/2014;
    int ResolveAggLevelExHelper(
                                                Int32 iLength,		// the number of attributes
                                                DSS_ID* iSet,		// the ID of each attribute
                                                Int32 * iOrgLevel, // whether each attribute appears in the original level of the metric
                                                Int32* iWanted,	// whether each attribute is wanted in the final aggregation
                                                Int32 iNumNonAggAttrs,	// the number of non-aggregatable attributes
                                                DSS_ID* iNonAggAttrIDs, // the ID of each non-aggregatable attribute
                                                EnumDSSAggregation* iNonAgg,	// the aggregation property of each non-aggregatable attribute
                                                Int32* oResult,	// whether each attribute is in the final aggregation level
                                                EnumDSSAggregation* oIntermediateAgg,	// the intermediate aggregation property for each attribute
                                void * ipLocalSchema); // local schema for MDX Attr
    
private:
    MCESchema* mpMCESchema;
    MBase::Buffer* mpBuffer;
};

#endif
