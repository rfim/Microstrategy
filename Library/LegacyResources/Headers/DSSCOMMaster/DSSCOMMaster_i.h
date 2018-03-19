#ifndef __MINIMIDL_DSSCOMMasterLib_IDL_H__
#define __MINIMIDL_DSSCOMMasterLib_IDL_H__
typedef GUID DSS_ID;

#include "PDCHeader/PDCwtypes.h"
#import "DSSIPhoneEnums.h"
#import "DSSRwdconstants.h"

typedef enum EnumDSSEmbedMethod
{
    DssEmbedReserved = 0,
    DssEmbedBothFiltersTogether = 1,
    DssEmbedReportIntoMetricFilter = 2,
    DssEmbedMetricIntoReportFilter = 3,
    DssEmbedTreeMetricThenReportFilter = 4,
    DssEmbedTreeReportThenMetricFilter = 5
} EnumDSSEmbedMethod;

typedef enum EnumDSSOuterJoinType
{
    DssOuterJoinReserved = 0,
    DssOuterJoinDefault = 1,
    DssOuterJoinEqui = 2,
    DssOuterJoinOuter = 3
} EnumDSSOuterJoinType;

typedef enum EnumDssSecurityCheckType
{
    DssSecurityCheckTypeReserved = 0x0,
    DssSecurityCheckTypeSpecialFG = 0x1,
    DssSecurityCheckTypeFG = 0x2,
    DssSecurityCheckTypeLinkedTemplateNode = 0x4,
    DssSecurityCheckTypeNormalNode = 0x8,
    DssSecurityCheckTypeAll =	0xf
} EnumDssSecurityCheckType;

typedef enum EnumDSSRWNodeRuntimeState
{
    EnumDSSRWNodeRuntimeStateReserved = 0,
    EnumDSSRWNodeRuntimeStateSpecialFGChecked = 0x00000001,
    EnumDSSRWNodeRuntimeStateNodeChecked = 0x00000002,
    EnumDSSRWNodeRuntimeStateAllChecked = 0x00000003,
    EnumDSSRWNodeRuntimeStateGBInChildren = 0x00000010,
    EnumDSSRWNodeRuntimeStateSubtreeEmpty = 0x00000020,
    EnumDSSRWNodeRuntimeStateStatic = 0x00000100,
    EnumDSSRWNodeRuntimeHasMetricCondition = 0x00001000,
    EnumDSSRWNodeRuntimeHasUnitCondition = 0x00002000,
    EnumDSSRWNodeRuntimeHasBothCondition = 0x00003000,
    EnumDSSRWNodeRuntimeOnPartialPath = 0x00010000,
    EnumDSSRWNodeRuntimeOnPartialTarget = 0x00020000,
    EnumDSSRWNodeRuntimePartialFlag = EnumDSSRWNodeRuntimeOnPartialPath  | EnumDSSRWNodeRuntimeOnPartialTarget
} EnumDSSRWNodeRuntimeState;

typedef enum EnumDSSNonSharedPropertyStatus
{
    EnumDSSNonSharedReserved = 0,
    EnumDSSNonSharedInitial13Properties = 0x00000001,
    EnumDSSNonSharedSelectorOrientation = 0x00000002,
    EnumDSSNonSharedActiveBits = EnumDSSNonSharedInitial13Properties  | EnumDSSNonSharedSelectorOrientation
} EnumDSSNonSharedPropertyStatus;

typedef enum EnumDSSMetricLimitType
{
    DssMetricLimitReserved = 0,
    DssMetricLimitAND = 1,
    DssMetricLimitOR = 2,
    DssMetricLimitExpression = 3
} EnumDSSMetricLimitType;

//typedef enum EnumDSSElementType
//{
//    DssElementReserved = 0,
//    DssElementConcrete = 1,
//    DssElementMetric = 2,
//    DssElementSubtotal = 3,
//    DssElementFirst = 4,
//    DssElementLast = 5,
//    DssElementDimension = 6,
//    DssElementConsolidation = 7,
//    DssElementFilter = 8,
//    DssElementCustomGroup = 9,
//    DssElementJoint =10,
//    DssElementBand =11,
//    DssElementResidue =12,
//    DssElementSubexpression =13,
//    DssElementAll = 14,
//    DssElementNode = 15,
//    DssElementNULL = 16
//} EnumDSSElementType;

//typedef enum EnumDSSSubtotalStyle
//{
//    DssSubtotalReserved = 0,
//    DssSubtotalNone = 1,
//    DssSubtotalGrandTotal = 2,
//    DssSubtotalOutline = 3,
//    DssSubtotalCube = 4
//} EnumDSSSubtotalStyle;
//
//typedef enum EnumDSSTemplateSubtotalType
//{
//    DssTemplateSubtotalReserved = 0,
//    DssTemplateSubtotalList = -1,
//    DssTemplateSubtotalDefinition = -2,
//    DssTemplateSubtotalDefault = -3,
//    DssTemplateSubtotalAll = -4
//} EnumDSSTemplateSubtotalType;

//typedef enum EnumDSSMetricType
//{
//    DssMetricReserved = 0,
//    DssMetricTraining = -6,
//    DssMetricDMX = -7,
//    DssMetricNormal = -8,
//    DssMetricSubtotal = -9,
//    DssMetricSubtotalSum = -10,
//    DssMetricSubtotalCount = -11,
//    DssMetricSubtotalAverage = -12,
//    DssMetricSubtotalMin = -13,
//    DssMetricSubtotalMax = -14,
//    DssMetricSubtotalProduct = -15,
//    DssMetricSubtotalMedian = -16,
//    DssMetricSubtotalMode = -17,
//    DssMetricSubtotalStdDev = -18,
//    DssMetricSubtotalVariance = -19,
//    DssMetricSubtotalGeometricMean = -20,
//    DssMetricSubtotalAggregation = -21,
//    DssMetricSubtotalReservedLastOne
//} EnumDSSMetricType;

typedef enum EnumDSSFilterType
{
    DssFilterReserved = 0,
    DssFilterNormal = 1,
    DssCustomGroup = 2,
    DssFilterPartition = 3
} EnumDSSFilterType;

//typedef enum EnumDSSSortType
//{
//    DssSortReserved = 0,
//    DssSortSubtotalsPosition = 1,
//    DssSortAttributeForm = 2,
//    DssSortAttributeDefault = 3,
//    DssSortMetric = 4,
//    DssSortMetricHierarchical = 5,
//    DssSortGroupByMetric = 6,
//    DssSortGroupBySubtotal = 7,
//    DssSortDimensionForm = 8,
//    DssSortDimensionDefault = 9,
//    DssSortConsolidationDefault = 10,
//    DssSortCustomGroupDefault = 11,
//    DssSortValue = 12,
//    DssSortCustomGroupForm = 13,
//    DssSortAttributeColumn = 14,
//    DssSortUnitSubtotal = 15,
//    DssSortDerivedElementDefault = 16,
//    DssSortDerivedElementForm = 17
//} EnumDSSSortType;

typedef enum EnumDSSSearchType
{
    DssSearchTypeReserved = 0,
    DssSQLSearch = 1,
    DssIndexedSearch = 2
} EnumDSSSearchType;

//typedef enum EnumDSSSubtotalsPosition
//{
//    DssSubtotalsPositionReserved =	0,
//    DssSubtotalsPositionMix =	1,
//    DssSubtotalsPositionFirst =	2,
//    DssSubtotalsPositionLast =	3,
//    DssSubtotalsPositionInherit =	4
//} EnumDSSSubtotalsPosition;
//
//typedef enum EnumDSSAxisName
//{
//    DssAxisReserved = 0,
//    DssAxisRows = 1,
//    DssAxisColumns = 2,
//    DssAxisPages = 3,
//    DssAxisChapters = 4
//} EnumDSSAxisName;

typedef enum EnumDSSAxesBitMap
{
    DssAxisNone = 0,
    DssAxisRowsBit = 1,
    DssAxisColumnsBit = 2,
    DssAxisPagesBit = 4,
    DssAxisChaptersBit = 8
} EnumDSSAxesBitMap;

//typedef enum EnumDSSTemplateUnitType
//{
//    DssTemplateReserved = 0,
//    DssTemplateAttribute = 1,
//    DssTemplateDimension = 2,
//    DssTemplateMetrics = 3,
//    DssTemplateCustomGroup = 4,
//    DssTemplateConsolidation = 5,
//    DssTemplatePrompt = 6,
//    DssTemplateRawUnit = 7
//} EnumDSSTemplateUnitType;

typedef enum EnumDSSDimtyUnitType
{
    DssDimtyUnitTypeDefault = 1,
    DssDimtyUnitTypeReserved = 0,
    DssDimtyUnitTypeAttribute = -1,
    DssDimtyUnitTypeDimension = -2,
    DssDimtyUnitTypeReportLevel = -3,
    DssDimtyUnitTypeReportBaseLevel = -4,
    DssDimtyUnitTypeRole = -5,
    DssDimtyUnitTypeCustomGroup = -6,
    DssDimtyUnitTypeConsolidation = -7,
    DssDimtyUnitTypeDropZone = -8,
    DssDimtyUnitTypeDynamicShortcut = -9
} EnumDSSDimtyUnitType;

typedef enum EnumDSSAggregation
{
    DssAggregationReserved = 0,
    DssAggregationNormal = 1,
    DssAggregationFirstInFact = 2,
    DssAggregationLastInFact = 3,
    DssAggregationFirstInRelationship = 4,
    DssAggregationLastInRelationship = 5
} EnumDSSAggregation;

typedef enum EnumDSSFiltering
{
    DssFilteringReserved = 0,
    DssFilteringApply = 1,
    DssFilteringAbsolute = 2,
    DssFilteringIgnore = 3,
    DssFilteringNone = 4
} EnumDSSFiltering;

typedef enum EnumDSSDimtyRelativePosition
{
    DssDimtyHighestAncestor = 0x7fffffff,
    DssDimtyLowestDescendant = 0x80000000
} EnumDSSDimtyRelativePosition;

//typedef enum EnumDSSReportType
//{
//    DssReportTypeReserved,
//    DssReportTypeBase,
//    DssReportTypeSubsetting,
//    DssReportTypePrivateBase,
//    DssReportTypePrivateSubset,
//    DssReportTypeRWBase,
//    DSSReportTypeCSQLPreExec,
//    DssReportTypeCube,
//    DssReportTypeSubset,
//    DssReportTypeIncrementalRefresh
//} EnumDSSReportType;
//
//typedef enum EnumDSSDisplayMode
//{
//    DssDisplayModeReserved,
//    DssDisplayModeGrid,
//    DssDisplayModeGraph,
//    DssDisplayModeEngine,
//    DssDisplayModeText,
//    DssDisplayModeDatamart,
//    DssDisplayModeBase,
//    DssDisplayModeGridGraph,
//    DssDisplayModeNonInteractive,
//    DssDisplayModeCube,
//    DssDisplayModeIncrementalRefresh,
//    DssDisplayModeTransaction
//} EnumDSSDisplayMode;

typedef enum EnumDSSDocumentEmbeddedState
{
    DssDocumentEmbedReserved = 0,
    DssDocumentEmbedEmbedded,
    DssDocumentEmbedLinked,
    DssDocumentEmbedLinkEmbedded
} EnumDSSDocumentEmbeddedState;

typedef enum EnumDSSDocumentPresentationType
{
    DssDocumentPresentationReserved = 0,
    DssDocumentPresentationPopup,
    DssDocumentPresentationReplace
} EnumDSSDocumentPresentationType;

typedef enum DSSShortcut_Type
{
    DssShortcutTypeReserved = 0,
    DssShortcutTypeMD,
    DssShortcutTypeEmbedded
} DSSShortcut_Type;

//typedef enum EnumDSSObjectType
//{
//    DssTypeUnknown =				-1,
//    DssTypeReserved =		0,
//    DssTypeGeneric =		5,
//    DssTypeFormatBAD =		46,
//    DssTypeFilter =						1,
//    DssTypeTemplate =					2,
//    DssTypeReportDefinition =			3,
//    DssTypeMetric =						4,
//    DssTypeStyle =						6,
//    DssTypeAggMetric =					7,
//    DssTypeFolder =						8,
//    DssTypeDevice =						9,
//    DssTypePrompt =						10,
//    DssTypeFunction =					11,
//    DssTypeAttribute =					12,
//    DssTypeFact =						13,
//    DssTypeDimension =					14,
//    DssTypeTable =						15,
//    DssTypeDatamartReport =				16,
//    DssTypeFactGroup =					17,
//    DssTypeShortcut =					18,
//    DssTypeResolution =					19,
//    DssTypeMonitor =					20,
//    DssTypeAttributeForm =				21,
//    DssTypeSchema =						22,
//    DssTypeFormat =						23,
//    DssTypeCatalog =					24,
//    DssTypeCatalogDefn =				25,
//    DssTypeColumn =						26,
//    DssTypePropertyGroup =		27,
//    DssTypePropertySet =				28,
//    DssTypeDBRole =						29,
//    DssTypeDBLogin =					30,
//    DssTypeDBConnection =				31,
//    DssTypeProject =					32,
//    DssTypeServerDef =					33,
//    DssTypeUser =						34,
//    DssTypeTransmitter =				35,
//    DssTypeConfiguration =				36,
//    DssTypeRequest =					37,
//    DssTypeScript =						38,
//    DssTypeSearch =						39,
//    DssTypeSearchFolder =				40,
//    DssTypeDatamart =					41,
//    DssTypeFunctionPackageDefinition =	42,
//    DssTypeRole =						43,
//    DssTypeSecurityRole =				44,
//    DssTypeLocale =						45,
//    DssTypeConsolidation =				47,
//    DssTypeConsolidationElement =		48,
//    DssTypeScheduleEvent =				49,
//    DssTypeScheduleObject =				50,
//    DssTypeScheduleTrigger =			51,
//    DssTypeLink =						52,
//    DssTypeDbTable =					53,
//    DssTypeTableSource =				54,
//    DssTypeDocumentDefinition =			55,
//    DssTypeDrillMap =					56,
//    DssTypeDBMS =						57,
//    DssTypeMDSecurityFilter =			58,
//    DssTypePromptAnswer =				59,
//    DssTypePromptAnswers =				60,
//    DssTypeGraphStyle =				61,
//    DssTypeChangeJournalSearch =		62,
//    DssTypeBlob =						63,
//    DssTypeDashboardTemplate =			64,
//    DssTypeFlag =						65,
//    DssTypeChangeJournal =		66,
//    DssTypeExternalShortcut =			67,
//    DssTypeExternalShortcutTarget =		68,
//    DssTypeReconciliation =				69,
//    DssTypeLayer =                      70,
//    DssTypeNCSObjects =				0xFF,
//    DssTypeReservedLastOne =	71
//} EnumDSSObjectType;

typedef enum EnumDSSSubTypes
{
    DssSubTypeUnknown = -1,
    DssSubTypeReserved = 0,
    DssSubTypeFilter = 0x0100,
    DssSubTypeCustomGroup = 0x0101,
    DssSubTypeFilterPartition = 0x0102,
    DssSubTypeTemplate = 0x0200,
    DssSubTypeReportGrid = 0x0300,
    DssSubTypeReportGraph = 0x0301,
    DssSubTypeReportEngine = 0x0302,
    DssSubTypeReportText = 0x0303,
    DssSubTypeReportDatamart = 0x0304,
    DssSubTypeReportBase = 0x0305,
    DssSubTypeReportGridAndGraph = 0x0306,
    DssSubTypeReportNonInteractive = 0x0307,
    DssSubTypeReportCube = 0x0308,
    DssSubTypeReportIncrementRefresh = 0x0309,
    DssSubTypeReportTransaction = 0x030a,
    DssSubTypeMetric = 0x0400,
    DssSubTypeSubtotalDefinition = 0x0401,
    DssSubTypeSystemSubtotal = 0x0402,
    DssSubTypeMetricDMX = 0x0403,
    DssSubTypeMetricTraining = 0x0404,
    DssSubTypeStyle = 0x0600,
    DssSubTypeAggMetric = 0x0700,
    DssSubTypeFolder = 0x0800,
    DssSubTypeFolderSystem = 0x0801,
    DssSubTypeDevice = 0x0900,
    DssSubTypePrompt = 0x0a00,
    DssSubTypePromptBoolean = 0x0a01,
    DssSubTypePromptLong = 0x0a02,
    DssSubTypePromptString = 0x0a03,
    DssSubTypePromptDouble = 0x0a04,
    DssSubTypePromptDate = 0x0a05,
    DssSubTypePromptObjects = 0x0a06,
    DssSubTypePromptElements = 0x0a07,
    DssSubTypePromptExpression = 0x0a08,
    DssSubTypePromptExpressionDraft = 0x0a09,
    DssSubTypePromptDimty = 0x0a0a,
    DssSubTypePromptBigDecimal = 0x0a0b,
    DssSubTypeFunction = 0x0b00,
    DssSubTypeFunctionThirdParty = 0x0b01,
    DssSubTypeAttribute = 0x0c00,
    DssSubTypeAttributeRole = 0x0c01,
    DssSubTypeAttributeTransformation = 0x0c02,
    DssSubTypeAttributeAbstract = 0x0c03,
    DssSubTypeFact = 0x0d00,
    DssSubTypeDimensionSystem = 0x0e00,
    DssSubTypeDimensionUser = 0x0e01,
    DssSubTypeDimensionOrdered = 0x0e02,
    DssSubTypeDimensionUserHierarchy = 0x0e03,
    DssSubTypeTable = 0x0f00,
    DssSubTypeTablePartitionMD = 0x0f01,
    DssSubTypeTablePartitionWH = 0x0f02,
    DssSubTypeDatamartReport = 0x1000,
    DssSubTypeFactGroup = 0x1100,
    DssSubTypeShortcut = 0x1200,
    DssSubTypeResolution = 0x1300,
    DssSubTypeAttributeForm = 0x1500,
    DssSubTypeFormSystem = 0x1501,
    DssSubTypeFormNormal = 0x1502,
    DssSubTypeSchema = 0x1600,
    DssSubTypeFormat = 0x1700,
    DssSubTypeCatalog = 0x1800,
    DssSubTypeCatalogDefn = 0x1900,
    DssSubTypeColumn = 0x1a00,
    DssSubTypeColumnNormal = 0x1a01,
    DssSubTypeColumnCustom = 0x1a02,
    DssSubTypePropertyGroup = 0x1b00,
    DssSubTypePropertySet = 0x1c00,
    DssSubTypeDBRole = 0x1d00,
    DssSubTypeDBRoleDataImport = 0x1d01,
    DssSubTypeDBRoleDataImportPrimary = 0x1d02,
    DssSubTypeDBLogin = 0x1e00,
    DssSubTypeDBConnection = 0x1f00,
    DssSubTypeProject = 0x2000,
    DssSubTypeServerDef = 0x2100,
    DssSubTypeUser = 0x2200,
    DssSubTypeUserGroup = 0x2201,
    DssSubTypeTransmitter = 0x2300,
    DssSubTypeConfiguration = 0x2400,
    DssSubTypeRequest = 0x2500,
    DssSubTypeSearch = 0x2700,
    DssSubTypeIndexedSearch = 0x2701,
    DssSubTypeRelationshipSearch = 0x2702,
    DssSubTypeSearchFolder = 0x2800,
    DssSubTypeSearchFolderCrossProject = 0x2801,
    DssSubTypeDatamart = 0x2900,
    DssSubTypeFunctionPackageDefinition = 0x2a00,
    DssSubTypeRole = 0x2b00,
    DssSubTypeRoleTransformation = 0x2b01,
    DssSubTypeSecurityRole = 0x2c00,
    DssSubTypeLocale = 0x2d00,
    DssSubTypeConsolidation = 0x2f00,
    DssSubTypeConsolidationElement = 0x3000,
    DssSubTypeScheduleEvent = 0x3100,
    DssSubTypeScheduleObject = 0x3200,
    DssSubTypeScheduleTrigger = 0x3300,
    DssSubTypeLink = 0x3400,
    DssSubTypeDBTable = 0x3500,
    DssSubTypeDBTablePMT = 0x3501,
    DssSubTypeTableSource = 0x3600,
    DssSubTypeDocumentDefinition = 0x3700,
    DssSubTypeReportWritingDocument = 0x3701,
    DssSubTypeDrillMap = 0x3800,
    DssSubTypeDBMS = 0x3900,
    DssSubTypeMDSecurityFilter = 0x3a00,
    DssSubTypeMonitorPerformance = 0x3b00,
    DssSubTypeMonitorJobs = 0x3b01,
    DssSubTypeMonitorUserConnections = 0x3b02,
    DssSubTypeMonitorDBConnections = 0x3b03,
    DssSubTypePromptAnswer = 0x3b80,
    DssSubTypePromptAnswerBoolean = 0x3b81,
    DssSubTypePromptAnswerLong = 0x3b82,
    DssSubTypePromptAnswerString = 0x3b83,
    DssSubTypePromptAnswerDouble = 0x3b84,
    DssSubTypePromptAnswerDate = 0x3b85,
    DssSubTypePromptAnswerObjects = 0x3b86,
    DssSubTypePromptAnswerElements = 0x3b87,
    DssSubTypePromptAnswerExpression = 0x3b88,
    DssSubTypePromptAnswerExpressionDraft = 0x3b89,
    DssSubTypePromptAnswerDimty = 0x3b8a,
    DssSubTypePromptAnswerBigDecimal = 0x3b8b,
    DssSubTypePromptAnswers = 0x3c00,
    DssSubTypeGraphStyle = 0x3d00,
    DssSubTypeChangeJournalSearch = 0x3e00,
    DssSubTypeBlobUnknown = 0x3f00,
    DssSubTypeBlobOther = 0x3f01,
    DssSubTypeBlobImage = 0x3f02,
    DssSubTypeBlobProjectPackage = 0x3f03,
    DssSubTypeBlobExcel = 0x3f04,
    DssSubTypeDashboardTemplate = 0x4000,
    DssSubTypeFlag = 0x4100,
    DssSubTypeChangeJournal = 0x4200,
    DssSubTypeExternalShortcutUnknown = 0x4300,
    DssSubTypeExternalShortcutURL = 0x4301,
    DssSubTypeExternalShortcutSnapshot = 0x4302,
    DssSubTypeExternalShortcutTarget = 0x4400,
    DssSubTypeReconciliation = 0x4500,
    DssSubTypeSubscriptionAddress = 0xFF01,
    DssSubTypeSubscriptionContact = 0xFF02,
    DssSubTypeSubscriptionInstance = 0xFF03
} EnumDSSSubTypes;

//typedef enum EnumDSSNodeType
//{
//    DssNodeReserved =		0,
//    DssNodeFormShortcut =			1,
//    DssNodeElementsObject =			2,
//    DssNodeConstant =				3,
//    DssNodeOperator =				4,
//    DssNodeShortcut =				5,
//    DssNodeTime =					6,
//    DssNodeRelationship =			7,
//    DssNodeResidue =				8,
//    DssNodeMe =						9,
//    DssNodeBigDecimal =				10,
//    DssNodeSQLQueryShortcut =		11,
//    DssNodeSQLQueryColumnShortcut = 12,
//    DssNodeSQLColumnShortcut =		13,
//    DssNodeGroup =					14,
//    DssNodeReference =				15,
//    DssNodeCellFormatData =			16
//} EnumDSSNodeType;

//typedef enum EnumDSSExpressionType
//{
//    DssExpressionReserved = 0,
//    DssExpressionGeneric,
//    DSSFilterSingleBaseFormQual,
//    DSSFilterMultiBaseFormQual,
//    DSSFilterJointFormQual,
//    DSSFilterListQual,
//    DSSFilterListFormQual,
//    DSSFilterJointListQual,
//    DSSFilterJointListFormQual,
//    DSSFilterSingleBaseFormExpression,
//    DSSFilterSingleMetricQual,
//    DSSFilterMultiMetricQual,
//    DSSFilterMetricExpression,
//    DSSFilterEmbedQual,
//    DSSFilterBranchQual,
//    DSSFilterRelationshipQual,
//    DSSFilterAllAttributeQual,
//    DSSFilterAttributeIDQual,
//    DSSFilterAttributeDESCQual,
//    DssExpressionAggMetric,
//    DssExpressionBanding,
//    DssFilterReportQual,
//    DssExpressionMDXSAPVariable,
//    DssExpressionSQLQueryQual,
//    DssExpressionCanceledPrompt,
//    DssExpressionElementList,
//    DssExpressionElementSingle
//} EnumDSSExpressionType;

typedef enum EnumDSSNodeDimty
{
    DssNodeDimtyReserved = 0,
    DssNodeDimtyNone = 1,
    DssNodeDimtyContinuation = 2,
    DssNodeDimtyExclusiveContinuation =	3,
    DssNodeDimtyOutputLevel = 4,
    DssNodeDimtyBreakBy = 5,
    DssNodeDimtyEmbedded = 6,
    DssNodeDimtyUnspecified = 7
} EnumDSSNodeDimty;

typedef enum EnumDSSSearchDomain
{
    DssSearchReserved = 0,
    DssSearchLocal = 1,
    DssSearchProject = 2,
    DssSearchRepository = 3,
    DssSearchConfiguration = 4,
    DssSearchConfigurationAndAllProjects = 5,
    DssSearchRemote = 6
} EnumDSSSearchDomain;

typedef enum EnumDSSSearchFlags
{
    DssSearchNameWildCard = 0x00000001,
    DssSearchAbbreviationWildCard = 0x00000002,
    DssSearchDescriptionWildCard = 0x00000004,
    DssSearchVisibleOnly = 0x00000008,
    DssSearchUsesOneOf = 0x00000010,
    DssSearchUsesRecursive = 0x00000020,
    DssSearchUsedByOneOf = 0x00000040,
    DssSearchUsedByRecursive = 0x00000080,
    DssSearchRootRecursive = 0x00000100,
    DssSearchModificationTime = 0x00000200,
    DssSearchFolderBrowseStyle = 0x00000400,
    DssSearchDereferenceShortcuts = 0x00000800,
    DssSearchEmptyProfileFolders = 0x00001000,
    DssSearchIncludeMyProfileFolders = 0x00002000,
    DssSearchHaveDependentsInProfileFolder = 0x00004000,
    DssSearchReturnObjectsOrderByObjectID = 0x00008000,
    DssSearchUsedByOutsideProfileFolder = 0x00040000,
    DssSearchIncludeMyObjectsFolder = 0x10000000,
    DssSearchIncludeMyReportsFolder = 0x20000000,
    DssSearchIncludeMyAnswersFolder = 0x40000000,
    DssSearchIncludeMyFavoritesFolder = 0x80000000,
    DssSearchNoNameTranslation = 0x00100000,
    DssSearchNoDescriptionTranslation = 0x00200000,
    DssSearchHasNameTranslation = 0x00400000,
    DssSearchHasDescriptionTranslation = 0x00800000,
    DssSearchUsesOneOfTypes = 0x01000000,
    DssSearchUsedByOneOfTypes = 0x02000000,
    DssSearchIgnoreACL = 0x04000000,
    DssSearchTrustedAuth = 0x00010000,
    DssSearchPassthruAuth = 0x00020000,
    DssSearchStandardAuth = 0x00030000,
    DssSearchLDAPDNAuth = 0x00080000,
    DssSearchUIDAuth = 0x00090000,
    DssSearchAuthentication = 0x000B0000,
    DssSearchReturnObjectsUnordered = 0x08000000
} EnumDSSSearchFlags;

typedef enum EnumDSSSearchUnit
{
    DssSearchUnitReserved = 0,
    DssSearchUnitHour = 1,
    DssSearchUnitDay = 2,
    DssSearchUnitWeek = 3,
    DssSearchUnitMonth = 4,
    DssSearchUnitYear = 5
} EnumDSSSearchUnit;

typedef enum EnumDSSPropertyGroupTypes
{
    DssPropertyGroupReserved =			0x00000000,
    DssPropertyGroupFilter =			0x00000001,
    DssPropertyGroupTemplate =			0x00000002,
    DssPropertyGroupReport =			0x00000004,
    DssPropertyGroupMetric =			0x00000008,
    DssPropertyGroupColumn =			0x00000010,
    DssPropertyGroupAggMetric =			0x00000020,
    DssPropertyGroupFolder =			0x00000040,
    DssPropertyGroupUser =				0x00000080,
    DssPropertyGroupPrompt =			0x00000100,
    DssPropertyGroupFunction =			0x00000200,
    DssPropertyGroupAttribute =			0x00000400,
    DssPropertyGroupFact =				0x00000800,
    DssPropertyGroupFactGroup =			0x00000800,
    DssPropertyGroupDimension =		0x00001000,
    DssPropertyGroupTable =				0x00002000,
    DssPropertyGroupFormat =			0x00004000,
    DssPropertyGroupShortcut =			0x00008000,
    DssPropertyGroupResolution =		0x00010000,
    DssPropertyGroupSearch =			0x00020000,
    DssPropertyGroupProject =			0x00040000,
    DssPropertyGroupDBRole =			0x00080000,
    DssPropertyGroupConsolidationElement =	0x00100000,
    DssPropertyGroupAttributeForm =		0x00200000,
    DssPropertyGroupDBMS =				0x00400000,
    DssPropertyGroupMDSecurityFilter =  0x00800000,
    DssPropertyGroupSchedule =			0x01000000,
    DssPropertyGroupDatamart =			0x02000000,
    DssPropertyGroupDocument =			0x04000000,
    DssPropertyGroupGeneralObjects =	0x80000000,
    DssPropertyGroupAll =				0xffffffff
} EnumDSSPropertyGroupTypes;

typedef enum EnumDSSObjectState
{
    DssStateDefnLoaded =			0x00000001,
    DssStateBrowserLoaded =			0x00000002,
    DssStateDatesLoaded =			0x00000004,
    DssStateCommentsLoaded =		0x00000008,
    DssStateDependentsLoaded =		0x00000010,
    DssStatePropertiesLoaded =		0x00000020,
    DssStateSecurityLoaded =		0x00000040,
    DssStateTotalObjectLoaded =		0x0000007f,
    DssStatePersisted =				0x00000100,
    DssStateDirty =					0x00000200,
    DssStateDeleted =				0x00000400,
    DssStateCorrupt =				0x00000800,
    DssStateDependentsDirty =		0x00001000,
    DssStateMissing =				0x00002000,
    DssStateError =					0x00004000,
    DssStateEmbedded =				0x00008000,
    DssStateContainer =				0x00010000,
    DssStateCached =				0x00020000,
    DssStatePromptsSubstituted =	0x00040000,
    DssStateTemplateAppObjectDirty = 0x00100000,
    DssStateTemplateContentDirty =	0x00200000,
    DssStateTemplateStructureDirty = 0x00400000,
    DssStateTemplateViewDirty =	0x00800000,
    DssStateFilterAppObjectDirty =	0x01000000,
    DssStateDocumentDataSourceDirty =	0x02000000,
    DssStateDiscardPromptLayoutAndAnswers =	0x02000000,
    DssStateDeletionHandled =		0x08000000,
    DssStateInstanceNotValidated =	0x10000000,
    DssStatePreserve =				0x20000000,
    DssStateMultiThreaded =			0x40000000,
    DssStateBypassWriteAccessCheck =	0x80000000
} EnumDSSObjectState;

//typedef enum EnumDSSFunction
//{
//    DssFunctionThirdParty = -2,
//    DssFunctionCustom = -1,
//    DssFunctionReserved = 0,
//    DssFunctionPlus = 1,
//    DssFunctionMinus = 2,
//    DssFunctionTimes = 3,
//    DssFunctionDivide = 4,
//    DssFunctionUnaryMinus = 5,
//    DssFunctionEquals = 6,
//    DssFunctionNotEqual = 7,
//    DssFunctionGreater = 8,
//    DssFunctionLess = 9,
//    DssFunctionGreaterEqual = 10,
//    DssFunctionLessEqual = 11,
//    DssFunctionSum = 12,
//    DssFunctionCount = 13,
//    DssFunctionAvg = 14,
//    DssFunctionMin = 15,
//    DssFunctionMax = 16,
//    DssFunctionBetween = 17,
//    DssFunctionLike = 18,
//    DssFunctionAnd = 19,
//    DssFunctionOr = 20,
//    DssFunctionNot = 21,
//    DssFunctionIn = 22,
//    DssFunctionRank = 23,
//    DssFunctionAbs = 24,
//    DssFunctionRunningSum = 25,
//    DssFunctionRunningAvg = 26,
//    DssFunctionMovingSum = 28,
//    DssFunctionMovingAvg = 27,
//    DssFunctionProduct = 29,
//    DssFunctionMedian = 30,
//    DssFunctionMode = 31,
//    DssFunctionStdev = 32,
//    DssFunctionVar = 33,
//    DssFunctionGeomean = 34,
//    DssFunctionEqualEnhanced = 35,
//    DssFunctionNotEqualEnhanced = 36,
//    DssFunctionGreaterEqualEnhanced = 37,
//    DssFunctionLessEqualEnhanced = 38,
//    DssFunctionBetweenEnhanced = 39,
//    DssFunctionBanding = 40,
//    DssFunctionBandingC = 41,
//    DssFunctionBandingP = 42,
//    DssFunctionNotLike = 43,
//    DssFunctionNotBetween = 44,
//    DssFunctionIntersect = 45,
//    DssFunctionIntersectIn = 46,
//    DssFunctionNullToZero = 47,
//    DssFunctionZeroToNull = 48,
//    DssFunctionApplySimple = 49,
//    DssFunctionApplyAggregation = 50,
//    DssFunctionApplyLogic = 51,
//    DssFunctionApplyComparison = 52,
//    DssFunctionApplyRelative = 53,
//    DssFunctionIsNull = 54,
//    DssFunctionIsNotNull = 55,
//    DssFunctionUcase = 56,
//    DssFunctionNotIn = 57,
//    DssFunctionNTile = 58,
//    DssFunctionPercentile = 59,
//    DssFunctionMovingMax = 60,
//    DssFunctionMovingMin = 61,
//    DssFunctionMovingDifference = 62,
//    DssFunctionMovingStdev = 63,
//    DssFunctionExpWghMovingAvg = 64,
//    DssFunctionMovingCount = 65,
//    DssFunctionRunningMax = 66,
//    DssFunctionRunningMin = 67,
//    DssFunctionRunningStdev = 68,
//    DssFunctionRunningCount = 69,
//    DssFunctionExpWghRunningAvg = 70,
//    DssFunctionNotBetweenEnhanced = 71,
//    DssFunctionConcat = 72,
//    DssFunctionFirstInRange = 73,
//    DssFunctionLastInRange = 74,
//    DssFunctionValueSegment = 75,
//    DssFunctionContains = 76,
//    DssFunctionBeginsWith = 77,
//    DssFunctionEndsWith = 78,
//    DssFunctionNotContains = 79,
//    DssFunctionNotBeginsWith = 80,
//    DssFunctionNotEndsWith = 81,
//    DssFunctionCase = 82,
//    DssFunctionCaseV = 83,
//    DssFunctionStdevP = 84,
//    DssFunctionRunningStdevP = 85,
//    DssFunctionMovingStdevP = 86,
//    DssFunctionNTileS = 87,
//    DssFunctionNTileVS = 88,
//    DssFunctionVarP = 89,
//    DssFunctionCurrentDate = 90,
//    DssFunctionDayOfMonth = 91,
//    DssFunctionDayOfWeek = 92,
//    DssFunctionDayOfYear = 93,
//    DssFunctionWeek = 94,
//    DssFunctionMonth = 95,
//    DssFunctionQuarter = 96,
//    DssFunctionYear = 97,
//    DssFunctionCurrentDateTime = 98,
//    DssFunctionCurrentTime = 99,
//    DssFunctionHour = 100,
//    DssFunctionMinute = 101,
//    DssFunctionSecond = 102,
//    DssFunctionMilliSecond = 103,
//    DssFunctionConcatNoBlank = 104,
//    DssFunctionLength = 105,
//    DssFunctionLower = 106,
//    DssFunctionLTrim = 107,
//    DssFunctionPosition = 108,
//    DssFunctionRTrim = 109,
//    DssFunctionSubStr = 110,
//    DssFunctionInitCap = 111,
//    DssFunctionTrim = 112,
//    DssFunctionRightStr = 113,
//    DssFunctionLeftStr = 114,
//    DssFunctionGreatest = 115,
//    DssFunctionLeast = 116,
//    DssFunctionFirst = 117,
//    DssFunctionLast = 118,
//    DssFunctionDate = 119,
//    DssFunctionDaysBetween = 120,
//    DssFunctionMonthsBetween = 121,
//    DssFunctionAddDays = 122,
//    DssFunctionAddMonths = 123,
//    DssFunctionMonthStartDate = 124,
//    DssFunctionMonthEndDate = 125,
//    DssFunctionYearStartDate = 126,
//    DssFunctionYearEndDate = 127,
//    DssFunctionIF = 128,
//    DssFunctionApplyOptional = 129,
//    DssFunctionApplyCSSecurityFilter = 130,
//    DssFunctionUnion = 131,
//    DssFunctionExcept = 132,
//    DssFunctionCoalesce = 133,
//    DssFunctionAdd = 134,
//    DssFunctionAverage = 135,
//    DssFunctionMultiply = 136,
//    DssFunctionBandingM = 137,
//    DssFunctionOLAPSum = 138,
//    DssFunctionOLAPAvg = 139,
//    DssFunctionOLAPCount = 140,
//    DssFunctionOLAPMax = 141,
//    DssFunctionOLAPMin = 142,
//    DssFunctionLag = 143,
//    DssFunctionLead = 144,
//    DssFunctionOLAPRank = 145,
//    DssFunctionRepeat = 146,
//    DssFunctionReservedLastOne,
//    DssFunctionTuple = 1000
//} EnumDSSFunction;

typedef enum DSSQualifier_Type
{
    DssQualifierReserved = 0,
    DssQualifierDistinct,
    DssQualifierAscending,
    DssQualifierDescending
} DSSQualifier_Type;

typedef enum EnumDSSERType
{
    DssERTypeReserved = 0,
    DssERType11 = 1,
    DssERType1M = 2,
    DssERTypeM1 = 3,
    DssERTypeMM = 4,
    DssERTypeParallel = 5,
    DssERTypePerpendicular = 6
} EnumDSSERType;

typedef enum EnumDSSPartialType
{
    DssPartialTypeReserved = 0,
    DssPartialTypeFF = 1,
    DssPartialTypeFP = 2,
    DssPartialTypePF = 3,
    DssPartialTypePP = 4,
    DssPartialTypeDefault = 5
} EnumDSSPartialType;

typedef enum EnumDSSDimensionType
{
    DssDimensionReserved = 0,
    DssDimensionSystem,
    DssDimensionUser,
    DssDimensionOrdered,
    DssDimensionUserHierarchy
} EnumDSSDimensionType;

typedef enum EnumDSSExtnType
{
    DssExtnTypeReserved = 0,
    DssExtnTypeXProduct,
    DssExtnTypeBreak,
    DssExtnTypeFact,
    DssExtnTypeAttribute,
    DssExtnTypeTable,
    DssExtnTypeDegradYes,
    DssExtnTypeDegradNo
} EnumDSSExtnType;

typedef enum EnumDSSJoinType
{
    DssJoinTypeReserved = 0,
    DssJoinTypeSmart,
    DssJoinTypeDirect,
    DssJoinTypeCorrelated,
    DssJoinTypeCross
} EnumDSSJoinType;

typedef enum EnumDSSRoleType
{
    DssRoleTypeReserved = 0,
    DssRoleTypeRole = 1,
    DssRoleTypeTransformation = 2
} EnumDSSRoleType;

typedef enum EnumDSSAttributeType
{
    DssAttributeTypeReserved = 0,
    DssAttributeTypeNormal = 1,
    DssAttributeTypeRole = 2,
    DssAttributeTypeTransformation = 3,
    DssAttributeTypeAbstract = 4
} EnumDSSAttributeType;

typedef enum EnumDSSColumnTypes
{
    DssColumnTypeReserved = 0,
    DssColumnTypeNormal = 1,
    DssColumnTypeCustom = 2
} EnumDSSColumnTypes;

typedef enum EnumDSSLoadSchema
{
    DssSchemaReserved =	0,
    DssSchemaAttributeForms =		0x00000001,
    DssSchemaAttributes =			0x00000002,
    DssSchemaColumns =				0x00000004,
    DssSchemaFactGroups =			0x00000008,
    DssSchemaFacts =				0x00000010,
    DssSchemaFunctions =			0x00000020,
    DssSchemaHierarchies =			0x00000040,
    DssSchemaRoles =				0x00000080,
    DssSchemaTables =				0x00000100,
    DssSchemaDbTables =				0x00000200,
    DssSchemaAll =					0x000003FF
} EnumDSSLoadSchema;

typedef enum EnumDSSSourceFlags
{
    DssSourceDefault = 0x00000000,
    DssSourceDefn = 0x00000001,
    DssSourceBrowser = 0x00000002,
    DssSourceDates = 0x00000004,
    DssSourceComments = 0x00000008,
    DssSourceDependents = 0x00000010,
    DssSourceProperties = 0x00000020,
    DssSourceSecurity = 0x00000040,
    DssSourceTotalObject = 0x0000007f,
    DssSourceChangeJournalComments = 0x00000080,
    DssSourceBrowserWithoutNameAndDescription = 0x00000080,
    DssSourceBrowserWithoutOwner = 0x00008000,
    DssSourceDepnDefn = 0x00000100,
    DssSourceDepnBrowser = 0x00000200,
    DssSourceDepnDates = 0x00000400,
    DssSourceDepnComments = 0x00000800,
    DssSourceDepnDependents = 0x00001000,
    DssSourceDepnProperties = 0x00002000,
    DssSourceDepnSecurity = 0x00004000,
    DssSourceTotalDepn = 0x00007f00,
    DssSourceDoNotValidateEmbeddedObjects = 0x00008000,
    DssSourceRecurseDepns = 0x00010000,
    DssSourceSkipNameTypeUniqueCheck = 0x00008000,
    DssSourceLocalSerialization = 0x00010000,
    DssSourceRemoteOnly = 0x00010000,
    DssSourceDoNotCache = 0x00020000,
    DssSourceLocalOnly = 0x00040000,
    DssSourceCacheOnly = DssSourceLocalOnly  | DssSourceRemoteOnly,
    DssSourceProbeOnly = 0x00080000,
    DssSourceChangeModificationTime = 0x00080000,
    DssSourceGCAllProjects = 0x00100000,
    DssSourceCreateProfileAnyway = 0x00100000,
    DssSourceDepnMissingIgnore = 0x00400000,
    DssSourceSerializeNoInstance = 0x00100000,
    DssSourceSerializeForPersistence = 0x00200000,
    DssSourceSaveObjectsIgnoreTableACL = 0x00400000,
    DssSourceCopyIdentical = 0x40000000,
    DssSourceCopyIsolated = 0x20000000,
    DssSourceSaveOverwrite = 0x01000000,
    DssSourceCopyMakeDependents = 0x01000000,
    DssSourceDeleteForce = 0x01000000,
    DssSourceCreateUserGroup = 0x01000000,
    DssSourceLoadFromCache = 0x20000000,
    DssSourceRefreshFromMD = 0x40000000,
    DssSourcePurgeClient = 0x01000000,
    DssSourcePurgeServer = 0x02000000,
    DssSourcePurgeConfiguration = 0x04000000,
    DssSourcePurgeIncludePinned = 0x08000000,
    DssSourceAltFolderACLInheritance = 0x02000000,
    DssSourceChangeRelativeID = 0x80000000,
    DssSourceValidateDependents = 0x04000000,
    DssSourceDoNotValidateDependents = 0x80000000,
    DssSourceCopyWithExternalEmbeddedReference = 0x08000000,
    DssSourceXMLCopyRemoveNonPrimaryNameTranslations = 0x02000000,
    DssSourceSetInitial = 0x10000000,
    DssSourceAncestors = 0x10000000,
    DssSourceFindHidden = 0x20000000,
    DssSourcePlainText = 0x40000000,
    DssSourceIncludeMe = 0x08000000,
    DssSourceOwner = 0x04000000,
    DssSourceXMLMask = DssSourceTotalObject  | DssSourceTotalDepn | DssSourceAncestors | DssSourceFindHidden | DssSourceIncludeMe | DssSourcePlainText | DssSourceOwner,
    DssSourceQueryProjectID = 0x02000000,
    DssSourceObjExistWhenUpdt = 0x20000000,
    DssSourceSaveMove = 0x20000000,
    DssSourceSaveRootObject = 0x40000000,
    DssSourceFindFresh = 0x10000000,
    DssSourceFindWriteable = 0x04000000,
    DssSourcePreserveVersion = 0x08000000,
    DssSourceRefreshObject = 0x08000000,
    DssSourceSkipOrderFolders = 0x10000000,
    DssSourceBypassAccessChecks = 0x00004000,
    DssSourceInheritACL = 0x00002000,
    DssSourceSaveInfoSelective = 0x00001000,
    DssSourceSkipValidateTrustee = 0x00000800,
    DssSourceSkipSchemaDepnValidation = 0x00000400,
    DssSourceSkipUserPropertyUniquenessValidation = 0x00000400,
    DssSourceSkipMappingID = 0x00800000,
    DssSourceBestEffort = 0x00008000
} EnumDSSSourceFlags;

typedef enum EnumDSSFolderType
{
    DssFolderReserved =0,
    DssFolderUser = 1,
    DssFolderSystem = 2,
    DssFolderSearch = 4,
    DssFolderImage = 5,
    DssFolderContainer = 7,
    DssFolderEmbedded = 8,
    DssFolderSearchCrossProject = 9
} EnumDSSFolderType;

typedef enum EnumDSSCatalogRestriction
{
    DssDBCRestrictClear = -1,
    DssDBCRestrictReserved = 0,
    DssDBCRestrictTablePrefix = 1,
    DssDBCRestrictOwner = 2,
    DssDBCRestrictRowLimit = 3,
    DssDbcRestrictColumn = 4
} EnumDSSCatalogRestriction;

typedef enum EnumDSSCatalogUpdateFlags
{
    DssDbcUpdateReserved =	0,
    DssDbcUpdateTables =	1,
    DssDbcUpdateAll =	2,
    DssDbcUpdateColumns =	3,
    DssDbcGetTableContent = 4,
    DssDbcGetTableSize = 5,
    DssDbcGetColumnCardinality = 6,
    DssDbcGetColumnContent = 7,
    DssDbcGetSourceOrder = 8,
    DssDbcGetDefaultSQL = 0x0100,
    DssDbcConsolidateColumns = 0x0200,
    DssDbcUseODBC = 0x0400,
    DssDbcIgnoreOldDataType = 0x0800,
    DssDbcUpperCaseNames = 0x1000,
    DssDbcIgnoreNameSpace = 0x2000,
    DssDbcGetTableKeys = 0x4000,
    DssDbcUpdateWithTableSize = 0x8000,
    DssDbcMDXSAPGetFlatHierarchyLevel00 = 0x00100000,
    DssDbcMDXGetMeasuresInDimension = 0x00200000
} EnumDSSCatalogUpdateFlags;

//typedef enum EnumDSSAccessEntryType
//{
//    DssAccessEntryTypeReserved = 0,
//    DssAccessEntryTypeObject = 1,
//    DssAccessEntryTypeAudit = 2
//} EnumDSSAccessEntryType;

typedef enum EnumDSSPropagateACL
{
    DssPropagateACLReserved = 0,
    DssPropagateACLDirectChildren = 0x01,
    DssPropagateACLTemporary = 0x02,
    DssPropagateACLReplace = 0x04,
    DssPropagateACLPrecisely = 0x08
} EnumDSSPropagateACL;

typedef enum EnumDSSComponentRole
{
    DssRoleUnknown = -1,
    DssRoleReserved = 0,
    DssRoleAnalyticalEngineServer = 1,
    DssRoleConfigurationSource = 2,
    DssRoleDBElementServer = 3,
    DssRoleElementNetClient = 4,
    DssRoleElementNetServer = 5,
    DssRoleElementOutSource = 6,
    DssRoleElementSource = 7,
    DssRoleInitiationSource = 8,
    DssRoleMD4Server = 9,
    DssRoleMD7Server = 10,
    DssRoleMetaDataServer = 11,
    DssRoleObjectOutSource = 12,
    DssRoleObjectSource = 13,
    DssRoleProjectSource = 14,
    DssRoleQueryEngineServer = 15,
    DssRoleReportExecutor = 16,
    DssRoleReportNetClient = 17,
    DssRoleReportNetServer = 18,
    DssRoleReportOutSource = 19,
    DssRoleReportSource = 20,
    DssRoleResolutionServer = 21,
    DssRoleServerNet = 22,
    DssRoleSourceNetClient = 23,
    DssRoleSourceNetServer = 24,
    DssRoleSQLEngineServer = 25,
    DssRoleSQLsServer = 26,
    DssRoleXTabFactory = 27,
    DssRoleAuthServer = 28,
    DssRoleClientServices = 29,
    DssRoleEngineLite = 30,
    DssRoleFunctionServer = 31,
    DssRoleDocumentSource = 32,
    DssRoleDocumentNetClient = 33,
    DssRoleDocumentNetSource = 34,
    DssRoleDocumentOutSource = 35,
    DssRoleDocumentNetServer = 36,
    DssRoleScriptServer = 37,
    DssRoleScriptEngine = 38,
    DssRoleCacheFileObjectSource = 39,
    DssRoleDatamartServer = 40,
    DssRoleMX2ETLServer = 41,
    DssRoleReportServices = 42,
    DssRoleDocumentServices = 43,
    DssRoleExportEngineServer = 44,
    DssRoleImportServices = 45,
    DssRoleObjectContextFactory = 46,
    DssRoleCubeServer = 47,
    DssRoleCubeNetClient = 48,
    DssRoleCubeNetServer = 49,
    DssRoleCliCubeServer = 50,
    DssRoleCatalogServer2 = 51,
    DssRoleCatalogSource = 52,
    DssRoleCatalogNetClient = 53,
    DssRoleCatalogNetServer = 54,
    DssRoleCatalogOutSource = 55,
    DssRoleSchemaManipulator = 56,
    DssRoleLastOne
} EnumDSSComponentRole;

typedef enum EnumDSSSourceComponentRole
{
    DssSourceRoleUnknown = -1,
    DssSourceRoleReserved = 0,
    DssSourceRoleMetadataServer = 1,
    DssSourceRoleContentSource = 2,
    DssSourceRoleSourceNetClient = 3,
    DssSourceRoleContentOutSource = 4,
    DssSourceRoleCacheFileObjectSource = 5,
    DssSourceRoleSearchServices = 6,
    DssSourceRoleCatalogServer2 = 7,
    DssSourceRoleCatalogContentSource = 8,
    DssSourceRoleCatalogNetClient = 9,
    DssSourceRoleLastOne
} EnumDSSSourceComponentRole;

typedef enum EnumDSSDataSourceType
{
    DssDataSourceTypeReserved = 0,
    DssDataSourceTypeConfiguration = 1,
    DssDataSourceTypeServer = 2,
    DssDataSourceTypeProjectMD7 = 3,
    DssDataSourceTypeProjectMD4 = 4,
    DssDataSourceTypeCacheFiles = 5,
    DssDataSourceTypeLastOne
} EnumDSSDataSourceType;

typedef enum EnumDSSDataSourceRefreshOption
{
    DssDataSourceRefreshOptionReserved = 0,
    DssDataSourceRefreshOptionNotRefresh,
    DssDataSourceRefreshOptionRefresh,
    DssDataSourceListUserProjects =		0x40
} EnumDSSDataSourceRefreshOption;

typedef enum EnumDSSConnectionMode
{
    DssConnectionModeReserved = 0,
    DssConnectionModeSimpleServer = 1,
    DssConnectionModeDirectAccess = 2,
    DssConnectionModeServerAccess = 3,
    DssConnectionModeWebAccess = 4,
    DssConnectionModeCastorServer = 5,
    DssConnectionModeOffline = 6,
    DssConnectionModeLastOne
} EnumDSSConnectionMode;

typedef enum EnumDSSApplicationType
{
    DssApplicationReserved = 0,
    DssApplicationOffice = 1,
    DssApplicationServerAdmin = 2,
    DssApplicationWebAdmin = 3,
    DssApplicationServer = 4,
    DssApplicationProjectUpgrade = 5,
    DssApplicationDSSWeb = 6,
    DssApplicationDSSScheduler = 7,
    DssApplicationCustomApp = 8,
    DssApplicationNarrowcastServer = 9,
    DssApplicationObjectManager = 10,
    DssApplicationODBOProvider = 11,
    DssApplicationODBOCubeDesigner = 12,
    DssApplicationCommandManager = 13,
    DssApplicationEnterpriseManager = 14,
    DssApplicationCommandLineInterface = 15,
    DssApplicationProjectBuilder = 16,
    DssApplicationConfigurationWizard = 17,
    DssApplicationMDScan = 18,
    DssApplicationCacheUtility = 19,
    DssApplicationFireEvent = 20,
    DssApplicationTypeJavaDesktop = 21,
    DssApplicationWebServices = 22,
    DSSApplicationOfficeWS = 23,
    DssApplicationTools = 24,
    DSSApplicationPortal = 25,
    DssApplicationTester = 26,
    DssApplicationMDUpdate = 27,
    DssApplicationCOMBrowser = 28,
    DSSApplicationSchedulerMobile = 29,
    DSSApplicationBulkTranslationTool = 30,
    DSSApplicationHealthCenter = 31,
    DSSApplicationCubeAdvisor = 32,
    DssApplicationReservedLastOne
} EnumDSSApplicationType;

typedef enum EnumDSSMDVersion
{
    DssMDVersionReserved = 0,
    DssMDVersionMD4 = 4,
    DssMDVersionMD7 = 6
} EnumDSSMDVersion;

typedef enum EnumDSSDataSourceFlags
{
    DssDataSourceUseWHPartition =			0x0001,
    DssDataSourceUsePersistentID =			0x0002,
    DssDataSourceMDConnectionMultiProcess = 0x0004,
    DssDataSourceUseProjectCacheObjectMaxMemoryConsumption = 0x0008,
    DssDataSourceUseProjectCacheElementMaxMemoryConsumption = 0x0010
} EnumDSSDataSourceFlags;

typedef enum EnumDSSCacheSetting
{
    DssCacheSettingReserved =	0,
    DssCacheSettingObjectMaxCacheCount,
    DssCacheSettingObjectMaxMemoryConsumption,
    DssCacheSettingObjectCacheTableBucketCount,
    DssCacheSettingObjectCacheTableBucketPerLock,
    DssCacheSettingObjectFileCachePath,
    DssCacheSettingObjectSwapPolicy,
    DssCacheSettingObjectCacheLifeTime,
    DssCacheSettingElementMaxCacheCount,
    DssCacheSettingElementMaxMemoryConsumption,
    DssCacheSettingElementCacheTableBucketCount,
    DssCacheSettingElementCacheTableBucketPerLock,
    DssCacheSettingElementFileCachePath,
    DssCacheSettingElementSwapPolicy,
    DssCacheSettingElementCacheLifeTime,
    DssCacheSettingReportMaxCacheCount,
    DssCacheSettingReportMaxMemoryConsumption,
    DssCacheSettingReportCacheTableBucketCount,
    DssCacheSettingReportCacheTableBucketPerLock,
    DssCacheSettingReportFileCachePath,
    DssCacheSettingReportSwapPolicy,
    DssCacheSettingReportCacheLifeTime,
    DssCacheSettingReportDisableCaching,
    DssCacheSettingReportCacheDBMatchingFlag,
    DssCacheSettingLinkCacheMaxMemConsumption,
    DssCacheSettingLinkCacheTableBucketCount,
    DssCacheSettingLinkCacheTableBucketPerLock,
    DssCacheSettingLinkCacheSwapPolicy,
    DssCacheSettingReportCacheLoadOnStart,
    DssCacheSettingMaxElementRowCount,
    DssCacheSettingElementCacheUsesSecurityFilters,
    DssCacheSettingElementCacheUseConnectionMap,
    DssCacheSettingElementCacheUsePassThroughLogin,
    DssCacheSettingPDFMaxMemoryConsumption,
    DssCacheSettingXMLMaxMemoryConsumption,
    DssCacheSettingExcelMaxMemoryConsumption,
    DssCacheSettingReportCacheSwapFactor,
    DssCacheSettingMaxCubeCount,
    DssCacheSettingMaxCubeMemUsage,
    DssCacheSettingCubeStorage,
    DssCacheSettingRWDMaxCacheCount,
    DssCacheSettingRWDMaxMemoryConsumption,
    DssCacheSettingRWDEnableCaching,
    DssCacheSettingCubeDBMatchingFlag,
    DssCacheSettingRWDCacheStorage,
    DssCacheSettingCacheEncryptionOption,
    DssCacheSettingCacheInfoFieldExcluded,
    DssCacheSettingCacheIgnoreDynamicDate,
    DssCacheSettingCubeLoadOnStart,
    DssCacheSettingHtmlMaxMemoryConsumption,
    DssCacheSettingCubePublishOption,
    DssCacheSettingCubeQuota,
    DssCacheSettingReportCacheLKUPMaxRAM,
    DssCacheSettingRWDCacheLKUPMaxRAM,
    DssCacheSettingCubeIndexGrowthCheckFreq,
    DssCacheSettingCubeIndexGrowthUpperBound,
    DssCacheSettingCatalogMaxMemoryConsumption,
    DssCacheSettingLastOne
} EnumDSSCacheSetting;

typedef enum EnumDSSCreateUser
{
    DssCreateUserReserved = 0,
    DssCreateUserMakeInbox = 0x01,
    DssCreateUserDoNotSave = 0x02,
    DssCreateUserImported = 0x04
} EnumDSSCreateUser;

typedef enum EnumDSSListGroup
{
    DssListGroupReserved = 0,
    DssListGroupSystemGroups = 0x01,
    DssListGroupRootGroups = 0x02
} EnumDSSListGroup;

typedef enum EnumDSSListUser
{
    DssListUserReserved = 0,
    DssListUserEnabledUsers = 0x01,
    DssListUserDisabledUsers = 0x02,
    DssListUserNoNormalUsers = 0x04,
    DssListUserNoSystemUsers = 0x08,
    DssListUserNoPseudoUsers = 0x10,
    DssListUserNoPlaceholders = 0x20
} EnumDSSListUser;

typedef enum EnumDSSPropertySet
{
    DssPropertySetReserved = 0,
    DssPropertySetVLDBSelect = 1,
    DssPropertySetVLDBReport = 2,
    DssPropertySetVLDBCreateTable = 3,
    DssPropertySetVLDBInsert = 4,
    DssPropertySetVLDBDropTable = 5,
    DssPropertySetVLDBDataType = 6,
    DssPropertySetVLDBFunction = 7,
    DssPropertySetVLDBSyntax = 8,
    DssPropertySetVLDBMisc = 9,
    DssPropertySetSchemaObjectFolders = 10,
    DssPropertySetPublicObjectFolders = 11,
    DssPropertySetTemplateObjectFolders = 12,
    DssPropertySetProjectProfileFolders = 13,
    DssPropertySetAuxiliaryObjectFolders = 14,
    DssPropertySetProfileObjectFolders = 15,
    DssPropertySetWebProperties = 16,
    DssPropertySetResultProperties = 17,
    DssPropertySetGraphInfo = 18,
    DssPropertySetPreferenceSet = 19,
    DssPropertySetCastorArch = 20,
    DssPropertySetTableDefinition = 21,
    DssPropertySetFormCategories = 22,
    DssPropertySetExpressions = 23,
    DssPropertySetWHCatalog = 24,
    DssPropertySetPrinting = 25,
    DssPropertySetTemplateFormatting = 26,
    DssPropertySetAutostyles = 27,
    DssPropertySetHeaderProperties = 28,
    DssPropertySetMeasureEditor = 29,
    DssPropertySetDefaultProjectProperties = 30,
    DssPropertySetServerConnection = 31,
    DssPropertySetAnalyticalEngineProperties = 32,
    DssPropertySetFactProperties = 33,
    DssPropertySetAttributeProperties = 34,
    DssPropertySetSystemObjectsFolder = 35,
    DssPropertySetMDSecurityLink = 36,
    DssPropertySetExportProperties = 37,
    DssPropertySetExportAdvancedProperties = 38,
    DssPropertySetGeneralTemplateProperties = 39,
    DssPropertySetSchemaUpdateProperties = 40,
    DssPropertySetColumnProperties = 41,
    DssPropertySetMDVersion = 42,
    DssPropertySetPersonalizationProperties = 43,
    DssPropertySetDesktopShortcuts = 44,
    DssPropertySetReportLayoutsCategories = 45,
    DssPropertySetReportLayouts = 46,
    DssPropertySetProjectBuilderInfo = 47,
    DssPropertySetLayoutInfo = 48,
    DssPropertySetProjectBuilderProjectInfo = 49,
    DssPropertySetDatamart = 50,
    DssPropertySetPBuilderObjectFolders = 51,
    DssPropertySetGeneralProperties = 52,
    DssPropertySetGeneralReportProperties = 53,
    DssPropertySetAuthenticationProperties = 54,
    DssPropertySetGraphicalLayout = 55,
    DssPropertySetFormattingNumber = 56,
    DssPropertySetFormattingAlignment = 57,
    DssPropertySetFormattingFont = 58,
    DssPropertySetFormattingBorder = 59,
    DssPropertySetFormattingPatterns = 60,
    DssPropertySetFormattingBanding = 61,
    DssPropertySetScheduling = 62,
    DssPropertySetReserved0 = 63,
    DssPropertySetSystemSQLTemplates = 64,
    DssPropertySetDBConnectionTimeouts = 65,
    DssPropertySetSystemObjectsVersion = 66,
    DssPropertySetElementServerProperties = 67,
    DssPropertySetOldStyleScheduling = 68,
    DssPropertySetTrivial = 69,
    DssPropertySetConfigWebProperties = 70,
    DssPropertySetReserved1 = 71,
    DssPropertySetVLDBGeneral = 72,
    DssPropertySetWebPreferences = 73,
    DssPropertySetAttributeElementProperties = 74,
    DssPropertySetColumnWidth = 75,
    DssPropertySetExportPDFSettings = 76,
    DssPropertySetWebPrintSettings = 77,
    DssPropertySetProjectObjectsFolder = 78,
    DssPropertySetProjectProperties = 79,
    DssPropertySetObjectTemplateProperties = 80,
    DssPropertySetDMXModelProperties = 81,
    DssPropertySetDataMiningExtensions = 82,
    DssPropertySetFormattingPosition = 83,
    DssPropertySetFormattingSize = 84,
    DssPropertySetFormattingAppearance = 85,
    DssPropertySetFormattingPadding = 86,
    DssPropertySetFormattingPicture = 87,
    DssPropertySetFormattingLine = 88,
    DssPropertySetFormattingCrosstab = 89,
    DssPropertySetFormattingSection = 90,
    DssPropertySetFormattingPage = 91,
    DssPropertySetFormattingNavigation = 92,
    DssPropertySetFormattingGroup = 93,
    DssPropertySetFormattingOrder = 94,
    DssPropertySetDocumentStyles = 95,
    DssPropertySetPromptProperties = 96,
    DssPropertySetDocumentProperties = 97,
    DssPropertySetMDXSyntax = 98,
    DssPropertySetMDXMemberProperties = 99,
    DssPropertySetVisualization = 100,
    DssPropertySetFormattingView = 101,
    DssPropertySetFormattingSelector = 102,
    DssPropertySetFormattingWidget = 103,
    DssPropertySetFormattingTransition = 104,
    DssPropertySetFormattingLayout = 105,
    DssPropertySetFormattingHTML = 106,
    DssPropertySetPromptLayoutProperties = 107,
    DssPropertySetPromptSearchProperties = 108,
    DssPropertySetExpressionPromptProperties = 109,
    DssPropertySetTableOfContents = 110,
    DssPropertySetBulkExport = 111,
    DssPropertySetFilterDetailsFormatProperties = 112,
    DssPropertySetWatermarkProperties = 113,
    DssPropertySetConfigurationProperties = 114,
    DssPropertySetLocaleProperties = 115,
    DssPropertySetAnnotations = 116,
    DssPropertySetScheduleProperties = 117,
    DssPropertySetIncrementalFetchProperties = 118,
    DssPropertySetMobile = 119,
    DssPropertySetGraphicalArchitect = 120,
    DssPropertySetFlashCompatibility = 121,
    DssPropertySetCustomSQLWriteback = 122,
    DssPropertySetFormattingAnalysis = 123,
    DssPropertySetFormattingTransactions = 124,
    DssPropertySetReservedLastOne
} EnumDSSPropertySet;

typedef enum EnumDSSAttributeDirection
{
    DssDirectionReserved = 0,
    DssDirectionAttributeOnly,
    DssDirectionParent,
    DssDirectionChildren,
    DssDirectionParentAndChildren
} EnumDSSAttributeDirection;

typedef enum EnumDSSRelationship
{
    DssRelationshipReserved = 0,
    DssRelationshipNone,
    DssRelationshipChild,
    DssRelationshipParent,
    DssRelationshipTop,
    DssRelationshipBottom
} EnumDSSRelationship;

//typedef enum EnumDSSBaseFormType
//{
//    DssBaseFormReserved = 0,
//    DssBaseFormDateTime,
//    DssBaseFormNumber,
//    DssBaseFormText,
//    DssBaseFormPicture,
//    DssBaseFormUrl,
//    DssBaseFormEmail,
//    DssBaseFormHTMLTag,
//    DssBaseFormDate,
//    DssBaseFormTime,
//    DssBaseFormSymbol,
//    DssBaseFormBigDecimal,
//    DssBaseFormPhoneNumber,
//    DssBaseFormLastOne
//} EnumDSSBaseFormType;

typedef enum EnumDSSDataTypeUse
{
    DssDataTypeUseReserved = 0,
    DssDataTypeUseDefault = 1,
    DssDataTypeUseThis = 2
} EnumDSSDataTypeUse;

typedef enum EnumDSSAttributeLockType
{
    DssLockReserved = 0,
    DssLockNone,
    DssLockCustom,
    DssLockLimit
} EnumDSSAttributeLockType;

typedef enum EnumDSSRootType
{
    DssRootReserved = 0,
    DssRootProject,
    DssRootConfiguration
} EnumDSSRootType;

typedef enum DSSForms_Type
{
    DssFormsReserved = 0,
    DssFormsTemplate,
    DssFormsBrowse
} DSSForms_Type;

typedef enum EnumDSSCreateFlags
{
    DssCreateReserved = 0,
    DssCreateProject = 1,
    DssCreateConfiguration = 2,
    DssCreateServerInstance = 3,
    DssCreateEntityMask = 0x0FFF,
    DssCreateFromString = 0x1000,
    DssCreateOnServerSide = 0x2000,
    DssCreateChangeJournal = 0x4000,
    DssCreateEnableQuickSearch = 0x8000
} EnumDSSCreateFlags;

typedef enum EnumDSSDeleteFlags
{
    DssDeleteReserved = 0,
    DssDeleteProject = 1,
    DssDeleteServerInstance = 2
} EnumDSSDeleteFlags;

typedef enum EnumDSSInternalDeleteFlags
{
    DssInternalDeleteReserved = 0,
    DssInternalDeleteProjectRollback = 0x00000001,
    DssInternalDeleteSkipAccessChecks = 0x00000002,
    DssInternalDeleteSkipCleanupLinkItems = 0x00000004
} EnumDSSInternalDeleteFlags;

//typedef enum EnumDSSPrivilegeType
//{
//    DssPrivilegesReserved = 0,
//    DssPrivilegesCreateAppObj = 1,
//    DssPrivilegesCreateConfigObj = 2,
//    DssPrivilegesCreateSchemaObj = 3,
//    DssPrivilegesScheduleRequest = 4,
//    DssPrivilegesUseArchitect = 5,
//    DssPrivilegesWebReportDetails = 6,
//    DssPrivilegesUseServerAdmin = 7,
//    DssPrivilegesUseMetricEditor = 8,
//    DssPrivilegesUseFilterEditor = 9,
//    DssPrivilegesUseTemplateEditor = 10,
//    DssPrivilegesUseReportEditor = 11,
//    DssPrivilegesUseConsolidationEditor = 12,
//    DssPrivilegesUseCustomGroupEditor = 13,
//    DssPrivilegesUseDocumentEditor = 14,
//    DssPrivilegesUsePromptEditor = 15,
//    DssPrivilegesBypassAccessChecks = 16,
//    DssPrivilegesWebAdministrator = 17,
//    DssPrivilegesWebUser = 18,
//    DssPrivilegesWebViewHistoryList = 19,
//    DssPrivilegesWebReportManipulations = 20,
//    DssPrivilegesWebPivotReport = 20,
//    DssPrivilegesWebCreateNewReport = 21,
//    DssPrivilegesWebObjectSearch = 22,
//    DssPrivilegesWebChangeUserOptions = 23,
//    DssPrivilegesWebSaveReport = 24,
//    DssPrivilegesWebDrilling = 25,
//    DssPrivilegesWebAdvancedDrilling = 25,
//    DssPrivilegesWebExport = 26,
//    DssPrivilegesWebPrintMode = 27,
//    DssPrivilegesWebDeleteObject = 28,
//    DssPrivilegesWebManageObject = 28,
//    DssPrivilegesWebPublish = 29,
//    DssPrivilegesUseServerCache = 30,
//    DssPrivilegesWebReportSQL = 31,
//    DssPrivilegesReserved2 = 32,
//    DssPrivilegesCreateDatamart = 33,
//    DssPrivilegesUseDatamartEditor = 34,
//    DssPrivilegesUseObjectManager = 35,
//    DssPrivilegesWebAddToHistoryList = 36,
//    DssPrivilegesWebChangeViewMode = 37,
//    DssPrivilegesWebNormalDrilling = 38,
//    DssPrivilegesWebDrillOnMetrics = 39,
//    DssPrivilegesWebFormatGridAndGraph = 40,
//    DssPrivilegesWebScheduling = 41,
//    DssPrivilegesWebSubscriptions = 41,
//    DssPrivilegesWebScheduleReport = 41,
//    DssPrivilegesWebSimultaneousExecution = 42,
//    DssPrivilegesWebSwitchPageByElements = 43,
//    DssPrivilegesWebSort = 44,
//    DssPrivilegesUseVLDBPropertyEditor = 45,
//    DssPrivilegesWebSaveTemplatesAndFilters = 46,
//    DssPrivilegesWebFilterOnSelections = 47,
//    DssPrivilegesScheduleAdministration = 48,
//    DssPrivilegesReserved3 = 49,
//    DssPrivilegesWebScheduleEMail = 49,
//    DssPrivilegesUseUserManager = 50,
//    DssPrivilegesUseJobMonitor = 51,
//    DssPrivilegesUseUserConnectionMonitor = 52,
//    DssPrivilegesUseDatabaseConnectionMonitor = 53,
//    DssPrivilegesUseScheduleMonitor = 54,
//    DssPrivilegesUseCacheMonitor = 55,
//    DssPrivilegesUseClusterMonitor = 56,
//    DssPrivilegesUseDatabaseInstanceManager = 57,
//    DssPrivilegesUseScheduleManager = 58,
//    DssPrivilegesUseProjectMonitor = 59,
//    DssPrivilegesReserved4 = 60,
//    DssPrivilegesWebSendNow = 60,
//    DssPrivilegesBypassSchemaAccessChecks = 61,
//    DssPrivilegesModifySchemaObjects = 61,
//    DssPrivilegesViewETLInformation = 62,
//    DssPrivilegesUseReportObjectsWindow = 63,
//    DssPrivilegesUseThresholdsEditor = 64,
//    DssPrivilegesUseFormattingEditor = 65,
//    DssPrivilegesSaveCustomAutoStyle = 66,
//    DssPrivilegesUseReportFilterEditor = 67,
//    DssPrivilegesCreateDerivedMetrics = 68,
//    DssPrivilegesModifySubtotals = 69,
//    DssPrivilegesModifyReportObjectList = 70,
//    DssPrivilegesWebCreateDerivedMetrics = 71,
//    DssPrivilegesWebModifySubtotals = 72,
//    DssPrivilegesWebModifyReportList = 73,
//    DssPrivilegesWebUseReportObjectsWindow = 74,
//    DssPrivilegesWebUseReportFilterEditor = 75,
//    DssPrivilegesWebUseDesignMode = 76,
//    DssPrivilegesWebAliasObjects = 77,
//    DssPrivilegesWebConfigureToolbars = 78,
//    DssPrivilegesWebUseQueryFilterEditor = 79,
//    DssPrivilegesWebReExecuteReportAgainstWH = 80,
//    DssPrivilegesSetAttributeDisplay = 81,
//    DssPrivilegesChooseAttributeFromDisplay = 81,
//    DssPrivilegesUseHistoryList = 82,
//    DssPrivilegesConfigureToolbars = 83,
//    DssPrivilegesChangeUserPreferences = 84,
//    DssPrivilegesUseReportDataOptions = 85,
//    DssPrivilegesUseDataExplorer = 86,
//    DssPrivilegesFormatGraph = 87,
//    DssPrivilegesModifySorting = 88,
//    DssPrivilegesViewSQL = 89,
//    DssPrivilegesCreateNewFolder = 90,
//    DssPrivilegesPivotReport = 91,
//    DssPrivilegesUseDesignMode = 92,
//    DssPrivilegesAliasObjects = 93,
//    DssPrivilegesUseGridOptions = 94,
//    DssPrivilegesSecondChuckStart = 95,
//    DssPrivilegesUseSearchEditor = 95,
//    DssPrivilegesReExecuteReportAgainstWH = 96,
//    DssPrivilegesUseDrillMapEditor = 97,
//    DssPrivilegesSendToEMail = 98,
//    DssPrivilegesUseFunctionPlugInEditor = 99,
//    DssPrivilegesWebSimpleGraphFormatting = 100,
//    DssPrivilegesWebUseLockedHeaders = 101,
//    DssPrivilegesWebSetColumnWidths = 102,
//    DssPrivilegesUseSecurityFilterManager = 103,
//    DssPrivilegesUseProjectStatusEditor = 104,
//    DssPrivilegesUseProjectDocumentation = 105,
//    DssPrivilegesWebExecuteDatamartReport = 106,
//    DssPrivilegesUseSubtotalEditor = 107,
//    DssPrivilegesUseFindAndReplaceDialog = 108,
//    DssPrivilegesUseCommandManager = 109,
//    DssPrivilegesUseRWDocumenEditor = 110,
//    DssPrivilegesExecuteRWDocument = 111,
//    DssPrivilegesWebExecuteRWDocument = 112,
//    DssPrivilegesUseOfficeIntelligence = 113,
//    DssPrivilegesWebCreateEmailAddress = 114,
//    DssPrivilegesWebPrintNow = 115,
//    DssPrivilegesWebScheduledPrinting = 116,
//    DssPrivilegesWebCreatePrintLocation = 117,
//    DssPrivilegesWebExportToFileNow = 118,
//    DssPrivilegesWebScheduledExportToFile = 119,
//    DssPrivilegesWebCreateFileLocation = 120,
//    DssPrivilegesServerMonitoring = 121,
//    DssPrivilegesDefineCustomSQLReport = 122,
//    DssPrivilegesDefineFreeformSQLReport = 122,
//    DssPrivilegesDefineOLAPCubeReport = 123,
//    DssPrivilegesImportOLAPCube = 124,
//    DssPrivilegesWebDocumentDesign = 125,
//    DssPrivilegesWebManageDocumentDatasets = 126,
//    DssPrivilegesWebModifyGridLevelInDocument = 127,
//    DssPrivilegesWebModifyUnitInGridInDocument = 127,
//    DssPrivilegesWebSetAttributeFormDisplay = 128,
//    DssPrivilegesWebChooseAttributeFormDisplay = 128,
//    DssPrivilegesWebExecuteBulkExportReport = 129,
//    DssPrivilegesWebExecuteBulkExport = 129,
//    DssPrivilegesWebNumberFormatting = 130,
//    DssPrivilegesUseDesktop = 131,
//    DssPrivilegesCreateShortcut = 132,
//    DssPrivilegesWebEnableMMTAccess = 133,
//    DssPrivilegesDrillWithIntelligentCube = 134,
//    DssPrivilegesDefineQueryBuilderReport = 135,
//    DssPrivilegesWebDefineQueryBuilderReport = 136,
//    DssPrivilegesUseMstrMobile = 137,
//    DssPrivilegesMobileViewDocument = 138,
//    DssPrivilegesUseIntegrityManager = 139,
//    DssPrivilegesCacheAdministration = 140,
//    DssPrivilegesUseInboxMonitor = 141,
//    DssPrivilegesInboxAdministration = 142,
//    DssPrivilegesUseCubeMonitor = 143,
//    DssPrivilegesThirdChuckStart = 144,
//    DssPrivilegesCubeAdministration = 144,
//    DssPrivilegesFireEvent = 145,
//    DssPrivilegesUseTransmitterDeviceManager = 146,
//    DssPrivilegesUseContactManager = 147,
//    DssPrivilegesUseSecurityRoleManager = 148,
//    DssPrivilegesAssignSecurityRoles = 149,
//    DssPrivilegesConfigureContactDataProfile = 150,
//    DssPrivilegesResetUserPasswords = 151,
//    DssPrivilegesLinkToExternalAccounts = 152,
//    DssPrivilegesGrantPrivilege = 153,
//    DssPrivilegesConfigureGroupMembership = 154,
//    DssPrivilegesUseDatabaseLoginManager = 155,
//    DssPrivilegesDefineSecurityFilter = 156,
//    DssPrivilegesClusterAdministration = 157,
//    DssPrivilegesLoadProject = 158,
//    DssPrivilegesIdleProject = 159,
//    DssPrivilegesDefineViewReport = 160,
//    DssPrivilegesUseCubeReportEditor = 161,
//    DssPrivilegesExecuteCubeReport = 162,
//    DssPrivilegesWebDefineViewReport = 163,
//    DssPrivilegesWebExecuteCubeReport = 164,
//    DssPrivilegesConfigureServerBasic = 165,
//    DssPrivilegesConfigureStatistics = 166,
//    DssPrivilegesConfigureSecurity = 167,
//    DssPrivilegesConfigureInbox = 168,
//    DssPrivilegesConfigureGoverning = 169,
//    DssPrivilegesConfigureCaches = 170,
//    DssPrivilegesConfigureProjectBasic = 171,
//    DssPrivilegesConfigureConnectionMap = 172,
//    DssPrivilegesConfigureProjectDataSource = 173,
//    DssPrivilegesEnableUser = 174,
//    DssPrivilegesConfigureSubscriptionSettings = 175,
//    DssPrivilegesConfigureAuditing = 176,
//    DssPrivilegesConfigureLanguageSettings = 177,
//    DssPrivilegesMonitorChangeJournal = 178,
//    DssPrivilegesUseBulkTranslationTool = 179,
//    DssPrivilegesUseBulkExportEditor = 180,
//    DssPrivilegesJobAdministration = 181,
//    DssPrivilegesUserConnectionAdministration = 182,
//    DssPrivilegesDatabaseConnectionAdministration = 183,
//    DssPrivilegesDuplicateProject = 184,
//    DssPrivilegesSubscribeOthers = 185,
//    DssPrivilegesSubscribeToEmailAlert = 186,
//    DssPrivilegesSendLinkInEmail = 187,
//    DssPrivilegesExportToPDF = 188,
//    DssPrivilegesExportToHTML = 189,
//    DssPrivilegesExportToExcel = 190,
//    DssPrivilegesExportToText = 191,
//    DssPrivilegesExportToFlash = 192,
//    DssPrivilegesViewNotes = 193,
//    DssPrivilegesAddNotes = 194,
//    DssPrivilegesEditNotes = 195,
//    DssPrivilegesUseDistributionServices = 196,
//    DssPrivilegesUseImmediateDelivery = 197,
//    DssPrivilegesEnableAdministrationFromWeb = 198,
//    DssPrivilegesUseTranslationEditor = 199,
//    DssPrivilegesWebUsePromptEditor = 200,
//    DssPrivilegesCreateHTMLContainer = 201,
//    DssPrivilegesImportTableFromMultipleSources = 202,
//    DssPrivilegesExecuteMultipleSourceReport = 203,
//    DssPrivilegesSavePersonalPromptAnswers = 204,
//    DssPrivilegesWebDefineDerivedElements = 205,
//    DssPrivilegesWebUseBasicThresholdEditor = 206,
//    DssPrivilegesWebDashboardDesign = 207,
//    DssPrivilegesWebSaveDerivedElements = 208,
//    DssPrivilegesWebUseAdvancedThresholdEditor = 209,
//    DssPrivilegesWebEditReportLinks = 210,
//    DssPrivilegesWebEditDrillingAndLinks = 210,
//    DssPrivilegesDefineDerivedElements = 211,
//    DssPrivilegesDashboardDesign = 212,
//    DssPrivilegesSaveDerivedElements = 213,
//    DssPrivilegesEditReportLinks = 214,
//    DssPrivilegesUseLinkEditor = 214,
//    DssPrivilegesWebEditNotes = 215,
//    DssPrivilegesDrillAndLink = 216,
//    DssPrivilegesWebDefineAdvancedReportOptions = 217,
//    DssPrivilegesWebCreateHTMLContainer = 218,
//    DssPrivilegesUseOLAPServices = 219,
//    DssPrivilegesUseDynamicSourcing = 220,
//    DssPrivilegesUseSQLTabInExportEditors = 221,
//    DssPrivilegesWebUseCustomGroupEditor = 222,
//    DssPrivilegesWebImportData = 223,
//    DssPrivilegesWebImportDatabase = 224,
//    DssPrivilegesDefineTransactionReport = 225,
//    DssPrivilegesWebConfigureTransaction = 226,
//    DssPrivilegesExecuteTransaction = 227,
//    DssPrivilegesWebVisualExploration = 228,
//    DssPrivilegesUseObjectManagerReadOnly = 229,
//    DssPrivilegesUseObjectManagerImportOnly = 230,
//    DssPrivilegesUseTranslationEditorBypass = 231,
//    DssPrivilegesWebUseObjectSharingEditor = 232,
//    DssPrivilegesWebCreateAnalysis = 233,
//    DssPrivilegesWebSaveAnalysis = 234,
//    DssPrivilegesPrintFromDevice = 235,
//    DssPrivilegesEmailScreenFromDevice = 236,
//    DssPrivilegesWebCreateDynamicAddressList = 237,
//    DssPrivilegesWebSubscribeDynamicAddressList = 238,
//    DssPrivilegesUseSendPreviewNow = 239,
//    DssPrivilegesMaximum = 240
//} EnumDSSPrivilegeType;

typedef enum EnumDSSAccessRightType
{
    DssAccessRightReserved = 0,
    DssAccessRightBrowse,
    DssAccessRightUseExecute,
    DssAccessRightRead,
    DssAccessRightWrite,
    DssAccessRightDelete,
    DssAccessRightControl,
    DssAccessRightUse,
    DssAccessRightExecute,
    DssAccessRightMaximum
} EnumDSSAccessRightType;

//typedef enum EnumDSSAccessRightFlags
//{
//    DssAccessRightReservedFlag = 0,
//    DssAccessRightBrowseFlag = 0x00000001,
//    DssAccessRightUseExecuteFlag = 0x00000002,
//    DssAccessRightReadFlag = 0x00000004,
//    DssAccessRightWriteFlag = 0x00000008,
//    DssAccessRightDeleteFlag = 0x00000010,
//    DssAccessRightControlFlag = 0x00000020,
//    DssAccessRightUseFlag = 0x00000040,
//    DssAccessRightExecuteFlag = 0x00000080,
//    DssAccessRightFullControlFlag = 0x000000ff,
//    DssAccessRightInheritable = 0x20000000,
//    DssAccessRightAudit = 0x40000000,
//    DssAccessRightAuditInheritable = 0x60000000
//} EnumDSSAccessRightFlags;

typedef enum EnumDSSAccessRightNamesFlags
{
    DssAccessRightNamesANDFlag = 0x00000001,
    DssAccessRightNamesORFlag = 0x00000002
} EnumDSSAccessRightNamesFlags;

typedef enum EnumDSSPromptType
{
    DssPromptReserved = 0,
    DssPromptBoolean,
    DssPromptLong,
    DssPromptString,
    DssPromptDouble,
    DssPromptDate,
    DssPromptObjects,
    DssPromptElements,
    DssPromptExpression,
    DssPromptExpressionDraft,
    DssPromptFilterDraft,
    DssPromptMetricDraft,
    DssPromptTemplateDraft,
    DssPromptDimty,
    DssPromptSort,
    DssPromptTemplateSubtotal,
    DssPromptBigDecimal,
    DssPromptReservedLastOne
} EnumDSSPromptType;

typedef enum EnumDSSPromptReuse
{
    DssReuseReserved = 0,
    DssReuseCancel =1,
    DssReuseDefault = 3,
    DssReusePrevious = 7,
    DssReuseBreak =	9,
    DssReuseDefaultOrBreak = 11,
    DssReusePreviousOrBreak = 15,
    DssReuseAutoCancel = 17,
    DssReuseAutoDefault =19,
    DssReuseAutoBreak = 25,
    DssReuseAutoDefaultOrBreak =27,
    DssReuseAutoClose = 49,
    DssReuseAutoCloseOrBreak = 57,
    DssReuseForceAutoClose = 113
} EnumDSSPromptReuse;

typedef enum EnumDSSPromptObjectType
{
    DssPromptTypeReserved = 0,
    DssPromptTypeObject,
    DssPromptTypeInstance,
    DssPromptTypeQuestion,
    DssPromptTypeBlankQuestion
} EnumDSSPromptObjectType;

typedef enum EnumDSSPromptAction
{
    DssPromptActionReserved = 0,
    DssPromptActionCancel,
    DssPromptActionContinue,
    DssPromptActionPause,
    DssPromptActionNotMatch
} EnumDSSPromptAction;

typedef enum EnumDSSPromptSystem
{
    DssPromptSystemCustom = -1,
    DssPromptSystemReserved = 0,
    DssPromptSystemLogin = 1,
    DssPromptSystemLDAPAttribute1 = 2,
    DssPromptSystemLDAPAttribute2 = 3,
    DssPromptSystemLDAPAttribute3 = 4,
    DssPromptSystemLDAPAttribute4 = 5,
    DssPromptSystemLDAPAttribute5 = 6,
    DssPromptSystemLDAPAttribute6 = 7,
    DssPromptSystemLDAPAttribute7 = 8,
    DssPromptSystemLDAPAttribute8 = 9,
    DssPromptSystemLDAPAttribute9 = 10,
    DssPromptSystemLDAPAttribute10 = 11,
    DssPromptSystemLDAPAttribute11 = 12,
    DssPromptSystemLDAPAttribute12 = 13,
    DssPromptSystemLDAPAttribute13 = 14,
    DssPromptSystemLDAPAttribute14 = 15,
    DssPromptSystemLDAPAttribute15 = 16,
    DssPromptSystemLDAPAttribute16 = 17,
    DssPromptSystemLDAPAttribute17 = 18,
    DssPromptSystemLDAPAttribute18 = 19,
    DssPromptSystemLDAPAttribute19 = 20,
    DssPromptSystemLDAPAttribute20 = 21,
    DssPromptSystemLDAPAttribute21 = 22,
    DssPromptSystemLDAPAttribute22 = 23,
    DssPromptSystemLDAPAttribute23 = 24,
    DssPromptSystemLDAPAttribute24 = 25,
    DssPromptSystemLDAPAttribute25 = 26,
    DssPromptSystemLDAPAttribute26 = 27,
    DssPromptSystemLDAPAttribute27 = 28,
    DssPromptSystemLDAPAttribute28 = 29,
    DssPromptSystemLDAPAttribute29 = 30,
    DssPromptSystemLDAPAttribute30 = 31,
    DssPromptSystemLDAPAttribute31 = 32,
    DssPromptSystemLDAPAttribute32 = 33,
    DssPromptSystemLDAPAttribute33 = 34,
    DssPromptSystemLDAPAttribute34 = 35,
    DssPromptSystemLDAPAttribute35 = 36,
    DssPromptSystemLDAPAttribute36 = 37,
    DssPromptSystemLDAPAttribute37 = 38,
    DssPromptSystemLDAPAttribute38 = 39,
    DssPromptSystemLDAPAttribute39 = 40,
    DssPromptSystemLDAPAttribute40 = 41,
    DssPromptSystemLDAPAttributeLastOne = 41,
    DssPromptSystemMobileID = 42,
    DssPromptSystemToken1 = 43,
    DssPromptSystemToken2 = 44,
    DssPromptSystemToken3 = 45,
    DssPromptSystemToken4 = 46,
    DssPromptSystemProfileID = 47,
    DssPromptSystemApplicationID = 48,
    DssPromptSystemPrivileges = 49,
    DssPromptSystemUID = 50,
    DssPromptSystemLastOne
} EnumDSSPromptSystem;

typedef enum EnumDSSPromptAnswerFlags
{
    DssPromptAnswerHandleUnAnswered =	0x00000001,
    DssPromptAnswerForStatistics =		0x00000002,
    DssPromptAnswerHandleFormat =		0x00000004
} EnumDSSPromptAnswerFlags;

typedef enum EnumDSSFunctionPropertySystem
{
    DssFunctionPropertyDistinct = 1,
    DssFunctionPropertyNull = 2,
    DssFunctionPropertyFactID = 3,
    DssFunctionPropertySystemCount = 3
} EnumDSSFunctionPropertySystem;

typedef enum EnumDSSTableType
{
    DssTableTypeReserved = 0,
    DssTableTypeLogical,
    DssTableTypePartitionMappingMD,
    DssTableTypePartitionMappingWH
} EnumDSSTableType;

typedef enum EnumDSSDBTableType
{
    DssDbTableTypeReserved = 0,
    DssDbTableTypeNormal,
    DssDbTableTypePMT
} EnumDSSDBTableType;

//typedef enum EnumDSSDrillType
//{
//    DssDrillReserved = 0,
//    DssDrillToParent,
//    DssDrillToChild,
//    DssDrillToUnit,
//    DssDrillToTemplate,
//    DssDrillToForm,
//    DssDrillToDetails,
//    DssDrillToMetric,
//    DssDrillFixTemplate,
//    DssDrillRemoveMetric,
//    DssDrillRemoveUnit,
//    DssDrillToGraphCoordinates
//} EnumDSSDrillType;

typedef enum EnumDSSDrillQual
{
    DssDrillQualReserved = 0,
    DssDrillQualOld,
    DssDrillQualNew,
    DssDrillQualBoth
} EnumDSSDrillQual;

typedef enum EnumDSSReportFilter
{
    DssReportFilterReserved = 0,
    DssReportFilterIgnore,
    DssReportFilterApply,
    DssReportFilterSmart
} EnumDSSReportFilter;

typedef enum EnumDSSKeepSort
{
    DssKeepSortReserved = 0,
    DssKeepSortDefinition,
    DssKeepSortOrder
} EnumDSSKeepSort;

typedef enum EnumDSSUserFilter
{
    DssUserFilterReserved = 0,
    DssUserFilterIgnore,
    DssUserFilterApply,
    DssUserFilterRelation,
    DssUserFilterEvaluate
} EnumDSSUserFilter;

//typedef enum EnumDSSDrillImportance
//{
//    DssDrillImportanceReserved = 0,
//    DssDrillImportanceHigh,
//    DssDrillImportanceMedium,
//    DssDrillImportanceLow,
//    DssDrillImportanceAll = 0x03,
//    DssDrillAddWithin = 0x04,
//    DssDrillForceWithin = 0x08,
//    DssDrillDetectPathOnly = 0x10,
//    DssDrillExcludeWithin = 0x20
//} EnumDSSDrillImportance;

typedef enum EnumDSSDrillPath
{
    DssDrillPathReserved = 0,
    DssDrillPathCustom,
    DssDrillPathDimension,
    DssDrillPathDetail,
    DssDrillPathUseMap,
    DssDrillPathForm
} EnumDSSDrillPath;

typedef enum EnumDSSDrillManipulation
{
    DssDrillManipulationReserved = 0,
    DssDrillManipulationApply = 1
} EnumDSSDrillManipulation;

//typedef enum EnumDSSFormType
//{
//    DssFormTypeReserved = 0,
//    DssFormTypeSystem,
//    DssFormTypeNormal,
//    DssFormTypeLastOne
//} EnumDSSFormType;

typedef enum EnumDSSGroupProperty
{
    DssGroupNode = -2,
    DssGroupName = 0,
    DssGroupStart = 1,
    DssGroupEnd = 2,
    DssGroupResidue = 3,
    DssGroupStatus = 4
} EnumDSSGroupProperty;

typedef enum EnumDSSBaseFormIndices
{
    DssBaseFormIndexTypeDependentDefault = -3,
    DssBaseFormIndexGroupNode = DssGroupNode,
    DssBaseFormIndexDisplayName = -1,
    DssBaseFormIndexGroupName = DssGroupName
} EnumDSSBaseFormIndices;

typedef enum EnumDSSSystemLinks
{
    DssSystemLinkReserved = 0,
    DssSystemLinkMDSecurity = 1,
    DssSystemLinkSecurityRole = 2,
    DssSystemLinkDBConnectionMap = 3,
    DssSystemLinkProfile = 4,
    DssSystemLinkPreferences = 5,
    DssSystemLinkSchedules = 6,
    DssSystemLinkReportSchedules = 7,
    DssSystemLinkDocumentSchedules = 8,
    DssSystemLinkGeneralSchedules = 9,
    DssSystemLinkReportScheduleRelation = 10,
    DssSystemLinkDocumentScheduleRelation = 11,
    DssSystemLinkWebPreferences1 = 12,
    DssSystemLinkWebPreferences2 = 13,
    DssSystemLinkWebPreferences3 = 14,
    DssSystemLinkUserInfo = 15,
    DssSystemLinkDataLocalePatterns = 16,
    DssSystemLinkReportAnnotations = 17,
    DssSystemLinkDocumentAnnotations = 18,
    DssSystemLinkLocalizedConnectionMap = 19,
    DssSystemLinkCacheInvalidationSchedule = 20,
    DssSystemLinkDefaultLocalizedConnection = 21,
    DssSystemLinkSearchEngineUserPreferences = 22,
    DssSystemLinkQuota = 23,
    DssSystemLinkProjectFlagging = 24,
    DssSystemLinkConfigurationFlagging = 25,
    DssSystemLinkSchedulesProject = 26,
    DssSystemLinkSchedulesReport = 27,
    DssSystemLinkSchedulesDocument = 28,
    DssSystemLinkDBRoleFolder = 29,
    DssSystemLinkReservedLastOne
} EnumDSSSystemLinks;

typedef enum EnumDSSDecomposable
{
    DssDecomposableReserved = 0,
    DssDecomposableDefault = 1,
    DssDecomposableFalse = 2,
    DssDecomposableTrue = 3,
    DssDecomposableOneLevel = 4
} EnumDSSDecomposable;

typedef struct DSSOBJECTINST
{
    DSS_ID ObjectID;
    DSS_ID ProjectID;
    EnumDSSObjectType Type;
    int State;
    DSS_ID VersionID;
} DSSOBJECTINST;

typedef enum EnumDSSFindObjectTypes
{
    DssTypeFlagAll =				0,
    DssTypeFlagFilter =				0x00000001,
    DssTypeFlagTemplate =			0x00000002,
    DssTypeFlagReportDefinition =	0x00000004,
    DssTypeFlagMetric =				0x00000008,
    DssTypeFlagReserved2 =			0x00000010,
    DssTypeFlagAgent =				0x00000020,
    DssTypeFlagAggMetric =			0x00000040,
    DssTypeFlagFolder =				0x00000080,
    DssTypeFlagOwner =				0x00000100,
    DssTypeFlagPrompt =				0x00000200,
    DssTypeFlagFunction =			0x00000400,
    DssTypeFlagAttribute =			0x00000800,
    DssTypeFlagFact =				0x00001000,
    DssTypeFlagDimension =			0x00002000,
    DssTypeFlagTable =				0x00004000,
    DssTypeFlagView =				0x00008000,
    DssTypeFlagFactGroup =			0x00010000,
    DssTypeFlagShortcut =			0x00020000,
    DssTypeFlagResolution =			0x00040000,
    DssTypeFlagDriver =				0x00080000,
    DssTypeFlagAttributeForm =		0x00100000,
    DssTypeFlagSchema =				0x00200000,
    DssTypeFlagFindObject =			0x00400000,
    DssTypeFlagCatalog =			0x00800000,
    DssTypeFlagCatalogDefn =		0x01000000,
    DssTypeFlagColumn =				0x02000000,
    DssTypeFlagPropertyGroup =		0x04000000,
    DssTypeFlagPropertySet =		0x08000000
} EnumDSSFindObjectTypes;

typedef enum DSSFolderSynch_Type
{
    DssSrcFolderRefreshed = 0x1,
    DssTrueSrcFolderRefreshed = 0x2,
    DssDstFolderRefreshed = 0x4,
    DssObjectRefreshed = 0x8
} DSSFolderSynch_Type;

typedef enum EnumDSSSessionFlags
{
    DssSessionCancelActiveJobsOnClose =	0x00000001,
    DssSessionDeleteReadMsgsOnClose =	0x00000010,
    DssSessionDeleteUnreadMsgsOnClose =	0x00000100,
    DssSessionChangePwdOnCreate =	0x00001000,
    DssSessionPreserveFlags =	0x00010000,
    DssSessionNewPassword =	0x00002000,
    DssSessionClientVersion =	0x00004000
} EnumDSSSessionFlags;

typedef enum EnumDSSSourceCommands
{
    DssSrcCmdSupport = 0,
    DssSrcCmdVersion = 1,
    DssSrcCmdMDVersion = 2,
    DssSrcCmdRefreshProperties = 3,
    DssSrcCmdNameLength = 4,
    DssSrcCmdPurge = 5,
    DssSrcCmdLocalVersion = 6,
    DssSrcCmdRemoteVersion = 7,
    DssSrcCmdDiagnostics = 8,
    DssSrcCmdEncryptionLevel = 9,
    DssSrcCmdConfigurationVersion = 10,
    DssSrcCmdMembership = 11,
    DssSrcCmdRefreshDBConnectionMap = 12,
    DssSrcCmdReadMDProperty = 13,
    DssSrcCmdWriteMDProperty = 14,
    DssSrcCmdDeleteMergeUser = 15,
    DssSrcCmdReadRDBMSPassword = 16,
    DssSrcCmdWriteRDBMSPassword = 17,
    DssSrcCmdPurgeAllLinkCaches = 18,
    DssSrcCmdPurgeLinkCache = 19,
    DssSrcCmdCollectGarbage = 20,
    DssSrcCmdCollectGarbageStatisticsOnly = 21,
    DssSrcCmdObjectCacheSnapshot = 22,
    DssSrcCmdRefreshCachedLocaleMap = 23,
    DssSrcCmdMetadataSchemaVersion = 24,
    DssSrcCmdRefreshMetadataLocales = 25,
    DssSrcCmdReSequenceCommentAndStringPropertyRows = 26,
    DssSrcCmdRefreshCachedPropertySetsAndPropertyGroups = 27,
    DssSrcCmdMigrateObjectDefinitions = 28,
    DssSrcCmdUpdateObjectDefinitions = 29,
    DssSrcCmdUpdateLinkItemCoordinate = 30,
    DssSrcCmdChangeObjectIDs = 31,
    DssSrcCmdQueryObjectExistence = 32,
    DssSrcCmdObjectDefinitionMigrated = 33,
    DssSrcCmdChangeObjectVersionIDs = 34,
    DssSrcCmdEmptyLegacyShortcutNames = 35,
    DssSrcCmdCurrentMetadataSchemaVersion = 36,
    DssSrcCmdConvertObjectsDefinitionToLeanFormat = 37,
    DssSrcCmdReadProfileCreationMode = 38,
    DssSrcCmdReservedLastOne
} EnumDSSSourceCommands;

typedef enum EnumDSSSourceProgressMethod
{
    DssSourceProgressReserved = 0,
    DssSourceProgressExecuteSearch,
    DssSourceProgressSaveObjects,
    DssSourceProgressDeleteObjects,
    DssSourceProgressSaveObjectsObject,
    DssSourceProgressDeleteObject
} EnumDSSSourceProgressMethod;

typedef enum EnumDSSEncryptionLevel
{
    DssEncryptionUnknown = -2,
    DssEncryptionNone = -1,
    DssEncryptionLevel0 = 0,
    DssEncryptionLevelAES = 1
} EnumDSSEncryptionLevel;

typedef enum EnumDSSDatamartColumnType
{
    DssDatamartColumnReserved = 0,
    DssDatamartColumnConcrete = 1,
    DssDatamartColumnAttribute = 2,
    DssDatamartColumnDimension = 3,
    DssDatamartColumnCustomGroup = 4,
    DssDatamartColumnCustomGroupP = 5,
    DssDatamartColumnConsolidation = 6,
    DssDatamartColumnConsolidationP = 7
} EnumDSSDatamartColumnType;

typedef enum EnumDSSDatamartValidation
{
    DssDatamartReserved = 0,
    DssDatamartValid = 1
} EnumDSSDatamartValidation;

typedef enum EnumDSSObjectLockFlags
{
    DssObjectLockDefault =	0,
    DssObjectLockConstituents = 	0x2,
    DssObjectLockPermanent =	0x4,
    DssObjectLockIntent =	0x8
} EnumDSSObjectLockFlags;

typedef enum EnumDSSLockQuery
{
    DssLockQueryReserved = 0,
    DssLockQueryState = 1,
    DssLockQueryGovernor = 2,
    DssLockQueryTime = 3,
    DssLockQueryOwner = 4,
    DssLockQueryComment = 5,
    DssLockQueryMachine = 6,
    DssLockQuerySession = 7,
    DssLockQueryAll = 8
} EnumDSSLockQuery;

typedef enum EnumDSSTextExFlags
{
    DssTextExDefault = 0x000,
    DssTextExExpandNothing = 0x001,
    DssTextExExpandEmbedded = 0x002,
    DssTextExExpandAll = 0x003,
    DssTextExExpandBits = 0x003,
    DssTextExMetricDescription = 0x004,
    DssTextExMetricConditionality = 0x008,
    DssTextExMetricFormula = 0x010,
    DssTextExMetricDimensionality = 0x020,
    DssTextExMetricTransformation = 0x040,
    DssTextExMetricBits = 0x07C,
    DssTextExSubtitles = 0x80000000,
    DssTextExTitleBits = 0x80000000
} EnumDSSTextExFlags;

typedef enum EnumDSSDetailsFlags
{
    DssDetailsDefault = 0x0000,
    DssDetailsExprExpandNothing = 0x0001,
    DssDetailsExprExpandEmbedded = 0x0002,
    DssDetailsExprExpandAll = 0x0003,
    DssDetailsExprExpandBits = 0x0003,
    DssDetailsMetricDescription = 0x0004,
    DssDetailsMetricConditionality = 0x0008,
    DssDetailsMetricFormula = 0x0010,
    DssDetailsMetricDimensionality = 0x0020,
    DssDetailsMetricTransformation = 0x0040,
    DssDetailsMetricBits = 0x007C,
    DssDetailsTemplateMetricLimit = 0x0080,
    DssDetailsPromptQuestions = 0x0100,
    DssDetailsPromptAnswers = 0x0200,
    DssDetailsPromptBits = 0x0300,
    DssDetailsReportDescription = 0x0400,
    DssDetailsFilterExpression = 0x0800,
    DssDetailsTemplateUnitsDescriptions = 0x1000,
    DssDetailsTemplateCombinedLimits = 0x2000,
    DssDetailsViewFilterExpression = 0x4000,
    DssDetailsViewTemplateMetricLimit = 0x8000,
    DssDetailsSecurityFilters = 0x10000,
    DssDetailsDrillUserFilter = 0x40000,
    DssTextExForDEText = 0x80000,
    DssDetailsTemplateSingleLimits = 0x100000,
    DssDetailsReportDetails = 0xF0000000,
    DssDetailsSubtitles = 0x80000000,
    DssDetailsTitleBits = 0x80000000
} EnumDSSDetailsFlags;

typedef enum EnumDSSReportXmlFlags
{
    DssReportXmlDtlsExprNothing = 0x00000001,
    DssReportXmlDtlsExprEmbedded = 0x00000002,
    DssReportXmlDtlsExprAll = 0x00000003,
    DssReportXmlDtlsExprBits = 0x00000003,
    DssReportXmlSQL = 0x00000004,
    DssReportXmlDtlsTemplateLimit = 0x00000008,
    DssReportXmlDtlsTemplateUnits = 0x00000010,
    DssReportXmlDtlsFilterExpr = 0x00000040,
    DssReportXmlViewReport = 0x00800000,
    DssReportXmlDtlsAll = 0x00000058,
    DssReportXmlNoDerivedPromptXML = 0x00000020,
    DssReportXmlWorkingSet = 0x00000080,
    DssReportXmlDisplayProperties = 0x00000100,
    DssReportXmlNoResolution = 0x00000200,
    DssReportXmlGraph = 0x00000400,
    DssReportXmlFilter = 0x00000800,
    DssReportXmlTemplates = 0x00001000,
    DssReportXmlBandingAutoApply = 0x00001000,
    DssReportXmlRawData = 0x00002000,
    DssReportXmlDrillHigh = 0x00004000,
    DssReportXmlDrillMedium = 0x00008000,
    DssReportXmlDrillLow = 0x00010000,
    DssReportXmlDrillAll = 0x0001C000,
    DssReportXmlGrid = 0x00020000,
    DssReportXmlBandingStyle = 0x00040000,
    DssReportXmlResultFolder = 0x00080000,
    DssReportXmlRelatedReports = 0x00100000,
    DssReportXmlNoPageHeader = 0x00200000,
    DssReportXmlMinimal = 0x00400000,
    DssReportXmlPutToInboxRead = 0x01000000,
    DssReportXmlStatusOnly = 0x02000000,
    DssReportXmlNoNumberFormatting = 0x04000000,
    DssReportXmlResultInboxMessage = 0x08000000,
    DssReportXmlDefaultFormatProperties = 0x10000000,
    DssReportXmlDatamartTableName = 0x20000000,
    DssReportXmlPageTreeStyle = 0x40000000,
    DssReportXmlPreserve = 0x80000000,
    DssReportXmlXMLCacheNotRelated = 0x8b000000,
    DssReportXmlXMLCacheLevel0 = 0,
    DssReportXmlXMLCacheLevel1 = 0x00004800,
    DssReportXmlXMLCacheLevel2 = 0x00004804,
    DssReportXmlXMLCacheLevel3 = 0x0000485C,
    DssReportXmlXMLCacheMustCheck =  0x74A7B7A3
} EnumDSSReportXmlFlags;

typedef enum EnumDSSReportXml2Flags
{
    DssReportXml2Reserved = 0x00000000,
    DssReportXml2Widths = 0x00000001,
    DssReportXml2ShortCSSNames = 0x00000002,
    DssReportXml2Widths2 = 0x00000004,
    DssReportXml2RawDates = 0x00000008,
    DssReportXml2FormDetails = 0x00000010,
    DssReportXml2NoElementIDs = 0x00000020,
    DssReportXml2NoFormatXML = 0x00000040,
    DssReportXml2Personalize = 0x00000080,
    DssReportXml2NoGridNode = 0x00000100,
    DssReportXml2MinimalPropertiesXML = 0x00000200,
    DssReportXml2ReportSourceXML = 0x00000400,
    DssReportXml2LimitsXML = 0x00000800,
    DssReportXml2PageCurrentElementsOnly = 0x00001000,
    DssReportXml2DrillPathsHighImportance = 0x00002000,
    DssReportXml2DrillPathsMediumImportance = 0x00004000,
    DssReportXml2DrillPathsLowImportance = 0x00006000,
    DssReportXml2DrillPathsTemplate = 0x00008000,
    DssReportXml2DrillPathsAll = 0x0000E000,
    DssReportXml2RWDDataElement = 0x00010000,
    DssReportXml2EnhancedTimeXML = 0x00020000,
    DssReportXml2NewSubtotalsXML = 0x00040000,
    DssReportXml2EnhancedGraphXML = 0x00080000,
    DssReportXml2XDAHierarchies = 0x00100000,
    DssReportXml2PageTreeStyle2 = 0x00200000,
    DssReportXml2ImageMap = 0x00400000,
    DssReportXml2DrillPathsWithin = 0x00800000,
    DssReportXml2DrillPathsDesired = 0x01000000,
    DssReportXml2RWDGraphsSpecial = 0x02000000,
    DssReportXml2RowColumnLabels = 0x04000000,
    DssReportXml2InstanceProperties = 0x08000000,
    DssReportXml2WidgetStrings = 0x10000000,
    DssReportXml2ReportDetails = 0x20000000,
    DssReportXml2AllPrompts = 0x40000000,
    DssReportXml2NewCssAndColumnHeader = 0x80000000
} EnumDSSReportXml2Flags;

typedef enum EnumDSSReportXml3Flags
{
    DssReportXml3Reserved = 0x00000000,
    DssReportXml3TerseElementIDs = 0x00000001,
    DssReportXml3ObjectComments = 0x00000002,
    DssReportXml3ReportHeaderFooter = 0x00000004,
    DssReportXml3MergeMultiFormHeaderNodes = 0x00000008,
    DssReportXml3PartialCSSRetrieval = 0x00000010,
    DssReportXml3FormatShortXML = 0x00000020,
    DssReportXml3NoThresholdsXML = 0x00000040,
    DssReportXml3OptimizedElementTree = 0x00000080,
    DssReportXml3PageByAxisOnly = 0x00000100,
    DssReportXml3TransactionRawData = 0x00000200
} EnumDSSReportXml3Flags;

typedef enum EnumDSSBrowseReportFlags
{
    DssBrowseReportReserved = 0x00000000,
    DssBrowseReportBindingTable = 0x00000001,
    DssBrowseReportSQLPreview = 0x00000002,
    DssBrowseReportDATAPreview = 0x00000004
} EnumDSSBrowseReportFlags;

typedef enum EnumDSSBindingTableObjectTypes
{
    DssBindingTableObjectAll = 0x00000000,
    DssBindingTableObjectTableAndDBTable = 0x00000001,
    DssBindingTableObjectReport = 0x00000002,
    DssBindingTableObjectTemplate = 0x00000004,
    DssBindingTableObjectColumn = 0x00000008
} EnumDSSBindingTableObjectTypes;

typedef enum EnumDSSDrillUnitSource
{
    DssDrillUnitReserved = 0,
    DssDrillUnitInstance = 1,
    DssDrillUnitLocal = 2,
    DssDrillUnitNone = 3,
    DssDrillUnitRow = 4,
    DssDrillUnitColumn = 5,
    DssDrillUnitPageBy = 6
} EnumDSSDrillUnitSource;

typedef enum EnumDSSPropertyXmlFlags
{
    DssPropertyXmlReserved = 0,
    DssPropertyXmlPropID = 0x00000001,
    DssPropertyXmlPropName = 0x00000002,
    DssPropertyXmlPropType = 0x00000004,
    DssPropertyXmlPropValue = 0x00000008,
    DssPropertyXmlPropDefault = 0x00000010,
    DssPropertyXmlPropSetID = 0x00000020,
    DssPropertyXmlPropUseDefault = 0x00000040,
    DssPropertyXmlPropTargetFrom = 0x00000080,
    DssPropertyXmlPropIndex = 0x00000100,
    DssPropertyXmlPropDescription = 0x00000200,
    DssPropertyXmlPropAll = 0x00000FFF,
    DssPropertyXmlSetID = 0x00001000,
    DssPropertyXmlSetName = 0x00002000,
    DssPropertyXmlSetChildCount = 0x00004000,
    DssPropertyXmlSetPartialChildCount = 0x00008000,
    DssPropertyXmlSetAll = 0x000FF000,
    DssPropertyXmlAll = 0x000FFFFF,
    DssPropertyXmlNonDefaultOnly = 0x80000000,
    DssPropertyXmlNonDefaultOnly2 = 0x40000000,
    DssPropertyXmlIgnoreMissing = 0x20000000,
    DssPropertyXmlIgnoreAssignedDefault = 0x10000000,
    DssPropertyXmlNameOneTime = 0x00100000
} EnumDSSPropertyXmlFlags;

typedef enum EnumDSSDBCKeyType
{
    DssDbcKeyReserved = 0,
    DssDbcKeyPrimary = 1,
    DssDbcKeyForeign = 2
} EnumDSSDBCKeyType;

typedef enum EnumDSSProfileFolders
{
    DssProfileFolderRoot = 0,
    DssProfileFolderReports = 1,
    DssProfileFolderAnswers = 2,
    DssProfileFolderFavorites = 3,
    DssProfileFolderOther = 4
} EnumDSSProfileFolders;

typedef enum EnumDSSXMLContextLongProperties
{
    DssXMLContextLongPropertySearchFlags = 0,
    DssXMLContextLongPropertyIs4TierSearch = 1,
    DssXMLContextLongPropertyResultFlags2 = 2,
    DssXMLContextLongPropertyIncludeColumnWidths = 3,
    DssXMLContextLongPropertyDefaultHeaderWidth = 4,
    DssXMLContextLongPropertyDefaultGridWidth = 5,
    DssXMLContextLongPropertyExtraWidth = 6,
    DssXMLContextLongPropertyGraphImageType = 7,
    DssXMLContextLongPropertyLoadExplicitOnly = 8,
    DssXMLContextLongPropertyPromptXMLFlags = 9,
    DssXMLContextLongPropertySaveDefnWithInfo = 10,
    DssXMLContextLongPropertyMaxNodesExceeded = 11,
    DssXMLContextLongPropertySeriesByColumns = 12,
    DssXMLContextLongPropertyDataPointsRows = 13,
    DssXMLContextLongPropertyDataPointsCols = 14,
    DssXMLContextLongPropertyResultPartial = 15,
    DssXMLContextLongPropertyDocumentXMLFlags = 16,
    DssXMLContextLongPropertyDontDereferenceObjectRef = 17,
    DssXMLContextLongPropertyCloneRefMap = 18,
    DssXMLContextLongPropertyHLPromptBCMode = 19,
    DssXMLContextLongPropertyResultFlags3 = 20,
    DssXMLContextLongPropertyObjectsXML = 21,
    DssXMLContextLongPropertyFormatPropertiesXML = 22,
    DssXMLContextLongPropertyPropertyXMLFlags = 23,
    DssXMLContextLongPropertyQuickSearchFlags = 24,
    DssXMLContextLongPropertyElementIDFlags = 25,
    DssXMLContextLongPropertyFolderContentsAncestorsInfo = 26,
    DssXMLContextLongPropertyCount
} EnumDSSXMLContextLongProperties;

typedef enum EnumDSSXMLContextIDispatchProperties
{
    DssXMLContextIDispatchPropertyExtendableParameterNode = 0,
    DssXMLContextIDispatchPropertyRWDocumentPropertiesNode = 1,
    DssXMLContextIDispatchPropertyRWTemplateGridPropertiesNode = 2,
    DssXMLContextIDispatchPropertyRWTemplateGraphPropertiesNode = 3,
    DssXMLContextIDispatchPropertyRWPropertiesNode = 4,
    DssXMLContextIDispatchPropertyUnitObjectsCollection = 5,
    DssXMLContextIDispatchPropertySourceTable = 6,
    DssXMLContextIDispatchPropertyRWTemplateNode = 7,
    DssXMLContextIDispatchPropertyRWDocumentInstance = 8,
    DssXMLContextIDispatchPropertyWorkingSet = 9,
    DssXMLContextIDispatchPropertyRWDDerivedMetrics = 10,
    DssXMLContextIDispatchPropertyCount
} EnumDSSXMLContextIDispatchProperties;

typedef enum EnumDSSXMLContextBSTRProperties
{
    DssXMLContextBSTRPropertyFormatControlList = 0,
    DssXMLContextBSTRPropertyResultUpdateTime = 1,
    DssXMLContextBSTRPropertyClientTimeZone = 2,
    DssXMLContextBSTRPropertyCount
} EnumDSSXMLContextBSTRProperties;

typedef enum EnumDSSXMLDocumentLongProperties
{
    DssXMLDocumentLongPropertyIsInitialized = 0,
    DssXMLDocumentLongPropertyBufferLimit = 1,
    DssXMLDocumentLongPropertyCount
} EnumDSSXMLDocumentLongProperties;

typedef enum EnumDSSManipulationOption
{
    DssManipulationDefault = 0,
    DssManipulationFreshInstance = 1,
    DssManipulationDonotApply = 2,
    DssManipulationDonotApplySQL = 4,
    DssManipulationDonotCheckPrivilige = 8
} EnumDSSManipulationOption;

typedef enum EnumDSSManipulationTarget
{
    DssManipulationTargetReserved = 0,
    DssManipulationTargetBase = 1,
    DssManipulationTargetView = 2
} EnumDSSManipulationTarget;

typedef enum EnumDSSManipulationType
{
    DssManipulationReserved = -1,
    DssManipulationNone = 0,
    DssManipulationFormat = 1,
    DssManipulationPivot = 2,
    DssManipulationSubset = 3,
    DssManipulationSQL = 4,
    DssManipulationResolve = 5
} EnumDSSManipulationType;

typedef enum EnumDSSReportManipulationMethod
{
    DssReportManipulationAddUnit = 1,
    DssReportManipulationAddMetric = 2,
    DssReportManipulationRemoveUnit = 3,
    DssReportManipulationRemoveBaseUnit = 4,
    DssReportManipulationSetFilter = 5,
    DssReportManipulationSetBaseFilter = 6,
    DssReportManipulationPivotUnit = 7,
    DssReportManipulationClearTemplate = 8,
    DssReportManipulationPageBy = 9,
    DssReportManipulationFormatAxis = 10,
    DssReportManipulationFormatUnit = 11,
    DssReportManipulationAxisSubtotals = 12,
    DssReportManipulationAxisSubtotalsObject = 13,
    DssReportManipulationUnitSubtotals = 14,
    DssReportManipulationUnitSubtotalsObject = 15,
    DssReportManipulationRemoveSubtotals = 16,
    DssReportManipulationAddDerivedMetric = 17,
    DssReportManipulationUpdateDerivedMetric = 18,
    DssReportManipulationConvertIntoDerivedMetric = 19,
    DssReportManipulationClearUnit = 20,
    DssReportManipulationLockUnit = 21,
    DssReportManipulationUnlockUnit = 22,
    DssReportManipulationAddPercentToTotalMetric = 23,
    DssReportManipulationAddRankMetric = 24,
    DssReportManipulationAddTransformationMetric = 25,
    DssReportManipulationSort = 26,
    DssReportManipulationRemoveSort = 27,
    DssReportManipulationChangeUnitDisplayName = 28,
    DssReportManipulationSetGraphProperties = 29,
    DssReportManipulationShowSubtotals = 30,
    DssReportManipulationHideSubtotals = 31,
    DssReportManipulationShowBanding = 32,
    DssReportManipulationHideBanding = 33,
    DssReportManipulationShowThresholds = 34,
    DssReportManipulationHideThresholds = 35,
    DssReportManipulationApplyStyle = 36,
    DssReportManipulationApplyStyleObject = 37,
    DssReportManipulationPutProperties = 38,
    DssReportManipulationEditBaseFilter = 39,
    DssReportManipulationEditFilter = 40,
    DssReportManipulationSetWidthColumnScenario = 41,
    DssReportManipulationSetWidthRowScenario = 42,
    DssReportManipulationSetWidthHeader = 43,
    DssReportManipulationSetWidthMetric = 44,
    DssReportManipulationSetWidthClear = 45,
    DssReportManipulationAddForm = 46,
    DssReportManipulationAddBaseForm = 47,
    DssReportManipulationRemoveForm = 48,
    DssReportManipulationRemoveBaseForm = 49,
    DssReportManipulationResetForms = 50,
    DssReportManipulationAddUnitToBase = 53,
    DssReportManipulationFormatTemplate = 54,
    DssReportManipulationSetDatamart = 55,
    DssReportManipulationSetTemplate = 56,
    DssReportManipulationSetBaseTemplate = 57,
    DssReportManipulationCreateThreshold = 58,
    DssReportManipulationUpdateThreshold = 59,
    DssReportManipulationRemoveThreshold = 60,
    DssReportManipulationReorderThreshold = 61,
    DssReportManipulationSubtotalPosition = 62,
    DssReportManipulationSetCubeReport = 63,
    DssReportManipulationAddLimit = 64,
    DssReportManipulationRemoveLimit = 65,
    DssReportManipulationAddLimitToBase = 66,
    DssReportManipulationRemoveLimitFromBase = 67,
    DssReportManipulationSetSourceTable = 68,
    DssReportManipulationAddDerivedElement = 69,
    DssReportManipulationUpdateDerivedElement = 70,
    DssReportManipulationFormatDerivedElement = 71,
    DssReportManipulationClearDerivedElementReference = 72,
    DssReportManipulationRemoveDerivedElement = 73,
    DssReportManipulationRemoveSubtotal = 74,
    DssReportManipulationSetDerivedElementProperties = 75,
    DssReportManipulationSetDerivedElementHierarchyOption = 76,
    DssReportManipulationAddListQuickGroup = 77,
    DssReportManipulationAddCalculationQuickGroup = 78,
    DssReportManipulationSetAbbreviation = 79,
    DssReportManipulationMakeTabular = 80,
    DssReportManipulationSetUnitData = 81,
    DssReportManipulationApplyDeliveryThresholds = 82,
    DssReportManipulationAddDerivedElementReference = 83,
    DssReportManipulationSelectDerivedElementReference = 84,
    DssReportManipulationUnSelectDerivedElementReference = 85,
    DssReportManipulationSetReportObjectProperties = 86,
    DssReportManipulationSetEvaluationOrder = 87,
    DssReportManipulationSetWidgetString = 88,
    DssReportManipulationRemoveWidgetString = 89,
    DssReportManipulationSetUnitHyperlinksXML = 90,
    DssReportManipulationSetTransactionInfoOnUnit = 91,
    DssReportManipulationRemoveTransactionInfoOnUnit = 92,
    DssReportManipulationSetUnitAliasParameter = 93,
    DssReportManipulationAddShortcutMetric = 94,
    DssReportManipulationAddTable = 95,
    DssReportManipulationEditTable = 96,
    DssReportManipulationAddSelectColumn = 97,
    DssReportManipulationEditSelectColumn = 98,
    DssReportManipulationAddJoin = 99,
    DssReportManipulationEditJoin = 100,
    DssReportManipulationGenerateJoins = 101,
    DssReportManipulationRemoveTable = 102,
    DssReportManipulationRemoveSelectColumn = 103,
    DssReportManipulationRemoveJoin = 104,
    DssReportManipulationCalculateGroupBy = 105,
    DssReportManipulationEditCondition = 106,
    DssReportManipulationSetDBRole = 107,
    DssReportManipulationConvertQBToFFS = 108,
    DssReportManipulationEditFFSSQL = 109,
    DssReportManipulationConvertToCube = 110,
    DssReportManipulationCancelConvertQBToFFS = 111,
    DssReportManipulationUnSelectAllDerivedElementReference = 112,
    DssReportManipulationAutoClosePrompts = 113,
    DSSReportManipulationUpdateWHTable = 114,
    DssReportManipulationLastOne
} EnumDSSReportManipulationMethod;

typedef enum EnumDSSMetricFormulaType
{
    DssMetricFormulaReserved = 0,
    DssMetricFormulaIdentity = 1,
    DssMetricFormulaDifference = 2,
    DssMetricFormulaVarianceRatio = 3
} EnumDSSMetricFormulaType;

typedef enum EnumDSSFormat
{
    DssFormatReserved = 0,
    DssFormatHeader,
    DssFormatGrid,
    DssFormatSubtotalHeader,
    DssFormatSubtotalGrid,
    DssFormatTemplateReport,
    DssFormatTemplateHeader,
    DssFormatTemplateHeaderValues,
    DssFormatTemplateSubtotalHeaders,
    DssFormatTemplateSubtotalValues,
    DssFormatTemplateData,
    DssFormatChildHeader,
    DssFormatChildGrid,
    DssFormatLastOne
} EnumDSSFormat;

typedef enum EnumDSSSymbol
{
    DssSymbolReserved = 0,
    DssSymbolBlackCircle =1,
    DssSymbolBlackSquare =2 ,
    DssSymbolLozenge =3,
    DssSymbolBlackDiamond =4,
    DssSymbolInkBlot =5,
    DssSymbolWheelofDharma =6,
    DssSymbolBlackFlorette =7,
    DssSymbolPlaceInterestSign =8,
    DssSymbolHeavyCheckMark =9,
    DssSymbolHeavyBallotX = 10,
    DssSymbolPencil = 11,
    DssSymbolBell = 12,
    DssSymbolTelephoneLocationSign = 13,
    DssSymbolEvenlope = 14,
    DssSymbolCircle = 15,
    DssSymbolNegativeCircledNumberZero = 16,
    DssSymbolNegativeCircledNumberOne = 17,
    DssSymbolNegativeCircledNumberTwo = 18,
    DssSymbolNegativeCircledNumberThree = 19,
    DssSymbolNegativeCircledNumberFour = 20,
    DssSymbolNegativeCircledNumberFive = 21,
    DssSymbolNegativeCircledNumberSix = 22,
    DssSymbolNegativeCircledNumberSeven = 23,
    DssSymbolNegativeCircledNumberEight = 24,
    DssSymbolNegativeCircledNumberNine = 25,
    DssSymbolNegativeCircledNumberTen = 26,
    DssSymbolBlackStar = 27,
    DssSymbolLeftwardsBlackArrow = 28,
    DssSymbolRightwardsBlackArrow = 29,
    DssSymbolUpwardsBlackArrow = 30,
    DssSymbolDownwardsBlackArrow = 31,
    DssSymbolNorthWestBlackArrow = 32,
    DssSymbolNorthEastBlackArrow = 33,
    DssSymbolSouthWestBlackArrow = 34,
    DssSymbolSouthEastBlackArrow = 35,
    DssSymbolLastOne = 35
} EnumDSSSymbol;

typedef enum EnumDSSValidationLevel
{
    DssValidationReserved = 0,
    DssValidationGood = 1,
    DssValidationRecoverable = 2,
    DssValidationFatal = 3,
    DssValidationBadApplyOrder = 4,
    DssValidationCircularAction = 5
} EnumDSSValidationLevel;

typedef enum EnumDSSIssueType
{
    DssIssueUnknown = 0,
    DssIssueMissingUnit = 1,
    DssIssueMissingForm = 2,
    DssIssueMissingMetricLeaf = 3,
    DssIssueMissingFormLeaf = 4,
    DssIssueBadFunction = 5,
    DssIssueBadConditionality = 6,
    DssIssueBadLevel = 7,
    DssIssueBadDynamicSort = 8,
    DssIssueBadAttribute = 9,
    DssIssueBadMetricInput = 10,
    DssIssueRecursiveReference = 11
} EnumDSSIssueType;

typedef enum EnumDSSIssueLocation
{
    DssIssueLocationUnknown = 0,
    DssIssueLocationUnit = 1,
    DssIssueLocationMetric = 2,
    DssIssueLocationLimit = 3,
    DssIssueLocationFilter = 4,
    DssIssueLocationThreshold = 5,
    DssIssueLocationReplacement = 6
} EnumDSSIssueLocation;

typedef enum EnumDSSAEAggregation
{
    DssAggregationDefault = 0,
    DssAggregationDisable = 1,
    DssAggregationEnable = 2
} EnumDSSAEAggregation;

typedef enum EnumDSSDEAggregation
{
    DssDEAggregationReserved = 0,
    DssDEAggregationSelf = 1,
    DssDEAggregationChildren = 2,
    DssDEAggregationIgnore = 3
} EnumDSSDEAggregation;

typedef enum EnumDSSPropertyLocation
{
    DssLocationReportTarget = 1,
    DssLocationReport = 2,
    DssLocationTemplateTarget = 3,
    DssLocationTemplate = 4,
    DssLocationObject = 5,
    DssLocationProject = 6,
    DssLocationDBRole = 7,
    DssLocationDBMS = 8,
    DssLocationDefault = 9
} EnumDSSPropertyLocation;

typedef enum EnumDSSSecurityModel
{
    DssSecurityModelReserved = 0,
    DssSecurityModel71 = 1,
    DssSecurityModel72 = 2,
    DssSecurityModelReservedLastOne
} EnumDSSSecurityModel;

typedef enum EnumDSSEmbeddedModel
{
    DssEmbeddedModelReserved = 0,
    DssEmbeddedModelInherit = 1,
    DssEmbeddedModelEmbed = 2,
    DssEmbeddedModelLink = 3
} EnumDSSEmbeddedModel;

//typedef enum EnumDSSDataType
//{
//    DssDataTypeUnknown = -1,
//    DssDataTypeReserved = 0,
//    DssDataTypeInteger = 1,
//    DssDataTypeUnsigned = 2,
//    DssDataTypeNumeric = 3,
//    DssDataTypeDecimal = 4,
//    DssDataTypeReal = 5,
//    DssDataTypeDouble = 6,
//    DssDataTypeFloat = 7,
//    DssDataTypeChar = 8,
//    DssDataTypeVarChar = 9,
//    DssDataTypeLongVarChar = 10,
//    DssDataTypeBinary = 11,
//    DssDataTypeVarBin = 12,
//    DssDataTypeLongVarBin = 13,
//    DssDataTypeDate = 14,
//    DssDataTypeTime = 15,
//    DssDataTypeTimeStamp = 16,
//    DssDataTypeShort =21,
//    DssDataTypeLong =22,
//    DssDataTypeMBChar =23,
//    DssDataTypeBool = 24,
//    DssDataTypePattern = 25,
//    DssDataTypeBigDecimal = 30,
//    DssDataTypeCellFormatData = 31,
//    DssDataTypeMissing = 32,
//    DssDataTypeNChar = 17,
//    DssDataTypeNVarChar = 18,
//    DssDataTypeNPattern = 29,
//    DssDataTypeUTF8Char = 33,
//    DssDataTypeInt64 = 34,
//    DssDataTypeLastOne
//} EnumDSSDataType;

typedef enum EnumDSSCustomGroupDisplay
{
    DssCustomGroupDefault = 0,
    DssCustomGroupFlatten = 1,
    DssCustomGroupHierarchical = 2
} EnumDSSCustomGroupDisplay;

typedef enum EnumDSSOrderParentFirst
{
    DssOrderParentFirstReserved = 0,
    DssOrderParentFirstDefault = 1,
    DssOrderParentFirstTrue = 2,
    DssOrderParentFirstFalse = 3
} EnumDSSOrderParentFirst;

typedef enum EnumDssValidationFlags
{
    DssValidationFlagReserved = 0x00000000,
    DssValidationBindingTableOnly = 0x00000001
} EnumDssValidationFlags;

typedef enum EnumDSSWarehouseCredentialSource
{
    DssWarehouseCredentialFromConnectionMap = 0,
    DssWarehouseCredentialPersistedOnUser = 1
} EnumDSSWarehouseCredentialSource;

typedef enum EnumDSSResolutionAnswerFlags
{
    DssShortXMLClosedPrompts = 0x00000000,
    DssShortXMLSavedAnswers = 0x00000001,
    DssResolutionAnswerKeepContainerClean = 0x00008000,
    DssShortXMLAnswersOrSubstitutions = 2,
    DssResolutionAnswerKeepAutoClose = 0x00010000,
    DssResolutionAnswerEnhancedTimeXML = 0x00020000
} EnumDSSResolutionAnswerFlags;

typedef enum EnumDSSResolutionCombineFlags
{
    DssResolutionCombineReserved = 0,
    DssResolutionCombineDefault = 1,
    DssResolutionCombineMergeReuse = 2,
    DssResolutionCombineMergeReuseReportStyle = 4
} EnumDSSResolutionCombineFlags;

typedef enum EnumDSSReportCacheDisableOptions
{
    DssReportCacheDisableNone =				0x00000000,
    DssReportCacheDisableAll =				0x00000001,
    DssReportCacheDisablePromptReport =			0x00000002,
    DssReportCacheDisableNonPromptReport =			0x00000004,
    DssReportCacheDisableXMLCache =				0x00000008,
    DssReportCacheDisableExportCache =			0x00000010
} EnumDSSReportCacheDisableOptions;

typedef enum EnumDSSReportObjects
{
    DssReportObjectReserved = 0,
    DssReportObjectMainDefinition = 1,
    DssReportObjectViewDefinition = 2,
    DssReportObjectBaseDefinition = 3,
    DssReportObjectMainTemplate = 4,
    DssReportObjectViewTemplate = 5,
    DssReportObjectBaseTemplate = 6,
    DssReportObjectMainFilter = 7,
    DssReportObjectViewFilter = 8,
    DssReportObjectBaseFilter = 9,
    DssReportObjectDatamart = 10
} EnumDSSReportObjects;

typedef enum EnumDSSExecutionControl
{
    DssExecutionReserved = -1,
    DssExecutionNone = 0,
    DssExecutionActions = 1,
    DssExecutionFormat = 2,
    DssExecutionViewTemplates = 3,
    DssExecutionView = 4,
    DssExecutionXTab = 6,
    DssExecutionSubset = 8,
    DssExecutionSQL = 10,
    DssExecutionViewPopulation = 12,
    DssExecutionViewCalculation = 13
} EnumDSSExecutionControl;

//typedef enum EnumDSSCurrentElementStatus
//{
//    DssCurrentElementReserved = 0,
//    DssCurrentElementUnset = 1,
//    DssCurrentElementChosen = 2,
//    DssCurrentElementDesired = 3
//} EnumDSSCurrentElementStatus;

typedef enum EnumDSSRWSelectorUnsetStatus
{
    DSSRWSelectorUnsetReserved = 0,
    DSSRWSelectorUnsetDefault = 1,
    DSSRWSelectorUnsetFirst = 2,
    DSSRWSelectorUnsetLast = 4
} EnumDSSRWSelectorUnsetStatus;

typedef enum EnumDSSColumnClass
{
    DssColumnClassReserved = 0,
    DssColumnClassHeader = 1,
    DssColumnClassExtra = 2,
    DssColumnClassGrid = 3
} EnumDSSColumnClass;

typedef enum EnumDSSWidthScenario
{
    DssWidthReserved = 0,
    DssWidthFitToDisplay = 1,
    DssWidthFitToContent = 2,
    DssWidthFixed = 3
} EnumDSSWidthScenario;

typedef enum EnumDSSKeepIndependent
{
    DssKeepIndependentReserved = 0,
    DssKeepIndependentDefault,
    DssKeepIndependentSet,
    DssKeepIndependentClear
} EnumDSSKeepIndependent;

typedef enum EnumDSSCustomGroupSortType
{
    DssCustomGroupSortReserved = 0,
    DssCustomGroupSortDefault = 1,
    DssCustomGroupSortID = 2,
    DssCustomGroupSortNoElement = 3
} EnumDSSCustomGroupSortType;

typedef enum EnumDSSEnableReportLevelCachingOptions
{
    DssEnableReportCachingReserved = 0x00000000,
    DssEnableReportLevelCachingNotSet =		0x00000001,
    DssEnableReportLevelCachingTrue =		0x00000002,
    DssEnableReportLevelCachingFalse =		0x00000003
} EnumDSSEnableReportLevelCachingOptions;

typedef enum EnumDSSServerDefManipulationMethod
{
    DssServerDefManipulationReserved,
    DssServerDefManipulationRegisterProject,
    DssServerDefManipulationUnregisterProject,
    DssServerDefManipulationModifyLoadOption,
    DssServerDefManipulationSetServerSetting,
    DssServerDefManipulationSetProjectSetting,
    DssServerDefManipulationModifyComm,
    DssServerDefManipulationAddDBRoles,
    DssServerDefManipulationRemoveDBRoles,
    DssServerDefManipulationModifyDesc,
    DssServerDefManipulationSetStatTableSetting,
    DssServerDefManipulationSetNotificationSetting,
    DssServerDefManipulationSetStatDBRole,
    DssServerDefManipulationMethodLastOne
} EnumDSSServerDefManipulationMethod;

typedef enum EnumDSSUserAccountServiceMethod
{
    DssUserAccountServiceReserved,
    DssUserAccountServiceCreateUser,
    DssUserAccountServiceDeleteUser,
    DssUserAccountServiceAddMember,
    DssUserAccountServiceRemoveMember,
    DssUserAccountServiceClearMembership,
    DssUserAccountServiceDuplicateUser,
    DssUserAccountServiceGetSecurityFilters,
    DssUserAccountServiceCreateGroup,
    DssUserAccountServiceSetSecurityFilters,
    DssUserAccountServiceDuplicateGroup,
    DssUserAccountServiceAdministrator,
    DssUserAccountServiceEveryone,
    DssUserAccountServicePulbic,
    DssUserAccountServiceSystemAdmins,
    DssUserAccountServiceSystemMonitors,
    DssUserAccountServiceUsers,
    DssUserAccountServiceGroups,
    DssUserAccountServiceCreateUserProfile,
    DssUserAccountServiceDeleteUserProfile,
    DssUserAccountServiceFindProfileFolder,
    DssUserAccountServiceGetAnalysisQuota,
    DssUserAccountServiceMethodLastOne
} EnumDSSUserAccountServiceMethod;

typedef enum EnumDSSSerializationBlockType
{
    DssSerializationBlockTypeNotFound = -1,
    DssSerializationBlockTypeICDSSTime = 1,
    DssSerializationBlockTypeICDSSTimeArray = 2
} EnumDSSSerializationBlockType;

typedef enum EnumDSSXDAType
{
    DssXDATypeReserved = 0,
    DssXDATypeRelational = 1,
    DssXDATypeRelationalTable = 1,
    DssXDATypeMDX = 2,
    DssXDATypeCustomSQLFreeForm = 3,
    DssXDATypeCustomSQLWizard = 4,
    DssXDATypeDataImport = 0x00000100,
    DssXDATypeDataImportFileExcel = 0x00000010  | DssXDATypeDataImport,
    DssXDATypeDataImportFileText = 0x00000020  | DssXDATypeDataImport,
    DssXDATypeDataImportCustomSQL = 0x00000030  | DssXDATypeDataImport,
    DssXDATypeDataImportTable = 0x00000040  | DssXDATypeDataImport,
    DssXDATypeDataImportOAuth = 0x00000050  | DssXDATypeDataImport,
    DssXDATypeDataImportOldSFDC = DssXDATypeDataImportOAuth,
    DssXDATypeDataImportCustomSQLWizard = 0x00000060   | DssXDATypeDataImport,
    DssXDATypeDataImportSFDC = 0x00000001  | DssXDATypeDataImportOAuth,
    DssXDATypeDataImportOAuthSFDC = 0x00000001  | DssXDATypeDataImportOAuth,
    DssXDATypeDataImportOAuthGDocs = 0x00000002  | DssXDATypeDataImportOAuth,
    DssXDATypeReduceMask = 0xFFFFFF00,
    DssXDATypeGroupMask = 0xFFFFFF00,
    DssXDATypeGroupMaskDataImport = 0xFFFFFFF0
} EnumDSSXDAType;

typedef enum EnumDSSXDASubtype
{
    DssXDASubtypeUnknown = -1,
    DssXDASubtypeReserved = 0,
    DssXDASubtypeRelational = 1,
    DssXDASubtypeMDXSAP = 2,
    DssXDASubtypeMDXMSAS2K = 3,
    DssXDASubtypeMDXMSAS2K5 = 4,
    DssXDASubtypeMDXHYSL = 5,
    DssXDASubtypeMDXHYSL7 = 5,
    DssXDASubtypeMDXHYSL9 = 6,
    DssXDASubtypeMDXHYSL9x = 7,
    DssXDASubtypeMDXTM1 = 8,
    DssXDASubtypeCount
} EnumDSSXDASubtype;

typedef enum EnumDSSMDXObjectType
{
    DssMDXObjectTypeReserved = 0,
    DssMDXObjectTypeLevel = 1,
    DssMDXObjectTypeMeasure = 2,
    DssMDXObjectTypeProperty = 3,
    DssMDXObjectTypeLevelDESCForm = 4,
    DssMDXObjectTypeHierarchy = 5,
    DssMDXObjectTypeDimension = 6,
    DssMDXObjectTypeLevelSourceOrder = 7
} EnumDSSMDXObjectType;

typedef enum EnumDSSMDXDimensionType
{
    DssMDXDimensionTypeReserved = 0,
    DssMDXDimensionTypeTime = 1,
    DssMDXDimensionTypeMeasure = 2,
    DssMDXDimensionTypeOther = 3,
    DssMDXDimensionTypeUnknown = 4
} EnumDSSMDXDimensionType;

typedef enum EnumDSSMDXPropertyType
{
    DssMDXPropertyTypeReserved = 0,
    DssMDXPropertyTypeMember = 0x0001,
    DssMDXPropertyTypeSystem = 0x0004,
    DssMDXPropertyTypeNotInRowset = 0x80000000
} EnumDSSMDXPropertyType;

typedef enum EnumDSSDimensionInternalType
{
    DssDimensionInternalTypeReserved = 0,
    DssDimensionInternalTypePrivate = 1,
    DssDimensionInternalTypeShared = 2,
    DssDimensionInternalTypeRelay = 3
} EnumDSSDimensionInternalType;

typedef enum EnumDSSMDXHierarchyStructure
{
    DssMDXHierarchyStructureReserved = 0,
    DssMDXHierarchyStructureFullyBalanced = 1,
    DssMDXHierarchyStructureRaggedBalanced = 2,
    DssMDXHierarchyStructureUnbalanced = 3,
    DssMDXHierarchyStructureNetwork = 4
} EnumDSSMDXHierarchyStructure;

typedef enum EnumDSSObjectScope
{
    DssObjectScopeReserved = 0,
    DssObjectScopeProject = 1,
    DssObjectScopeTable = 2
} EnumDSSObjectScope;

typedef enum EnumDSSImportOptions
{
    DssImportOptionReserved = -1,
    DssImportOptionDefault = 0,
    DssImportOptionDoNotCheckPrivilege = 1,
    DssImportOptionFromWeb = 2
} EnumDSSImportOptions;

typedef enum EnumDSSImportFlags
{
    DssImportFlagDefault = 0x0000000,
    DssImportFlagDoNotReuse = 0x00000001,
    DssImportFlagCreateReport = 0x00000100,
    DssImportFlagAddDefaultContent = 0x00000200,
    DssImportFlagObjectModifiable = 0x00000800,
    DssImportFlagsOverwrite = 0x00001000,
    DssImportFlagSaveObjects = 0x00100000,
    DssImportFlagDeleteUnusedObjects = 0x00200000,
    DssImportFlagDelaySaving = 0x00400000,
    DssImportFlagSavePhysicalObjects = 0x00800000,
    DssImportFlagNewObjectsManaged = 0x01000000,
    DssImportFlagGetMeasuresInDimension = 0x10000000
} EnumDSSImportFlags;

typedef enum EnumDSSImportDestination
{
    DssImportDestinationDefault = 0,
    DssImportDestinationTables = 1,
    DssImportDestinationDBTables = 2,
    DssImportDestinationColumns = 3,
    DssImportDestinationAttributes = 4,
    DssImportDestinationAttributeForms = 5,
    DssImportDestinationDimensions = 6,
    DssImportDestinationFacts = 7,
    DssImportDestinationMetrics = 8,
    DssImportDestinationReports = 9,
    DssImportDestinationPrompts = 10,
    DssImportDestinationCount = 11
} EnumDSSImportDestination;

typedef enum EnumDSSSchemaPreloading
{
    DssSchemaPreloadingReserved = 0,
    DssSchemaPreloadingDoNotLoad = 1,
    DssSchemaPreloadingLoad = 2
} EnumDSSSchemaPreloading;

typedef enum EnumDSSXDARefreshStatus
{
    DssXDARefreshStatusReserved = -1,
    DssXDARefreshStatusInitial = 0,
    DssXDARefreshStatusUnchanged = 1,
    DssXDARefreshStatusChanged = 2,
    DssXDARefreshStatusMissing = 3,
    DssXDARefreshStatusNew = 4
} EnumDSSXDARefreshStatus;

typedef enum EnumDSSQBJoinType
{
    DssQBJoinTypeReserved = -1,
    DssQBJoinTypeInner = 0,
    DssQBJoinTypeLeftOuter = 1,
    DssQBJoinTypeRightOuter = 2,
    DssQBJoinTypeFullOuter = 3
} EnumDSSQBJoinType;

typedef enum EnumDSSXDAMDXSAPVariableType
{
    DssXDAMDXSAPVariableTypeReserved = 0,
    DssXDAMDXSAPVariableTypeMember = 1,
    DssXDAMDXSAPVariableTypeNumeric = 2,
    DssXDAMDXSAPVariableTypeHierarchy = 3,
    DssXDAMDXSAPVariableTypeUpperBound = 3
} EnumDSSXDAMDXSAPVariableType;

typedef enum EnumDSSXDAMDXSAPVariableProcessingType
{
    DssXDAMDXSAPVariableProcessingTypeReserved = 0,
    DssXDAMDXSAPVariableProcessingTypeUserInput = 1,
    DssXDAMDXSAPVariableProcessingTypeUpperBound = 1
} EnumDSSXDAMDXSAPVariableProcessingType;

typedef enum EnumDSSXDAMDXSAPVariableSelectionType
{
    DssXDAMDXSAPVariableSelectionTypeReserved = 0,
    DssXDAMDXSAPVariableSelectionTypeValue = 1,
    DssXDAMDXSAPVariableSelectionTypeInterval = 2,
    DssXDAMDXSAPVariableSelectionTypeComplex = 3,
    DssXDAMDXSAPVariableSelectionTypeSelection = 4,
    DssXDAMDXSAPVariableSelectionTypeUpperBound = 4
} EnumDSSXDAMDXSAPVariableSelectionType;

typedef enum EnumDSSXDAMDXSAPVariableInputType
{
    DssXDAMDXSAPVariableInputTypeReserved = -1,
    DssXDAMDXSAPVariableInputTypeOptional = 0,
    DssXDAMDXSAPVariableInputTypeMandatory = 1,
    DssXDAMDXSAPVariableInputTypeMandatoryNotInitial = 2,
    DssXDAMDXSAPVariableInputTypeUpperBound = 2
} EnumDSSXDAMDXSAPVariableInputType;

typedef enum EnumDSSFormRestrictionFlags
{
    DssFormRestrictionNone = 0,
    DssFormRestrictionNotSupported = 1,
    DssFormRestrictionNotForSAPFiltering = 2,
    DssFormRestrictionCompound = 4,
    DssFormRestrictionNotExternalKeyForm = 8
} EnumDSSFormRestrictionFlags;

typedef enum EnumDSSDimensionRestrictionFlags
{
    DssDimensionRestrictionNone = 0,
    DssDimensionRestrictionNotForSAPFiltering = 1
} EnumDSSDimensionRestrictionFlags;

typedef enum EnumDSSMDXMemberPropertyInfoIndex
{
    DssMDXMemberPropertyInfoReserved = -1,
    DssMDXMemberPropertyInfoAll = 0,
    DssMDXMemberPropertyInfoSyntax = 1,
    DssMDXMemberPropertyInfoCreation = 2,
    DssMDXMemberPropertyInfoMapping = 3,
    DssMDXMemberPropertyInfoInRowset = 4,
    DssMDXMemberPropertyInfoCount = 5
} EnumDSSMDXMemberPropertyInfoIndex;

typedef enum EnumDSSMDXMemberPropertyInfoMask
{
    DssMDXMemberPropertyAllMask = 0xFFFFFFFF,
    DssMDXMemberPropertySyntaxCodeMask = 0x0000000F,
    DssMDXMemberPropertyCreationCodeMask = 0x00000030,
    DssMDXMemberPropertyMappingCodeMask = 0x000000C0,
    DssMDXMemberPropertyInRowsetCodeMask = 0x00000300,
    DssMDXMemberPropertyReservedFieldMask = 0xF0000000,
    DssMDXMemberPropertyFlagNotSupported = 0x80000000
} EnumDSSMDXMemberPropertyInfoMask;

typedef enum EnumDSSMDXMemberPropertyCreation
{
    DssMDXMemberPropertyCreationNo = 0x0,
    DssMDXMemberPropertyCreationYes = 0x1,
    DssMDXMemberPropertyCreationDefault = 0x3
} EnumDSSMDXMemberPropertyCreation;

typedef enum EnumDSSMDXMemberPropertyMapping
{
    DssMDXMemberPropertyMappingNo = 0x0,
    DssMDXMemberPropertyMappingYes = 0x1,
    DssMDXMemberPropertyMappingDefault = 0x3
} EnumDSSMDXMemberPropertyMapping;

typedef enum EnumDSSMDXMemberPropertyInRowset
{
    DssMDXMemberPropertyInRowsetNo = 0x0,
    DssMDXMemberPropertyInRowsetYes = 0x1,
    DssMDXMemberPropertyInRowsetDefault = 0x3
} EnumDSSMDXMemberPropertyInRowset;

typedef enum EnumDSSMDXMemberPropertySyntax
{
    DssMDXMemberPropertySyntaxDefault = 0x0,
    DssMDXMemberPropertySyntaxProperty = 0x1,
    DssMDXMemberPropertySyntaxDimensionProperty = 0x2,
    DssMDXMemberPropertySyntaxDimensionLevelProperty = 0x3,
    DssMDXMemberPropertySyntaxLevelProperty = 0x4
} EnumDSSMDXMemberPropertySyntax;

typedef enum EnumDSSDocumentSaveAs
{
    DssDocumentSaveAsOverwrite = 0x01,
    DssDocumentSaveAsDefaultPromptAnswers = 0x02,
    DssDocumentSaveAsPromptAnswers = 0x04,
    DssDocumentSaveAsRemoveNonPrimaryNameTranslations = 0x08
} EnumDSSDocumentSaveAs;

typedef enum EnumDSSDocumentManipulationType
{
    DssDocumentManipulationReserved = -1,
    DssDocumentManipulationNone = 0,
    DssDocumentManipulationFormat = 1,
    DssDocumentManipulationReExecute = 2,
    DssDocumentManipulationGroupByUnits = 4,
    DssDocumentManipulationReportViews = 8,
    DssDocumentManipulationGroupByElements = 0x1000,
    DssDocumentManipulationEvaluateViews = 0x2000,
    DssDocumentManipulationRemoveViews = 0x400
} EnumDSSDocumentManipulationType;

typedef enum EnumDSSRWIterationStatus
{
    DSSRWIterationStatusGo = 0x00000001,
    DSSRWIterationStatusRollback = 0x00000002,
    DSSRWIterationStatusStop = 0x00000003,
    DSSRWIterationStatusSequentialMask = 0x00000003,
    DSSRWIterationStatusBitwiseMask = 0xE0000000,
    DSSRWIterationStatusPartialParentRequested = 0x20000000,
    DSSRWIterationStatusPartial = 0x40000000,
    DSSRWIterationStatusDontFail = 0x80000000,
    DSSRWIterationStatusNoSubtree = 0x01000000,
    DSSRWIterationStatusCurrentPanel = 0x02000000
} EnumDSSRWIterationStatus;

typedef enum EnumDSSIncrementalFilterStatus
{
    DSSIncrementalFilterStatusNotSet = 0x00000001,
    DSSIncrementalFilterStatusAvailable = 0x00000002,
    DSSIncrementalFilterStatusPartiallyAvailable = 0x00000004,
    DSSIncrementalFilterStatusUnAvailable = 0x00000008
} EnumDSSIncrementalFilterStatus;

typedef enum EnumCSSCollectionStatus
{
    EnumTypeCSSCollectionReserved = 0x0,
    EnumTypeCSSCollectionReInitTemplate = 0x1,
    EnumTypeCSSCollectionClear = 0x2,
    EnumTypeCSSCollectionInfoRemove = 0x4
} EnumCSSCollectionStatus;

typedef enum EnumDSSDocumentReportViewStatus
{
    DssDocumentViewStatusEmpty = 0x0,
    DssDocumentViewStatusReady = 0x1,
    DssDocumentViewStatusAdded = 0x2,
    DssDocumentViewStatusRemoved = 0x4,
    DssDocumentViewStatusChanged = 0x8,
    DssDocumentViewStatusViewDataChange = 0x10,
    DssDocumentViewStatusViewCalculationChange = 0x20,
    DssDocumentViewStatusViewXTabChange = 0x40,
    DssDocumentViewStatusChangedForTransaction = 0x80
} EnumDSSDocumentReportViewStatus;

typedef enum EnumDSSRWPreLoadPanelState
{
    DssRWPreLoadAllPanels = 0x0,
    DssRWLoadCurrentPanel = 0x1,
    DssRWUseDocumentSetting = 0x2
} EnumDSSRWPreLoadPanelState;

typedef enum EnumDSSRWManipulationMethod
{
    DssRWManipulationAddDataSet = 1,
    DssRWManipulationRemoveDataSet = 2,
    DssRWManipulationClearDataSets = 3,
    DssRWManipulationAddNode = 4,
    DssRWManipulationRemoveNode = 5,
    DssRWManipulationClearNodes = 6,
    DssRWManipulationFormatNode = 7,
    DssRWManipulationEditNode = 8,
    DssRWManipulationAddGroupByUnit = 9,
    DssRWManipulationAddSort = 10,
    DssRWManipulationAddField = 11,
    DssRWManipulationAddObjectField = 12,
    DssRWManipulationEditField = 13,
    DssRWManipulationEditObjectField = 14,
    DssRWManipulationRemoveField = 15,
    DssRWManipulationClearFields = 16,
    DssRWManipulationFormatField = 17,
    DssRWManipulationAddAttributeFormToField = 18,
    DssRWManipulationReportManipulation = 19,
    DssRWManipulationSetDocumentProperties = 20,
    DssRWManipulationFormatGroupByUnit = 21,
    DssRWManipulationFormatGroupByUnitXML = 22,
    DssRWManipulationAddAttributeFormToGroupByUnit = 23,
    DssRWManipulationSetPageThroughElement = 24,
    DssRWManipulationEditSectionNode = 25,
    DssRWManipulationAddGroupBySection = 26,
    DssRWManipulationRemoveGroupBySection = 27,
    DssRWManipulationMoveGroupBySection = 28,
    DssRWManipulationClearSorts = 29,
    DssRWManipulationMoveNode = 30,
    DssRWManipulationSetDefaultDataSet = 31,
    DssRWManipulationSetGridDisplayMode = 32,
    DssRWManipulationCopyField = 33,
    DssRWManipulationMoveField = 34,
    DssRWManipulationCopyTemplateNode = 35,
    DssRWManipulationMoveTemplateNode = 36,
    DssRWManipulationSetGraphProperties = 37,
    DssRWManipulationSetControlDefaultFormat = 38,
    DssRWManipulationAddCopyTemplateNodeFromDataset = 39,
    DssRWManipulationAddDerivedMetricToDataset = 40,
    DssRWManipulationUpdateDerivedMetricOnDataset = 41,
    DssRWManipulationRemoveDerivedMetricFromDataset = 42,
    DssRWManipulationSetPageByStyle = 43,
    DssRWManipulationSetCurrentGroupByElement = 44,
    DssRWManipulationSetNextCurrentGroupByElement = 45,
    DssRWManipulationFormatDerivedMetricXML = 46,
    DssRWManipulationShowDocumentThresholds = 47,
    DssRWManipulationHideDocumentThresholds = 48,
    DssRWManipulationSetViewFilter = 49,
    DssRWManipulationAddLayout = 50,
    DssRWManipulationSetCurrentLayout = 51,
    DssRWManipulationImportDocument = 52,
    DssRWManipulationCreateThreshold = 53,
    DssRWManipulationUpdateThreshold = 54,
    DssRWManipulationRemoveThreshold = 55,
    DssRWManipulationReorderThresholds = 56,
    DssRWManipulationSetViewTemplate = 57,
    DssRWManipulationEditDataSet = 58,
    DssRWManipulationAddLinkTemplateNodeToDataset = 59,
    DssRWManipulationChangeTemplateNodeOrigin = 60,
    DssRWManipulationAddControl = 61,
    DssRWManipulationFormatControl = 62,
    DssRWManipulationSetCurrentControlElements = 63,
    DssRWManipulationInsertControlGroupBySection = 64,
    DssRWManipulationClearCurrentControlElements = 65,
    DssRWManipulationRemoveControlGroupBySection = 66,
    DssRWManipulationAddControlTarget = 67,
    DssRWManipulationRemoveControlTarget = 68,
    DssRWManipulationClearControlTargets = 69,
    DssRWManipulationRemoveControl = 70,
    DssRWManipulationClearControls = 71,
    DssRWManipulationSetCurrentNode = 72,
    DssRWManipulationSetControlSource = 73,
    DssRWManipulationCopyControl = 74,
    DssRWManipulationSetCurrentControlElementsFromGraph = 75,
    DssRWManipulationSetAbbreviation = 76,
    DssRWManipulationClearGroupByUnits = 77,
    DssRWManipulationSetControlShowAll = 78,
    DssRWManipulationMakeTabular = 79,
    DssRWManipulationPopulateTemplateNodeFromDataset = 80,
    DssRWManipulationSetControlAllAlias = 81,
    DssRWManipulationSetCurrentControlElementStatus = 82,
    DssRWManipulationSetRWDObjectProperties = 83,
    DssRWManipulationAddRWDDefinitionSubtotal = 84,
    DssRWManipulationRemoveRWDDefinitionSubtotal = 85,
    DssRWManipulationRemoveGroupByAndFieldGroupSortInLayout = 87,
    DssRWManipulationSetWidgetString = 88,
    DssRWManipulationRemoveWidgetString = 89,
    DssRWManipulationCopyPanel = 90,
    DssRWManipulationCopyPanelStack = 91,
    DssRWManipulationMoveControlGroupBySection = 92,
    DssRWManipulationSetHyperlinksXML = 93,
    DssRWManipulationSwitchControlGroupBySectionToUnitCondition = 94,
    DssRWManipulationSwitchUnitConditionToControlGroupBySection = 95,
    DssRWManipulationSetControlUnsetStatus = 96,
    DssRWManipulationReplaceDataset = 97,
    DssRWManipulationSetCurrentGroupByElementByUnitID = 98,
    DssRWManipulationChangeControlCondition = 99,
    DssRWManipulationToggleCurrentSelectionOnControl = 100,
    DssRWManipulationAddTransactionReport = 101,
    DssRWManipulationRemoveTransactionReport = 102,
    DssRWManipulationClearTransactionReports = 103,
    DssRWManipulationSetTransactionReport = 104,
    DssRWManipulationSetControlAvailableActionTypes = 105,
    DssRWManipulationSetTransactionInfoOnField = 106,
    DssRWManipulationSetControlSubsequentAction = 107,
    DssRWManipulationPerformActionOnActionSelector = 108,
    DssRWManipulationUpdateTransactionData = 109,
    DssRWManipulationSetTransactionChangeType = 110,
    DssRWManipulationSetSubsequentActionObject = 111,
    DssRWManipulationSetSubsequentDisplayMessage = 112,
    DssRWManipulationRemoveTransactionInfoOnField = 113,
    DssRWManipulationSetControlDisplayText = 114,
    DssRWManipulationAddShortcutMetric = 115,
    DssRWManipulationSetDelimiter = 116,
    DssRWManipulationSetAttributeFormDisplayOption = 117,
    DssRWManipulationSetAttributeForms = 118,
    DssRWManipulationAddDocumentView = 119,
    DssRWManipulationRemoveDocumentView = 120,
    DssRWManipulationSetCurrentDocumentView = 121,
    DssRWManipulationEditDocumentView = 122,
    DssRWManipulationSetControlDataSource = 124,
    DssRWManipulationSetControlWriteBackForm = 125,
    DssRWManipulationSetControlFilterType = 126,
    DssRWManipulationDrill = 130,
    DssRWManipulationAddAssociatedRWNodeInfo = 131,
    DssRWManipulationClearAssociatedRWNodeInfo = 132,
    DssRWManipulationValidateDerivedMetric = 133,
    DssRWManipulationApplyReconciliation = 140,
    DssRWManipulationLastOne
} EnumDSSRWManipulationMethod;

//typedef enum EnumDSSIteratorType
//{
//    DssIteratorReserved = 0,
//    DssIteratorSection,
//    DssIteratorGridGraph,
//    DssIteratorFieldGroup,
//    DssIteratorRWControl
//} EnumDSSIteratorType;

typedef enum EnumDSSExportFormat
{
    DssExportFormatReserved = 0,
    DssExportFormatExcel = 1,
    DssExportFormatPdf = 2,
    DssExportFormatCSV = 3,
    DssExportFormatHtml = 4,
    DssExportFormatPlainText = 5,
    DssExportFormatXml = 6,
    DssExportFormatFlash = 7,
    DssExportFormatView = 8,
    DssExportFormatInteractive = 9,
    DssExportFormatEditable = 10,
    DssExportFormatExportFlash = 11,
    DssExportFormatFlashInMht = 12,
    DssExportFormatFlashInPdf = 13,
    DssExportFormatMobileSmartPhone = 14,
    DssExportFormatMobileTablet = 15,
    DssExportFormatLastOne,
    DssExportFormatInstance = 100
} EnumDSSExportFormat;

//typedef enum EnumDSSRWTreeType
//{
//    DssRWTreeTypeReserved = 0,
//    DssRWTreeTypeContent,
//    DssRWTreeTypePageHeader,
//    DssRWTreeTypePageFooter,
//    DssRWTreeTypeClipBoard,
//    DssRWTreeTypeLastOne
//} EnumDSSRWTreeType;

//typedef enum EnumDSSFieldGroupDataLevel
//{
//    DssFieldGroupDataLevelReserved = 0,
//    DssFieldGroupDataLevelDefault,
//    DssFieldGroupDataLevelSimple,
//    DssFieldGroupDataLevelDefaultReport,
//    DssFieldGroupDataLevelGroupBy,
//    DssFieldGroupDataLevelLastOne
//} EnumDSSFieldGroupDataLevel;
//
//typedef enum EnumDSSFieldGroupJoin
//{
//    DssFieldGroupJoinReserved = 0,
//    DssFieldGroupConcatenate,
//    DssFieldGroupCompound,
//    DssFieldGroupCrossJoin,
//    DssFieldGroupJoinLastOne
//} EnumDSSFieldGroupJoin;

typedef enum EnumDSSRWDIssueType
{
    DssRWDIssueReserved = 0,
    DssRWDMissingUnit = 1,
    DssRWDMissingForm = 2,
    DssRWDMissingMetricLeaf = 3,
    DssRWDBadMetricInput = 4,
    DssRWDBadUnitType = 5,
    DssRWDMetricLimit = 6,
    DssRWDBadPageByAxis = 7,
    DssRWDMissingDataSource = 8,
    DssRWDUnresolvedField = 9,
    DssRWDMissingSortObject = 10,
    DssRWDNonEmptyTemplate = 11,
    DssRWDEmptyForms = 12,
    DssRWDBadTemplateOrigin = 13,
    DssRWDMissingTarget = 14,
    DssRWDDanglingTarget = 15,
    DssRWDIssueLastOne
} EnumDSSRWDIssueType;

typedef enum EnumDSSDefaultFormats
{
    DSSDefaultFormatReserved = 0,
    DSSDefaultFormatLabel = 1,
    DSSDefaultFormatText = 2,
    DSSDefaultFormatImage = 3,
    DSSDefaultFormatCrosstab = 4,
    DSSDefaultFormatLine = 5,
    DSSDefaultFormatShape = 6,
    DSSDefaultFormatSection = 7,
    DSSDefaultFormatTitle = 8,
    DSSDefaultFormatHTML = 9,
    DSSDefaultFormatPanelStack = 10,
    DSSDefaultFormatControl = 11,
    DSSDefaultFormatUnselectedItem = 12,
    DSSDefaultFormatSelectedItem = 13,
    DSSDefaultFormatHighlightedItem = 14,
    DSSDefaultFormatGridTitle = 15,
    DSSDefaultFormatPanelStackTitle = 16,
    DSSDefaultFormatPanel = 17,
    DSSDefaultFormatControlTitle = 18,
    DSSDefaultFormatLastOne
} EnumDSSDefaultFormats;

typedef EnumDSSDataFlag EnumDSSCellInfoFlags;
//typedef enum EnumDSSCellInfoFlags
//{
//    DssDataOk = 0x00,
//    DssDataNull = 0x01,
//    DssDataInvalidCell = 0x02,
//    DssDataTruncate = 0x04,
//    DssDataOverflow = 0x08,
//    DssDataUnderflow = 0x10,
//    DssDataZerodivide = 0x20,
//    DssDataInvalid = 0x40,
//    DssDataUnknown = 0x80,
//    DssDataUnCalculated = 0x81,
//    DssDataMissing = 0x82,
//    DssDataRWInvalid = 0x03,
//    DssDataIgnoreCheckNull = 0x83
//} EnumDSSCellInfoFlags;

//typedef enum EnumDSSRWLinkType
//{
//    DssRWLinkReserved = 0,
//    DssRWLinkHyperLink,
//    DssRWLinkImage,
//    DssRWLinkToolTip,
//    DssRWLinkURL,
//    DssRWLinkHTML,
//    DssRWLinkResolvedHyperLinks,
//    DssRWLinkLastOne
//} EnumDSSRWLinkType;

typedef enum EnumDSSObjectCollectionSerializationFlags
{
    DssObjectCollectionSerializationReserved = 0x00000000,
    DssObjectCollectionSerializationVersionIDNullForRTF = 0x00000001,
    DssObjectCollectionSerializationRequest = 0x00000002,
    DssObjectCollectionSerializationForCache = 0x00000004
} EnumDSSObjectCollectionSerializationFlags;

typedef enum EnumDSSSearchScope
{
    DssSearchScopeReserved = 0,
    DssSearchScopeStandaloneObjects = 0x00000001,
    DssSearchScopeManagedObjects = 0x00000002,
    DssSearchScopeRootedObjects = 0x00000004,
    DssSearchScopeUnrootedObjects = 0x00000008,
    DssSearchScopeManagedMask = DssSearchScopeStandaloneObjects  | DssSearchScopeManagedObjects,
    DssSearchScopeRootedMask = DssSearchScopeRootedObjects  | DssSearchScopeUnrootedObjects,
    DssSearchScopeAllObjects = 0x0000001f
} EnumDSSSearchScope;

typedef enum EnumDSSSynchFlags
{
    DssSynchFlagsReserved = 0x00000000,
    DssSynchObjectParentChanged = 0x00000001,
    DssSynchObjectVersionChanged = 0x00000002,
    DssSynchObjectCreationTimeChanged = 0x00000004,
    DssSynchObjectModificationTimeChanged = 0x00000008,
    DssSynchObjectNameChanged = 0x00000010,
    DssSynchObjectOwnerChanged = 0x00000020,
    DssSynchObjectOutOfDate = 0x00000040
} EnumDSSSynchFlags;

typedef enum EnumDSSAPIVersion
{
    DssAPIVersionReserved = 0,
    DssAPIVersion700 = 7000,
    DssAPIVersion710 = 7100,
    DssAPIVersion711 = 7101,
    DssAPIVersion720 = 7500,
    DssAPIVersion721 = 7510,
    DssAPIVersion722 = 7520,
    DssAPIVersion723 = 7530,
    DssAPIVersion724 = 7540,
    DssAPIVersion750 = 7600,
    DssAPIVersion751 = 7610,
    DssAPIVersion752 = 7620,
    DssAPIVersion753 = 7630,
    DssAPIVersion754 = 7640,
    DssAPIVersion800 = 8000,
    DssAPIVersion801 = 8010,
    DssAPIVersion802 = 8020,
    DssAPIVersion803 = 8030,
    DssAPIVersion810 = 8100,
    DssAPIVersion812 = 8120,
    DssAPIVersion820 = 8200,
    DssAPIVersion900 = 9000,
    DssAPIVersion901 = 9010,
    DssAPIVersion902 = 9020,
    DssAPIVersion920 = 9200,
    DssAPIVersion921 = 9210,
    DssAPIVersion921a = 9211,
    DssAPIVersion921m = 9212,
    DssAPIVersion930 = 9300
} EnumDSSAPIVersion;

typedef enum EnumDSSRWExportOption
{
    DssRWExportDefault = -1,
    DssRWExportAll = 0x00000000,
    DssRWExportCurrent = 0x00000001,
    DssRWExportNoSlicing = 0x00000002,
    DssRWExportPPE = 0x00000004,
    DssRWExportXML = 0x00000008,
    DssRWExportTEXT = 0x00000010,
    DssRWExportBinary = 0x00000020,
    DssRWExportPartialRetrieval = 0x00000040,
    DssRWExportSlice = 0x00000080,
    DssRWExportOnDemandCurrent = 0x00000100
} EnumDSSRWExportOption;

typedef enum EnumDSSDetailsContextProperties
{
    DssDetailsContextPropertyLocale = 0,
    DssDetailsContextPropertyResultUpdateTime = 1,
    DssDetailsContextPropertyReportInstance = 2,
    DssDetailsContextPropertyTextExFlags = 3,
    DssDetailsContextPropertyFilterDetailsFormat = 4,
    DssDetailsContextFilterDetailsMode = 5,
    DssDetailsContextShowAttributeName = 6,
    DssDetailsContextCarriageAfterAttributeName = 7,
    DssDetailsContextCarriageBetweenElements = 8,
    DssDetailsContextTrimEelements = 9,
    DssDetailsContextUseNameOrSymbolOperator = 10,
    DssDetailsContextIncludeAttributeFormName = 11,
    DssDetailsContextParentheseAroundConditions = 12,
    DssDetailsContextCarriageBetweenConditions = 13,
    DssDetailsContextLogicalOperatorBetweenConditions = 14,
    DssDetailsContextExpandShortcutFilters = 15,
    DssDetailsContextUseDelimetersAroundObjectName = 16,
    DssDetailsContextSeparatorAfterAttributeName = 17,
    DssDetailsContextDelimitersAroundElementList = 18,
    DssDetailsContextSeparatorBetweenElements = 19,
    DssDetailsContextSeparatorBetweenLastTwoElements = 20,
    DssDetailsContextCarriageReturnBetweenElements = 21,
    DssDetailsContextApplyCapitalizationToElements = 22,
    DssDetailsContextDisplayedForms = 23,
    DssDetailsContextDelimiterAroundStringsInAQs = 24,
    DssDetailsContextTreatInAsInListConditions = 25,
    DssDetailsContextCapitalizationForLogicalOperators = 26,
    DssDetailsContextReportFilter = 27,
    DssDetailsContextViewFilter = 28,
    DssDetailsContextMetricLimits = 29,
    DssDetailsContextUnitConditions = 30,
    DssDetailsContextFiltersInConditionalMetrics = 31,
    DssDetailsContextDelimitersAroundObjectNames = 32,
    DssDetailsContextRetrieveSpecificConditionsOnly = 33,
    DssDetailsContextShowAttributeNameElementPrompt = 34,
    DssDetailsContextFilterDetailsAttributeName = 35,
    DssDetailsIsViewFilterExpression = 36,
    DssDetailsIncludeMQsInViewFilter = 37,
    DssDetailsExprConditionSeparator = 38,
    DssDetailsIndentTab = 39,
    DssDetailsReportDetailsContents = 40,
    DssDetailsFilterDetailsContents = 41,
    DssDetailsFilterDetailsLabels = 42,
    DssDetailsFilterDetailsLabelsCR = 43,
    DssDetailsFilterDetailsSubExpressionCR = 44,
    DssDetailsFilterDetailsReportFilterName = 45,
    DssDetailsFilterDetailsOrder = 46,
    DssDetailsPromptDetailsTitleIndex = 47,
    DssDetailsMetricsDimention = 48,
    DssDetailsMetricsTransformation = 49,
    DssDetailsMetricsFormula = 50,
    DssDetailsMetricsConditionality = 51,
    DssDetailsConditionalMetricsOnly = 52,
    DssDetailsViewBaseUnits = 53,
    DssDetailsNonMetricsUnits = 54,
    DssDetailsBaseTemplateName = 55,
    DssDetailsTemplateMetrics = 56,
    DssDetailsTemplateDescription = 57,
    DssDetailsPromptIndex = 58,
    DssDetailsApplicationType = 59,
    DssDetailsApplicationSubType = 60,
    DssDetailsFilterDetailsLabelsDefault = 61,
    DssDetailsFilterDetailsLabelsCRDefault = 62,
    DssDetailsReportDetailsReportDescription = 63,
    DssDetailsReportDetailsFilterDetails = 64,
    DssDetailsReportDetailsPromptDetails = 65,
    DssDetailsReportDetailsTemplateDetails = 66,
    DssDetailsFilterDetailsReportFilter = 67,
    DssDetailsFilterDetailsViewFilter = 68,
    DssDetailsFilterDetailsReportLimits = 69,
    DssDetailsFilterDetailsViewLimits = 70,
    DssDetailsFilterDetailsUnitConditions = 71,
    DssDetailsFilterDetailsUserDrillFilter = 72,
    DssDetailsCustomDelimiterForLastTwoElements = 73,
    DssDetailsIncludeUserSecurityFilters = 74,
    DssDetailsFilterDetailsObjectAlias = 75,
    DssDetailsReportFilterDescription = 76,
    DssDetailsCarriageReturnString = 77,
    DssDetailsReportDetailsAnnotations = 78,
    DssDetailsTemplateReference = 79,
    DssDetailsLogicalOperatorDepth = 80,
    DssDetailsDynamicDates = 81,
    DssDetailsPromptUnAnswered = 82,
    DssDetailsPromptEmptyExpressions = 83,
    DssDetailsPromptIncludeUnusedPrompts = 84,
    DssDetailsContextFilterDetailsFormatCount,
    DssDetailsPromptDefaultEmptyString,
    DssDetailsPromptDetailsIncludeLabel,
    DssDetailsReportDetailsNumberLocale,
    DssDetailsReportDetailsMessageLocale,
    DssDetailsReportDetailsMDLocale,
    DssDetailsReportDetailsInterfaceLocale,
    DssDetailsPromptValueTextFormatString,
    DssDetailsAttributeFormDataType,
    DssDetailsUserRuntime,
    DssDetailsDocumentDetailsDocumentDefnition,
    DssDetailsContextPropertyCount
} EnumDSSDetailsContextProperties;

typedef enum EnumDSSXMLContextProperties
{
    DssXMLContextPropertyDetailsContext = 0,
    DssXmlContextPropertyDPI = 1,
    DssXmlContextFormatValuesCollection = 2,
    DssXMLContextPropertyCount
} EnumDSSXMLContextProperties;

typedef enum EnumDSSXMLContextSetProperties
{
    DssXMLContextSetPropertyNodeKeyStrings = 0,
    DssXMLContextSetPropertyNodePathKeyStrings,
    DssXMLContextSetPropertyReferedDataElementIDStrings,
    DssXMLContextSetPropertyCount,
    DssXMLContextSetPropertyStringsCount = 3
} EnumDSSXMLContextSetProperties;

typedef enum EnumDSSXMLContextSetPropertyOperations
{
    DssXMLContextSetPropertyOperationInsert = 0,
    DssXMLContextSetPropertyOperationErase,
    DssXMLContextSetPropertyOperationFind,
    DssXMLContextSetPropertyOperationEmpty,
    DssXMLContextSetPropertyOperationSize,
    DssXMLContextSetPropertyOperationItem,
    DssXMLContextSetPropertyOperationCount
} EnumDSSXMLContextSetPropertyOperations;

typedef enum EnumDSSXMLContextMapProperties
{
    DssXMLContextMapPropertySettingIDToResultFlag = 0,
    DssXMLContextMapPropertySettingIDToResult2Flag,
    DssXMLContextMapPropertySettingIDToResult3Flag,
    DssXMLContextMapPropertyCount,
    DssXMLContextMapPropertyInt32ToInt32Count = 3
} EnumDSSXMLContextMapProperties;

typedef enum EnumDSSXMLContextMapPropertyOperations
{
    DssXMLContextMapPropertyOperationInsert = 0,
    DssXMLContextMapPropertyOperationErase,
    DssXMLContextMapPropertyOperationFind,
    DssXMLContextMapPropertyOperationFindKey,
    DssXMLContextMapPropertyOperationEmpty,
    DssXMLContextMapPropertyOperationSize
} EnumDSSXMLContextMapPropertyOperations;

typedef enum EnumDSSRWDocumentValidationOption
{
    DssRWDocumentValidationDefault = 0,
    DssRWDocumentValidationReparseLabel = 0x0001,
    DssRWDocumentValidationReresolveField = 0x0002,
    DssRWDocumentValidationCheckControl = 0x0004
} EnumDSSRWDocumentValidationOption;

typedef enum EnumDSSThresholdManipulationFlags
{
    DssThresholdManipulationDefault =	0x00000000
} EnumDSSThresholdManipulationFlags;
//
//typedef enum EnumDSSThresholdScope
//{
//    DssThresholdScopeReserved = 0,
//    DssThresholdScopeMetricOnly = 1,
//    DssThresholdScopeSubtotalOnly = 2,
//    DssThresholdScopeMetricAndSubtotal = 3
//} EnumDSSThresholdScope;

typedef enum EnumDSSSubtotalLevelType
{
    DssSubtotalLevelReserved = 0,
    DssSubtotalLevelAll = 1,
    DssSubtotalLevelGroupBy = 2,
    DssSubtotalLevelAcross = 3
} EnumDSSSubtotalLevelType;

typedef enum EnumDSSDataStatus
{
    DssDataStatusReserved = 0,
    DssDataStatusOk = 1,
    DssDataStatusMissing = 2,
    DssDataStatusUnknown = 3
} EnumDSSDataStatus;

typedef enum EnumMetricValueSpecialRequest
{
    DssMetricValueReserved =	0,
    DssMetricValueMinMax =	1
} EnumMetricValueSpecialRequest;

typedef enum EnumDSSReferenceQualities
{
    DssReferenceNoQualities = 0,
    DssReferenceStrong = 0x00000001,
    DssReferenceWeak = 0x00000002,
    DssReferenceAnyStrength = 0x0000000f,
    DssReferenceSelf = 0x00000010,
    DssReferenceOther = 0x00000020,
    DssReferenceSelfOther = 0x00000030,
    DssReferenceContainer = 0x00000100,
    DssReferenceNotContainer = 0x00000200,
    DssReferenceInternalEmbedded = 0x00000400,
    DssReferenceExternalEmbedded = 0x00000800,
    DssReferenceStandAlone = DssReferenceContainer  | DssReferenceNotContainer,
    DssReferenceEmbedded = DssReferenceInternalEmbedded  | DssReferenceExternalEmbedded,
    DssReferenceAllEmbedding = DssReferenceStandAlone  | DssReferenceEmbedded,
    DssReferenceThisProject = 0x00001000,
    DssReferenceCrossProject = 0x00002000,
    DssReferenceConfiguration = 0x00004000,
    DssReferenceNotConfiguration = 0x00008000,
    DssReferenceAnyProject = 0x0000f000,
    DssReferenceTargetInternal = DssReferenceContainer  | DssReferenceInternalEmbedded,
    DssReferenceTargetExternal = DssReferenceNotContainer  | DssReferenceExternalEmbedded,
    DssReferenceTargetContainer = 0x00010000,
    DssReferenceReplace = 0x00020000,
    DssReferenceInContainerObject = 0x00040000,
    DssReferenceInEmbeddedObjects = 0x00080000,
    DssReferenceSourceAllContainer = 0x000c0000,
    DssReferenceBroken =0x00100000
} EnumDSSReferenceQualities;

typedef enum EnumDSSLocale
{
    DssLocaleReserved = 0x007f,
    DssLocaleNeutral = 0x0000,
    DssLocalePrimary = 0x0800,
    DssLocaleEffective = 0x0400,
    DssLocalePrimaryActual = 0x0c00,
    DssLocaleLanguageIDMask = 0x000003ff,
    DssLocaleBaseLocaleIDMask = 0x0000ffff,
    DssLocaleMaximumID = 0x1fffffff,
    DssLocaleAfrikaans = 0x0436,
    DssLocaleAlbanian = 0x041c,
    DssLocaleArabicUnitedArabEmirates = 0x3801,
    DssLocaleArabicBahrain = 0x3c01,
    DssLocaleArabicAlgeria = 0x1401,
    DssLocaleArabicEgypt = 0x0c01,
    DssLocaleArabicIraq = 0x0801,
    DssLocaleArabicJordan = 0x2c01,
    DssLocaleArabicKuwait = 0x3401,
    DssLocaleArabicLebanon = 0x3001,
    DssLocaleArabicLibya = 0x1001,
    DssLocaleArabicMorocco = 0x1801,
    DssLocaleArabicOman = 0x2001,
    DssLocaleArabicQatar = 0x4001,
    DssLocaleArabicSaudiArabia = 0x0401,
    DssLocaleArabicSyria = 0x2801,
    DssLocaleArabicTunisia = 0x1c01,
    DssLocaleArabicYemen = 0x2401,
    DssLocaleArmenian = 0x042b,
    DssLocaleAzeriLatin = 0x042c,
    DssLocaleAzeriCyrillic = 0x082c,
    DssLocaleBasque = 0x042d,
    DssLocaleBelarusian = 0x0423,
    DssLocaleBulgarian = 0x0402,
    DssLocaleCatalan = 0x0403,
    DssLocaleChineseChina = 0x0804,
    DssLocaleChineseHongKongSAR = 0x0c04,
    DssLocaleChineseMacauSAR = 0x1404,
    DssLocaleChineseSingapore = 0x1004,
    DssLocaleChineseTaiwan = 0x0404,
    DssLocaleCroatian = 0x041a,
    DssLocaleCzech = 0x0405,
    DssLocaleDanish = 0x0406,
    DssLocaleDutchNetherlands = 0x0413,
    DssLocaleDutchBelgium = 0x0813,
    DssLocaleEnglishAustralia = 0x0c09,
    DssLocaleEnglishBelize = 0x2809,
    DssLocaleEnglishCanada = 0x1009,
    DssLocaleEnglishCaribbean = 0x2409,
    DssLocaleEnglishIreland = 0x1809,
    DssLocaleEnglishJamaica = 0x2009,
    DssLocaleEnglishNewZealand = 0x1409,
    DssLocaleEnglishPhillippines = 0x3409,
    DssLocaleEnglishSouthAfrica = 0x1c09,
    DssLocaleEnglishTrinidad = 0x2c09,
    DssLocaleEnglishUnitedKingdom = 0x0809,
    DssLocaleEnglishUnitedStates = 0x0409,
    DssLocaleEstonian = 0x0425,
    DssLocaleFarsi = 0x0429,
    DssLocaleFinnish = 0x040b,
    DssLocaleFaroese = 0x0438,
    DssLocaleFrenchFrance = 0x040c,
    DssLocaleFrenchBelgium = 0x080c,
    DssLocaleFrenchCanada = 0x0c0c,
    DssLocaleFrenchLuxembourg = 0x140c,
    DssLocaleFrenchSwitzerland = 0x100c,
    DssLocaleGaelicIreland = 0x083c,
    DssLocaleGaelicScotland = 0x043c,
    DssLocaleGermanGermany = 0x0407,
    DssLocaleGermanAustria = 0x0c07,
    DssLocaleGermanLiechtenstein = 0x1407,
    DssLocaleGermanLuxembourg = 0x1007,
    DssLocaleGermanSwitzerland = 0x0807,
    DssLocaleGreek = 0x0408,
    DssLocaleHebrew = 0x040d,
    DssLocaleHindi = 0x0439,
    DssLocaleHungarian = 0x040e,
    DssLocaleIcelandic = 0x040f,
    DssLocaleIndonesian = 0x0421,
    DssLocaleItalianItaly = 0x0410,
    DssLocaleItalianSwitzerland = 0x0810,
    DssLocaleJapanese = 0x0411,
    DssLocaleKorean = 0x0412,
    DssLocaleLatvian = 0x0426,
    DssLocaleLithuanian = 0x0427,
    DssLocaleMacedonian = 0x042f,
    DssLocaleMalayMalaysia = 0x043e,
    DssLocaleMalayBrunei = 0x083e,
    DssLocaleMaltese = 0x043a,
    DssLocaleMarathi = 0x044e,
    DssLocaleNorwegianBokml = 0x0414,
    DssLocaleNorwegianNynorsk = 0x0814,
    DssLocalePolish = 0x0415,
    DssLocalePortuguesePortugal = 0x0816,
    DssLocalePortugueseBrazil = 0x0416,
    DssLocaleRaetoRomance = 0x0417,
    DssLocaleRomanianRomania = 0x0418,
    DssLocaleRomanianMoldova = 0x0818,
    DssLocaleRussian = 0x0419,
    DssLocaleRussianMoldova = 0x0819,
    DssLocaleSanskrit = 0x044f,
    DssLocaleSerbianCyrillic = 0x0c1a,
    DssLocaleSerbianLatin = 0x081a,
    DssLocaleSetsuana = 0x0432,
    DssLocaleSlovenian = 0x0424,
    DssLocaleSlovak = 0x041b,
    DssLocaleSorbian = 0x042e,
    DssLocaleSpanishSpain = 0x0c0a,
    DssLocaleSpanishArgentina = 0x2c0a,
    DssLocaleSpanishBolivia = 0x400a,
    DssLocaleSpanishChile = 0x340a,
    DssLocaleSpanishColombia = 0x240a,
    DssLocaleSpanishCostaRica = 0x140a,
    DssLocaleSpanishDominicanRepublic = 0x1c0a,
    DssLocaleSpanishEcuador = 0x300a,
    DssLocaleSpanishGuatemala = 0x100a,
    DssLocaleSpanishHonduras = 0x480a,
    DssLocaleSpanishMexico = 0x080a,
    DssLocaleSpanishNicaragua = 0x4c0a,
    DssLocaleSpanishPanama = 0x180a,
    DssLocaleSpanishPeru = 0x280a,
    DssLocaleSpanishPuertoRico = 0x500a,
    DssLocaleSpanishParaguay = 0x3c0a,
    DssLocaleSpanishElSalvador = 0x440a,
    DssLocaleSpanishUruguay = 0x380a,
    DssLocaleSpanishVenezuela = 0x200a,
    DssLocaleSouthernSotho = 0x0430,
    DssLocaleSwahili = 0x0441,
    DssLocaleSwedishSweden = 0x041d,
    DssLocaleSwedishFinland = 0x081d,
    DssLocaleTamil = 0x0449,
    DssLocaleTatar = 0x0444,
    DssLocaleThai = 0x041e,
    DssLocaleTurkish = 0x041f,
    DssLocaleTsonga = 0x0431,
    DssLocaleUkrainian = 0x0422,
    DssLocaleUrdu = 0x0420,
    DssLocaleUzbekCyrillic = 0x0843,
    DssLocaleUzbekLatin = 0x0443,
    DssLocaleVietnamese = 0x042a,
    DssLocaleXhosa = 0x0434,
    DssLocaleYiddish = 0x043d,
    DssLocaleZulu = 0x0435
} EnumDSSLocale;

typedef enum EnumDSSRepositoryType
{
    DssRepositoryTypeReserved =	0,
    DssRepositoryDbContent,
    DssRepositoryFileSystem,
    DssRepositoryHybrid
} EnumDSSRepositoryType;

typedef enum EnumSubscriptionDeliveryType
{
    DssDeliveryTypeReserved = 0,
    DssDeliveryTypeEmail = 0x00000001,
    DssDeliveryTypeFile = 0x00000002,
    DssDeliveryTypePrinter = 0x00000004,
    DssDeliveryTypeCustom = 0x00000008,
    DssDeliveryTypeInbox = 0x00000010,
    DssDeliveryTypeClient = 0x00000020,
    DssDeliveryTypeCache = 0x00000040,
    DssDeliveryTypeMobile = 0x00000080,
    DssDeliveryTypeMobileBlackberry = 0x00000100,
    DssDeliveryTypeMobileIPhone = 0x00000200,
    DssDeliveryTypeMobileIPad = 0x00000400,
    DssDeliveryTypeSnapshot = 0x00001000,
    DssDeliveryTypeLastOne = 0x00002000,
    DssDeliveryTypeCount = 12,
    DssDeliveryTypeAll = 0x00000FFF,
    DssDeliveryTypeAllIncludingSnapshot = 0x00001FFF
} EnumSubscriptionDeliveryType;

typedef enum EnumSubscriptionMobileClientType
{
    DssMobileClientTypeReserved = 0,
    DssMobileClientBlackBerry = 0x00000001,
    DssMobileClientiPhone = 0x00000002,
    DssMobileClientiPad = 0x00000004
} EnumSubscriptionMobileClientType;

typedef enum EnumSubscriptionContactType
{
    DssSubscriptionContactTypeReserved = 0,
    DssSubscriptionContactTypeContact = 0x00000001,
    DssSubscriptionContactTypeContactCollection = 0x00000002,
    DssSubscriptionContactTypeMSTRUser = 0x00000004,
    DssSubscriptionContactTypeMSTRUserGroup = 0x00000008,
    DssSubscriptionContactTypeLDAPUser = 0x00000010,
    DssSubscriptionContactTypeDRL = 0x00000020,
    DssSubscriptionContactTypeCount = 6,
    DssSubscriptionContactTypeAll = 0x0000003F
} EnumSubscriptionContactType;

typedef enum EnumDSSNCSSubInstField
{
    DssNCSSubInstFieldReserved =	0,
    DssNCSSubInstID = 0x00000001,
    DssNCSSubInstName = 0x00000002,
    DssNCSSubInstStatus = 0x00000004,
    DssNCSSubInstCreationTime = 0x00000008,
    DssNCSSubInstParentID = 0x00000010,
    DssNCSSubInstProjectID = 0x00000020,
    DssNCSSubInstDataID = 0x00000040,
    DssNCSSubInstDataType = 0x00000080,
    DssNCSSubInstIsPersonalized = 0x00000100,
    DssNCSSubInstChangeability = 0x00000200,
    DssNCSSubInstOwnerID = 0x00000400,
    DssNCSSubInstRecipientID = 0x00000800,
    DssNCSSubInstTriggerID = 0x00001000,
    DssNCSSubInstDeliveryType = 0x00002000,
    DssNCSSubInstAddressID = 0x00004000,
    DssNCSSubInstNotificationAddressID = 0x00008000,
    DssNCSSubInstVersionID = 0x00010000,
    DssNCSSubInstModificationTime = 0x00020000,
    DssNCSSubInstInitialID = 0x00040000,
    DssNCSSubInstFieldCount = 19,
    DssNCSSubInstSelectCount = 101,
    DssNCSSubInstBrowsingInfo = 0x0007FFFF,
    DssNCSSubInstPersonalizationPrompt = 0x10000000,
    DssNCSSubInstPersonalizationDeltas = 0x20000000,
    DssNCSSubInstPersonalizationData = 0x40000000,
    DssNCSSubInstPersonalizationDelivery = 0x80000000,
    DssNCSSubInstPersonalizationTransmission = 0x01000000,
    DssNCSSubInstPersonalizationAlert = 0x02000000,
    DssNCSSubInstPersonalizationExtraXMLInfo = 0x04000000,
    DssNCSSubInstPersonalizationBurst = 0x08000000,
    DssNCSSubInstPersonalizationAll = 0xFF000000,
    DssNCSSubInstAllTriggerIDs = 0x00100000,
    DssNCSSubInstShowInvalidContact = 0x00200000,
    DssNCSSubInstWithPersonalization = 0xFF07FFFF,
    DssNCSSubInstWithExtraXMLInfoPersonalization = 0x0407FFFF,
    DssNCSSubInstWithTrigger = 0x0017FFFF,
    DssNCSSubInstWithPersonalizationAndTrigger = 0xFF17FFFF
} EnumDSSNCSSubInstField;

typedef enum EnumDSSNCSAddressField
{
    DssNCSAddressFieldReserved =	0,
    DssNCSAddressContactID = 0x00000001,
    DssNCSAddressID = 0x00000002,
    DssNCSAddressName = 0x00000004,
    DssNCSAddressPhysicalAddress = 0x00000008,
    DssNCSAddressDeliveryType = 0x00000010,
    DssNCSAddressIsDefault = 0x00000020,
    DssNCSAddressDeviceID = 0x00000040,
    DssNCSAddressVersionID = 0x00000080,
    DssNCSAddressCreationTime = 0x00000100,
    DssNCSAddressModificationTime = 0x00000200,
    DssNCSAddressFieldCount = 10,
    DssNCSAddressFieldAll = 0x000003FF
} EnumDSSNCSAddressField;

typedef enum EnumDSSNCSContactField
{
    DssNCSContactFieldReserved =	0,
    DssNCSContactID = 0x00000001,
    DssNCSContactLogin = 0x00000002,
    DssNCSContactPassword = 0x00000004,
    DssNCSContactType = 0x00000008,
    DssNCSContactStatus = 0x00000010,
    DssNCSContactDesc = 0x00000020,
    DssNCSContactMSTRUserID = 0x00000040,
    DssNCSContactVersionID = 0x00000080,
    DssNCSContactCreationTime = 0x00000100,
    DssNCSContactModificationTime = 0x00000200,
    DssNCSContactFieldCount = 10,
    DssNCSContactBrowsingInfo =  0x000003FF,
    DssNCSContactAddresses = 0x10000000,
    DssNCSContactMembers = 0x20000000,
    DssNCSContactCollections = 0x40000000,
    DssNCSContactPersonalization = 0x00001000,
    DssNCSContactWithAddresses = 0x100003FF,
    DssNCSContactCollectionWithMembers = 0x200003FF,
    DssNCSContactWithPersonalization = 0x000013FF,
    DssNCSContactWithAddressesAndCollections = 0x500003FF,
    DssNCSContactWithPersonalizationAndAddressesAndCollections = 0x500013FF,
    DssNCSContactFieldAll = 0x700013FF
} EnumDSSNCSContactField;

typedef enum EnumDSSNCSContactSourceField
{
    DssNCSContactDefaultField = 1,
    DssNCSContactDefinitionField = 2,
    DssNCSContactLightWeightField = 3,
    DssNCSContactDefinitionIncludingPersonalization = 4
} EnumDSSNCSContactSourceField;

typedef enum EnumDSSNCSAddressSourceField
{
    DssNCSAddressDefaultField = 1,
    DssNCSAddressLightWeightField = 2
} EnumDSSNCSAddressSourceField;

typedef enum EnumDSSNCSInstanceSourceField
{
    DssNCSInstanceDefaultField = 1,
    DssNCSInstanceDefinitionField = 2,
    DssNCSInstanceDefaultFieldWithDisabledContact = 3,
    DssNCSInstanceDefinitionFieldWithDisabledContact = 4,
    DssNCSInstanceLightWeightField = 5,
    DssNCSInstanceCountField = 6,
    DssNCSInstanceAllInstancesLightWeightField = 7
} EnumDSSNCSInstanceSourceField;

typedef enum EnumDSSNCSSearchType
{
    DSSNCSSearchTypeReserved =	-1,
    DSSNCSSearchSubInstBySubInstInfo = 0,
    DSSNCSSearchSubInstByTrigger = 1,
    DSSNCSSearchContactByContactInfo = 2,
    DSSNCSSearchContactByDeliveryType = 3,
    DSSNCSSearchContactAndCollectionByDeliveryType = 4,
    DSSNCSSearchSubInstAll = 5,
    DSSNCSSearchSubInstBySubInstInfoWithAddress = 6,
    DSSNCSSearchSubInstBySubInstInfoWithAddressAndOwner = 7,
    DSSNCSSearchSubInstByTriggerWithOwner = 8,
    DSSNCSSearchSubInstBySubInstInfoWithMSTRUserID = 9
} EnumDSSNCSSearchType;

typedef enum EnumDSSNCSOrderByContactField
{
    DssNCSOrderByContactFieldReserved =	0,
    DssNCSOrderByContactLoginASC = 1,
    DssNCSOrderByContactLoginDESC = 2,
    DssNCSOrderByContactTypeASC = 3,
    DssNCSOrderByContactTypeDESC = 4,
    DssNCSOrderByContactStatusASC = 5,
    DssNCSOrderByContactStatusDESC = 6,
    DssNCSOrderByContactDescASC = 7,
    DssNCSOrderByContactDescDESC = 8
} EnumDSSNCSOrderByContactField;

typedef enum EnumDSSNCSEmailDestinationType
{
    DssNCSEmailDestinationTypeReserved = 0,
    DssNCSEmailDestinationTypeTo = 1,
    DssNCSEmailDestinationTypeCC = 2,
    DssNCSEmailDestinationTypeBCC = 4
} EnumDSSNCSEmailDestinationType;

typedef enum EnumDSSNCSSubscriptionType
{
    DssNCSSubscriptionSingle = 1,
    DssNCSSubscriptionMultipleParent = 2,
    DssNCSSubscriptionMultipleChild = 3
} EnumDSSNCSSubscriptionType;

typedef enum EnumDSSNCSSubscriptionValidationType
{
    DssNCSSubscriptionInvalid = 0,
    DssNCSSubscriptionValid = 1
} EnumDSSNCSSubscriptionValidationType;

typedef enum EnumDSSNCSContactStatusType
{
    DssNCSContactStatusReserved = -1,
    DssNCSContactDisabled = 0,
    DssNCSContactEnabled = 1
} EnumDSSNCSContactStatusType;

typedef enum EnumDSSNCSAddressIsDefaultType
{
    DssNCSAddressDefault = -1,
    DssNCSAddressIsNotDefault = 0
} EnumDSSNCSAddressIsDefaultType;

typedef enum EnumDSSNCSMiscObjectFlags
{
    DssNCSMiscObjectFlagsReserved =	0,
    DssNCSOverwriteObjectIfExists = 0x00000001,
    DssNCSAutoInvalidateObject = 0x00000002,
    DssNCSDuplicatePreserveVersion = 0x00000004,
    DssNCSSaveInstancesWithSendNow = 0x00000008,
    DssNCSSaveContactsWithPersonalization = 0x00000010,
    DssNCSDoNotSaveDRLContact = 0x00000020,
    DssNCSDoNotUpdateInstance = 0x00000040
} EnumDSSNCSMiscObjectFlags;

typedef struct StructDSSObjectReference
{
    DSS_ID mID;
    EnumDSSObjectType mType;
    DSS_ID mProjectID;
    DSS_ID mContainerID;
    EnumDSSObjectType mContainerType;
    EnumDSSReferenceQualities mQualities;
} StructDSSObjectReference;

typedef enum EnumDSSRWTreeManipulation
{
    DssRWTreeManipulationReserved = 0,
    DssRWTreeManipulationAddNew,
    DssRWTreeManipulationPushOne,
    DssRWTreeManipulationPushAll,
    DssRWTreeManipulationSubTree
} EnumDSSRWTreeManipulation;

typedef enum EnumDSSLocaleFor
{
    DssLocaleForDateNumber,
    DssLocaleForMessage,
    DssLocaleForMD,
    DssLocaleForData,
    DssLocaleForInterface,
    DssLocaleForLastOne
} EnumDSSLocaleFor;

typedef enum EnumDSSPromptAnswerReuse
{
    DssPromptAnswerReuseReserved = 0,
    DssPromptAnswerReuseList,
    DssPromptAnswerReuseDefault,
    DssPromptAnswerReuseDefaultAndClose
} EnumDSSPromptAnswerReuse;

typedef enum EnumDSSSQLOperationType
{
    DssSQLOperationTypeReserved = 0x00000000,
    DssSQLOperationTypeSelect = 0x00000001,
    DssSQLOperationTypeSet = 0x00000002,
    DssSQLOperationTypeDelete = 0x00000003,
    DssSQLOperationTypeSimpleInsert = 0x00000004,
    DssSQLOperationTypeParameterizedInsert = 0x00000005,
    DssSQLOperationTypeBatchQuery = 0x00000006,
    DssSQLOperationTypeUpdate = 0x00000007
} EnumDSSSQLOperationType;

typedef enum EnumDSSSQLSetOperator
{
    DssSQLSetOperatorReserved = 0x00000000,
    DssSQLSetOperatorUnion = 0x00000001,
    DssSQLSetOperatorUnionAll = 0x00000002,
    DssSQLSetOperatorIntersect = 0x00000003,
    DssSQLSetOperatorMinus = 0x00000004
} EnumDSSSQLSetOperator;

typedef enum EnumDSSSQLJoinType
{
    DssSQLJoinTypeReserved = -1,
    DssSQLJoinTypeInner = 0,
    DssSQLJoinTypeLeftOuter = 1,
    DssSQLJoinTypeRightOuter = 2,
    DssSQLJoinTypeFullOuter = 3,
    DssSQLJoinTypeCross = 4
} EnumDSSSQLJoinType;

typedef enum EnumDSSCubesFlags
{
    DssCubesDefault = 0x00000000
} EnumDSSCubesFlags;

typedef enum EnumDSSCubeObjectsFlags
{
    DssCubeObjectsDefault = 0x00000000
} EnumDSSCubeObjectsFlags;

typedef enum EnumDSSSourceFeatures
{
    DssSourceFeatureSingleHierarchyPerDimensionOnTemplate = 0x00000001
} EnumDSSSourceFeatures;

typedef enum EnumDSSSQLAutoJoinFlags
{
    DssSQLAutoJoinReserved = 0x00000000,
    DssSQLAutoJoinUsePkFk = 0x00000001,
    DssSQLAutoJoinUseNameAndType = 0x00000002,
    DssSQLAutoJoinUsePkFkIndirect = 0x00000004
} EnumDSSSQLAutoJoinFlags;

typedef enum EnumDSSEngineSchemaLoadingAction
{
    DssEngineSchemaReserved = 0,
    DssEngineSchemaLoad = 1,
    DssEngineSchemaUnload = 2,
    DssEngineSchemaReload = 3
} EnumDSSEngineSchemaLoadingAction;

typedef enum EnumDSSRWTemplateNodePopulateOptions
{
    DssRWTemplateNodePopulateReserved = 0,
    DssRWTemplateNodePopulateWithFormat,
    DssRWTemplateNodePopulateWithoutFormat,
    DssRWTemplateNodePopulateLinkToDataset
} EnumDSSRWTemplateNodePopulateOptions;

typedef enum EnumDSSDocumentManipulationOption
{
    DssDocumentManipulationDefault = 0,
    DssDocumentManipulationDonotDuplicate = 1
} EnumDSSDocumentManipulationOption;

typedef enum EnumDSSDataRetriever
{
    DssDataRetrieverDefault = 0,
    DssDataRetrieverXML = 1
} EnumDSSDataRetriever;

typedef enum EnumDSSImportControl
{
    DssImportControlReserved = 0,
    DssImportControlYes = 1,
    DssImportControlNo = 2,
    DssImportControlDefault = 3
} EnumDSSImportControl;

typedef enum EnumDSSImportControlType
{
    DssImportControlTypeReserved = 0,
    DssImportControlTypeOverwriteName = 1,
    DssImportControlTypeKeepDefaultPromptAnswer = 2,
    DssImportControlTypeKeepPromptOptionality = 3,
    DssImportControlTypeGetFlatHierarchyLevel00 = 4,
    DssImportControlTypeGetFlatHierarchyLevel01Suffix = 5,
    DssImportControlTypeGetSourceOrder = 6,
    DssImportControlTypeGetSourceOrderForDimension = 6,
    DssImportControlTypeGetSourceOrderForHierarchy = 7,
    DssImportControlTypeGetMeasuresInDimension = 8,
    DssImportControlTypeGetMeasuresInDimensionDefault = 9,
    DssImportControlTypeReservedLastOne
} EnumDSSImportControlType;

typedef enum EnumDSSTemplateSearchFlags
{
    DssTemplateSearchDefault = 0,
    DssTemplateSearchIgnoreUnitsUnderTemplateDimensions = 0x01,
    DssTemplateSearchDisabledUnits = 0x02,
    DssTemplateSearchDisabledMetrics = 0x04,
    DssTemplateSearchDisabledPromptUnits = 0x08,
    DssTemplateSearchDEonDT = 0x10,
    DssTemplateSearchUnderlyingPrompts = 0x20
} EnumDSSTemplateSearchFlags;

typedef enum EnumDSSDefaultCellFormatType
{
    DSSDefaultCellFormatTypeReserved = 0,
    DSSDefaultCellFormatTypeGeneral = 1,
    DSSDefaultCellFormatTypeMix = 2,
    DSSDefaultCellFormatTypeLastOne
} EnumDSSDefaultCellFormatType;

typedef enum EnumDSSNCSFilterField
{
    DssNCSFilterFieldReserved = 0,
    DssNCSFilterFieldContactType = 1,
    DssNCSFilterFieldContactName = 2,
    DssNCSFilterFieldContactDescription = 3,
    DssNCSFilterFieldContactStatus = 4,
    DssNCSFilterFieldAddressName = 5,
    DssNCSFilterFieldPhysicalAddress = 6,
    DssNCSFilterFieldAddressType = 7,
    DssNCSFilterFieldContactCreationDate = 8,
    DssNCSFilterFieldContactModificationDate = 9,
    DssNCSFilterFieldContactID = 10,
    DssNCSFilterFieldMSTRUserID = 11,
    DssNCSFilterFieldInstanceDeliveryType = 12,
    DssNCSFilterFieldInstanceCreationTime = 13,
    DssNCSFilterFieldInstanceOwnerID = 14,
    DssNCSFilterFieldInstanceParentID = 15,
    DssNCSFilterFieldInstanceRecipientID = 16,
    DssNCSFilterFieldInstanceRecipientName = 17,
    DssNCSFilterFieldInstanceReportOrDocumentName = 18,
    DssNCSFilterFieldInstanceProjectName = 19,
    DssNCSFilterFieldInstanceID = 20,
    DssNCSFilterFieldInstanceName = 21,
    DssNCSFilterFieldMySubscriptions = 22,
    DssNCSFilterFieldInstanceOwnerName = 23,
    DssNCSFilterFieldLastOne
} EnumDSSNCSFilterField;

typedef enum EnumDSSNCSFilterType
{
    DssNCSFilterTypeReserved = 0,
    DssNCSFilterTypeExactly = 1,
    DssNCSFilterTypeStartingWith = 2,
    DssNCSFilterTypeContains = 3,
    DssNCSFilterTypeGreaterEqual = 4,
    DssNCSFilterTypeLessEqual = 5,
    DssNCSFilterTypeNotEqual = 6,
    DssNCSFilterTypeIn = 7,
    DssNCSFilterTypeLastOne
} EnumDSSNCSFilterType;

typedef enum EnumDSSNCSAllowChangeMaskType
{
    DssNCSAllowChangeNone = 0x00000000,
    DssNCSAllowChangeDelivery = 0x00000001,
    DssNCSAllowChangePersonalization = 0x00000002,
    DssNCSAllowUnsubscribe = 0x00000004
} EnumDSSNCSAllowChangeMaskType;

typedef enum EnumDSSContentServiceType
{
    DssContentServiceTypeReserved =	0,
    DssContentServiceTypeHL,
    DssContentServiceTypeNCS,
    DssContentServiceTypeLast,
    DssContentServiceTypeCount = 2
} EnumDSSContentServiceType;

typedef enum EnumDSSContentServiceStatus
{
    DssContentServiceStatusReserved =	-1,
    DssContentServiceStatusOffline = 0,
    DssContentServiceStatusOnline,
    DssContentServiceStatusLast
} EnumDSSContentServiceStatus;

typedef enum EnumDSSMultilingualStringQualities
{
    DssMultilingualStringNoQualities = 0,
    DssMultilingualStringInDefinition = 0x00000001,
    DssMultilingualStringInBrowser = 0x00000002,
    DssMultilingualStringInComments = 0x00000008,
    DssMultilingualStringInAllParts = 0x0000007f,
    DssMultilingualStringInContainerObject = 0x00000100,
    DssMultilingualStringInEmbeddedObjects = 0x00000200,
    DssMultilingualStringAllContainer = 0x00000300,
    DssMultilingualStringInReferencedManagedFirstClassObjects = 0x00000400,
    DssMultilingualStringAllContainerAndReferencedManagedFirstClassObjects = 0x00000700,
    DssMultilingualStringInLoadedParts = 0x00000800
} EnumDSSMultilingualStringQualities;

typedef enum EnumDSSMultilingualStringAspects
{
    DssMultilingualStringChecksBitmask = 0x000000ff,
    DssMultilingualStringValidateLength = 0x00000100,
    DssMultilingualStringValidateCharacters = 0x00000200,
    DssMultilingualStringTrimWhitespace = 0x00000400,
    DssMultilingualStringExcludeFromIteration = 0x00000800,
    DssMultilingualString = DssSourceDefn,
    DssMultilingualName = DssSourceBrowser  | DssMultilingualStringValidateLength | DssMultilingualStringValidateCharacters | DssMultilingualStringTrimWhitespace,
    DssMultilingualDescription = DssSourceBrowser  | DssMultilingualStringValidateLength | DssMultilingualStringTrimWhitespace,
    DssMultilingualComment = DssSourceComments  | DssMultilingualStringTrimWhitespace
} EnumDSSMultilingualStringAspects;

typedef enum EnumDSSDrillPathFlags
{
    DRILL_PATH_4TIER_STYLE = 1,
    DRILL_PATH_PERSONALIZE = 2,
    DRILL_PATH_NO_INSTANCE = 4,
    DRILL_PATH_WITHIN_CUBE = 8,
    DRILL_PATH_FORCE_WITHIN = 16,
    DRILL_PATH_FORCE_RETURN_NO_PATH = 32,
    DRILL_PATH_USE_CACHED_PATHS = 64,
    DRILL_PATH_NO_METRIC_PATH = 128,
    DRILL_PATH_DEFAULT = DRILL_PATH_PERSONALIZE,
    DRILL_PATH_XML = DRILL_PATH_4TIER_STYLE  | DRILL_PATH_NO_INSTANCE,
    DRILL_PATH_PERSONALIZE_XML = DRILL_PATH_XML  | DRILL_PATH_PERSONALIZE,
    DRILL_PATH_GRAPH = DRILL_PATH_4TIER_STYLE
} EnumDSSDrillPathFlags;

typedef enum EnumDSSWorkingSetFlags
{
    DDssWorkingSetReserved = 0,
    DssWorkingSetDefault = 1,
    DssWorkingSetRWD = 2
} EnumDSSWorkingSetFlags;

typedef enum EnumDSSNCSSubInstRunningState
{
    DssNCSSubInstRunningStateNormal = 0x0000,
    DssNCSSubInstRunningStateAlertSecondPass = 0x0001,
    DssNCSSubInstRunningStateToConvertToDoc = 0x0002,
    DssNCSSubInstRunningStateSegmented = 0x0004,
    DssNCSSubInstRunningStateMobileReportFailure = 0x0008
} EnumDSSNCSSubInstRunningState;

typedef enum EnumDSSNCSZippingCompressionType
{
    DssNCSZippingCompressionTypeZip = 0,
    DssNCSZippingCompressionTypeGZ = 1
} EnumDSSNCSZippingCompressionType;

typedef enum EnumDSSDerivedElementSaveAs
{
    DssDerivedElementSaveAsDefault = 0x00,
    DssDerivedElementSaveAsClearDomain = 0x01
} EnumDSSDerivedElementSaveAs;

typedef enum EnumDSSViewEvaluateFlags
{
    DssViewEvaluateDefault = 0x00000000,
    DssViewEvaluateNoEval = 0x00000001,
    DssViewEvaluateSource = 0x00000002,
    DssViewEvaluateDetail = 0x00000004,
    DssViewEvaluateNoJoin = 0x00000008,
    DssViewEvaluateNoDerivedElement = 0x00000010,
    DssViewEvaluateXTab = 0x00000020,
    DssViewEvaluateNoDisplay = 0x00000040,
    DssViewEvaluateNoViewPopulation = 0x00000080,
    DssViewEvaluateNoViewCalculation = 0x00000100,
    DssViewEvaluateNoViewXTab = 0x00000200,
    DssViewEvaluateTransactionEnabled = 0x00000400
} EnumDSSViewEvaluateFlags;

typedef enum EnumDSSPurgeStatisticsExFlags
{
    DssPurgeStatisticsExServerSpecific = 0x00000001,
    DssPurgeStatisticsExAllBasicTables = 0x00000002,
    DssPurgeStatisticsExAllDetailTables = 0x00000004,
    DssPurgeStatisticsExServerTables = 0x00000008,
    DssPurgeStatisticsExServerLevel = 0x00000010
} EnumDSSPurgeStatisticsExFlags;

//typedef enum EnumDSSGraphType
//{
//    DssGraphTypeReserved = 0,
//    DssGraphTypeVertical_Area_Absolute = 1,
//    DssGraphTypeVertical_Area_Stacked = 2,
//    DssGraphTypeVertical_Area_BiPolar_Absolute = 3,
//    DssGraphTypeVertical_Area_BiPolar_Stacked = 4,
//    DssGraphTypeVertical_Area_DualAxis_Absolute = 5,
//    DssGraphTypeVertical_Area_DualAxis_Stacked = 6,
//    DssGraphTypeVertical_Area_Percent = 7,
//    DssGraphTypeHorizontal_Area_Absolute = 8,
//    DssGraphTypeHorizontal_Area_Stacked = 9,
//    DssGraphTypeHorizontal_Area_BiPolar_Absolute = 10,
//    DssGraphTypeHorizontal_Area_BiPolar_Stacked = 11,
//    DssGraphTypeHorizontal_Area_DualAxis_Absolute = 12,
//    DssGraphTypeHorizontal_Area_DualAxis_Stacked = 13,
//    DssGraphTypeHorizontal_Area_Percent = 14,
//    DssGraphTypeVertical_Bar_Side_by_Side = 15,
//    DssGraphTypeVertical_Bar_Stacked = 16,
//    DssGraphTypeVertical_Bar_DualAxis_Side_by_Side = 17,
//    DssGraphTypeVertical_Bar_DualAxis_Stacked = 18,
//    DssGraphTypeVertical_Bar_BiPolar_Side_by_Side = 19,
//    DssGraphTypeVertical_Bar_BiPolar_Stacked = 20,
//    DssGraphTypeVertical_Bar_Percent = 21,
//    DssGraphTypeHorizontal_Bar_Side_by_Side = 22,
//    DssGraphTypeHorizontal_Bar_Stacked = 23,
//    DssGraphTypeHorizontal_Bar_DualAxis_Side_by_Side = 24,
//    DssGraphTypeHorizontal_Bar_DualAxis_Stacked = 25,
//    DssGraphTypeHorizontal_Bar_BiPolar_Side_by_Side = 26,
//    DssGraphTypeHorizontal_Bar_BiPolar_Stacked = 27,
//    DssGraphTypeHorizontal_Bar_Percent = 28,
//    DssGraphTypeVertical_Line_Absolute = 29,
//    DssGraphTypeVertical_Line_Stacked = 30,
//    DssGraphTypeVertical_Line_BiPolar_Absolute = 31,
//    DssGraphTypeVertical_Line_BiPolar_Stacked = 32,
//    DssGraphTypeVertical_Line_DualAxis_Absolute = 33,
//    DssGraphTypeVertical_Line_DualAxis_Stacked = 34,
//    DssGraphTypeVertical_Line_Percent = 35,
//    DssGraphTypeHorizontal_Line_Absolute = 36,
//    DssGraphTypeHorizontal_Line_Stacked = 37,
//    DssGraphTypeHorizontal_Line_BiPolar_Absolute = 38,
//    DssGraphTypeHorizontal_Line_BiPolar_Stacked = 39,
//    DssGraphTypeHorizontal_Line_DualAxis_Absolute = 40,
//    DssGraphTypeHorizontal_Line_DualAxis_Stacked = 41,
//    DssGraphTypeHorizontal_Line_Percent = 42,
//    DssGraphTypePies = 43,
//    DssGraphTypeRing_Pie = 44,
//    DssGraphTypeMultiple_Pies = 45,
//    DssGraphTypeMultiple_Ring_Pies = 46,
//    DssGraphTypeMultiple_Proportional = 47,
//    DssGraphTypeMultiple_Proportional_Ring_Pies = 48,
//    DssGraphTypePie_Bar = 49,
//    DssGraphTypeRing_Pie_Bar = 50,
//    DssGraphTypeX_Y_Scatter = 51,
//    DssGraphTypeDualAxis_X_Y_Scatter = 52,
//    DssGraphTypeX_Y_Scatter_with_Labels = 53,
//    DssGraphTypeDualAxis_X_Y_Scatter_with_Labels = 54,
//    DssGraphTypePolar_Coordinate_Chart = 55,
//    DssGraphTypeDualAxis_Polar_Coordinate_Chart = 56,
//    DssGraphTypeRadar_Line = 57,
//    DssGraphTypeRadar_Stacked_Area = 58,
//    DssGraphTypeDualAxis_Radar_Line = 59,
//    DssGraphTypeDualAxis_Radar_Stacked_Area = 60,
//    DssGraphTypeBubble = 61,
//    DssGraphTypeBubble_DualAxis = 62,
//    DssGraphTypeGantt_Chart = 63,
//    DssGraphTypeHiLo_Stock_Chart = 64,
//    DssGraphTypeHiLo_DualAxis_Stock_Chart = 65,
//    DssGraphTypeHiLoOpen_Stock_Chart = 66,
//    DssGraphTypeHiLoOpen_DualAxis_Stock_Chart = 67,
//    DssGraphTypeHiLoOpenClose_Stock_Chart = 68,
//    DssGraphTypeHiLoOpenClose_DualAxis_Stock_Chart = 69,
//    DssGraphTypeSpectral_Chart = 70,
//    DssGraphTypeVertical_Histogram = 71,
//    DssGraphTypeHorizontal_Histogram = 72,
//    DssGraphTypeTable_Chart = 73,
//    DssGraphTypeD3_Riser_Bars = 74,
//    DssGraphTypeD3_Riser_Pyramids = 75,
//    DssGraphTypeD3_Riser_Octogans = 76,
//    DssGraphTypeD3_Riser_Cut_Corner_Bars = 77,
//    DssGraphTypeD3_Floating_Cubes = 78,
//    DssGraphTypeD3_Floating_Spheres = 79,
//    DssGraphTypeD3_Connect_Group_Areas = 80,
//    DssGraphTypeD3_Connect_Group_Ribbons = 81,
//    DssGraphTypeD3_Connect_Group_Steps = 82,
//    DssGraphTypeD3_Connect_Series_Areas = 83,
//    DssGraphTypeD3_Connect_Series_Ribbons = 84,
//    DssGraphTypeD3_Connect_Series_Steps = 85,
//    DssGraphTypeD3_Surface = 86,
//    DssGraphTypeD3_Surface_with_Sides = 87,
//    DssGraphTypeD3_Honeycomb_Surface = 88,
//    DssGraphTypeD3_X_Y_Z_Scatter_Chart = 89,
//    DssGraphTypeD3_X_Y_Z_Scatter_with_Labels_Chart = 90,
//    DssGraphTypeVertical_Bar_Absolute = 91,
//    DssGraphTypeHorizontal_Bar_Absolute = 92,
//    DssGraphTypeBubble_With_Labels = 93,
//    DssGraphTypeBubble_DualAxis_With_Labels = 94,
//    DssGraphTypeGauge = 95,
//    DssGraphTypeFunnel = 96,
//    DssGraphTypePareto = 97,
//    DssGraphTypePareto_Percent = 98,
//    DssGraphTypeBoxPlot = 99,
//    DssGraphTypeRadar_Area_Stacked = 100,
//    DssGraphTypeRadar_Absolute_Area = 101,
//    DssGraphTypeVertical_Bar_BiPolar_Absolute = 102,
//    DssGraphTypeVertical_Bar_DualAxis_Absolute = 103,
//    DssGraphTypeHorizontal_Bar_BiPolar_Absolute = 104,
//    DssGraphTypeHorizontal_Bar_DualAxis_Absolute = 105,
//    DssGraphRadar_Line_Stacked = 106,
//    DssGraphRadar_DualAxis_Radar_Stacked_Line = 107,
//    DssGraphRadar_DualAxis_Radar_Area = 108,
//    DssGraphTypeMaxDeskTopGraphTypes = 109,
//    DssGraphTypeBarArea = 201,
//    DssGraphTypeBarLine = 202,
//    DssGraphTypeAreaLine = 203,
//    DssGraphTypeDualAxisBarArea = 204,
//    DssGraphTypeDualAxisBarLine = 205,
//    DssGraphTypeDualAxisAreaLine = 206
//} EnumDSSGraphType;

typedef enum EnumDSSExtendedType
{
    DssExtendedTypeReserved = 0x00000000,
    DssExtendedTypeRelational = 1,
    DssExtendedTypeMDX = 2,
    DssExtendedTypeCustomSQLFreeForm = 3,
    DssExtendedTypeCustomSQLWizard = 4,
    DssExtendedTypeDataImport = 0x00000100,
    DssExtendedTypeDataImportFileExcel = 0x00000010  | DssExtendedTypeDataImport,
    DssExtendedTypeDataImportFileText = 0x00000020  | DssExtendedTypeDataImport,
    DssExtendedTypeDataImportCustomSQL = 0x00000030  | DssExtendedTypeDataImport,
    DssExtendedTypeDataImportTable = 0x00000040  | DssExtendedTypeDataImport,
    DssExtendedTypeReservedLastOne
} EnumDSSExtendedType;

typedef enum EnumDSSTransactionType
{
    DssTransactionReserved = 0,
    DssTransactionEnableChangeJournal,
    DssTransactionDisableChangeJournal,
    DssTransactionPurgeChangeJournal,
    DssTransactionSaveObject,
    DssTransactionSaveObjects,
    DssTransactionSaveLinkItems,
    DssTransactionDeleteObject,
    DssTransactionDeleteObjects,
    DssTransactionDeleteProject,
    DssTransactionCopyObject,
    DssTransactionMergeUser,
    DssTransactionCollectGarbage,
    DssTransactionWriteSystemProperty,
    DssTransactionWriteDatabasePassword,
    DssTransactionReservedLastOne
} EnumDSSTransactionType;

typedef enum EnumDSSChangeType
{
    DssChangeReserved = 0,
    DssChangeCreateObject,
    DssChangeChangeObject,
    DssChangeDeleteObject,
    DssChangeTouchFolder,
    DssChangeCreateLinkItem,
    DssChangeChangeLinkItem,
    DssChangeDeleteLinkItem,
    DssChangeChangeJournalEnable,
    DssChangeChangeJournalDisable,
    DssChangeChangeJournalPurge,
    DssChangeReservedLastOne
} EnumDSSChangeType;

typedef enum EnumDSSStringParameterType
{
    DssStringParameterReserved = 0,
    DssStringParameterConstant,
    DssStringParameterPrompt,
    DssStringParameterObject,
    DssStringParameterDate
} EnumDSSStringParameterType;

typedef enum EnumDSSQueryLiteFuncType
{
    DssQueryLiteFuncTypeReserved = 0,
    DssQueryLiteFuncTypeEquals = 1,
    DssQueryLiteFuncTypeNotEqual,
    DssQueryLiteFuncTypeGreater,
    DssQueryLiteFuncTypeGreaterEqual,
    DssQueryLiteFuncTypeLess,
    DssQueryLiteFuncTypeLessEqual,
    DssQueryLiteFuncTypeIn,
    DssQueryLiteFuncTypeNotIn,
    DssQueryLiteFuncTypeIsNull,
    DssQueryLiteFuncTypeIsNotNull,
    DssQueryLiteFuncTypeLike,
    DssQueryLiteFuncTypeContains,
    DssQueryLiteFuncTypeBeginsWith,
    DssQueryLiteFuncTypeEndsWith,
    DssQueryLiteFuncTypeNotContains,
    DssQueryLiteFuncTypeNotBeginsWith,
    DssQueryLiteFuncTypeNotEndsWith,
    DssQueryLiteFuncTypeNotLike,
    DssQueryLiteFuncTypeCount,
    DssQueryLiteFuncTypeSum,
    DssQueryLiteFuncTypeAnd,
    DssQueryLiteFuncTypeHasBit
} EnumDSSQueryLiteFuncType;

//typedef enum EnumDSSViewMedia
//{
//    DssViewMediaAvailableViewStatic = 0x00000001,
//    DssViewMediaAvailableViewInteractive = 0x00000002,
//    DssViewMediaAvailableViewEditable = 0x00000004,
//    DssViewMediaAvailableViewFlash = 0x00000008,
//    DssViewMediaAvailableExportExcel = 0x00000010,
//    DssViewMediaAvailableExportPDF = 0x00000020,
//    DssViewMediaAvailableExportHTML = 0x00000040,
//    DssViewMediaAvailableExportFlash = 0x00000080,
//    DssViewMediaAvailableExportExcelPlainText = 0x00000100,
//    DssViewMediaAvailableExportCSV = 0x00000200,
//    DssViewMediaAvailableExportPlainText = 0x00000400,
//    DssViewMediaAvailableViewAnalysis = 0x00000800,
//    DssViewMediaAvailableMediaMask = 0x07ffffff,
//    DssViewMediaReserved = 0,
//    DssViewMediaViewStatic = 0x08000000,
//    DssViewMediaViewInteractive = 0x10000000,
//    DssViewMediaViewEditable = 0x18000000,
//    DssViewMediaViewFlash = 0x20000000,
//    DssViewMediaExportExcel = 0x28000000,
//    DssViewMediaExportPDF = 0x30000000,
//    DssViewMediaExportHTML = 0x38000000,
//    DssViewMediaExportFlash = 0x40000000,
//    DssViewMediaExportExcelPlainText = 0x48000000,
//    DssViewMediaExportCSV = 0x50000000,
//    DssViewMediaExportPlainText = 0x58000000,
//    DssViewMediaViewAnalysis = 0x60000000,
//    DssViewMediaDefaultMask = 0xf8000000
//} EnumDSSViewMedia;

typedef enum EnumDSSChangeJournalListXMLFlags
{
    DssChangeJournalListXMLDefault = 0
} EnumDSSChangeJournalListXMLFlags;

typedef enum EnumDSSChangeJournalSearchFlags
{
    DssChangeJournalSearchDefault = 0,
    DssChangeJournalSearchNotTransactionTypes = 1,
    DssChangeJournalSearchNotTransactionSources = 2,
    DssChangeJournalSearchUseObjectFiltering = 4,
    DssChangeJournalSearchUseChangeTypeFiltering = 8,
    DssChangeJournalSearchShowSystemUser = 16
} EnumDSSChangeJournalSearchFlags;

typedef enum EnumDSSNCSCleanUpNCSRepository
{
    DssNCSCleanUpNCSRepositoryInvalidSubscriptions = 0
} EnumDSSNCSCleanUpNCSRepository;

typedef enum EnumDSSNCSFindDependentReturnFlags
{
    DssNCSFindDependentReturnContact = 0x00000001,
    DssNCSFindDependentReturnAddress = 0x00000002,
    DssNCSFindDependentReturnSubscription = 0x00000004,
    DssNCSFindDependentReturnDevice = 0x00000008,
    DssNCSFindDependentReturnDefault = 0x0000000F
} EnumDSSNCSFindDependentReturnFlags;

typedef enum EnumDSSNCSFindDependentSearchFlags
{
    DssNCSFindDependentSearchExpandGroup = 0x00000001,
    DssNCSFindDependentSearchExpandDefaultAddress = 0x00000002,
    DssNCSFindDependentSearchOwner = 0x00000004,
    DssNCSFindDependentSearchRecipient = 0x00000008,
    DssNCSFindDependentSearchDefault = 0x0000000F
} EnumDSSNCSFindDependentSearchFlags;

typedef enum EnumDSSIncrementalFetchType
{
    DssIncrementalFetchReserved = 0,
    DssIncrementalFetchGroupByUnit = 1,
    DssIncrementalFetchDetails = 2,
    DssIncrementalFetchGraphs = 3,
    DssIncrementalFetchGrids = 4
} EnumDSSIncrementalFetchType;

typedef enum EnumUpdateTransactionDataType
{
    DssTransactionUpdateReserved = 0,
    DssTransactionUpdateAttributeFormOnTemplateNode = 1,
    DssTransactionUpdateMetricOnTemplateNode = 2,
    DssTransactionUpdateField = 3,
    DssTransactionMarkOnTemplateNode = 4,
    DssTransactionUnMarkOnTemplateNode = 5
} EnumUpdateTransactionDataType;

typedef enum EnumDSSPromptAnswersType
{
    DssPromptAnswersReserved = 0,
    DssPromptAnswersUser,
    DssPromptAnswersDiagnostic
} EnumDSSPromptAnswersType;

typedef enum EnumDSSDataMethod
{
    DssDataMethodReserved = 0,
    DssDataMethodNone,
    DssDataMethodBySQL,
    DssDataMethodByConnection
} EnumDSSDataMethod;

typedef enum EnumDSSOLAPNullPosition
{
    DssOLAPNullPositionLast = 0,
    DssOLAPNullPositionFirst,
    DssOLAPNullPositionSmallest,
    DssOLAPNullPositionLargest
} EnumDSSOLAPNullPosition;

typedef enum EnumFlexModelType
{
    DssFlexModelReserved = 0,
    DssFlexModelXTab = 1,
    DssFlexModelControl = 2,
    DssFlexModelFieldGroup = 3,
    DssFlexModelNormalGroupBy = 4,
    DssFlexModelCellFormat = 5,
    DssFlexModelLayout = 6
} EnumFlexModelType;

typedef enum EnumDSSBaseFormFlags
{
    DssBaseFormFlagNone =	0x00000000,
    DssBaseFormFlagExternalKeyForm =	0x00000001
} EnumDSSBaseFormFlags;

typedef enum EnumDSSImportDocumentOptions
{
    DssImportDocumentDefault = 0x00,
    DssImportDocumentAutoLayoutName = 0x01
} EnumDSSImportDocumentOptions;

typedef enum EnumDSSStatDisplayField
{
    DssStatDisplayExecStartTime = 0x00000000,
    DssStatDisplayExecFinishTime = 0x00000001,
    DssStatDisplayTotalDuration = 0x00000002,
    DssStatDisplaySQLDuration = 0x00000003,
    DssStatDisplayAnaDuration = 0x00000004,
    DssStatDisplayOtherDuration = 0x00000005,
    DssStatDisplayRowsReturned = 0x00000006,
    DssStatDisplayColumnsReturned = 0x00000007,
    DssStatDisplayCntTempTables = 0x00000008,
    DssStatDisplayCntPasses = 0x00000009,
    DssStatDisplayCntSQLPasses = 0x0000000a,
    DssStatDisplayCntAnaPasses = 0x0000000b,
    DssStatDisplayDBUserID = 0x0000000c,
    DssStatDisplayDBInstance = 0x0000000d,
    DssStatDisplayTableAccessed = 0x0000000e,
    DssStatDisplayDBErrorMsg = 0x0000000f,
    DssStatDisplaySQLDetails = 0x00000010,
    DssStatDisplaySQLDetailsBase = 0x00000011,
    DssStatDisplaySQLViewForDS = 0x00000012
} EnumDSSStatDisplayField;

typedef enum EnumDSSTriState
{
    DssTriStateUnset = -1,
    DssTriStateFalse = 0,
    DssTriStateTrue = 1
} EnumDSSTriState;

//typedef enum EnumBinaryContextFlags
//{
//    DssBinaryContextFlexDefn = 0x01,
//    DssBinaryContextFlexData = 0x02,
//    DssBinaryContextFlexSourceInfo = 0x08,
//    DssBinaryContextFlexSheetsInfo = 0x10,
//    DssBinaryContextFlagsRepublishBehavior = 0x20,
//    DssBinaryContextFlexDITransformationInfo = 0x40,
//    DssBinaryContextFlexCrossTabFlatPreview = 0x80,
//    DssBinaryContextFlexDrill = 0x04,
//    DssBinaryContextFlexImage = 0x08,
//    DssBinaryContextFlexFlashExport = 0x100,
//    DssBinaryContextFlexFlashExportPdf = 0x200,
//    DssBinaryContextFlexOnDemand = 0x01000000,
//    DssBinaryContextFlexOnDemandDataOnly = 0x02000000,
//    DssBinaryContextFlexSeparableBinary = 0x04000000,
//    DssBinaryContextFlexServiceAnyState = 0x10000000,
//    DssBinaryContextDataRelatedBitMask = DssBinaryContextFlexData  | DssBinaryContextFlexOnDemand | DssBinaryContextFlexOnDemandDataOnly
//} EnumBinaryContextFlags;

typedef enum EnumDSSDataPreviewJSONFlags
{
    DssDataPreviewJSONMapping = 0x01,
    DssDataPreviewJSONData = 0x02,
    DssDataPreviewJSONSourceInfo = 0x08,
    DssDataPreviewJSONSheetsInfo = 0x10,
    DssDataPreviewJSONDITransfsInfo = 0x40
} EnumDSSDataPreviewJSONFlags;

//typedef enum EnumUnitTransactionFlags
//{
//    DssUnitTransactionReserved = 0x00,
//    DssUnitTransactionEditable = 0x01,
//    DssUnitTransactionRequired = 0x10000000
//} EnumUnitTransactionFlags;
//
//typedef enum EnumBinaryContextDetailFlags
//{
//    DssBinaryContextDetailCurrentLayout = 0x01,
//    DssBinaryContextDetailAnnotation = 0x02,
//    DssBinaryContextDetailVisFramework = 0x04,
//    DssBinaryContextDetailCurrentGroupBy = 0x08,
//    DssBinaryContextDetailPartialRetrieval = 0x10,
//    DssBinaryContextDetailSlice = 0x20,
//    DssBinaryContextDetailPPE = 0x40,
//    DssBinaryContextDetailResolvedFormats = 0x80,
//    DssBinaryContextDetailDrillHigh = 0x100,
//    DssBinaryContextDetailDrillMedium = 0x200,
//    DssBinaryContextDetailDrillLow = 0x400,
//    DssBinaryContextGraphUseVirtualFonts = 0x800,
//    DssBinaryContextDetailSpecifiedNodeOnly = 0x1000,
//    DssBinaryContextDrillDetailInfo = 0x2000,
//    DssBinaryContextDetailPartialObjectRetrieval = 0x4000,
//    DssBinaryContextDetailInitialSlice = 0x8000,
//    DssBinaryContextDetailInitialLayout = 0x10000,
//    DssBinaryContextDetailSpecificLayout = 0x20000,
//    DssBinaryContextDetailUnitObjectFormats = 0x40000,
//    DssBinaryContextDetailExpressionEx = 0x80000,
//    DssBinaryContextGraphUseSpecialSeries = 0x100000,
//    DssBinaryContextDetailDataSliceOptimization = 0x200000,
//    DssBinaryContextDetailNoUnresolvedFormats = 0x400000
//} EnumBinaryContextDetailFlags;

typedef enum EnumDSSExternalSourceFlags
{
    DssExternalSourceNone =	0x00000000,
    DssExternalSourceForBrowsing =	0x00000001,
    DssExternalSourceSearchElementsEnabled =	0x00000002
} EnumDSSExternalSourceFlags;

typedef enum EnumFormatPropertySet
{
    EnumFormattingNumber = 0x10000,
    EnumFormattingAlignment = 0x20000,
    EnumFormattingFont = 0x30000,
    EnumFormattingBorder = 0x40000,
    EnumFormattingPatterns = 0x50000,
    EnumFormattingBanding = 0x60000,
    EnumFormattingLine = 0x70000,
    EnumFormattingSize = 0x80000,
    EnumFormattingAppearance = 0x90000,
    EnumFormattingCrosstab = 0xA0000,
    EnumFormattingPadding = 0xB0000,
    EnumFormattingPicture = 0xC0000,
    EnumFormattingOrder = 0xD0000,
    EnumFormattingView = 0xE0000,
    EnumFormattingLayout = 0xF0000,
    EnumFormattingHTML = 0x100000,
    ENumFormattingFilterDetails = 0x110000,
    EnumFormattingSelector = 0x120000,
    EnumFormattingVisualization = 0x130000,
    EnumFormattingTransitions = 0x140000,
    EnumFormattingPage = 0x150000,
    EnumFormattingNavigation = 0x160000,
    EnumFormattingWatermark = 0x170000,
    EnumFormattingIncrementalFetch = 0x180000,
    EnumFormattingPosition = 0x190000,
    EnumFormattingSection = 0x1A0000,
    EnumFormattingGroup = 0x1B0000,
    EnumFormattingRWDProperties = 0x1C0000,
    EnumFormattingWidget = 0x1D0000,
    EnumFormattingAnalysis = 0x1E0000,
    EnumFormattingTransactions = 0x1F0000,
    EnumFormattingMax = 0xFF0000
} EnumFormatPropertySet;

typedef enum EnumDSSTemplateUnitFormats
{
    DssTemplateUnitHeaderFormat = 0,
    DssTemplateUnitGridFormat = 1,
    DssTemplateUnitSubtotalHeaderFormat = 2,
    DssTemplateUnitSubtotalGridFormat = 3,
    DssTemplateUnitFormatMax = 4
} EnumDSSTemplateUnitFormats;

typedef enum EnumDSSImportDataType
{
    DssImportDataReserved = 0x00000000,
    DssImportDataBinary = 0x00000001,
    DssImportDataOtherTypes = 0x00000002
} EnumDSSImportDataType;

typedef enum EnumDSSDataImportColumnUpdateStateFlags
{
    DssDataImportColumnUpdateStateReserved = 0,
    DssDataImportColumnUpdateStateUnchanged = 0x00000001,
    DssDataImportColumnUpdateStateNew = 0x00000002,
    DssDataImportColumnUpdateStateMissing = 0x00000004,
    DssDataImportColumnUpdateStateChanged = 0x00000008,
    DssDataImportColumnUpdateStateGroupMask = 0xFFFFFFF0,
    DssDataImportColumnUpdateStatePersisted = 0x10000000
} EnumDSSDataImportColumnUpdateStateFlags;

typedef enum EnumDSSDataSetGroupStatus
{
    EnumDSSDataSetGroupStatusReserved = 0,
    EnumDSSDataSetGroupStatusTreeIndicesChanged = 0x00000001,
    EnumDSSDataSetGroupStatusViewChanged = 0x00000002,
    EnumDSSDataSetGroupStatusXTabOnly = 0x00000004,
    EnumDSSDataSetGroupInitAll = 0xffffffff
} EnumDSSDataSetGroupStatus;

typedef enum EnumDSSPrivilegeUpdatesType
{
    DssPrivilegeUpdatesNone = 0x00000000,
    DssPrivilegeUpdatesIntroducedIn90 = 0x00000001,
    DssPrivilegeUpdatesIntroducedIn901 = 0x00000002,
    DssPrivilegeUpdatesIntroducedIn902 = 0x00000004,
    DssPrivilegeUpdatesIntroducedIn921 = 0x00000008,
    DssPrivilegeUpdatesAll = 0xffffffff
} EnumDSSPrivilegeUpdatesType;

typedef enum EnumDSSDBRoleType
{
    DssDBRoleReserved = 0,
    DssDBRoleNormal = 1,
    DssDBRoleDataImport = 2,
    DssDBRoleDataImportPrimary = 3
} EnumDSSDBRoleType;

typedef enum EnumDSSCubeRefreshType
{
    DssCubeRefreshTypeReserved = 0,
    DssCubeRefreshAdd = 1,
    DssCubeRefreshDelete = 2,
    DssCubeRefreshUpdate = 3,
    DssCubeRefreshUpSert = 4,
    DssCubeRefreshTypeMax
} EnumDSSCubeRefreshType;

typedef enum EnumDSSCubeIncrementType
{
    DssCubeIncrementTypeReserved = 0,
    DssCubeRefreshUseFilter = 1,
    DssCubeRefreshUseReport = 2
} EnumDSSCubeIncrementType;

typedef enum EnumDSSCubeRepublishBehaviorType
{
    DssCubeRepublishReserved = 0,
    DssCubeRepublishReplaceCube = 1,
    DssCubeRepublishAddToCube = 2,
    DssCubeRepublishDynamicRepublish = 3,
    DssCubeRepublishUpsert = 4,
    DssCubeRepublishBehaviorTypeMax
} EnumDSSCubeRepublishBehaviorType;

typedef enum EnumSchemaSerializationLevel
{
    EnumSerializationReserved = 0,
    EnumSerializationLite = 1,
    EnumSerializationCSIGeneration = 2,
    EnumSerializationComplete = 3,
    EnumSerializationCSIUnitTesting = 4,
} EnumSchemaSerializationLevel;

typedef enum EnumDSSNCSFileDeliveryCrossPlatformTypes
{
    DSSNCSFileDeliveryCrossPlatformReserved = 0x00000000,
    DSSNCSFileDeliveryFromUnixToWindows = 0x00000001,
    DSSNCSFileDeliveryFromWindowsToUnix = 0x00000002
} EnumDSSNCSFileDeliveryCrossPlatformTypes;

typedef enum EnumSchemaLoadingMode
{
    EnumSchemaLoadReserved = 0,
    EnumSchemaLoadCOM = 1,
    EnumSchemaLoadDirect = 2,
    EnumSchemaLoadBoth = 3
} EnumSchemaLoadingMode;

//typedef enum EnumDSSTransactionMappingFlags
//{
//    DssTransactionMappingDefault = 0,
//    DssTransactionMappingOptional = 0x00000001
//} EnumDSSTransactionMappingFlags;

typedef EnumDSSTransactionDataChangeType EnumDSSTransactionDataType;
//typedef enum EnumDSSTransactionDataType
//{
//    DssTransactionDataReserved = 0,
//    DssTransactionDataUpdate = 1,
//    DssTransactionDataDelete = 2,
//    DssTransactionDataInsert = 4,
//    DssTransactionDataOriginal = 8,
//    DssTransactionDataCommit = 16,
//    DssTransactionDataLastOne
//} EnumDSSTransactionDataType;

typedef enum EnumDSSUpdateTransactionDataType
{
    DssUpdateTransactionDataReserved = 0,
    DssUpdateTransactionDataAppend,
    DssUpdateTransactionDataReplace,
    DssUpdateTransactionDataClear,
    DssUpdateTransactionDataLastOne
} EnumDSSUpdateTransactionDataType;

typedef enum EnumDSSDocumentReportStatus
{
    DssDocumentReportReserved = 0,
    DssDocumentReportExecuting,
    DssDocumentReportCompleted,
    DssDocumentReportError,
    DssDocumentReportStopped
} EnumDSSDocumentReportStatus;

typedef enum EnumDSSTransactionDataOperationType
{
    DssTransactionDataOperationReserved = 0,
    DssTransactionDataOperationUpdate = 1,
    DssTransactionDataOperationMark = 2,
    DssTransactionDataOperationUnMark = 4,
    DssTransactionDataOperationLastOne
} EnumDSSTransactionDataOperationType;

typedef enum EnumDSSAddDataElementFlag
{
    EnumDSSAddDataElementReserved = 0,
    EnumDSSAddDataElementNoACLCheck = 1
} EnumDSSAddDataElementFlag;

/*
typedef enum EnumDSSRWControlFilterType
{
    EnumDSSRWControlSlicing = 0,
    EnumDSSRWControlUnitCondition = 1
} EnumDSSRWControlFilterType;
*/

typedef enum EnumDSSShortcutObjectType
{
    DssShortcutObjectTypeReserved,
    DssShortcutObjectTypeObject,
    DssShortcutObjectTypeURL,
    DssShortcutObjectTypeSnapshot,
    DssShortcutObjectTypeLastOne
} EnumDSSShortcutObjectType;

typedef enum EnumDSSNonSharedProperty
{
    EnumDSSNonSharedPropertyReserved = 0,
    EnumDSSNonSharedPropertyTop = 1,
    EnumDSSNonSharedPropertyLeft = 2,
    EnumDSSNonSharedPropertyWidth = 3,
    EnumDSSNonSharedPropertyHeight = 4,
    EnumDSSNonSharedPropertyVisible = 5,
    EnumDSSNonSharedPropertyWidthMode = 6,
    EnumDSSNonSharedPropertyHeightMode = 7,
    EnumDSSNonSharedPropertyTitleHeight = 8,
    EnumDSSNonSharedPropertyUsePageWidthAsLayoutWidth = 9,
    EnumDSSNonSharedPropertyMaxHeight = 10,
    EnumDSSNonSharedPropertyCanGrow = 11,
    EnumDSSNonSharedPropertyCanShrink = 12,
    EnumDSSNonSharedPropertyHideIfEmpty = 13,
    EnumDSSNonSharedPropertySelectorOrientation = 14
} EnumDSSNonSharedProperty;

typedef enum EnumDSSDocumentViewOption
{
    EnumDSSDocumentViewOptionReserved = 0,
    EnumDSSDocumentViewOptionNoDocumentView = 1,
    EnumDSSDocumentViewOptionFirstView = 2,
    EnumDSSDocumentViewOptionCurrentView = 3,
    EnumDSSDocumentViewOptionAllViews = 4,
    EnumDSSDocumentViewOptionRemoveNonSharedProperties = 5
} EnumDSSDocumentViewOption;

typedef enum EnumDSSEMMigrationOperationType
{
    DssEMMigrationOperationReserved = 0,
    DssEMMigrationPopulateObject = 1,
    DssEMMigrationPopulateSecurityFilterRelate = 2,
    DssEMMigrationPopulateScheduleRelates = 3,
    DssEMMigrationPopulateIDsTable = 4,
    DssEMMigrationRepopulateRelates = 5,
    DssEMMigrationOperationReservedLastOne = 6
} EnumDSSEMMigrationOperationType;

typedef enum EnumDSSEMCommandStatus
{
    DssEMCommandSuccess = 0,
    DssEMCommandInProgress = 1,
    DssEMCommandError = 2,
    DssEMCommandNotFound = 3,
    DssEMCommandSuccessWithoutJournaling = 4,
    DssEMCommandPartialSuccess = 5,
    DssEMCommandPartialSuccessWithoutJournaling = 6
} EnumDSSEMCommandStatus;

typedef enum EnumDSSEMObjectID
{
    DssEMObjectReserved = 0,
    DssEMObjectUser = 1,
    DssEMObjectProject = 2,
    DssEMObjectAttributeForm = 3,
    DssEMObjectTable = 4,
    DssEMObjectTransformation = 5,
    DssEMObjectHierarchy = 6,
    DssEMObjectColumn = 7,
    DssEMObjectReport = 8,
    DssEMObjectDocument = 9,
    DssEMObjectFilter = 10,
    DssEMObjectTemplate = 11,
    DssEMObjectMetric = 12,
    DssEMObjectCustomGroup = 13,
    DssEMObjectConsolidation = 14,
    DssEMObjectPrompt = 15,
    DssEMObjectServerDef = 16,
    DssEMObjectEvent = 17,
    DssEMObjectSchedule = 18,
    DssEMObjectFact = 19,
    DssEMObjectAttribute = 20,
    DssEMObjectUserGroup = 21,
    DssEMObjectDBConnection = 22,
    DssEMObjectDBTable = 23,
    DssEMObjectSecurityFilter = 24,
    DssEMObjectTransmitter = 25,
    DssEMObjectDevice = 26,
    DssEMObjectDBInstance = 27,
    DssEMObjectLastOne = 28
} EnumDSSEMObjectID;

typedef enum EnumDSSRWDManipulationCategory
{
    EnumDSSRWDManipulationCategoryReserved = 0,
    EnumDSSRWDManipulationCategoryDrill = 1,
    EnumDSSRWDManipulationCategoryDerivedElement = 2,
    EnumDSSRWDManipulationCategoryPivotSort = 4,
    EnumDSSRWDManipulationCategoryUnitLimit = 8,
    EnumDSSRWDManipulationCategoryViewFilter = 16,
    EnumDSSRWDManipulationCategoryLastOne
} EnumDSSRWDManipulationCategory;

//typedef enum EnumDSSAttributeFormDisplayOption
//{
//    DssAttributeFormReserved = 0,
//    DssAttributeFormDefault = 1,
//    DssAttributeFormBrowse = 2,
//    DssAttributeFormDisplay = 3,
//    DssAttributeFormCustom = 4
//} EnumDSSAttributeFormDisplayOption;

typedef enum EnumDSSXMLFlags
{
    DssXMLFlagsReserved = 0,
    DssXMLLeanXML = 0x00000001,
    DssXMLLeanJSON = 0x00000002,
    DssXMLAllTranslations = 0x00000100,
    DssXMLFlagsLastOne
} EnumDSSXMLFlags;

typedef enum EnumDSSGetElementsResultFlag
{
    DssGetElementsResultFlagReserved = 0,
    DssGetElementsResultFlagTerseElementID = 0x00000001,
    DssGetElementsResultFlagLeafElements = 0x00000002
} EnumDSSGetElementsResultFlag;

typedef enum EnumDRLDefinitionField
{
    DssDRLDefinitionReserved = 0,
    DssDRLDefinitionDevice = 0x00000001,
    DssDRLDefinitionAddress = 0x00000002,
    DssDRLDefinitionMSTRUser = 0x00000004,
    DssDRLDefinitionPrmoptAnswer = 0x00000008,
    DssDRLDefinitionNoficationDevice = 0x00000010,
    DssDRLDefinitionNoficationAddress = 0x00000020,
    DssDRLDefinitionRecipientName = 0x00000040,
    DssDRLDefinitionRecipientID = 0x00000080,
    DssDRLDefinitionFieldsCount = 8
} EnumDRLDefinitionField;

typedef enum EnumDRLContactStatus
{
    DssDRLContactStatusReserved = 0,
    DssDRLContactStatusUnExpanded = 0x00000001,
    DssDRLContactStatusExpanded = 0x00000002
} EnumDRLContactStatus;

typedef enum EnumContactMode
{
    DssContactModeReserved = 0,
    DssContactNormalMode = 1,
    DssCGExpandedMode = 2
} EnumContactMode;

typedef enum EnumDSSDEManipulationFlags
{
    EnumDSSDEManipulationReserved = 0,
    EnumDSSDEManipulationDesignMode = 0x00000001
} EnumDSSDEManipulationFlags;

typedef enum EnumDSSProducts
{
    DssProductReserved = 0x00000000,
    DssProductPlatform = 0x00000001,
    DssProductCloudProfessional = 0x00000002,
    DssProductCloudPersonal = 0x00000004,
    DssProductLastOne
} EnumDSSProducts;

typedef enum EnumDSSDisplayNode
{
    DssDisplayNodeTypeReserved = 0,
    DssDisplayNodeTypeDisplayFolder = 1,
    DssDisplayNodeTypeMeasureGroup = 2
} EnumDSSDisplayNode;

typedef enum EnumDSSPopulationStrategy
{
    DssPopulationReserved = 0,
    DssPopulationDoNotCopy,
    DssPopulationEmbed,
    DssPopulationEmbedIdentical,
    DssPopulationCopy,
    DssPopulationCopyIfManaged,
    DssPopulationAsk,
    DssPopulationEmbedLegacy,
    DssPopulationSetToNull
} EnumDSSPopulationStrategy;

typedef enum EnumDSSLinkFlags
{
    DssLinkFreshItems = 0x00000001,
    DssLinkLocalOnly = 0x00000002,
    DssLinkWritable = 0x00000004,
    DssLinkReadOnly = 0x00000008,
    DssLinkFindModifiedOnly = 0x40000000,
    DssLinkPerformingDelete = 0x80000000
} EnumDSSLinkFlags;

typedef enum EnumDSSServerCommand
{
    DssCmdReserved = 0x000,
    DssCmdGetJobs,
    DssCmdGetUserSessions,
    DssCmdGetProjectInstances,
    DssCmdCancelJobs,
    DssCmdCancelJob,
    DssCmdOpenProject,
    DssCmdCloseProject,
    DssCmdIdleProject,
    DssCmdResumeProject,
    DssCmdSignIn,
    DssCmdSignOut,
    DssCmdLogoutUserFromProject,
    DssCmdDisconnectUserSession,
    DssCmdStopServer,
    DssCmdBackup,
    DssCmdRestore,
    DssCmdChangeConfiguration,
    DssCmdObjectServerCmds,
    DssCmdReportServerCmds,
    DssCmdRunReportByID,
    DssCmdPing,
    DssCmdRequestExecute,
    DssCmdGetRequestResult,
    DssCmdDatamartReportExecute,
    DssCmdRunDocumentByID,
    DssCmdCacheAdmin,
    DssCmdExecuteFolder,
    DssCmdHistoryListAdmin,
    DssCmdObjectServerGarbageCollect,
    DssCmdBatchLDAPImport,
    DssCmdPurgeStatistics,
    DssCmdCubeAdmin,
    DssCmdLastOne
} EnumDSSServerCommand;

typedef enum EnumDSSRefreshOption
{
    DssServerRefreshReserved = 0x0,
    DssServerRefreshNo = 0x1,
    DssServerRefreshSynch = 0x2,
    DssServerRefreshAsynch = 0x4,
    DssServerRefreshRegister = 0x8
} EnumDSSRefreshOption;

typedef enum EnumDSSCacheExpCommand
{
    DssCacheExpCommandInvalidate = 0x0,
    DssCacheExpCommandDelete = 0x1,
    DssCacheExpCommandElementPurge = 0x2,
    DssCacheExpCommandInvalidateMultiple = 0x3,
    DssCacheExpCommandDeleteMultiple = 0x4
} EnumDSSCacheExpCommand;

typedef enum EnumDSSCacheExpOptions
{
    DssCacheExpOptionsInboxOnly = 0x1,
    DssCacheExpOptionsReportCacheOnly = 0x2,
    DssCacheExpOptionsBoth = DssCacheExpOptionsInboxOnly  | DssCacheExpOptionsReportCacheOnly
} EnumDSSCacheExpOptions;

typedef enum EnumDSSHistoryListCommand
{
    DssHistoryListCommandCleanup = 0x8B,
    DssHistoryListCommandDeleteCache = 0x8C
} EnumDSSHistoryListCommand;

typedef enum EnumDSSCubeCommand
{
    DssCubeCommandDelete = 0x1,
    DssCubeCommandActivate = 0x2,
    DssCubeCommandDeactivate = 0x3,
    DssCubeCommandRefresh = 0x4
} EnumDSSCubeCommand;

typedef enum EnumDSSElementCacheCommand
{
    DssElementCacheCommandPurgeAll = -1,
    DssElementCacheCommandReserve = 0,
    DssElementCacheCommandPurgeByAttribute = 0x1,
    DssElementCacheCommandRefresh = 0x2
} EnumDSSElementCacheCommand;

typedef enum EnumDSSAutoCommit
{
    DssAutoCommitNo = 0,
    DssAutoCommitYes = 1
} EnumDSSAutoCommit;

typedef enum EnumDSSNetProtocol
{
    DssNetProtocolDefault = 0,
    DssNetProtocolTCP = 1,
    DssNetProtocolIPX = 2,
    DssNetProtocolNB = 3,
    DssNetProtocolUDP = 4,
    DssNetProtocolMUDP = 5
} EnumDSSNetProtocol;

typedef enum EnumDSSServerNetStatus
{
    DssServerNetInit = 0x0,
    DssServerNetDisconnect,
    DssServerNetConnectAsync,
    DssServerNetConnectSync
} EnumDSSServerNetStatus;

typedef enum EnumDSSMsgType
{
    DssMsgTypeReserved = 0x0,
    DssMsgTypeRequest,
    DssMsgTypeResult,
    DssMsgTypeProgress,
    DssMsgTypeError
} EnumDSSMsgType;

typedef enum EnumDSSAuthModes
{
    DssAuthReserved = 0x00,
    DssAuthStandard = 0x01,
    DssAuthNTCredential = 0x02,
    DssAuthPassThrough = 0x04,
    DssAuthAnonymous = 0x08,
    DssAuthLDAP = 0x10,
    DssAuthWarehousePassthrough = 0x20,
    DssAuthSimpleSecurityPlugIn = 0x40,
    DssAuthIntegrated = 0x80
} EnumDSSAuthModes;

typedef enum EnumDSSNetworkSerializationVersion
{
    DssSerializationNone = 0,
    DssSerializationMsg = 1,
    DssSerializationBlockStream = 2
} EnumDSSNetworkSerializationVersion;

typedef enum EnumDSSInBoxMsgRsltType
{
    DssInBoxMsgRsltTypeReserved,
    DssInBoxMsgRsltTypeReport = 1,
    DssInBoxMsgRsltTypeDocument = 2,
    DssInBoxMsgRsltTypeDocumentInstance = 3,
    DssInBoxMsgRsltTypePDF = 4,
    DssInBoxMsgRsltTypeExcel = 5,
    DssInBoxMsgRsltTypeCSV = 6,
    DssInBoxMsgRsltTypeFlash = 7,
    DssInBoxMsgRsltTypeNoRslt
} EnumDSSInBoxMsgRsltType;

typedef enum EnumDSSInBoxMessageStatus
{
    DssInBoxStatusReserved,
    DssInBoxNewMsg = 0x00000001,
    DssInBoxReadMsg = 0x00000002,
    DssInBoxDeletedMsg = 0x00000004,
    DssInBoxDeferredMsg = 0x20000000,
    DssInBoxWorkingMsg = 0x40000000,
    DssInBoxSavedMsg = 0x80000000,
    DssInBoxKeepAsIsMsg = 0x01000000,
    DssInBoxCacheHitMsg = 0x02000000,
    DssInBoxStaticMsg = 0x04000000,
    DssInBoxiPadSubscriptionMsg = 0x08000000,
    DssInBoxGridModeMsg = 0x00000010,
    DssInBoxGraphModeMsg = 0x00000020,
    DssInBoxGridGraphModeMsg = DssInBoxGraphModeMsg  | DssInBoxGridModeMsg,
    DssInBoxResultReportDef = 0x00000100,
    DssInBoxNotSaveExportResultMsg = 0x00800000
} EnumDSSInBoxMessageStatus;

typedef enum EnumDSSInBoxMsgType
{
    DssInBoxMsgTypeReserved,
    DssInBoxMsgTypeGeneral,
    DssInBoxMsgTypePersistent,
    DssInBoxMsgTypeActive
} EnumDSSInBoxMsgType;

typedef enum EnumDSSInBoxSettingID
{
    DssInBoxMaxMsgCount,
    DssInBoxMaxMsgRetentionTime,
    DssInBoxMsgAutoRemove
} EnumDSSInBoxSettingID;

typedef enum EnumDSSUserRequestType
{
    DssUserRequestTypeReserved = 0,
    DssUserRequestReport,
    DssUserRequestDynamicReport,
    DssUserRequestDocument,
    DssUserRequestWorkbook,
    DssUserRequestChildReport,
    DssUserRequestSearching,
    DssUserRequestDatamartReport,
    DssUserRequestReportWritingDoc,
    DssUserRequestRWDataset,
    DssUserRequestDynamicRWD,
    DssUserRequestCube,
    DssUserRequestIncrementalRefresh,
    DssUserRequestRWDTransactionReport
} EnumDSSUserRequestType;

typedef enum EnumDSSUserRequestStatus
{
    DssUserRequestStatusReserved = 0,
    DssUserRequestResultReady = 1,
    DssUserRequestWaitForPrompt = 2,
    DssUserRequestJobExecutionError = 3,
    DssUserRequestQueued = 4,
    DssUserRequestSqlGeneration = 5,
    DssUserRequestSqlExecution = 6,
    DssUserRequestInAnalyticalEngine = 7,
    DssUserRequestInResolutionServer = 8,
    DssUserRequestWaitingForCache = 9,
    DssUserRequestUpdatingCache = 10,
    DssUserRequestWaiting = 13,
    DssUserRequestWaitingOnGovernor = 14,
    DssUserRequestWaitingForProject = 15,
    DssUserRequestWaitingForChildren = 16,
    DssUserRequestPackingSendingOutput = 17,
    DssUserRequestExecutionError = 18,
    DssUserRequestConstructResult = 19,
    DssUserRequestHTMLResult = 20,
    DssUserRequestXMLResult = 21,
    DssUserRequestRunningOnOtherNode = 22,
    DssUserRequestLoadingPrompt = 23,
    DssUserRequestInExportEngine = 24
} EnumDSSUserRequestStatus;

typedef enum EnumDSSUserProperty
{
    DssUserPropertyReserved = 0,
    DssUserPropertyViewMode = 1,
    DssUserPropertyKeepAsIs = 2,
    DssUserPropertyStatic = 3,
    DssUserPropertyDisplayName = 4,
    DssUserPropertyMsgOwnerName = 5,
    DssUserPropertyObjViewMedia = 6,
    DssUserPropertyLastIndex
} EnumDSSUserProperty;

typedef enum EnumDSSInBoxMsgField
{
    DssInBoxMsgFieldReserved =	0,
    DssInBoxMsgMessageID = 0x00000001,
    DssInBoxMsgSubInstID = 0x00000002,
    DssInBoxMsgObjectID = 0x00000004,
    DssInBoxMsgFolderID = 0x00000008,
    DssInBoxMsgProjectID = 0x00000010,
    DssInBoxMsgObjectName = 0x00000020,
    DssInBoxMsgObjectOwnerName = 0x00000040,
    DssInBoxMsgProjectName = 0x00000080,
    DssInBoxMsgObjectDesc = 0x00000100,
    DssInBoxMsgFolderName = 0x00000200,
    DssInBoxMsgStartTime = 0x00000400,
    DssInBoxMsgFinishTime = 0x00000800,
    DssInBoxMsgOwnerID = 0x00001000,
    DssInBoxMsgParentID = 0x00002000,
    DssInBoxMsgResultID = 0x00004000,
    DssInBoxMsgTriggerID = 0x00008000,
    DssInBoxMsgMessageStatus = 0x00010000,
    DssInBoxMsgType = 0x00020000,
    DssInBoxMsgRequestStatus = 0x00040000,
    DssInBoxMsgRequestType = 0x00080000,
    DssInBoxMsgClientType = 0x00100000,
    DssInBoxMsgCreationTime = 0x00200000,
    DssInBoxMsgModificationTime = 0x00400000,
    DssInBoxMsgLocale = 0x00800000,
    DssInBoxMsgLastErrorCode = 0x01000000,
    DssInBoxMsgExecutingNode = 0x02000000,
    DssInBoxMsgMessageText = 0x04000000,
    DssInBoxMsgDisplayName = 0x08000000,
    DssInBoxMsgObjViewMedia = 0x10000000,
    DssInBoxMsgOwnerName = 0x20000000,
    DssInBoxMsgObjAbbreviation = 0x40000000,
    DssInBoxMsgMessageFieldAll = -1,
    DssInBoxMsgMessageFieldCount = 31
} EnumDSSInBoxMsgField;

typedef enum EnumIDSSUserSessionErrors
{
    E_IDSSUserSession_Not_Initialized = 0x80040200
} EnumIDSSUserSessionErrors;

typedef enum EnumDSSInfoType
{
    DssInfoTypeReserved = 0,
    DssSessionInfo = 1,
    DssUserProjectInfo = 2
} EnumDSSInfoType;

typedef enum EnumDSSDBConnectionStatus
{
    DssDBConnectionBusy,
    DssDBConnectionCached
} EnumDSSDBConnectionStatus;

typedef enum EnumDSSGetDBConnOption
{
    DssGetDBConnOptionDetail,
    DssGetDBConnOption
} EnumDSSGetDBConnOption;

typedef enum EnumDSSProjectStatus
{
    DssProjectStatusOffline = -1,
    DssProjectStatusOfflinePending = -2,
    DssProjectStatusErrorState = -3,
    DssProjectStatusOnlinePending = -4,
    DssProjectStatusActive = 0x0000,
    DssProjectStatusExecIdle = 0x0001,
    DssProjectStatusRequestIdle = 0x0002,
    DssProjectStatusMetadataIdle = DssProjectStatusExecIdle,
    DssProjectStatusWarehouseIdle = DssProjectStatusExecIdle,
    DssProjectStatusWHExecIdle = 0x0004,
    DssProjectStatusFullIdle = DssProjectStatusExecIdle  | DssProjectStatusRequestIdle | DssProjectStatusMetadataIdle | DssProjectStatusWarehouseIdle | DssProjectStatusWHExecIdle
} EnumDSSProjectStatus;

typedef enum EnumDSSProjectLoadOption
{
    DssProjectLoadActive = 0x000,
    DssProjectLoadFullIdle = 0x001,
    DssProjectLoadOffline = 0x002,
    DssProjectLoadAsynch = 0x004
} EnumDSSProjectLoadOption;

typedef enum EnumDSSProjectServerDestination
{
    DssProjectServerLocal = 0x000,
    DssProjectServerRemote = 0x001,
    DssProjectServerAll = 0x002
} EnumDSSProjectServerDestination;

typedef Int32 DssJobId;

typedef enum EnumDSSJobStatus
{
    DssJobStatusReady = 0,
    DssJobStatusExecuting = 1,
    DssJobStatusWaiting = 2,
    DssJobStatusCompleted = 3,
    DssJobStatusError = 4,
    DssJobStatusCanceled = 5,
    DssJobStatusStopped = 6,
    DssJobStatusWaitingOnGoverner =7,
    DssJobStatusWaitingForAutoPrompt =8,
    DssJobStatusWaitingForProject =9,
    DssJobStatusWaitingForCache =10,
    DssJobStatusWaitingForChildren =11,
    DssJobStatusWaitingForFetchingResults =12,
    DssJobStatusLoadingPrompt =13,
    DssJobStatusResolvingDestination =14,
    DssJobStatusDelivering =15,
    DssJobStatusExporting =16
} EnumDSSJobStatus;

typedef enum EnumIDSSJobStep
{
    DSSJobStepSQLGeneration = 0,
    DSSJobStepSQLExecution = 1,
    DSSJobStepLoadMetaData = 2,
    DSSJobStepXTab = 3
} EnumIDSSJobStep;

typedef enum EnumDSSGetJobOptions
{
    DssGetJobOptionBrowsing = 0x001,
    DssGetJobOptionDetail = 0x002,
    DssGetJobOptionSQL = 0x004,
    DssGetJobOptionDiagnostics = 0x008
} EnumDSSGetJobOptions;

typedef enum EnumDSSCloseJobOption
{
    DssCloseJobNormal = 0x0,
    DssCloseJobImmediate = 0x1
} EnumDSSCloseJobOption;

typedef BSTR DssClusterMemberId;

typedef Int32 DssServerLoad;

typedef enum EnumDSSClusterMemberStatus
{
    DssClusterMemberStatusExecuting = 1,
    DssClusterMemberStatusPause,
    DssClusterMemberStatusStopped
} EnumDSSClusterMemberStatus;

typedef enum EnumDSSProjectLevelClusteringPolicy
{
    DssProjectLevelClusteringPolicyDisabled = 0,
    DssProjectLevelClusteringPolicyProjectMapping = 1,
    DssProjectLevelClusteringPolicyProjectRedundency = 2
} EnumDSSProjectLevelClusteringPolicy;

typedef enum EnumDSSLicenseComplianceCategory
{
    DssLicenseComplianceCategoryCPU = 1,
    DssLicenseComplianceCategoryNamedUsers = 2,
    DssLicenseComplianceCategoryActivation = 3
} EnumDSSLicenseComplianceCategory;

typedef enum EnumDSSServerJobType
{
    DssJobRequestMetadata = 0,
    DssJobRequestReport = 1,
    DssJobRequestDocument = 2,
    DssJobRequestElement = 3,
    DssJobRequestSQL = 4,
    DssJobRequestBatchChildReport = 5,
    DssJobRequestBatchRequestExecute = 6,
    DssJobRequestDatamartReport = 7
} EnumDSSServerJobType;

typedef enum EnumDSSProcessReturnCode
{
    E_DSS_INSUFFICIENT_DATA = 0x80040300,
    E_DSS_CANCELED = 0x80040301,
    S_RESULT_IN_MSG = 0x80000300,
    S_RESULT_IN_RPT_INSTANCE = 0x80000301,
    S_PROCESS_MSG = 0x80000302,
    S_PROCESS_RPT_INSTANCE = 0x80000303,
    S_DSS_PROCESSING = 0x80000304
} EnumDSSProcessReturnCode;

typedef enum EnumDSSProcessSuccessFlags
{
    DSSPROCESS_S_WAIT_FOR_USER_INPUT = 0x00040001,
    DSSPROCESS_S_SEND_TO_USER_ERRMSG = 0x00040002,
    DSSPROCESS_S_SEND_TO_USER_OUTMSG = 0x00040004
} EnumDSSProcessSuccessFlags;

typedef enum EnumDSSProcessOptionFlags
{
    DssProcessCacheOnlyLookup = 0x1,
    DssProcessResolutionKeepEmbeddedStructure = 0x100,
    DssProcessResolutionPromptInPrompt = 0x1000,
    DssProcessResolutionRestorePrompts = 0x2000,
    DssProcessResolutionResolveSubsetOnly = 0x4000,
    DssProcessResolutionResolveLite = 0x8000,
    DssProcessResolutionPreloadedSchema = 0x0800,
    DssProcessResolutionNoTakeOwnership = 0x0400,
    DssProcessResolutionDoNotIgnoreDormantPrompts = 0x0200,
    DssProcessResolutionIgnoreSystemPrompt = 0x10000
} EnumDSSProcessOptionFlags;

typedef unsigned Int32 MSIDBC_ID;

typedef enum EnumDSSChannelType
{
    DssChannelReserved = 0,
    DssChannelTCPIP,
    DssChannelConsole
} EnumDSSChannelType;

typedef enum {
    MS_ACCESS = 100,
    ORACLE = 200,
    SQL_SERVER = 300,
    INFORMIX = 400,
    SYBASE = 500,
    REDBRICK = 600,
    DB2 = 700,
    TANDEM = 800,
    TERADATA = 900,
    UNKNOWN = 1000,
    GENERIC = 1100,
    SAP = 1200,
    NETEZZA = 1300,
    EXCEL = 1400,
    MSFT_AS = 1500,
    ESSBASE = 1600,
    MYSQL = 1700,
    POSTGRESQL = 1800,
    HPNEOVIEW = 1900,
    METAMATRIX = 2000,
    DATALLEGRO = 2100,
    COMPOSITE = 2200,
    ASTER = 2300,
    VERTICA = 2400,
    OPENACCESS = 2500,
    SYBASE_SQL_ANY = 2600,
    SYBASE_IQ = 2700,
    PARACCEL = 2800,
    TM1 = 2900,
    XQUERY = 3000 ,
    HIVE = 3100,
    SAND = 3200,
    SALESFORCE = 3300,
    HIVETHRIFT = 3400,
    VECTORWISE = 3500,
    ENTERPRISEDB = 3600,
    KOGNITIOWX2 = 3700,
    EXASOLUTION = 3800,
    INFORMATICA = 3900,
    SAPHANA = 4000
} DATABASE_TYPE;

typedef enum EnumDSSDatabaseType
{
    DssDatabaseTypeReserved = 0,
    DssDBAccess = MS_ACCESS,
    DssDBOracle = ORACLE,
    DssDBSQLServer = SQL_SERVER,
    DssDBInformix = INFORMIX,
    DssDBSybase = SYBASE,
    DssDBRedBrick = REDBRICK,
    DssDBDB2 = DB2,
    DssDBTandem = TANDEM,
    DssDBTeradata = TERADATA,
    DssDBUnknown = UNKNOWN,
    DssDBGeneric = GENERIC,
    DssMDXSAP = SAP,
    DssDBNetezza = NETEZZA,
    DssDBExcel = EXCEL,
    DssMDXMicrosoftAS = MSFT_AS,
    DssMDXEssbase = ESSBASE,
    DssDBMySQL = MYSQL,
    DssDBPostgreSQL = POSTGRESQL,
    DssDBHPNeoview = HPNEOVIEW,
    DssDBMetamatrix = METAMATRIX,
    DssDBDATAllegro = DATALLEGRO,
    DssDBComposite = COMPOSITE,
    DssDBAster = ASTER,
    DssDBVertica = VERTICA,
    DssDBOpenAccess = OPENACCESS,
    DssDBSybaseSQLAny = SYBASE_SQL_ANY,
    DssDBSybaseIQ = SYBASE_IQ,
    DssDBParAccel = PARACCEL,
    DssMDXTM1 = TM1,
    DssXQuery = XQUERY,
    DssDBHive = HIVE,
    DssDBSAND = SAND,
    DssDBSalesforce = SALESFORCE,
    DssDBHiveThrift = HIVETHRIFT,
    DssDBVectorWise = VECTORWISE,
    DssDBEnterpriseDB = ENTERPRISEDB,
    DssDBKognitioWX2 = KOGNITIOWX2,
    DssDBEXASolution = EXASOLUTION,
    DssDBInformatica = INFORMATICA,
    DssDBSAPHANA = SAPHANA
} EnumDSSDatabaseType;

typedef enum EnumDSSDatabaseVersion
{
    DssDatabaseVersionDefault =	-1,
    DssDatabaseVersionReserved = 0,
    DssDBTandemMPD45 = 1,
    DssDBTandemMX1 = 2,
    DssDBTandemMPD42 = 3,
    DssDBTeradataV2R1 = 4,
    DssDBTeradataV2R20002 = 5,
    DssDBTeradataV2R21 = 6,
    DssDBTeradatav2R3 = 7,
    DssDBOracle733 = 8,
    DssDBOracle8003 = 9,
    DssDBSQLServer65 = 10,
    DssDBSQLServer70 = 11,
    DssDBAccess20 = 12,
    DssDBAccess70 = 13,
    DssDBSybaseAdaptive115 = 14,
    DssDBSybaseSQL112 = 15,
    DssDBSybaseIQ112 = 16,
    DssDBIBMDB2OS39041 = 17,
    DssDBIBMDB2OS39050 = 18,
    DssDBIBMUDBSMP50 = 19,
    DssDBIBMUDBEEE50 = 20,
    DssDBIBMDB2PE12 = 21,
    DssDBIBMDB2CS212 = 22,
    DssDBIBMDB2400V3R7 = 23,
    DssDBIBMDB2400V4R1 = 24,
    DssDBIBMDB2400V4R2 = 25,
    DssDBInformixODS724UC1 = 26,
    DssDBInformixXPS82 = 27,
    DssDBAdabaseD6112 = 28,
    DssDBRedBrick5007 = 29,
    DssDBRedBrick5012 = 30,
    DssDBRedBrick5105 = 31,
    DssDBTeradataNTV2R2 = 32,
    DssDBTeradataNTV2R3 = 33,
    DssDBOracle8i = 34,
    DssDBSybaseIQ12 = 35,
    DssDBIBMUDB52 = 36,
    DssDBIBMDB2400V4R3 = 37,
    DssDBInformixODS731 = 38,
    DssDBInformixUDO92 = 39,
    DssDBInformixXPS83 = 40,
    DssDBIBMUDB61 = 41,
    DssDBOracle805 = 42,
    DssDBOracle8iR2 = 43,
    DssDBIBMUDB7 = 44,
    DssDBIBMDB2OS39062 = 45,
    DssDBIBMDB2OS3907 = 46,
    DssDBIBMDB2400V4R4 = 47,
    DssDBIBMDB2400V4R5 = 48,
    DssDBRedBrick6 = 49,
    DssDBTeradataV2R4 = 50,
    DssDBSybaseAdaptive12 = 51,
    DssDBSQLServer2000 = 52,
    DssDBAccess2000 = 53,
    DssDBInformix10 = 54,
    DssDBOracle8iR3 = 55,
    DssDBOracle9i = 56,
    DssRedBrick61 = 57,
    DssDBIBMUDB8 = 58,
    DssDBOracle8iR2SE = 59,
    DssDBTeradataV2R41 = 61,
    DssDBAccess2002 = 62,
    DssDBIBMDB2400V5R1 = 63,
    DssDBSybaseAdaptive125 = 64,
    DssDBRedBrick62 = 65,
    DssDBIBMDB2400V5R2 = 66,
    DssDBTeradataV2R5 = 67,
    DssMDXSAPBW30 = 68,
    DssDBOracle10g = 69,
    DssDBTeradataV2R51 = 70,
    DssDBInformixIDS93 = 71,
    DssDBInformixIDS94 = 72,
    DssDBRedBrick63 = 73,
    DssDBIBMDB2OS3908 = 74,
    DssDBNetezza22 = 75,
    DssDBExcel2003 = 76,
    DssDBNetezza25 = 77,
    DssMDXEssbaseHyperion = 78,
    DssMDXMicrosoftAS2005 = 79,
    DssDBMySQL50 = 80,
    DssDBPostgreSQL81 = 81,
    DssDBInformixIDS10 = 82,
    DssDBOracle10gR2 = 83,
    DssDBTeradataV2R6 = 84,
    DssDBTeradataV2R61 = 85,
    DssDBSybaseASE15 = 86,
    DssDBSQLServer2005 = 87,
    DssMDXMicrosoftAS2000 = 88,
    DssDBIBMDB2400V5R4 = 89,
    DssDBNetezza3 = 90,
    DssDBSybaseIQ127 = 91,
    DssDBIBMUDB91 = 92,
    DssDBTeradataV2R62 = 93,
    DssDBPostgreSQL82 = 94,
    DssDBHPNeoview22 = 95,
    DssDBOracle11g = 96,
    DssDBNetezza4 = 97,
    DssMDXEssbaseHyperion9 = 98,
    DssDBGreenplum3x = 99,
    DssDBHPNeoview23 = 100,
    DssDBIBMUDB95 = 101,
    DssDBTeradata12 = 102,
    DssDBIBMUDB91zOS = 103,
    DssDBAccess2007 = 104,
    DssDBIBMDB2400V6R1 = 105,
    DSSDBMetamatrix55 = 106,
    DSSDBDATAllegro3x = 107,
    DSSDBComposite450 = 108,
    DssDBAccess2003 = 109,
    DssDBSQLServer2008 = 110,
    DssDBMySQL51 =111,
    DssMDXMicrosoftAS2008 = 112,
    DssDBAsternCluster30 =113,
    DssDBVertica25 =114,
    DssMDXEssbaseHyperion9x = 115,
    DssDBOpenAccess14 = 116,
    DssDBSybaseSQLAny11 = 117,
    DssDBNetezza46 = 118,
    DssDBTeradata13 = 119,
    DssDBSybaseIQ15 = 120,
    DssDBSybaseIQ151 = 121,
    DssDBHPNeoview24 = 122,
    DssDBIBMUDB97 = 123,
    DssDBNetezza50 = 124,
    DssDBVertica30 =125,
    DssMDXSAPBW7x =126,
    DssDBOracle11gR2 =127,
    DssDBInformixIDS115 = 128,
    DssDBPostgreSQL84 = 129,
    DssDBSQLServer2008NativeClient = 130,
    DssDBParAccel2x = 131,
    DssDBNetezza4xDBlytix10 =132,
    DssMDXTM1x =133,
    DssDBIBMDB2400V7R1 = 134,
    DssDBAsternCluster40 =135,
    DssDBInfobright33 =136,
    DssDBVertica40 =137,
    DssDBHive05x =138,
    DssDBSybaseIQ152 = 139,
    DssDBAsternCluster45 =140,
    DssDBGreenplum4x = 141,
    DssXQuery1x =142,
    DssDBHPNeoview25 = 143,
    DssMDXEssbaseHyperion11x = 144,
    DssDBSANDCDBMS61 = 145,
    DSSDBComposite510 = 146,
    DssDBNetezza60 = 147,
    DssDBSQLServer2008R2ParallelDataWH = 148,
    DssDBPostgreSQL90 = 149,
    DssDBTeradata131 = 150,
    DssDBParAccel3x = 151,
    DssDBHive06x = 152,
    DssDBHive07x = 153,
    DssDBSalesforce1 = 154,
    DssDBPostgreSQL91 = 155,
    DssDBHiveThrift1 = 156,
    DssDBIBMUDBV10zOS = 157,
    DssDBVectorWise15 = 158,
    DssDBSQLServer2012 = 159,
    DssDBInfobright40 = 160,
    DssDBSybaseIQ153 = 161,
    DssDBSybaseIQ154 = 162,
    DssDBEnterpriseDB9x = 163,
    DssDBKognitioWX27x = 164,
    DssDBEXASolution3x = 165,
    DssDBEXASolution40 = 166,
    DssDBVertica5051 = 167,
    DssDBParAccel31x = 168,
    DssDBParAccel35x = 169,
    DssDBComposite6x = 170,
    DssDBInformatica91x = 171,
    DssDBIBMUDB101 = 172,
    DssDBAsternCluster46 =173,
    DssDBVectorWise20x = 174,
    DssMDXMicrosoftAS2005and08BinaryConnector = 175,
    DssMDXMicrosoftAS2012BinaryConnector = 176,
    DssDBTeradata14 = 177,
    DssDBHiveThrift08x = 178,
    DssDBHive08x = 179,
    DssDBVertica60 = 180,
    DssDBSAPHANA1 = 181,
    DssMDXMicrosoftAS2012 = 182
} EnumDSSDatabaseVersion;

typedef enum EnumDSSDBConnectionCache
{
    DssDBConnectionCacheReserved = 0,
    DssDBConnectionCacheNone,
    DssDBConnectionCacheReuse,
    DssDBConnectionCacheSerialReuse
} EnumDSSDBConnectionCache;

typedef enum EnumDSSDBConnectionMultiProcessOption
{
    DssDBConnectionMultiProcessReserved = 0,
    DssDBConnectionMultiProcessMultiThreadMode = 1,
    DssDBConnectionMultiProcessMultiProcessMode = 2
} EnumDSSDBConnectionMultiProcessOption;

typedef enum EnumDSSDBConnectionDriver
{
    DssDBConnectionDriverReserved = 0,
    DssDBConnectionDriverODBC = 1,
    DssDBConnectionDriverNativeDBAPI = 2
} EnumDSSDBConnectionDriver;

typedef enum EnumDSSDBConnectionExecutionMode
{
    DssDBConnectionExecutionReserved = 0,
    DssDBConnectionExecutionAsynchConnection,
    DssDBConnectionExecutionAsynchStatement,
    DssDBConnectionExecutionSynchronous
} EnumDSSDBConnectionExecutionMode;

typedef enum EnumDSSPUSettingID
{
    DssPUDisplayInfoCollectionPeriodicity,
    DssPUMaintenancePeriodicity
} EnumDSSPUSettingID;

typedef enum EnumDSSQueueServiceScheme
{
    DssServicingSchemeReserved = 0,
    DssServicingSchemeDefault = 1,
    DssServicingSchemeFixedThreadCooperative = DssServicingSchemeDefault,
    DssServicingSchemeFixedThread,
    DssServicingSchemeHighestPriorityFirst,
    DssServicingSchemeWeightedShare
} EnumDSSQueueServiceScheme;

typedef enum EnumDSSQueuePriorityScheme
{
    DssJobPrioritySchemeReserved = 0,
    DssJobPrioritySchemeDefault = 1,
    DssJobPrioritySchemeUserSupplied = DssJobPrioritySchemeDefault,
    DssJobPrioritySchemeRandom,
    DssJobPrioritySchemeMap
} EnumDSSQueuePriorityScheme;

typedef enum EnumDSSQueuePriority_Factors
{
    DssQueuePriority_Rreserved = 0,
    DssQueuePriority_UserPriority,
    DssQueuePriority_CostPriority,
    DssQueuePriority_RandomPriority
} EnumDSSQueuePriority_Factors;

typedef enum EnumDSSCacheSwapPolicy
{
    DssCacheSwapPolicyLRU = 0x00,
    DssCacheSwapPolicyLHC = 0x01
} EnumDSSCacheSwapPolicy;

typedef enum EnumDSSServerProjectSettingID
{
    DssServerProjectMaxResultRowCount = 0,
    DssServerProjectMaxReportExecutionTime = 1,
    DssServerProjectMaxMessageRetentionTime = 2,
    DssServerProjectMaxJobPerUserAccount = 4,
    DssServerProjectMaxJobPerUserConnection = 5,
    DssServerProjectMaxJobPerProject = 6,
    DssServerProjectMaxUserConnectionPerProject = 7,
    DssServerProjectCollectStatisticsFlag = 8,
    DssServerProjectLoadFlag = 9,
    DssServerProjectMaxReportCacheMemConsumption = 10,
    DssServerProjectMaxReportCacheCount = 11,
    DssServerProjectMaxReportCacheDiskSpace = 12,
    DssServerProjectReportCacheLookupTableSize = 13,
    DssServerProjectMaxObjectCacheMemConsumption = 14,
    DssServerProjectMaxObjectCacheCount = 15,
    DssServerProjectObjectCacheLookupTableSize = 16,
    DssServerProjectMaxElementCacheMemConsumption = 17,
    DssServerProjectMaxElementCacheCount = 18,
    DssServerProjectElementCacheLookupTableSize = 19,
    DssProjectObjectMaxCacheCount = DssServerProjectMaxObjectCacheCount,
    DssProjectObjectMaxCacheMemoryConsumption = DssServerProjectMaxObjectCacheMemConsumption,
    DssProjectObjectCacheTableBucketCount = DssServerProjectObjectCacheLookupTableSize,
    DssServerProjectObjectCacheTableBucketPerLock = 23,
    DssProjectObjectCacheTableBucketPerLock = DssServerProjectObjectCacheTableBucketPerLock,
    DssServerProjectObjectFileCachePath = 24,
    DssProjectObjectFileCachePath = DssServerProjectObjectFileCachePath,
    DssServerProjectObjectSwapPolicy = 25,
    DssProjectObjectSwapPolicy = DssServerProjectObjectSwapPolicy,
    DssServerProjectObjectCacheLifeTime = 26,
    DssProjectObjectCacheLifeTime = DssServerProjectObjectCacheLifeTime,
    DssProjectElementMaxCacheCount = DssServerProjectMaxElementCacheCount,
    DssProjectElementMaxCacheMemoryConsumption = DssServerProjectMaxElementCacheMemConsumption,
    DssProjectElementCacheTableBucketCount = DssServerProjectElementCacheLookupTableSize,
    DssServerProjectElementCacheTableBucketPerLock = 30,
    DssProjectElementCacheTableBucketPerLock = DssServerProjectElementCacheTableBucketPerLock,
    DssServerProjectElementFileCachePath = 31,
    DssProjectElementFileCachePath = DssServerProjectElementFileCachePath,
    DssServerProjectElementSwapPolicy = 32,
    DssProjectElementSwapPolicy = DssServerProjectElementSwapPolicy,
    DssServerProjectElementCacheLifeTime = 33,
    DssProjectElementCacheLifeTime = DssServerProjectElementCacheLifeTime,
    DssProjectReportMaxCacheCount = DssServerProjectMaxReportCacheCount,
    DssProjectReportMaxCacheMemoryConsumption = DssServerProjectMaxReportCacheMemConsumption,
    DssProjectReportCacheTableBucketCount = DssServerProjectReportCacheLookupTableSize,
    DssServerProjectReportCacheTableBucketPerLock = 37,
    DssProjectReportCacheTableBucketPerLock = DssServerProjectReportCacheTableBucketPerLock,
    DssServerProjectReportFileCachePath = 38,
    DssProjectReportFileCachePath = DssServerProjectReportFileCachePath,
    DssServerProjectReportSwapPolicy = 39,
    DssProjectReportSwapPolicy = DssServerProjectReportSwapPolicy,
    DssServerProjectReportCacheLifeTime = 40,
    DssProjectReportCacheLifeTime = DssServerProjectReportCacheLifeTime,
    DssServerProjectDisableReportCaching = 41,
    DssServerProjectMaxAERowCount = 42,
    DssServerProjectMaxElementRowCount = 43,
    DssServerProjectReportCacheDBMatchingFlag = 44,
    DssServerProjectLinkCacheMaxMemConsumption = 45,
    DssServerProjectLinkCacheTableBucketCount = 46,
    DssServerProjectLinkCacheTableBucketPerLock = 47,
    DssServerProjectLinkCacheSwapPolicy = 48,
    DssServerProjectMX2DBUserName = 49,
    DssServerProjectMX2DBPassword = 50,
    DssServerProjectMX2RepositoryName = 51,
    DssServerProjectMX2RepositoryUserName = 52,
    DssServerProjectMX2RepositoryPassword = 53,
    DssServerProjectMX2DSN = 54,
    DssServerProjectMX2RepositoryFolder = 55,
    DssServerProjectMaxExecutingJobsPerUser = 56,
    DssServerProjectMaxInternalResultRowCount = 57,
    DssServerProjectETLUseCache = 58,
    DssServerProjectReportCacheLoadOnStart = 59,
    DssServerProjectElementCacheUsesSecurityFilters = 60,
    DssServerProjectElementCacheUseConnectionMap = 61,
    DssServerProjectElementCacheUsePassThroughLogin = 62,
    DssServerProjectEnablePersonizedDrillPath = 63,
    DssServerProjectStatsPurgeTimeout = 64,
    DssServerProjectReportCacheSwapFactor = 65,
    DssServerProjectMaxCubeCount = 66,
    DssServerProjectMaxCubeMemUsage = 67,
    DssServerProjectCubeStorage = 68,
    DssServerProjectMaxInteractiveJobPerProject = 69,
    DssServerProjectMaxScheduledReportExecutionTime = 70,
    DssServerProjectScheduledReportExecutionRetryCount = 71,
    DssServerProjectMaxCubeResultRowCount = 72,
    DssServerProjectMaxDatamartResultRowCount = 73,
    DssServerProjectMaxPromptWaitingTime = 74,
    DssServerProjectRWDMaxCacheCount = 75,
    DssServerProjectRWDMaxMemoryConsumption = 76,
    DssServerProjectEnableRWDCaching = 77,
    DssServerProjectFileZippingCompressionEnabled = 78,
    DssServerProjectFileZippingCompressionType = 79,
    DssServerProjectFileZippingCompressionLevel = 80,
    DssServerProjectEmailZippingCompressionEnabled = 81,
    DssServerProjectEmailZippingCompressionType = 82,
    DssServerProjectEmailZippingCompressionLevel = 83,
    DssServerProjectCubeDBMatchingFlag = 84,
    DssServerProjectMaxInteractiveSessionPerUser = 85,
    DssServerProjectRWDCacheStorage = 86,
    DssServerProjectCacheEncryptionOption = 87,
    DssServerProjectMaxHistoryListSubscriptionCount = 88,
    DssServerProjectMaxCacheUpdateSubscriptionCount = 89,
    DssServerProjectMaxEmailSubscriptionCount = 90,
    DssServerProjectMaxFileSubscriptionCount = 91,
    DssServerProjectMaxPrintSubscriptionCount = 92,
    DssServerProjectReRunHistoryListSubscription = 93,
    DssServerProjectReRunEmailFilePrintSubscription = 94,
    DssServerProjectDoNotCreateUpdateCaches = 95,
    DssServerProjectCacheInfoFieldExcluded = 96,
    DssServerProjectMaxWHJobExecTime = 97,
    DssServerProjectMaxMobileSubscriptionCount = 98,
    DssServerProjectWebServerLocation = 99,
    DssServerProjectCacheIgnoreDynamicDate = 100,
    DssServerProjectCubeLoadOnStart = 101,
    DSSServerProjectMaxSQLGenMemConsumption = 102,
    DssServerProjectInvalidateCacheEventId = 103,
    DssServerProjectDeliveryErrorHandlingError = 104,
    DssServerProjectDeliveryErrorHandlingNoDataReturned = 105,
    DssServerProjectDeliveryErrorHandlingPartialResults = 106,
    DssServerProjectDistributionServiceEmailFooterEnabled = 107,
    DssServerProjectDistributionServiceEmailFooter = 108,
    DssServerProjectHistoryListKeepDocumentAvailableForManipulation = 109,
    DssServerProjectDistributionServicePrintRangeEnabled = 110,
    DssServerProjectStatisticsSQLTimeout = 111,
    DssServerProjectStatisticsPurgeTimeout = 112,
    DssServerProjectCubePublishOption = 113,
    DssServerProjectEnableMobilePushUpdate = 114,
    DssServerProjectDataUploadSizeLimit = 115,
    DssServerProjectCubeQuota = 116,
    DssServerProjectReportCacheLKUPMaxRAM = 117,
    DssServerProjectRWDCacheLKUPMaxRAM = 118,
    DssServerProjectMaxMemoryPerDataFetch = 119,
    DssServerProjectCubeIndexGrowthCheckFreq = 120,
    DssServerProjectCubeIndexGrowthUpperbound = 121,
    DssServerProjectMaxMsgCountPerProject = 122,
    DssServerProjectHistoryListSaveExportResultForRWD = 123,
    DssServerProjectHistoryListSaveChildMsgForRWD = 124,
    DssServerProjectFileZippingCompressionExtension = 125,
    DssServerProjectEmailZippingCompressionExtension = 126,
    DssServerProjectEnableSearch = 127,
    DssServerProjectEnableURLFileUpload = 128,
    DssServerProjectEnableURLFileUploadViaHttpHttps = 129,
    DssServerProjectEnableURLFileUploadViaFtpFtps = 130,
    DssServerProjectEnableURLFileUploadViaFile = 131,
    DssServerProjectDeliveryErrorHandlingNotifyNoDataReturned = 132,
    DssServerProjectDeliveryErrorHandlingNotifyPartialResults = 133,
    DssServerProjectLastSetting
} EnumDSSServerProjectSettingID;

typedef enum EnumDSSCubePublishOption
{
    DssServerProjectCubePublishNotLoaded = 1
} EnumDSSCubePublishOption;

typedef enum EnumDSSCacheInfoField
{
    DssServerProjectCacheInfoPromptAnswer = 1
} EnumDSSCacheInfoField;

typedef enum EnumDSSServerProjectCacheEncryptionOption
{
    DssServerProjectCacheEncryptionLevelNone = 0,
    DssServerProjectCacheEncryptionLevelLow = 0x01,
    DssServerProjectCacheEncryptionLevelHigh = 0x02
} EnumDSSServerProjectCacheEncryptionOption;

typedef enum EnumDSSServerProjectCubeDBMatchingFlag
{
    DssServerProjectCubeDBMatchingFlagReserved = 0,
    DssServerProjectCubeMatchDBConnection = 0x01
} EnumDSSServerProjectCubeDBMatchingFlag;

typedef enum EnumDSSServerProjectReportCacheDBMatchingFlag
{
    DssServerProjectReportCacheDBMatchingFlagReserved = 0,
    DssServerProjectReportCacheMatchUserID = 0x01,
    DssServerProjectReportCacheMatchDBLogin = 0x02,
    DssServerProjectReportCacheMatchDBConnection = 0x04
} EnumDSSServerProjectReportCacheDBMatchingFlag;

typedef enum EnumDSSStatisticModuleID
{
    DssStatisticModuleIDReserved = 0,
    DssStatisticModuleUser = 1,
    DssStatisticModuleReport = 2,
    DssStatisticModuleMetadata = 3,
    DssStatisticModuleElementReque = 4,
    DssStatisticModuleReportStepLevel = 5,
    DssStatisticModuleSession = 6,
    DssStatisticModuleReportBasic = 7,
    DssStatisticModuleReportDetail = 8,
    DssStatisticModuleCache = 9,
    DssStatisticModuleDocument = 10,
    DssStatisticLastModule = 10
} EnumDSSStatisticModuleID;

typedef enum EnumDSSStatisticTableID
{
    DssStatisticTableIDReserved = 0,
    DssStatisticSessionTableID = 1,
    DssStatisticScheduleTableID =2,
    DssStatisticReportTableID =3,
    DssStatisticReportSecurityTableID =4,
    DssStatisticReportStepTableID =5,
    DssStatisticReportSQLTableID = 6,
    DssStatisticCacheHitTableID = 7,
    DssStatisticDocumentTableID = 8,
    DssStatisticDocumentStepTableID = 9,
    DssStatisticReportColTableID =10,
    DssStatisticProjSessionTableID =11,
    DssStatisticReportManipTableID =12,
    DssStatisticMemoryTableID = 13,
    DssStatisticPromptAnswersTableID = 14,
    DssStatisticPerfMonTableID = 15,
    DssStatisticMessageTableID = 16,
    DssStatisticInBoxActTableID = 17,
    DssStatisticCubeRepTableID = 18,
    DssStatisticMBDeviceTableID = 19,
    DssStatisticMBExecTableID = 20,
    DssStatisticLastTable = 20
} EnumDSSStatisticTableID;

typedef enum EnumDSSStatisticDataID
{
    DssStatisticDataIDReserved = 0,
    DssStatisticUserSessionID,
    DssStatisticJobHeaderID,
    DssStatisticJobID,
    DssStatisticMDReqHeaderID,
    DssStatisticMDReqID,
    DssStatisticElemReqHeaderID,
    DssStatisticElemReqID,
    DssStatisticStepJobID,
    DssStatisticSessionId,
    DssStatisticScheduleSessionID,
    DssStatisticReportJobID,
    DssStatisticSecurityID,
    DssStatisticReportStepID,
    DssStatisticReportSQLID,
    DssStatisticReportCOLID,
    DssStatisticCacheId,
    DssStatisticDocumentJobId,
    DssStatisticUserModuleReserved = DssStatisticModuleUser*100,
    DssStatisticUserServer,
    DssStatisticUserProject,
    DssStatisticUserName,
    DssStatisticUserMachine,
    DssStatisticUserConnectTime,
    DssStatisticUserDisconnectTime,
    DssStatisticUserLastData,
    DssStatisticReportModuleReserved = DssStatisticModuleReport*100,
    DssStatisticJobStartTime,
    DssStatisticJobRepResolTime,
    DssStatisticJobSqlEngTime,
    DssStatisticJobQueryExecTime,
    DssStatisticJobAnalytTime,
    DssStatisticJobResolutionCPUTime,
    DssStatisticJobSqlEngineCPUTime,
    DssStatisticJobQueryExecutionCPUTime,
    DssStatisticJobAnalyticalCPUTime,
    DssStatisticJobFinishTime,
    DssStatisticJobDuration,
    DssStatisticJobStatus,
    DssStatisticJobServerName,
    DssStatisticJobProjectID,
    DssStatisticJobUserID,
    DssStatisticJobWebMachine,
    DssStatisticJobClientMachine,
    DssStatisticJobMachineName = DssStatisticJobClientMachine,
    DssStatisticJobReportID,
    DssStatisticJobTemplate,
    DssStatisticJobFilter,
    DssStatisticJobSource,
    DssStatisticJobCacheIndicator,
    DssStatisticJobSessionID,
    DssStatisticJobSqlSteps,
    DssStatisticJobFinalResultSize,
    DssStatisticHeaderJobTime,
    DssStatisticJobStatsProject,
    DssStatisticJobStatsSessionID,
    DssStatisticJobLastData,
    DssStatisticMdReqModuleReserved = DssStatisticModuleMetadata*100,
    DssStatisticMdReqServerName,
    DssStatisticMdReqProjectID,
    DssStatisticMdReqUserID,
    DssStatisticMdReqWebMachine,
    DssStatisticMdReqClientMachine,
    DssStatisticMdReqMachineName = DssStatisticMdReqClientMachine,
    DssStatisticMdReqJobSource,
    DssStatisticMdReqCacheIndicator,
    DssStatisticMdReqSessionID,
    DssStatisticMdReqStartTime,
    DssStatisticMdReqObjSrvTime,
    DssStatisticMdReqFinishTime,
    DssStatisticMdReqJobStatus,
    DssStatisticMdReqHeaderTime,
    DssStatisticMdReqStatsProject,
    DssStatisticMdReqStatsSessionID,
    DssStatisticMdReqLastData,
    DssStatisticElemReqModuleReserved = DssStatisticModuleElementReque*100,
    DssStatisticElemReqServerName,
    DssStatisticElemReqProjectID,
    DssStatisticElemReqUserID,
    DssStatisticElemReqWebMachine,
    DssStatisticElemReqClientMachine,
    DssStatisticElemReqMachineName = DssStatisticElemReqClientMachine,
    DssStatisticElemReqSource,
    DssStatisticElemCacheIndicator,
    DssStatisticElemSessionID,
    DssStatisticElemReqStartTime,
    DssStatisticElemReqSqlEngTime,
    DssStatisticElemReqResolutionTime,
    DssStatisticElemQueryExecTime,
    DssStatisticElemReqElemSrvTime,
    DssStatisticElemReqFinishTime,
    DssStatisticElemReqDuration,
    DssStatisticElemReqResultSize,
    DssStatisticElemReqJobStatus,
    DssStatisticElemReqHeaderTime,
    DssStatisticElemReqStatsProject,
    DssStatisticElemReqStatsSessionID,
    DssStatisticElemReqLastData,
    DssStatisticStepModuleReserved = DssStatisticModuleReportStepLevel*100,
    DssStatisticStepSequenceNumber,
    DssStatisticStepTablesAccessed,
    DssStatisticStepResultSize,
    DssStatisticStepDurationTime,
    DssStatisticStepSql,
    DssStatisticStepType,
    DssStatisticStepSessionID,
    DssStatisticStepLastData,
    DssStatisticSessionModuleReserved = DssStatisticModuleSession*100,
    DssStatisticSessionServerID,
    DssStatisticSessionSessionID,
    DssStatisticSessionServerName,
    DssStatisticSessionServerMachine,
    DssStatisticSessionWebMachine,
    DssStatisticSessionServerInstance,
    DssStatisticSessionProjectID,
    DssStatisticSessionUserID,
    DssStatisticSessionClientMachine,
    DssStatisticSessionConnectTime,
    DssStatisticSessionDisConnectTime,
    DssStatisticSessionSessionEvent,
    DssStatisticSessionEventTime,
    DssStatisticSessionEventSource,
    DssStatisticSessionEventNotes,
    DssStatisticSchedulerSessionID,
    DssStatisticSchedulerServerID,
    DssStatisticSchedulerProjectID,
    DssStatisticSchedulerTriggerID,
    DssStatisticSchedulerScheduleID,
    DssStatisticSchedulerScheduleType,
    DssStatisticSchedulerHitCache,
    DssStatisticSessionLastData,
    DssStatisticReportNewModuleReserved = DssStatisticModuleReportBasic*100,
    DssStatisticReportServerID,
    DssStatisticReportSessionId,
    DssStatisticReportServerName,
    DssStatisticReportServerMachine,
    DssStatisticReportProjectID,
    DssStatisticReportUserID,
    DssStatisticReportReportID,
    DssStatisticReportReportType,
    DssStatisticReportFilterID,
    DssStatisticReportEmbeddedFilter,
    DssStatisticReportTemplateID,
    DssStatisticReportEmbeddedTemplate,
    DssStatisticReportParentJobID,
    DssStatisticReportDBInstanceID,
    DssStatisticReportDBUserID,
    DssStatisticReportParentIndicator,
    DssStatisticReportRequestRecTime,
    DssStatisticReportRequestQueueTime,
    DssStatisticReportExecStartTime,
    DssStatisticReportExecFinishTime,
    DssStatisticReportSQLPasses,
    DssStatisticReportJobStatus,
    DssStatisticReportJobErrorCode,
    DssStatisticReportCancelledIndicator,
    DssStatisticReportAdhocIndicator,
    DssStatisticReportPromptIndicator,
    DssStatisticReportDatamartIndicator,
    DssStatisticReportElementLoadIndicator,
    DssStatisticReportDrillIndicator,
    DssStatisticReportScheduleIndicator,
    DssStatisticReportCacheCreateIndicator,
    DssStatisticReportPriorityNumber,
    DssStatisticReportJobCost,
    DssStatisticReportFinalResultSize,
    DssStatisticReportErrorMessage,
    DssStatisticReportSecurityJobID,
    DssStatisticReportSecuritySessionID,
    DssStatisticReportSecurityServerID,
    DssStatisticReportSecurityProjectID,
    DssStatisticReportSecurityFilterID,
    DssStatisticReportNewLastData,
    DssStatisticReportDetailModuleReserved = DssStatisticModuleReportDetail*100,
    DssStatisticReportDetailStepSequence,
    DssStatisticReportDetailSessionID,
    DssStatisticReportDetailServerID,
    DssStatisticReportDetailProjectID,
    DssStatisticReportDetailStepType,
    DssStatisticReportDetailStartTime,
    DssStatisticReportDetailFinishTime,
    DssStatisticReportDetailQueueTime,
    DssStatisticReportDetailCPUTime,
    DssStatisticReportDetailStepDuration,
    DssStatisticReportDetailSQLSessionID,
    DssStatisticReportDetailSQLServerID,
    DssStatisticReportDetailSQLProjectID,
    DssStatisticReportDetailSQLPassSequence,
    DssStatisticReportDetailSQLPassStartTime,
    DssStatisticReportDetailSQLPassFinishTime,
    DssStatisticReportDetailSQLPassExecTime,
    DssStatisticReportDetailSQLStatement,
    DssStatisticReportDetailSQLPassType,
    DssStatisticReportDetailSQLPassTotalTableAccessed,
    DssStatisticReportDetailDBErrorMessage,
    DssStatisticReportDetailColSessionID,
    DssStatisticReportDetailColServerID,
    DssStatisticReportDetailColProjectID,
    DssStatisticReportDetailColTableID,
    DssStatisticReportDetailColColumnID,
    DssStatisticReportDetailColColumnName,
    DssStatisticReportDetailColSQLClauseTypeID,
    DssStatisticReportDetailColCounter,
    DssStatisticReportDetailLastData,
    DssStatisticCacheModuleReserved = DssStatisticModuleCache*100,
    DssStatisticCacheIndex,
    DssStatisticCacheSessionID,
    DssStatisticCacheServerID,
    DssStatisticCacheProjectID,
    DssStatisticCacheHitTime,
    DssStatisticCacheHitType,
    DssStatisticCacheCreatorJobID,
    DssStatisticCacheCreatorSessionID,
    DssStatisticParentJobID,
    DssStatisticCacheLastData,
    DssStatisticDocumentModuleReserved = DssStatisticModuleDocument*100,
    DssStatisticDocumentServerID,
    DssStatisticDocumentSessionID,
    DssStatisticDocumentServerName,
    DssStatisticDocumentServerMachine,
    DssStatisticDocumentProjectID,
    DssStatisticDocumentUserID,
    DssStatisticDocumentDocumentID,
    DssStatisticDocumentRequestRecTime,
    DssStatisticDocumentRequestQueueTime,
    DssStatisticDocumentQueueTime,
    DssStatisticDocumentStartTime,
    DssStatisticDocumentFinishTime,
    DssStatisticDocumentJobFinishTime,
    DssStatisticDocumentExecStatus,
    DssStatisticDocumentExecErrorCode,
    DssStatisticDocumentReportNumber,
    DssStatisticDocumentCancelledIndicator,
    DssStatisticDocumentPromptIndicator,
    DssStatisticDocumentCachedIndicator,
    DssStatisticDocumentStepJobID,
    DssStatisticDocumentStepSequence,
    DssStatisticDocumentStepSessionID,
    DssStatisticDocumentStepServerID,
    DssStatisticDocumentStepProjectID,
    DssStatisticDocumentStepType,
    DssStatisticDocumentStepStartTime,
    DssStatisticDocumentStepFinishTime,
    DssStatisticDocumentStepQueueTime,
    DssStatisticDocumentStepCPUTime,
    DssStatisticDocumentStepDuration,
    DssStatisticDocumentLastData,
    DssStatisticProjSessSessionID =DssStatisticProjSessionTableID*100,
    DssStatisticProjSessServerID,
    DssStatisticProjSessServerName,
    DssStatisticProjSessServerMachine,
    DssStatisticProjSessUserID,
    DssStatisticProjSessProjectID,
    DssStatisticProjSessConnectTime,
    DssStatisticProjSessDisconnectTime,
    DssStatisticProjSessEventNotes,
    DssStatisticRepManipJobID = DssStatisticReportManipTableID*100,
    DssStatisticRepManipSessionID,
    DssStatisticRepManipServerID,
    DssStatisticRepManipProjectID,
    DssStatisticRepManipManipSequenceID,
    DssStatisticRepManipManipType,
    DssStatisticRepManipManipXML,
    DssStatisticRepManipStartTime,
    DssStatisticRepManipFinishTime,
    DssStatisticRepManipQueueTime,
    DssStatisticRepManipCPUTime,
    DssStatisticRepManipManipDuration,
    DssStatisticRepManipChildJobID,
    DssStatisticMemoryJobID =  DssStatisticMemoryTableID*100,
    DssStatisticMemorySessionID,
    DssStatisticMemoryServerID,
    DssStatisticMemoryProjectID,
    DssStatisticMemorySequenceID,
    DssStatisticMemoryMemoryAMT,
    DssStatisticMemoryFailureIndic,
    DssStatisticMemoryExecutionType,
    DssStatisticPrmptAnsTableReserved = DssStatisticPromptAnswersTableID*100,
    DssStatisticPrmptAnsJobID,
    DssStatisticPrmptAnsSessionID,
    DssStatisticPrmptAnsOrderID,
    DssStatisticPrmptAnsLocationGUID,
    DssStatisticPrmptAnsLocationType,
    DssStatisticPrmptAnsPromptAnswers,
    DssStatisticPrmptAnsAnswersType,
    DssStatisticPrmptAnsTitle,
    DssStatisticPrmptAnsName,
    DssStatisticPrmptAnsGUID,
    DssStatisticPrmptAnsLocationDesc,
    DssStatisticPrmptAnsIsRequired,
    DssStatisticPrmptAnsAnswersGUID,
    DssStatisticPrmptAnsIsDefault,
    DssStatisticPrmptAnsProjectID,
    DssStatisticPrmptAnsServerMachine,
    DssStatisticPrmptAnsServerID
} EnumDSSStatisticDataID;

typedef enum EnumDSSNotificationModuleID
{
    DssNotificationModuleIDReserved = 0,
    DssNotificationModuleFile = 1,
    DssNotificationModulePrint = 2,
    DssNotificationModuleHistoryList = 3,
    DssNotificationModuleEmail = 4,
    DssNotificationModuleMobile = 5,
    DssNotificationModuleCache = 6,
    DssNotificationLastModule = 6
} EnumDSSNotificationModuleID;

typedef enum EnumDSSNotificationDataID
{
    DssNotificationDataIDReserved = 0,
    DssNotificationFileModuleReserved = DssNotificationModuleFile*100,
    DssNotificationFileEnableEmail,
    DssNotificationFileRecipientName,
    DssNotificationFileOnwerName,
    DssNotificationFileReportOrDocumentName,
    DssNotificationFileProjectName,
    DssNotificationFileDeliveryMethod,
    DssNotificationFileSchedule,
    DssNotificationFileDeliveryStatus,
    DssNotificationFileDate,
    DssNotificationFileTime,
    DssNotificationFileFileLocation,
    DssNotificationFileLinkToFile,
    DssNotificationFileErrorMessage,
    DssNotificationFileAppendText,
    DssNotificationFileTextToAppend,
    DssNotificationFileOnlySendSuccessToRecipient,
    DssNotificationFileSendFailureToEmailAddress,
    DssNotificationFileSubscriptionName,
    DssNotificationFileLastData,
    DssNotificationPrintModuleReserved = DssNotificationModulePrint*100,
    DssNotificationPrintEnableEmail,
    DssNotificationPrintRecipientName,
    DssNotificationPrintOnwerName,
    DssNotificationPrintReportOrDocumentName,
    DssNotificationPrintProjectName,
    DssNotificationPrintDeliveryMethod,
    DssNotificationPrintSchedule,
    DssNotificationPrintDeliveryStatus,
    DssNotificationPrintDate,
    DssNotificationPrintTime,
    DssNotificationPrintPrinterName,
    DssNotificationPrintErrorMessage,
    DssNotificationPrintAppendText,
    DssNotificationPrintTextToAppend,
    DssNotificationPrintOnlySendSuccessToRecipient,
    DssNotificationPrintSendFailureToEmailAddress,
    DssNotificationPrintSubscriptionName,
    DssNotificationPrintLastData,
    DssNotificationHistoryListModuleReserved = DssNotificationModuleHistoryList*100,
    DssNotificationHistoryListEnableEmail,
    DssNotificationHistoryListRecipientName,
    DssNotificationHistoryListOnwerName,
    DssNotificationHistoryListReportOrDocumentName,
    DssNotificationHistoryListProjectName,
    DssNotificationHistoryListDeliveryMethod,
    DssNotificationHistoryListSchedule,
    DssNotificationHistoryListDeliveryStatus,
    DssNotificationHistoryListDate,
    DssNotificationHistoryListTime,
    DssNotificationHistoryListLinkToHistoryList,
    DssNotificationHistoryListErrorMessage,
    DssNotificationHistoryListAppendText,
    DssNotificationHistoryListTextToAppend,
    DssNotificationHistoryListOnlySendSuccessToRecipient,
    DssNotificationHistoryListSendFailureToEmailAddress,
    DssNotificationHistoryListSubscriptionName,
    DssNotificationHistoryListLastData,
    DssNotificationEmailModuleReserved = DssNotificationModuleEmail*100,
    DssNotificationEmailEnableEmail,
    DssNotificationEmailRecipientName,
    DssNotificationEmailOnwerName,
    DssNotificationEmailReportOrDocumentName,
    DssNotificationEmailProjectName,
    DssNotificationEmailDeliveryMethod,
    DssNotificationEmailSchedule,
    DssNotificationEmailDeliveryStatus,
    DssNotificationEmailDate,
    DssNotificationEmailTime,
    DssNotificationEmailEmailAddress,
    DssNotificationEmailErrorMessage,
    DssNotificationEmailAppendText,
    DssNotificationEmailTextToAppend,
    DssNotificationEmailSendFailureToEmailAddress,
    DssNotificationEmailSubscriptionName,
    DssNotificationEmailLastData,
    DssNotificationMobileModuleReserved = DssNotificationModuleMobile*100,
    DssNotificationMobileEnableEmail,
    DssNotificationMobileRecipientName,
    DssNotificationMobileOnwerName,
    DssNotificationMobileReportOrDocumentName,
    DssNotificationMobileProjectName,
    DssNotificationMobileDeliveryMethod,
    DssNotificationMobileSchedule,
    DssNotificationMobileDeliveryStatus,
    DssNotificationMobileDate,
    DssNotificationMobileTime,
    DssNotificationMobileErrorMessage,
    DssNotificationMobileAppendText,
    DssNotificationMobileTextToAppend,
    DssNotificationMobileSendFailureToEmailAddress,
    DssNotificationMobileSubscriptionName,
    DssNotificationMobileLastData,
    DssNotificationCacheModuleReserved = DssNotificationModuleCache*100,
    DssNotificationCacheEnableEmail,
    DssNotificationCacheRecipientName,
    DssNotificationCacheOnwerName,
    DssNotificationCacheReportOrDocumentName,
    DssNotificationCacheProjectName,
    DssNotificationCacheDeliveryMethod,
    DssNotificationCacheSchedule,
    DssNotificationCacheDeliveryStatus,
    DssNotificationCacheDate,
    DssNotificationCacheTime,
    DssNotificationCacheEmailAddress,
    DssNotificationCacheErrorMessage,
    DssNotificationCacheAppendText,
    DssNotificationCacheTextToAppend,
    DssNotificationCacheSendFailureToEmailAddress,
    DssNotificationCacheSubscriptionName,
    DssNotificationCacheLastData
} EnumDSSNotificationDataID;

typedef enum EnumDSSServerSettingID
{
    DssServerMaxJobsPerServer = 0,
    DssServerMaxUserConnectionPerServer,
    DssServerBackupPeriodicity,
    DssServerBackupDirectory,
    DssServerMSIPerformanceMonitoringFlag,
    DssServerStartupDiagnosticsFlag,
    DssServerMSILockCheckingFlag,
    DssServerBackupIgnoreFlag,
    DssServerSchedulerFlag,
    DssServerObjectMaxCacheCount = 9,
    DssServerObjectMaxCacheMemoryConsumption = 10,
    DssServerObjectCacheTableBucketCount = 11,
    DssServerObjectCacheTableBucketPerLock = 12,
    DssServerObjectFileCachePath = 13,
    DssServerObjectSwapPolicy = 14,
    DssServerObjectCacheLifeTime = 15,
    DssServerInboxFilePath = 16,
    DssServerMaxUserConnectionIdleTime = 17,
    DssServerMaxInboxMsgCountPerUser = 18,
    DssServerDropTempTableFlag = 19,
    DssServerConnectionLimit = 20,
    DssServerSynchCallTimeout = 21,
    DssServerMaxXmlNodes = 22,
    DssServerLoadBalanceMode = 23,
    DssServerThreadIdleTimeOut = 24,
    DssServerMaxThreadPerCPU = 25,
    DssServerMinBalanceInterval = 26,
    DssServerMaxLoadDeviation = 27,
    DssServerMaxMemoryUsage = 28,
    DssServerMinFreeMemory = 29,
    DssServerClusterIPOption = 30,
    DssServerClusterGroupAddress = 31,
    DssServerDefaultProjectMaxResultRowCount = 32,
    DssServerDefaultProjectMaxReportExecutionTime = 33,
    DssServerDefaultProjectMaxMessageRetentionTime = 34,
    DssServerDefaultProjectMaxJobPerUserAccount = 35,
    DssServerDefaultProjectMaxJobPerUserConnection = 36,
    DssServerDefaultProjectMaxJobPerProject = 37,
    DssServerDefaultProjectMaxUserConnectionPerProject = 38,
    DssServerDefaultProjectCollectStatisticsFlag = 39,
    DssServerDefaultProjectLoadFlag = 40,
    DssServerDefaultProjectMaxReportCacheMemConsumption = 41,
    DssServerDefaultProjectMaxReportCacheCount = 42,
    DssServerDefaultProjectMaxReportCacheDiskSpace = 43,
    DssServerDefaultProjectReportCacheLookupTableSize = 44,
    DssServerDefaultProjectMaxObjectCacheMemConsumption = 45,
    DssServerDefaultProjectMaxObjectCacheCount = 46,
    DssServerDefaultProjectMaxElementCacheMemConsumption = 47,
    DssServerDefaultProjectMaxElementCacheCount = 48,
    DssServerDefaultProjectElementCacheLookupTableSize = 49,
    DssServerDefaultProjectObjectCacheTableBucketPerLock = 50,
    DssServerDefaultProjectObjectFileCachePath = 51,
    DssServerDefaultProjectObjectSwapPolicy = 52,
    DssServerDefaultProjectObjectCacheLifeTime = 53,
    DssServerDefaultProjectElementCacheTableBucketPerLock = 54,
    DssServerDefaultProjectElementFileCachePath = 55,
    DssServerDefaultProjectElementSwapPolicy = 56,
    DssServerDefaultProjectElementCacheLifeTime = 57,
    DssServerDefaultProjectReportCacheTableBucketPerLock = 58,
    DssServerDefaultProjectReportFileCachePath = 59,
    DssServerDefaultProjectReportSwapPolicy = 60,
    DssServerDefaultProjectReportCacheLifeTime = 61,
    DssServerDefaultProjectDisableReportCaching = 62,
    DssServerDefaultProjectMaxAERowCount = 63,
    DssServerDefaultProjectMaxElementRowCount = 64,
    DssServerDefaultProjectReportCacheDBMatchingFlag = 65,
    DssServerDefaultProjectLinkCacheMaxMemConsumption = 66,
    DssServerDefaultProjectLinkCacheTableBucketCount = 67,
    DssServerDefaultProjectLinkCacheTableBucketPerLock = 68,
    DssServerDefaultProjectLinkCacheReplacementPolicy = 69,
    DssServerMaxWebConnectionIdleTime = 70,
    DssServerMaxXmlDrillPaths = 71,
    DssServerAuthSettings = 72,
    DssServerClusterPort = 73,
    DssServerMaxUsedVirtualByte = 74,
    DssServerMaxUsedPrivateByte = 75,
    DssServerMaxContractLimitAbsolute = 76,
    DssServerMaxContractLimitPercentage = 77,
    DssServerMemoryReserve = 78,
    DssServerMemoryReservePercentage = 79,
    DssServerMemoryRequestIdleTime = 80,
    DssServerMCMWatermark = 81,
    DssServerStatsLogging2AllProjects = 82,
    DssServerStatsLogging2ProjectGUID = 83,
    DssServerVirtualByteLimitPercent = 84,
    DssServerWSRPBucketCount = 85,
    DssServerWSRPMaxResultCount = 86,
    DssServerWSRPMaxMemoryConsumption = 87,
    DssServerWSRPSwapPath = 88,
    DssServerDefaultProjectReportCacheLoadOnStart = 89,
    DssServerCacheCleanUpFrequency = 90,
    DssServerMaxInboxMsgLifeTime = 91,
    DssServerDefaultLicenseComplianceCheckTime = 92,
    DssServerMaxSchedulerConnectionIdleTime = 93,
    DssServerMSIPersistPerformanceCounter = 94,
    DssServerMSIPersistPerformanceCounterFrequency = 95,
    DssServerMSIPersistPerformanceCounterList = 96,
    DssServerLicenseComplianceInfo = 97,
    DssServerPDFMaxMemoryConsumption = 98,
    DssServerXMLMaxMemoryConsumption = 99,
    DssServerExcelMaxMemoryConsumption = 100,
    DssServerProjectLevelClustering = 101,
    DssServerProjectLevelClusteringPolicyProjectMapping = 102,
    DssServerProjectLevelClusteringPolicyProjectReduncency = 103,
    DssServerProjectFailoverLatency = 104,
    DssServerProjectMigrationLatency = 105,
    DssServerMaxCommandsInQueueLimit = 106,
    DssServerClusteringStartupMembership = 107,
    DssServerCSDBRole = 108,
    DssServerHLRepositoryType = 109,
    DssServerNCSCSDBRole = 110,
    DssServerMaxInteractiveJobsPerServer = 111,
    DssServerAuthSettings2 = 112,
    DssServerMaxLoginAttempt = 113,
    DssServerClusteringLoadBalancingFactor = 114,
    DssServerHLAutoDeleteMsgCount = 115,
    DssServerHLAutoDeleteErrorMsgFirst = 116,
    DssServerHLAutoDeleteTimestamp = 117,
    DssServerIgnoreGovernorOnChild = 118,
    DssServerAuthSettings3 = 119,
    DssServerMemoryReserveHigh32 = 120,
    DssServerDefaultProjectCubeStorage = 121,
    DssServerHtmlMaxMemoryConsumption = 122,
    DssServerEnableReportCacheBackup = 123,
    DssServerUserAffinityFlag = 124,
    DssServerUserAffinityLoadBalancingFlag = 125,
    DssServerHybridCentralInboxFilePath = 126,
    DssServerIncludeLDAPUsersForAuditoringFlag = 127,
    DssServerAutoStartSearchEngine = 128,
    DssServerCatalogMaxMemoryConsumption = 129,
    DssServerDefLastSetting
} EnumDSSServerSettingID;

typedef enum EnumDSSServerClusteringIPOption
{
    DssServerClusteringTCP = 1,
    DssServerClusteringUDPUnicast = 4,
    DssServerClusteringUDPMulticast = 5
} EnumDSSServerClusteringIPOption;

typedef enum EnumDSSUserAccountChangeFlags
{
    DssUserAccountChangeReserved = 0x00000000,
    DssUserAccountChangePassword = 0x00000001,
    DssUserAccountChangePrivileges = 0x00000002,
    DssUserAccountChangeMembership = 0x00000004,
    DssUserAccountChangeExternalAccounts = 0x00000008,
    DssUserAccountChangeEnabled = 0x00000010,
    DssUserAccountChangeAll = 0x0000001f
} EnumDSSUserAccountChangeFlags;

typedef enum EnumDSSAuthInfo_Type
{
    DssAuthInfoAuthMode = 0,
    DssAuthInfoLogin,
    DssAuthInfoServer,
    DssAuthInfoPort,
    DssAuthInfoLastOne
} EnumDSSAuthInfo_Type;

typedef enum EnumDSSLoginInternalUserFlags
{
    DssLoginInternalUser_DoNotSearchLDAPGroups =	0x01
} EnumDSSLoginInternalUserFlags;

typedef enum EnumDSSLDAPServerSecureConnection
{
    DssLDAPServerSecureConnectionPlainText,
    DssLDAPServerSecureConnectionSSL
} EnumDSSLDAPServerSecureConnection;

typedef enum EnumDSSLDAPImport
{
    DssLDAPImportNotImport,
    DssLDAPImportImport,
    DssLDAPImportForceNotImport,
    DssLDAPImportForceImport
} EnumDSSLDAPImport;

typedef enum EnumDSSLDAPVendor
{
    DssLDAPVendorOthers,
    DssLDAPVendorNovell,
    DssLDAPVendorADS,
    DssLDAPVendorSunOne,
    DssLDAPVendoriPlanet = DssLDAPVendorSunOne,
    DssLDAPVendorIBM,
    DssLDAPVendorOpenLDAP,
    DssLDAPVendorHPUX,
    DssLDAPVendorLatest
} EnumDSSLDAPVendor;

typedef enum EnumDSSLDAPVersion
{
    DssLDAPVersionV3
} EnumDSSLDAPVersion;

typedef enum EnumDSSLDAPSynch
{
    DssLDAPSynchReserved = 0,
    DssLDAPSynchUser = 0x01,
    DssLDAPSynchGroup = 0x02,
    DssLDAPSynchAll = DssLDAPSynchUser  | DssLDAPSynchGroup
} EnumDSSLDAPSynch;

typedef enum EnumDSSLDAPStringCoding
{
    DssLDAPStringCodingUTF8,
    DssLDAPStringCodingMultiByte
} EnumDSSLDAPStringCoding;

typedef enum EnumDSSLDAPCallConvention
{
    DssLDAPCallConventionReserved,
    DssLDAPCallConventionCDecl,
    DssLDAPCallConventionStdCall,
    DssLDAPCallConventionUnicode
} EnumDSSLDAPCallConvention;

typedef enum EnumDSSLDAPPlatform
{
    DssLDAPPlatformReserved,
    DssLDAPPlatformWindows,
    DssLDAPPlatformSolaris,
    DssLDAPPlatformUnix = DssLDAPPlatformSolaris,
    DssLDAPPlatformAIX,
    DssLDAPPlatformLinux,
    DssLDAPPlatformHPUX,
    DssLDAPPlatformLastone
} EnumDSSLDAPPlatform;

typedef enum EnumDSSLDAPVerification
{
    DssLDAPVerificationBind,
    DssLDAPVerificationPasswordCompare
} EnumDSSLDAPVerification;

typedef enum EnumDSSTrustWebServerStatus
{
    DssTrustWebServerStatusDefault = 0,
    DssTrustWebServerStatusEnabled = 0x01,
    DssTrustWebServerStatusRemoved = 0x8000
} EnumDSSTrustWebServerStatus;

typedef enum EnumDSSLicenseScheme
{
    DssLicenseSchemeLatest = -1,
    DssLicenseScheme0701xx = 1,
    DssLicenseScheme070202 = 2,
    DssLicenseScheme070203 = 3,
    DssLicenseScheme070204 = 4,
    DssLicenseScheme070501 = 5,
    DssLicenseScheme080000 = 6,
    DssLicenseScheme080002 = 7,
    DssLicenseScheme090000 = 8,
    DssLicenseScheme090001m = 9
} EnumDSSLicenseScheme;

typedef enum EnumDSSLicenseTypes
{
    DssLicenseTypeNone = 0,
    DssLicenseTypeWebReporterBit = 0x00000001,
    DssLicenseTypeWebAnalystBit = 0x00000002,
    DssLicenseTypeWebProfessionalBit = 0x00000004,
    DssLicenseTypeDesktopAnalystBit = 0x00000008,
    DssLicenseTypeDesktopDesignerBit = 0x00000010,
    DssLicenseTypeArchitectBit = 0x00000020,
    DssLicenseTypeAdministratorBit = 0x00000040,
    DssLicenseTypeGutenbergBit = 0x00000080,
    DssLicenseTypeOfficeIntelligenceBit = 0x00000100,
    DssLicenseTypeOLAPServicesBit = 0x00000200,
    DssLicenseTypeWebMultimediaTrainingBit = 0x00000400,
    DssLicenseTypeWebReportClientBundleBit = 0x00000800,
    DssLicenseTypeMobileBit = 0x00001000,
    DssLicenseTypeIntegrityManagerBit = 0x00002000,
    DssLicenseTypeMultiSourceBit = 0x00004000,
    DssLicenseTypeDistributionBit = 0x00008000,
    DssLicenseTypeCommandManagerBit = 0x00010000,
    DssLicenseTypeEnterpriseManagerBit = 0x00020000,
    DssLicenseTypeObjectManagerBit = 0x00040000,
    DssLicenseTypeTransactionServicesBit = 0x00080000
} EnumDSSLicenseTypes;

typedef enum EnumDSSLicenseType
{
    DssLicenseTypeReserved = 0,
    DssLicenseTypeWebReporter = 1,
    DssLicenseTypeWebAnalyst = 2,
    DssLicenseTypeWebProfessional = 3,
    DssLicenseTypeDesktopAnalyst = 4,
    DssLicenseTypeDesktopDesigner = 5,
    DssLicenseTypeArchitect = 6,
    DssLicenseTypeAdministrator = 7,
    DssLicenseTypeGutenberg = 8,
    DssLicenseTypeOfficeIntelligence = 9,
    DssLicenseTypeOLAPServices = 10,
    DssLicenseTypeWebMultimediaTraining = 11,
    DssLicenseTypeWebReportClientBundle = 12,
    DssLicenseTypeMobile = 13,
    DssLicenseTypeIntegrityManager = 14,
    DssLicenseTypeMultiSource = 15,
    DssLicenseTypeDistribution = 16,
    DssLicenseTypeCommandManager = 17,
    DssLicenseTypeEnterpriseManager = 18,
    DssLicenseTypeObjectManager = 19,
    DssLicenseTypeTransactionServices = 20,
    DssLicenseTypeCount = 20
} EnumDSSLicenseType;

typedef enum EnumAuditorTypes
{
    DssAuditorDefault = 0x00000000,
    DssAuditorIncludeLDAPGroups = 0x00000001
} EnumAuditorTypes;

typedef enum EnumDSSPasswordPolicy
{
    DssPasswordPolicyMininumLength = 0,
    DssPasswordPolicyMinimumLength = 0,
    DssPasswordPolicyMinimumChars = 1,
    DssPasswordPolicyMinimumUpperCases = 2,
    DssPasswordPolicyMinimumLowerCases = 3,
    DssPasswordPolicyMinimumNumbers = 4,
    DssPasswordPolicyMinimumSpecials = 5,
    DssPasswordPolicyHistoryCount = 6,
    DssPasswordPolicyLastOne
} EnumDSSPasswordPolicy;

typedef enum EnumDSSSecurityPolicy
{
    DssSecurityPolicyPersistLoginCredential = 0,
    DssSecurityPolicyLastOne
} EnumDSSSecurityPolicy;

typedef enum EnumDSSSecurityPersistLoginCredential
{
    DssSecurityPersistLoginCredentialNone = 0,
    DssSecurityPersistLoginCredentialWH = 1,
    DssSecurityPersistLoginCredentialLDAP = 2,
    DssSecurityPersistLoginCredentialLastOne
} EnumDSSSecurityPersistLoginCredential;

typedef enum EnumDSSSourceManipulatorFlags
{
    DssSourceManipulatorDefault = 0x00000000,
    DssSourceManipulatorNoObjectLocks = 0x00000001,
    DssSourceManipulatorChangeModificationTime = 0x00000002,
    DssSourceManipulatorValidateOnClose = 0x00000020,
    DssSourceManipulatorMergeTranslations = 0x00000040,
    DssSourceManipulatorMergeTranslationsReplace = 0x00000080,
    DssSourceManipulatorAdjustActualPrimaryTranslations = 0x00000100,
    DssSourceManipulatorExpectNoActionOnMerge = 0x00000200,
    DssSourceManipulatorNoLogUnchanged = 0x00000400,
    DssSourceManipulatorDeferLoadFromSource = 0x00000800,
    DssSourceManipulatorDetectPreOrionAutoStyles = 0x00001000,
    DssSourceManipulatorNotPurgeClientCache = 0x00002000,
    DssSourceManipulatorIncludeManifest = 0x00004000,
    DssSourceManipulatorMergeToObjectContext = 0x00008000,
    DssSourceManipulatorClearMonPrimaryNameTranslationsOnKeepBoth = 0x00010000,
    DssSourceManipulatorLogOnly = 0x10000000,
    DssSourceManipulatorNoValidation = 0x20000000,
    DssSourceManipulatorTryReplaceOnNameClash = 0x40000000,
    DssSourceManipulatorMigrateTranslationsFromBaseLocales = 0x80000000
} EnumDSSSourceManipulatorFlags;

typedef struct StructDSSUserAuditResult
{
    wchar_t mFullName[256];
    wchar_t mLoginName[256];
    EnumDSSLicenseTypes mLicenseTypes;
} StructDSSUserAuditResult;

typedef enum EnumDSSAuditUserFilter
{
    DssAuditReserved = 0x00000000,
    DssAuditEnabledUsers = 0x00000001,
    DssAuditDisabledUsers = 0x00000002,
    DssAuditIgnoreEnabledFlag = 0x00000003
} EnumDSSAuditUserFilter;

typedef enum EnumDSSServerSummary
{
    DssServerSummaryReserved = 0,
    DssServerSummaryAverageReportQueueTime,
    DssServerSummaryAverageReportResponseTime,
    DssServerSummaryOpenReports,
    DssServerSummaryExecutingReports,
    DssServerSummaryConnectedUserSessions,
    DssServerSummaryOpenDatabaseConnections,
    DssServerSummaryActiveDatabaseConnections
} EnumDSSServerSummary;

typedef enum EnumDSSNTCounter
{
    DssServerNTCounterReserved = 0,
    DssServerNTCounterHandleCount,
    DssServerNTCounterThreadCount,
    DssServerNTCounterPrivateBytes,
    DssServerNTCounterWorkingSet,
    DssServerNTCounterVirtualBytes,
    DssServerNTCounterProcessorTime,
    DssServerNTCounterNonPagedPoolByte,
    DssServerNTCounterPagedPoolByte
} EnumDSSNTCounter;

typedef enum EnumDSSServerMemMisc
{
    DssServerDSSBufferReserved = 0,
    DssServerDSSBufferNumber,
    DssServerDSSBufferSize,
    DssServerPrivateCommitted,
    DssServerPrivateReserved,
    DssServerImageCommitted,
    DssServerImageReserved,
    DssServerMappedCommitted,
    DssServerMappedReserved,
    DssServertotalFree,
    DssServertotalPrivate,
    DssServertotalReserved,
    DssServertotalImage,
    DssServertotalCommitted,
    DssServertotalMapped,
    DssServerProcessHeapSize,
    DssServerCRTHeapSize
} EnumDSSServerMemMisc;

typedef enum EnumDSSServerObjectCategory
{
    DssServerObjCatReportInstance,
    DssServerObjCatUserSession,
    DssServerObjCatXMLDocument,
    DssServerObjCatXMLNode,
    DssServerObjCatJob,
    DssServerObjCatInboxMsg,
    DssServerObjCatCDSSMsg,
    DssServerObjCatJobTask,
    DssServerObjCatSQLs,
    DssServerObjCatXMLContext,
    DssServerObjCatDocumentInstance,
    DssServerObjCatProcessContext,
    DssServerObjCatCommand,
    DssServerObjCatResoutionWorkspace,
    GC_LAST_OBJECT_CAT_COUNT = DssServerObjCatResoutionWorkspace
} EnumDSSServerObjectCategory;

typedef enum EnumDSSAllocationOutputMedium
{
    DssOutputMediumPrompt = 0x0001,
    DssOutputMediumConsole = 0x0002,
    DssOutputMediumBeep = 0x0004,
    DssOutputMediumFile = 0x0010,
    DssOutputMediumFileAppend = 0x0020,
    DssOutputMediumInMessage = 0x0040,
    DssOutputMediumSpecial = 0x8000
} EnumDSSAllocationOutputMedium;

typedef enum EnumDSSServerStatus
{
    DssServerReserved = 0x00,
    DssServerReady,
    DssServerNotAvailable
} EnumDSSServerStatus;

typedef enum EnumDSSServerStopOption
{
    DssServerStopNormal = 0,
    DssServerStopImmediately,
    DssServerStopAbort
} EnumDSSServerStopOption;

typedef enum EnumDSSServerIdleOption
{
    DssServerIdleNormal = 0
} EnumDSSServerIdleOption;

typedef enum EnumIDSSServerResumeOption
{
    DSSServerResumeNormal = 0
} EnumIDSSServerResumeOption;

typedef enum EnumIDSSServerBackupOption
{
    DSSServerBackupNormal = 0
} EnumIDSSServerBackupOption;

typedef enum EnumIDSSServerRestoreOption
{
    DSSServerRestoreNormal = 0
} EnumIDSSServerRestoreOption;

typedef enum EnumDSSServerDiagnosticsFlag
{
    DssDiagnosticsUser = 0x0,
    DssDiagnosticsSystem
} EnumDSSServerDiagnosticsFlag;

typedef enum EnumDSSUserPrivilegesFlag
{
    DssUserPrivilegesConfigLevel = 0x01,
    DssUserPrivilegesProjectLevel = 0x02
} EnumDSSUserPrivilegesFlag;

typedef enum EnumDSSLDAPBatchImport
{
    DssLDAPBatchImportReserved = 0,
    DssLDAPBatchImportDoNotSynchUser = 0x01,
    DssLDAPBatchImportDoNotSynchGroup = 0x02,
    DssLDAPBatchImportDoNotImportUser = 0x04,
    DssLDAPBatchImportDoNotImportGroup = 0x08,
    DssLDAPBatchImportWithSearchFilters = 0x10
} EnumDSSLDAPBatchImport;

typedef enum EnumDSSLDAPBatchImportStatus
{
    DssLDAPBatchImportStatusReserved = 0,
    DssLDAPBatchImportStatusFinished = 1,
    DssLDAPBatchImportStatusImporting = 2,
    DssLDAPBatchImportStatusCanceled = 3,
    DssLDAPBatchImportStatusError = 4
} EnumDSSLDAPBatchImportStatus;

typedef enum EnumDSSGetHLMsgsSubCommands
{
    DssServerGetHLMsgsReserve = -1,
    DssServerGetUserHL = 0,
    DssServerGetHLMsgs = 1,
    DssServerDeleteHLMsgsForScheduler = 2,
    DssServerCleanupAllMsgForUser = 3
} EnumDSSGetHLMsgsSubCommands;

typedef enum EnumDSSWebServerStatus
{
    DssWebServerOpen,
    DssWebServerClose,
    DssWebServerNotAvailable
} EnumDSSWebServerStatus;

typedef enum EnumDSSWebConnectionStatus
{
    DssWebConnectionIdle,
    DssWebConnectionBusy,
    DssWebConnectionError
} EnumDSSWebConnectionStatus;

typedef enum EnumDSSMergeTranslations
{
    DssMergeTranslationsDefault = 0x00000000,
    DssMergeTranslationsFromOther = 0x00000001,
    DssMergeTranslationsEntireObject = 0x00000002,
    DssMergeTranslationsNullify = 0x00000004,
    DssMergeTranslationsOverWrite = 0x00000008,
    DssMergeTranslationsKeep = 0x00000010,
    DssMergeTranslationReferencedManagedFirstClassObjects =  0x00000020
} EnumDSSMergeTranslations;

typedef enum EnumDSSDimensionExpansionDepth
{
    DssDimensionExpansionDepthDefault = -1,
    DssDimensionExpansionDepthAll = -2,
    DssDimensionExpansionDepthMin = -2,
    DssDimensionExpansionDepthMax = 1000
} EnumDSSDimensionExpansionDepth;

typedef enum EnumDSSDimensionInfosSnapshotFlags
{
    DssDimensionInfosSnapshotReserved = 0,
    DssDimensionInfosSnapshotDimensions = 0x0001,
    DssDimensionInfosSnapshotHierarchies = 0x0002,
    DssDimensionInfosSnapshotDoNotLogError = 0x8000
} EnumDSSDimensionInfosSnapshotFlags;

typedef enum EnumDSSTableExtraInformation
{
    DssTableExtraInformationDefault = 0,
    DssTableExtraInformationNotForHDAMDX = 0x0001,
    DssTableExtraInformationHasNonOptionalSAPVariables = 0x0002
} EnumDSSTableExtraInformation;

typedef enum EnumDSSGeographicalRole
{
    DssGeographicalRoleUndefined = -1,
    DssGeographicalRoleNone = 0,
    DssGeographicalRoleCity = 1,
    DssGeographicalRoleState = 2,
    DssGeographicalRoleCountry = 3,
    DssGeographicalRoleLocation = 4,
    DssGeographicalRoleLatitude = 5,
    DssGeographicalRoleLongitude = 6,
    DssGeographicalRoleOther = 7,
    DssGeographicalRoleZipCode = 8,
    DssGeographicalRoleLastOne = 9
} EnumDSSGeographicalRole;

typedef enum EnumDSSAggregateFromBaseStyle
{
    DssAggregateFromBaseStyleReserved = 0,
    DssAggregateFromBaseStyleNone = 1,
    DssAggregateFromBaseStyleTotalFromMetric = 2,
    DssAggregateFromBaseStyleIndividual = 3
} EnumDSSAggregateFromBaseStyle;

typedef enum EnumDSSDITransformationType
{
    DssDITransformationReserved = 0,
    DssDITransformationUnCrossTab = 1
} EnumDSSDITransformationType;

typedef enum EnumDSSFileStructInfo
{
    DssFileStructDefault = 0,
    DssFileStructCrossTabbed = 1
} EnumDSSFileStructInfo;

typedef enum EnumDSSDITransformationFlags
{
    DssDITransformationFlagDefault = 0,
    DssDITransformationFlagDoNotApply = 1
} EnumDSSDITransformationFlags;

typedef enum EnumDSSDataImportColumnDeriveFlags
{
    DssDataImportColumnDeriveFlagsDefault = 0,
    DssDataImportColumnDeriveYear = 0x00000001,
    DssDataImportColumnDeriveQuarter = 0x00000002,
    DssDataImportColumnDeriveQuarterOfYear = 0x00000004,
    DssDataImportColumnDeriveMonth = 0x00000008,
    DssDataImportColumnDeriveMonthOfYear = 0x00000010,
    DssDataImportColumnDeriveWeek = 0x00000020,
    DssDataImportColumnDeriveWeekOfYear = 0x00000040,
    DssDataImportColumnDeriveDate = 0x00000080,
    DssDataImportColumnDeriveDayOfMonth = 0x00000100,
    DssDataImportColumnDeriveDayOfWeek = 0x00000200,
    DssDataImportColumnDeriveHour = 0x00000400,
    DssDataImportColumnDeriveMinute = 0x00000800,
    DssDataImportColumnDeriveSecond = 0x00001000,
    DssDataImportColumnDeriveDateTimeMask = 0x00001FFF,
    DssDataImportColumnDeriveDateTimeFirst = DssDataImportColumnDeriveYear,
    DssDataImportColumnDeriveDateTimeLast = DssDataImportColumnDeriveSecond,
    DssDataImportColumnDeriveCountry = 0x10000000,
    DssDataImportColumnDeriveState = 0x20000000,
    DssDataImportColumnDeriveCity = 0x40000000,
    DssDataImportColumnDeriveGeoMask = 0x70000000,
    DssDataImportColumnDeriveGeoFirst = DssDataImportColumnDeriveCity,
    DssDataImportColumnDeriveGeoLast = DssDataImportColumnDeriveCountry,
    DssDataImportColumnDeriveFlagIsDerived = 0x80000000,
    DssDataImportColumnDeriveFlagsUndefined = 0xFFFFFFFF
} EnumDSSDataImportColumnDeriveFlags;

typedef enum EnumDSSTerseIDFlags
{
    DssTerseIDDefault = 0,
    DssTerseIDSourceUnitID = 0x00000001,
    DssTerseIDDisplayName = 0x00000002,
    DssTerseIDSourceUnitIDDisplayName = 0x00000003,
    DssTerseIDConsElContainer = 0x80000000,
    DssTerseIDConsElContainerSourceUnitID = 0x80000001
} EnumDSSTerseIDFlags;

typedef struct DSSMDSYSOBJECTINF
{
    DSS_ID ObjectID;
    EnumDSSObjectType Type;
    wchar_t szName[256];
    DSS_ID ConfigID;
} DSSMDSYSOBJECTINF;

typedef enum DSSMsgProcessor_Errors
{
    DSSMSGPROCESSOR_E_NOTCACHED = 0x80041600,
    DSSMGSPROCESSOR_E_MSGINVALID = 0x80041601
} DSSMsgProcessor_Errors;

typedef BYTE DSSData_Flags;

//typedef enum EnumDSSFunctionType
//{
//    DssFunctionTypeReserved = 0,
//    DssFunctionTypeGeneric = 1,
//    DssFunctionTypeSimple = 2,
//    DssFunctionTypeAggregation = 3,
//    DssFunctionTypeRelative = 4,
//    DssFunctionTypeIndex = 5,
//    DssFunctionTypeQualify = 6,
//    DssFunctionTypeVVToS = 7
//} EnumDSSFunctionType;

typedef enum EnumDSSPatternType
{
    DssPatternTypeReserved = 0,
    DssPatternTypeNumeric = 1
} EnumDSSPatternType;

//typedef enum EnumDSSParameterType
//{
//    DssParameterReserved = 0x00,
//    DssParameterScalar = 0x01,
//    DssParameterVector = 0x02
//} EnumDSSParameterType;

typedef enum EnumDSSComparisonOptions
{
    DssRelationalFloor = 1,
    DssRelationalCeiling = 2,
    DssRelationalRound = 3,
    DssRelationalOpen = 4,
    DssRelationalCloseFloor = 5,
    DssRelationalCloseCeiling = 6,
    DssRelationalCloseRound = 7
} EnumDSSComparisonOptions;

//typedef enum EnumDSSFunctionSQLType
//{
//    DssFunctionSQLTypeReserved = 0,
//    DssFunctionSQLTypeAggregation = 1,
//    DssFunctionSQLTypeLogic = 2,
//    DssFunctionSQLTypeArithmetic = 3,
//    DssFunctionSQLTypeComparison = 4,
//    DssFunctionSQLTypeRelative = 5
//} EnumDSSFunctionSQLType;

typedef enum EnumDSSFunctionSQLSubType
{
    DssFunctionSQLSubTypeCustom = -1,
    DssFunctionSQLSubTypeReserved = 0,
    DssFunctionSQLSubTypeAnd = 1,
    DssFunctionSQLSubTypeOr = 2,
    DssFunctionSQLSubTypeNot = 3,
    DssFunctionSQLSubTypeSum = 4,
    DssFunctionSQLSubTypeCount = 5,
    DssFunctionSQLSubTypeMax = 6,
    DssFunctionSQLSubTypeMin = 7,
    DssFunctionSQLSubTypeIn = 8,
    DssFunctionSQLSubTypeEqual = 9,
    DssFunctionSQLSubTypeIntersect = 10,
    DssFunctionSQLSubTypeIntersectIn = 11,
    DssFunctionSQLSubTypeApplySimple = 12,
    DssFunctionSQLSubTypeApplyAggregation = 13,
    DssFunctionSQLSubTypeApplyLogic = 14,
    DssFunctionSQLSubTypeApplyComparison = 15,
    DssFunctionSQLSubTypeApplyRelative = 16,
    DssFunctionSQLSubTypePatternMatch = 17,
    DssFunctionSQLSubTypeAvg = 18,
    DSSFunctionSQLSubTypeUnion = 19,
    DSSFunctionSQLSubTypeExcept = 20,
    DSSFunctionSQLSubTypeString = 21
} EnumDSSFunctionSQLSubType;

typedef enum EnumDSSFuncPrecedence
{
    DssFuncPrecedenceNotApplicable = 0,
    DssFuncPrecedence10 = 10,
    DssFuncPrecedence20 = 20,
    DssFuncPrecedence30 = 30,
    DssFuncPrecedence40 = 40,
    DssFuncPrecedence45 = 45,
    DssFuncPrecedence50 = 50,
    DssFuncPrecedence60 = 60,
    DssFuncPrecedence70 = 70
} EnumDSSFuncPrecedence;

typedef enum EnumDSSFuncRemovePattern
{
    DssFuncRemovePatternNotApplied = 0,
    DssFuncRemovePatternOne = 1,
    DssFuncRemovePatternFact = 2,
    DssFuncRemovePatternNULL = 3
} EnumDSSFuncRemovePattern;

typedef enum EnumDMXModelPropertyIDs
{
    DssPropertyIDDMXModel = 1,
    DssPropertyIDDriverVersion = 2,
    DssPropertyIDDriverInfo = 3
} EnumDMXModelPropertyIDs;

//typedef enum EnumSimpleOperatorType
//{
//    DssFuncUnknown = 0,
//    DssFuncPlus = 1,
//    DssFuncMinus = 2,
//    DssFuncTimes = 3,
//    DssFuncDivide = 4,
//    DssFuncUnaryMinus = 5,
//    DssFuncEquals = 6,
//    DssFuncNotEqual = 7,
//    DssFuncGreater = 8,
//    DssFuncLess = 9,
//    DssFuncGreaterEqual = 10,
//    DssFuncLessEqual = 11,
//    DssFuncSum = 12,
//    DssFuncCount = 13,
//    DssFuncAvg = 14,
//    DssFuncMin = 15,
//    DssFuncMax = 16,
//    DssFuncBetween = 17,
//    DssFuncLike = 18,
//    DssFuncAnd = 19,
//    DssFuncOr = 20,
//    DssFuncNot = 21,
//    DssFuncIn = 22,
//    DssFuncRank = 23,
//    DssFuncAbs = 24,
//    DssFuncRunningSum = 25,
//    DssFuncRunningAvg = 26,
//    DssFuncMovingAvg = 27,
//    DssFuncMovingSum = 28,
//    DssFuncProduct = 29,
//    DssFuncMedian = 30,
//    DssFuncMode = 31,
//    DssFuncStdev = 32,
//    DssFuncVar = 33,
//    DssFuncGeomean = 34,
//    DssFuncEqualEnhanced = 35,
//    DssFuncNotEqualEnhanced = 36,
//    DssFuncGreaterEqualEnhanced = 37,
//    DssFuncLessEqualEnhanced = 38,
//    DssFuncBetweenEnhanced = 39,
//    DssFuncBanding = 40,
//    DssFuncBandingC = 41,
//    DssFuncBandingP = 42,
//    DssFuncNotLike = 43,
//    DssFuncNotBetween = 44,
//    DssFuncIntersect = 45,
//    DssFuncIntersectIn = 46,
//    DssFuncNullToZero = 47,
//    DssFuncZeroToNull = 48,
//    DssFuncApplySimple = 49,
//    DssFuncApplyAggregation = 50,
//    DssFuncApplyLogic = 51,
//    DssFuncApplyComparison = 52,
//    DssFuncApplyRelative = 53,
//    DssFuncIsNull = 54,
//    DssFuncIsNotNull = 55,
//    DssFuncN_Tile = 56,
//    DssFuncPercentile = 57,
//    DssFuncMovingMax = 58,
//    DssFuncMovingMin = 59,
//    DssFuncMovingDifference = 60,
//    DssFuncMovingStdev = 61,
//    DssFuncExpWghMovingAvg = 62,
//    DssFuncMovingCount = 63,
//    DssFuncRunningMax = 64,
//    DssFuncRunningMin = 65,
//    DssFuncRunningStdev = 66,
//    DssFuncRunningCount = 67,
//    DssFuncExpWghRunningAvg = 68,
//    DssFuncNotIn = 69,
//    DssFuncNotBetweenEnhanced = 70,
//    DssFuncUCase = 71,
//    DssFuncConcat = 72,
//    DssFuncFirstInRange = 73,
//    DssFuncLastInRange = 74,
//    DssFuncNTile_V = 75,
//    DssFuncStdevP = 76,
//    DssFuncRunningStdevP = 77,
//    DssFuncMovingStdevP = 78,
//    DssFuncNTile_S = 79,
//    DssFuncVarP = 80,
//    DssFuncNTile_VS = 81,
//    DssFuncCase = 82,
//    DssFuncCaseV = 83,
//    DssFuncBeginsWith = 84,
//    DssFuncContains = 85,
//    DssFuncEndsWith = 86,
//    DssFuncNotBeginsWith = 87,
//    DssFuncNotContains = 88,
//    DssFuncNotEndsWith = 89,
//    DssFuncFirstTrue = 90,
//    DssFuncMax2 = 91,
//    DssFuncMin2 = 92,
//    DssFuncFirst = 93,
//    DssFuncLast = 94,
//    DssFuncIF = 95,
//    DssFuncCurrentDate = 100,
//    DssFuncDayOfMonth = 101,
//    DssFuncDayOfWeek = 102,
//    DssFuncDayOfYear = 103,
//    DssFuncWeek = 104,
//    DssFuncMonth = 105,
//    DssFuncQuarter = 106,
//    DssFuncYear = 107,
//    DssFuncCurrentDateTime = 108,
//    DssFuncCurrentTime = 109,
//    DssFuncHour = 110,
//    DssFuncMinute = 111,
//    DssFuncSecond = 112,
//    DssFuncMilliSecond = 113,
//    DssFuncDate = 125,
//    DssFuncDaysBetween = 126,
//    DssFuncMonthsBetween = 127,
//    DssFuncAddDays = 128,
//    DssFuncAddMonths = 129,
//    DssFuncMonthStartDate = 130,
//    DssFuncMonthEndDate = 131,
//    DssFuncYearStartDate = 132,
//    DssFuncYearEndDate = 133,
//    DssFuncConcatNoBlank = 114,
//    DssFuncLength = 115,
//    DssFuncLCase = 116,
//    DssFuncLTrim = 117,
//    DssFuncPosition = 118,
//    DssFuncRTrim = 119,
//    DssFuncSubStr = 120,
//    DssFuncInitCap = 121,
//    DssFuncTrim = 122,
//    DssFuncRightStr = 123,
//    DssFuncLeftStr = 124,
//    DssFuncApplyOptional = 134,
//    DssFuncApplyCSSecurityFilter = 135,
//    DssFuncAdd = 139,
//    DssFuncAverage = 140,
//    DssFuncMultiply = 141,
//    DssFuncBandingM = 142,
//    DssFuncOLAPSum = 143,
//    DssFuncOLAPAvg = 144,
//    DssFuncOLAPCount = 145,
//    DssFuncOLAPMax = 146,
//    DssFuncOLAPMin = 147,
//    DssFuncLag = 148,
//    DssFuncLead = 149,
//    DssFuncOLAPRank = 150,
//    DssFuncRepeat = 151
//} EnumSimpleOperatorType;

typedef enum EnumDSSDataFlags
{
    DssDataValid = 0,
    DssDataNegInfinity = 1,
    DssDataPosInfinity = 2,
    DssDataUndefined = 3
} EnumDSSDataFlags;

//typedef enum EnumDssSliceFlag
//{
//    DssOriginalSlice = 0,
//    DssDisplayValueSlice = 1,
//    DssDisplayIndexSlice = 2
//} EnumDssSliceFlag;
//
//typedef enum EnumOrderingType
//{
//    DssDefaultOrdering = 0,
//    DssExtendedOrdering = 1
//} EnumOrderingType;

//typedef enum EnumEventType
//{
//    DssEventOriginalMetric = 1,
//    DssEventCompoundMetric = 2,
//    DssEventConsolidation = 3,
//    DssEventMetricLimit = 4,
//    DssEventTemplateSubtotal = 5,
//    DssEventDerivedElement = 6,
//    DssEventLastOne
//} EnumEventType;

typedef enum EnumAnalyticalPropertyIDs
{
    DssPropertyIDEvaluationOrdering = 1,
    DssPropertyIDDisplayNullOnTop = 2,
    DssNullChecking = 3,
    DssDisplayNullReplace = 4,
    DssDisplayNullReplaceValue = 5,
    DssNotCalculatedDisplay = 6,
    DssPropertyIDSubtotalDimensionalityAware = 7,
    DssQueryPopulationDimensionalityAware = 8,
    DssConsolidationSubtotal = 9,
    DssDifferentiateDuplicatedRows = 10,
    DssRemoveNullGroup = 11,
    DssSubsetRowLimit = 12,
    DssTrimEmptySections = 13,
    DssSubtotalNonAggMetricAware = 14,
    DssAutoSelectionForUnset = 15,
    DssNullCheckType = 16,
    DssMetricLevelDetermination = 17,
    DssCustomGroupDisplayForJointElments = 18,
    DssFilteringOnStringValues = 19
} EnumAnalyticalPropertyIDs;

//typedef enum EnumNullCheckingOption
//{
//    DssNumericErrorReserved = -100,
//    DssNumericErrorSkip = 0,
//    DssNumericErrorTreatAs = 1,
//    DssNumericErrorPropagate = 2,
//    DssNumericErrorCustom = 3,
//    DssNumericErrorCalcOnly = 4
//} EnumNullCheckingOption;

//typedef enum EnumThresholdResultType
//{
//    ThresholdInvalid = -1,
//    ThresholdFalse = 0,
//    ThresholdTrue = 1
//} EnumThresholdResultType;

typedef enum HashOptimizationLevel
{
    eHashUnkown = -1,
    eAggregationPartialMatch = 0,
    eFilterPartialMatch = 1,
    eCHash = 2,
    eXORHash = 3,
    eLinearHash = 4
} HashOptimizationLevel;

typedef enum EnumEvaluatorType
{
    DssEvaluatorReserved =		0,
    DssEvaluatorCalculation =	1,
    DssEvaluatorGroup =			2,
    DssEvaluatorFilter =		3,
    DssEvaluatorResidue =		4,
    DssEvaluatorElementSingle =	5,
    DssEvaluatorElementList =	6
} EnumEvaluatorType;

typedef enum EnumDSSXTabCustomGroupElement
{
    DSSXTAB_FilterNode = 0,
    DSSXTAB_Band = 1,
    DSSXTAB_Element = 2,
    DSSXTAB_BandedElement = 3
} EnumDSSXTabCustomGroupElement;

typedef Int32 DSSXTabKey;

typedef enum DSSXTAB_KEY_VALUE
{
    DSSXTAB_ORIGINALKEY_BASE = 0,
    DSSXTAB_TOTAL_KEY = -100,
    DSSXTAB_KEY_UNSET = -1
} DSSXTAB_KEY_VALUE;

typedef enum DSSXTab_Errors
{
    DSSXTAB_E_NOTINITIALIZED = 0x80042400,
    DSSXTAB_E_ALREADYINITIALIZED = 0x80042401,
    DSSXTAB_E_WRONGTEMPLATE = 0x80042402,
    DSSXTAB_E_WRONGDATATYPE = 0x80042403,
    DSSXTAB_E_INVALIDINDEX = 0x80042404,
    DSSXTAB_E_DATATOOLONG = 0x80042405,
    DSSXTAB_E_DATATOOSHORT = 0x80042406,
    DSSXTAB_E_INTERNAL = 0x80042407,
    DSSXTAB_E_DATANONNUMERIC = 0x80042408,
    DSSXTAB_E_INVALID_ENUMERATE_VAL = 0x80042409,
    DSSXTAB_E_UNSPECIFIED = 0x80042410,
    DSSXTAB_E_ALREADYLOAD = 0x80042420,
    DSSXTAB_E_LEVELMISMATCH = 0x0042440,
    DSSXTAB_E_VARIABLESIZETYPE = 0x0042441,
    DSSCALC_E_BADPARAMCOUNT = 0x80042450,
    DSSCALC_E_TYPEMISMATCH = 0x80042451,
    DSSCALC_E_INTERNAL = 0x80042452,
    DSSXTAB_E_INVALID_LAYOUT = 0x80042460
} DSSXTab_Errors;

typedef enum DSSXTab_OBJ_STATUS
{
    DSSXTAB_OBJ_INITIALIZED = 0x00,
    DSSXTAB_OBJ_UNINITIALIZED = 0x01,
    DSSXTAB_OBJ_INITIALIZING = 0x02,
    DSSXTAB_OBJ_CORRUPTED = 0x03
} DSSXTab_OBJ_STATUS;

typedef enum DFCXTABCOMPRESULT
{
    DFCXTAB_GREAT,
    DFCXTAB_EQUAL,
    DFCXTAB_LESS,
    DFCXTAB_INCOMPARABLE
} DFCXTABCOMPRESULT;

typedef enum DSSCubeSliceType
{
    DSS_CONSTANT_SLICE,
    DSS_METRIC_SLICE,
    DSS_PROPERTY_SLICE
} DSSCubeSliceType;

typedef enum DSSSQLStmtStatus_Type
{
    DssSQLStmtCompleted = 1,
    DssSQLStmtNotCompleted = 2
} DSSSQLStmtStatus_Type;

typedef enum DSSSQLType_Type
{
    DssSQLUnkown = 0,
    DssSQLSelect = 1,
    DssSQLInsert = 2,
    DssSQLCreate = 3
} DSSSQLType_Type;

typedef enum EnumDSSReportFlags
{
    DssReportSourceFlagReserved = 0x00000000,
    DssReportFresh = 0x00000001,
    DssReportDefaultAutoprompt = 0x00000002,
    DssReportSecurityChecked = 0x00000004,
    DssReportElementBrowse = 0x00000008,
    DssReportUseLocalCache = 0x00000010,
    DssReportUseOutSourceCache = 0x00000080,
    DssReportUpdateLocalCache = 0x00000020,
    DssReportUpdateOutSourceCache = 0x00000100,
    DssReportRefreshKeepAnswers = 0x00000200,
    DssReportInboxKeepResult = 0x00000400,
    DssReportSaveToInbox = 0x00000800,
    DssReportReprompt = 0x00001000,
    DssReportPersistForCache = 0x00002000,
    DssReportRunSynch = 0x00000040,
    DssReportPersistLocalDepns = 0x10000000,
    DssReportLocalOnly = 0x20000000,
    DssReportPersistAllDepns = 0x40000000,
    DssReportUseAllCache = DssReportUseLocalCache  | DssReportUseOutSourceCache,
    DssReportUpdateAllCache = DssReportUpdateLocalCache  | DssReportUpdateOutSourceCache,
    DssReportCacheDefault = DssReportUseAllCache  | DssReportUpdateAllCache,
    DssReportCacheOptionRefreshLocal =DssReportUpdateAllCache  | DssReportUseOutSourceCache,
    DssReportCacheOptionNotRefresh =DssReportCacheDefault,
    DssReportCacheOptionRefreshServer =DssReportUpdateAllCache,
    DssReportCreateInBoxMsg = 0x80000000,
    DssReportNewStream = 0x04000000,
    DssReportWorkingSetElementBrowse = 0x00002000,
    DssReportExecutionNoResultRetrieval = 0x00004000,
    DssReportLoadPreserveViewReport = 0x00008000,
    DssReportRWDocumentSubReportExecution = 0x00010000,
    DssReportLoadPreserveViewDataSets = 0x00020000,
    DssReportSerializeReportViews = 0x00400000,
    DssReportTryAutoClosePrompt = 0x00800000,
    DssReportDEChanged = 0x00100000,
    DssReportDEonDTElementBrowse = 0x00200000,
    DSSReportWebQueryBuilderOrFFSQL =  0x08000000
} EnumDSSReportFlags;

typedef enum EnumDSSReportActions
{
    DssActionReserved = 0x0,
    DssActionResolve = 0x00000001,
    DssActionGenerateSQL = 0x00000002,
    DssActionExecuteSQL = 0x00000004,
    DssActionAlerts = 0x00000008,
    DssActionXTab = 0x00000010,
    DssActionEvaluateView = 0x00000020,
    DssActionExecuteAll = 0x8000003f,
    DssActionGetElementOnly = 0x00000040,
    DssActionGenerateDatamart = 0x00000080,
    DssActionResolveCB = 0x00000100,
    DssActionGenerateSQLCB = 0x00000200,
    DssActionExecuteSQLCB = 0x00000400,
    DssActionAlertsCB = 0x00000800,
    DssActionXTabCB = 0x00001000,
    DssActionUpdateCacheCB = 0x00002000,
    DssActionReportPreview = 0x00004000,
    DssActionExecuteAllCB = 0x00001f00,
    DssActionGenerateDatamartCB = 0x00008000,
    DssActionResolveRetrieve = 0x00010000,
    DssActionGenerateSQLRetrieve = 0x00020000,
    DssActionExecuteSQLRetrieve = 0x00040000,
    DssActionAlertsRetrieve = 0x00080000,
    DssActionXTabRetrieve = 0x00100000,
    DssActionUpdateCacheRetrieve = 0x00200000,
    DssActionFindCacheRetrieve = 0x00400000,
    DssActionExecuteAllRetrieve = 0x001f0000,
    DssActionGenerateDatamartRetrieve = 0x00800000,
    DssActionUseLocalCache = 0x01000000,
    DssActionUseOutSourceCache = 0x20000000,
    DssActionUpdateLocalCache = 0x02000000,
    DssActionForceExecute = 0x04000000,
    DssActionLocalCacheOnly = 0x08000000,
    DssActionRefresh = 0x10000000,
    DssActionUpdateOutSourceCache = 0x40000000,
    DssActionAllCache = 0x7f000000,
    DssActionUseAllCache = DssActionUseLocalCache  | DssActionUseOutSourceCache | DssActionUpdateLocalCache | DssActionUpdateOutSourceCache,
    DssActionSubsetting = 0x80000000
} EnumDSSReportActions;

typedef enum EnumDSSReportNext
{
    DssReportNextReserved = 0,
    DssReportNextResolutionEngine,
    DssReportNextSQLGenEngine,
    DssReportNextQueryEngine,
    DssReportNextAlertEngine,
    DssReportNextAnalyticEngine,
    DssReportNextElementServer,
    DssReportNextReportSourceServer,
    DssReportNextUpdateCache,
    DssReportNextFindCache,
    DssReportNextError = 0x1000,
    DssReportNextFinished,
    DssReportNextSubsetting,
    DssReportNextExportEngine,
    DssReportNextFindCube,
    DssReportNextUpdateCube,
    DssReportNextCSIGenEngine,
    DssReportNextSubsetEngine,
    DssReportNextMergeCubeData,
    DssReportNextProcessCubeData
} EnumDSSReportNext;

typedef enum EnumDSSReportStates
{
    DssActionResolveDone = 0x00000001,
    DssActionGenerateSQLDone = 0x00000002,
    DssActionExecuteSQLDone = 0x00000004,
    DssActionXTabDone = 0x00000008,
    DssActionAlertsDone = 0x00000010,
    DssActionUpdateCacheDone = 0x00000020,
    DssActionFindCacheDone = 0x00000040,
    DssActionWaitingForPrompts = 0x00000080,
    DssActionEvaluateViewDone = 0x02000000,
    DssActionViewCalculationDone = 0x00004000,
    DssActionViewPopulationDone = 0x00000010,
    DssReportAEViewAllDone = 0x02004010,
    DssActionAllDone = 0x027040ff,
    DssReportDefinition = 0x00000100,
    DssReportAppObjects = 0x00000200,
    DssReportResolution = 0x00000400,
    DssReportSQLs = 0x00000800,
    DssReportXTab = 0x00001000,
    DssReportXTabView = 0x00002000,
    DssReportAlerts = 0x00004000,
    DssReportAppObjectsComplete = 0x00010000,
    DssReportAllData = 0x04013f00,
    DssReportResultReadyInBase = 0x00020000,
    DssReportInWaitingList = 0x00040000,
    DssReportHasPrompts = 0x00080000,
    DssReportBaseExecuting = 0x00100000,
    DssReportBaseExecutionCompleted = 0x00200000,
    DssActionSubsettingDone = 0x00400000,
    DssActionReportSourceServerDone = 0x00800000,
    DssActionElementBrowseDone = 0x01000000,
    DssReportDrillInstance = 0x04000000,
    DssReportElements = 0x08000000,
    DssReportLoadComplete = 0x10000000,
    DssReportResolutionInProgress = 0x20000000,
    DssReportViewTemplatesEvaluationDone = 0x40000000
} EnumDSSReportStates;

typedef enum EnumDSSReportContentStates
{
    DssReportContentDefinition = DssReportDefinition,
    DssReportContentAppObjects = DssReportAppObjects,
    DssReportContentResolution = DssReportResolution,
    DssReportContentSQLs = DssReportSQLs,
    DssReportContentXTab = DssReportXTab,
    DssReportContentXTabView = DssReportXTabView,
    DssReportContentAppObjectsComplete = DssReportAppObjectsComplete,
    DssReportContentResultReadyInBase = DssReportResultReadyInBase,
    DssReportContentHasPrompts = DssReportHasPrompts,
    DssReportContentAllData = DssReportAllData,
    DssReportContentDrillInstance = DssReportDrillInstance,
    DssReportContentElements = DssReportElements,
    DssReportContentLoadComplete = DssReportLoadComplete,
    DssReportContentOriginalStates = 0x1c0b3f00
} EnumDSSReportContentStates;

typedef enum EnumDSSReportActionStates
{
    DssReportActionResolveDone = DssActionResolveDone,
    DssReportActionGenerateSQLDone = DssActionGenerateSQLDone,
    DssReportActionExecuteSQLDone = DssActionExecuteSQLDone,
    DssReportActionXTabDone = DssActionXTabDone,
    DssReportActionViewPopulationDone = DssActionViewPopulationDone,
    DssReportActionUpdateCacheDone = DssActionUpdateCacheDone,
    DssReportActionFindCacheDone = DssActionFindCacheDone,
    DssReportActionWaitingForPrompts = DssActionWaitingForPrompts,
    DssReportActionGenerateCSIDone = 0x00000100,
    DssReportActionSubsetCubeDone = 0x00000200,
    DssReportActionMergeCubeDataDone = 0x00000400,
    DssReportActionProcessCubeDataDone = 0x00000800,
    DssReportActionFindCubeDone = 0x00001000,
    DssReportActionUpdateCubeDone = 0x00002000,
    DssReportActionViewCalculationDone = DssActionViewCalculationDone,
    DssReportActionInWaitingList = DssReportInWaitingList,
    DssReportActionBaseExecuting = DssReportBaseExecuting,
    DssReportActionBaseExecutionCompleted = DssReportBaseExecutionCompleted,
    DssReportActionSubsettingDone = DssActionSubsettingDone,
    DssReportActionReportSourceServerDone = DssActionReportSourceServerDone,
    DssReportActionElementBrowseDone = DssActionElementBrowseDone,
    DssReportActionEvaluateViewDone = DssActionEvaluateViewDone,
    DssReportActionAEViewAllDone = DssReportAEViewAllDone,
    DssReportActionAllDone = DssActionAllDone  | DssReportActionUpdateCubeDone | DssReportActionFindCubeDone | DssReportActionMergeCubeDataDone | DssReportActionProcessCubeDataDone | DssReportActionGenerateCSIDone | DssReportActionSubsetCubeDone,
    DssReportActionResolutionInProgress = DssReportResolutionInProgress,
    DssReportActionViewTemplatesEvaluationDone = DssReportViewTemplatesEvaluationDone,
    DssReportActionOriginalStates = 0x63f440ff
} EnumDSSReportActionStates;

typedef enum EnumDSSExecutionStatus
{
    DssExecutionCompleted = 0,
    DssExecutionInProgress,
    DssExecutionPaused,
    DssExecutionError,
    DssExecutionPrompts,
    DssExecutionCanceled
} EnumDSSExecutionStatus;

typedef enum EnumDSSRSSActions
{
    DssRSSActionNone = 0,
    DssRSSActionFinished,
    DssRSSActionPrompts,
    DssRSSActionAnsweringPrompts
} EnumDSSRSSActions;

typedef enum EnumDSSReportCachedFlag
{
    DssClientSideCached = 0x00000001,
    DssServerSideCached = 0x00000002,
    DssClusterServerCached = 0x00000004
} EnumDSSReportCachedFlag;

typedef enum EnumDSSReportServerConnectionMode
{
    DssReportServerConnectionModeReserved = 0,
    DssReportServerConnectionServer = 0x00000001,
    DssReportServerConnectionClient = 0x00000002,
    DssReportServerConnection2Tier = 0x00000004
} EnumDSSReportServerConnectionMode;

typedef enum EnumDSSReportSaveAs
{
    DssReportSaveAsDefault = 0,
    DssReportSaveAsOriginalTemplate = 1,
    DssReportSaveAsOriginalFilter = 2,
    DssReportSaveAsWithAnswers = 4,
    DssReportSaveAsPruneAnswers = 8,
    DssReportSaveAsDeclinePrompts = 0x10,
    DssReportSaveAsOverwrite = 0x20,
    DssReportSaveAsEmbedded = 0x40,
    DssReportSaveAsModifyObjects = 0x80,
    DSSReportSaveAsEmbeddedFilter = 0x100,
    DSSReportSaveAsEmbeddedTemplate = 0x200,
    DssReportSaveAsJustFilter = 0x400,
    DssReportSaveAsJustTemplate = 0x800,
    DssReportSaveAsWithPrompts = 0x1000,
    DssReportSaveAsFilterWithPrompts = 0x2000,
    DssReportSaveAsTemplateWithPrompts = 0x4000,
    DssReportSaveAsClearCurrentElements = 0x8000,
    DssReportSaveAs8iWarning = 0x00010000,
    DssReportSaveAsRemoveNonPrimaryNameTranslations = 0x00020000,
    DssReportSaveAsFromWeb = 0x00040000
} EnumDSSReportSaveAs;

typedef enum EnumDSSReportExecutionSetting
{
    DssReportExecutionSettingMaxRowCount = 0,
    DssReportExecutionSettingDrillComplete,
    DssReportExecutionSettingCacheCreateJobID,
    DssReportExecutionSettingCacheHitJobID,
    DssReportExecutionSettingExecutionContext,
    DssReportExecutionSettingExport,
    DssReportExecutionSettingCacheLoadingFlag,
    DssReportExecutionSettingCaceledErroredJobID,
    DssReportExecutionSettingDBMatchingFlag,
    DssReportExecutionSettingNormalReportHittingCube,
    DssReportExecutionSettingOpenViewCount,
    DssReportExecutionSettingCubeRepublish,
    DssReportExecutionSettingLastOne
} EnumDSSReportExecutionSetting;

typedef enum DSSReportSerializationFlags
{
    DssReportSerializationReserved = 0x00000000,
    DssReportCacheSerialization = 0x00000001,
    DssReportNewSerialization = DssReportNewStream,
    DssReportCacheSerialization2 = 0x00000002,
    DssReportCacheSerialization3 = 0x00000004,
    DssReportSerializeWSResultPool = 0x00000008,
    DssReport8iSerialization = 0x00000010,
    DssReportRWDSerialization = DssReportRWDocumentSubReportExecution,
    DssReportDetailSerialization = 0x00040000,
    DssReportSkipXTab = 0x00080000,
    DssReportRWDMessageSerialization = 0x00200000,
    DssReportRWDCacheSerialization = 0x00800000,
    DssReportPerserveViewDatasetsSerialization = DssReportLoadPreserveViewDataSets,
    DssReportReportViewsSerialization = DssReportSerializeReportViews
} DSSReportSerializationFlags;

typedef enum EnumDSSReportServerGraph
{
    DssReportServerGraphReserved = 0,
    DssReportServerGraphAutoArrange = 0x00000001,
    DssReportServerGraphExcelExport = 0x00000002,
    DssReportServerGraphPopulteGraphStyle = 0x00000004
} EnumDSSReportServerGraph;

typedef enum EnumDSSWorkingSetSearchFlags
{
    DssWorkingSetSearchReserved = 0
} EnumDSSWorkingSetSearchFlags;

typedef enum EnumDSSDataImportStates
{
    DataImportStateReserved = 0,
    DataImportStateDone = 0x00000001,
    DataImportStateInProgress = 0x00000002,
    DataImportStateError = 0x00000004,
    DataImportStateMask = 0x000000FF,
    DataImportStateFlagHasOC = 0x00000100,
    DataImportStateFlagsMask = 0x0000FF00,
    DataImportActionInit = 0x00010000,
    DataImportActionColumnMapping = 0x00020000,
    DataImportActionObjectMapping = 0x00040000,
    DataImportActionMappingChange = 0x00080000,
    DataImportActionCubeDefinitionSave = 0x00100000,
    DataImportActionCubePublish = 0x00200000,
    DataImportActionsMask = 0x00FF0000
} EnumDSSDataImportStates;

typedef enum EnumDSSDataImportOperations
{
    DataImportOperationReserved = 0,
    DataImportOperationCreate =  1,
    DataImportOperationUpdate =  2,
    DataImportOperationEdit =  3
} EnumDSSDataImportOperations;

typedef enum EnumDSSReportStreamFlags
{
    DssReportStreamFlagReserved = 0x00000000,
    DssReportStreamAscii = 0x00000001,
    DssReportStreamComma = 0x00000002
} EnumDSSReportStreamFlags;

typedef enum EnumDSSReportPropertyAttributes
{
    DssReportPropertyDefault = 0x00000000,
    DssReportPropertyNoSerialize = 0x00000001,
    DssReportPropertySerializeToXML = 0x00000002
} EnumDSSReportPropertyAttributes;

typedef enum EnumDSSQueryStatisticsFields
{
    DssQueryStatFieldsReserved = 0x00000000,
    DssQueryStatPassCount = 0x00000001,
    DssQueryStatStartTime = 0x00000002,
    DssQueryStatFinishTime = 0x00000004,
    DssQueryStatDuration = 0x00000008,
    DssQueryStatSQLPassCount = 0x00000010,
    DssQueryStatSQLDuration = 0x00000011,
    DssQueryStatAnalyticalPassCount = 0x00000012,
    DssQueryStatAnalyticalDuration = 0x00000014,
    DssQueryStatDBErrorMsg = 0x00000018,
    DssQueryStatTempTableCount = 0x00000020,
    DssQueryStatDBDefinitions = 0x00000021,
    DssQueryStatPasses = 0x00000022,
    DssQueryStatTables = 0x00000024,
    DssQueryStatDataSourceType = 0x00000028,
    DssQueryStatStartDateTime = 0x00000040,
    DssQueryStatFinishDateTime = 0x00000041,
    DssQueryStatSQLGenTime = 0x00000048,
    DssQueryStatSQLExecutionTime = 0x00000049,
    DssQueryStatTotalFetchTime = 0x0000004A,
    DssQueryStatDataFetchTime = 0x0000004B,
    DssQueryStatOtherProcTime = 0x0000004C,
    DssQueryStatTheorImprovement = 0x0000004D,
    DssQueryStatParallelMode = 0x0000004E,
    DssEnumUserDefinedStart = 0x01000000
} EnumDSSQueryStatisticsFields;

typedef enum EnumDSSPassStatisticsFields
{
    DssPassStatFieldsReserved = 0x00000000,
    DssPassStatSQLType = 0x00000001,
    DssPassStatSQLString = 0x00000002,
    DssPassStatStartTime = 0x00000004,
    DssPassStatFinishTime = 0x00000008,
    DssPassStatDuration = 0x00000010,
    DssPassStatRowCount = 0x00000011,
    DssPassStatColumnCount = 0x00000012,
    DssPassStatTables = 0x00000014,
    DssPassStatDBDefinition = 0x00000018,
    DssPassStatTableCount = 0x00000020
} EnumDSSPassStatisticsFields;

typedef enum EnumDSSQueryStatisticsSQLClauseType
{
    DssQueryStatClauseFieldsReserved = 0x00000000,
    DssQueryStatClauseSelect = 0x00000001,
    DssQueryStatClauseSelectGroupBy = 0x00000002,
    DssQueryStatClauseSelectAgg = 0x00000004,
    DssQueryStatClauseFrom = 0x00000008,
    DssQueryStatClauseWhere = 0x00000010,
    DssQueryStatClauseOrderBy = 0x00000011
} EnumDSSQueryStatisticsSQLClauseType;

typedef enum EnumDSSExportSettingID
{
    DssExportReserved = 0,
    DssExportOption,
    DssExportLocale,
    DssExportMemLimit,
    DssExportReportWebSetting,
    DssExportStartRow,
    DssExportStartColumn,
    DssExportMaxRow,
    DssExportMaxColumn,
    DssExportGraphStartRow,
    DssExportGraphStartColumn,
    DssExportGraphMaxRow,
    DssExportGraphMaxColumn,
    DssExportInboxMessageAlias,
    DssExportLastSetting
} EnumDSSExportSettingID;

typedef enum EnumDSSScriptStatementType
{
    DssScriptStatementTypeReserved,
    DssScriptSimpleStatement,
    DssScriptParallelStatement,
    DssScriptSerialStatement,
    DssScriptSelectionStatement,
    DssScriptIterationStatement
} EnumDSSScriptStatementType;

typedef enum EnumDSSCommandParameterType
{
    DssCommandParameterTypeReserved,
    DssCommandParameterLong,
    DssCommandParameterBSTR,
    DssCommandParameterStream
} EnumDSSCommandParameterType;

typedef enum EnumDSSStatementParametersMode
{
    DssStatementParametersModeReserved,
    DssStatementParameterStream,
    DssStatementParameterVector
} EnumDSSStatementParametersMode;

typedef enum EnumDSSRequestLoadType
{
    DssRequestLoadFromMD,
    DssRequestLoadFromMemory,
    DssRequestLoadFromMsg
} EnumDSSRequestLoadType;

typedef enum EnumDSSJobPriorityFactor
{
    DssJobPriorityFactorReserved = -1,
    DssJobPriorityFactorProjectID,
    DssJobPriorityFactorUserID,
    DssJobPriorityFactorReportCost,
    DssJobPriorityFactorRequestType,
    DssJobPriorityFactorApplication
} EnumDSSJobPriorityFactor;

typedef enum EnumDSSJobRequestType
{
    DssJobRequestTypeReserved = -1,
    DssJobRequestAnyType,
    DssJobRequestTypeElement,
    DssJobRequestTypeObject,
    DssJobRequestTypeReport,
    DssJobRequestTypeDocument,
    DssJobRequestBatchRequest,
    DssJobRequestLastType
} EnumDSSJobRequestType;

typedef enum EnumDSSApplication
{
    DssApplicationTypeReserved = -1,
    DssApplicationAgent = 0,
    DssApplicationWeb = 1,
    DssApplicationScheduler = 2,
    DssApplicationCustom = 3,
    DssApplicationNCS = 9,
    DssApplicationJavaDesktop = 21
} EnumDSSApplication;

typedef enum EnumDSSReportCostFactor
{
    DssReportCostFactorReserved = -1,
    DssReportCostUserSupplied = 1,
    DssReportCostEstimated,
    DssReportCostHistorical,
    DssReportCostResultSetSize,
    DssReportCostLastFactor
} EnumDSSReportCostFactor;

typedef enum EnumDSSDateTimeMode
{
    DssDateTimeModeLocaleSensitive,
    DssDateTimeModeLocaleInsensitive
} EnumDSSDateTimeMode;

typedef enum EnumDSSDynamicTime
{
    DssTimeEnd,
    DssTimeBeginning,
    DssTimeNow,
    DssTimeToday,
    DssTimeThisWeek,
    DssTimeThisMonth,
    DssTimeThisYear,
    DssTimeThisSunday,
    DssTimeThisMonday,
    DssTimeThisTuesday,
    DssTimeThisWednesday,
    DssTimeThisThursday,
    DssTimeThisFriday,
    DssTimeThisSaturday,
    DssTimeNotDynamicTime = -100,
    DssTimeInvalidTime = -101,
    DssTimeStaticDate = 0x20,
    DssTimeDynamicHour = 0x40,
    DssTimeDynamicMinute = 0x80
} EnumDSSDynamicTime;

typedef enum EnumDSSDayOfWeek
{
    DssDaySunday = 0x00000001,
    DssDayMonday = 0x00000002,
    DssDayTuesday = 0x00000004,
    DssDayWednesday = 0x00000008,
    DssDayThursday = 0x00000010,
    DssDayFriday = 0x00000020,
    DssDaySaturday =0x00000040
} EnumDSSDayOfWeek;

typedef enum EnumDSSTimeGranularityLevel
{
    DssGranularityMilliseconds,
    DssGranularitySeconds,
    DssGranularityMinutes,
    DssGranularityHour,
    DssGranularityDay,
    DssGranularityMonth,
    DssGranularityYear,
    DssGranularityWeek,
    DssGranularityDayofweek
} EnumDSSTimeGranularityLevel;

typedef enum EnumDSSMemberRecurrence
{
    DssRecurrenceNotRecurrent = -100,
    DssRecurrenceLast = -1,
    DssRecurrenceEvery = -2,
    DssRecurrenceOn = -3
} EnumDSSMemberRecurrence;

typedef enum EnumDSSRecurrenceLevel
{
    DssRecurrenceLevelSecond = 0,
    DssRecurrenceLevelMinute = 1,
    DssRecurrenceLevelHour = 2,
    DssRecurrenceLevelDay = 3,
    DssRecurrenceLevelWeek = 4,
    DssRecurrenceLevelDayOfWeek = 5,
    DssRecurrenceLevelMonth = 6,
    DssRecurrenceLevelYear = 7
} EnumDSSRecurrenceLevel;

typedef enum EnumDSSScheduleTriggerType
{
    DssTriggerEvent,
    DssTriggerTimePointSeries
} EnumDSSScheduleTriggerType;

typedef enum EnumDSSScheduleAdminOption
{
    DssScheduleAdminNormal,
    DssScheduleAdminDetail
} EnumDSSScheduleAdminOption;

typedef enum EnumDSSStatsCubeAction
{
    DssStatsCubeActionReserved = 		0x00000000,
    DssStatsCubePublish = 				0x00010000,
    DssStatsCubeRepublish = 			0x00020000,
    DssStatsCubeIncrementalRefresh =	0x00030000,
    DssStatsCubeViewReportHit =			0x00040000,
    DssStatsCubeDynamicSourceHit = 		0x00050000,
    DssStatsCubeDelete = 				0x00060000,
    DssStatsCubeActivate = 				0x00070000,
    DssStatsCubeDeactivate = 			0x00080000,
    DssStatsCubeLoad = 					0x00090000,
    DssStatsCubeUnload = 				0x000A0000,
    DssStatsCubeIncrementFilter =		0x00001000,
    DssStatsCubeIncrementReport = 		0x00002000,
    DssStatsCubeRepublishReplace =		0x00000001,
    DssStatsCubeRepublishAdd = 			0x00000002,
    DssStatsCubeRepublishDynamic = 		0x00000003,
    DssStatsCubeRepublishUpsert =		0x00000004,
    DssStatsCubeRefreshAdd =			0x00000005,
    DssStatsCubeRefreshDelete = 		0x00000006,
    DssStatsCubeRefreshUpdate = 		0x00000007,
    DssStatsCubeRefreshUpsert = 		0x00000008
} EnumDSSStatsCubeAction;

typedef enum EnumDSSMobileOS
{
    DssMobileOSReserved = 		0,
    DssMobileOSiOS =	 				1,
    DssMobileOSAndroid =	 			2,
    DssMobileOSWindowsMobile =			3,
    DssMobileOSBlackBerry =				4,
    DssMobileOSLast =					4
} EnumDSSMobileOS;

typedef enum EnumDSSMobileExecType
{
    DssMobileExecReserved = 	0,
    DssMobileExecUserReq =				1,
    DssMobileExecPreCache =				2,
    DssMobileExecAppRecovery =			3,
    DssMobileExecLast =					3
} EnumDSSMobileExecType;

typedef enum EnumDSSMobileCacheHit
{
    DssMobileCacheReserved =			0,
    DssMobileCacheNone =				1,
    DssMobileCacheDevice =				2,
    DssMobileCacheServer =				3,
    DssMobileCacheLast =				3
} EnumDSSMobileCacheHit;

typedef enum EnumDSSMobileNetworkType
{
    DssMobileNetworkReserved = 				0x00000000,
    DssMobileNetworkiOSNone =				0x01000000,
    DssMobileNetworkiOSWiFi =				0x01000001,
    DssMobileNetworkiOSWWAN =				0x01000002,
    DssMobileNetworkAndroidBluetooth =		0x02000000,
    DssMobileNetworkAndroidDummy =			0x02000001,
    DssMobileNetworkAndroidEithernet =		0x02000002,
    DssMobileNetworkAndroidMobile =			0x02000003,
    DssMobileNetworkAndroidMobile_DUN =		0x02000004,
    DssMobileNetworkAndroidMobile_HIPRI =	0x02000005,
    DssMobileNetworkAndroidMobile_MMS =		0x02000006,
    DssMobileNetworkAndroidMobile_SUPL =	0x02000007,
    DssMobileNetworkAndroidWIFI =			0x02000008,
    DssMobileNetworkAndroidWIMAX =			0x02000009
} EnumDSSMobileNetworkType;

typedef enum EnumDSSMobileStatsFormat
{
    DssMobileStatsFormatReserved = 		0,
    DssMobileStatsFormatXML =						1,
    DssMobileStatsFormatDefault = DssMobileStatsFormatXML,
    DssMobileStatsFormatLast =						1
} EnumDSSMobileStatsFormat;

//typedef enum EnumDSSDocumentType
//{
//    DssDocumentTypeReserved = 0,
//    DssDocumentTypeBase = 1,
//    DssDocumentTypeView = 2,
//    DssDocumentTypeReportWritingDoc = 3
//} EnumDSSDocumentType;
//
//typedef enum EnumDSSRWTemplateOrigin
//{
//    DssRWTemplateOriginReserved = 0,
//    DssRWTemplateOriginLocal = 1,
//    DssRWTemplateOriginReportView = 2
//} EnumDSSRWTemplateOrigin;
//
//typedef enum EnumDSSRWNodeType
//{
//    DssRWNodeReserved = 0,
//    DssRWNodeSection = 1,
//    DssRWNodeElement = 2,
//    DssRWNodeTemplate,
//    DssRWNodeFieldGroup,
//    DssRWNodeDocument,
//    DssRWNodeControl,
//    DssRWNodeLastOne
//} EnumDSSRWNodeType;

typedef enum EnumDSSRWOptions
{
    DssRWOptionReserved = 0,
    DssRWOptionGroupBy,
    DssRWOptionFormatting,
    DssRWOptionDataSlicing,
    DssRWOptionLastOne
} EnumDSSRWOptions;

//typedef enum EnumDSSRWSectionType
//{
//    DssRWSectionTypeReserved = 0,
//    DssRWSectionContent,
//    DssRWSectionReportHeader,
//    DssRWSectionReportFooter,
//    DssRWSectionPageHeader,
//    DssRWSectionPageFooter,
//    DssRWSectionGroupBySection,
//    DssRWSectionGroupByHeader,
//    DssRWSectionGroupByFooter,
//    DssRWSectionBody,
//    DssRWSectionBodyHeader,
//    DssRWSectionDetails,
//    DssRWSectionBodyFooter,
//    DssRWSectionSubSection,
//    DssRWSectionGeneric,
//    DssRWSectionControlGroupBySection,
//    DssRWSectionContainer,
//    DssRWSectionPanel,
//    DssRWSectionUnitCondition,
//    DssRWSectionLastOne
//} EnumDSSRWSectionType;

typedef enum EnumDSSSectionElementType
{
    DssSectionElementReserved = 0,
    DssSectionElementTemplate = 1,
    DssSectionElementFieldGroup = 2,
    DssSectionElementSubReport = 3,
    DssSectionElementLastOne
} EnumDSSSectionElementType;

typedef enum EnumDSSRWNodeExecutionStatus
{
    DssRWNodeExecutionReserved = 0,
    DssRWNodeExecutionCompleted,
    DssRWNodeExecutionInProgress,
    DssRWNodeExecutionPaused,
    DssRWNodeExecutionError,
    DssRWNodeExecutionPrompts,
    DssRWNodeExecutionCanceled
} EnumDSSRWNodeExecutionStatus;

//typedef enum EnumDSSRWFieldType
//{
//    DssRWFieldReserved = 0,
//    DssRWFieldShape = 1,
//    DssRWFieldImage = 2,
//    DssRWFieldText = 3,
//    DssRWFieldConcatenation = 3,
//    DssRWFieldObjectName = 4,
//    DssRWFieldObjectValue = 5,
//    DssRWFieldLabel = 6,
//    DssRWFieldLine = 7,
//    DssRWFieldAutoText = 8,
//    DssRWFieldCalculation = 9,
//    DssRWFieldHTML = 10,
//    DssRWFieldLastOne
//} EnumDSSRWFieldType;

typedef enum EnumDSSShapeType
{
    DssShapeTypeReserved = 0,
    DssShapeTypeNone,
    DssShapeTypeLines,
    DssShapeTypeRectangle,
    DssShapeTypeFilledRectangle,
    DssShapeTypeRoundedRectangle,
    DssShapeTypeFilledRoundedRectangle
} EnumDSSShapeType;

//typedef enum EnumDSSAutoText
//{
//    DssAutoTextReserved = 0,
//    DssAutoTextPage,
//    DssAutoTextNPage,
//    DssAutoTextDateTime,
//    DssAutoTextUser,
//    DssAutoTextDocument,
//    DssAutoTextProject,
//    DssAutoTextGroupBy,
//    DssAutoTextPrompt,
//    DssAutoTextExecutionTime,
//    DssAutoTextReport,
//    DssAutoTextReportFilterDetails,
//    DssAutoTextReportTemplateDetails,
//    DssAutoTextReportExecutionTime,
//    DssAutoTextAttributeElementID,
//    DssAutoTextObjectID,
//    DssAutoTextDocumentMessageID,
//    DssAutoTextPromptAnswerXML,
//    DssAutoTextServerName,
//    DssAutoTextDocumentID,
//    DssAutoTextReportID,
//    DssAutoTextWebServer,
//    DssAutoTextViewFilter,
//    DssAutoTextNotes,
//    DssAutoTextDocumentDescription,
//    DssAutoTextReportDescription,
//    DssAutoTextReportDetails,
//    DssAutoTextDocumentPromptDetails,
//    DssAutoTextReportPromptDetails,
//    DssAutoTextReportFilterDetail,
//    DssAutoTextReportLimitDetails,
//    DssAutoTextTitle,
//    DssAutoTextCurrentSelectionID,
//    DssAutoTextAttributeLongElementID,
//    DssAutoTextCurrentSelectionElementID,
//    DssAutoTextLastOne
//} EnumDSSAutoText;

typedef enum EnumDSSRWRetrieveFieldsOption
{
    DssRWRetrieveFieldsDefault = 0x00,
    DssRWRetrieveFieldsAllSubTree = 0x01,
    DssRWRetrieveFieldsLeafFields = 0x02
} EnumDSSRWRetrieveFieldsOption;

typedef enum EnumDSSRWNameResolutionOption
{
    DSSRWNameResolutionRefresh = 0x01
} EnumDSSRWNameResolutionOption;

typedef enum EnumDSSPageThroughElementStatus
{
    DssPageThroughElementReserved = 0,
    DssPageThroughElementAll = 1,
    DssPageThroughElementNone = 2,
    DssPageThroughElementSelect = 3
} EnumDSSPageThroughElementStatus;

//typedef enum EnumDSSRWPageByStyle
//{
//    DssRWPageByStyleReserved = 0,
//    DssRWPageByStyleIndividual = 1,
//    DssRWPageByStyleAll = 2,
//    DssRWPageByStyleBoth = 3
//} EnumDSSRWPageByStyle;

typedef enum EnumDSSCurrentElementAction
{
    DssCurrentElementActionReserved = 0,
    DssCurrentElementActionFirst = 1,
    DssCurrentElementActionLast = 2,
    DssCurrentElementActionPrevious = 3,
    DssCurrentElementActionNext = 4
} EnumDSSCurrentElementAction;

typedef enum EnumDSSRWFieldSpecialType
{
    DssRWFieldSpecialTypeNormal = 0,
    DssRWFieldSpecialTypeThreshold = 1
} EnumDSSRWFieldSpecialType;

//typedef enum EnumDSSRWControlType
//{
//    DssRWControlTypeReserved = 0,
//    DssRWControlTypeElementList = 1,
//    DssRWControlTypeMetricList = 2,
//    DssRWControlTypeTabList = 3,
//    DssRWControlTypeAction = 4
//} EnumDSSRWControlType;
//
//typedef enum EnumDSSRWControlActionType
//{
//    DssRWControlActionTypeReserved = 0x00000000,
//    DssRWControlActionTypeUndo = 0x00000001,
//    DssRWControlActionTypeRedo = 0x00000002,
//    DssRWControlActionTypeDiscard = 0x00000004,
//    DssRWControlActionTypeSubmit = 0x00000008,
//    DssRWControlActionTypeRecalculate = 0x00000010,
//    DssRWControlActionTypeAll = 0xffffffff
//} EnumDSSRWControlActionType;

//typedef enum EnumDSSRWControlSubsequentAction
//{
//    DssRWControlSubsequentActionReserved = 0x00000000,
//    DssRWControlSubsequentActionDisplayMessage = 0x00000001,
//    DssRWControlSubsequentActionDisplayTransactionReport = 0x00000002,
//    DssRWControlSubsequentActionRefreshGrid = 0x00000004,
//    DssRWControlSubsequentActionReexecuteDocument = 0x00000008,
//    DssRWControlSubsequentActionExecuteNewObject = 0x00000010,
//    DssRWControlSubsequentActionForceExecution = 0x10000000,
//    DssRWControlSubsequentActionInvalidateClientCache = 0x20000000,
//    DssRWControlSubsequentActionPassPromptAnswer = 0x40000000
//} EnumDSSRWControlSubsequentAction;

//typedef enum EnumDSSRWNodeTransactionChangeType
//{
//    DssRWNodeTransactionChangeTypeReserved = 0x00000000,
//    DssRWNodeTransactionChangeTypeDefault = 0x00000001,
//    DssRWNodeTransactionChangeTypeMarkSelection = 0x00000002
//} EnumDSSRWNodeTransactionChangeType;

typedef enum EnumDSSRWGroupByUnitType
{
    DssRWGroupByUnitReserved = 0,
    DssRWGroupByUnitObject = 1,
    DssRWGroupByUnitMetric = 2
} EnumDSSRWGroupByUnitType;

//typedef enum EnumDSSRWControlFormatType
//{
//    DssRWControlFormatReserved = 0,
//    DssRWControlFormatControl = 1,
//    DssRWControlFormatUnselectedItem = 2,
//    DssRWControlFormatSelectedItem = 3,
//    DssRWControlFormatHighlightedItem = 4,
//    DssRWControlFormatControlTitle = 5,
//    DssRWControlFormatLastOne
//} EnumDSSRWControlFormatType;
//
//typedef enum EnumDSSRWNodeFormatType
//{
//    DssRWNodeFormatReserved = 0,
//    DssRWNodeFormatNode = 1,
//    DssRWNodeFormatTitle = 2,
//    DssRWNodeFormatLastOne
//} EnumDSSRWNodeFormatType;

typedef enum EnumDSSRWLayoutFlag
{
    DssRWLayoutDefault =	0,
    DssRWLayoutTab =	0x01
} EnumDSSRWLayoutFlag;

typedef enum EnumDSSDocumentActions
{
    DssDocumentActionReserved = 0x00000000,
    DssDocumentActionExecute = 0x00000001,
    DssDocumentActionResolve = 0x00000002,
    DssDocumentActionPersonalize = 0x00000004,
    DssDocumentActionRender = 0x00000008,
    DssDocumentActionExecuteAll = 0x0000000F,
    DssDocumentActionExportPDF = 0x10000000,
    DssDocumentActionExportExcel = 0x20000000,
    DssDocumentActionExportCSV = 0x40000000,
    DssDocumentActionExportHTML = 0x80000000,
    DssDocumentActionExportFlash = 0x04000000,
    DssDocumentFormatEngineExport = 0xF4000000,
    DssDocumentNewStream = DssReportNewSerialization
} EnumDSSDocumentActions;

typedef enum EnumDSSDocumentFlags
{
    DssDocumentExecuteReserved = 0x00000000,
    DssDocumentExecuteRefresh = 0x00000001,
    DssDocumentExecuteHTML = 0x00000002,
    DssDocumentExecuteXML = 0x00000004,
    DssDocumentExecuteReport = 0x00000008,
    DssDocumentExecuteGraphs = 0x00000010,
    DssDocumentExecuteInboxKeepAsIs = 0x00000020,
    DssDocumentExecuteSaveToInbox = 0x00000040,
    DssDocumentExecuteHTMLCache = 0x00000080,
    DssDocumentExecuteRunAsynch = 0x00000200,
    DssDocumentExecuteDefaultAutoprompt = 0x00000400,
    DssDocumentExecuteStatic = 0x00000800,
    DssDocumentExecuteSerializeReportInstance = 0x00000100,
    DssDocumentExecuteNewStream2 = 0x00100000,
    DssDocumentSerializeNoExportResults = 0x00200000,
    DssDocumentSerializeForCache = 0x00800000,
    DssDocumentExecuteExportCurrent = 0x00001000,
    DssDocumentExecuteExportAll = 0x00002000,
    DssDocumentExecuteUseRWDCache = 0x00004000,
    DssDocumentExecuteUpdateRWDCache = 0x00008000,
    DssDocumentExecuteNoUpdateDatasetCache = 0x00010000,
    DssDocumentSerializeGetDocumentDetail = DssReportDetailSerialization,
    DssDocumentExecuteReprompt = 0x00400000,
    DssDocumentExecuteFlash = 0x08000000,
    DssDocumentExecuteIPadBinary = 0x02000000,
    DssDocumentExecuteOnBackground = 0x00080000
} EnumDSSDocumentFlags;

typedef enum EnumDSSDocumentStatus
{
    DssDocumentStatusReserved = 0,
    DssDocumentStatusCompleted,
    DssDocumentStatusInProgress,
    DssDocumentStatusError,
    DssDocumentStatusPrompts,
    DssDocumentStatusCanceled,
    DssDocumentStatusExecutionInProgressServer
} EnumDSSDocumentStatus;

typedef enum EnumDSSDocumentXMLFlags
{
    DssDocumentXMLDefinition = 0x0001,
    DssDocumentXMLData = 0x0002,
    DssDocumentXMLResults = 0x0004,
    DssDocumentXMLID = 0x0008,
    DssDocumentXMLDates = 0x0010,
    DssDocumentXMLBrowser = 0x0020,
    DssDocumentXMLProperties = 0x0040,
    DssDocumentXMLErrors = 0x0080,
    DssDocumentXMLPartialPageByTree = 0x0100,
    DssDocumentXMLFlexDefinition = 0x0400,
    DssDocumentXMLFlexXTabData = 0x0800,
    DssDocumentXMLFlexResource = 0x1000,
    DssDocumentXMLDrill = 0x0200,
    DSSDocumentXMLDocumentDetails = 0x2000,
    DssDocumentXMLFlexVizDefinition = 0x4000,
    DssDocumentXMLDefinitionCurrentPanel = 0x00008000,
    DssDocumentXMLFullPageByTree = 0x00010000,
    DssDocumentXMLInstanceResolution = 0x00020000,
    DssDocumentXMLPartialCSSRetrieval = 0x00040000,
    DssDocumentXMLDefinitionLayout = 0x00080000,
    DssDocumentXMLOptimizedPageByTree = 0x00100000,
    DssDocumentXMLPageByTreeCurrentElementOnly = 0x00200000,
    DssDocumentXMLPartialUpdate = 0x00400000 ,
    DssDocumentXMLTransactionReportDefinition = 0x00800000,
    DssDocumentXMLTransactionReportData = 0x10000000,
    DssDocumentXMLCheckFlashBinary = 0x04000000,
    DssDocumentXMLAllDataSets = 0x40000000
} EnumDSSDocumentXMLFlags;

typedef enum EnumDSSDocumentServerConnectionMode
{
    DssDocumentServerConnectionModeReserved = 0,
    DssDocumentServerConnectionServer,
    DssDocumentServerConnectionClient
} EnumDSSDocumentServerConnectionMode;

typedef enum EnumDSSDocumentElementType
{
    DssDocumentElementReserved = 0,
    DssDocumentElementDSSObject,
    DssDocumentElementDirect,
    DssDocumentElementURL
} EnumDSSDocumentElementType;

typedef enum EnumDSSDocumentReportMode
{
    DssDocumentReportModeReserved = 0,
    DssDocumentReportModeGrid,
    DssDocumentReportModeGraph
} EnumDSSDocumentReportMode;

typedef enum EnumDSSDocumentStates
{
    DssDocumentImagePath2 =			0x00000001,
    DssDocumentResolutionDone =			0x00000002,
    DssDocumentReportsResultReady =			0x00000004,
    DssDocumentDataPreparationDone =			0x00000008,
    DssDocumentExportingDone =			0x00000010,
    DssDocumentDataElementListReady =			0x00000020,
    DssDocumentGroupByElementListReady =		0x00000040,
    DssDocumentDataViewsReady =			0x00000080,
    DssDocumentRWAll =			0x000000ff,
    DssDocumentSelectionCacheReady =	0x01000000,
    DssDocumentHasControls =			0x02000000,
    DssDocumentHasSubtotalMaps =		0x04000000,
    DssDocumentHasGraphs =			0x08000000,
    DssDocumentDefinitionModified =			0x10000000,
    DssDocumentInitialDefinitionDirty =			0x20000000,
    DssDocumentDefinitionModified2 =			0x40000000,
    DssDocumentNotExecuted =			0x80000000,
    DssDocumentIFIndexTreeReady =		0x00000100,
    DssDocumentViewsEvaluationReady =			0x00000200,
    DssDocumentViewsRemoveReady =			0x00000400,
    DssDocumentViewsHaveBeenEvaluated =			0x00000800,
    DssDocumentPartialLoaded =			0x00100000,
    DssDocumentHitCache =				0x00200000,
    DssDocumentConvertedFromReport =	0x00400000,
    DssDocumentSecurityChecked =				0x00080000
} EnumDSSDocumentStates;

typedef enum EnumDSSDocumentResolutionFlag
{
    EnumDSSDocumentResolutionInitialize = 0,
    EnumDSSDocumentResolutionSubstitute = 1
} EnumDSSDocumentResolutionFlag;

typedef enum EnumDSSRWEaseCreationFlags
{
    DssRWEaseCreationDefault = 0x00000000,
    DssRWEaseCreationIDIdentical = 0x00000001,
    DssRWEaseCreationLinkReportView = 0x00000002,
    DssRWEaseCreationEmbedReportInstance = 0x00000004,
    DssRWEaseCreationLiteDocument = 0x00000008,
    DssRWEaseCreationAnalysisDocument = 0x00000010
} EnumDSSRWEaseCreationFlags;

typedef enum EnumDSSDocumentExecutionSetting
{
    DssDocumentExecutionSettingSkipHistory = 0,
    DssDocumentExecutionSettingActionHistory = 1,
    DssDocumentExecutionCreationFlags = 2,
    DssDocumentExecutionSettingLastOne
} EnumDSSDocumentExecutionSetting;

typedef enum EnumDSSCachedReportStates
{
    DssCachedReportStatusReserved =		0x0,
    DssCachedReportStatusReady =		0x00000001,
    DssCachedReportStatusProcessing =	0x00000002,
    DssCachedReportStatusInvalid =		0x00000004,
    DssCachedReportStatusExpired =		0x00000008,
    DssCachedReportStatusLoaded =		0x00000010,
    DssCachedReportStatusUpdated =		0x00000020,
    DssCachedReportStatusDirty =		0x00000040,
    DssCachedReportStatusWaitToRemove =	0x00000080,
    DssCachedReportStatusFiled =		0x00000100,
    DssCachedReportStatusResultDirty =	0x00000200,
    DssCachedReportStatusDeleted =		0x00000400,
    DssCachedReportStatusXMLCacheDirty =0x00000800,
    DssCachedReportStatusProcessPending =0x00001000,
    DssCachedReportStatusUpdating =		0x00002000
} EnumDSSCachedReportStates;

typedef enum EnumDSSReportCacheInfoOptions
{
    DssCacheInfoBrowsing = 0x01,
    DssCacheInfoDetail = 0x02,
    DssCacheInfoClusterSynch = 0x04,
    DssCacheInfoEnhanced = 0x08,
    DssCacheInfoCheck = 0x1000,
    DssCacheInfoIncludeForeign = 0x2000
} EnumDSSReportCacheInfoOptions;

typedef enum EnumDSSCachingIDType
{
    DssCachingIDTypeReserved = 0,
    DssCachingReportID = 0x01,
    DssCachingCacheID = 0x02,
    DssCachingTableName = 0x03,
    DssCachingSecurityFilterID = 0x04,
    DssCachingReportIDAndSFID = 0x05,
    DssCachingDataLocale = 0x06,
    DssCachingUserID = 0x07,
    DssCachingDBLoginID = 0x08,
    DssCachingDBConnectionID = 0x09,
    DssCachingWHLoginName = 0x0A,
    DssCachingCreator = 0x0B,
    DssCachingReportIDAndUserID = 0x0C
} EnumDSSCachingIDType;

typedef enum EnumClusterCacheUpdateOperation
{
    DssClusterAddCacheEntry = 0x01,
    DssClusterDeleteCacheEntry = 0x02,
    DssClusterRefreshLKUPTable = 0x04,
    DssClusterAddInboxRefCount = 0x08,
    DssClusterRelInboxRefCount = 0x10,
    DssClusterUpdateCacheEntry = 0x20,
    DssClusterExpireCacheEntry = 0x40,
    DssClusterInvalidateCacheEntry = 0x80,
    DssClusterDeleteAllCacheEntry = 0x100,
    DssClusterExpireAllCacheEntry = 0x200,
    DssClusterInvalidateAllCacheEntry = 0x400
} EnumClusterCacheUpdateOperation;

typedef enum EnumDSSCacheTypes
{
    DssCacheTypeReserved =			0x0,
    DssCacheTypeMatchingCache =		0x00000001,
    DssCacheTypeResultCache =		0x00000002,
    DssCacheTypeXMLPage =			0x00000004,
    DssCacheTypeForeignCache =		0x00000008,
    DssCacheTypePDFCache =			0x00000010,
    DssCacheTypeExcelCache =			0x00000020,
    DssCacheTypeCSVCache =			0x00000040,
    DssCacheTypeFlashCache =			0x00000080,
    DssCacheTypeHtmlCache =			0x00000100,
    DssCacheTypeExportCache = DssCacheTypePDFCache  | DssCacheTypeExcelCache | DssCacheTypeCSVCache | DssCacheTypeFlashCache | DssCacheTypeHtmlCache
} EnumDSSCacheTypes;

typedef enum EnumDSSReportViewFlags
{
    DssRequestAttributeInColumn = 0x00000010
} EnumDSSReportViewFlags;

typedef enum EnumDSSRequestFlags
{
    DssRequestReserved = 0,
    DssRequestGraphResult = 0x00000001,
    DssRequestNoGridResult = 0x00000002,
    DssRequestReportUseServerCache = 0x00000004,
    DssRequestReportUpdateServerCache = 0x00000008,
    DssRequestResolutionInFullXML = 0x00000020,
    DssRequestNewReportSerialization = 0x00000040,
    DssRequestNewReportSerializationInServer = 0x00000080,
    DssRequestDocument = 0x00000100,
    DssRequestNewBatchRequest = 0x00000200,
    DssRequestObjectOnTheFly = 0x00000400,
    DssRequestRequestSubJobIDs = 0x00000800,
    DssRequestRequestSubJobIDsComform = 0x00001000,
    DssRequestResultWithSubJobIDs = 0x00002000,
    DssRequestWithLocales = 0x00004000,
    DssRequestResultWithExportContext = 0x00008000,
    DssRequestReportAllServerCache = DssRequestReportUseServerCache  | DssRequestReportUpdateServerCache
} EnumDSSRequestFlags;

typedef enum EnumDSSEventServicesObjectType
{
    DssEventServicesObjectTypeMD = 0,
    DssEventServicesObjectTypeIndexMD =1,
    DssEventServicesObjectTypeBinaryCollection =2
} EnumDSSEventReceiverType;

typedef enum EnumLimitPolicy
{
    DssLimitPolicyAbsolute = 0x0001,
    DssLimitPolicyPercentage = 0x0002
} EnumLimitPolicy;

typedef enum EnumDSSRWApplySelectionFlag
{
    DssRWApplySelectionDefault =	0,
    DssRWApplySelectionMatchControlSourceID =	0x01,
    DssRWApplySelectionMatchControlName = 0x02
} EnumDSSRWApplySelectionFlag;

typedef enum EnumDSSMonitorType
{
    DssMonitorTypeReserved,
    DssMonitorTypePerformanceCounter,
    DssMonitorTypeJobs,
    DssMonitorUserConnections,
    DssMonitorDBConnections,
    DssMonitorTypeLastOne
} EnumDSSMonitorType;

typedef enum EnumCellFormatDataTypes
{
    DssCellFormatDataTypeReserved = 0,
    DssCellFormatDataTypeDouble = 1,
    DssCellFormatDataTypeMIX = 2,
    DssCellFormatDataTypePercent = 7,
    DssCellFormatDataTypeTestStart = 100,
    DssCellFormatDataTypeUSD = 101,
    DssCellFormatDataTypeRMB = 102,
    DssCellFormatDataTypeOthersStart = 300
} EnumCellFormatDataTypes;

typedef enum EnumDSSSupportedFormatTypes
{
    DssSupportedFormatTypeReserved = 0,
    DssSupportedFormatTypeText = 0x00000001,
    DssSupportedFormatTypeExcel = 0x00000002,
    DssSupportedFormatTypePDF = 0x00000004,
    DssSupportedFormatTypeHTMLWithoutImages = 0x00000008,
    DssSupportedFormatTypeHTMLWithImages = 0x00000010
} EnumDSSSupportedFormatTypes;

typedef enum EnumDSSMDUpdateFlags
{
    DssMDUpdateFlagDefault = 0,
    DssMDUpdateFlagNeedUpdateCheckOnly = 1,
    DssMDUpdateFlagQuickCheck = 0x00010000
} EnumDSSMDUpdateFlags;

typedef enum EnumDSSCubeStates
{
    DssCubeReserved =	0x0,
    DssCubeProcessing =	0x00000001,
    DssCubeActive =	0x00000002,
    DssCubePersisted =	0x00000004,
    DssCubeInfoDirty =	0x00000008,
    DssCubeDirty =	0x00000010,
    DssCubeLoaded =	0x00000020,
    DssCubeReady =	0x00000040,
    DssCubeLoadPending =	0x00000080,
    DssCubeUnloadPending =	0x00000100,
    DssCubePendingForEngine =	0x00000200,
    DssCubeImported =	0x00000400,
    DssCubeForeign =	0x00000800
} EnumDSSCubeStates;

typedef enum EnumDSSCubeInfoOptions
{
    DssCubeInfoBrowsing = 0x01,
    DssCubeInfoDetail = 0x02,
    DssCubeInfoIncludeForeign = 0x1000
} EnumDSSCubeInfoOptions;

typedef enum EnumDSSIDType
{
    DssCubeIDTypeReserved = 0,
    DssCubeDefnID = 0x01,
    DssCubeInstID = 0x02
} EnumDSSIDType;

typedef enum EnumClusterCubeUpdateOperation
{
    DssClusterAddCubeEntry = 0x01,
    DssClusterDeleteCubeEntry = 0x02,
    DssClusterActivateCubeEntry = 0x03,
    DssClusterDeactivateCubeEntry = 0x04,
    DssClusterPersistCubeEntry = 0x05
} EnumClusterCubeUpdateOperation;

typedef enum EnumDSSContextFlags
{
    DssContextFlagDefault = 0x00000000,
    DssContextFlagTemporary = 0x00000001,
    DssContextFlagProbeOnly = 0x00000002,
    DssContextFlagNoExternal = 0x00000004,
    DssContextFlagSupportLookUpByID = 0x00000008,
    DssContextFlagNoDependentsImport = 0x00000010,
    DssContextFlagNoIndirectImport = 0x00000020,
    DssContextFlagIterateUsingEdit = 0x0000040,
    DssContextFlagIterateOverEdit = 0x0000080,
    DssContextFlagIterateMerged = 0x0000100,
    DssContextFlagIterateChild = 0x0000200,
    DssContextFlagFindBySingleIDAllContexts = 0x00000400,
    DssContextFlagConsiderRecentChanges = 0x00000800,
    DssContextFlagSaveOverwrite = 0x00010000,
    DssContextFlagSavePreserveVersion = 0x00020000,
    DssContextFlagSaveSkipNameTypeUniqueCheck = 0x0008000,
    DssContextFlagCopyEditContext = 0x00010000,
    DssContextFlagCopyReuseEdit = 0x00020000,
    DssContextFlagCopyFoldInEditContext = 0x00040000,
    DssContextFlagSaveToStreamAll = 0x00010000,
    DssContextFlagSaveToStreamNoReferenced = 0x00020000,
    DssContextFlagInsertAsReferenced = 0x00010000,
    DssContextFlagInsertNoOwnedCopy = 0x00020000,
    DssContextFlagIterateExcludeEdited = 0x00010000,
    DssContextEditContextCheckedOut = 0x00080000
} EnumDSSContextFlags;

typedef enum EnumDSSContextStatus
{
    DssContextReserved = 0,
    DssContextUnspecified = 1,
    DssContextExternal = 2,
    DssContextOwned = 3,
    DssContextCheckedOut = 4,
    DssContextMissing = 5,
    DssContextReferenced = 6
} EnumDSSContextStatus;

typedef enum EnumDSSGenericsTemporality
{
    DssGenericsReserved = 0,
    DssGenericsTransient = 4,
    DssGenericsTemporary = 3,
    DssGenericsPermanent = 2,
    DssGenericsDynamic = 1
} EnumDSSGenericsTemporality;

typedef enum EnumDSSConflictResolution
{
    DssConflictNone = 0,
    DssConflictExisting = 0x01,
    DssConflictReplace = 0x02,
    DssConflictEarlier = 0x03,
    DssConflictLater = 0x04,
    DssConflictKeepBoth = 0x05,
    DssConflictMask = 0x0f,
    DssConflictForce = 0x10,
    DssConflictPreserveHidden = 0x20,
    DssConflictForceKeepBoth = DssConflictForce  | DssConflictKeepBoth,
    DssConflictForceReplace = DssConflictForce  | DssConflictReplace
} EnumDSSConflictResolution;

typedef enum EnumDSSConflictDomain
{
    DssConflictReserved = 0,
    DssConflictExplicitObject = 1,
    DssConflictFolder = 2,
    DssConflictObjectType = 3,
    DssConflictApplication = 4,
    DssConflictSchema = 5,
    DssConflictConfiguration = 6,
    DssConflictAll = 7
} EnumDSSConflictDomain;

typedef enum EnumDSSMergeServicesFlags
{
    DssMergeServiceDefault = 0,
    DssMergeServiceLocalOnly = 0x00000001
} EnumDSSMergeServicesFlags;

typedef enum EnumDSSSourceManipulationCommands
{
    DssSourceManipulationReserved = 0,
    DssSourceManipulationMergeFile = 1,
    DssSourceManipulatorUpdateSchema = 2,
    DssSourceManipulatorCreateUndoFile = 3,
    DssSourceManipulatorCreatePackage = 4,
    DssSourceManipulationReadManifest = 5,
    DssSourceManipulationSearchReplacePackage = 6,
    DssSourceManipulationReadManifestFromObject = 7
} EnumDSSSourceManipulationCommands;

typedef enum EnumDSSSourceManipulationSchema
{
    DssSourceManipulationTableKey = 0x00000001,
    DssSourceManipulationEntryLevel = 0x00000002,
    DssSourceManipulationLogicalSize = 0x00000004,
    DssSourceManipulationSchemaAll = 0x0000ffff
} EnumDSSSourceManipulationSchema;

typedef enum EnumDSSInheritACL
{
    DssInheritACLReserved = 0,
    DssInheritACLOriginal = 0x00000001,
    DssInheritACLInherit = 0x00000002,
    DssInheritACLUserFullControl = 0x00000004,
    DssInheritACLNormal = DssInheritACLOriginal  | DssInheritACLInherit,
    DssInheritACLAll = DssInheritACLNormal  | DssInheritACLUserFullControl,
    DssInheritACLInheritableFolderOnly = DssInheritACLInherit
} EnumDSSInheritACL;

typedef enum EnumDSSSourceManipulatorOp
{
    DssSourceManipulatorOpReserved = 0,
    DssSourceManipulatorOpFind = 1,
    DssSourceManipulatorOpMerge = 2,
    DssSourceManipulatorOpDelete = 3,
    DssSourceManipulatorOpSave = 4,
    DssSourceManipulatorOpDependents = 5
} EnumDSSSourceManipulatorOp;

typedef enum EnumDSSWeakReferenceType
{
    DssWeakReferenceNone = 0,
    DssWeakReferenceAttributeTables = 0x00000001,
    DssWeakReferenceFactTables = 0x00000002,
    DssWeakReferenceParentAttributes = 0x00000004,
    DssWeakReferenceParentGroups = 0x00000008,
    DssWeakReferenceChildAttributes = 0x00000010,
    DssWeakReferenceChildGuide = 0x00000020,
    DssWeakReferenceMDXCubeObjectSpace = 0x00000040,
    DssWeakReferenceAttributeMDXCube = 0x00000080,
    DssWeakReferenceRoleAttributeTables = 0x00000100,
    DssWeakReferenceTaggedObjects = 0x00000200,
    DssWeakReferenceAttributeJointChildren = 0x00000400,
    DssWeakReferenceAll = 0x3fffffff,
    DssWeakReferenceAttributeMainTable = 0x40000000
} EnumDSSWeakReferenceType;

typedef enum EnumDSSExtendedLoadLevels
{
    DssExtendedLoadNone = 0x00000000,
    DssExtendedLoadCore = 0x00000001,
    DssExtendedLoadDependencies = 0x00000002,
    DssExtendedLoadTranslate = 0x00000004,
    DssExtendedLoadMergeDirect = 0x00000010,
    DssExtendedLoadPath = 0x00000020,
    DssExtendedLoadRare = 0x00000040,
    DssExtendedLoadAll = 0x0000ffff,
    DssExtendedLoadPropertySets = 0x00010000,
    DssExtendedLoadFromStream = 0x00020000
} EnumDSSExtendedLoadLevels;

typedef enum EnumDSSMergeReconciliation
{
    DssMergeReconciliationNone = 0x00,
    DssMergeReconciliationUseScoped = 0x10,
    DssMergeReconciliationExport = 0x20,
    DssMergeReconciliationCollect = 0x40,
    DssMergeReconciliationQuick = DssMergeReconciliationExport
} EnumDSSMergeReconciliation;

typedef enum EnumDSSExtendedItem
{
    DssExtendedItemReserved = 0x0,
    DssExtendedItemIsPrimaryDataset = 0x00000001,
    DssExtendedItemUseName = 0x00000002,
    DssExtendedItemUseDescription = 0x00000003,
    DssExtendedItemMultilingualUseName = 0x00000004,
    DssExtendedItemMultilingualUseDescription = 0x00000005,
    DssExtendedItemLastOne
} EnumDSSExtendedItem;

typedef enum EnumDSSParserFlags
{
    DssParserFlagReserved = 0,
    DssParserFlagRWExpression = 0x0002,
    DssParserFlagAllNumberBigDecimal = 0x0004,
    DssParserFlagCheckWarehouseName = 0x1000
} EnumDSSParserFlags;

typedef enum EnumDSSTokenType
{
    DssTokenTypeEndOfInput =  -1,
    DssTokenTypeReserved =   0,
    DssTokenTypeError =   1,
    DssTokenTypeUnknown =   2,
    DssTokenTypeEmpty =   3,
    DssTokenTypeExclamationPoint =  33,
    DssTokenTypeNumberSign =  35,
    DssTokenTypeDollar =  36,
    DssTokenTypePercent =  37,
    DssTokenTypeAmpersand =  38,
    DssTokenTypeLeftParen =  40,
    DssTokenTypeRightParen =  41,
    DssTokenTypeAsterisk =  42,
    DssTokenTypePlusSign =  43,
    DssTokenTypeComma =  44,
    DssTokenTypeMinusSign =  45,
    DssTokenTypeDot =  46,
    DssTokenTypeFwdSlash =  47,
    DssTokenTypeColon =  58,
    DssTokenTypeSemicolon =  59,
    DssTokenTypeLeftAngleBracket =  60,
    DssTokenTypeEqualSign =  61,
    DssTokenTypeRightAngleBracket = 62,
    DssTokenTypeQuestionMark =  63,
    DssTokenTypeAtSign =  64,
    DssTokenTypeLeftSquareBracket = 91,
    DssTokenTypeRightSquareBracket =93,
    DssTokenTypeCaret =  94,
    DssTokenTypeUnderscore =  95,
    DssTokenTypeLeftAccolade = 123,
    DssTokenTypeVerticalBar = 124,
    DssTokenTypeRightAccolade = 125,
    DssTokenTypeTilda = 126,
    DssTokenTypeLiteral = 258,
    DssTokenTypeIdentifier = 259,
    DssTokenTypeReserved1 = 260,
    DssTokenTypeDoubleQString = 261,
    DssTokenTypeInteger = 262,
    DssTokenTypeFloat = 263,
    DssTokenTypeAttrRef = 264,
    DssTokenTypeDimRef = 265,
    DssTokenTypeMetricRef = 266,
    DssTokenTypeAggMetricRef = 267,
    DssTokenTypeFactRef = 268,
    DssTokenTypeReportRef = 269,
    DssTokenTypeFilterRef = 270,
    DssTokenTypeRoleRef = 271,
    DssTokenTypeTableRef = 272,
    DssTokenTypePromptRef = 273,
    DssTokenTypeColumnRef = 274,
    DssTokenTypeAttrFormRef = 275,
    DssTokenTypeDimFormRef = 276,
    DssTokenTypeSimpInfixFun1 = 277,
    DssTokenTypeSimpInfixFun2 = 278,
    DssTokenTypeSimpPrefixFun = 279,
    DssTokenTypeAggFun = 280,
    DssTokenTypeRelativeFun = 281,
    DssTokenTypeCompareFun = 282,
    DssTokenTypeAndFun = 283,
    DssTokenTypeOrFun = 284,
    DssTokenTypeNotFun = 285,
    DssTokenTypeBoolPrefixFun = 286,
    DssTokenTypeIntersectFun = 287,
    DssTokenTypeReserved2 = 288,
    DssTokenTypeInSetFun = 289,
    DssTokenTypeBetweenFun = 290,
    DssTokenTypeLikeFun = 291,
    DssTokenTypeEnhCompareFun = 292,
    DssTokenTypeEnhBetweenFun = 293,
    DssTokenTypeArrowUp = 294,
    DssTokenTypeArrowDown = 295,
    DssTokenTypeArrowUpDown = 296,
    DssTokenTypeAggFirstInFact = 297,
    DssTokenTypeAggLastInFact = 298,
    DssTokenTypeKwdTrue = 299,
    DssTokenTypeKwdFalse = 300,
    DssTokenTypeKwdBreakBy = 301,
    DssTokenTypeKwdSortBy = 302,
    DssTokenTypeKwdFactID = 303,
    DssTokenTypeDateTime = 304,
    DssTokenTypeCaseFun = 305,
    DssTokenTypeFreeVariable = 306,
    DssTokenTypeLongNumber = 307,
    DssTokenTypeRWMetricRef = 308,
    DssTokenTypeCSQLRef = 309,
    DssTokenTypeConsolidationRef = 310,
    DssTokenTypeEndOfTypes
} EnumDSSTokenType;

typedef enum EnumDSSTokenLevel
{
    DssTokenLevelReserved =   0,
    DssTokenLevelClient =	1,
    DssTokenLevelLexed =	2,
    DssTokenLevelResolved =	3,
    DssTokenLevelParsed =	4
} EnumDSSTokenLevel;

typedef enum EnumDSSTokenState
{
    DssTokenStateError =  -1,
    DssTokenStateReserved =   0,
    DssTokenStateInitial =	1,
    DssTokenStateOkay =   2
} EnumDSSTokenState;

typedef enum EnumDSSParserOutFlags
{
    DssParserOutNone =	0x00000000,
    DssParserOutMetricObjectInfo =	0x00000001,
    DssParserOutMetricOptions =	0x00000002,
    DssParserOutMetricHeaderFormat =	0x00000004,
    DssParserOutMetricGridFormat =	0x00000008,
    DssParserOutMetricSubtotals =	0x00000010,
    DssParserOutMetricExtendedProperties =	0x00000020,
    DssParserOutTotalMetric =	0x0000FFFF,
    DssParserOutTokenStream =	0x00010000,
    DssParserOutLocalSymbolFolder =	0x00020000,
    DssParserOutSearchResultsFolder =	0x00040000
} EnumDSSParserOutFlags;

typedef enum EnumDSSMetricEditType
{
    DssMetricEditTypeReserved =	0x00000000,
    DssMetricEditTypeAdvancedMetric =	0x00010000,
    DssMetricEditTypeSimpleMetric =	0x00020000,
    DssMetricEditTypeFunctionMetric =	0x00030000
} EnumDSSMetricEditType;

typedef enum EnumDSSTokenSectionType
{
    DssTokenSectionTypeReserved =	0x00000000,
    DssTokenSectionTypeFunction =	0x00010000,
    DssTokenSectionTypeFunctionParameters =	0x00020000,
    DssTokenSectionTypeFunctionParameterSeparator =	0x00020001,
    DssTokenSectionTypeFunctionArguments =	0x00030000,
    DssTokenSectionTypeFunctionArgumentSeparator =	0x00030001,
    DssTokenSectionTypeDimensionality =	0x00040000,
    DssTokenSectionTypeDimensionalityUnitSeparator =	0x00040001,
    DssTokenSectionTypeConditionality =	0x00050000,
    DssTokenSectionTypeTransformations =	0x00060000,
    DssTokenSectionTypeTransformationsUnitSeparator =	0x00060001
} EnumDSSTokenSectionType;

typedef enum EnumDSSContentType
{
    DssContentTypeReserved = 0,
    DssContentMsg = 1,
    DssContentCache,
    DssContentTypeCount
} EnumDSSContentType;

typedef enum EnumDSSBooleanOperatorType
{
    DssBooleanOperatorTypeReserved = 0,
    DssBooleanOperatorAnd = 1,
    DssBooleanOperatorOr = 2,
    DssBooleanOperatorNot = 3
} EnumDSSBooleanOperatorType;

typedef enum EnumDSSBooleanExpressionNodeType
{
    DssBooleanExpressionNodeTypeReserved = 0,
    DssBooleanExpressionNodeLeaf = 1,
    DssBooleanExpressionNodeBranch = 2,
    DssPatternExpressionNodeLeaf = 3
} EnumDSSBooleanExpressionNodeType;

typedef enum EnumDSSQueryLiteObjectType
{
    DssQueryLiteObjectTypeReserved =	0,
    DssQueryLiteObjectSqlSimpleInsert = 1,
    DssQueryLiteObjectSqlParamInsert = 2,
    DssQueryLiteObjectSqlDelete = 3,
    DssQueryLiteObjectSqlSelect = 4,
    DssQueryLiteObjectWhereClause = 5,
    DssQueryLiteObjectQuery = 6,
    DssQueryLiteObjectBatchQuery = 7,
    DssQueryLiteObjectSelectQuery = 8,
    DssQueryLiteObjectParamInsertQuery = 9,
    DssQueryLiteObjectDbProfile = 10,
    DssQueryLiteObjectQueryResult = 11,
    DssQueryLiteObjectTableSchema = 12,
    DssQueryLiteObjectSqlUpdate = 13,
    DssQueryLiteObjectQueryCriteria = 14,
    DssQueryLiteObjectBinaryData = 15,
    DssQueryLiteObjectTableColumn = 16
} EnumDSSQueryLiteObjectType;

typedef enum EnumDSSContentServiceTable
{
    DssContentServiceTableReserved =	0,
    DssContentserviceTableContent = 1,
    DssContentserviceTableMsgInfo,
    DssContentserviceTableNCSSubInst,
    DssContentserviceTableNCSPersonalization,
    DssContentserviceTableNCSRelSubInstTrigger,
    DssContentserviceTableNCSContact,
    DssContentserviceTableNCSAddress,
    DssContentserviceTableNCSRelContactColl,
    DssContentServiceTableSysProperty,
    DssContentServiceTableCache
} EnumDSSContentServiceTable;

typedef enum EnumDSSContentServiceObjectType
{
    DssContentServiceObjectTypeReserved =	0,
    DssContentServiceObjectRepositoryProfile,
    DssContentServiceObjectDbProfile,
    DssContentServiceObjectRepository,
    DssContentServiceObjectContentCollection,
    DssContentServiceObjectContent,
    DssContentServiceObjectHLContentService,
    DssContentServiceObjectNCSContentService,
    DssContentServiceObjectQueryEx,
    DssContentServiceObjectHLSearch
} EnumDSSContentServiceObjectType;

typedef enum EnumDSSContentField
{
    DssContentFieldReserved =	-1,
    DssContentContentID = 0,
    DssContentSubID,
    DssContentSliceID,
    DssContentValSeq,
    DssContentRefCount,
    DssContentValue,
    DssContentTableCount
} EnumDSSContentField;

typedef enum EnumDSSContentVersion
{
    DssContentVersionReserved =	0,
    DssContentVersion1,
    DssContentVersionLast
} EnumDSSContentVersion;

typedef enum EnumDSSResultFlag
{
    DssResultFlagReserved = 0,
    DssResultSQL = 0x00000001,
    DssResultPromptAnswer = 0x00000002,
    DssResultPDF = 0x00000010,
    DssResultExcel = 0x00000020,
    DssResultCSV = 0x00000040,
    DssResultDocHTML = 0x00000080,
    DssResultDocStream = 0x00000100,
    DssResultViewSettings = 0x00000200,
    DssResultRptDefn = 0x00000400,
    DssResultCache = 0x00000800,
    DssResultFlash = 0x00001000,
    DssResultRWDBinary = 0x00002000,
    DssResultFlashInPdf = 0x00004000,
    DssResultFlagCount = 13,
    DssResultExportFormats = DssResultPDF  | DssResultExcel | DssResultCSV | DssResultFlash | DssResultFlashInPdf,
    DssResultAll = -1
} EnumDSSResultFlag;

typedef enum EnumDSSBinaryDataLockType
{
    DssBinaryDataLockTypeReserved =	0,
    DssBinaryDataLockRead,
    DssBinaryDataLockWrite
} EnumDSSBinaryDataLockType;

typedef enum EnumDSSNCSPersonalizationType
{
    DssNCSPersonalizationTypeReserved =	0,
    DssNCSPersonalizationPrompt = 0x10000000,
    DssNCSPersonalizationDeltas = 0x20000000,
    DssNCSPersonalizationData = 0x40000000,
    DssNCSPersonalizationDelivery = 0x80000000,
    DssNCSPersonalizationTransmission = 0x01000000,
    DssNCSPersonalizationAlert = 0x02000000,
    DssNCSPersonalizationExtraXMLInfo = 0x04000000,
    DssNCSPersonalizationBurst = 0x08000000,
    DssNCSPersonalizationDRLPrompt = 0x00000001,
    DssNCSPersonalizationDRLDeliverySetting = 0x00000002,
    DssNCSPersonalizationDRLRecipientDefForGeneric = 0x00000010,
    DssNCSPersonalizationDRLRecipientDefForEmail = 0x00000020,
    DssNCSPersonalizationDRLRecipientDefForFile = 0x00000040,
    DssNCSPersonalizationDRLRecipientDefForPrint = 0x00000080,
    DssNCSPersonalizationTypeCount = 14,
    DssNCSPersonalizationDRLRecipientDef = 0x000000f0
} EnumDSSNCSPersonalizationType;

typedef enum EnumDSSNCSPersonalizationField
{
    DssNCSPersonalizationFieldReserved =	-1,
    DssNCSPersonalizationInstID = 0,
    DssNCSPersonalizationSubID,
    DssNCSPersonalizationValSeq,
    DssNCSPersonalizationValue,
    DssNCSPersonalizationFieldCount
} EnumDSSNCSPersonalizationField;

typedef enum EnumDSSNCSRelSubInstTriggerField
{
    DssNCSRelSubInstTriggerFieldReserved =	-1,
    DssNCSRelSubInstTriggerInstID = 0,
    DssNCSRelSubInstTriggerTriggerID,
    DssNCSRelSubInstTriggerFieldCount
} EnumDSSNCSRelSubInstTriggerField;

typedef enum EnumDSSNCSDeliveryAgentType
{
    DssNCSDeliveryAgentTypeReserved =	0,
    DssNCSDeliveryAgentTypeDevice,
    DssNCSDeliveryAgentTypeTransmitter
} EnumDSSNCSDeliveryAgentType;

typedef enum EnumDSSNCSRelContactCollField
{
    DssNCSRelContactCollFieldReserved =	-1,
    DssNCSRelContactCollContactID = 0,
    DssNCSRelContactCollCollID,
    DssNCSRelContactCollFieldCount
} EnumDSSNCSRelContactCollField;

typedef enum DssCSProperty
{
    DssCSPropertyReserved =	-1,
    DssCSPropSqlEncodingType = 0,
    DssCSPropCharChunkSize,
    DssCSPropBinChunkSize,
    DssCSPropInListMax
} DssCSProperty;

typedef enum EnumDSSFuncPtn
{
    EnumCDSSFuncPtnMODReserved =	-1,
    EnumDSSFuncPtnMOD = 0,
    EnumDSSFuncPtnFLOOR = 1,
    EnumDSSFuncPtnFieldCount
} EnumDSSFuncPtn;

//typedef enum EnumDSSGraphObject
//{
//    DssGraphNull = -3,
//    DssGraphBackground = 1,
//    DssGraphFrame = 2,
//    DssGraphFootnote = 3,
//    DssGraphSubtitle = 4,
//    DssGraphTitle = 5,
//    DssGraphLegendArea = 6,
//    DssGraphLegendLine = 7,
//    DssGraphLegendMarker = 8,
//    DssGraphLegendText = 9,
//    DssGraphCurve = 257,
//    DssGraphDataLine = 258,
//    DssGraphDataMarker = 259,
//    DssGraphDataText = 260,
//    DssGraphDivBiPolar = 261,
//    DssGraphErrorBar = 262,
//    DssGraphLinrExp = 263,
//    DssGraphLinrLine = 264,
//    DssGraphLinrLog = 265,
//    DssGraphLinrNatLog = 266,
//    DssGraphLinrNPoly = 267,
//    DssGraphLinrText = 268,
//    DssGraphMeanLine = 269,
//    DssGraphMovAvgLine = 270,
//    DssGraphO1Body = 271,
//    DssGraph2DO1Label = 272,
//    DssGraphO1Major = 273,
//    DssGraphO1Minor = 274,
//    DssGraphO1Super = 275,
//    DssGraphO1Title = 276,
//    DssGraphO2Body = 277,
//    DssGraph2DO2Label = 278,
//    DssGraphO2Major = 279,
//    DssGraphO2Minor = 280,
//    DssGraphO2Title = 281,
//    DssGraphConnectDataLine = 282,
//    DssGraphRiser = 286,
//    DssGraphRiserShadow = 287,
//    DssGraphStdDevLine = 288,
//    DssGraphX1Body = 289,
//    DssGraphX1Label = 290,
//    DssGraphX1Major = 291,
//    DssGraphX1Minor = 292,
//    DssGraphXTitle = 293,
//    DssGraphX1Zero = 294,
//    DssGraphY1Body = 295,
//    DssGraphY1Label = 296,
//    DssGraphY1Major = 297,
//    DssGraphY1Minor = 298,
//    DssGraphY1Title = 299,
//    DssGraphY1Zero = 300,
//    DssGraphY2Body = 301,
//    DssGraphY2Label = 302,
//    DssGraphY2Major = 303,
//    DssGraphY2Minor = 304,
//    DssGraphY2Title = 305,
//    DssGraphY2Zero = 306,
//    DssGraph3DShowColumnHeaders = 318,
//    DssGraph3DShowLeftHeaders = 321,
//    DssGraph3DShowRightHeaders = 323,
//    DssGraph3DShowRowHeaders = 325,
//    DssGraphOBPBox = 329,
//    DssGraphOBPMedian = 330,
//    DssGraphOBPStem = 331,
//    DssGraphOBPTail = 332,
//    DssGraphPieFeelerLine = 337,
//    DssGraphPieFeelerLabel = 338,
//    DssGraphPieGroupLabel = 339,
//    DssGraphPieRingLabel = 340,
//    DssGraphPieSlice = 341,
//    DssGraphPieSliceCrust = 342,
//    DssGraphPieSliceFace = 343,
//    DssGraphPieSliceRing = 344,
//    DssGraphPolarAxisCircles = 346,
//    DssGraphPolarAxisLabels = 347,
//    DssGraphPolarAxisLines = 348,
//    DssGraphPolarAxisThick = 349,
//    DssGraphBubbleQuadLine = 363,
//    DssGraphNestedLabelFont = 364,
//    DssGraphRiser2 = 396,
//    DssGraphRiser3 = 397,
//    DssGraphSeriesTitle = 398,
//    DssGraphConnectStackLine = 399,
//    DssGraphDataTextStackTotal = 401,
//    DssGraphOPILBLValue = 418,
//    DssGraphUserLine1 = 414,
//    DssGraphUserLine2 = 415,
//    DssGraphUserLine3 = 416,
//    DssGraphUserLine4 = 417,
//    DssGraphDataTextConnectLine = 431,
//    DssGraphGauge = 444,
//    DssGraphGaugeTitle = 450,
//    DssGraphGaugeRange = 451,
//    DssGraphY3Body = 477,
//    DssGraphY3Label = 478,
//    DssGraphY3Major = 479,
//    DssGraphY3Minor = 480,
//    DssGraphY4Body = 483,
//    DssGraphY4Label = 484,
//    DssGraphY4Major = 485,
//    DssGraphY4Minor = 486,
//    DssGraph3DColHeader = 512,
//    DssGraph3DColHeaderBox = 514,
//    DssGraph3DColTitle = 515,
//    DssGraph3DColTitleBox = 517,
//    DssGraph3DDataText = 518,
//    DssGraph3DDataLabelBox = 519,
//    DssGraph3DFloor1 = 521,
//    DssGraph3DFloor2 = 522,
//    DssGraph3DFloor3 = 523,
//    DssGraph3DFrameGridX = 524,
//    DssGraph3DFrameGridY = 525,
//    DssGraph3DFrameGridZ = 526,
//    DssGraph3DLastRiserFace = 527,
//    DssGraph3DLeftWall1 = 528,
//    DssGraph3DLeftWall2 = 529,
//    DssGraph3DLeftWall3 = 530,
//    DssGraph3DLYHeader = 531,
//    DssGraph3DLYHeaderBox = 533,
//    DssGraph3DLYTitle = 534,
//    DssGraph3DLYTitleBox = 536,
//    DssGraph3DRightWall1 = 537,
//    DssGraph3DRightWall2 = 538,
//    DssGraph3DRightWall3 = 539,
//    DssGraph3DRiserFace1 = 540,
//    DssGraph3DRiserFace2 = 541,
//    DssGraph3DRiserFace3 = 542,
//    DssGraph3DRiserFace4 = 543,
//    DssGraph3DRiserFace5 = 544,
//    DssGraph3DRiserFace6 = 545,
//    DssGraph3DRiserFace7 = 546,
//    DssGraph3DRiserFace8 = 547,
//    DssGraph3DRiserFace9 = 548,
//    DssGraph3DRiserGridX = 549,
//    DssGraph3DRiserGridY = 550,
//    DssGraph3DRiserGridZ = 551,
//    DssGraph3DRowHeader = 552,
//    DssGraph3DRowHeaderBox = 554,
//    DssGraph3DRowTitle = 555,
//    DssGraph3DRowTitleBox = 557,
//    DssGraph3DRYHeader = 558,
//    DssGraph3DRYHeaderBox = 560,
//    DssGraph3DRYTitle = 561,
//    DssGraph3DRYTitleBox = 563,
//    DssGraph3DScatterLine = 564,
//    DssGraph3DTieLineX = 565,
//    DssGraph3DTieLineY = 566,
//    DssGraph3DTieLineZ = 567,
//    DssGraph3DRiserVarFace = 568,
//    DssGraphGaugeBorder = 1319,
//    DssGraphGaugeNeedleBase = 1320,
//    DssGraphGaugeNeedleBaseLine = 1330,
//    DssGraphGaugeRangeLine = 1331,
//    DssGraphAllTexts = 10000,
//    DssGraphAllTitles = 10001,
//    DssGraphAllAxes = 10002,
//    DssGraphZBody = 10003,
//    DssGraphTrendLine = 10004,
//    DssGraphY1RightBody = 10005,
//    DssGraphY1RightTitle = 10006
//} EnumDSSGraphObject;

//typedef enum EnumDSSGraphProperty
//{
//    DssGraphTextContent = 9999,
//    DssGraphContent = 9999,
//    DssGraphPropertyReserved = 0,
//    DssGraphAutoFitO1 = 4,
//    DssGraphAutoFitO2 = 5,
//    DssGraphAutoFitX = 6,
//    DssGraphAutoFitY1 = 7,
//    DssGraphAutoFitY2 = 8,
//    DssGraphGroupSpacing = 10,
//    DssGraphRiserWidth = 11,
//    DssGraphColorBySeries = 12,
//    DssGraphCurveColorAsSeries = 13,
//    DssGraphSDMOVAFactor = 14,
//    DssGraph2DAngle = 18,
//    DssGraph2DDepth = 20,
//    DssGraphDepthMode = 21,
//    DssGraph2ScaleDirectionX = 22,
//    DssGraph2ScaleDirectionY1 = 23,
//    DssGraph2ScaleDirectionY2 = 24,
//    DssGraph2ExcludeZeroX = 25,
//    DssGraph2ExcludeZeroY1 = 26,
//    DssGraph2ExcludeZeroY2 = 27,
//    DssGraphFormatDataTextX = 28,
//    DssGraphFormatDataTextY1 = 29,
//    DssGraphFormatDataTextY2 = 30,
//    DssGraphFormatX = 33,
//    DssGraphFormatY1 = 34,
//    DssGraphFormatY2 = 35,
//    DssGraphFormatZ = 36,
//    DssGraphGridLineO1 = 37,
//    DssGraphGridLineO2 = 38,
//    DssGraphGridLineX = 39,
//    DssGraphGridLineY1 = 40,
//    DssGraphGridLineY2 = 41,
//    DssGraphLabelModeO1 = 48,
//    DssGraphLabelModeO2 = 49,
//    DssGraphLegendLocation = 52,
//    DssGraphTitleXLocation = 54,
//    DssGraphTitleY1Location = 55,
//    DssGraphTitleY2Location = 56,
//    DssGraph2LogScaleX = 57,
//    DssGraph2LogScaleY1 = 58,
//    DssGraph2LogScaleY2 = 59,
//    DssGraphMarkerShape = 60,
//    DssGraphMarkerSize = 61,
//    DssGraph2DPlaceX = 65,
//    DssGraph2DPlaceY1 = 66,
//    DssGraph2DPlaceY2 = 67,
//    DssGraphConnectLine = 68,
//    DssGraphShowStepLine = 70,
//    DssGraph2ScaleEndX = 74,
//    DssGraph2ScaleEndY1 = 75,
//    DssGraph2ScaleEndY2 = 76,
//    DssGraph2DScaleFreqO1 = 87,
//    DssGraph2DScaleFreqO2 = 88,
//    DssGraph2DScaleFreqX = 89,
//    DssGraph2DScaleFreqY1 = 90,
//    DssGraph2DScaleFreqY2 = 91,
//    DssGraphScaleX = 92,
//    DssGraphScaleY1 = 93,
//    DssGraphScaleY2 = 94,
//    DssGraphShowSDDataLineType = 96,
//    DssGraphSDRiserType = 97,
//    DssGraphShowSDLine = 98,
//    DssGraphShowSDCurve = 99,
//    DssGraphShowSDLinrCorr = 101,
//    DssGraphShowSDLinrExp = 102,
//    DssGraphShowSDLinrFormula = 103,
//    DssGraphShowSDLinrLine = 104,
//    DssGraphShowSDLinrLog = 105,
//    DssGraphShowSDLinrNatLog = 106,
//    DssGraphShowSDLinrNPoly = 107,
//    DssGraphSDPolyFactor = 108,
//    DssGraphShowSDMeanLine = 109,
//    DssGraphShowSDMOVALine = 110,
//    DssGraphShowSDSTDDLine = 111,
//    DssGraphShowDataText = 113,
//    DssGraphShowDivBiPolar = 114,
//    DssGraphShowErrorBar = 115,
//    DssGraphShowLegendText = 116,
//    DssGraphShow2DO1Label = 117,
//    DssGraphShow2DO1 = -117,
//    DssGraphShow2DO2 = 118,
//    DssGraphOffScaleTypeX = 119,
//    DssGraphOffScaleTypeY1 = 120,
//    DssGraphOffScaleTypeY2 = 121,
//    DssGraphShowQuadrants = 122,
//    DssGraphShowRiserShadows = 123,
//    DssGraphShow2DXTitle = 124,
//    DssGraphShow2DY1Title = 125,
//    DssGraphShow2DY2Title = 126,
//    DssGraphShow2DXLabel = 127,
//    DssGraphShow2DX = -127,
//    DssGraphShow2DY1Label = 128,
//    DssGraphShow2DY1 = -128,
//    DssGraphShow2DY2Label = 129,
//    DssGraphShow2DY2 = -129,
//    DssGraphShowZeroLineX = 130,
//    DssGraphShowZeroLineY1 = 131,
//    DssGraphShowZeroLineY2 = 132,
//    DssGraph2DSideO1 = 133,
//    DssGraph2DSideO2 = 134,
//    DssGraph2DSideX = 135,
//    DssGraph2DSideY1 = 136,
//    DssGraph2DSideY2 = 137,
//    DssGraph2DSDSplitY = 140,
//    DssGraphStaggerO1 = 142,
//    DssGraphStaggerO2 = 143,
//    DssGraphStaggerX = 144,
//    DssGraphStaggerY1 = 145,
//    DssGraphStaggerY2 = 146,
//    DssGraphUniformBorders = 148,
//    DssGraphPieDepthValue = 192,
//    DssGraphPieHoleSizeValue = 198,
//    DssGraphPieInset = 200,
//    DssGraphPiesPerRowValue = 201,
//    DssGraphPieRotationValue = 202,
//    DssGraphShowFeeler = 204,
//    DssGraphShowLBLFeelerLabel = 205,
//    DssGraphShowLBLFeeler = -205,
//    DssGraphShowLBLPie = 206,
//    DssGraphShowLBLRing = 207,
//    DssGraphDeleteSlice = 208,
//    DssGraphDetachSlice = 209,
//    DssGraphPieRestore = 210,
//    DssGraphPieTiltValue = 211,
//    DssGraphPieTiltOn = 212,
//    DssGraphShowPolarCoordinateAxis = 215,
//    DssGraphShowPolarConnectLines = 218,
//    DssGraphStockDataValueType = 222,
//    DssGraphShowBubbleGrid = 245,
//    DssGraphSDShowDataText = 247,
//    DssGraphShowTitleSeries = 248,
//    DssGraphConnectStackBars = 249,
//    DssGraphLegendBox = 250,
//    DssGraphLinkColors = 260,
//    DssGraph3DCustomView = 265,
//    DssGraph3DGridFloor = 277,
//    DssGraph3DGridLWall = 278,
//    DssGraph3DGridRiser = 279,
//    DssGraph3DGridRWall = 280,
//    DssGraph3DColTitleLocation = 285,
//    DssGraph3DLYTitleLocation = 287,
//    DssGraph3DRowTitleLocation = 288,
//    DssGraph3DRYTitleLocation = 289,
//    DssGraph3DScalefreqX = 299,
//    DssGraph3DScalefreqY = 300,
//    DssGraph3DScalefreqZ = 301,
//    DssGraphShow3DScatterLine = 308,
//    DssGraphShow3DLineToLWall = 309,
//    DssGraphShow3DLineToFloor = 310,
//    DssGraphShow3DLineToRWall = 311,
//    DssGraphSD3DRiserType = 312,
//    DssGraphShow3DShowDataLabelsBox = 313,
//    DssGraph3DOffScaleTypeX = 314,
//    DssGraph3DOffScaleTypeY = 315,
//    DssGraph3DOffScaleTypeZ = 316,
//    DssGraphShow3DCHeader = 318,
//    DssGraphShow3DColTitle = 319,
//    DssGraph3DShowDataText = 320,
//    DssGraphShow3DLNumber = 321,
//    DssGraphShow3DLYTitle = 322,
//    DssGraphShow3DRNumber = 323,
//    DssGraphShow3DRYTitle = 324,
//    DssGraphShow3DRHeader = 325,
//    DssGraphShow3DRowTitle = 326,
//    DssGraphShow3DFloor = 327,
//    DssGraphShow3DLegend = 328,
//    DssGraphShow3DLWall = 329,
//    DssGraphShow3DRWall = 330,
//    DssGraph3DViewStruct = 337,
//    DssGraphACMColorModel = 549,
//    DssGraphLegendItems = 554,
//    DssGraphSeriesColor = 556,
//    DssGraphReverseGroup = 558,
//    DssGraphReverseSeries = 559,
//    DssGraphSeriesByColumn = 560,
//    DssGraphAreaBackColor = 561,
//    DssGraphAreaTransparency = 563,
//    DssGraphAreaColor = 564,
//    DssGraphAreaPattern = 566,
//    DssGraphAreaSFX = 567,
//    DssGraphFontAlign = 570,
//    DssGraphFontTransparency = 573,
//    DssGraphFontColor = 574,
//    DssGraphFontName = 576,
//    DssGraphFontRotation = 578,
//    DssGraphVirtualFontSize = 583,
//    DssGraphFontStyle = 584,
//    DssGraphType = 585,
//    DssGraphLineTransparency = 587,
//    DssGraphLineColor = 588,
//    DssGraphLinePattern = 589,
//    DssGraphVirtualLineWidth = 591,
//    DssGraphFootnoteLocation = 592,
//    DssGraphFrameLocation = 593,
//    DssGraphSubTitleLocation = 594,
//    DssGraphTitleLocation = 595,
//    DssGraphShowfootnote = 596,
//    DssGraphShowSubtitle = 597,
//    DssGraphShowTitle = 598,
//    DssGraphFontBox = 601,
//    DssGraphFontSize = 603,
//    DssGraphLockFontSize = 604,
//    DssGraphLineWidth = 605,
//    DssGraphAreaAlpha = 608,
//    DssGraphLineAlpha = 609,
//    DssGraphFontAlpha = 610,
//    DssGraphAreaCustomBevelEffect = 612,
//    DssGraphFormatDataTextAdv = 616,
//    DssGraphFormatXAdv = 618,
//    DssGraphFormatY1Adv = 619,
//    DssGraphFormatY2Adv = 620,
//    DssGraphFormatZAdv = 621,
//    DssGraphFormatRingLabelAdv = 623,
//    DssGraphMiddleValue = 630,
//    DssGraphSplitYPosition = 631,
//    DssGraphAxisShowUserLine = 632,
//    DssGraphAxisUserLineValue = 633,
//    DssGraphAxisUserLineAxis = 634,
//    DssGraphMarkerLayout = 637,
//    DssGraphPieDataValueLoc = 638,
//    DssGraphPieDataLabelLoc = 639,
//    DssGraph2DOrdSpaceMode = 642,
//    DssGraphLegendLock = 644,
//    DssGraphGridStepY1 = 645,
//    DssGraphGridStepY2 = 646,
//    DssGraphGridStepX = 647,
//    DssGraphDataTextShowConnectLine = 652,
//    DssGraphGridLineZ = 658,
//    DssGraphScaleZ = 659,
//    DssGraphStaggerZ = 660,
//    DssGraph2ScaleEndZ = 662,
//    DssGraph2ExcludeZeroZ = 663,
//    DssGraph2ScaleDirectionZ = 664,
//    DssGraphGridStepZ = 665,
//    DssGraphGaugeInfo = 668,
//    DssGraphGaugeFancyBox = 669,
//    DssGraphSDMarkerSize = 671,
//    DssGraphSDMarkerShape = 683,
//    DssGraphMetric1 = 690,
//    DssGraphMetric2 = 691,
//    DssGraphStockStyle = 692,
//    DssGraph2ScaleDirectionY3 = 693,
//    DssGraph2ExcludeZeroY3 = 694,
//    DssGraphFormatDataTextY3 = 695,
//    DssGraphGridLineY3 = 697,
//    DssGraphFormatY3Adv = 699,
//    DssGraphGridStepY3 = 701,
//    DssGraphOffScaleTypeY3 = 702,
//    DssGraphShow2DY3 = 704,
//    DssGraph2DSideY3 = 706,
//    DssGraphStaggerY3 = 707,
//    DssGraph2DPlaceY3 = 709,
//    DssGraph2ScaleEndY3 = 710,
//    DssGraph2DScaleFreqY3 = 713,
//    DssGraphScaleY3 = 714,
//    DssGraph2ScaleDirectionY4 = 715,
//    DssGraph2ExcludeZeroY4 = 716,
//    DssGraphFormatDataTextY4 = 717,
//    DssGraphGridLineY4 = 719,
//    DssGraphFormatY4Adv = 721,
//    DssGraphGridStepY4 = 723,
//    DssGraphOffScaleTypeY4 = 724,
//    DssGraphShow2DY4 = 726,
//    DssGraph2DSideY4 = 728,
//    DssGraphStaggerY4 = 729,
//    DssGraph2DPlaceY4 = 731,
//    DssGraph2ScaleEndY4 = 732,
//    DssGraph2DScaleFreqY4 = 735,
//    DssGraphScaleY4 = 736,
//    DssGraphSpiderLines = 739,
//    DssGraphPolarStartAngle = 740,
//    DssGraphMetric3 = 742,
//    DssGraphRadarCircularDataLines = 782,
//    DssGraphRemoteAxis = 784,
//    DssGraphConnectFirstLast = 790,
//    DssGraphGaugeShowAxisLabelsOutside = 791,
//    DssGraphGaugeShowNeedle = 792,
//    DssGraphTailShape = 793,
//    DssGraphPieForceRound = 794,
//    DssGraphGaugeStyleValue = 795,
//    DssGraphGaugeBorderStyleValue = 796,
//    DssGraphGaugeBorderThicknessValue = 797,
//    DssGraphGaugeRangeThicknessValue = 798,
//    DssGraphGaugeRangeStartAngleValue = 799,
//    DssGraphGaugeRangeStopAngleValue = 800,
//    DssGraphGaugeNeedleStyleValue = 801,
//    DssGraphCurvedLines = 804,
//    DssGraphAreaBevelEffect = 879,
//    DssGraphFontAntiAlias = 881,
//    DssGraphUseFirstSeriesForRingLabel = 1001,
//    DssGraphColorPalette = 10000,
//    DssGraphOriContent = 10001,
//    DssGraphFillEffect = 10002,
//    DssGraphGradient = 10003,
//    DssGraphSDShowToolTip = 10004,
//    DssGraphSDToolTip = 10005,
//    DssGraphSDRegressionType = 10006,
//    DssGraph2LogScale = 10007,
//    DssGraphManualMajor = 10008,
//    DssGraphManualMinor = 10009,
//    DssGraph2ScaleDirection = 10010,
//    DssGraphShow2DAxisLine = 10011,
//    DssGraph2ExcludeZero = 10012,
//    DssGraph2ScaleUseMax = 10013,
//    DssGraph2ScaleUseMin = 10014,
//    DssGraph2ScaleMax = 10015,
//    DssGraph2ScaleMin = 10016,
//    DssGraphShow2DAxisLabels = 10017,
//    DssGraph2ScaleEndMax = 10018,
//    DssGraph2ScaleEndMin = 10019,
//    DssGraphShow2DAxisMajor = 10020,
//    DssGraphShow2DAxisMinor = 10021,
//    DssGraph2DSide = 10022,
//    DssGraphStyleMajor = 10023,
//    DssGraphStyleMinor = 10024,
//    DssGraphCountMajor = 10025,
//    DssGraphCountMinor = 10026,
//    DssGraphShowTotalValue = 10027,
//    DssGraph2DPie = 10028,
//    DssGraphShowDataValue = 10029,
//    DssGraphShowDVPercent = 10030,
//    DssGraphShowDataLabel = 10031,
//    DssGraphShowPieLabelOnSide = 10032,
//    DssGraphShowMarker = 10033,
//    DssGraphShowMetric1 = 10034,
//    DssGraphShowMetric2 = 10035,
//    DssGraphShowMetric3 = 10036,
//    DssGraphMetric1Style = 10037,
//    DssGraphMetric2Style = 10038,
//    DssGraphMetric3Style = 10039,
//    DssGraphShowLine = 10040,
//    DssGraphShowToolTip = 10041,
//    DssGraphToolTip = 10042,
//    DssGraphAxisAutoFit = 10043,
//    DssGraphAxisStagger = 10044,
//    DssGraphAxisScaleFreq = 10045,
//    DssGraphMetric1Value = 10046,
//    DssGraphMetric2Value = 10047,
//    DssGraphMetric3Value = 10048,
//    DssGraph2AxisFormatText = 10049,
//    DssGraph2AxisFormatDataText = 10050,
//    DssGraphDPI = 10051,
//    DssGraphDocSize = 10052,
//    DssGraphAutoArrange = 10053,
//    DssGraph2DSplitYAutomatic = 10054,
//    DssGraphHistogramRangeMin = 10055,
//    DssGraphHistogramRangeMax = 10056,
//    DssGraphHistogramBuckets = 10057,
//    DssGraphGaugeSAngle = 10058,
//    DssGraphGaugeEAngle = 10059,
//    DssGraphGaugeThickness = 10060,
//    DssGraphGauge1Show = 10061,
//    DssGraphGauge1Min = 10062,
//    DssGraphGauge1Max = 10063,
//    DssGraphGauge2Show = 10064,
//    DssGraphGauge2Min = 10065,
//    DssGraphGauge2Max = 10066,
//    DssGraphGauge3Show = 10067,
//    DssGraphGauge3Min = 10068,
//    DssGraphGauge3Max = 10069,
//    DssGraphGauge4Show = 10070,
//    DssGraphGauge4Min = 10071,
//    DssGraphGauge4Max = 10072,
//    DssGraphGauge5Show = 10073,
//    DssGraphGauge5Min = 10074,
//    DssGraphGauge5Max = 10075,
//    DssGraphAxisDataTextPlace = 10076,
//    DssGraphAxisNumberFormat = 10077,
//    DssGraphAxisDataNumberFormat = 10078,
//    DssGraphAxisHasErrorBar = 10079,
//    DssGraphAxisHasMiddleValue = 10080,
//    DssGraphHeight = 10081,
//    DssGraphWidth = 10082,
//    DssGraphPicture = 10083,
//    DssGraph3DTurnXCounterClockwise = 10084,
//    DssGraph3DTurnYCounterClockwise = 10085,
//    DssGraph3DTurnZCounterClockwise = 10086,
//    DssGraph3DTurnXClockwise = 10087,
//    DssGraph3DTurnYClockwise = 10088,
//    DssGraph3DTurnZClockwise = 10089,
//    DssGraph3DMoveXTowardsOrigin = 10090,
//    DssGraph3DMoveYTowardsOrigin = 10091,
//    DssGraph3DMoveZTowardsOrigin = 10092,
//    DssGraph3DMoveXAwayFromOrigin = 10093,
//    DssGraph3DMoveYAwayFromOrigin = 10094,
//    DssGraph3DMoveZAwayFromOrigin = 10095,
//    DssGraph3DDecreaseCubeX = 10096,
//    DssGraph3DDecreaseCubeY = 10097,
//    DssGraph3DDecreaseCubeZ = 10098,
//    DssGraph3DIncreaseCubeX = 10099,
//    DssGraph3DIncreaseCubeY = 10100,
//    DssGraph3DIncreaseCubeZ = 10101,
//    DssGraph3DDecreaseWallX = 10102,
//    DssGraph3DDecreaseWallY = 10103,
//    DssGraph3DDecreaseWallZ = 10104,
//    DssGraph3DIncreaseWallX = 10105,
//    DssGraph3DIncreaseWallY = 10106,
//    DssGraph3DIncreaseWallZ = 10107,
//    DssGraph3DPanXLeft = 10108,
//    DssGraph3DPanYUp = 10109,
//    DssGraph3DRelative = 10110,
//    DssGraph3DPanXRight = 10111,
//    DssGraph3DPanYDown = 10112,
//    DssGraph3DAbsolute = 10113,
//    DssGraph3DZoomIn = 10114,
//    DssGraph3DIncreasePerspectiveDistortion = 10115,
//    DssGraph3DStepSize = 10116,
//    DssGraph3DZoomOut = 10117,
//    DssGraph3DDecreasePerspectiveDistortion = 10118,
//    DssGraph3DInitMatrix = 10119,
//    DssGraphGaugeQBShow = 10120,
//    DssGraphGaugeQBStart = 10121,
//    DssGraphGaugeQBStop = 10122,
//    DssGraphEnhancedSeriesEffect = 10123,
//    DssGraphLocale = 10124,
//    DssGraph3DViewStructName = 10125,
//    DssGraphLabelsDistance = 10126,
//    DssGraphLegendDetails = 10127,
//    DssGraphDefaultsInitialized = 10128,
//    DssGraphShow3DAxisMajor = 10129,
//    DssGraphGradientColorMode = 10130,
//    DssGraphLegendDetailsNEW = 10131,
//    DssGraphFrameLocationNEW = 10132,
//    DssGraph3DColTitleLocationNEW = 10133,
//    DssGraph3DRowTitleLocationNEW = 10134,
//    DssGraphLabelsDistanceNEW = 10135,
//    DssGraphDocSizeNEW = 10136,
//    DssGraphUseManualHistogramBuckets = 10137,
//    DssGraphFontSize100 = 10138,
//    DssGraphOffScaleType = 10139,
//    DssGraphUseSpecialSeries = 10140,
//    DssGraphAxisAllowFractionalGrids = 10141,
//    DssGraphInfoReserved = 30000,
//    DssGraphInfoAutosizeHorizontal = 30011,
//    DssGraphInfoAutosizeVertical = 30012,
//    DssGraphInfoSeriesByColumn = 30013,
//    DssGraphInfoIncludeSubtotal = 30014,
//    DssGraphInfoSuppressLastLevel = 30016,
//    DssGraphInfoRecalculateChartFrame = 30026,
//    DssGraphInfoRecalculateLegend = 30027,
//    DssGraphInfoNestedLabel = 30038,
//    DssGraphInfoNestedLabelBracket = 30039,
//    DssGraphInfoUseMaxCategoriesAsMin = 30048,
//    DssGraphInfoMaxSeries = 30103,
//    DssGraphInfoMaxSeriesEverSet = 30104
//} EnumDSSGraphProperty;

//typedef enum EnumDSSGraphDataLineType
//{
//    DssGraphDataLineNone = 0,
//    DssGraphDataLineMarker = 1,
//    DssGraphDataLineLine = 2,
//    DssGraphDataLineBoth = DssGraphDataLineMarker | DssGraphDataLineLine
//} EnumDSSGraphDataLineType;
//
//typedef enum EnumDSSGraphMarkerLayout
//{
//    DssGraphMarkerLeft,
//    DssGraphMarkerRight,
//    DssGraphMarkerCentered,
//    DssGraphMarkerAbove,
//    DssGraphMarkerBelow,
//    DssGraphMarkerAuto
//} EnumDSSGraphMarkerLayout;
//
//typedef enum EnumDSSGraphFontAlignment
//{
//    DssGraphFontAlignLeft = 0,
//    DssGraphFontAlignCenter = 1,
//    DssGraphFontAlignRight = 2,
//    DssGraphFontAlignJustify = 3
//} EnumDSSGraphFontAlignment;

//typedef enum EnumDSSGraphFontStyle
//{
//    DssGraphFontStyleReserved = 0,
//    DssGraphFontStyleBold = 1,
//    DssGraphFontStyleItalic = 2,
//    DssGraphFontStyleUnderline = 4,
//    DssGraphFontStyleStrikeout = 8
//} EnumDSSGraphFontStyle;
//
//typedef enum EnumDSSGraphFontRotation
//{
//    DssGraphFontRotateNormal = 0,
//    DssGraphFontRotateHotel = 1,
//    DssGraphFontRotate90 = 2,
//    DssGraphFontRotate180 = 3,
//    DssGraphFontRotate270 = 4,
//    DssGraphFontRotate45 = 5,
//    DssGraphFontRotate315 = 6,
//    DssGraphFontRotateCustom = 7
//} EnumDSSGraphFontRotation;
//
//typedef enum EnumDSSGraphFontBoxType
//{
//    DssGraphFontBoxTypeNone = 0,
//    DssGraphFontBoxTypeSingleLine = 1,
//    DssGraphFontBoxTypeDoubleLine = 2,
//    DssGraphFontBoxTypeExtrude = 3,
//    DssGraphFontBoxTypeFrameBevel = 4,
//    DssGraphFontBoxTypeFrameReverseBevel = 5
//} EnumDSSGraphFontBoxType;

//typedef enum EnumDSSGraphFillEffect
//{
//    DssGraphFillSimple = 0,
//    DssGraphFillWash = 3,
//    DssGraphFillPicture = 4,
//    DssGraphFillUserPicture = 6,
//    DssGraphFillBrush = 13,
//    DssGraphFillAdvancedWash = 14,
//    DssGraphFillPattern = 101
//} EnumDSSGraphFillEffect;
//
//typedef enum EnumDSSGraphFillBevelEffect
//{
//    DssGraphFillBevelNone = 0,
//    DssGraphFillBevelSmoothEdge = 1,
//    DssGraphFillBevelChiselEdge = 2,
//    DssGraphFillBevelDonut = 3,
//    DssGraphFillBevelSphere = 4
//} EnumDSSGraphFillBevelEffect;
//
//typedef enum EnumDSSGraphGradientType
//{
//    DssGraphGradientReserved = 0,
//    DssGraphGradientCircular = 9,
//    DssGraphGradientRectangular = 10,
//    DssGraphGradientLinear = 14
//} EnumDSSGraphGradientType;

//typedef enum EnumDSSGraphGradientColoringMode
//{
//    DssGraphGradientColorModeReserved = 0x0,
//    DssGraphGradientColorModeSingle = 0x1,
//    DssGraphGradientColorModeDouble = 0x2,
//    DssGraphGradientColorModeCustom = 0x4
//} EnumDSSGraphGradientColoringMode;
//
//typedef enum EnumDSSGraphPictureFlipType
//{
//    DssGraphPictureFlipNull = 0,
//    DssGraphPictureFlipHorizontal = 1,
//    DssGraphPictureFlipVertical = 2,
//    DssGraphPictureFlipBoth = 3
//} EnumDSSGraphPictureFlipType;
//
//typedef enum EnumDSSGraphPictureScaleType
//{
//    DssGraphPictureScaleNone = 0,
//    DssGraphPictureScale2Fit = 1,
//    DssGraphPictureScale2Frame = 2,
//    DssGraphPictureScale2Background = 3,
//    DssGraphPictureTiled = 4,
//    DssGraphPictureTiled2Frame = 5,
//    DssGraphPictureTiled2Background = 6,
//    DssGraphPictureBrickHorizontal = 7,
//    DssGraphPictureBrickVertical = 8,
//    DssGraphPictureStack = 9,
//    DssGraphPictureScaleProportional = 10
//} EnumDSSGraphPictureScaleType;

//typedef enum EnumDSSGraphPictureFormat
//{
//    DssGraphPictureJPEG = 0,
//    DssGraphPictureGIF = 1,
//    DssGraphPicturePNG = 2,
//    DssGraphPictureWMF = 3,
//    DssGraphPictureBMP = 4
//} EnumDSSGraphPictureFormat;
//
//typedef enum EnumDSSGraphExportFormat
//{
//    DssGraphExportJPEG = 0,
//    DssGraphExportPNG = 1,
//    DssGraphExportPCT = 2,
//    DssGraphExportPCX = 3,
//    DssGraphExportPSD = 4,
//    DssGraphExportTGA = 5,
//    DssGraphExportTIF = 6,
//    DssGraphExportBMP = 7,
//    DssGraphExportGIF = 100,
//    DssGraphExportCGIF = 101,
//    DssGraphExportPDF = 102
//} EnumDSSGraphExportFormat;

//typedef enum EnumDSSGraphLineStyle
//{
//    DssGraphLineStyleSolid = 0,
//    DssGraphLineStyleDash = 1,
//    DssGraphLineStyleDot = 2,
//    DssGraphLineStyleDotDash = 3,
//    DssGraphLineStyleDashDotDot = 4,
//    DssGraphLineStyleMediumDash = 5,
//    DssGraphLineStyleShortDash = 6,
//    DssGraphLineStyleLongestDash = 7,
//    DssGraphLineStyleLongDot = 8,
//    DssGraphLineStyleDotDotDot = 9,
//    DssGraphLineStyleDashDashDot = 10,
//    DssGraphLineStyleDashDashDotDot = 11,
//    DssGraphLineStyleLongDashDot = 12,
//    DssGraphLineStyleLongDashDotDot = 13,
//    DssGraphLineStyleLongDashDashDot = 14,
//    DssGraphLineStyleLongDashDashDotDot = 15
//} EnumDSSGraphLineStyle;
//
//typedef enum EnumDSSGraphLegendPosition
//{
//    DssGraphLegendPositionFreeFloat = 0,
//    DssGraphLegendPositionRight = 1,
//    DssGraphLegendPositionLeft = 2,
//    DssGraphLegendPositionBottom = 3
//} EnumDSSGraphLegendPosition;

//typedef enum EnumDSSGraphDepthMode
//{
//    DssGraphDepthNone = 0,
//    DssGraphDepthFake3D = 1,
//    DssGraphDepthTrue3D = 2
//} EnumDSSGraphDepthMode;
//
//typedef enum EnumDSSGraphDataTextPosition
//{
//    DssGraphDTPositionCenter = 0,
//    DssGraphDTPositionOutMin = 1,
//    DssGraphDTPositionInMin = 2,
//    DssGraphDTPositionInMax = 3,
//    DssGraphDTPositionOutMax = 4,
//    DssGraphDTPositionOnSide = 5,
//    DssGraphDTPositionPieSmart = 6,
//    DssGraphDTPositionPieSmartRadial = 7,
//    DssGraphDTPositionPieSmartStacked = 8
//} EnumDSSGraphDataTextPosition;
//
//typedef enum EnumDSSGraphMetricValueStyle
//{
//    DssGraphMetricVerticalLine = 0,
//    DssGraphMetricLine = 1,
//    DssGraphMetricArea = 2,
//    DssGraphMetricBar = 3,
//    DssGraphMetricDot = 4,
//    DssGraphMetricArea2 = 5
//} EnumDSSGraphMetricValueStyle;

//typedef enum EnumDSSGraphDataTextShowType
//{
//    DssGraphDataTextNone = 0x00,
//    DssGraphDataTextValue = 0x01,
//    DssGraphDataTextLabel = 0x02,
//    DssGraphDataTextBoth = 0x03,
//    DssGraphDataTextAbs = 0x04,
//    DssGraphDataTextAbsValue = 0x05,
//    DssGraphDataTextAbsBoth = 0x07,
//    DssGraphDataTextTotal = 0x08,
//    DssGraphDataTextZValue = 0x10,
//    DssGraphDataTextPercent = 0x20,
//    DssGraphDataTextYValue = 0x40,
//    DssGraphDataTextInternal = 0x80,
//    DssGraphDataTextSeriesDependent = 0x100,
//    DssGraphDataTextQualityObjectMode = 0x200
//} EnumDSSGraphDataTextShowType;
//
//typedef enum EnumDSSGraphStockDataValueType
//{
//    DssGraphDataValueHigh = 0,
//    DssGraphDataValueLow = 1,
//    DssGraphDataValueOpen = 2,
//    DssGraphDataValueClose = 3
//} EnumDSSGraphStockDataValueType;

//typedef enum EnumDSSGraphGridLineType
//{
//    DssGraphGridNone = 0,
//    DssGraphGridRegular = 1,
//    DssGraphGridBoth = 2,
//    DssGraphGridInnerTicks = 3,
//    DssGraphGridOuterTicks = 4,
//    DssGraphGridSpanningTicks = 5
//} EnumDSSGraphGridLineType;
//
//typedef enum EnumDSSBoxPlotTailShape
//{
//    DssBoxPlotTailShapeRectangle = 1,
//    DssBoxPlotTailShapeTBar = 2,
//    DssBoxPlotTailShapeIBar = 3
//} EnumDSSBoxPlotTailShape;
//
//typedef enum EnumDSSGraphRegressionType
//{
//    DssGraphRegressionNone = 0,
//    DssGraphRegressionLinear = DssGraphShowSDLinrLine,
//    DssGraphRegressionLog = DssGraphShowSDLinrLog,
//    DssGraphRegressionPoly = DssGraphShowSDLinrNPoly,
//    DssGraphRegressionExp = DssGraphShowSDLinrExp,
//    DssGraphRegressionMA = DssGraphShowSDMOVALine,
//    DssGraphRegressionNatLog = DssGraphShowSDLinrNatLog,
//    DssGraphRegressionSTDD = DssGraphShowSDSTDDLine,
//    DssGraphRegressionMean = DssGraphShowSDMeanLine
//} EnumDSSGraphRegressionType;

//typedef enum EnumDSSGraphRiserEmphasisType
//{
//    DssGraphRiserEmphasisNone = 0,
//    DssGraphRiserEmphasisLineOrLineToBar = 1,
//    DssGraphRiserEmphasisAreaOrAreaToBar = 2
//} EnumDSSGraphRiserEmphasisType;
//
//typedef enum EnumDSSGraphNumberFormatType
//{
//    DssGraphNumberFormatDefault = -2,
//    DssGraphNumberFormatFixed = 0,
//    DssGraphNumberFormatCurrency = 1,
//    DssGraphNumberFormatDate = 2,
//    DssGraphNumberFormatTime = 3,
//    DssGraphNumberFormatPercentage = 4,
//    DssGraphNumberFormatFraction = 5,
//    DssGraphNumberFormatScientific = 6,
//    DssGraphNumberFormatCustom = 7,
//    DssGraphNumberFormatSpecial = 8,
//    DssGraphNumberFormatGeneral = 9,
//    DssGraphNumberFormatFromGrid = 10
//} EnumDSSGraphNumberFormatType;

//typedef enum EnumDSSGraphFractionNumberFormatType
//{
//    DssGraphFractionNumberFormatOneDigit = 1,
//    DssGraphFractionNumberFormatTwoDigits = 2,
//    DssGraphFractionNumberFormatThreeDigits = 3,
//    DssGraphFractionNumberFormatHalves = 1000,
//    DssGraphFractionNumberFormatQuarters = 1001,
//    DssGraphFractionNumberFormatEights = 1002,
//    DssGraphFractionNumberFormatTenths = 1003,
//    DssGraphFractionNumberFormatHundreths = 1004
//} EnumDSSGraphFractionNumberFormatType;
//
//typedef enum EnumDSSGraphNumberFormatCurrencyPosition
//{
//    DssGraphNumberFormatCurrencyPositionLeft = 0,
//    DssGraphNumberFormatCurrencyPositionRight = 1,
//    DssGraphNumberFormatCurrencyPositionLeftSpace = 2,
//    DssGraphNumberFormatCurrencyPositionRightSpace = 3
//} EnumDSSGraphNumberFormatCurrencyPosition;
//
//typedef enum EnumDSSGraphNumberFormatNegativeNumber
//{
//    DssGraphNumberFormatNegativeNumberDefault = 1,
//    DssGraphNumberFormatNegativeNumberRed = 2,
//    DssGraphNumberFormatNegativeNumberParenthesis = 3,
//    DssGraphNumberFormatNegativeNumberParenthesisRed = 4
//} EnumDSSGraphNumberFormatNegativeNumber;

//typedef enum EnumDSSGraphColorMode
//{
//    DssGraphColorModeByFace = 0,
//    DssGraphColorModeBySeries = 1,
//    DssGraphColorModeByGroup = 2,
//    DssGraphColorModeByAngle = 3,
//    DssGraphColorModeByHeight = 4,
//    DssGraphColorModeByValueX = 5,
//    DssGraphColorModeByValueY = 6,
//    DssGraphColorModeByValueZ = 7
//} EnumDSSGraphColorMode;
//
//typedef enum EnumDSSGraphMarkerShape
//{
//    DssGraphMarkerShapeNone = 0,
//    DssGraphMarkerShapeRectangle = 1,
//    DssGraphMarkerShapeStar45 = 2,
//    DssGraphMarkerShapePlus = 3,
//    DssGraphMarkerShapeCircle = 4,
//    DssGraphMarkerShapeDiamond = 5,
//    DssGraphMarkerShapeSpikedX = 6,
//    DssGraphMarkerShapePlainX = 7,
//    DssGraphMarkerShapeTriangle1 = 8,
//    DssGraphMarkerShapeStarSkewed = 9,
//    DssGraphMarkerShapeFatPlus = 10,
//    DssGraphMarkerShapeStar90 = 11,
//    DssGraphMarkerShapeSoftX = 12,
//    DssGraphMarkerShapePiratePlus = 13,
//    DssGraphMarkerShapeFatX = 14,
//    DssGraphMarkerShapeCastle = 15,
//    DssGraphMarkerShapeTriangle2 = 16,
//    DssGraphMarkerShapeTriangle3 = 17,
//    DssGraphMarkerShapeTriangle4 = 18,
//    DssGraphMarkerShapeTriangle5 = 19,
//    DssGraphMarkerShapeTriangle6 = 20,
//    DssGraphMarkerShapeTriangle7 = 21,
//    DssGraphMarkerShapeTriangle8 = 22,
//    DssGraphMarkerShapeEllipse = 23,
//    DssGraphMarkerShapeSquare = 24,
//    DssGraphMarkerShapeHexagon = 25,
//    DssGraphMarkerShapePentagon = 26,
//    DssGraphMarkerShapeHouse = 27,
//    DssGraphMarkerShapePentagram = 28,
//    DssGraphMarkerShapeFontMarker = 29,
//    DssGraphMarkerShapeBoxedPlus = 30,
//    DssGraphMarkerShapeBoxedX = 31,
//    DssGraphMarkerShapeHourGlass = 32,
//    DssGraphMarkerShapeHourGlassTransparent = 33,
//    DssGraphMarkerShapeVerticalLine = 34,
//    DssGraphMarkerShapeHorizontalLine = 35,
//    DssGraphMarkerShapeAsterisk = 36,
//    DssGraphMarkerShapeStar5 = 37,
//    DssGraphMarkerShapeStar6 = 38,
//    DssGraphMarkerShapeStar8 = 39,
//    DssGraphMarkerShapeBevelBox = 40,
//    DssGraphMarkerShapeReverseBevelBox = 41,
//    DssGraphMarkerShapeSlimHorizontalLine = 42,
//    DssGraphMarkerShapeSlimVerticalLine = 43,
//    DssGraphMarkerShapeSlimPlus = 44,
//    DssGraphMarkerShapeSlimBoxedPlus = 45,
//    DssGraphMarkerShapeSlimX = 46,
//    DssGraphMarkerShapeSlimBoxedX = 47,
//    DssGraphMarkerShapeRotatedHourGlass = 48,
//    DssGraphMarkerShapeSmallBevelBox = 49
//} EnumDSSGraphMarkerShape;

//typedef enum EnumDSSGraph3DRiserType
//{
//    DssGraph3DRiserBar = 0,
//    DssGraph3DRiserColumnArea = 1,
//    DssGraph3DRiserColumnLine = 2,
//    DssGraph3DRiserColumnStep = 3,
//    DssGraph3DRiserCube = 4,
//    DssGraph3DRiserCutCorner = 5,
//    DssGraph3DRiserDiamond = 6,
//    DssGraph3DRiserHoneyComb = 7,
//    DssGraph3DRiserModel = 8,
//    DssGraph3DRiserOctagon = 9,
//    DssGraph3DRiserPyramid = 10,
//    DssGraph3DRiserRowArea = 11,
//    DssGraph3DRiserRowLine = 12,
//    DssGraph3DRiserRowStep = 13,
//    DssGraph3DRiserScatter = 14,
//    DssGraph3DRiserSquare = 15,
//    DssGraph3DRiserSurface = 16,
//    DssGraph3DRiser2DArea = 17,
//    DssGraph3DRiser2DBars = 18,
//    DssGraph3DRiserBoxPlot = 19,
//    DssGraph3DRiserBoxPlotTC = 20,
//    DssGraph3DRiser2DBubble = 21,
//    DssGraph3DRiserContourPlot = 22,
//    DssGraph3DRiserGantt = 23,
//    DssGraph3DRiser2DHArea = 24,
//    DssGraph3DRiser2DHBars = 25,
//    DssGraph3DRiserHBoxPlot = 26,
//    DssGraph3DRiserHBoxPlotTC = 27,
//    DssGraph3DRiser2DHBubble = 28,
//    DssGraph3DRiserHContourPlot = 29,
//    DssGraph3DRiser2DHHistogram = 30,
//    DssGraph3DRiser2DHLines = 31,
//    DssGraph3DRiser2DHScatter = 32,
//    DssGraph3DRiserHinge = 33,
//    DssGraph3DRiser2DHistogram = 34,
//    DssGraph3DRiser2DLines = 35,
//    DssGraph3DRiserMultiPies = 36,
//    DssGraph3DRiserOpenHighClose = 37,
//    DssGraph3DRiserPie = 38,
//    DssGraph3DRiserPieBar = 39,
//    DssGraph3DRiserPolar = 40,
//    DssGraph3DRiserRadar = 41,
//    DssGraph3DRiser2DScatter = 42,
//    DssGraph3DRiserSpectralMap = 43,
//    DssGraph3DRiserTableChart = 44,
//    DssGraph3DRiserTextChart = 45,
//    DssGraph3DRiserRadarArea = 46,
//    DssGraph3DRiserJapaneseStock = 47,
//    DssGraph3DRiserStemLeaf = 48,
//    DssGraph3DRiserMultiBar = 49,
//    DssGraph3DRiserWaterFall = 50,
//    DssGraph3DRiserHWaterFall = 51,
//    DssGraph3DRiserFacetedFloater = 52,
//    DssGraph3DRiserFacetedRiser = 53,
//    DssGraph3DRiserBalanceScoreCard = 54,
//    DssGraph3DRiserProductPosition = 55,
//    DssGraph3DRiserResourceReturn = 56,
//    DssGraph3DRiserTimeSeries = 57,
//    DssGraph3DRiser4YChart = 58
//} EnumDSSGraph3DRiserType;

//typedef enum EnumDSSGraphStockLayout
//{
//    DssGraphStockLayoutDefault = 0,
//    DssGraphStockLayoutVerticalLine = 1,
//    DssGraphStockLayoutCandleStick = 2,
//    DssGraphStockLayoutLine = 3,
//    DssGraphStockLayoutArea = 4,
//    DssGraphStockLayoutLineCircle = 5,
//    DssGraphStockLayoutDot = 6,
//    DssGraphStockLayoutArea2 = 7
//} EnumDSSGraphStockLayout;
//
//typedef enum EnumDSSGraphGaugeStyle
//{
//    DssGraphGaugeStyleCircular = 0,
//    DssGraphGaugeStyleAlphaNumeric = 1,
//    DssGraphGaugeStyleLinear = 2,
//    DssGraphGaugeStyleLED = 3,
//    DssGraphGaugeStyleMultiLED = 4,
//    DssGraphGaugeStyleThermometer = 5,
//    DssGraphGaugeStyleCircularNew = 6
//} EnumDSSGraphGaugeStyle;

//typedef enum EnumDSSGraphGaugeBorderStyle
//{
//    DssGraphGaugeBorderStyleNone = 0,
//    DssGraphGaugeBorderStyleSimple = 1,
//    DssGraphGaugeBorderStyle3D = 2,
//    DssGraphGaugeBorderStyleEmboss = 3,
//    DssGraphGaugeBorderStyleDonut = 4,
//    DssGraphGaugeBorderStyleMetallic = 5,
//    DssGraphGaugeBorderStyleClipped = 6
//} EnumDSSGraphGaugeBorderStyle;
//
//typedef enum EnumDSSGraphGaugeNeedleStyle
//{
//    DssGraphGaugeNeedleStyleSimple = 0,
//    DssGraphGaugeNeedleStyleSimpleLong = 1,
//    DssGraphGaugeNeedleStyleSimpleWithBase = 2,
//    DssGraphGaugeNeedleStyleSteeple = 3,
//    DssGraphGaugeNeedleStyleSteepleLong = 4,
//    DssGraphGaugeNeedleStyleRectangular = 5,
//    DssGraphGaugeNeedleStyleThin = 6,
//    DssGraphGaugeNeedleStyleThinLong = 7,
//    DssGraphGaugeNeedleStyleBar = 100,
//    DssGraphGaugeNeedleStyleSlider = 101,
//    DssGraphGaugeNeedleStyleThermometer = 102,
//    DssGraphGaugeNeedleStyleSymbol = 103
//} EnumDSSGraphGaugeNeedleStyle;

typedef enum EnumDSSGraphOffScaleType
{
    DssGraphOffScaleHide = 0,
    DssGraphOffScaleClip = 1,
    DssGraphOffScaleShow = 2
} EnumDSSGraphOffScaleType;

//typedef enum EnumDSSGraphTitleName
//{
//    DssGraphTitleReserved = 0,
//    DssGraphTitleTitle = DssGraphTitle,
//    DssGraphTitleSubtitle = DssGraphSubtitle,
//    DssGraphTitleFootnote = DssGraphFootnote,
//    DssGraphTitleCategory = DssGraphXTitle,
//    DssGraphTitleSeries = DssGraphSeriesTitle,
//    DssGraphTitleY1 = DssGraphY1Title,
//    DssGraphTitleY2 = DssGraphY2Title,
//    DssGraphTitleY1Right = DssGraphY1RightTitle
//} EnumDSSGraphTitleName;
//
//typedef enum EnumDSSGraphAxisName
//{
//    DssGraphAxisNameReserved = 0,
//    DssGraphAxisCategory = DssGraphO1Body,
//    DssGraphAxisSeries = DssGraphO2Body,
//    DssGraphAxisX = DssGraphX1Body,
//    DssGraphAxisY1 = DssGraphY1Body,
//    DssGraphAxisY2 = DssGraphY2Body,
//    DssGraphAxisY3 = DssGraphY3Body,
//    DssGraphAxisY4 = DssGraphY4Body,
//    DssGraphAxisZ = DssGraphZBody,
//    DssGraphAxisY1Right = DssGraphY1RightBody
//} EnumDSSGraphAxisName;

//typedef enum EnumDSSGraphAxisBooleanProperty
//{
//    DssGraphAxisBooleanReserved = 0,
//    DssGraphAxisDrawInReverse = DssGraph2ScaleDirection,
//    DssGraphAxisShowAxisLine = DssGraphShow2DAxisLine,
//    DssGraphAxisUseLogrithmicScale = DssGraph2LogScale,
//    DssGraphAxisAlwaysIncludeZero = DssGraph2ExcludeZero,
//    DssGraphAxisUseMinValue = DssGraph2ScaleUseMin,
//    DssGraphAxisUseMaxValue = DssGraph2ScaleUseMax,
//    DssGraphAxisShowAxisLabels = DssGraphShow2DAxisLabels,
//    DssGraphAxisShowMaximumLabel = DssGraph2ScaleEndMax,
//    DssGraphAxisShowMinimumLabel = DssGraph2ScaleEndMin,
//    DssGraphAxisShowCustomLine = DssGraphAxisShowUserLine,
//    DssGraphAxisShowGridLinesMajor = DssGraphShow2DAxisMajor,
//    DssGraphAxisUseManualGridMajor = DssGraphManualMajor,
//    DssGraphAxisShowGridLinesMinor = DssGraphShow2DAxisMinor,
//    DssGraphAxisUseManualGridMinor = DssGraphManualMinor,
//    DssGraphAxisAutoFitLabel = DssGraphAxisAutoFit,
//    DssGraphAxisStaggerLabel = DssGraphAxisStagger,
//    DssGraphAxisAllowFractionalGridStep = DssGraphAxisAllowFractionalGrids
//} EnumDSSGraphAxisBooleanProperty;

//typedef enum EnumDSSGraphAxisLongProperty
//{
//    DssGraphAxisLongReserved = 0,
//    DssGraphAxisLabelsLocation = DssGraph2DSide,
//    DssGraphAxisGridStyleMajor = DssGraphStyleMajor,
//    DssGraphAxisGridStyleMinor = DssGraphStyleMinor,
//    DssGraphAxisLabelFreq = DssGraphAxisScaleFreq,
//    DssGraphAxisOffScaleType = DssGraphOffScaleType
//} EnumDSSGraphAxisLongProperty;
//
//typedef enum EnumDSSGraphAxisLabelLocation
//{
//    DssGraphAxisLabelLowSide = 0,
//    DssGraphAxisLabelHighSide = 1,
//    DssGraphAxisLabelBothSides = 2,
//    DssGraphAxisLabelNeitherSide = 3
//} EnumDSSGraphAxisLabelLocation;
//
//typedef enum EnumDSSGraphAxisDoubleProperty
//{
//    DssGraphAxisDoubleReserved = 0,
//    DssGraphAxisMinValue = DssGraph2ScaleMin,
//    DssGraphAxisMaxValue = DssGraph2ScaleMax,
//    DssGraphAxisAxisCustomValue = DssGraphAxisUserLineValue,
//    DssGraphAxisGridIntervalMajor = DssGraphCountMajor,
//    DssGraphAxisGridIntervalMinor = DssGraphCountMinor
//} EnumDSSGraphAxisDoubleProperty;

//typedef enum EnumDSSGraphSeriesBooleanProperty
//{
//    DssGraphSeriesBooleanReserved = 0,
//    DssGraphSeriesShowStepLine = DssGraphShowStepLine,
//    DssGraphSeriesOnSecondaryAxis = DssGraph2DSDSplitY,
//    DssGraphSeriesConnectBubbles = DssGraphShowSDLine,
//    DssGraphSeriesShowMarkers = DssGraphShowMarker,
//    DssGraphSeriesShowDataText = DssGraphSDShowDataText,
//    DssGraphSeriesShowLeaderLines = DssGraphShowFeeler,
//    DssGraphSeriesShowEquation = DssGraphShowSDLinrFormula,
//    DssGraphSeriesShowCoefficients = DssGraphShowSDLinrCorr,
//    DssGraphSeriesShowToolTip = DssGraphSDShowToolTip,
//    DssGraphSeriesDeleteSlice = DssGraphDeleteSlice
//} EnumDSSGraphSeriesBooleanProperty;
//
//typedef enum EnumDSSGraphSeriesLongProperty
//{
//    DssGraphSeriesLongReserved = 0,
//    DssGraphSeriesRiserType = DssGraphSDRiserType,
//    DssGraphSeriesRiserShape = DssGraphSDMarkerShape,
//    DssGraphSeriesPieSliceDetach = DssGraphDetachSlice,
//    DssGraphSeriesMarkerSize = DssGraphSDMarkerSize,
//    DssGraphSeriesMarkerShape = DssGraphSDMarkerShape,
//    DssGraphSeriesRegressionType = DssGraphSDRegressionType,
//    DssGraphSeriesOrder = DssGraphSDPolyFactor,
//    DssGraphSeriesPeriod = DssGraphSDMOVAFactor,
//    DssGraphSeries3DRiserType = DssGraphSD3DRiserType,
//    DssGraphSeriesPieDataTextType = DssGraphShowLBLFeeler
//} EnumDSSGraphSeriesLongProperty;

//typedef enum EnumDSSGraphBooleanProperty
//{
//    DssGraphBooleanReserved = 0,
//    DssGraphUseDepth = DssGraphDepthMode,
//    DssGraphShowAxisSplit = DssGraphShowDivBiPolar,
//    DssGraphShowTotalValues = DssGraphShowTotalValue,
//    DssGraphShowErrBar = DssGraphAxisHasErrorBar,
//    DssGraphHasMiddleValue = DssGraphAxisHasMiddleValue,
//    DssGraphIs2DcircularPie = DssGraph2DPie,
//    DssGraphShowDataValues = DssGraphShowDataValue,
//    DssGraphShowDataValuesAsPercentage = DssGraphShowDVPercent,
//    DssGraphShowDataLabels = DssGraphShowDataLabel,
//    DssGraphShowLeaderLines = DssGraphShowFeeler,
//    DssGraphShowLabelsOnSide = DssGraphShowPieLabelOnSide,
//    DssGraphShowPieName = DssGraphShowLBLPie,
//    DssGraphShowPieTotal = DssGraphShowLBLRing,
//    DssGraphShowMarkers = DssGraphShowMarker,
//    DssGraphShowLines = DssGraphShowLine,
//    DssGraphConnectDataPoints = DssGraphShowSDLine,
//    DssGraphUseSpiderGridLines = DssGraphSpiderLines,
//    DssGraphUseCircularDataLines = DssGraphRadarCircularDataLines,
//    DssGraphShowY1AxisOnLeft = DssGraphRemoteAxis,
//    DssGraphMetric1Show = DssGraphShowMetric1,
//    DssGraphMetric2Show = DssGraphShowMetric2,
//    DssGraphMetric3Show = DssGraphShowMetric3,
//    DssGraphTieLinesToFloor = DssGraphShow3DLineToFloor,
//    DssGraphTieLinesToLeftWall = DssGraphShow3DLineToLWall,
//    DssGraphTieLinesToRightWall = DssGraphShow3DLineToRWall,
//    DssGraphTieLinesToNeighbor = DssGraphShow3DScatterLine,
//    DssGraphPieSizeProportional = DssGraphPieForceRound,
//    DssGraph3DShowFloor = DssGraphShow3DFloor,
//    DssGraph3DShowRWall = DssGraphShow3DRWall,
//    DssGraph3DShowLWall = DssGraphShow3DLWall,
//    DssGraphPolarConnectFirstLast = DssGraphConnectFirstLast,
//    DssGraphToolTipShow = DssGraphShowToolTip,
//    DssGraphUseSeriesColor = DssGraphCurveColorAsSeries,
//    DssGraphDualAxisAutomatic = DssGraph2DSplitYAutomatic,
//    DssGraphGaugeBand1Show = DssGraphGauge1Show,
//    DssGraphGaugeBand2Show = DssGraphGauge2Show,
//    DssGraphGaugeBand3Show = DssGraphGauge3Show,
//    DssGraphGaugeBand4Show = DssGraphGauge4Show,
//    DssGraphGaugeBand5Show = DssGraphGauge5Show,
//    DssGraphGaugeShowMarkerNeedle = DssGraphGaugeShowNeedle,
//    DssGraphGaugeDrawAxisLabelsOutside = DssGraphGaugeShowAxisLabelsOutside,
//    DssGraphLinkWallAndFloorColors = DssGraphLinkColors,
//    DssGraphValueAxisCrossesCat = DssGraph2DOrdSpaceMode,
//    DssGraphUseUniformBorders = DssGraphUniformBorders,
//    DssGraphDataTextShow = DssGraphSDShowDataText,
//    DssGraphRestorePie = DssGraphPieRestore,
//    DssGraphDisplaySeriesLines = DssGraphConnectStackBars,
//    DssGraphDataTextConnectLineShow = DssGraphDataTextShowConnectLine,
//    DssGraphAbsoluteFontAndLine = DssGraphLockFontSize,
//    DssGraphRingLabelUseFirstSeries = DssGraphUseFirstSeriesForRingLabel
//} EnumDSSGraphBooleanProperty;

//typedef enum EnumDSSGraphLongProperty
//{
//    DssGraphLongReserved = 0,
//    DssGraphDepth = DssGraph2DDepth,
//    DssGraphDepthDirection = DssGraph2DAngle,
//    DssGraphSplitAxisPosition = DssGraphSplitYPosition,
//    DssGraphDataValuesType = DssGraphShowDataText,
//    DssGraphOverlap = DssGraphGroupSpacing,
//    DssGraphGapWidth = DssGraphRiserWidth,
//    DssGraphRiserShape = DssGraphSDMarkerShape,
//    DssGraphMiddleValueSize = DssGraphMiddleValue,
//    DssGraphPieTilt = DssGraphPieTiltValue,
//    DssGraphPieDepth = DssGraphPieDepthValue,
//    DssGraphPieRotation = DssGraphPieRotationValue,
//    DssGraphExplodePie = DssGraphDetachSlice,
//    DssGraphPiesPerRow = DssGraphPiesPerRowValue,
//    DssGraphDataTextPosition = DssGraphAxisDataTextPlace,
//    DssGraphMarkerSize2D = DssGraphSDMarkerSize,
//    DssGraphMarkerShape2D = DssGraphSDMarkerShape,
//    DssGraphPolarAngle = DssGraphPolarStartAngle,
//    DssGraphNumberOfBuckets = DssGraphHistogramBuckets,
//    DssGraphStockLayout = DssGraphStockStyle,
//    DssGraphMetricStyle1 = DssGraphMetric1Style,
//    DssGraphMetricStyle2 = DssGraphMetric2Style,
//    DssGraphMetricStyle3 = DssGraphMetric3Style,
//    DssGraphBoxPlotTailShape = DssGraphTailShape,
//    DssGraphGaugeStartAngle = DssGraphGaugeRangeStartAngleValue,
//    DssGraphGaugeEndAngle = DssGraphGaugeRangeStopAngleValue,
//    DssGraphGaugeCircularThickness = DssGraphGaugeThickness,
//    DssGraphGaugeDrawDataLabels = DssGraphShowDataValue,
//    DssGraphFunnelWidth = DssGraphPieDepthValue,
//    DssGraphFunnelHeight = DssGraphPieRotationValue,
//    DssGraphPieDataTextType = DssGraphShowLBLFeeler,
//    DssGraphGaugeBoundingBoxType = DssGraphGaugeFancyBox,
//    DssGraphColorMode = DssGraphACMColorModel,
//    DssGraphPieHoleSize = DssGraphPieHoleSizeValue,
//    DssGraphStockDataType = DssGraphStockDataValueType,
//    DssGraphPieDataLabelPosition = DssGraphPieDataLabelLoc,
//    DssGraphPieDataValuePosition = DssGraphPieDataValueLoc,
//    DssGraphLocaleID = DssGraphLocale,
//    DssGraphTurnXCounterClockwise3D = DssGraph3DTurnXCounterClockwise,
//    DssGraphTurnYCounterClockwise3D = DssGraph3DTurnYCounterClockwise,
//    DssGraphTurnZCounterClockwise3D = DssGraph3DTurnZCounterClockwise,
//    DssGraphTurnXClockwise3D = DssGraph3DTurnXClockwise,
//    DssGraphTurnYClockwise3D = DssGraph3DTurnYClockwise,
//    DssGraphTurnZClockwise3D = DssGraph3DTurnZClockwise,
//    DssGraphMoveXTowardsOrigin3D = DssGraph3DMoveXTowardsOrigin,
//    DssGraphMoveYTowardsOrigin3D = DssGraph3DMoveYTowardsOrigin,
//    DssGraphMoveZTowardsOrigin3D = DssGraph3DMoveZTowardsOrigin,
//    DssGraphMoveXAwayFromOrigin3D = DssGraph3DMoveXAwayFromOrigin,
//    DssGraphMoveYAwayFromOrigin3D = DssGraph3DMoveYAwayFromOrigin,
//    DssGraphMoveZAwayFromOrigin3D = DssGraph3DMoveZAwayFromOrigin,
//    DssGraphDecreaseCubeX3D = DssGraph3DDecreaseCubeX,
//    DssGraphDecreaseCubeY3D = DssGraph3DDecreaseCubeY,
//    DssGraphDecreaseCubeZ3D = DssGraph3DDecreaseCubeZ,
//    DssGraphIncreaseCubeX3D = DssGraph3DIncreaseCubeX,
//    DssGraphIncreaseCubeY3D = DssGraph3DIncreaseCubeY,
//    DssGraphIncreaseCubeZ3D = DssGraph3DIncreaseCubeZ,
//    DssGraphDecreaseWallX3D = DssGraph3DDecreaseWallX,
//    DssGraphDecreaseWallY3D = DssGraph3DDecreaseWallY,
//    DssGraphDecreaseWallZ3D = DssGraph3DDecreaseWallZ,
//    DssGraphIncreaseWallX3D = DssGraph3DIncreaseWallX,
//    DssGraphIncreaseWallY3D = DssGraph3DIncreaseWallY,
//    DssGraphIncreaseWallZ3D = DssGraph3DIncreaseWallZ,
//    DssGraphPanXLeft3D = DssGraph3DPanXLeft,
//    DssGraphPanYUp3D = DssGraph3DPanYUp,
//    DssGraphRelative3D = DssGraph3DRelative,
//    DssGraphPanXRight3D = DssGraph3DPanXRight,
//    DssGraphPanYDown3D = DssGraph3DPanYDown,
//    DssGraphAbsolute3D = DssGraph3DAbsolute,
//    DssGraphZoomIn3D = DssGraph3DZoomIn,
//    DssGraphIncreasePerspectiveDistortion3D = DssGraph3DIncreasePerspectiveDistortion,
//    DssGraphStepSize3D = DssGraph3DStepSize,
//    DssGraphZoomOut3D = DssGraph3DZoomOut,
//    DssGraphDecreasePerspectiveDistortion3D = DssGraph3DDecreasePerspectiveDistortion,
//    DssGraphInitMatrix3D = DssGraph3DInitMatrix,
//    DssGraphGaugeStyle = DssGraphGaugeStyleValue,
//    DssGraphGaugeBorderStyle = DssGraphGaugeBorderStyleValue,
//    DssGraphGaugeBorderThickness = DssGraphGaugeBorderThicknessValue,
//    DssGraphGaugeNeedleStyle = DssGraphGaugeNeedleStyleValue
//} EnumDSSGraphLongProperty;

//typedef enum EnumDSSGraphDoubleProperty
//{
//    DssGraphDoubleReserved = 0,
//    DssGraphHistogramMinRange = DssGraphHistogramRangeMin,
//    DssGraphHistogramMaxRange = DssGraphHistogramRangeMax,
//    DssGraphGaugeBand1Min = DssGraphGauge1Min,
//    DssGraphGaugeBand1Max = DssGraphGauge1Max,
//    DssGraphGaugeBand2Min = DssGraphGauge2Min,
//    DssGraphGaugeBand2Max = DssGraphGauge2Max,
//    DssGraphGaugeBand3Min = DssGraphGauge3Min,
//    DssGraphGaugeBand3Max = DssGraphGauge3Max,
//    DssGraphGaugeBand4Min = DssGraphGauge4Min,
//    DssGraphGaugeBand4Max = DssGraphGauge4Max,
//    DssGraphGaugeBand5Min = DssGraphGauge5Min,
//    DssGraphGaugeBand5Max = DssGraphGauge5Max
//} EnumDSSGraphDoubleProperty;
//
//typedef enum EnumDSSGraphStringProperty
//{
//    DssGraphStringReserved = 0,
//    DssGraphMetric1Label = DssGraphMetric1Value,
//    DssGraphMetric2Label = DssGraphMetric2Value,
//    DssGraphMetric3Label = DssGraphMetric3Value
//} EnumDSSGraphStringProperty;

//typedef enum EnumDSSGraphStyleType
//{
//    DssGraphStyleTypeReserved = 0,
//    DssGraphStyleTypeBase = 1,
//    DssGraphStyleTypeBaseNoBinary = 2,
//    DssGraphStyleTypeDerived = 3
//} EnumDSSGraphStyleType;
//
//typedef enum EnumDSSGraphImportFlags
//{
//    DssGraphImportReserved = 0x00000000,
//    DssGraphImportFromOldReport = 0x00000001,
//    DssGraphImportPreserveGraphType = 0x00000002,
//    DssGraphImportDefault = 0x00000004,
//    DssGraphImportForce = 0x00000008,
//    DssGraphImportUseVisibility = 0x00000010,
//    DssGraphImportUseGraphType = 0x00000020,
//    DssGraphImportUseVirtualFontSize = 0x00000040,
//    DssGraphImportUseEnhancedSeriesEffect = 0x00000080,
//    DssGraphImportUseNestedLabels = 0x00000100,
//    DssGraphImportGenerateDelta = 0x00000200
//} EnumDSSGraphImportFlags;

typedef enum EnumDSSRWDCacheStates
{
    DssRWDCacheStatusReserved = 0x0,
    DssRWDCacheStatusReady = 0x00000001,
    DssRWDCacheStatusProcessing = 0x00000002,
    DssRWDCacheStatusInvalid = 0x00000004,
    DssRWDCacheStatusExpired = 0x00000008,
    DssRWDCacheStatusLoaded = 0x00000010,
    DssRWDCacheStatusPersisted = 0x00000020,
    DssRWDCacheStatusInfoDirty = 0x00000040,
    DssRWDCacheStatusResultDirty = 0x00000080,
    DssRWDCacheStatusConvertedFromReport = 0x00000100,
    DssRWDCacheStatusForeign = 0x00000200
} EnumDSSRWDCacheStates;

typedef enum EnumDSSRWDCacheTypes
{
    DssRWDCacheTypeReserved = 0x0,
    DssRWDCacheTypeMatching = 0x00000001
} EnumDSSRWDCacheTypes;

typedef enum EnumDSSRWDCacheIDType
{
    DssRWDCacheIDTypeReserved = 0,
    DssRWDCacheRWDID = 0x01,
    DssRWDCacheCacheID = 0x02,
    DssRWDCacheReportID = 0x03,
    DssRWDCacheTableName = 0x04,
    DssRWDCacheSecurityFilter = 0x05,
    DssRWDCacheUserID = 0x06,
    DssRWDCacheDBLoginID = 0x07,
    DssRWDCacheDBConnectionID = 0x08,
    DssRWDCacheDataLocale = 0x09,
    DssRWDCacheExportFormat = 0x0A,
    DssRWDCacheWHLoginName = 0x0B
} EnumDSSRWDCacheIDType;

typedef enum EnumDSSRWDCacheInfoOptions
{
    DssInfoBrowsing = 0x01,
    DssInfoDetail = 0x02,
    DssInfoClusterSynch = 0x04,
    DssInfoIncludeExcel = 0x010,
    DssInfoIncludePdf = 0x020,
    DssInfoIncludeCSV = 0x040,
    DssInfoIncludeHtml = 0x080,
    DssInfoIncludeXml = 0x100,
    DssInfoIncludeInstance = 0x200,
    DssInfoIncludeConverted = 0x400,
    DssInfoIncludeForeign = 0x80000000,
    DssInfoDefault = 0x1F1,
    DssInfoAllLocal = 0x1F2,
    DssInfoAll = 0x800001F2
} EnumDSSRWDCacheInfoOptions;

typedef enum EnumDSSRWDCacheOptions
{
    DssRWDCacheEnableNone = 0x00000000,
    DssRWDCacheEnablePrompt = 0x00000001,
    DssRWDCacheEnableNonPrompt = 0x00000002,
    DssRWDCacheEnableXml = 0x00000010,
    DssRWDCacheEnablePdf = 0x00000020,
    DssRWDCacheEnableExcel = 0x00000040,
    DssRWDCacheEnableCsv = 0x00000080,
    DssRWDCacheEnableHtml = 0x00000100,
    DssRWDCacheEnableAll = 0x000001F3,
    DssRWDCacheEnableGroupBy = 0x00000200
} EnumDSSRWDCacheOptions;

typedef enum EnumDSSRWDLevelCacheOptions
{
    DssEnableRWDLevelCacheNotSet = 0x80000000,
    DssEnableRWDLevelCacheNone = 0x00000000,
    DssEnableRWDLevelCacheXml = 0x00000010,
    DssEnableRWDLevelCachePdf = 0x00000020,
    DssEnableRWDLevelCacheExcel = 0x00000040,
    DssEnableRWDLevelCacheCsv = 0x00000080,
    DssEnableRWDLevelCacheHtml = 0x00000100,
    DssEnableRWDLevelCacheAll = 0x000001F0,
    DssEnableRWDLevelCacheGroupBy = 0x00000200
} EnumDSSRWDLevelCacheOptions;

typedef enum EnumICDSSClusterBroadcastMessageType
{
    CDSSClusterBroadcastMetadata =  0x01,
    CDSSClusterBroadcastCache = 0x02,
    CDSSClusterBroadcastSession = 0x03,
    CDSSClusterBroadcastInbox = 0x04,
    CDSSClusterBroadcastCube = 0x05,
    CDSSClusterBroadcastRWDCache = 0x06,
    CDSSClusterBroadcastMetadataBulk = 0x07,
    CDSSClusterBroadcastIndexMD = 0x08,
    CDSSClusterBroadcastBinaryCollection = 0x09,
    CDSSClusterBroadcastProjectInfo = 0x0a,
    CDSSClusterBroadcastBinary = 0x10,
    DssClusterBroadcastMsgHeaderLength = 32
} EnumICDSSClusterBroadcastMessageType;

typedef enum EnumDSSSourceCommand
{
    DssSourceCommandReserved,
    DssSourceCommandReserved2,
    DssSourceCommandSaveObjects,
    DssSourceCommandReserved3,
    DssSourceCommandDeleteObjects,
    DssSourceCommandGarbageCollection,
    DssSourceCommandSetChangeJournalState,
    DssSourceCommandGetChangeJournalState,
    DssSourceCommandPurgeChangeJournal,
    DssSourceCommandSearchChangeJournal,
    DSSSourceCommandDeleteMergeUser,
    DSSSourceCommandFindObjectsByPaths,
    DSSSourceCommandCopyObject
} EnumDSSSourceCommand;

typedef enum EnumDSSHyperlinkAnswerMode
{
    DssHyperlinkAnswerReserved = 0,
    DssHyperlinkAnswerSame = 1,
    DssHyperlinkAnswerNone = 2,
    DssHyperlinkAnswerClose = 3,
    DssHyperlinkAnswerDynamic = 4,
    DssHyperlinkAnswerStatic = 5,
    DssHyperlinkAnswerCurrentUnit = 6,
    DssHyperlinkAnswerAllValidUnits = 7,
    DssHyperlinkAnswerUseDefault = 8
} EnumDSSHyperlinkAnswerMode;

typedef enum EnumDSSHyperlinkType
{
    DssHyperlinkURL = 0,
    DssHyperlinkObject = 1
} EnumDSSHyperlinkType;

typedef enum EnumDSSCatalogFlags
{
    DssCatalogDefault = 0x00000000,
    DssCatalogGetTables = 0x00000001,
    DssCatalogGetColumns = 0x00000002,
    DssCatalogGetTablePrimaryKeys = 0x00000004,
    DssCatalogGetTableForeignKeys = 0x00000008,
    DssCatalogGetTableKeys = DssCatalogGetTablePrimaryKeys  | DssCatalogGetTableForeignKeys,
    DssCatalogGetTableSize = 0x00000010,
    DssCatalogGetTableContent = 0x00000020,
    DssCatalogGetColumnCardinality = 0x00000040,
    DssCatalogGetColumnContent = 0x00000080,
    DssCatalogGetFullCatalog = 0x000000ff,
    DssCatalogSelectedOnly = 0x00000100,
    DssCatalogApplyConnectionMapping = 0x00000200,
    DssCatalogAllNamespaces = 0x00000400,
    DssCatalogBypassCache = 0x00000800,
    DssCatalogGetFresh = 0x00001000,
    DssCatalogIgnoreNamespace = 0x00002000,
    DssCatalogIgnoreCase = 0x00004000,
    DssCatalogIgnoreInvalidNames = 0x00008000,
    DssCatalogReuseMatching = 0x00010000,
    DssCatalogReuseCompatible = 0x00020000,
    DssCatalogReuseAny = 0x00040000,
    DssCatalogAugmentExisting = 0x00080000,
    DssCatalogSortDescending = 0x00100000,
    DssCatalogSortTableNameFirst = 0x00200000,
    DssCatalogPopulateColumn = 0x00400000,
    DssCatalogGetNamespaces = 0x01000000,
    DssCatalogGenerateJSON = 0x04000000,
    DssCatalogGenerateObjectIds = 0x02000000,
    DssCatalogRequest = 0x00800000,
    DssCatalogCompareWithMetadata = 0x08000000,
    DssCatalogCompareWithReport = 0x40000000,
    DssCatalogGetSetWorkloadManagementSQL = 0x10000000,
    DssCatalogGetRemoveWorkloadManagmentSQL = 0x20000000,
    DssCatalogGetTeradataViewColumns = 0x40000000
} EnumDSSCatalogFlags;

typedef enum EnumDSSCatalogStateFlags
{
    DssCatalogStateDefault = 0x00000000,
    DssCatalogStateSelected = 0x00000001,
    DssCatalogStateFresh = 0x00000002,
    DssCatalogStateMissing = 0x00000004,
    DssCatalogStateUnexpected = 0x00000008,
    DssCatalogStateCompatibleSmaller = 0x00000010,
    DssCatalogStateCompatibleLarger = 0x00000020,
    DssCatalogStateCompatible = 0x00000040,
    DssCatalogStateIncompatible = 0x00000080,
    DssCatalogStateCompatibilityMask = 0x000000f0,
    DssCatalogStateFreshTables = 0x00000100,
    DssCatalogStateMissingTables = 0x00000200,
    DssCatalogStateFreshColumns = 0x00000400,
    DssCatalogStateMissingColumns = 0x00000800,
    DssCatalogStateAllTables = 0x00001000,
    DssCatalogStateAllColumns = 0x00002000,
    DssCatalogStateExceedLimit = 0x00004000,
    DssCatalogStatePartitionMappingTable = 0x00010000,
    DssCatalogStateDummyPartitionMappingTable = 0x00020000,
    DssCatalogStatePartitionSliceTable = 0x00040000,
    DssCatalogStateTeradataView = 0x00080000
} EnumDSSCatalogStateFlags;

typedef enum EnumDSSCatalogMode
{
    DssCatalogModeDefault = 0x00000001,
    DssCatalogModeSQL = 0x00000002,
    DssCatalogModeODBC = 0x00000003
} EnumDSSCatalogMode;

typedef enum EnumDSSCatalogCacheMode
{
    DssCatalogCacheModeDefault = 0x00000000,
    DssCatalogCacheModePerNamespace = 0x00000001,
    DssCatalogCacheModeTableOnly = 0x00000002
} EnumDSSCatalogCacheMode;

typedef enum EnumSEFacetFlag
{
    SE_Facet_ModificationTime = 0x00000001,
    SE_Facet_CreationTime = 0x00000002,
    SE_Facet_Type = 0x00000004,
    SE_Facet_Path = 0x00000008,
    SE_Facet_Owner = 0x00000010,
    SE_Facet_All = 0xFFFFFFFF
} EnumSEFacetFlag;

typedef enum EnumSESearchCommandFlag
{
    SE_Search_CrawlerServer_GetCrawlerJob = 0,
    SE_Search_CrawlerServer_CrawlAsync,
    SE_Search_CrawlerServer_GetProcessingCrawlerJobs,
    SE_Search_CrawlerServer_GetFinishedCrawlerJobs,
    SE_Search_CrawlerJob_GetJobID,
    SE_Search_CrawlerJob_Pause,
    SE_Search_CrawlerJob_Resume,
    SE_Search_CrawlerJob_Cancel,
    SE_Search_CrawlerJob_GetCrawlerJobState,
    SE_Search_CrawlerJob_GetCrawledObjects,
    SE_Search_IndexWriter_Optimize,
    SE_Search_IndexWriter_GetProjectIndexInfo,
    SE_Search_IndexWriter_DestroyProject,
    SE_Search_Properties_Refresh,
    SE_AutoCompleter_GetFlushInterval,
    SE_AutoCompleter_SetFlushInterval,
    SE_AutoCompleter_FindSuggestQueryList,
    SE_AutoCompleter_InsertQuery,
    SE_AutoCompleter_GetDBType,
    SE_AutoCompleter_SetDBType,
    SE_AutoCompleter_GetFrequency,
    SE_AutoCompleter_SetFrequency,
    SE_AutoCompleter_RemoveQuery,
    SE_AutoCompleter_GetQueryCount,
    SE_AutoCompleter_DeleteAllQueries,
    SE_IsIndexPathValid,
    SE_IsIndexMatchMD,
    SE_ForceIndexMatchMD,
    SE_IndexSearch_Search,
    SE_SearchSuggester_GetSuggestions,
    SE_SearchSuggester_AddFrequency,
    SE_SearchSuggester_GetFrequency,
    SE_SearchSuggester_SetFrequency,
    SE_Search_CrawlerServer_StartCrawl,
    SE_Search_CrawlerServer_Pause,
    SE_Search_CrawlerServer_Resume,
    SE_Search_CrawlerServer_Destroy,
    SE_Search_CrawlerServer_Enable,
    SE_Search_CrawlerServer_StartIncrementalCrawl,
    SE_Search_CrawlerServer_StopIncrementalCrawl,
    SE_Search_CrawlerServer_GetProjectIndexMetadata,
    SE_SearchService_Start,
    SE_SearchService_Stop,
    SE_SearchService_GetIsStarted,
    SE_SearchService_GetSearchEngineProperties,
    SE_SearchService_SetSearchEngineProperties
} EnumSESearchCommandFlag;

typedef enum EnumSEDocumentType
{
    SE_Document_Type_MDObject = 0,
    SE_Document_Type_Cache = 1,
    SE_Document_Type_InboxMessage = 2,
    SE_Document_Type_SQLObject = 3,
    SE_Document_Type_Unknown = 255
} EnumSEDocumentType;

typedef enum SearchClientType
{
    SEARCH_SERVICE_CLIENT_TYPE_UNDEFINED = 0,
    SEARCH_SERVICE_CLIENT_TYPE_MD = 1,
    SEARCH_SERVICE_CLIENT_TYPE_CACHE = 2
} SearchClientType;

typedef enum EnumSEFacetType
{
    SE_FacetType_CreationTime = 0,
    SE_FacetType_ModifyTime = 1,
    SE_FacetType_Type = 2,
    SE_FacetType_Owner = 3,
    SE_FacetType_Path = 4
} EnumSEFacetType;

typedef enum EnumSESearchType
{
    SE_SearchType_SimpleSearch = 0,
    SE_SearchType_RelationshipSearch = 1
} EnumSESearchType;

typedef enum EnumDSSSearchCrawlerState
{
    DssSearchCrawlerStateIdle = 0,
    DssSearchCrawlerStateCrawling = 1,
    DssSearchCrawlerStatePaused = 2,
    DssSearchCrawlerStateCanceled = 3,
    DssSearchCrawlerStateCompleted = 4,
    DssSearchCrawlerStateCompletedOn = 4,
    DssSearchCrawlerStateCompletedOff = 5,
    DssSearchCrawlerStateIncrementalCrawling = 6,
    DssSearchCrawlerStatePending = 7
} EnumDSSSearchCrawlerState;

typedef enum EnumDSSSearchCrawlerType
{
    DssSearchCrawlerTypeMD = 0,
    DssSearchCrawlerTypeCache = 1
} EnumDSSSearchCrawlerType;

typedef enum EnumDSSIndexSearchDomainFlag
{
    DssIndexSearchDomainMD = 0x00000001,
    DssIndexSearchDomainCache = 0x00000002,
    DssIndexSearchDomainInbox = 0x00000004
} EnumDSSIndexSearchDomainFlag;

typedef enum EnumDSSSearchPropertyType
{
    DssSearchPropertyTypeUser = 0,
    DssSearchPropertyTypeSearchEngine = 1,
    DssSearchPropertyTypeProject = 2
} EnumDSSSearchPropertyType;

typedef enum EnumDSSSearchPreferenceType
{
    SEARCH_PREFERENCE_TYPE_UNDEFINED = 0,
    SEARCH_PREFERENCE_TYPE_CUSTOM_QUERY = 1
} EnumDSSSearchPreferenceType;

typedef enum EnumDSSQuickSearchFlags
{
    DssQuickSearchFlagsReserved = 0x00000000,
    DssQuickSearchFlagsKeepOriginalOrder = 0x00000001,
    DssQuickSearchFlagsIsQuickSearchEnabled = 0x00000002
} EnumDSSQuickSearchFlags;

typedef enum EnumDSSBlobType
{
    DssBlobReserved = 0,
    DssBlobOther = 1,
    DssBlobImage = 2,
    DssBlobProjectPackage = 3,
    DssBlobExcel = 4
} EnumDSSBlobType;

typedef enum EnumDSSReconcileType
{
    DssReconcileReserved = 0,
    DssReconcileNoneByFailure = 1,
    DssReconcileNoneByAbsence = 2,
    DssReconcileReference = 3,
    DssReconcilePlaceholder = 4,
    DssReconcileByDefault = 5
} EnumDSSReconcileType;

typedef enum EnumDSSReconcileConstraintType
{
    DssConstraintReserved = 0,
    DssConstraintDependsOn = 1
} EnumDSSReconcileConstraintType;

typedef enum EnumDSSReconcileRoles
{
    DssReconcileRoleReserved = 0,
    DssReconcileRoleManifest = 0x01,
    DssReconcileRoleMain = 0x02,
    DssReconcileRoleInDelta = 0x04
} EnumDSSReconcileRoles;

typedef enum EnumDSSSchemaManipulation
{
    DssSchemaManipulationReserved = 0,
    DssSchemaManipulationCreateObject = 1,
    DssSchemaManipulationDeleteObject = 2,
    DssSchemaManipulationRenameObject = 3,
    DssSchemaManipulationAddFactInfo = 4,
    DssSchemaManipulationRemoveFactInfo = 5,
    DssSchemaManipulationUpdateFactInfo = 6,
    DssSchemaManipulationAddAttributeInfo = 7,
    DssSchemaManipulationRemoveAttributeInfo = 8,
    DssSchemaManipulationUpdateAttributeInfo = 9,
    DssSchemaManipulationAutoRecognizeTable = 10,
    DssSchemaManipulationSetAsLookupTable = 11,
    DssSchemaManipulationAddAttributeForm = 12,
    DssSchemaManipulationRemoveAttributeForm = 13,
    DssSchemaManipulationUpdateAttributeForm = 14,
    DssSchemaManipulationUpdateColumn = 15,
    DssSchemaManipulationCreateRelationship = 16,
    DssSchemaManipulationGetRelationship = 17,
    DssSchemaManipulationUpdateAlias = 18,
    DssSchemaManipulationSetTablePrimaryDBRole = 19,
    DssSchemaManipulationAddTableDBRole = 20,
    DssSchemaManipulationRemoveTableDBRole = 21,
    DssSchemaManipulationRemoveRelationship = 22,
    DssSchemaManipulationChangeGuide = 23,
    DssSchemaManipulationChangeERType = 24,
    DssSchemaManipulationLastOne
} EnumDSSSchemaManipulation;

class ICDSSWriteBlockStream {
public:
    virtual HRESULT STDMETHODCALLTYPE WriteBoolean(VARIANT_BOOL) = 0;
    virtual HRESULT STDMETHODCALLTYPE WriteShort(short) = 0;
    virtual HRESULT STDMETHODCALLTYPE WriteShortArray(short *,
                                                      unsigned Int32) = 0;
    virtual HRESULT STDMETHODCALLTYPE WriteInt(Int32) = 0;
    virtual HRESULT STDMETHODCALLTYPE WriteIntArray(Int32 *,
                                                    unsigned Int32) = 0;
    virtual HRESULT STDMETHODCALLTYPE WriteFloat(float) = 0;
    virtual HRESULT STDMETHODCALLTYPE WriteFloatArray(float *,
                                                      unsigned Int32) = 0;
    virtual HRESULT STDMETHODCALLTYPE WriteDouble(double) = 0;
    virtual HRESULT STDMETHODCALLTYPE WriteDoubleArray(double *,
                                                       unsigned Int32) = 0;
    virtual HRESULT STDMETHODCALLTYPE WriteByte(char) = 0;
    virtual HRESULT STDMETHODCALLTYPE WriteByteArray(char *,
                                                     unsigned Int32) = 0;
    virtual HRESULT STDMETHODCALLTYPE WriteChar(wchar_t) = 0;
    virtual HRESULT STDMETHODCALLTYPE WriteString(wchar_t *) = 0;
    virtual HRESULT STDMETHODCALLTYPE WriteBSTR(BSTR) = 0;
    virtual HRESULT STDMETHODCALLTYPE WriteGUID(DSS_ID *) = 0;
    virtual HRESULT STDMETHODCALLTYPE WriteGUIDArray(DSS_ID *,
                                                     unsigned Int32) = 0;
    virtual HRESULT STDMETHODCALLTYPE WriteInt64(Int64) = 0;
#ifndef _MAC 
	#ifndef __ANDROID__
    virtual HRESULT STDMETHODCALLTYPE WriteTime(ICDSSTime *) = 0;
    virtual HRESULT STDMETHODCALLTYPE WriteTimeArray(ICDSSTime **,
                                                     unsigned Int32) = 0;
	#endif
#endif
    virtual HRESULT STDMETHODCALLTYPE SafeWriteInt(Int32) = 0;
    virtual HRESULT STDMETHODCALLTYPE WriteVariant(VARIANT *) = 0;
    virtual HRESULT STDMETHODCALLTYPE WriteBlockHeader(short) = 0;
    virtual HRESULT STDMETHODCALLTYPE FinishWriteBlock() = 0;
    virtual HRESULT STDMETHODCALLTYPE FinishWriteBlockEx(short) = 0;
    virtual HRESULT STDMETHODCALLTYPE IsEmpty(VARIANT_BOOL *) = 0;
    virtual HRESULT STDMETHODCALLTYPE Rewind() = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Size(unsigned Int32 *) = 0;
};
class ICDSSReadBlockStream {
public:
    virtual HRESULT STDMETHODCALLTYPE ReadBoolean(VARIANT_BOOL *) = 0;
    virtual HRESULT STDMETHODCALLTYPE ReadShort(short *) = 0;
    virtual HRESULT STDMETHODCALLTYPE ReadShortArray(unsigned Int32,
                                                     short *) = 0;
    virtual HRESULT STDMETHODCALLTYPE ReadInt(Int32 *) = 0;
    virtual HRESULT STDMETHODCALLTYPE ReadIntArray(unsigned Int32,
                                                   Int32 *) = 0;
    virtual HRESULT STDMETHODCALLTYPE ReadFloat(float *) = 0;
    virtual HRESULT STDMETHODCALLTYPE ReadFloatArray(unsigned Int32,
                                                     float *) = 0;
    virtual HRESULT STDMETHODCALLTYPE ReadDouble(double *) = 0;
    virtual HRESULT STDMETHODCALLTYPE ReadDoubleArray(unsigned Int32,
                                                      double *) = 0;
    virtual HRESULT STDMETHODCALLTYPE ReadByte(char *) = 0;
    virtual HRESULT STDMETHODCALLTYPE ReadByteArray(unsigned Int32,
                                                    char *) = 0;
    virtual HRESULT STDMETHODCALLTYPE ReadChar(wchar_t *) = 0;
    virtual HRESULT STDMETHODCALLTYPE ReadStringLength(unsigned Int32 *) = 0;
    virtual HRESULT STDMETHODCALLTYPE ReadString(unsigned Int32,
                                                 wchar_t *) = 0;
    virtual HRESULT STDMETHODCALLTYPE ReadBSTR(BSTR *) = 0;
    virtual HRESULT STDMETHODCALLTYPE ReadGUID(DSS_ID *) = 0;
    virtual HRESULT STDMETHODCALLTYPE ReadGUIDArray(unsigned Int32,
                                                    DSS_ID *) = 0;
    virtual HRESULT STDMETHODCALLTYPE ReadInt64(Int64 *) = 0;
#ifndef _MAC
	#ifndef __ANDROID__
    HRESULT STDMETHODCALLTYPE ReadTime(ICDSSTime *);
    HRESULT STDMETHODCALLTYPE ReadTimeArray(unsigned Int32,
                                                    ICDSSTime **);
    HRESULT STDMETHODCALLTYPE ReadTimeOrEnterBlock(ICDSSTime *,
                                                           short *);
    #endif															
#endif
    virtual HRESULT STDMETHODCALLTYPE ReadSafeInt(Int32 *) = 0;
    virtual HRESULT STDMETHODCALLTYPE ReadVariant(VARIANT *) = 0;
    virtual HRESULT STDMETHODCALLTYPE ReadBlockHeader(short *) = 0;
    virtual HRESULT STDMETHODCALLTYPE FinishReadBlock() = 0;
    virtual HRESULT STDMETHODCALLTYPE IsThereData(VARIANT_BOOL *) = 0;
    virtual HRESULT STDMETHODCALLTYPE Rewind() = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Size(unsigned Int32 *) = 0;

    virtual HRESULT STDMETHODCALLTYPE MoreDataInBlock(VARIANT_BOOL *) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_StreamVersion(Int32 *) = 0;
    virtual HRESULT STDMETHODCALLTYPE put_StreamVersion(Int32) = 0;
};
class ICDSSTemplateMetric {
#ifndef _MAC
	#ifndef __ANDROID__
    virtual HRESULT STDMETHODCALLTYPE get_Index(Int32 *) = 0;
    virtual HRESULT STDMETHODCALLTYPE put_Index(Int32) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Metric(ICDSSMetric **) = 0;
    virtual HRESULT STDMETHODCALLTYPE put_Metric(ICDSSMetric *) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_MetricID(DSS_ID *) = 0;
    virtual HRESULT STDMETHODCALLTYPE put_MetricID(DSS_ID *) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_MetricLimit(ICDSSExpression **) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_MetricExports(ICDSSExports **) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_EvaluationOrder(Int32 *) = 0;
    virtual HRESULT STDMETHODCALLTYPE put_EvaluationOrder(Int32) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_IsDecomposed(EnumDSSDecomposable *) = 0;
    virtual HRESULT STDMETHODCALLTYPE put_IsDecomposed(EnumDSSDecomposable) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_AliasNS(BSTR *) = 0;
	#endif
#endif
};

class IDSSReportInstance;
class ICDSSReportInstance;
class IDSSObjectInfo;
class ICDSSObjectInfo;
class IDSSElement;
class ICDSSElement;
class IDSSElements;
class ICDSSElements;
class IDSSFolder;
class ICDSSFolder;
class ICDSSWriteMessage;
class ICDSSTypeMapping;
#endif
