/*
 *  DSSrwdconstants.h
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 2/26/10.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#ifndef RWD_Constants_H
#define RWD_Constants_H

#import "DSSIPhoneEnums.h"

//tehe
typedef enum EnumDssSliceFlag
{
    DssOriginalSlice = 0,
    DssDisplayValueSlice = 1,
    DssDisplayIndexSlice = 2,
    DssDisplayExtremeSlice = 3
} EnumDssSliceFlag;

typedef enum EnumDSSXTabKeyValue
{
    DSSXTabOriginalKeyBase = 0,
    DSSXTabTotalKey = -100,
    DSSXTabAnyKey = -5,  //junwang, 03/26/2012
    DSSXTabKeyUnset = -1
} EnumDSSXTabKeyValue;

//#import "../../../Common/MsiCom/MsiCom/Msi_Variant.h"
/*#ifndef _Msi_Variant
 typedef enum EnumVariantType
 {
 VT_EMPTY = 0,
 VT_NULL	 = 1,
 VT_I2	 = 2,
 VT_I4	 = 3,
 VT_R4	 = 4,
 VT_R8	 = 5,
 VT_CY	 = 6,
 VT_DATE	 = 7,
 VT_BSTR	 = 8,
 VT_DISPATCH	 = 9,
 VT_ERROR	 = 10,
 VT_BOOL	 = 11,
 VT_VARIANT	 = 12,
 VT_UNKNOWN	 = 13,
 VT_DECIMAL	 = 14,
 VT_I1	 = 16,
 VT_UI1	 = 17,
 VT_UI2	 = 18,
 VT_UI4	 = 19,
 VT_I8	 = 20,
 VT_UI8	 = 21,
 VT_INT	 = 22,
 VT_UINT	 = 23,
 VT_VOID	 = 24,
 VT_HRESULT	 = 25,
 VT_PTR	 = 26,
 VT_SAFEARRAY	 = 27,
 VT_CARRAY	 = 28,
 VT_USERDEFINED	 = 29,
 VT_LPSTR	 = 30,
 VT_LPWSTR	 = 31,
 VT_RECORD	 = 36,
 VT_FILETIME	 = 64,
 VT_BLOB	 = 65,
 VT_STREAM	 = 66,
 VT_STORAGE	 = 67,
 VT_STREAMED_OBJECT	 = 68,
 VT_STORED_OBJECT	 = 69,
 VT_BLOB_OBJECT	 = 70,
 VT_CF	 = 71,
 VT_CLSID	 = 72,
 VT_BSTR_BLOB	 = 0xfff,
 VT_VECTOR	 = 0x1000,
 VT_ARRAY	 = 0x2000,
 VT_BYREF	 = 0x4000,
 VT_RESERVED	 = 0x8000,
 VT_ILLEGAL	 = 0xffff,
 VT_ILLEGALMASKED	 = 0xfff,
 VT_TYPEMASK	 = 0xfff,
 } EnumVariantType;
 #endif*/

#ifndef _Msi_WinError
typedef enum EnumDSSErrorType
{
    //S_OK			= 0,
    //S_FALSE		,//= 1,
    //E_FAIL		,//= 2,
    //E_POINTER		,//= 3,
    E_DATASTREAM	= 4,
    E_DEFNSTREAM	,//= 5,
    //E_INVALIDARG	,//= 6,
    E_DATABLOCK		= 7,
    E_UNEXPECTED_DATATYPE ,//= 8,
    E_OUT_OF_MEMORY	,//= 9,
    E_INVALIDINDEX	,//= 10,
    //E_NOTIMPL		,//= 11,
    E_HANDLE_TYPE	= 12,
    E_ASSERT		,//= 13,
    E_EMPTY_DATASET ,//= 14,
    E_NaN			,//= 15,
    E_DEFN_EXCEED_LIMIT ,//= 16,
    E_READFILE,			//= 17,
    E_WRITEFILE         // = 18 tehe
}EnumDSSErrorType;
#endif

typedef enum EnumBinaryDefnBlockType
{
    BINARY_BLOCK_UNKNOWN = 0,
    BINARY_BLOCK_DOCUMENT_INSTANCE = 1,
    BINARY_BLOCK_DOCUMENT_INSTANCE_HEADER,
    BINARY_BLOCK_DOCUMENT_DEFINITION,
    BINARY_BLOCK_DEFAULT_FORMATS,
    BINARY_BLOCK_FORMAT,
    BINARY_BLOCK_PROPERTY_SET,
    BINARY_BLOCK_PROPERTY,
    BINARY_BLOCK_RWD_DATA_SETS,
    BINARY_BLOCK_RWD_DATA_SET,
    BINARY_BLOCK_RWD_UNIT_OBJECTS,
    BINARY_BLOCK_RWD_UNIT_OBJECT,
    BINARY_BLOCK_RWD_DERIVED_METRICS,
    BINARY_BLOCK_OBJECT_COLLECTION,
    BINARY_BLOCK_OBJECT,
    BINARY_BLOCK_OBJECT_INFO,
    BINARY_BLOCK_OBJECT_DEFINITION,
    BINARY_BLOCK_FORMATS,
    BINARY_BLOCK_SUBTOTALS,
    BINARY_BLOCK_SUBTOTAL,
    BINARY_BLOCK_SUBTOTAL_MAPS,
    BINARY_BLOCK_SUBTOTAL_MAP,
    BINARY_BLOCK_ANNOTATION,
    BINARY_BLOCK_RW_TREES,
    BINARY_BLOCK_RW_TREE,
    BINARY_BLOCK_RW_NODES,
    BINARY_BLOCK_RW_NODE,
    BINARY_BLOCK_RW_NODE_INFO,
    BINARY_BLOCK_RW_NODE_DEFINITION,
    BINARY_BLOCK_RW_GROUP_BY_UNITS,
    BINARY_BLOCK_RW_GROUP_BY_UNIT,
    BINARY_BLOCK_ELEMENTS,
    BINARY_BLOCK_ELEMENT,
    BINARY_BLOCK_RW_TARGETS,
    BINARY_BLOCK_RW_FIELDS,
    BINARY_BLOCK_RW_FIELD,
    BINARY_BLOCK_ATTRIBUTE_FORMS,
    BINARY_BLOCK_ATTRIBUTE_FORM,
    BINARY_BLOCK_RW_CONTROLS,
    BINARY_BLOCK_RW_CONTROL,
    BINARY_BLOCK_THRESHOLDS,
    BINARY_BLOCK_THRESHOLD,
    BINARY_BLOCK_SORTS,
    BINARY_BLOCK_SORT,
    BINARY_BLOCK_AXES,
    BINARY_BLOCK_AXIS,
    BINARY_BLOCK_TEMPLATE_UNITS,
    BINARY_BLOCK_TEMPLATE_UNIT,
    BINARY_BLOCK_TEMPLATE_UNIT_INFO,
    BINARY_BLOCK_TEMPLATE_UNIT_DEFINITION,
    BINARY_BLOCK_UNIT_LIMIT,
    BINARY_BLOCK_CONSOLIDATION_ELEMENT,
    BINARY_BLOCK_TEMPLATE_METRIC,
    BINARY_BLOCK_NODE_INFO,
    BINARY_BLOCK_GRAPH_DEFN,
    BINARY_BLOCK_ATTRIBUTE,
    BINARY_BLOCK_COLUMN,
    BINARY_BLOCK_ATTRIBUTE_COLUMN_MAPPING,
    BINARY_BLOCK_METRIC_COLUMN_MAPPING,
    BINARY_BLOCK_REPORT_INSTANCE,
    BINARY_BLOCK_REPORT_INSTANCE_HEADER,
    BINARY_BLOCK_REPORT_DEFINITION,
    BINARY_BLOCK_REPORT_DATA,
    BINARY_BLOCK_DATA_COLUMN,
    BINARY_BLOCK_EXPRESSION,
    BINARY_BLOCK_NODE,
    BINARY_BLOCK_NODES,
    BINARY_BLOCK_NODE_DEFINITION,
    BINARY_BLOCK_UNIT_CONDITION,
    BINARY_BLOCK_VIEW_FILTER,
    BINARY_BLOCK_OPERATOR_PROPERTY,
    BINARY_BLOCK_UNSET_STATUS,
    BINARY_BLOCK_DISABLED_METRICS,
    BINARY_BLOCK_DISABLED_UNITS,
    BINARY_BLOCK_DOCUMENT_RESULT_FORMATS,
    BINARY_BLOCK_TEMPLATE_FORMATS,
    BINARY_BLOCK_SOURCE_INFO,
    BINARY_BLOCK_SHEETS_INFO,
    BINARY_BLOCK_DOCUMENT_DATA_IMPORT_HEADER,
    BINARY_BLOCK_DEFAULT_FORMATS_NEW_OBJECT,
    
    // mluo 11/15/2010, Blocks for editable property in the template unit and template metric
    BINARY_BLOCK_UNIT_TRANSACTION_INFO,
    BINARY_BLOCK_RWD_TRANSACTION_REPORTS,
    BINARY_BLOCK_RWD_TRANSACTION_REPORT,
    
    // mluo 12/1/2010 for IVE
    BINARY_BLOCK_BASE_FORMS_INFO,
    BINARY_BLOCK_THRESHOLD_CONDITION,
    BINARY_BLOCK_THRESHOLD_ADDITIONAL_PROPERTIES,
    BINARY_BLOCK_UNIT_SUBTOTALS,
    
    // mluo 02/03/2011 for ive
    BINARY_BLOCK_REPLACE_ATTRIBUTE_IDS,
    // mluo 01/06/2011 for Transaction
    BINARY_BLOCK_RW_CONTROL_ACTION_INFO,
    
    // mluo, 2011-01-05, Transaction mappings serialization
    BINARY_BLOCK_TRANSACTION_INPUT_MAPPINGS,
    
    // mluo, 2011-02-24, Resolved formats for metric condition selector
    BINARY_BLOCK_RESOLVED_FORMATS,
    
    // JLI, 3/15/2011 Add the control title
    BINARY_BLOCK_CONTROL_TITLE,
    
    // mluo 5/12/2011, for has derived element info
    BINARY_BLOCK_HAS_DERIVED_ELEMENT,
    // forms in unit object
    BINARY_BLOCK_FORMS,
    // graph property count
    BINARY_BLOCK_GRAPH_DEFN_COUNT,
    
    // lcao 2011-8-15 for multi-document view
    BINARY_BLOCK_DOCUMENT_VIEWS,
    BINARY_BLOCK_DOCUMENT_VIEW,
    BINARY_BLOCK_NONSHAREDPROPERTIES_IN_VIEW,
    BINARY_BLOCK_NONSHAREDPROPERTIES_IN_VIEWS,
    
    // lcao 2011-8-31, for data driven input
    BINARY_BLOCK_CONTROL_TRANSACTION_INFO = 99,
    BINARY_BLOCK_TARGET_DATASETS = 100,
    
    // dli 6/19/12 serach selector
    BINARY_BLOCK_FORMAT_IN_UNITOBJECT = 102,
    
    // lcao 2011-11-3, store DSSDataElement ID in control, for DDIC
    BINARY_BLOCK_CONTROL_DATA_SOURCE = 103,
    
    BINARY_BLOCK_METRIC_TEXT = 106,
    
    // clark, added for dssdimty and dssdimtyunit
    BINARY_BLOCK_NODE_DIMTY = 107,
    BINARY_BLOCK_DIMTY_DEFN = 108,
    BINARY_BLOCK_DIMTYUNIT= 109,
    
    BINARY_BLOCK_NODE_PROPERTY = 105,
    
    BINARY_BLOCK_ATTRIBUTE_FORM_PROPERTY = 110,
    
    // wguo 8/9/2011 associated template node info
    BINARY_BLOCK_RW_ASSOCIATED_TEMPLATE_NODES_INFO = 111,
    BINARY_BLOCK_RW_ASSOCIATED_TEMPLATE_NODE = 112,
    
    // dli 6/20/12 search selector
    BINARY_BLOCK_DISPLAY_ATTRIBUTE_FORM = 113,
    
    // dli 6/19/12 search selector
    BINARY_BLOCK_DATA_TEMPLATE_FORMS_INFO = 123,
    
    // lcao 2012-5-24 595953
    BINARY_BLOCK_HAS_ALIAS = 124,
    
    // clark 01/11/2012, field calculation, data source id and linked fields/subfields
    BINARY_BLOCK_RW_FIELD_CALCULATION = 118,
    BINARY_BLOCK_RW_FIELD_DATASETID = 119,
    BINARY_BLOCK_RW_LINKEDFIELDS = 120,
    BINARY_BLOCK_RW_AUTO_TEXT_TYPE = 114,
    
    // lcao 2/2/2012
    BINARY_BLOCK_RW_SPECIAL_FIELD_GROUP_NODE = 115,
    BINARY_BLOCK_RW_NODE_SEPARATOR = 116,
    
    // dli 2/21/2012
    BINARY_BLOCK_NODE_DATA_SOURCE_ID = 117,
    BINARY_BLOCK_OPERATOR_FUNCTION_PROPERTY = 122,
    BINARY_BLOCK_OPERATOR_FUNCTION_PROPERTY_NAME = 135,
    
    // dli 7/16/2012
    BINARY_BLOCK_RW_FIELD_CALCULATIONRESOLVED = 127,
    
    // dli 7/17/2012
    BINARY_BLOCK_BROWSE_SORTS = 129,
    BINARY_BLOCK_RW_FIELD_CALCULATION_TEXT = 130,
    
    BINARY_BLOCK_ATTRIBUTE_BROWSE_FORMS = 128,
    
    BINARY_BLOCK_CONTROL_MATCH_FORMS = 132,
    BINARY_BLOCK_OFFLINETRANSACTIONINFO = 136,
    BINARY_BLOCK_OFFLINETRANSACTIONINFOS = 137,
    BINARY_BLOCK_TRANSACTION_THRESHOLDS = 138,
    BINARY_BLOCK_OFFLINE_COLUMN_INDEX = 139,
    BINARY_BLOCK_TRANSACTION_ACTION = 140,
    BINARY_BLOCK_OFFLINETRANSACTION_KEY = 141,
    BINARY_BLOCK_OLAPSORTS = 142,
    BINARY_BLOCK_OPERATOR_SYSTEM_FUNCTION_PROPERTY = 143,
    BINARY_BLOCK_FUNCTION_ORDERSIGNIFICANT = 144,
    
    // mluo 4/5/2010 Start a block for Drill informaiton
    BINARY_BLOCK_DOCUMENT_DRILL = 200,
    BINARY_BLOCK_DRILL_PATHS,
    BINARY_BLOCK_DRILL_PATH,
    BINARY_BLOCK_OBJECT_SECURITY,
    BINARY_BLOCK_ACL,
    BINARY_BLOCK_TEMPLATE_PROPS,
    BINARY_BLOCK_DRILL_PATHS_EXTENDED,
    
    // mluo 4/5/2010 Start a block for Data
    BINARY_BLOCK_DOCUMENT_DATA = 400,
    
    BINARY_BLOCK_DOCUMENT_DATASET = 401,
    
    BINARY_BLOCK_DATASET_PROMPT_ANSWER = 402,
    
    BINARY_BLOCK_DOCUMENT_INSTANCE_HEADER2 = 500,
    
    BINARY_BLOCK_DOCUMENT_INSTANCE_VERSION = 501,
    
    BINARY_BLOCK_DOCUMENT_INSTANCE_SMARTCLIENT = 502,
    
    BINARY_BLOCK_DOCUMENT_SMARTCLIENT_PROPERTY = 503,
    
    BINARY_BLOCK_DOCUMENT_IMAGE = 600,
    
    // mluo 2010-11-15 IVE-DataImport range: 800-999
    // Support for multi-form attribute mapping
    
    BINARY_BLOCK_ATTRIBUTE_MAPPING             = 800,
    BINARY_BLOCK_FORM_COLUMN_MAPPING           = 801,
    
    BINARY_BLOCK_OPERATOR_FUNCTION = 805,
    
    BINARY_BLOCK_METRIC_DECOMPOSABLE = 806,
    
    BINARY_BLOCK_RWD_TRANSACTION_REPORT_CSI = 807,
    
    BINARY_BLOCK_TEMPLATE_SUBTOTAL_TYPE_MAPPING = 808,
    BINARY_BLOCK_TEMPLATE_USERDEFINED_SUBTOTAL = 809,
    BINARY_BLOCK_METRIC_PARTITION_FUNCTION_ID = 810,
    BINARY_BLOCK_BIGDECIMAL = 811,
    BINARY_BLOCK_METRIC_USERDEFINED_SUBTOTAL = 812,
    
    // wguo 1/12/2011 Start a block for drill path binary version. Range: 1000-1199
    // Do not add other blocks in this range
    BINARY_BLOCK_DRILL_PATH_EXTENDED_PROPERTIES_910 = 1000,
    
    // jzeng 08/01/14 - document unit properties
    BINARY_BLOCK_UNIT_INFOS_COLLECTION = 1207,
    BINARY_BLOCK_RWD_UNIT_PROPS_UNIT_INFOS = 1208,
    BINARY_BLOCK_RWD_UNIT_PROPS_UNIT_INFO = 1209,
    BINARY_BLOCK_RWD_UNIT_PROPS_FORM_INFO = 1210,
    
    // jzeng 07/31/14 PlaceHolder Selector
    BINARY_BLOCK_RWD_CONTROL_PLACEHOLDER_INFO = 1215,
    
    // jzeng 06/23/2014 dropzone
    BINARY_BLOCK_TEMPLATE_DROPZONE_SET = 1216,
    BINARY_BLOCK_TEMPLATE_DROPZONE_INFO = 1217,
    
    BINARY_BLOCK_CONSOLIDATION_TYPE = 1218,
    BINARY_BLOCK_RWD_DATASET_SERVEMODE = 1219,
    
    // jzeng, 08/07/2014 – such as CDSSTemplate:: Seasonality
    BINARY_BLOCK_TEMPLATE_DRAW_VISUALIZATION = 1220,
    // jzeng, 08/07/2014 – such as CDSSTemplateMetric:: Models
    BINARY_BLOCK_TEMPLATEMETRIC_DRAW_VISUALIZATION = 1221,
    BINARY_BLOCK_TEMPLATEMETRIC_MODELS = 1222,
    // jzeng 08/11/14 PlaceHolder
    BINARY_BLOCK_PLACE_HOLDER_UNIT_CONDITIONS = 1223,
    BINARY_BLOCK_PLACE_HOLDER_UNIT_CONDITION = 1224,
    // yche 9/10/2014 Document Theme definition ID
    BINARY_BLOCK_DOCUMENT_THEME_DEFN_ID = 1225,
    // jzeng, 09/11/2014 – Serialize Element properties map
    BINARY_BLOCK_ELEMENTS_PROPERTIES_MAP = 1226,
    BINARY_BLOCK_ELEMENTS_PROPERTIES_MAP_KEY = 1227,
    BINARY_BLOCK_ELEMENTS_PROPERTIES_MAP_VALUE = 1228,
    
    // yaocli 11/13/2014 - Attribute Links in Document
    BINARY_BLOCK_DOCUMENT_ATTRIBUTE_LINKS = 1229,
    BINARY_BLOCK_DOCUMENT_ATTRIBUTE_LINK = 1230,
    
    BINARY_BLOCK_NODE_DATATYPE = 1231,	//ledong 3/12/2015
    BINARY_BLOCK_HAS_DIMTY = 1232, //msun, 3/13/2015
    //for attribute selector 9/1/2015 msun
    BINARY_BLOCK_ATTRIBUTE_SELECTOR_REPLACE_ATTRIBUTE = 1233,
    BINARY_BLOCK_ATTRIBUTE_SELECTOR_CANDIDATE_ATTRIBUTES = 1234,
    BINARY_BLOCK_REPLACE_BY_UNIT_ID = 1235,
    BINARY_BLOCK_IS_PUSHED_DOWN = 1236,
	
	BINARY_BLOCK_DOCUMENT_WIDGET = 1238,
    
    // weilin 3/22/2016 - support recursive attribute
    BINARY_BLOCK_EXPAND_COLLECTION = 1239,
    BINARY_BLOCK_EXPAND_INFO = 1240,
    BINARY_BLOCK_COLLAPSE_COLLECTION = 1241,
    BINARY_BLOCK_COLLAPSE_INFO = 1242,
    BINARY_BLOCK_EXPANDALL = 1243,
    BINARY_BLOCK_EXPAND_COLLAPSE_INFORMATION = 1244,
    
    // weilin 2016/06/28 US54024 Support secondary templates in map
    BINARY_BLOCK_SECONDARY_DATA = 1239,
    
} EnumBinaryDefnBlockType;

//typedef unsigned char DSSDataFlag,

typedef enum EnumDSSDataFlag
{
    DssDataOk = 0x00,
    DssDataNull = 0x01,
    DssDataInvalidCell = 0x02,
    DssDataTruncate = 0x04,
    DssDataOverflow = 0x08,
    DssDataUnderflow = 0x10,
    DssDataZerodivide = 0x20,
    DssDataInvalid = 0x40,
    DssDataUnknown = 0x80,
    DssDataUnCalculated = 0x81,
    DssDataMissing = 0x82,
    DssDataRWInvalid = 0x03,
    DssDataIgnoreCheckNull = 0x83
} EnumDSSDataFlag;

typedef enum EnumDSSCellHandleType
{
    DssCellHandleReserved			= 0,
    DssCellHandleXTabHeaderElement	= 1,
    DssCellHandleXTabData			= 2,
    DssCellHandleTemplateUnit		= 3,
    DssCellHandleTemplateUnitAndForm= 4,
    DssCellHandleCustomGroupAndDepth= 5,
    DssCellHandleField,
    DssCellHandleControl,
    DssCellHandleGroupByElement,
    DssCellHandlePaddingLeft,
    DssCellHandlePaddingUp,
    
    DssCellHandleLastOne
}EnumDSSCellHandleType;

typedef enum EnumDSSRWTemplateOrigin
{
    DssRWTemplateOriginReserved = 0,
    DssRWTemplateOriginLocal,
    DssRWTemplateOriginReportView
}EnumDSSRWTemplateOrigin;

typedef enum EnumDSSThresholdScope
{
    DssThresholdScopeReserved       = 0,
    DssThresholdScopeMetricOnly     = 1,
    DssThresholdScopeSubtotalOnly   = 2,
    DssThresholdScopeMetricAndSubtotal = 3
} EnumDSSThresholdScope;

/**
 * Constants for manipulation type
 */
typedef enum EnumManipulationCommandTypes
{
    ManipulationReserved= 0,
    ManipulationSetSelectionFromTemplate= 1,
    ManipulationSetSelectionFromControl= 2,
    ManipulationSortTemplate= 3,
    ManipulationPivotUnit= 4,
    ManipulationMoveMetric= 5,
    ManipulationSetGroupByElement= 6,
    ManipulationSetTemplateDisplayMode= 7,
    ManipulationSetLayout= 8,
    ManipulationSetTemplateWindowState= 9,
    
    // Added in the ive project
    
    ManipulationAddTemplate= 10,
    ManipulationAddUnitToTemplate= 11,
    ManipulationAddAttributeFormToTemplate= 12,
    ManipulationAddMetricToTemplate= 13,
    ManipulationRemoveUnit= 14,
    ManipulationPivotAll= 15,
    ManipulationAddLayout= 16,
    ManipulationRemoveRWNode= 17,
    ManipulationEditTemplateNode= 18,
    ManipulationSetNodeFormat= 19,
    ManipulationSetFieldFormat= 20,
    ManipulationAddField= 21,
    ManipulationRemoveField= 22,
    ManipulationEditField= 23,
    ManipulationEnableFilter= 24,
    ManipulationAddControl= 25,
    ManipulationRemoveControl= 26,
    ManipulationAddViewFilter= 27,
    ManipulationRemoveViewFilter= 28,
    ManipulationAddUnitConditionToTemplate= 29,
    ManipulationApplySortsToTemplate= 30,
    ManipulationDrill = 31,
    ManipulationSetCurrentPanel = 32,
    ManipulationDataChange = 33,
    ManipulationCommit = 34,
    ManipulationDiscard = 35,
    ManipulationRecalculate = 36,
    ManipulationMarkRow = 37,
    // add for map widget layout info window.
    ManipulationSetGroupByElementForLayoutInfoWindw = 38,
    
    ManipulationDataChangeForTxRcdAndXTab = 39, //2011-11-2 lcao, data change for transaction record and xtab display
    
    ManipulationSetCurrentControlElementStatus = 82,
    
    ManipulationAddDerivedMetricToGrid = 40, // clark, add derived metric to grid
    ManipulationAddDerivedMetricToDataset = 41,
    ManipulationUpdateDerivedMetric = 42,
    ManipulationRemoveDerivedMetricFromDataset = 43,
    ManipulationAddRankMetric = 44,
    ManipulationAddPercentToTotalMetric = 45,
    ManipulationAddShortcutMetric = 46,
    ManipulationValidateDerivedMetric = 47,
    ManipulationDuplicateTemplate = 48,
    ManipulationInsertLayout = 49,
    ManipulationAddTargets = 50,
    ManipulationRemoveTargets = 51,
    ManipulationSetTargets = 52,
    ManipulationCreateThreshold = 53,
    ManipulationClearThresholds = 54,
    ManipulationShowThresholds = 55,
    ManipulationRemoveLayout = 56,
    ManipulationMoveLayout = 57,
    
    ManipulationToggleCurrentSelectionOnControl = 100,
    ManipulationSetControlProperties = 101,
    ManipulationSetUnsetFromControl = 102,
    ManipulationSetPanelVisible = 103,
    ManipulationSetNodeProperties = 104,
    ManipulationSetProperties = 105,
    ManipulationSetFieldProperties = 106,
    ManipulationApplySegment = 123,
    ManipulationAddDataset = 124,
    ManipulationRemoveTemplate = 125,
    ManipulationSetGraphBinary = 126,
    ManipulationSetGraphDefaultProperties = 127,
    ManipulationAddControlOnTemplate = 128,
    ManipulationSetSelectionFromDIC = 130,
    ManipulationRevertDDIC = 131,
    ManipulationDiscardTransaction = 132,
    ManipulationSaveAs = 190, // clark, add ManipulationSaveAs
    ManipulationDuplicateLayout = 200, // duplicate layout
    ManipulationRemovePanel = 201,
    ManipulationSetActionOnControl = 202,
    ManipulationShowBanding = 203,
    ManipulationSetTemplateProperties = 204,
    ManipulationSetGraphProperties = 205,
    ManipulationExpandRecursiveAttribute = 206
}EnumManipulationCommandTypes;

// moved EnumDSSGraphHeaderProperties to EnumBasicGraphProperties in IphoneEnums.h



typedef enum EnumGraphMajorType
{
    MajorTypeVerticalArea= 0x00000001,
    
    /** Specifies a major graph type of Horizontal Area. */
    MajorTypeHorizontalArea= 0x00000002,
    
    /** Specifies a major graph type of Vertical Bar. */
    MajorTypeVerticalBar= 0x00000004,
    
    /** Specifies a major graph type of Horizontal Bar. */
    MajorTypeHorizontalBar= 0x00000008,
    
    /** Specifies a major graph type of Vertical Line. */
    MajorTypeVerticalLine= 0x00000010,
    
    /** Specifies a major graph type of Horizontal Line. */
    MajorTypeHorizontalLine= 0x00000020,
    
    /** Specifies a major graph type of Pie. */
    MajorTypePie= 0x00000040,
    
    /** Specifies a major graph type of Scatter. */
    MajorTypeScatter= 0x00000080,
    
    /** Specifies a major graph type of Polar. */
    MajorTypePolar= 0x00000100,
    
    /** Specifies a major graph type of Radar. */
    MajorTypeRadar= 0x00000200,
    
    /** Specifies a major graph type of Bubble. */
    MajorTypeBubble= 0x00000400,
    
    /** Specifies a major graph type of Hilo Stock. */
    MajorTypeHiloStock= 0x00000800,
    
    /** Specifies a major graph type of Histogram. */
    MajorTypeHistogram= 0x00001000,
    
    /** Specifies a major graph type of 3D Risers. */
    MajorType3DRisers= 0x00002000,
    
    /** Specifies a major graph type of 3D Floating. */
    MajorType3DFloating= 0x00004000,
    
    /** Specifies a major graph type of 3D Connect Group. */
    MajorType3DConnectGroup= 0x00008000,
    
    /** Specifies a major graph type of 3D Connect Series. */
    MajorType3DConnectSeries= 0x00010000,
    
    /** Specifies a major graph type of 3D Surface. */
    MajorType3DSurface= 0x00020000,
    
    /** Specifies a major graph type of 3D Scatter. */
    MajorType3DScatter= 0x00040000,
    
    /**
     * Soecifies a major graph type of Gauge.
     *
     * @since MicroStrategy Web 7.5.1
     */
    MajorTypeGauge= 0x00080000,
    
    /**
     * Soecifies a major graph type of Funnel.
     *
     * @since MicroStrategy Web 7.5.1
     */
    MajorTypeFunnel= 0x00100000,
    
    /**
     * Soecifies a major graph type of Parseto.
     *
     * @since MicroStrategy Web 7.5.1
     */
    MajorTypePareto= 0x00200000,
    
    /**
     * Soecifies a major graph type of Box Plot.
     *
     * @since MicroStrategy Web 7.5.1
     */
    MajorTypeBoxPlot= 0x00400000,
    
    /**
     * Soecifies a major graph type of Gantt.
     *
     * @since MicroStrategy Web 8.0.0
     */
    MajorTypeGantt= 0x00800000,
    
    /**
     * Specifies a major graph type Combination
     */
    MajorTypeCombination = 0x01000000
}EnumGraphMajorType;

typedef enum EnumMFGraphAttributeID
{
    FillColor=564,
    AreaTransparent=563,
    GradientProperties=567,
    FillAlpha=608,
    FillType=10002,
    LineTransparent=587,
    LineColor=588,
    LineStyle=589,
    LockFontSize=604,
    LineThickness=605,
    GraphAreaAlpha = 608,
    LineAlpha=609,
    BorderStyle=601,
    BevelProperties=612,
    FontColor=574,
    FontFamily=576,
    FontStyle=584,
    FontSize=603,
    FontAlpha=610,
    Alignment=570,
    Rotation=578,
    FormatString=-616,
    TextBoxValue=9999,
    RiserType=97,
    //TRENDLINES
    TrendlineLineColorFromSeries=13,
    TrendlinePeriod=14,
    TrendlineShowCoeficient=101,
    TrendlineShowFormula=103,
    TrendlineTypeLinearShow=104,
    TrendlineTypeLogarithmicShow=105,
    TrendlineTypeNaturalLogarithmicShow=106,
    TrendlineTypePolynomialShow=107,
    TrendlineTypePolynomialFitShow=108,
    TrendlineTypeMeanShow=109,
    TrendlineTypeMovingAverageShow=110,
    TrendlineTypeStandardDeviationShow=111,
    //Defaultvalues
    FootnoteShow=596,
    SubtitleShow=597,
    TitleShow=598,
    LegendShow=116,
    LegendBoxType=250,
    LegendMarkerOrientation=637,
    LegendPosition=644,
    DataLabelGeneralShow=113,
    DataLabelSeriesShow=247,
    DataLabelXPlacement=65,
    DataLabelYPlacement=66,
    DataLabelFormat=-113,
    SeriesRiserColor=556,
    //Axes
    AxisAllowFractionalGrids=10141,
    AxisXDescendingAxis=22,
    AxisY1DescendingAxis=23,
    AxisY2DescendingAxis=24,
    AxisXExcludeZero=25,
    AxisY1ExcludeZero=26,
    AxisY2ExcludeZero=27,
    AxisCategoryGridStyle=37,
    AxisCategory2GridStyle=38,//????????????
    AxisXGridStyle=39,
    AxisY1GridStyle=40,
    AxisY2GridStyle=41,
    AxisXLogScale=57,
    AxisY1LogScale=58,
    AxisY2LogScale=59,
    AxisXScaleEnd=74,
    AxisY1ScaleEnd=75,
    AxisY2ScaleEnd=76,
    AxisXScale=92,
    AxisY1Scale=93,
    AxisY2Scale=94,
    AxisCategoryLinesShow=-117,
    AxisXCategoryTitleShow=124,
    AxisY1TitleShow=125,
    AxisY2TitleShow=126,
    AxisXLinesShow=-127,
    AxisY1LinesShow=-128,
    AxisY2LinesShow=-129,
    AxisCategoryReverseOrder=558,
    AxisXFormatString=618,
    AxisY1FormatString=619,
    AxisY2FormatString=620,
    AxisZFormatString=621,
    AxisXGridStep=647,
    AxisY1GridStep=645,
    AxisY2GridStep=646,
    //CustomLines
    CustomLineShow=632,
    CustomLineValue=633,
    //Tooltips
    SeriesDependentToolTipShow=10004,
    SeriesDependentToolTipValue=10005,
    SeriesGeneralToolTipShow=10041,
    SeriesGeneralToolTipValue=10042,
    SeriesMarkerShape=60,
    SeriesMarkerSize=61,
    SeriesDependentMarkerSize=671,
    //Otherproperties
    SeriesDependentDataLineType=96,
    SeriesDependentBubbleLineShow=98,
    SeriesOnY2=140,
    //Graph
    GraphOverlap=10,
    GraphGapWidth=11,
    GraphDepthShow=21,
    GraphDepthDepth=20,
    GraphDepthDirection=18,
    GraphAxisSplitShow=114,
    GraphColorMode=549,
    GraphCurvedLines=804,
    EnhancedSeriesEffect=10123,
    //GraphPie
    GraphPieDepth=192,
    GraphPieElementsPerRow=201,
    GraphPieRotation=202,
    GraphPieDataLabelsShowFormat=-205,
    GraphPieLeaderLinesShow=204,
    GraphPieGroupLabelShow=206,
    GraphPieTotalShow=207,
    GraphPieExplode=209,
    GraphPieTilt=211,
    GraphPieDataValueLocation=638,
    GraphPieDataLabelLocation=639,
    
    BubbleScatterUnknown1 = 683,
    BubbleScatterUnknown2 = 671,
} EnumMFGraphAttributeID;

typedef enum EnumMFGraphObjectID
{
    
    Background = 1,
    GraphFrame = 2,
    Footnote = 3,
    Subtitle = 4,
    Title = 5,
    LegendArea = 6,
    LegendText = 9,
    DataLine = 258,
    DataLabel = 260,
    ErrorBar = 262,
    //Trendline
    TrendlineExponential = 263,
    TrendlineLinear = 264,
    TrendlineLogarithmic = 265,
    LinrNatLog = 266,
    TrendlinePolynomial = 267,
    LinrText = 268,
    MeanLine = 269,
    TrendlineMovingAverage = 270,
    //Category Axis
    AxesCategoryLabel = 272,
    AxesCategoryMajorLine = 273,
    AxesCategoryMinorLine = 274,
    // AxesCategoryTitle = AxesXTitle,
    AxesCategorySuper = 275,
    Riser = 286,
    //X Axis
    AxesXBody = 289,
    AxesXLabel = 290,
    AxesXMajorLine = 291,
    AxesXMinorLine = 292,
    AxesXTitle = 293,
    AxesXZero = 294,
    //Y1 Axis
    AxesY1Body = 295,
    AxesY1Label = 296,
    AxesY1MajorLine = 297,
    AxesY1MinorLine = 298,
    AxesY1Title = 299,
    AxesY1Zero = 294,
    //Y2 Axis
    AxesY2Body = 301,
    AxesY2Label = 302,
    AxesY2MajorLine = 303,
    AxesY2MinorLine = 304,
    AxesY2Title = 305,
    AxesY2Zero = 306,
    //Pie
    PieGroupLabel = 339,
    
    PieRingTotalLabel = 340,
    //Custom Lines
    CustomLine1 = 414,
    CustomLine2 = 415,
    CustomLine3 = 416,
    CustomLine4 = 417,
    PieDataLabel = 418,
    //Defaults
    Default = 9999,
    DefaultTitlesText = 10001,
    DefaultAxesText = 10002,
    DefaultText = 10000,
} EnumMFGraphObjectID;

typedef enum EnumGraphMarkerShape
{
    GraphMarkerShapeNone = 0,
    GraphMarkerShapeRectangle = 1,
    GraphMarkerShapeStar45 = 2,
    GraphMarkerShapePlus = 3,
    GraphMarkerShapeCircle = 4,
    GraphMarkerShapeDiamond = 5,
    GraphMarkerShapeSpikedX = 6,
    GraphMarkerShapePlainX = 7,
    GraphMarkerShapeTriangle1 = 8,
    GraphMarkerShapeStarSkewed = 9,
    GraphMarkerShapeFatPlus = 10,
    GraphMarkerShapeStar90 = 11,
    GraphMarkerShapeSoftX = 12,
    GraphMarkerShapePiratePlus = 13,
    GraphMarkerShapeFatX = 14,
    GraphMarkerShapeCastle = 15,
    GraphMarkerShapeTriangle2 = 16,
    GraphMarkerShapeTriangle3 = 17,
    GraphMarkerShapeTriangle4 = 18,
    GraphMarkerShapeTriangle5 = 19,
    GraphMarkerShapeTriangle6 = 20,
    GraphMarkerShapeTriangle7 = 21,
    GraphMarkerShapeTriangle8 = 22,
    GraphMarkerShapeEllipse = 23,
    GraphMarkerShapeSquare = 24,
    GraphMarkerShapeHexagon = 25,
    GraphMarkerShapePentagon = 26,
    GraphMarkerShapeHouse = 27,
    GraphMarkerShapePentagram = 28,
    GraphMarkerShapeFontMarker = 29,
    GraphMarkerShapeBoxedPlus = 30,
    GraphMarkerShapeBoxedX = 31,
    GraphMarkerShapeHourGlass = 32,
    GraphMarkerShapeHourGlassTransparent = 33,
    GraphMarkerShapeVerticalLine = 34,
    GraphMarkerShapeHorizontalLine = 35,
    GraphMarkerShapeAsterisk = 36,
    GraphMarkerShapeStar5 = 37,
    GraphMarkerShapeStar6 = 38,
    GraphMarkerShapeStar8 = 39,
    GraphMarkerShapeBevelBox = 40,
    GraphMarkerShapeReverseBevelBox = 41,
    GraphMarkerShapeSlimHorizontalLine = 42,
    GraphMarkerShapeSlimVerticalLine = 43,
    GraphMarkerShapeSlimPlus = 44,
    GraphMarkerShapeSlimBoxedPlus = 45,
    GraphMarkerShapeSlimX = 46,
    GraphMarkerShapeSlimBoxedX = 47,
    GraphMarkerShapeRotatedHourGlass = 48,
    GraphMarkerShapeSmallBevelBox = 49,
} EnumGraphMarkerShape;

typedef enum EnumDSSRWControlActionType
{
    DssRWControlActionTypeReserved	        = 0x00000000,
    DssRWControlActionTypeUndo				= 0x00000001,
    DssRWControlActionTypeRedo				= 0x00000002,
    DssRWControlActionTypeDiscard			= 0x00000004,
    DssRWControlActionTypeSubmit			= 0x00000008,
    DssRWControlActionTypeRecalculate		= 0x00000010,
    DssRWControlActionTypeAll				= 0xffffffff,
} EnumDSSRWControlActionType;

typedef enum EnumDSSRWControlSubsequentAction
{
    DssRWControlSubsequentActionReserved				= 0x00000000,
    DssRWControlSubsequentActionDisplayMessage			= 0x00000001,
    DssRWControlSubsequentActionDisplayTransactionReport= 0x00000002,
    DssRWControlSubsequentActionRefreshGrid				= 0x00000004,
    DssRWControlSubsequentActionReexecuteDocument		= 0x00000008,
    DssRWControlSubsequentActionExecuteNewObject		= 0x00000010,
    DssRWControlSubsequentActionForceExecution			= 0x10000000,
    DssRWControlSubsequentActionInvalidateClientCache			= 0x20000000,
    DssRWControlSubsequentActionPassPromptAnswer		= 0x40000000,
} EnumDSSRWControlSubsequentAction;

typedef enum EnumDSSTransactionMappingFlags
{
    DssTransactionMappingDefault		        = 0,	// Never used
    
    /// The input column is optional
    DssTransactionMappingOptional				= 0x00000001,
    
} EnumDSSTransactionMappingFlags;

typedef enum EnumDSSTransactionDataChangeType
{
    DssTransactionDataReserved		        = 0x00000000,	// Never used
    DssTransactionDataUpdate				= 0x00000001,
    DssTransactionDataDelete				= 0x00000002,
    DssTransactionDataInsert				= 0x00000004,
    DssTransactionDataOriginal				= 0x00000008,
    DssTransactionDataCommit				= 0x00000010,
    
    DssTransactionRecordLast
    
} EnumDSSTransactionDataChangeType;

typedef enum EnumDSSFunctionSQLType
{
    DssFunctionSQLTypeReserved = 0,
    DssFunctionSQLTypeAggregation = 1,
    DssFunctionSQLTypeLogic = 2,
    DssFunctionSQLTypeArithmetic = 3,
    DssFunctionSQLTypeComparison = 4,
    DssFunctionSQLTypeRelative = 5
} EnumDSSFunctionSQLType;

typedef enum EnumDSSFunctionType
{
    DssFunctionTypeReserved = 0,
    DssFunctionTypeGeneric = 1,
    DssFunctionTypeSimple = 2,
    DssFunctionTypeAggregation = 3,
    DssFunctionTypeRelative = 4,
    DssFunctionTypeIndex = 5,
    DssFunctionTypeQualify = 6,
    DssFunctionTypeVVToS = 7
} EnumDSSFunctionType;

typedef enum EnumNullCheckingOption
{
    DssNumericErrorReserved = -100,
    DssNumericErrorSkip = 0,
    DssNumericErrorTreatAs = 1,
    DssNumericErrorPropagate = 2,
    DssNumericErrorCustom = 3,
    DssNumericErrorCalcOnly = 4
} EnumNullCheckingOption;

typedef enum EnumOrderingType
{
    DssDefaultOrdering = 0,
    DssExtendedOrdering = 1,
    DssOrionOrdering = 2
} EnumOrderingType;

typedef enum EnumDSSRWNodeTransactionChangeType
{
    DssRWNodeTransactionChangeTypeReserved		= 0x00000000,
    DssRWNodeTransactionChangeTypeDefault		= 0x00000001,
    DssRWNodeTransactionChangeTypeMarkSelection	= 0x00000002,
} EnumDSSRWNodeTransactionChangeType;

typedef enum EnumDSSRWMarkRowAction
{
    DssRWMarkRowUnset			= 1,
    DssRWMarkRowSelect			= 2,
} EnumDSSRWMarkRowAction;

typedef enum EnumDSSAttributeFormDisplayOption
{
    DssAttributeFormReserved    = 0,
    DssAttributeFormDefault     = 1,
    DssAttributeFormBrowse      = 2,
    DSSAttributeFormDisplay     = 3,
    DssAttributeFormCustom      = 4
} EnumDSSAttributeFormDisplayOption;
/*
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
 DSSDefaultFormatHTML= 9,
 DSSDefaultFormatPanelStack = 10,
 DSSDefaultFormatControl = 11,
 DSSDefaultFormatUnselectedItem = 12,
 DSSDefaultFormatSelectedItem = 13,
 DSSDefaultFormatHighlightedItem = 14,
 DSSDefaultFormatGridTitle = 15,
 DSSDefaultFormatPanelStackTitle = 16,
 DSSDefaultFormatPanel = 17,
 DSSDefaultFormatControlTitle = 18,
 DSSDefaultFormatLastOne = 19,
 } EnumDSSDefaultFormats;
 */
typedef enum EnumDSSParameterType
{
    DssParameterReserved = 0x00,
    DssParameterScalar = 0x01,
    DssParameterVector = 0x02
} EnumDSSParameterType;

typedef enum UseForAggType
{
    eDefault = 0,
    eUseDisplayedElement,
    eUseLeafElement
} UseForAggType;

typedef enum EnumDSSRuntimeSubtotalType
{
    DssRuntimeSubtotalDefault = 0x00000,
    DssRuntimeSubtotalExtremeMin = 0x00001,
    DssRuntimeSubtotalExtremeMax = 0x00002,
    DssRuntimeSubtotalExtremeSizeBy = 0x00010,
    DssRuntimeSubtotalExtremeColorBy = 0x00020,
    DssRuntimeSubtotalRefLine = 0x00004,
    DssRuntimeSubtotalTrendLine = 0x00008,
    DssRuntimeSubtotalPieTotal = 0x00040,
    DssRuntimeSubtotalGlobal = 0x00100,
    DssRuntimeSubtotalPerRow = 0x00200,
    DssRuntimeSubtotalPerColumn = 0x00400,
    DssRuntimeSubtotalPerRC = DssRuntimeSubtotalPerRow  | DssRuntimeSubtotalPerColumn,
    DssRuntimeSubtotalPerCell = 0x00800,
    DssRuntimeSubtotalCustom = 0x01000,
    DssRuntimeSubtotalDifferent = 0x02000,
    DssRuntimeSubtotalGroupedByX = 0x10000,
    DssRuntimeSubtotalGroupedByY = 0x20000,
    DssRuntimeSubtotalDualAxisBit = 0xf00000
} EnumDSSRuntimeSubtotalType;

#endif
