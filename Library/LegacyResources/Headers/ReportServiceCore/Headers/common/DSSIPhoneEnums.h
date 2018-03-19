/*
 *  DSSIPhoneEnums.h
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 2/26/10.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#ifndef IPHONEENUMS_H
#define IPHONEENUMS_H
typedef enum EnumDSSTemplateUnitType
{
    DssTemplateReserved = 0,
    DssTemplateAttribute = 1,
    DssTemplateDimension = 2,
    DssTemplateMetrics = 3,
    DssTemplateCustomGroup = 4,
    DssTemplateConsolidation = 5,
    DssTemplatePrompt = 6,
	DssTemplateRawUnit = 7,	
	DssTemplateMetric = 8		
} EnumDSSTemplateUnitType;

typedef enum EnumDSSTabularDataUnitType
{
    DssUnitReserved = 0,
    DssUnitAttribute = 1,
    DssUnitDimension = 2,
    DssUnitMetrics = 3,
    DssUnitCustomGroup = 4,
    DssUnitConsolidation = 5,
    DssUnitPrompt = 6,
    DssUnitRawUnit = 7,
    DssUnitRecursiveAttribute = 8,
    DssUnitRecursiveConsolidation = 9
} EnumDSSTabularDataUnitType;

typedef enum EnumDSSDataType
{
    DssDataTypeUnknown = -1,
    DssDataTypeReserved = 0,
    DssDataTypeInteger = 1,
    DssDataTypeUnsigned = 2,
    DssDataTypeNumeric = 3,
    DssDataTypeDecimal = 4,
    DssDataTypeReal = 5,
    DssDataTypeDouble = 6,
    DssDataTypeFloat = 7,
    DssDataTypeChar = 8,
    DssDataTypeVarChar = 9,
    DssDataTypeLongVarChar = 10,
    DssDataTypeBinary = 11,
    DssDataTypeVarBin = 12,
    DssDataTypeLongVarBin = 13,
    DssDataTypeDate = 14,
    DssDataTypeTime = 15,
    DssDataTypeTimeStamp = 16,
    DssDataTypeShort =21,
    DssDataTypeLong =22,
    DssDataTypeMBChar =23,
    DssDataTypeBool = 24,
    DssDataTypePattern = 25,
    DssDataTypeBigDecimal = 30,
    DssDataTypeCellFormatData = 31,
    DssDataTypeMissing = 32,
    DssDataTypeNChar = 17,
    DssDataTypeNVarChar = 18,
    DssDataTypeNPattern = 29,
	DssDataTypeUTF8Char = 33,
    DssDataTypeInt64 = 34,  //lwang, add support for int64
} EnumDSSDataType;

typedef enum EnumDSSBaseFormType
{
    DssBaseFormReserved = 0,
    DssBaseFormDateTime,
    DssBaseFormNumber,
    DssBaseFormText,
    DssBaseFormPicture,
    DssBaseFormUrl,
    DssBaseFormEmail,
    DssBaseFormHTMLTag,
    DssBaseFormDate,
    DssBaseFormTime,
    DssBaseFormSymbol,
    DssBaseFormBigDecimal
} EnumDSSBaseFormType;

typedef enum EnumDSSDocumentType
{
	DssDocumentTypeReserved			= 0,
	DssDocumentTypeBase				= 1,	// A base Document
	DssDocumentTypeView				= 2,	// A DocumentView
	DssDocumentTypeReportWritingDoc	= 3,	// A ReportWriting Document
} EnumDSSDocumentType;

typedef enum EnumDSSRWNodeType {
	DssRWNodeReserved	= 0,
	DssRWNodeSection	= 1,
	DssRWNodeElement	= 2,
	DssRWNodeTemplate,
	DssRWNodeFieldGroup,
	DssRWNodeDocument,
	DssRWNodeControl,
	DssRWNodeLastOne
} EnumDSSRWNodeType;

// This enumeration corresponds to "EnumDSSXMLRWSectionType" enumeration in XML API
typedef enum EnumDSSRWSectionType
{
	DssRWSectionTypeReserved	= 0,
	DssRWSectionContent,
	DssRWSectionReportHeader,
	DssRWSectionReportFooter,
	DssRWSectionPageHeader,
	DssRWSectionPageFooter,
	DssRWSectionGroupBySection,
	DssRWSectionGroupByHeader,
	DssRWSectionGroupByFooter,
	DssRWSectionBody,
	DssRWSectionBodyHeader,
	DssRWSectionDetails,
	DssRWSectionBodyFooter,
	DssRWSectionSubSection,
	DssRWSectionGeneric,
	DssRWSectionControlGroupBySection,
	DssRWSectionContainer,
	DssRWSectionPanel,
	DssRWSectionUnitCondition,
	DssRWSectionLastOne
} EnumDSSRWSectionType;


typedef enum EnumDSSRWFieldType
{
	DssRWFieldReserved = 0,
	DssRWFieldShape				= 1,
	DssRWFieldImage				= 2,
	DssRWFieldText		= 3,
	DssRWFieldConcatenation		= 3,
	DssRWFieldObjectName		= 4,
	DssRWFieldObjectValue		= 5,
	DssRWFieldLabel				= 6,
	DssRWFieldLine				= 7,
	DssRWFieldAutoText			= 8,
	DssRWFieldCalculation		= 9,
	DssRWFieldHTML				= 10,
	
	// Any new type should be added before the following one
	DssRWFieldLastOne
} EnumDSSRWFieldType;



typedef enum EnumDSSAutoText
{
	DssAutoTextReserved = 0,
	DssAutoTextPage,
	DssAutoTextNPage,
	DssAutoTextDateTime,
	DssAutoTextUser,
	DssAutoTextDocument,
	DssAutoTextProject,
	DssAutoTextGroupBy,
	DssAutoTextPrompt,
	DssAutoTextExecutionTime,
	DssAutoTextReport,
	DssAutoTextReportFilterDetails,
	DssAutoTextReportTemplateDetails,
	DssAutoTextReportExecutionTime,
	DssAutoTextAttributeElementID,
	DssAutoTextObjectID,
	DssAutoTextDocumentMessageID,
	DssAutoTextPromptAnswerXML,
	DssAutoTextServerName,
	DssAutoTextDocumentID,
	DssAutoTextReportID,
	DssAutoTextWebServer,
	DssAutoTextViewFilter,
	DssAutoTextNotes,
	DssAutoTextDocumentDescription,
	DssAutoTextReportDescription,
	DssAutoTextReportDetails,
	DssAutoTextDocumentPromptDetails,
	DssAutoTextReportPromptDetails,
	DssAutoTextReportFilterDetail,
	DssAutoTextReportLimitDetails,
	DssAutoTextTitle,
	
    DssAutoTextCurrentSelectionID,
    DssAutoTextAttributeLongElementID,
    DssAutoTextCurrentSelectionElementID,
	// Any new type should be added before the following one
	DssAutoTextLastOne
} EnumDSSAutoText;

typedef enum EnumDSSRWControlType
{
	DssRWControlTypeReserved	= 0,
	DssRWControlTypeElementList	= 1,
	DssRWControlTypeMetricList = 2,
	DssRWControlTypeTabList = 3,
	DssRWControlTypeAction  = 4,
    DssRWControlTypeMultipleUnit = 6,
    DssRWControlTypeAttributeList = 8
} EnumDSSRWControlType;

typedef enum EnumDSSRWControlFormatType
{
   	DssRWControlFormatReserved  = 0,      // Never used
	DssRWControlFormatControl			= 1,
	DssRWControlFormatUnselectedItem	= 2,
	DssRWControlFormatSelectedItem	= 3,
	DssRWControlFormatHighlightedItem = 4,
	DssRWControlFormatControlTitle = 5,
	DssRWControlFormatLastOne   
} EnumDSSRWControlFormatType;

typedef enum EnumDSSRWControlFilterType
{
    EnumDSSRWControlFilterTypeReserved = 0,
    EnumDSSRWControlFilterTypeSlicing = 1,
    EnumDSSRWControlFilterTypeUnitCondition = 2
} EnumDSSRWControlFilterType;

typedef enum EnumDSSRWNodeFormatType
{
    DssRWNodeFormatReserved  = 0,      // Never used
	
	DssRWNodeFormatNode			= 1,
	DssRWNodeFormatTitle		= 2,
	
	DssRWNodeFormatLastOne   
} EnumDSSRWNodeFormatType;

typedef enum EnumDSSElementType
{
	DssElementReserved		= 0,
	DssElementConcrete		= 1,
	DssElementMetric		= 2,
	DssElementSubtotal		= 3,
	DssElementFirst			= 4,	
	DssElementLast			= 5,
	DssElementDimension		= 6,
	DssElementConsolidation	= 7,
	
	// These element types appear in custom groups
	DssElementFilter		= 8,
	DssElementCustomGroup	= 9,
	DssElementJoint			=10,
	DssElementBand			=11,
	DssElementResidue		=12,
	DssElementSubexpression =13,
	
	DssElementAll			= 14,
	DssElementNode			= 15,
	DssElementNULL			= 16,
    DssElementAny           = 17,
    DssElementAttribute     = 18,
} EnumDSSElementType;


//*********************************************************
//		Enumerations for Subtotal and DSSSort
//*********************************************************

// This enumeration corresponds to "EnumDSSXMLSubtotalStyle" enumeration in WEB API
typedef enum EnumDSSSubtotalStyle 
{
	DssSubtotalReserved		= 0,	// Reserved value
	
	DssSubtotalNone			= 1,	// No axis subtotals appear on this axis.  (The default setting)
	
	DssSubtotalGrandTotal	= 2,	// Add a subtotal at the outermost level of the axis.
	
	DssSubtotalOutline		= 3,	// Add an axis subtotal at every level of the axis.  This includes the outermost level.
	
	DssSubtotalCube			= 4		// Add axis subtotals for every possible combination of units on the axis.
} EnumDSSSubtotalStyle ;


// This enumeration corresponds to "EnumDSSXMLTemplateSubtotalType" enumeration in WEB API
typedef enum EnumDSSTemplateSubtotalType 
{
	DssTemplateSubtotalReserved	= 0,	//Invalid template subtotal type.
	
	DssTemplateSubtotalList			= -1,	//The template subtotal is defined by giving an explicit list of the corresponding metric subtotals.
	
	DssTemplateSubtotalDefinition	= -2,	//The template subtotal is defined by taking all metric subtotals with the same name as the template subtotal.
	
	DssTemplateSubtotalDefault		= -3,	//The subtotal is defined by using the default subtotals from each metric.
	
	DssTemplateSubtotalAll			= -4	//Defines multiple template subtotals, so that every available metric subtotal is used.  We group subtotals from different metrics by name.
} EnumDSSTemplateSubtotalType;


// This enumeration corresponds to "EnumDSSXMLMetricType" enumeration in WEB API
typedef enum EnumDSSMetricType
{
	// Add new types of normal metrics at the beginning of this enumeration (before 'Normal')
	// Add new types of system subtotals at the end of this enumeration (before 'LastOne')
	
	DssMetricReserved	= 0,	//Invalid template subtotal type.
	
	//"A metric that contains a model for data-mining")]
	DssMetricDMX				= -7,
	
	//"An ordinary metric")]
	DssMetricNormal				= -8,	//A metric
	
	//"A customized subtotal definition (not a system subtotal)")]
	DssMetricSubtotal			= -9,	//A subtotal definition, but not a standard subtotal
	
	//"The summation of the all values (a system subtotal)")]
	DssMetricSubtotalSum		= -10,	
	
	//"The number of values being aggregated (a system subtotal)")]
	DssMetricSubtotalCount		= -11,	
	
	//"The normal average (a system subtotal)")]
	DssMetricSubtotalAverage	= -12,	
	
	//"The minimum value (a system subtotal)")]
	DssMetricSubtotalMin		= -13,	
	
	//"The maximum value (a system subtotal)")]
	DssMetricSubtotalMax		= -14,	
	
	//"The product of all of the values (a system subtotal)")]
	DssMetricSubtotalProduct	= -15,	
	
	//"The middle value when the values are sorted (a system subtotal)")]
	DssMetricSubtotalMedian		= -16,	
	
	//"The value that occurs most frequently (a system subtotal)")]
	DssMetricSubtotalMode		= -17,	
	
	//"A measure of the spread of the values (a system subtotal)")]
	DssMetricSubtotalStdDev		= -18,	
	
	//"The square of the standard deviation (a system subtotal)")]
	DssMetricSubtotalVariance	= -19,	
	
	//"The n-th root of the product of the values (a system subtotal)")]
	DssMetricSubtotalGeometricMean	= -20,
	
	//"The aggregation function used to perform further aggregation in subsetting")]
	DssMetricSubtotalAggregation = -21,
	
	// Always leave this one at the end
	// NB - the code knows that the enumeration is going backwards
	// NB - we know that this constant is inside the range
	DssMetricSubtotalReservedLastOne
} EnumDSSMetricType;

// This enumeration corresponds to "EnumDSSXMLSortType" enumeration in WEB API
typedef //"Technique used by sort-key to collate rows of data")]
enum EnumDSSSortType
{
	DssSortReserved		= 0,
	
	//"This sort modifies the meaning of DssSubtotalsPositionDefault for subsequent sorts in the collection")]
	DssSortSubtotalsPosition		= 1,
	
	//"The sort discriminates on the value of a form of an attribute")]
	DssSortAttributeForm			= 2,
	
	//"The sort specifies the attribute default sort to be applied")]
	DssSortAttributeDefault			= 3,
	
	//"The sort discriminates by taking values from the body of the grid")]
	DssSortMetric					= 4,
	
	//"The sort is the same as DssSortMetric except that the sort pays attention to the hierarchy induced by the template")]
	DssSortMetricHierarchical		= 5,
	
	//"The sort groups together all the rows that use the same metric")]
	DssSortGroupByMetric			= 6,
	
	//"The sort groups together rows if they have the same template subtotal in their header for the given attribute")]
	DssSortGroupBySubtotal			= 7,
	
	//"The sort is similar to the attribute form sort but applies to whatever attribute(s) are substituted for the given dimension")]
	DssSortDimensionForm			= 8,
	
	//"The sort is similar to the default attribute sort but applies to whatever attribute(s) are substituted for the given dimension")]
	DssSortDimensionDefault			= 9,
	
	//"The sort orders the rows so that the consolidation is put into its natural order")]
	DssSortConsolidationDefault		= 10,
	
	//"The sort orders the rows so that the custom group is put into its natural order")]
	DssSortCustomGroupDefault		= 11,
	
	//"The sort can only be used in an expression and it sorts the data that to which we want to apply the OLAP function so that it is in ascending/descending order")]
	DssSortValue					= 12,
	
	//"The sort orders the custom group elements according to underneath attribute form(s)")]
	DssSortCustomGroupForm			= 13,
	
	//"The sort orders elements by attribute elements indentified by column offset ")]
	DssSortAttributeColumn			= 14,
	
	//"DSSSort the unit elements according the subtotal tags on the unit ")]
	DssSortUnitSubtotal				= 15,
	
	//"The sort orders the DE elements according to the definition structure and underneath attribute sort definition")]
	DssSortDerivedElementDefault	= 16,
	
	//"The sort orders the DE elements according to the underneath attribute form value")]
	DssSortDerivedElementForm		= 17,
    
    //zhyang, selector sort optimization. The sort result is serialized form server side and saved in attribute, 
    //there is no need to sort it again. just get the result will be fine.
    DssSortAttributeOpt             = 18,
    
	//yimliu, 2014/07/22, 923948. Need to be consistent with server.
	DssSortConsolidationForm        = 24,
    
    DssSortByExplicitTemplateAxisRow = 18,
    DssSortByExplicitTemplateAxisColumn = 19,
    DssSortByExplicitTemplateAxisPage = 20,
    DssSortByImplicitTemplateAxisRow = 21,
    DssSortByImplicitTemplateAxisColumn = 22,
    DssSortByImplicitTemplateAxisPage = 23,
    DssSortByTemplateAxisRow = 25,
    DssSortByTemplateAxisColumn = 26,
    DssSortByTemplateAxisPage = 27,
    DssSortByExplicitDropZoneRoot = 32,
    DssSortByExplicitDropZoneEnd = DssSortByExplicitDropZoneRoot + 255,
    DssSortByImplicitDropZoneRoot = 512,
    DssSortByImplicitDropZoneEnd = DssSortByImplicitDropZoneRoot + 255,
    DssSortByDropZoneRoot = 1024,
    DssSortByDropZoneEnd = DssSortByDropZoneRoot + 255,
    DssSortByDynamicShortcutRoot = 1300,
    DssSortByDynamicShortcutEnd = DssSortByDynamicShortcutRoot + 255,
    
    //msun, 2016/6/8. financial report
    DssSortRecursiveAttributeDefault = 2048,
    DssSortRecursiveAttributeForm = 2049,
    DssSortRecursiveDerivedElementDefault = 2050,
    DssSortRecursiveDerivedElementForm = 2051,
    DssSortLastOne
} EnumDSSSortType;


// This enumeration corresponds to "EnumDSSXMLSubtotalsPosition" enumeration in WEB API
typedef //"Places to which a sort-key could collate a subtotal element")]
enum EnumDSSSubtotalsPosition
{
	DssSubtotalsPositionReserved	=	0,
	
	//"No special treatment for subtotal element")]
	DssSubtotalsPositionMix		=	1,	// No special treatment
	
	//"Put subtotal rows before ordinary rows")]
	DssSubtotalsPositionFirst	=	2,	// Put subtotal rows before ordinary rows.
	
	//"Put subtotal rows after ordinar rows")]
	DssSubtotalsPositionLast	=	3,	// Put subtotal rows after ordinar rows.
	
	//"Use the same subtotal position as was used for the previous sort in the collection")]
	DssSubtotalsPositionInherit	=	4	// Use the same subtotal position as was used for the previous sort in the collection.
} EnumDSSSubtotalsPosition;


//*********************************************************
//		Enumerations for AppObj
//*********************************************************

// This enumeration corresponds to "EnumDSSXMLAxisName" enumeration in WEB API
typedef enum EnumDSSAxisName
{
	DssAxisReserved	= 0,				
	
	//"The row axis")]
	DssAxisRows		= 1,			// The Rows axis - always exists
	
	//"The column axis")]
	DssAxisColumns	= 2,			// The Columns axis - nearly always exists
	
	//"The page axis")]
	DssAxisPages	= 3,			// The Pages axis, if any
	
	//"The chapter axis")]
	DssAxisChapters	= 4,			// The Chapters axis, if any (for Tensor specification)
	
	//All
	DssAxisAll = 5
	
} EnumDSSAxisName;

//****************************************************
//		Enumerations for ReportDefinition
//****************************************************


// Whether or not this is a subset report
typedef //"Classification of DSS Report Definition objects by whether they are defined as a subset of another report")]
enum EnumDSSReportType
{
	DssReportTypeReserved,
	//"This is either a regular report or a designated base report")]
	DssReportTypeBase,				// Report is independent of other reports, either a regular report or designated base
	//"This is a subset of a designated base report")]
	DssReportTypeSubsetting,		// Report is defined as a subset of a designated base report
	//"This is the base of a working set report")]
	DssReportTypePrivateBase,	// With the introduction of 8.0 WS model, private base is no longer used. Kept for backward compatibility.
	//"This is the subset of a working set report")]
	DssReportTypePrivateSubset,// With the introduction of 8.0 WS model, private subset is no longer used. Kept for backward compatibility.
	DssReportTypeRWBase,
	DSSReportTypeCSQLPreExec,				
	
	//"This is a shared cube report")]
	DssReportTypeCube,				// Add for Orion, indicate a shared cube definition
	
	//"This is a subset report")]
	DssReportTypeSubset,			// Add for Orion, indicate a subset report definition
} EnumDSSReportType;

// This property is used to define the (prefered) display mode for a report
typedef //"Ways of displaying the result of a report execution")]
enum EnumDSSDisplayMode
{
	DssDisplayModeReserved,
	
	//"The report is displayed as a grid")]
	DssDisplayModeGrid,				// Report is displayed as a grid of data 1
	
	//"The report is displayed as a graph")]
	DssDisplayModeGraph,			// Report is displayed as a graph 2
	
	//"The report is displayed as SQL")]
	DssDisplayModeEngine,			// Report is displayed as SQL 3
	
	//"The report is displayed as text")]
	DssDisplayModeText,				// Report is displayed using text 4
	
	//"The report is considered as a datamart report")]
	DssDisplayModeDatamart,			// Report is a datamart 5 
	
	//"The report is considered as a designated base report")]
	DssDisplayModeBase,				// Report is a designated base. 6 
	
	//"The report is displayed as both grid and graph")]
	DssDisplayModeGridGraph,		// Report is displayed as both grid and graph 7
	
	//"The report should be accessed via large file export - it is too big to display")]
	DssDisplayModeNonInteractive,	// Report is too large to display 8
	
	//"The report is considered as a cube report")]
	DssDisplayModeCube,				// Report is a designated base. 9 
	
	DssDisplayModeWidget,           // Report is displayed as a widget 10
	
} EnumDSSDisplayMode;

// ***************************************************************************
//		Enumerations for DSSObjectInfo
// ***************************************************************************

// EnumDSSObjectType enumerates all the types of COM objects that offer
// the IDSSObjectInfo interface.

// ** WARNING: Do not use values >= 1000 for this enumeration. 
// ** MD4 Server uses those values to record extended type info 
//
// Note: If you add or change ANY object type, also update DSSComMsgs.MC in the
// section starting from DSSCOM_STR_TYPE_START
//
// Note: If you add or chance ANY object type, update the gObjectLevelFromType function
// in DSSComDefs.cpp
// 
typedef
enum EnumDSSObjectType
{
	// Generic types
	DssTypeUnknown =				-1,
	DssTypeReserved =		0,
	
	// BEWARE: The reason why the numbers are not consecutive is that over time, we've deleted some types
	// but we didn't want to change the actual values of the existing types for fear of breaking anybodies code 
	
	// The following types are not published in DSSCOM.TLB. They are either internal or are for future use
#ifndef _DSSCOM_PUBLIC_TYPELIB
	DssTypeGeneric =		5,	// This type is now used by dummy object for the lock category.
	
	// DssTypeAgent =			6,	// fcamargo 8/29/01 Used for DssTypeStyle
	// DssTypeOwner =			9,	// hurwood, 2005-10-06, Used for DssTypeDevice
	// DssTypeView =			16,	// JLI 5/30/2000 Used for DssTypeDatamartReport
	// DssTypeDriver =			20,	// gzhou 4/21/2003 Used for DssTypeMonitor
	// DssTypeFindObject =		23, // fcamargo 8/29/01 Used for DssTypeFormat
	// DssTypeUserGroup =		35,	// mzhu 2005-09-27 Used for DssTypeTransmitter
	// DssTypeFunction2 =		41,	// JLI 5/30/2000 Used for DssTypeDatamart
	// DssTypeInBox =			45, // craczko 2/1/2007 Used for DssTypeLocale
	
	// fcamargo 8/29/01
	// BEWARE: We originally re-used DssTypeInBox and DssTypeInBoxMsg (45 and 46)
	// for Style and DSSFormat. However, this was a mistake because YOU CANNOT REUSE TYPES
	// IF ONE OBJECT IS A CONFIGURATION OBJECT AND THE OTHER A PROJECT OBJECT.
	// Because of that we were forced to change Style and DSSFormat to use Agent and FindObject.
	// However, for backwards compatability of development Boyd projetcs( engine regression
	// mem leak). We added some special code to deal with Style and DSSFormat objects using 45 and 46.
	// BEWARE: Be very careful if you want to re-use 45 and 46!!!!
	DssTypeFormatBAD =		46, // Used for DSSFormat objects created during Boyd
#endif
	
	// Persistent object types  (first class objects), these types can exist in the metadata
	DssTypeFilter =						1,
	DssTypeTemplate =					2,
	DssTypeReportDefinition =			3,
	DssTypeMetric =						4,
	DssTypeStyle =						6,
	DssTypeAggMetric =					7,
	DssTypeFolder =						8,
	DssTypeDevice =						9,
	DssTypePrompt =						10,
	DssTypeFunction =					11,
	DssTypeAttribute =					12,
	DssTypeFact =						13,
	DssTypeDimension =					14,
	DssTypeTable =						15,
	DssTypeDatamartReport =				16,
	DssTypeFactGroup =					17,
	DssTypeShortcut =					18,
	DssTypeResolution =					19,
	DssTypeMonitor =					20, 
	DssTypeAttributeForm =				21,
	DssTypeSchema =						22,
	DssTypeFormat =						23,
	DssTypeCatalog =					24,
	DssTypeCatalogDefn =				25,
	DssTypeColumn =						26,
	// It's been decided that this should not be removed from public typelib all the way 
	//		(but we keep it hidden, as API users aren't really supposed to be messing with them)
	DssTypePropertyGroup =		27,		
	DssTypePropertySet =				28,
	DssTypeDBRole =						29,
	DssTypeDBLogin =					30,
	DssTypeDBConnection =				31,
	DssTypeProject =					32,
	DssTypeServerDef =					33,
	DssTypeUser =						34,  
	DssTypeTransmitter =				35,
	DssTypeConfiguration =				36,
	DssTypeRequest =					37,
	DssTypeScript =						38,
	DssTypeSearch =						39,
	DssTypeSearchFolder =				40,
	DssTypeDatamart =					41,
	DssTypeFunctionPackageDefinition =	42,
	DssTypeRole	 =						43,
	DssTypeSecurityRole =				44,
	DssTypeLocale =						45,
	DssTypeConsolidation =				47,
	DssTypeConsolidationElement =		48,		
	DssTypeScheduleEvent =				49,
	DssTypeScheduleObject =				50,
	DssTypeScheduleTrigger =			51,
	DssTypeLink =						52,
	DssTypeDbTable =					53,
	DssTypeTableSource =				54,
	DssTypeDocumentDefinition =			55,
	DssTypeDrillMap =					56,
	DssTypeDBMS =						57,
	DssTypeMDSecurityFilter =			58,
	DssTypePromptAnswer =				59,
	DssTypePromptAnswers =				60,
	DssTypeGraphStyle	=				61,
	DssTypeChangeJournalSearch =		62,
    DssTypeBlob =                       63,
	
	// Binh Nguyen 2006-12-11 :
	// Added NCS objects, stored in and managed by not MD but NCSContentService
	DssTypeNCSObjects	=				0xFF,
	
	// BEWARE +++ BEWARE +++ BEWARE
	// DO NOT CREATE NEW DSS OBJECTS HERE
	// USE UP THE GAPS IN OUR ENUMERATION BEFORE CREATING NEW NUMBERS
	
	DssTypeReservedLastOne =	73
}  EnumDSSObjectType;

// EnumDSSSubType
// This enumeration lists the subtypes of objects
///typedef enum EnumDSSSubType
///{
///    DssSubTypeFilter =         0x0100,
///    DssSubTypeCustomGroup =    0x0101,
///}  EnumDSSSubType;

// EnumDSSNodeType
// This enumeration lists the types of nodes available in expressions
typedef enum EnumDSSNodeType
{
	DssNodeReserved =		0,
	DssNodeFormShortcut =			1,
	DssNodeElementsObject =			2,
	DssNodeConstant =				3,
	DssNodeOperator =				4,
	DssNodeShortcut =				5,
	DssNodeTime =					6,
	DssNodeRelationship =			7,
	DssNodeResidue =				8,
	DssNodeMe =						9,
	DssNodeBigDecimal =				10,
	DssNodeSQLQueryShortcut =		11,
	DssNodeSQLQueryColumnShortcut = 12,
	DssNodeSQLColumnShortcut =		13,
	DssNodeGroup =					14,
	DssNodeReference =				15,
	DssNodeCellFormatData =			16,
	DssNodeGridUnitShortcut =       17,
    DssNodeFieldShortcut =          18,
    DssNodeControlShortcut =        19
} EnumDSSNodeType;

typedef //"Classification applied to a DSS DSSAttribute Form object")]
enum EnumDSSFormType
{
	//"Never used")] 
	DssFormTypeReserved = 0,
	//"DSSAttribute form may be supported by many attributes")]
	DssFormTypeSystem,
	//"DSSAttribute form should only be supported by one attribute")]
	DssFormTypeNormal
} EnumDSSFormType;

typedef //"current element status")]
enum EnumDSSCurrentElementStatus
{
	DssCurrentElementReserved	= 0,
	DssCurrentElementUnset		= 1,
	DssCurrentElementChosen		= 2,
	DssCurrentElementDesired	= 3,
} EnumDSSCurrentElementStatus;

typedef enum EnumDSSRWTreeType {
	DssRWTreeTypeReserved	= 0,
	DssRWTreeTypeContent,
	DssRWTreeTypePageHeader,
	DssRWTreeTypePageFooter,
	DssRWTreeTypeClipBoard,
	
	// Any new type should be added before the following one
	DssRWTreeTypeLastOne
} EnumDSSRWTreeType;


typedef enum EnumDSSFieldGroupDataLevel {
	DssFieldGroupDataLevelReserved	= 0,
	DssFieldGroupDataLevelDefault,
	DssFieldGroupDataLevelSimple,
	DssFieldGroupDataLevelDefaultReport,
	DssFieldGroupDataLevelGroupBy,
	
	// Any new type should be added before the following one
	DssFieldGroupDataLevelLastOne
} EnumDSSFieldGroupDataLevel;


typedef enum EnumDSSFieldGroupJoin {
	DssFieldGroupJoinReserved	= 0,
	DssFieldGroupConcatenate,
	DssFieldGroupCompound,
	DssFieldGroupCrossJoin,
	
	// Any new type should be added before the following one
	DssFieldGroupJoinLastOne
} EnumDSSFieldGroupJoin;


typedef enum EnumDSSIteratorType
{
	DssIteratorReserved = 0,	// Never used
	DssIteratorSection,
	DssIteratorGrid,
	DssIteratorFieldGroup,
	DssIteratorControl,
	DssIteratorGroupby
} EnumDSSIteratorType;

typedef unsigned char DSSDataFlag;

/*
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
 DssDataRWInvalid = 0x03	
 } EnumDSSDataFlag;
 */

typedef enum EnumDSSDataModelType {
	DssModelReserved	= 0,
	DssModelXTab		= 1,
	DssModelControl		= 2,
	DssModelFieldGroup,
	DssModelGlobal,
	DssModelGroupby,
	DssModelLastOne
} EnumDSSDataModelType;

typedef enum EnumDSSGraphGradientType
{
    DssGraphGradientReserved = 0,
    DssGraphGradientCircular = 9,
    DssGraphGradientRectangular = 10,
    DssGraphGradientLinear = 14,
    DssGraphGradientStepLinear = 15
} EnumDSSGraphGradientType;

typedef enum EnumDSSGraphObject
{
    DssGraphNull = -3,
    DssGraphBackground = 1,
    DssGraphFrame = 2,
    DssGraphFootnote = 3,
    DssGraphSubtitle = 4,
    DssGraphTitle = 5,
    DssGraphLegendArea = 6,
    DssGraphLegendLine = 7,
    DssGraphLegendMarker = 8,
    DssGraphLegendText = 9,
    DssGraphCurve = 257,
    DssGraphDataLine = 258,
    DssGraphDataMarker = 259,
    DssGraphDataText = 260,
    DssGraphDivBiPolar = 261,
    DssGraphErrorBar = 262,
    DssGraphLinrExp = 263,
    DssGraphLinrLine = 264,
    DssGraphLinrLog = 265,
    DssGraphLinrNatLog = 266,
    DssGraphLinrNPoly = 267,
    DssGraphLinrText = 268,
    DssGraphMeanLine = 269,
    DssGraphMovAvgLine = 270,
    DssGraphO1Body = 271,
    DssGraph2DO1Label = 272,
    DssGraphO1Major = 273,
    DssGraphO1Minor = 274,
    DssGraphO1Super = 275,
    DssGraphO1Title = 276,
    DssGraphO2Body = 277,
    DssGraph2DO2Label = 278,
    DssGraphO2Major = 279,
    DssGraphO2Minor = 280,
    DssGraphO2Title = 281,
    DssGraphConnectDataLine = 282,
    DssGraphRiser = 286,
    DssGraphRiserShadow = 287,
    DssGraphStdDevLine = 288,
    DssGraphX1Body = 289,
    DssGraphX1Label = 290,
    DssGraphX1Major = 291,
    DssGraphX1Minor = 292,
    DssGraphXTitle = 293,
    DssGraphX1Zero = 294,
    DssGraphY1Body = 295,
    DssGraphY1Label = 296,
    DssGraphY1Major = 297,
    DssGraphY1Minor = 298,
    DssGraphY1Title = 299,
    DssGraphY1Zero = 300,
    DssGraphY2Body = 301,
    DssGraphY2Label = 302,
    DssGraphY2Major = 303,
    DssGraphY2Minor = 304,
    DssGraphY2Title = 305,
    DssGraphY2Zero = 306,
    DssGraph3DShowColumnHeaders = 318,
    DssGraph3DShowLeftHeaders = 321,
    DssGraph3DShowRightHeaders = 323,
    DssGraph3DShowRowHeaders = 325,
    DssGraphOBPBox = 329,
    DssGraphOBPMedian = 330,
    DssGraphOBPStem = 331,
    DssGraphOBPTail = 332,
    DssGraphPieFeelerLine = 337,
    DssGraphPieFeelerLabel = 338,
    DssGraphPieGroupLabel = 339,
    DssGraphPieRingLabel = 340,
    DssGraphPieSlice = 341,
    DssGraphPieSliceCrust = 342,
    DssGraphPieSliceFace = 343,
    DssGraphPieSliceRing = 344,
    DssGraphPolarAxisCircles = 346,
    DssGraphPolarAxisLabels = 347,
    DssGraphPolarAxisLines = 348,
    DssGraphPolarAxisThick = 349,
    DssGraphBubbleQuadLine = 363,
    DssGraphNestedLabelFont = 364,
    DssGraphRiser2 = 396,
    DssGraphRiser3 = 397,
    DssGraphSeriesTitle = 398,
    DssGraphConnectStackLine = 399,
	DssGraphDataTextStackTotal = 401,
    DssGraphOPILBLValue = 418,
    DssGraphUserLine1 = 414,
    DssGraphUserLine2 = 415,
    DssGraphUserLine3 = 416,
    DssGraphUserLine4 = 417,
    DssGraphUserLine5 = 418,
    DssGraphDataTextConnectLine = 431,
    DssGraphGauge = 444,
    DssGraphGaugeTitle = 450,
    DssGraphGaugeRange = 451,
    DssGraphY3Body = 477,
    DssGraphY3Label = 478,
    DssGraphY3Major = 479,
    DssGraphY3Minor = 480,
    DssGraphY4Body = 483,
    DssGraphY4Label = 484,
    DssGraphY4Major = 485,
    DssGraphY4Minor = 486,
    DssGraph3DColHeader = 512,
    DssGraph3DColHeaderBox = 514,
    DssGraph3DColTitle = 515,
    DssGraph3DColTitleBox = 517,
    DssGraph3DDataText = 518,
    DssGraph3DDataLabelBox = 519,
    DssGraph3DFloor1 = 521,
    DssGraph3DFloor2 = 522,
    DssGraph3DFloor3 = 523,
    DssGraph3DFrameGridX = 524,
    DssGraph3DFrameGridY = 525,
    DssGraph3DFrameGridZ = 526,
    DssGraph3DLastRiserFace = 527,
    DssGraph3DLeftWall1 = 528,
    DssGraph3DLeftWall2 = 529,
    DssGraph3DLeftWall3 = 530,
    DssGraph3DLYHeader = 531,
    DssGraph3DLYHeaderBox = 533,
    DssGraph3DLYTitle = 534,
    DssGraph3DLYTitleBox = 536,
    DssGraph3DRightWall1 = 537,
    DssGraph3DRightWall2 = 538,
    DssGraph3DRightWall3 = 539,
    DssGraph3DRiserFace1 = 540,
    DssGraph3DRiserFace2 = 541,
    DssGraph3DRiserFace3 = 542,
    DssGraph3DRiserFace4 = 543,
    DssGraph3DRiserFace5 = 544,
    DssGraph3DRiserFace6 = 545,
    DssGraph3DRiserFace7 = 546,
    DssGraph3DRiserFace8 = 547,
    DssGraph3DRiserFace9 = 548,
    DssGraph3DRiserGridX = 549,
    DssGraph3DRiserGridY = 550,
    DssGraph3DRiserGridZ = 551,
    DssGraph3DRowHeader = 552,
    DssGraph3DRowHeaderBox = 554,
    DssGraph3DRowTitle = 555,
    DssGraph3DRowTitleBox = 557,
    DssGraph3DRYHeader = 558,
    DssGraph3DRYHeaderBox = 560,
    DssGraph3DRYTitle = 561,
    DssGraph3DRYTitleBox = 563,
    DssGraph3DScatterLine = 564,
    DssGraph3DTieLineX = 565,
    DssGraph3DTieLineY = 566,
    DssGraph3DTieLineZ = 567,
    DssGraph3DRiserVarFace = 568,
    DssGraphGaugeBorder = 1319,
    DssGraphGaugeNeedleBase = 1320,
    DssGraphGaugeNeedleBaseLine = 1330,
    DssGraphGaugeRangeLine = 1331,
    DssGraphAllTexts = 10000,
    DssGraphAllTitles = 10001,
    DssGraphAllAxes = 10002,
    DssGraphZBody = 10003,
    DssGraphTrendLine = 10004,
    DssGraphY1RightBody = 10005,
    DssGraphY1RightTitle = 10006,
    DssGraphRefLine = 10007,
    DssGraphStraightLine = 10008,
    DssGraphTargetLine = 10009,
    DssGraphRefLineText = 10010,
    DssGraphTrendLineText = 10011,
    DssGraphTextAsShape = 10012,
    DssGraphAxisOriginLine
} EnumDSSGraphObject;

typedef enum EnumDSSGraphProperty
{
    DssGraphTextContent = 9999,
    DssGraphContent = 9999,
    DssGraphPropertyReserved = 0,
    DssGraphAutoFitO1 = 4,
    DssGraphAutoFitO2 = 5,
    DssGraphAutoFitX = 6,
    DssGraphAutoFitY1 = 7,
    DssGraphAutoFitY2 = 8,
    DssGraphGroupSpacing = 10,
    DssGraphRiserWidth = 11,
    DssGraphColorBySeries = 12,
    DssGraphCurveColorAsSeries = 13,
    DssGraphSDMOVAFactor = 14,
    DssGraph2DAngle = 18,
    DssGraph2DDepth = 20,
    DssGraphDepthMode = 21,
    DssGraph2ScaleDirectionX = 22,
    DssGraph2ScaleDirectionY1 = 23,
    DssGraph2ScaleDirectionY2 = 24,
    DssGraph2ExcludeZeroX = 25,
    DssGraph2ExcludeZeroY1 = 26,
    DssGraph2ExcludeZeroY2 = 27,
    DssGraphFormatDataTextX = 28,
    DssGraphFormatDataTextY1 = 29,
    DssGraphFormatDataTextY2 = 30,
    DssGraphFormatX = 33,
    DssGraphFormatY1 = 34,
    DssGraphFormatY2 = 35,
    DssGraphFormatZ = 36,
    DssGraphGridLineO1 = 37,
    DssGraphGridLineO2 = 38,
    DssGraphGridLineX = 39,
    DssGraphGridLineY1 = 40,
    DssGraphGridLineY2 = 41,
    DssGraphLabelModeO1 = 48,
    DssGraphLabelModeO2 = 49,
    DssGraphLegendLocation = 52,
    DssGraphTitleXLocation = 54,
    DssGraphTitleY1Location = 55,
    DssGraphTitleY2Location = 56,
    DssGraph2LogScaleX = 57,
    DssGraph2LogScaleY1 = 58,
    DssGraph2LogScaleY2 = 59,
    DssGraphMarkerShape = 60,
    DssGraphMarkerSize = 61,
    DssGraph2DPlaceX = 65,
    DssGraph2DPlaceY1 = 66,
    DssGraph2DPlaceY2 = 67,
    DssGraphConnectLine = 68,
    DssGraphShowStepLine = 70,
    DssGraph2ScaleEndX = 74,
    DssGraph2ScaleEndY1 = 75,
    DssGraph2ScaleEndY2 = 76,
    DssGraph2DScaleFreqO1 = 87,
    DssGraph2DScaleFreqO2 = 88,
    DssGraph2DScaleFreqX = 89,
    DssGraph2DScaleFreqY1 = 90,
    DssGraph2DScaleFreqY2 = 91,
    DssGraphScaleX = 92,
    DssGraphScaleY1 = 93,
    DssGraphScaleY2 = 94,
    DssGraphShowSDDataLineType = 96,
    DssGraphSDRiserType = 97,
    DssGraphShowSDLine = 98,
    DssGraphShowSDCurve = 99,
    DssGraphShowSDLinrCorr = 101,
    DssGraphShowSDLinrExp = 102,
    DssGraphShowSDLinrFormula = 103,
    DssGraphShowSDLinrLine = 104,
    DssGraphShowSDLinrLog = 105,
    DssGraphShowSDLinrNatLog = 106,
    DssGraphShowSDLinrNPoly = 107,
    DssGraphSDPolyFactor = 108,
    DssGraphShowSDMeanLine = 109,
    DssGraphShowSDMOVALine = 110,
    DssGraphShowSDSTDDLine = 111,
    DssGraphShowDataText = 113,
    DssGraphShowDivBiPolar = 114,
    DssGraphShowErrorBar = 115,
    DssGraphShowLegendText = 116,
    DssGraphShow2DO1Label = 117,
    DssGraphShow2DO1 = -117,
    DssGraphShow2DO2 = 118,
 	DssGraphOffScaleTypeX	= 119,
	DssGraphOffScaleTypeY1	= 120,
	DssGraphOffScaleTypeY2	= 121,
	DssGraphShowQuadrants = 122,
    DssGraphShowRiserShadows = 123,
    DssGraphShow2DXTitle = 124,
    DssGraphShow2DY1Title = 125,
    DssGraphShow2DY2Title = 126,
    DssGraphShow2DXLabel = 127,
    DssGraphShow2DX = -127,
    DssGraphShow2DY1Label = 128,
    DssGraphShow2DY1 = -128,
    DssGraphShow2DY2Label = 129,
    DssGraphShow2DY2 = -129,
    DssGraphShowZeroLineX = 130,
    DssGraphShowZeroLineY1 = 131,
    DssGraphShowZeroLineY2 = 132,
    DssGraph2DSideO1 = 133,
    DssGraph2DSideO2 = 134,
    DssGraph2DSideX = 135,
    DssGraph2DSideY1 = 136,
    DssGraph2DSideY2 = 137,
    DssGraph2DSDSplitY = 140,
    DssGraphStaggerO1 = 142,
    DssGraphStaggerO2 = 143,
    DssGraphStaggerX = 144,
    DssGraphStaggerY1 = 145,
    DssGraphStaggerY2 = 146,
    DssGraphUniformBorders = 148,
    DssGraphPieDepthValue = 192,
    DssGraphPieHoleSizeValue = 198,
    DssGraphPieInset = 200,
    DssGraphPiesPerRowValue = 201,
    DssGraphPieRotationValue = 202,
    DssGraphShowFeeler = 204,
    DssGraphShowLBLFeelerLabel = 205,
    DssGraphShowLBLFeeler = -205,
    DssGraphShowLBLPie = 206,
    DssGraphShowLBLRing = 207,
    DssGraphDeleteSlice = 208,
    DssGraphDetachSlice = 209,
    DssGraphPieRestore = 210,
    DssGraphPieTiltValue = 211,
    DssGraphPieTiltOn = 212,
    DssGraphShowPolarCoordinateAxis = 215,
    DssGraphShowPolarConnectLines = 218,
    DssGraphStockDataValueType = 222,
    DssGraphShowBubbleGrid = 245,
    DssGraphSDShowDataText = 247,
    DssGraphShowTitleSeries = 248,
    DssGraphConnectStackBars = 249,
    DssGraphLegendBox = 250,
    DssGraphLinkColors = 260,
    DssGraph3DCustomView = 265,
    DssGraph3DGridFloor = 277,
    DssGraph3DGridLWall = 278,
    DssGraph3DGridRiser = 279,
    DssGraph3DGridRWall = 280,
    DssGraph3DColTitleLocation = 285,
    DssGraph3DLYTitleLocation = 287,
    DssGraph3DRowTitleLocation = 288,
    DssGraph3DRYTitleLocation = 289,
    DssGraph3DScalefreqX = 299,
    DssGraph3DScalefreqY = 300,
    DssGraph3DScalefreqZ = 301,
    DssGraphShow3DScatterLine = 308,
    DssGraphShow3DLineToLWall = 309,
    DssGraphShow3DLineToFloor = 310,
    DssGraphShow3DLineToRWall = 311,
    DssGraphSD3DRiserType = 312,
    DssGraphShow3DShowDataLabelsBox = 313,
 	DssGraph3DOffScaleTypeX	= 314,
	DssGraph3DOffScaleTypeY	= 315,
	DssGraph3DOffScaleTypeZ	= 316,
	DssGraphShow3DCHeader = 318,
    DssGraphShow3DColTitle = 319,
    DssGraph3DShowDataText = 320,
    DssGraphShow3DLNumber = 321,
    DssGraphShow3DLYTitle = 322,
    DssGraphShow3DRNumber = 323,
    DssGraphShow3DRYTitle = 324,
    DssGraphShow3DRHeader = 325,
    DssGraphShow3DRowTitle = 326,
    DssGraphShow3DFloor = 327,
    DssGraphShow3DLegend = 328,
    DssGraphShow3DLWall = 329,
    DssGraphShow3DRWall = 330,
    DssGraph3DViewStruct = 337,
    DssGraphACMColorModel = 549,
    DssGraphLegendItems = 554,
    DssGraphSeriesColor = 556,
    DssGraphReverseGroup = 558,
    DssGraphReverseSeries = 559,
    DssGraphSeriesByColumn = 560,
    DssGraphAreaBackColor = 561,
    DssGraphAreaTransparency = 563,
    DssGraphAreaColor = 564,
    DssGraphAreaPattern = 566,
    DssGraphAreaSFX = 567,
    DssGraphFontAlign = 570,
    DssGraphFontTransparency = 573,
    DssGraphFontColor = 574,
    DssGraphFontName = 576,
    DssGraphFontRotation = 578,
    DssGraphVirtualFontSize = 583,
    DssGraphFontStyle = 584,
    DssGraphType = 585,
    DssGraphLineTransparency = 587,
    DssGraphLineColor = 588,
    DssGraphLinePattern = 589,
    DssGraphVirtualLineWidth = 591,
    DssGraphFootnoteLocation = 592,
    DssGraphFrameLocation = 593,
    DssGraphSubTitleLocation = 594,
    DssGraphTitleLocation = 595,
    DssGraphShowfootnote = 596,
    DssGraphShowSubtitle = 597,
    DssGraphShowTitle = 598,
    DssGraphFontBox = 601,
    DssGraphFontSize = 603,
    DssGraphLockFontSize = 604,
    DssGraphLineWidth = 605,
    DssGraphAreaAlpha = 608,
    DssGraphLineAlpha = 609,
    DssGraphFontAlpha = 610,
    DssGraphAreaCustomBevelEffect = 612,
    DssGraphFormatDataTextAdv = 616,
    DssGraphFormatXAdv = 618,
    DssGraphFormatY1Adv = 619,
    DssGraphFormatY2Adv = 620,
    DssGraphFormatZAdv = 621,
    DssGraphMiddleValue = 630,
    DssGraphSplitYPosition = 631,
    DssGraphAxisShowUserLine = 632,
    DssGraphAxisUserLineValue = 633,
    DssGraphAxisUserLineAxis = 634,
    DssGraphMarkerLayout = 637,
    DssGraphPieDataValueLoc = 638,
    DssGraphPieDataLabelLoc = 639,
    DssGraph2DOrdSpaceMode = 642,
    DssGraphLegendLock = 644,
    DssGraphGridStepY1 = 645,
    DssGraphGridStepY2 = 646,
    DssGraphGridStepX = 647,
    DssGraphDataTextShowConnectLine = 652,
    DssGraphGridLineZ = 658,
    DssGraphScaleZ = 659,
    DssGraphStaggerZ = 660,
    DssGraph2ScaleEndZ = 662,
    DssGraph2ExcludeZeroZ = 663,
    DssGraph2ScaleDirectionZ = 664,
    DssGraphGridStepZ = 665,
    DssGraphGaugeInfo = 668,
    DssGraphGaugeFancyBox = 669,
    DssGraphSDMarkerSize = 671,
    DssGraphSDMarkerShape = 683,
    DssGraphMetric1 = 690,
    DssGraphMetric2 = 691,
    DssGraphStockStyle = 692,
    DssGraph2ScaleDirectionY3 = 693,
    DssGraph2ExcludeZeroY3 = 694,
    DssGraphGridLineY3 = 697,
    DssGraphFormatY3Adv = 699,
    DssGraphGridStepY3 = 701,
	DssGraphOffScaleTypeY3 = 702,
    DssGraphShow2DY3 = 704,
    DssGraph2DSideY3 = 706,
    DssGraphStaggerY3 = 707,
    DssGraph2DPlaceY3 = 709,
    DssGraph2ScaleEndY3 = 710,
    DssGraph2DScaleFreqY3 = 713,
    DssGraphScaleY3 = 714,
    DssGraph2ScaleDirectionY4 = 715,
    DssGraph2ExcludeZeroY4 = 716,
    DssGraphGridLineY4 = 719,
    DssGraphFormatY4Adv = 721,
    DssGraphGridStepY4 = 723,
 	DssGraphOffScaleTypeY4 = 724,
	DssGraphShow2DY4 = 726,
    DssGraph2DSideY4 = 728,
    DssGraphStaggerY4 = 729,
    DssGraph2DPlaceY4 = 731,
    DssGraph2ScaleEndY4 = 732,
    DssGraph2DScaleFreqY4 = 735,
    DssGraphScaleY4 = 736,
    DssGraphSpiderLines = 739,
    DssGraphPolarStartAngle = 740,
    DssGraphMetric3 = 742,
    DssGraphRadarCircularDataLines = 782,
    DssGraphRemoteAxis = 784,
    DssGraphConnectFirstLast = 790,
    DssGraphGaugeShowAxisLabelsOutside = 791,
    DssGraphGaugeShowNeedle = 792,
    DssGraphTailShape = 793,
    DssGraphPieForceRound = 794,
    DssGraphGaugeStyleValue = 795,
    DssGraphGaugeBorderStyleValue = 796,
    DssGraphGaugeBorderThicknessValue = 797,
    DssGraphGaugeRangeThicknessValue = 798,
    DssGraphGaugeRangeStartAngleValue = 799,
    DssGraphGaugeRangeStopAngleValue = 800,
    DssGraphGaugeNeedleStyleValue = 801,
    DssGraphCurvedLines = 804,
    DssGraphAreaBevelEffect = 879,
    DssGraphFontAntiAlias = 881,
    DssGraphFormatDataTextY3 = 695,
    DssGraphFormatDataTextY4 = 717,
    DssGraphBudgetManualMaxBarSize	   = 1002,
	DssGraphBudgetMaxBarSize	= 1003,

    DssGraphColorPalette = 10000,
    DssGraphOriContent = 10001,
    DssGraphFillEffect = 10002,
    DssGraphGradient = 10003,
    DssGraphSDShowToolTip = 10004,
    DssGraphSDToolTip = 10005,
    DssGraphSDRegressionType = 10006,
    DssGraph2LogScale = 10007,
    DssGraphManualMajor = 10008,
    DssGraphManualMinor = 10009,
    DssGraph2ScaleDirection = 10010,
    DssGraphShow2DAxisLine = 10011,
    DssGraph2ExcludeZero = 10012,
    DssGraph2ScaleUseMax = 10013,
    DssGraph2ScaleUseMin = 10014,
    DssGraph2ScaleMax = 10015,
    DssGraph2ScaleMin = 10016,
    DssGraphShow2DAxisLabels = 10017,
    DssGraph2ScaleEndMax = 10018,
    DssGraph2ScaleEndMin = 10019,
    DssGraphShow2DAxisMajor = 10020,
    DssGraphShow2DAxisMinor = 10021,
    DssGraph2DSide = 10022,
    DssGraphStyleMajor = 10023,
    DssGraphStyleMinor = 10024,
    DssGraphCountMajor = 10025,
    DssGraphCountMinor = 10026,
    DssGraphShowTotalValue = 10027,
    DssGraph2DPie = 10028,
    DssGraphShowDataValue = 10029,
    DssGraphShowDVPercent = 10030,
    DssGraphShowDataLabel = 10031,
    DssGraphShowPieLabelOnSide = 10032,
    DssGraphShowMarker = 10033,
    DssGraphShowMetric1 = 10034,
    DssGraphShowMetric2 = 10035,
    DssGraphShowMetric3 = 10036,
    DssGraphMetric1Style = 10037,
    DssGraphMetric2Style = 10038,
    DssGraphMetric3Style = 10039,
    DssGraphShowLine = 10040,
    DssGraphShowToolTip = 10041,
    DssGraphToolTip = 10042,
    DssGraphAxisAutoFit = 10043,
    DssGraphAxisStagger = 10044,
    DssGraphAxisScaleFreq = 10045,
    DssGraphMetric1Value = 10046,
    DssGraphMetric2Value = 10047,
    DssGraphMetric3Value = 10048,
    DssGraph2AxisFormatText = 10049,
    DssGraph2AxisFormatDataText = 10050,
    DssGraphDPI = 10051,
    DssGraphDocSize = 10052,
    DssGraphAutoArrange = 10053,
    DssGraph2DSplitYAutomatic = 10054,
    DssGraphHistogramRangeMin = 10055,
    DssGraphHistogramRangeMax = 10056,
    DssGraphHistogramBuckets = 10057,
    DssGraphGaugeSAngle = 10058,
    DssGraphGaugeEAngle = 10059,
    DssGraphGaugeThickness = 10060,
    DssGraphGauge1Show = 10061,
    DssGraphGauge1Min = 10062,
    DssGraphGauge1Max = 10063,
    DssGraphGauge2Show = 10064,
    DssGraphGauge2Min = 10065,
    DssGraphGauge2Max = 10066,
    DssGraphGauge3Show = 10067,
    DssGraphGauge3Min = 10068,
    DssGraphGauge3Max = 10069,
    DssGraphGauge4Show = 10070,
    DssGraphGauge4Min = 10071,
    DssGraphGauge4Max = 10072,
    DssGraphGauge5Show = 10073,
    DssGraphGauge5Min = 10074,
    DssGraphGauge5Max = 10075,
    DssGraphAxisDataTextPlace = 10076,
    DssGraphAxisNumberFormat = 10077,
    DssGraphAxisDataNumberFormat = 10078,
    DssGraphAxisHasErrorBar = 10079,
    DssGraphAxisHasMiddleValue = 10080,
    DssGraphHeight = 10081,
    DssGraphWidth = 10082,
    DssGraphPicture = 10083,
    DssGraph3DTurnXCounterClockwise = 10084,
    DssGraph3DTurnYCounterClockwise = 10085,
    DssGraph3DTurnZCounterClockwise = 10086,
    DssGraph3DTurnXClockwise = 10087,
    DssGraph3DTurnYClockwise = 10088,
    DssGraph3DTurnZClockwise = 10089,
    DssGraph3DMoveXTowardsOrigin = 10090,
    DssGraph3DMoveYTowardsOrigin = 10091,
    DssGraph3DMoveZTowardsOrigin = 10092,
    DssGraph3DMoveXAwayFromOrigin = 10093,
    DssGraph3DMoveYAwayFromOrigin = 10094,
    DssGraph3DMoveZAwayFromOrigin = 10095,
    DssGraph3DDecreaseCubeX = 10096,
    DssGraph3DDecreaseCubeY = 10097,
    DssGraph3DDecreaseCubeZ = 10098,
    DssGraph3DIncreaseCubeX = 10099,
    DssGraph3DIncreaseCubeY = 10100,
    DssGraph3DIncreaseCubeZ = 10101,
    DssGraph3DDecreaseWallX = 10102,
    DssGraph3DDecreaseWallY = 10103,
    DssGraph3DDecreaseWallZ = 10104,
    DssGraph3DIncreaseWallX = 10105,
    DssGraph3DIncreaseWallY = 10106,
    DssGraph3DIncreaseWallZ = 10107,
    DssGraph3DPanXLeft = 10108,
    DssGraph3DPanYUp = 10109,
    DssGraph3DRelative = 10110,
    DssGraph3DPanXRight = 10111,
    DssGraph3DPanYDown = 10112,
    DssGraph3DAbsolute = 10113,
    DssGraph3DZoomIn = 10114,
    DssGraph3DIncreasePerspectiveDistortion = 10115,
    DssGraph3DStepSize = 10116,
    DssGraph3DZoomOut = 10117,
    DssGraph3DDecreasePerspectiveDistortion = 10118,
    DssGraph3DInitMatrix = 10119,
    DssGraphGaugeQBShow = 10120,
    DssGraphGaugeQBStart = 10121,
    DssGraphGaugeQBStop = 10122,
    DssGraphEnhancedSeriesEffect = 10123,
    DssGraphLocale = 10124,
    DssGraph3DViewStructName = 10125,
    DssGraphLabelsDistance = 10126,
    DssGraphLegendDetails = 10127,
    DssGraphDefaultsInitialized = 10128,
    DssGraphShow3DAxisMajor = 10129,
    DssGraphGradientColorMode = 10130,
    DssGraphLegendDetailsNEW = 10131,
    DssGraphFrameLocationNEW = 10132,
    DssGraph3DColTitleLocationNEW = 10133,
    DssGraph3DRowTitleLocationNEW = 10134,
    DssGraphLabelsDistanceNEW = 10135,
    DssGraphDocSizeNEW = 10136,
    DssGraphUseManualHistogramBuckets = 10137,
    DssGraphFontSize100 = 10138,
	DssGraphOffScaleType = 10139,
	DssGraphUseSpecialSeries	= 10140,
    DssGraphAllowFractionalGridLine = 10141,       //disable the fractional grid lines
    DssGraphIncrementColor		= 10142,
	DssGraphDecrementColor		= 10143,
    DSSGraphShowEdgeDataMarkerInLineChart = 10151,   //In MicroChart widget, we show data marker for end point in line chart.
    DssGraphLineThicknessInMicroChart = 10152,       //In MicroChart widget, set the line thickness for line chart.
    DssGraphCondenseLabel = 10160,
    DSSGraphIsOnlyAdditionalMetrics = 10170,

	DssGraphPieRadius = 20016,
	DssGraphPieSliceQuadrant,
 
	// New features: Interlaced Grid
	DssGraphUseInterlacedGrid = 20027,
	
	// New features: Smart Scale Break
	DssGraphUseSmartBreak,
	DssGraphSmartBreakStyle,
	DssGraphSmartBreakStart,
	DssGraphSmartBreakEnd,
	DssGraphSmartBreakUsePercent,
	
	// New features: Quality Bands
	DssGraphUseQualityBand,
	DssGraphQualityBandUsePercent,
	DssGraphQualityBandPosition,
	DssGraphQualityBandShowTitle,
	
	// New features: Chart Area
	DssGraphUseChartArea,
	DssGraphChartAreaNumber,
	DssGraphChartAreaOrientation,
	DssGraphChartAreaSpacePecentage,
	DssGraphChartAreaDestination,
	DssGraphChartAreaShowTitle,
	
	// New option: Gauge chart
	DssGraphGaugeBorderClipped,
	
	// New feature for Pareto
	DssGraphCumulativePercentLabel,
	
	// 3D options
	DssGraphNew3DTurnXClockwise,
	DssGraphNew3DTurnYClockwise,
	DssGraphNew3DTurnZClockwise,
    DssGraphNew3DMoveXAwayFromOrigin,
    DssGraphNew3DMoveYAwayFromOrigin,
    DssGraphNew3DMoveZAwayFromOrigin,
    DssGraphNew3DIncreaseCubeX,
    DssGraphNew3DIncreaseCubeY,
    DssGraphNew3DIncreaseCubeZ,
    DssGraphNew3DIncreaseWallX,
    DssGraphNew3DIncreaseWallY,
    DssGraphNew3DIncreaseWallZ,
	DssGraphNew3DPanXRight,
	DssGraphNew3DPanYDown,
	DssGraphNew3DZoomIn,
	DssGraphNew3DIncreasePerspectiveDistortion,
	DssGraphNewUse3DMarkers, //Used in 3D Scatter chart.
	
	// Smart label feature for non-Pie/Funnel chart
    DssGraphNewUseSmartLabels,
	
	//iPhone charting start from 21000
	DssGraphIsIPhoneCharting = 21000,
	DssGraphUseGlobalAutoScale = 21001,
	DssGraphRowHeaderIndex,
	DssGraphGridType,
	DssGridGraphDisplayMode,
	DssGridColumnScenario,
	DssGridRowScenario,
	DssGridWidth,
	DssGridMergeCells,
	DssGridMergeColumnCells,
	DssGridLongNames,
	DssGridTemplateShowRowHeaders,
	DssGridTemplateShowColHeaders,
	DssGridShowExtraColumn,
	DssGridLockRowHeaders,
	DssGridLockColumnHeaders,
	DssGridTitleOverlapViewMode,
	DssGridBorderStyleLeft,
	DssGridBorderStyleRight,
	DssGridBorderStyleTop,
	DssGridBorderStyleBottom,
	DssGridFontStyleBold,
	DssMicroChartNegColor,	// Negative color for bar micro chart.
	DssMicroChartUseMetricThresholds, //If metric threshold is usable for microchart.
    
    DssGraphAnimationOrientation, //The orientation of graph animation.
	
	// Support legend text retrieval
	DssGraphLegendItemText,
	
	// Support second user line for Graph Matrix.
	DssGraphAxisShowSecondUserLine,
	DssGraphAxisSecondUserLineValue,
	
	// To customize category label area height in pixels.
	DssGraphCategoryLabelAreaHeight,
	
	// For gradient based on transparency.
	DssGraphGradientEndAlpha,
	
	// For GridChart Support
	DssGraphGridChartDataGroup,
	DssGraphGridChartValueIndex,
	DssGraphGridChartColorIndex,
	DssGraphGridChartIsColorUniform,
	DssGraphGridChartUniformColorMode,
	DssGraphGridChartUniformColors,
	DssGraphGridChartIsAutoFitMode,
	DssGraphGridChartMaxDataVal,
	DssGraphGridChartMinDataVal,
	DssGraphGridChartMaxColorVal,
	DssGraphGridChartMinColorVal,
	DssGraphGridChartLowColor,
	DssGraphGridChartHighColor,
	DssGraphGridChartMarkerShape,
	
	// For Graph Matrix Support
	DssGraphMatrixUndefined,
	DssGraphMatrixSizeMode,			//< autofit or propotional
	DssGraphMatrixSizeIndex,		//< index for size metric
	DssGraphMatrixSizeMaxVal,		//< corresponding value for max size
	DssGraphMatrixSizeMinVal,		//< corresponding value for min size
	DssGraphMatrixColorMode,		//< uniform(by x,y, both) or specific
	DssGraphMatrixColorIndex,		//< index for color metric
	DssGraphMatrixColorMaxVal,		//< corresponding value for max color
	DssGraphMatrixColorMinVal,		//< corresponding value for max color
	DssGraphMatrixUniformColorArity,//< color number when by x or by Y
	DssGraphMatrixLowColor,			//< specific color or uniform when byXY.
	DssGraphMatrixHighColor,		//< specific color
	DssGraphMatrixDataGroup,		//< DataGroup.
	DssGraphMatrixMarkerShape,		//< MarkerShape, for grid/scatter/bubble/standalone point in line chart.
	DssGraphMatrixColorArity,		//< How many colors are involved.
	DssGraphMatrixUniformColors,	//< Uniform colors when color mode is not specific (byx byy breakbyrow, breakbycol).
	DssGraphMatrixScatterRadiusRatio,
	DssGraphMatrixManualMaxRadiusRatio,//< ratio to the max radius
	DssGraphMatrixColorByArity,     //< how many unit are used for colorby.
	DssGraphMatrixColorByUnitIndex, //< Index of units used for color by
	DssGraphMatrixElementMaxSize,		//< Used in size mode = proportional /or max elment size mode = manual, max element size(bar width, line width, etc).
	DssGraphMatrixElementMinSize,		//< Used in size mode = proportional, min element size
	DssGraphMatrixDropZoneUnitNumber,	//< How Many Units are used in one specified drop zone.
	DssGraphMatrixBubbleSizeByNum,		//< How many metric objects are in size by drop zone.
	DssGraphMatrixEstimateGraphHeight,	//< The rough graph height when widget first layout GM
	DssGraphMatrixEstimateGraphWidth,	//< The rough graph width ...
	DssGraphMatrixYAxisLabelWidthLimit,	//< The width limit for Y axis labels.
	DssGraphMatrixXAxisLabelWidthLimit,	//< The width limit for X axis labels.
	DssGraphMatrixAxisLabelUseManualLimit,	 //< Whether to use manual limit for axis labels.
	DssGraphMatrixAutoMaxBarWidth,			 //< The maximum bar width specified in RR spec.
	DssGraphMatrixEnableNumericAxisGridLine, //< Enable grid line for value axis.
	DssGraphMatrixNumericAxisLineColor,		 //< Color for aixs line drawn by chart instance for data area (not for uniform axis).
	DssGraphMatrixAxisLabelSetFontSize,		 //< Font Size for axis labels.
	DssGraphMatrixAxisLabelSetFontName,		 //< Font Name for axis labels.
	DssGraphMatrixAxisLabelAbbreviation,	 //< If axis label would be abbreviated
	DssGraphMatrixShouldXAxisLabelRotate,	 //< If x axis label should be rotated.
	DssGraphMatrixLayoutMode,				 //< How to layout each chart.
	DssGraphMatrixNumericAxisShownTickNumber,//< Number of Ticks shown for specific value axis
	DssGraphMatrixNumericAxisShownTickValue, //< Tick values for the shown ticks of uniform axis.
	DssGraphMatrixUnifomColorInSeries,		 //< Uniform color is used in line seires or not.
	DssGraphMatrixUsePerfectPadding,		 //< Use Perfect Padding or not.
	DssGraphMatrixBubbleBoundaryPadding,	 //< Space between bubble and the chart boundary.
	DssGraphMatrixMaxElementRadius,			 //< Max Element size.
	DssGraphMatrixLineChartSizeInput,		 //< the size input in line chart case, it could be a number or a ratio.
	DssGraphMatrixNumberCondenseMode,		 //< the condense mode across the metrics.
	DssGraphMatrixCondensedMetricIndex,		 //< the index of condensed metric.
	DssGraphMatrixCondensedMetricNumber,	 //< the number of condensed metrics.
	DssGraphMatrixCondenseLimit,             //< the lower bounder for label condense.
	DssGraphMatrixExtraPadding,				 //< the extra required padding.
	DssGraphMatrixAddedMetricTypeToTooltip,	 //< extra metric to be added in tooltip
	DssGraphMatrixMaxElementSizeMode,		 //< size mode for the max element : manual vs automatic
	DssGraphMatrixMinElementSizeMode,		 //< size mode for the min element : manual, proportional vs automaticDss
    DssGraphMatrixFixedTickSize,             //< the fixed size in fixed dimension of tick object.
    DssGraphMatrixUniformAxisHostMetricIndex,//<
    DssGraphMatrixUniformAxisHostMetricNumber,//<
    DssGraphMatrixIsSingleLineSeriesInLineChart,  //< If it is a line chart containing only one line series
    DssGraphMatrixIsValueAxisForPercentChart,     //< If the value axis is used under a percent chart.
    DssGraphMatrixAxisLayout,                     //< Explain how axis is assigned: is there a x-axis? what is its type.
    DssGraphMatrixPieGroupSectionCount,           //< How many sections each group would be broken into for GM pie chart.
    DssGraphMatrixPieAngleUnitCount,              //< How many metric units are there in the angle drop zone.
    DssGraphMatrixPieSliceIncludeMetrics,         //< If metrics is included in slicing drop zone.
    DssGraphMatrixIsPieSliced,                    //< Whether or not the pie is sliced.
    DssGraphMatrixIsSliceHasTemplateMetric,       //< Whether or not the template-metric in the slice dropzone.
    DssGraphMatrixIsColorbyHasTemplateMetric,       //< Whether or not the template-metric in the colorby dropzone.
    DssGraphMatrixIsXHasTemplateMetric,       //< Whether or not the template-metric in the X dropzone.
    DssGraphMatrixIsYHasTemplateMetric,       //< Whether or not the template-metric in the Y dropzone.
    DssGraphMatrixIsBreakByHasTemplateMetric,   //< Whether or not the template-metric in the Break-by dropzone.
    DssGraphMatrixIsRowOrColHasTemplateMetric,       //< Whether or not the template-metric in the Row or Col dropzone.
    DssGraphMatrixIsEmptyXDropzoneWithTemplateMetric, //< Empty X-Dropzone with template-metric.
    DssGraphMatrixIsEmptyYDropzoneWithTemplateMetric, //< Empty Y-Dropzone with template-metric.
    DssGraphMatrixIsEmptyXDropzone,
    DssGraphMatrixIsEmptyYDropzone,
    DssGraphMatrixValueAxisZoomScale,             //< The zoom scale along different value axis, should be associated with different object id.
    DssGraphMatrixHasChartWithOnlyMarkers,    //< In the GM, some chart only has markers : dot line,area,etc.
    DssGraphMatrixIsAutomaticSubShape,
    
    DssGraphMatrixUseFixedMarkerSize,  //< In the GM, special handle for marker in Grid chart.
    
    //New feature Custom min/max for Graph Matrix
    DSSGraphMatrixUseCustomMinMax,   //< Whether or not the custom min/max for graph matrix is enabled.
    
    //For Data Label support
    DssGraphMatrixDataLabelEnabled,    //< Whether or not the Data Label for graph matrix is enabled.
    DSSGraphMatrixDataLabelRotated,
    DssGraphMatrixInitedDataLabelRotation,  //<< Whether should recalculate the data label rotation.
    DSSGraphMatrixAxisAdjusted,
    
    //ColorBy for GM
    DSSGraphMatrixColorByMap,
    DSSGraphMatrixColorByKeyInfo,
    
    //Pie Subtotal
    DSSGraphMatrixPieSubtotalMap,
    
	//For Time Series Support
	DssTimeSeriesYAxisLabelMinSpan,		//< The Mimimual Sapce between Each label on Y axis in TimeSeries
    DssTimeSeriesYAxisExtendHeight,     //< The height of time intervals
	
	//	GraphInfo related properties - started from 30000
	// jas - renumbered to match the EnumDSSGraphHeaderProperties/EnumBasicGraphProperties ordering
	DssGraphInfoReserved = 30000,
	DssGraphInfoMajorType = 30007,
	DssGraphInfoMinorType = 30008,
	DssGraphInfoAutosizeHorizontal = 30011,
	DssGraphInfoAutosizeVertical = 30012,
	DssGraphInfoSeriesByColumn = 30013,
	DssGraphInfoIncludeSubtotal = 30014,
	DssGraphInfoSuppressLastLevel = 30016,
	DssGraphInfoRecalculateChartFrame = 30026,
	DssGraphInfoRecalculateLegend = 30027,
	DssGraphInfoNestedLabel = 30038,
	DssGraphInfoNestedLabelBracket = 30039,
	DssGraphInfoUseMaxCategoriesAsMin = 30048,
	DssGraphInfoUseZerosForGraphNulls = 30049,

} EnumDSSGraphProperty;

typedef enum EnumDSSGraphDataLineType
{
    DssGraphDataLineNone = 0,
    DssGraphDataLineMarker = 1,
    DssGraphDataLineLine = 2,
    DssGraphDataLineBoth = DssGraphDataLineMarker | DssGraphDataLineLine
} EnumDSSGraphDataLineType;

typedef enum EnumDSSGraphMarkerLayout
{
    DssGraphMarkerLeft,
    DssGraphMarkerRight,
    DssGraphMarkerCentered,
    DssGraphMarkerAbove,
    DssGraphMarkerBelow,
    DssGraphMarkerAuto
} EnumDSSGraphMarkerLayout;

typedef enum EnumDSSGraphFontAlignment
{
    DssGraphFontAlignLeft = 0,
    DssGraphFontAlignCenter = 1,
    DssGraphFontAlignRight = 2,
    DssGraphFontAlignJustify = 3
} EnumDSSGraphFontAlignment;

typedef enum EnumDSSGraphFontStyle
{
    DssGraphFontStyleReserved = 0,
    DssGraphFontStyleBold = 1,
    DssGraphFontStyleItalic = 2,
    DssGraphFontStyleUnderline = 4,
    DssGraphFontStyleStrikeout = 8
} EnumDSSGraphFontStyle;

typedef enum EnumDSSGraphFontRotation
{
    DssGraphFontRotateNormal = 0,
    DssGraphFontRotateHotel = 1,
    DssGraphFontRotate90 = 2,
    DssGraphFontRotate180 = 3,
    DssGraphFontRotate270 = 4,
    DssGraphFontRotate45 = 5,
    DssGraphFontRotate315 = 6,
    DssGraphFontRotateCustom = 7
} EnumDSSGraphFontRotation;

typedef enum EnumDSSGraphFontBoxType
{
    DssGraphFontBoxTypeNone = 0,
    DssGraphFontBoxTypeSingleLine = 1,
    DssGraphFontBoxTypeDoubleLine = 2,
    DssGraphFontBoxTypeExtrude = 3,
    DssGraphFontBoxTypeFrameBevel = 4,
    DssGraphFontBoxTypeFrameReverseBevel = 5
} EnumDSSGraphFontBoxType;

typedef enum EnumDSSGraphFillEffect
{
    DssGraphFillSimple = 0,
    DssGraphFillWash = 3,
    DssGraphFillPicture = 4,
    DssGraphFillUserPicture = 6,
    DssGraphFillBrush = 13,
    DssGraphFillAdvancedWash = 14,
    DssGraphFillPattern = 101
} EnumDSSGraphFillEffect;

typedef enum EnumDSSGraphFillBevelEffect
{
    DssGraphFillBevelNone = 0,
    DssGraphFillBevelSmoothEdge = 1,
    DssGraphFillBevelChiselEdge = 2,
    DssGraphFillBevelDonut = 3,
    DssGraphFillBevelSphere = 4
} EnumDSSGraphFillBevelEffect;

typedef enum EnumDSSGraphGradientColoringMode
{
    DssGraphGradientColorModeReserved = 0x0,
    DssGraphGradientColorModeSingle = 0x1,
    DssGraphGradientColorModeDouble = 0x2,
    DssGraphGradientColorModeCustom = 0x4
} EnumDSSGraphGradientColoringMode;

typedef enum EnumDSSGraphPictureFlipType
{
    DssGraphPictureFlipNull = 0,
    DssGraphPictureFlipHorizontal = 1,
    DssGraphPictureFlipVertical = 2,
    DssGraphPictureFlipBoth = 3
} EnumDSSGraphPictureFlipType;

typedef enum EnumDSSGraphPictureScaleType
{
    DssGraphPictureScaleNone = 0,
    DssGraphPictureScale2Fit = 1,
    DssGraphPictureScale2Frame = 2,
    DssGraphPictureScale2Background = 3,
    DssGraphPictureTiled = 4,
    DssGraphPictureTiled2Frame = 5,
    DssGraphPictureTiled2Background = 6,
    DssGraphPictureBrickHorizontal = 7,
    DssGraphPictureBrickVertical = 8,
    DssGraphPictureStack = 9,
    DssGraphPictureScaleProportional = 10
} EnumDSSGraphPictureScaleType;

typedef enum EnumDSSGraphPictureFormat
{
    DssGraphPictureJPEG = 0,
    DssGraphPictureGIF = 1,
    DssGraphPicturePNG = 2,
    DssGraphPictureWMF = 3,
    DssGraphPictureBMP = 4
} EnumDSSGraphPictureFormat;

typedef enum EnumDSSGraphExportFormat
{
    DssGraphExportJPEG = 0,
    DssGraphExportPNG = 1,
    DssGraphExportPCT = 2,
    DssGraphExportPCX = 3,
    DssGraphExportPSD = 4,
    DssGraphExportTGA = 5,
    DssGraphExportTIF = 6,
    DssGraphExportBMP = 7,
    DssGraphExportGIF = 100,
    DssGraphExportCGIF = 101,
    DssGraphExportPDF = 102
} EnumDSSGraphExportFormat;

typedef enum EnumDSSGraphLineStyle
{
    DssGraphLineStyleSolid = 0,
    DssGraphLineStyleDash = 1,
    DssGraphLineStyleDot = 2,
    DssGraphLineStyleDotDash = 3,
    DssGraphLineStyleDashDotDot = 4,
    DssGraphLineStyleMediumDash = 5,
    DssGraphLineStyleShortDash = 6,
    DssGraphLineStyleLongestDash = 7,
    DssGraphLineStyleLongDot = 8,
    DssGraphLineStyleDotDotDot = 9,
    DssGraphLineStyleDashDashDot = 10,
    DssGraphLineStyleDashDashDotDot = 11,
    DssGraphLineStyleLongDashDot = 12,
    DssGraphLineStyleLongDashDotDot = 13,
    DssGraphLineStyleLongDashDashDot = 14,
    DssGraphLineStyleLongDashDashDotDot = 15
} EnumDSSGraphLineStyle;

typedef enum EnumDSSGraphLegendPosition
{
    DssGraphLegendPositionFreeFloat = 0,
    DssGraphLegendPositionRight = 1,
    DssGraphLegendPositionLeft = 2,
    DssGraphLegendPositionBottom = 3
} EnumDSSGraphLegendPosition;

typedef enum EnumDSSGraphDepthMode
{
    DssGraphDepthNone = 0,
    DssGraphDepthFake3D = 1,
    DssGraphDepthTrue3D = 2
} EnumDSSGraphDepthMode;

typedef enum EnumDSSGraphDataTextPosition
{
    DssGraphDTPositionCenter = 0,
    DssGraphDTPositionOutMin = 1,
    DssGraphDTPositionInMin = 2,
    DssGraphDTPositionInMax = 3,
    DssGraphDTPositionOutMax = 4,
    DssGraphDTPositionOnSide = 5,
    DssGraphDTPositionPieSmart = 6,
    DssGraphDTPositionPieSmartRadial = 7,
    DssGraphDTPositionPieSmartStacked = 8
} EnumDSSGraphDataTextPosition;

typedef enum EnumDSSGraphMetricValueStyle
{
    DssGraphMetricVerticalLine = 0,
    DssGraphMetricLine = 1,
    DssGraphMetricArea = 2,
    DssGraphMetricBar = 3,
    DssGraphMetricDot = 4,
    DssGraphMetricArea2 = 5
} EnumDSSGraphMetricValueStyle;

typedef enum EnumDSSGraphDataTextShowType
{
    DssGraphDataTextNone = 0x00,
    DssGraphDataTextValue = 0x01,
    DssGraphDataTextLabel = 0x02,
    DssGraphDataTextBoth = 0x03,
    DssGraphDataTextAbs = 0x04,
    DssGraphDataTextAbsValue = 0x05,
    DssGraphDataTextAbsBoth = 0x07,
    DssGraphDataTextTotal = 0x08,
    DssGraphDataTextZValue = 0x10,
    DssGraphDataTextPercent = 0x20,
    DssGraphDataTextYValue = 0x40,
    DssGraphDataTextInternal = 0x80,
    DssGraphDataTextSeriesDependent = 0x100,
    DssGraphDataTextQualityObjectMode = 0x200
} EnumDSSGraphDataTextShowType;

typedef enum EnumDSSGraphStockDataValueType
{
    DssGraphDataValueHigh = 0,
    DssGraphDataValueLow = 1,
    DssGraphDataValueOpen = 2,
    DssGraphDataValueClose = 3
} EnumDSSGraphStockDataValueType;

typedef enum EnumDSSGraphGridLineType
{
    DssGraphGridNone = 0,
    DssGraphGridRegular = 1,
    DssGraphGridBoth = 2,
    DssGraphGridInnerTicks = 3,
    DssGraphGridOuterTicks = 4,
    DssGraphGridSpanningTicks = 5
} EnumDSSGraphGridLineType;

typedef enum EnumDSSBoxPlotTailShape
{
    DssBoxPlotTailShapeRectangle = 1,
    DssBoxPlotTailShapeTBar = 2,
    DssBoxPlotTailShapeIBar = 3
} EnumDSSBoxPlotTailShape;

typedef enum EnumDSSGraphRegressionType
{
    DssGraphRegressionNone = 0,
    DssGraphRegressionLinear = DssGraphShowSDLinrLine,
    DssGraphRegressionLog = DssGraphShowSDLinrLog,
    DssGraphRegressionPoly = DssGraphShowSDLinrNPoly,
    DssGraphRegressionExp = DssGraphShowSDLinrExp,
    DssGraphRegressionMA = DssGraphShowSDMOVALine,
    DssGraphRegressionNatLog = DssGraphShowSDLinrNatLog,
    DssGraphRegressionSTDD = DssGraphShowSDSTDDLine,
    DssGraphRegressionMean = DssGraphShowSDMeanLine,
	DssGraphRegressionPow = 10000
} EnumDSSGraphRegressionType;

typedef enum EnumDSSGraphRiserEmphasisType
{
    DssGraphRiserEmphasisNone = 0,
    DssGraphRiserEmphasisLineOrLineToBar = 1,
    DssGraphRiserEmphasisAreaOrAreaToBar = 2
} EnumDSSGraphRiserEmphasisType;

typedef enum EnumDSSGraphNumberFormatType
{
    DssGraphNumberFormatDefault = -2,
    DssGraphNumberFormatFixed = 0,
    DssGraphNumberFormatCurrency = 1,
    DssGraphNumberFormatDate = 2,
    DssGraphNumberFormatTime = 3,
    DssGraphNumberFormatPercentage = 4,
    DssGraphNumberFormatFraction = 5,
    DssGraphNumberFormatScientific = 6,
    DssGraphNumberFormatCustom = 7,
    DssGraphNumberFormatSpecial = 8,
    DssGraphNumberFormatGeneral = 9,
    DssGraphNumberFormatFromGrid = 10
} EnumDSSGraphNumberFormatType;

typedef enum EnumDSSGraphFractionNumberFormatType
{
    DssGraphFractionNumberFormatOneDigit = 1,
    DssGraphFractionNumberFormatTwoDigits = 2,
    DssGraphFractionNumberFormatThreeDigits = 3,
    DssGraphFractionNumberFormatHalves = 1000,
    DssGraphFractionNumberFormatQuarters = 1001,
    DssGraphFractionNumberFormatEights = 1002,
    DssGraphFractionNumberFormatTenths = 1003,
    DssGraphFractionNumberFormatHundreths = 1004
} EnumDSSGraphFractionNumberFormatType;

typedef enum EnumDSSGraphNumberFormatCurrencyPosition
{
    DssGraphNumberFormatCurrencyPositionLeft = 0,
    DssGraphNumberFormatCurrencyPositionRight = 1,
    DssGraphNumberFormatCurrencyPositionLeftSpace = 2,
    DssGraphNumberFormatCurrencyPositionRightSpace = 3
} EnumDSSGraphNumberFormatCurrencyPosition;

typedef enum EnumDSSGraphNumberFormatNegativeNumber
{
    DssGraphNumberFormatNegativeNumberDefault = 1,
    DssGraphNumberFormatNegativeNumberRed = 2,
    DssGraphNumberFormatNegativeNumberParenthesis = 3,
    DssGraphNumberFormatNegativeNumberParenthesisRed = 4
} EnumDSSGraphNumberFormatNegativeNumber;

typedef enum EnumDSSGraphColorMode
{
    DssGraphColorModeByFace = 0,
    DssGraphColorModeBySeries = 1,
    DssGraphColorModeByGroup = 2,
    DssGraphColorModeByAngle = 3,
    DssGraphColorModeByHeight = 4,
    DssGraphColorModeByValueX = 5,
    DssGraphColorModeByValueY = 6,
    DssGraphColorModeByValueZ = 7
} EnumDSSGraphColorMode;

typedef enum EnumDSSGraphMarkerShape
{
    DssGraphMarkerShapeNone = 0,
    DssGraphMarkerShapeRectangle = 1,
    DssGraphMarkerShapeStar45 = 2,
    DssGraphMarkerShapePlus = 3,
    DssGraphMarkerShapeCircle = 4,
    DssGraphMarkerShapeDiamond = 5,
    DssGraphMarkerShapeSpikedX = 6,
    DssGraphMarkerShapePlainX = 7,
    DssGraphMarkerShapeTriangle1 = 8,
    DssGraphMarkerShapeStarSkewed = 9,
    DssGraphMarkerShapeFatPlus = 10,
    DssGraphMarkerShapeStar90 = 11,
    DssGraphMarkerShapeSoftX = 12,
    DssGraphMarkerShapePiratePlus = 13,
    DssGraphMarkerShapeFatX = 14,
    DssGraphMarkerShapeCastle = 15,
    DssGraphMarkerShapeTriangle2 = 16,
    DssGraphMarkerShapeTriangle3 = 17,
    DssGraphMarkerShapeTriangle4 = 18,
    DssGraphMarkerShapeTriangle5 = 19,
    DssGraphMarkerShapeTriangle6 = 20,
    DssGraphMarkerShapeTriangle7 = 21,
    DssGraphMarkerShapeTriangle8 = 22,
    DssGraphMarkerShapeEllipse = 23,
    DssGraphMarkerShapeSquare = 24,
    DssGraphMarkerShapeHexagon = 25,
    DssGraphMarkerShapePentagon = 26,
    DssGraphMarkerShapeHouse = 27,
    DssGraphMarkerShapePentagram = 28,
    DssGraphMarkerShapeFontMarker = 29,
    DssGraphMarkerShapeBoxedPlus = 30,
    DssGraphMarkerShapeBoxedX = 31,
    DssGraphMarkerShapeHourGlass = 32,
    DssGraphMarkerShapeHourGlassTransparent = 33,
    DssGraphMarkerShapeVerticalLine = 34,
    DssGraphMarkerShapeHorizontalLine = 35,
    DssGraphMarkerShapeAsterisk = 36,
    DssGraphMarkerShapeStar5 = 37,
    DssGraphMarkerShapeStar6 = 38,
    DssGraphMarkerShapeStar8 = 39,
    DssGraphMarkerShapeBevelBox = 40,
    DssGraphMarkerShapeReverseBevelBox = 41,
    DssGraphMarkerShapeSlimHorizontalLine = 42,
    DssGraphMarkerShapeSlimVerticalLine = 43,
    DssGraphMarkerShapeSlimPlus = 44,
    DssGraphMarkerShapeSlimBoxedPlus = 45,
    DssGraphMarkerShapeSlimX = 46,
    DssGraphMarkerShapeSlimBoxedX = 47,
    DssGraphMarkerShapeRotatedHourGlass = 48,
    DssGraphMarkerShapeSmallBevelBox = 49
} EnumDSSGraphMarkerShape;

typedef enum EnumDSSGraph3DRiserType
{
    DssGraph3DRiserBar = 0,
    DssGraph3DRiserColumnArea = 1,
    DssGraph3DRiserColumnLine = 2,
    DssGraph3DRiserColumnStep = 3,
    DssGraph3DRiserCube = 4,
    DssGraph3DRiserCutCorner = 5,
    DssGraph3DRiserDiamond = 6,
    DssGraph3DRiserHoneyComb = 7,
    DssGraph3DRiserModel = 8,
    DssGraph3DRiserOctagon = 9,
    DssGraph3DRiserPyramid = 10,
    DssGraph3DRiserRowArea = 11,
    DssGraph3DRiserRowLine = 12,
    DssGraph3DRiserRowStep = 13,
    DssGraph3DRiserScatter = 14,
    DssGraph3DRiserSquare = 15,
    DssGraph3DRiserSurface = 16,
    DssGraph3DRiser2DArea = 17,
    DssGraph3DRiser2DBars = 18,
    DssGraph3DRiserBoxPlot = 19,
    DssGraph3DRiserBoxPlotTC = 20,
    DssGraph3DRiser2DBubble = 21,
    DssGraph3DRiserContourPlot = 22,
    DssGraph3DRiserGantt = 23,
    DssGraph3DRiser2DHArea = 24,
    DssGraph3DRiser2DHBars = 25,
    DssGraph3DRiserHBoxPlot = 26,
    DssGraph3DRiserHBoxPlotTC = 27,
    DssGraph3DRiser2DHBubble = 28,
    DssGraph3DRiserHContourPlot = 29,
    DssGraph3DRiser2DHHistogram = 30,
    DssGraph3DRiser2DHLines = 31,
    DssGraph3DRiser2DHScatter = 32,
    DssGraph3DRiserHinge = 33,
    DssGraph3DRiser2DHistogram = 34,
    DssGraph3DRiser2DLines = 35,
    DssGraph3DRiserMultiPies = 36,
    DssGraph3DRiserOpenHighClose = 37,
    DssGraph3DRiserPie = 38,
    DssGraph3DRiserPieBar = 39,
    DssGraph3DRiserPolar = 40,
    DssGraph3DRiserRadar = 41,
    DssGraph3DRiser2DScatter = 42,
    DssGraph3DRiserSpectralMap = 43,
    DssGraph3DRiserTableChart = 44,
    DssGraph3DRiserTextChart = 45,
    DssGraph3DRiserRadarArea = 46,
    DssGraph3DRiserJapaneseStock = 47,
    DssGraph3DRiserStemLeaf = 48,
    DssGraph3DRiserMultiBar = 49,
    DssGraph3DRiserWaterFall = 50,
    DssGraph3DRiserHWaterFall = 51,
    DssGraph3DRiserFacetedFloater = 52,
    DssGraph3DRiserFacetedRiser = 53,
    DssGraph3DRiserBalanceScoreCard = 54,
    DssGraph3DRiserProductPosition = 55,
    DssGraph3DRiserResourceReturn = 56,
    DssGraph3DRiserTimeSeries = 57,
    DssGraph3DRiser4YChart = 58
} EnumDSSGraph3DRiserType;

typedef enum EnumDSSGraphStockLayout
{
    DssGraphStockLayoutDefault = 0,
    DssGraphStockLayoutVerticalLine = 1,
    DssGraphStockLayoutCandleStick = 2,
    DssGraphStockLayoutLine = 3,
    DssGraphStockLayoutArea = 4,
    DssGraphStockLayoutLineCircle = 5,
    DssGraphStockLayoutDot = 6,
    DssGraphStockLayoutArea2 = 7
} EnumDSSGraphStockLayout;

typedef enum EnumDSSGraphGaugeStyle
{
    DssGraphGaugeStyleCircular = 0,
    DssGraphGaugeStyleAlphaNumeric = 1,
    DssGraphGaugeStyleLinear = 2,
    DssGraphGaugeStyleLED = 3,
    DssGraphGaugeStyleMultiLED = 4,
    DssGraphGaugeStyleThermometer = 5,
    DssGraphGaugeStyleCircularNew = 6
} EnumDSSGraphGaugeStyle;

typedef enum EnumDSSGraphGaugeBorderStyle
{
    DssGraphGaugeBorderStyleNone = 0,
    DssGraphGaugeBorderStyleSimple = 1,
    DssGraphGaugeBorderStyle3D = 2,
    DssGraphGaugeBorderStyleEmboss = 3,
    DssGraphGaugeBorderStyleDonut = 4,
    DssGraphGaugeBorderStyleMetallic = 5,
    DssGraphGaugeBorderStyleClipped = 6
} EnumDSSGraphGaugeBorderStyle;

typedef enum EnumDSSGraphGaugeNeedleStyle
{
    DssGraphGaugeNeedleStyleSimple = 0,
    DssGraphGaugeNeedleStyleSimpleLong = 1,
    DssGraphGaugeNeedleStyleSimpleWithBase = 2,
    DssGraphGaugeNeedleStyleSteeple = 3,
    DssGraphGaugeNeedleStyleSteepleLong = 4,
    DssGraphGaugeNeedleStyleRectangular = 5,
    DssGraphGaugeNeedleStyleThin = 6,
    DssGraphGaugeNeedleStyleThinLong = 7,
    DssGraphGaugeNeedleStyleBar = 100,
    DssGraphGaugeNeedleStyleSlider = 101,
    DssGraphGaugeNeedleStyleThermometer = 102,
    DssGraphGaugeNeedleStyleSymbol = 103
} EnumDSSGraphGaugeNeedleStyle;

typedef enum EnumDSSGraphTitleName
{
    DssGraphTitleReserved = 0,
    DssGraphTitleTitle = DssGraphTitle,
    DssGraphTitleSubtitle = DssGraphSubtitle,
    DssGraphTitleFootnote = DssGraphFootnote,
    DssGraphTitleCategory = DssGraphXTitle,
    DssGraphTitleSeries = DssGraphSeriesTitle,
    DssGraphTitleY1 = DssGraphY1Title,
    DssGraphTitleY2 = DssGraphY2Title,
    DssGraphTitleY1Right = DssGraphY1RightTitle
} EnumDSSGraphTitleName;

typedef enum EnumDSSGraphAxisName
{
    DssGraphAxisNameReserved = 0,
    DssGraphAxisCategory = DssGraphO1Body,
    DssGraphAxisSeries = DssGraphO2Body,
    DssGraphAxisX = DssGraphX1Body,
    DssGraphAxisY1 = DssGraphY1Body,
    DssGraphAxisY2 = DssGraphY2Body,
    DssGraphAxisY3 = DssGraphY3Body,
    DssGraphAxisY4 = DssGraphY4Body,
    DssGraphAxisZ = DssGraphZBody,
    DssGraphAxisY1Right = DssGraphY1RightBody
} EnumDSSGraphAxisName;

typedef enum EnumDSSGraphAxisBooleanProperty
{
    DssGraphAxisBooleanReserved = 0,
    DssGraphAxisDrawInReverse = DssGraph2ScaleDirection,
    DssGraphAxisShowAxisLine = DssGraphShow2DAxisLine,
    DssGraphAxisUseLogrithmicScale = DssGraph2LogScale,
    DssGraphAxisAlwaysIncludeZero = DssGraph2ExcludeZero,
    DssGraphAxisUseMinValue = DssGraph2ScaleUseMin,
    DssGraphAxisUseMaxValue = DssGraph2ScaleUseMax,
    DssGraphAxisShowAxisLabels = DssGraphShow2DAxisLabels,
    DssGraphAxisShowMaximumLabel = DssGraph2ScaleEndMax,
    DssGraphAxisShowMinimumLabel = DssGraph2ScaleEndMin,
    DssGraphAxisShowCustomLine = DssGraphAxisShowUserLine,
    DssGraphAxisShowGridLinesMajor = DssGraphShow2DAxisMajor,
    DssGraphAxisUseManualGridMajor = DssGraphManualMajor,
    DssGraphAxisShowGridLinesMinor = DssGraphShow2DAxisMinor,
    DssGraphAxisUseManualGridMinor = DssGraphManualMinor,
    DssGraphAxisAutoFitLabel = DssGraphAxisAutoFit,
    DssGraphAxisStaggerLabel = DssGraphAxisStagger
} EnumDSSGraphAxisBooleanProperty;

typedef enum EnumDSSGraphAxisLongProperty
{
    DssGraphAxisLongReserved = 0,
    DssGraphAxisLabelsLocation = DssGraph2DSide,
    DssGraphAxisGridStyleMajor = DssGraphStyleMajor,
    DssGraphAxisGridStyleMinor = DssGraphStyleMinor,
    DssGraphAxisLabelFreq = DssGraphAxisScaleFreq
} EnumDSSGraphAxisLongProperty;

typedef enum EnumDSSGraphAxisLabelLocation
{
    DssGraphAxisLabelLowSide = 0,
    DssGraphAxisLabelHighSide = 1,
    DssGraphAxisLabelBothSides = 2,
    DssGraphAxisLabelNeitherSide = 3
} EnumDSSGraphAxisLabelLocation;

typedef enum EnumDSSGraphAxisDoubleProperty
{
    DssGraphAxisDoubleReserved = 0,
    DssGraphAxisMinValue = DssGraph2ScaleMin,
    DssGraphAxisMaxValue = DssGraph2ScaleMax,
    DssGraphAxisAxisCustomValue = DssGraphAxisUserLineValue,
    DssGraphAxisGridIntervalMajor = DssGraphCountMajor,
    DssGraphAxisGridIntervalMinor = DssGraphCountMinor
} EnumDSSGraphAxisDoubleProperty;

typedef enum EnumDSSGraphSeriesBooleanProperty
{
    DssGraphSeriesBooleanReserved = 0,
    DssGraphSeriesShowStepLine = DssGraphShowStepLine,
    DssGraphSeriesOnSecondaryAxis = DssGraph2DSDSplitY,
    DssGraphSeriesConnectBubbles = DssGraphShowSDLine,
    DssGraphSeriesShowMarkers = DssGraphShowMarker,
    DssGraphSeriesShowDataText = DssGraphSDShowDataText,
    DssGraphSeriesShowLeaderLines = DssGraphShowFeeler,
    DssGraphSeriesShowEquation = DssGraphShowSDLinrFormula,
    DssGraphSeriesShowCoefficients = DssGraphShowSDLinrCorr,
    DssGraphSeriesShowToolTip = DssGraphSDShowToolTip,
    DssGraphSeriesDeleteSlice = DssGraphDeleteSlice
} EnumDSSGraphSeriesBooleanProperty;

typedef enum EnumDSSGraphSeriesLongProperty
{
    DssGraphSeriesLongReserved = 0,
    DssGraphSeriesRiserType = DssGraphSDRiserType,
    DssGraphSeriesRiserShape = DssGraphSDMarkerShape,
    DssGraphSeriesPieSliceDetach = DssGraphDetachSlice,
    DssGraphSeriesMarkerSize = DssGraphSDMarkerSize,
    DssGraphSeriesMarkerShape = DssGraphSDMarkerShape,
    DssGraphSeriesRegressionType = DssGraphSDRegressionType,
    DssGraphSeriesOrder = DssGraphSDPolyFactor,
    DssGraphSeriesPeriod = DssGraphSDMOVAFactor,
    DssGraphSeries3DRiserType = DssGraphSD3DRiserType,
    DssGraphSeriesPieDataTextType = DssGraphShowLBLFeeler
} EnumDSSGraphSeriesLongProperty;

typedef enum EnumDSSGraphBooleanProperty
{
    DssGraphBooleanReserved = 0,
    DssGraphUseDepth = DssGraphDepthMode,
    DssGraphShowAxisSplit = DssGraphShowDivBiPolar,
    DssGraphShowTotalValues = DssGraphShowTotalValue,
    DssGraphShowErrBar = DssGraphAxisHasErrorBar,
    DssGraphHasMiddleValue = DssGraphAxisHasMiddleValue,
    DssGraphIs2DcircularPie = DssGraph2DPie,
    DssGraphShowDataValues = DssGraphShowDataValue,
    DssGraphShowDataValuesAsPercentage = DssGraphShowDVPercent,
    DssGraphShowDataLabels = DssGraphShowDataLabel,
    DssGraphShowLeaderLines = DssGraphShowFeeler,
    DssGraphShowLabelsOnSide = DssGraphShowPieLabelOnSide,
    DssGraphShowPieName = DssGraphShowLBLPie,
    DssGraphShowPieTotal = DssGraphShowLBLRing,
    DssGraphShowMarkers = DssGraphShowMarker,
    DssGraphShowLines = DssGraphShowLine,
    DssGraphConnectDataPoints = DssGraphShowSDLine,
    DssGraphUseSpiderGridLines = DssGraphSpiderLines,
    DssGraphUseCircularDataLines = DssGraphRadarCircularDataLines,
    DssGraphShowY1AxisOnLeft = DssGraphRemoteAxis,
    DssGraphMetric1Show = DssGraphShowMetric1,
    DssGraphMetric2Show = DssGraphShowMetric2,
    DssGraphMetric3Show = DssGraphShowMetric3,
    DssGraphTieLinesToFloor = DssGraphShow3DLineToFloor,
    DssGraphTieLinesToLeftWall = DssGraphShow3DLineToLWall,
    DssGraphTieLinesToRightWall = DssGraphShow3DLineToRWall,
    DssGraphTieLinesToNeighbor = DssGraphShow3DScatterLine,
    DssGraphPieSizeProportional = DssGraphPieForceRound,
    DssGraph3DShowFloor = DssGraphShow3DFloor,
    DssGraph3DShowRWall = DssGraphShow3DRWall,
    DssGraph3DShowLWall = DssGraphShow3DLWall,
    DssGraphPolarConnectFirstLast = DssGraphConnectFirstLast,
    DssGraphToolTipShow = DssGraphShowToolTip,
    DssGraphUseSeriesColor = DssGraphCurveColorAsSeries,
    DssGraphDualAxisAutomatic = DssGraph2DSplitYAutomatic,
    DssGraphGaugeBand1Show = DssGraphGauge1Show,
    DssGraphGaugeBand2Show = DssGraphGauge2Show,
    DssGraphGaugeBand3Show = DssGraphGauge3Show,
    DssGraphGaugeBand4Show = DssGraphGauge4Show,
    DssGraphGaugeBand5Show = DssGraphGauge5Show,
    DssGraphGaugeShowMarkerNeedle = DssGraphGaugeShowNeedle,
    DssGraphGaugeDrawAxisLabelsOutside = DssGraphGaugeShowAxisLabelsOutside,
    DssGraphLinkWallAndFloorColors = DssGraphLinkColors,
    DssGraphValueAxisCrossesCat = DssGraph2DOrdSpaceMode,
    DssGraphUseUniformBorders = DssGraphUniformBorders,
    DssGraphDataTextShow = DssGraphSDShowDataText,
    DssGraphRestorePie = DssGraphPieRestore,
    DssGraphDisplaySeriesLines = DssGraphConnectStackBars,
    DssGraphDataTextConnectLineShow = DssGraphDataTextShowConnectLine
} EnumDSSGraphBooleanProperty;

typedef enum EnumDSSGraphLongProperty
{
    DssGraphLongReserved = 0,
    DssGraphDepth = DssGraph2DDepth,
    DssGraphDepthDirection = DssGraph2DAngle,
    DssGraphSplitAxisPosition = DssGraphSplitYPosition,
    DssGraphDataValuesType = DssGraphShowDataText,
    DssGraphOverlap = DssGraphGroupSpacing,
    DssGraphGapWidth = DssGraphRiserWidth,
    DssGraphRiserShape = DssGraphSDMarkerShape,
    DssGraphMiddleValueSize = DssGraphMiddleValue,
    DssGraphPieTilt = DssGraphPieTiltValue,
    DssGraphPieDepth = DssGraphPieDepthValue,
    DssGraphPieRotation = DssGraphPieRotationValue,
    DssGraphExplodePie = DssGraphDetachSlice,
    DssGraphPiesPerRow = DssGraphPiesPerRowValue,
    DssGraphDataTextPosition = DssGraphAxisDataTextPlace,
    DssGraphMarkerSize2D = DssGraphSDMarkerSize,
    DssGraphMarkerShape2D = DssGraphSDMarkerShape,
    DssGraphPolarAngle = DssGraphPolarStartAngle,
    DssGraphNumberOfBuckets = DssGraphHistogramBuckets,
    DssGraphStockLayout = DssGraphStockStyle,
    DssGraphMetricStyle1 = DssGraphMetric1Style,
    DssGraphMetricStyle2 = DssGraphMetric2Style,
    DssGraphMetricStyle3 = DssGraphMetric3Style,
    DssGraphBoxPlotTailShape = DssGraphTailShape,
    DssGraphGaugeStartAngle = DssGraphGaugeRangeStartAngleValue,
    DssGraphGaugeEndAngle = DssGraphGaugeRangeStopAngleValue,
    DssGraphGaugeCircularThickness = DssGraphGaugeThickness,
    DssGraphGaugeDrawDataLabels = DssGraphShowDataValue,
    DssGraphFunnelWidth = DssGraphPieDepthValue,
    DssGraphFunnelHeight = DssGraphPieRotationValue,
    DssGraphPieDataTextType = DssGraphShowLBLFeeler,
    DssGraphGaugeBoundingBoxType = DssGraphGaugeFancyBox,
    DssGraphColorMode = DssGraphACMColorModel,
    DssGraphPieHoleSize = DssGraphPieHoleSizeValue,
    DssGraphStockDataType = DssGraphStockDataValueType,
    DssGraphPieDataLabelPosition = DssGraphPieDataLabelLoc,
    DssGraphPieDataValuePosition = DssGraphPieDataValueLoc,
    DssGraphLocaleID = DssGraphLocale,
    DssGraphTurnXCounterClockwise3D = DssGraph3DTurnXCounterClockwise,
    DssGraphTurnYCounterClockwise3D = DssGraph3DTurnYCounterClockwise,
    DssGraphTurnZCounterClockwise3D = DssGraph3DTurnZCounterClockwise,
    DssGraphTurnXClockwise3D = DssGraph3DTurnXClockwise,
    DssGraphTurnYClockwise3D = DssGraph3DTurnYClockwise,
    DssGraphTurnZClockwise3D = DssGraph3DTurnZClockwise,
    DssGraphMoveXTowardsOrigin3D = DssGraph3DMoveXTowardsOrigin,
    DssGraphMoveYTowardsOrigin3D = DssGraph3DMoveYTowardsOrigin,
    DssGraphMoveZTowardsOrigin3D = DssGraph3DMoveZTowardsOrigin,
    DssGraphMoveXAwayFromOrigin3D = DssGraph3DMoveXAwayFromOrigin,
    DssGraphMoveYAwayFromOrigin3D = DssGraph3DMoveYAwayFromOrigin,
    DssGraphMoveZAwayFromOrigin3D = DssGraph3DMoveZAwayFromOrigin,
    DssGraphDecreaseCubeX3D = DssGraph3DDecreaseCubeX,
    DssGraphDecreaseCubeY3D = DssGraph3DDecreaseCubeY,
    DssGraphDecreaseCubeZ3D = DssGraph3DDecreaseCubeZ,
    DssGraphIncreaseCubeX3D = DssGraph3DIncreaseCubeX,
    DssGraphIncreaseCubeY3D = DssGraph3DIncreaseCubeY,
    DssGraphIncreaseCubeZ3D = DssGraph3DIncreaseCubeZ,
    DssGraphDecreaseWallX3D = DssGraph3DDecreaseWallX,
    DssGraphDecreaseWallY3D = DssGraph3DDecreaseWallY,
    DssGraphDecreaseWallZ3D = DssGraph3DDecreaseWallZ,
    DssGraphIncreaseWallX3D = DssGraph3DIncreaseWallX,
    DssGraphIncreaseWallY3D = DssGraph3DIncreaseWallY,
    DssGraphIncreaseWallZ3D = DssGraph3DIncreaseWallZ,
    DssGraphPanXLeft3D = DssGraph3DPanXLeft,
    DssGraphPanYUp3D = DssGraph3DPanYUp,
    DssGraphRelative3D = DssGraph3DRelative,
    DssGraphPanXRight3D = DssGraph3DPanXRight,
    DssGraphPanYDown3D = DssGraph3DPanYDown,
    DssGraphAbsolute3D = DssGraph3DAbsolute,
    DssGraphZoomIn3D = DssGraph3DZoomIn,
    DssGraphIncreasePerspectiveDistortion3D = DssGraph3DIncreasePerspectiveDistortion,
    DssGraphStepSize3D = DssGraph3DStepSize,
    DssGraphZoomOut3D = DssGraph3DZoomOut,
    DssGraphDecreasePerspectiveDistortion3D = DssGraph3DDecreasePerspectiveDistortion,
    DssGraphInitMatrix3D = DssGraph3DInitMatrix,
    DssGraphGaugeStyle = DssGraphGaugeStyleValue,
    DssGraphGaugeBorderStyle = DssGraphGaugeBorderStyleValue,
    DssGraphGaugeBorderThickness = DssGraphGaugeBorderThicknessValue,
    DssGraphGaugeNeedleStyle = DssGraphGaugeNeedleStyleValue
} EnumDSSGraphLongProperty;

typedef enum EnumDSSGraphDoubleProperty
{
    DssGraphDoubleReserved = 0,
    DssGraphHistogramMinRange = DssGraphHistogramRangeMin,
    DssGraphHistogramMaxRange = DssGraphHistogramRangeMax,
    DssGraphGaugeBand1Min = DssGraphGauge1Min,
    DssGraphGaugeBand1Max = DssGraphGauge1Max,
    DssGraphGaugeBand2Min = DssGraphGauge2Min,
    DssGraphGaugeBand2Max = DssGraphGauge2Max,
    DssGraphGaugeBand3Min = DssGraphGauge3Min,
    DssGraphGaugeBand3Max = DssGraphGauge3Max,
    DssGraphGaugeBand4Min = DssGraphGauge4Min,
    DssGraphGaugeBand4Max = DssGraphGauge4Max,
    DssGraphGaugeBand5Min = DssGraphGauge5Min,
    DssGraphGaugeBand5Max = DssGraphGauge5Max
} EnumDSSGraphDoubleProperty;

typedef enum EnumDSSGraphStringProperty
{
    DssGraphStringReserved = 0,
    DssGraphMetric1Label = DssGraphMetric1Value,
    DssGraphMetric2Label = DssGraphMetric2Value,
    DssGraphMetric3Label = DssGraphMetric3Value
} EnumDSSGraphStringProperty;

typedef enum EnumDSSGraphStyleType
{
    DssGraphStyleTypeReserved = 0,
    DssGraphStyleTypeBase = 1,
    DssGraphStyleTypeBaseNoBinary = 2,
    DssGraphStyleTypeDerived = 3
} EnumDSSGraphStyleType;

typedef enum EnumDSSGraphImportFlags
{
    DssGraphImportReserved = 0x00000000,
    DssGraphImportFromOldReport = 0x00000001,
    DssGraphImportPreserveGraphType = 0x00000002,
    DssGraphImportDefault = 0x00000004,
    DssGraphImportForce = 0x00000008,
    DssGraphImportUseVisibility = 0x00000010,
    DssGraphImportUseGraphType = 0x00000020,
    DssGraphImportUseVirtualFontSize = 0x00000040
} EnumDSSGraphImportFlags;

typedef enum EnumDSSGraphType
{
    DssGraphTypeReserved = 0,
    DssGraphTypeVertical_Area_Absolute = 1,
    DssGraphTypeVertical_Area_Stacked = 2,
    DssGraphTypeVertical_Area_BiPolar_Absolute = 3,
    DssGraphTypeVertical_Area_BiPolar_Stacked = 4,
    DssGraphTypeVertical_Area_DualAxis_Absolute = 5,
    DssGraphTypeVertical_Area_DualAxis_Stacked = 6,
    DssGraphTypeVertical_Area_Percent = 7,
    DssGraphTypeHorizontal_Area_Absolute = 8,
    DssGraphTypeHorizontal_Area_Stacked = 9,
    DssGraphTypeHorizontal_Area_BiPolar_Absolute = 10,
    DssGraphTypeHorizontal_Area_BiPolar_Stacked = 11,
    DssGraphTypeHorizontal_Area_DualAxis_Absolute = 12,
    DssGraphTypeHorizontal_Area_DualAxis_Stacked = 13,
    DssGraphTypeHorizontal_Area_Percent = 14,
    DssGraphTypeVertical_Bar_Side_by_Side = 15,
    DssGraphTypeVertical_Bar_Stacked = 16,
    DssGraphTypeVertical_Bar_DualAxis_Side_by_Side = 17,
    DssGraphTypeVertical_Bar_DualAxis_Stacked = 18,
    DssGraphTypeVertical_Bar_BiPolar_Side_by_Side = 19,
    DssGraphTypeVertical_Bar_BiPolar_Stacked = 20,
    DssGraphTypeVertical_Bar_Percent = 21,
    DssGraphTypeHorizontal_Bar_Side_by_Side = 22,
    DssGraphTypeHorizontal_Bar_Stacked = 23,
    DssGraphTypeHorizontal_Bar_DualAxis_Side_by_Side = 24,
    DssGraphTypeHorizontal_Bar_DualAxis_Stacked = 25,
    DssGraphTypeHorizontal_Bar_BiPolar_Side_by_Side = 26,
    DssGraphTypeHorizontal_Bar_BiPolar_Stacked = 27,
    DssGraphTypeHorizontal_Bar_Percent = 28,
    DssGraphTypeVertical_Line_Absolute = 29,
    DssGraphTypeVertical_Line_Stacked = 30,
    DssGraphTypeVertical_Line_BiPolar_Absolute = 31,
    DssGraphTypeVertical_Line_BiPolar_Stacked = 32,
    DssGraphTypeVertical_Line_DualAxis_Absolute = 33,
    DssGraphTypeVertical_Line_DualAxis_Stacked = 34,
    DssGraphTypeVertical_Line_Percent = 35,
    DssGraphTypeHorizontal_Line_Absolute = 36,
    DssGraphTypeHorizontal_Line_Stacked = 37,
    DssGraphTypeHorizontal_Line_BiPolar_Absolute = 38,
    DssGraphTypeHorizontal_Line_BiPolar_Stacked = 39,
    DssGraphTypeHorizontal_Line_DualAxis_Absolute = 40,
    DssGraphTypeHorizontal_Line_DualAxis_Stacked = 41,
    DssGraphTypeHorizontal_Line_Percent = 42,
    DssGraphTypePies = 43,
    DssGraphTypeRing_Pie = 44,
    DssGraphTypeMultiple_Pies = 45,
    DssGraphTypeMultiple_Ring_Pies = 46,
    DssGraphTypeMultiple_Proportional = 47,
    DssGraphTypeMultiple_Proportional_Ring_Pies = 48,
    DssGraphTypePie_Bar = 49,
    DssGraphTypeRing_Pie_Bar = 50,
    DssGraphTypeX_Y_Scatter = 51,
    DssGraphTypeDualAxis_X_Y_Scatter = 52,
    DssGraphTypeX_Y_Scatter_with_Labels = 53,
    DssGraphTypeDualAxis_X_Y_Scatter_with_Labels = 54,
    DssGraphTypePolar_Coordinate_Chart = 55,
    DssGraphTypeDualAxis_Polar_Coordinate_Chart = 56,
    DssGraphTypeRadar_Line = 57,
    DssGraphTypeRadar_Stacked_Area = 58,
    DssGraphTypeDualAxis_Radar_Line = 59,
    DssGraphTypeDualAxis_Radar_Stacked_Area = 60,
    DssGraphTypeBubble = 61,
    DssGraphTypeBubble_DualAxis = 62,
    DssGraphTypeGantt_Chart = 63,
    DssGraphTypeHiLo_Stock_Chart = 64,
    DssGraphTypeHiLo_DualAxis_Stock_Chart = 65,
    DssGraphTypeHiLoOpen_Stock_Chart = 66,
    DssGraphTypeHiLoOpen_DualAxis_Stock_Chart = 67,
    DssGraphTypeHiLoOpenClose_Stock_Chart = 68,
    DssGraphTypeHiLoOpenClose_DualAxis_Stock_Chart = 69,
    DssGraphTypeSpectral_Chart = 70,
    DssGraphTypeVertical_Histogram = 71,
    DssGraphTypeHorizontal_Histogram = 72,
    DssGraphTypeTable_Chart = 73,
    DssGraphTypeD3_Riser_Bars = 74,
    DssGraphTypeD3_Riser_Pyramids = 75,
    DssGraphTypeD3_Riser_Octogans = 76,
    DssGraphTypeD3_Riser_Cut_Corner_Bars = 77,
    DssGraphTypeD3_Floating_Cubes = 78,
    DssGraphTypeD3_Floating_Spheres = 79,
    DssGraphTypeD3_Connect_Group_Areas = 80,
    DssGraphTypeD3_Connect_Group_Ribbons = 81,
    DssGraphTypeD3_Connect_Group_Steps = 82,
    DssGraphTypeD3_Connect_Series_Areas = 83,
    DssGraphTypeD3_Connect_Series_Ribbons = 84,
    DssGraphTypeD3_Connect_Series_Steps = 85,
    DssGraphTypeD3_Surface = 86,
    DssGraphTypeD3_Surface_with_Sides = 87,
    DssGraphTypeD3_Honeycomb_Surface = 88,
    DssGraphTypeD3_X_Y_Z_Scatter_Chart = 89,
    DssGraphTypeD3_X_Y_Z_Scatter_with_Labels_Chart = 90,
    DssGraphTypeVertical_Bar_Absolute = 91,
    DssGraphTypeHorizontal_Bar_Absolute = 92,
    DssGraphTypeBubble_With_Labels = 93,
    DssGraphTypeBubble_DualAxis_With_Labels = 94,
    DssGraphTypeGauge = 95,
    DssGraphTypeFunnel = 96,
    DssGraphTypePareto = 97,
    DssGraphTypePareto_Percent = 98,
    DssGraphTypeBoxPlot = 99,
    DssGraphTypeRadar_Area_Stacked = 100,
    DssGraphTypeRadar_Absolute_Area = 101,
    DssGraphTypeVertical_Bar_BiPolar_Absolute = 102,
    DssGraphTypeVertical_Bar_DualAxis_Absolute = 103,
    DssGraphTypeHorizontal_Bar_BiPolar_Absolute = 104,
    DssGraphTypeHorizontal_Bar_DualAxis_Absolute = 105,
    DssGraphRadar_Line_Stacked = 106,
    DssGraphRadar_DualAxis_Radar_Stacked_Line = 107,
    DssGraphRadar_DualAxis_Radar_Area = 108,
    DssGraphTypeMaxDeskTopGraphTypes = 109,
    DssGraphTypeBarArea = 201,
    DssGraphTypeBarLine = 202,
    DssGraphTypeAreaLine = 203,
    DssGraphTypeDualAxisBarArea = 204,
    DssGraphTypeDualAxisBarLine = 205,
    DssGraphTypeDualAxisAreaLine = 206,
    DssGraphTypeVerticalBudgetLipStick = 207,
    DssGraphTypeHorizontalBudgetLipStick = 208,
    DssGraphTypeVerticalBudgetOverlappingBars = 209,
    DssGraphTypeHorizontalBudgetOverlappingBars = 210,

    //pie
	DssGraphTypeRingPie = 20015,

	DssGraphTypeHorizontal_CategoryPies = 20018,
    DssGraphTypeVertical_CategoryPies,
    DssGraphTypeHorizontal_CategoryPies_Absolute = 20018,
    DssGraphTypeHorizontal_CategoryPies_Stacked,
    DssGraphTypeHorizontal_CategoryPies_Percent,
    DssGraphTypeVertical_CategoryPies_Absolute,
    DssGraphTypeVertical_CategoryPies_Stacked,
    DssGraphTypeVertical_CategoryPies_Percent,
    DssGraphTypeScatter_Pies,
    DssGraphTypeBubble_Pies,
    DssGraphTypeGrid_Pies,

} EnumDSSGraphType;

typedef enum EnumDSSRWLinkType
{
	DssRWLinkReserved	= 0,
	DssRWLinkHyperLink,
	DssRWLinkImage,
	DssRWLinkToolTip,
	DssRWLinkURL,
	DssRWLinkHTML,
	DssRWLinkResolvedHyperLinks,
	
	// Any new RW link type should be added before the following one
	DssRWLinkLastOne
} EnumDSSRWLinkType;

// Rough classification of how likely it is that a user will want to follow a drill path
typedef enum EnumDSSDrillImportance
{
	DssDrillImportanceReserved	= 0,
	DssDrillImportanceHigh,
	DssDrillImportanceMedium,
	DssDrillImportanceLow,
	
	DssDrillImportanceAll		= 0x03,
	
	DssDrillAddWithin			= 0x04,	// this can be ORed with high/medium/low.
	DssDrillForceWithin			= 0x08,	// this can be ORed with high/medium/low
	DssDrillDetectPathOnly		= 0x10,	// indicates that the caller's intention is only to see if there's at least one drill path
	DssDrillExcludeWithin		= 0x20 // exclude all within paths, can be ORed with /high/medium/low
} EnumDSSDrillImportance;

// Broad classification of how a drill action transforms a template
typedef enum EnumDSSDrillType
{
	DssDrillReserved = 0,
	DssDrillToParent,
	DssDrillToChild,
	DssDrillToUnit,
	DssDrillToTemplate,
	DssDrillToForm,
	DssDrillToDetails,
	DssDrillToMetric,
	DssDrillFixTemplate,
	DssDrillRemoveMetric,
	DssDrillRemoveUnit,
	DssDrillToGraphCoordinates
} EnumDSSDrillType;

typedef enum EnumBinaryContextFlags
{
	DssBinaryContextFlexDefn	= 0x01,
	DssBinaryContextFlexData	= 0x02,
	DssBinaryContextFlexDrill	= 0x04,
	DssBinaryContextFlexImage   = 0x08,
	DssBinaryContextFlexWidget   = 0x0800, // D3 Widget Precache
	DssBinaryContextFlexOnDemand = 0x01000000,
	DssBinaryContextFlexOnDemandDataOnly = 0x02000000,
    DssBinaryContextFlexSeparableBinary = 0x04000000,
    DssBinaryContextFlexDataSetsBinary = 0x08000000,
} EnumBinaryContextFlags;

typedef enum EnumBinaryContextDetailFlags
{
	DssBinaryContextDetailCurrentLayout	= 0x01,
	DssBinaryContextDetailAnnotation	= 0x02,
	DssBinaryContextDetailVisFramework	= 0x04,
	DssBinaryContextDetailCurrentGroupBy	= 0x08,
	DssBinaryContextDetailPartialRetrieval	= 0x10,
	DssBinaryContextDetailSlice			= 0x20,	// For binary data: export the user specified slice using extended parameter
	DssBinaryContextDetailPPE			= 0x40,		// For bianry data: slices the first group by and keep no slicing for the other section.
	DssBinaryContextDetailResolvedFormats	= 0x80,		// For binary defn: Pregenerate aggregated formats for template
	DssBinaryContextDetailDrillHigh		= 0x100,
	DssBinaryContextDetailDrillMedium	= 0x200,
	DssBinaryContextDetailDrillLow		= 0x400,
	DssBinaryContextGraphUseVirtualFonts= 0x800,
	DssBinaryContextDetailSpecifiedNodeOnly	= 0x1000,	// For partial definition retreival to include specified node only, not the whole path which is hte default behavior
	DssBinaryContextDetailInitialSlice = 0x8000,
	DssBinaryContextDetailInitialLayout = 0x10000,
	DssBinaryContextDetailSpecificLayout = 0x20000,
    DssBinaryContextDetailUnitObjectFormats = 0x40000,
    DssBinaryContextDetailExpressionEx = 0x80000,
	DssBinaryContextGraphUseSpecialseries = 0x100000,
	DssBinaryContextDetailDataSliceOptimization = 0x200000,// For Binary data: new binary data format to optimize binary data loading on client side
    DssBinaryContextDetailNonResolvedFormats = 0x400000,
    DssBinaryContextDetailCompressed = 0x800000,
    DssBinaryContextDetailGlobalLookupTableHeader   = 0x1000000,
    DssBinaryContextDetailCompressedForSlowNetwork = 0x2000000,
    DssBinaryContextDetailAllLayout = 0x4000000,
    DssBinaryContextDetailCSIOnly = 0x8000000,
    DssBinaryConTextDetialAllBinaryParts = 0x10000000,
    DssBinaryContextDetailOfflineUC = 0x20000000//2^29. Setted on node model after offline UC selection.
} EnumBinaryContextDetailFlags;

typedef enum
{
	BGPReserved = 0,
	BGPIsChartAvailable		= 1,
	BGPGraphTypesAvailable	= 2,
	BGPStartRow		= 3,
	BGPStartCol		= 4,
	BGPMaxRow		= 5,	
	BGPMaxCol		= 6,
	BGPMajorType	= 7,
	BGPMinorType	= 8,
	BGPHeight		= 9,
	BGPWidth		= 10,
	BGPAutosizeHorizontal	= 11,
	BGPAutosizeVertical		= 12,
	BGPSeriesByCol			= 13,
	BGPIncludeSubtotal		= 14,
	BGPImageType	= 15,
	BGPSuppressLastLevel	= 16,
	BGPTotalRow		= 17,
	BGPTotalCol		= 18,
	BGPErrorNumber	= 19,
	BGPUseZerosForGraphNulls = 20,
	BGPRecalculateChartFrame = 26,
	BGPRecalculateLegend = 27,
	BGPUseNestedLabels = 38,
	BGPUseNestedLabelsBracket = 39,
	BGPUseMaxCategoriesAsMin = 48,
} EnumBasicGraphProperties;		

typedef enum
{
	DssXMLRWSelectorUnsetStatusReserved = 0,
	DssXMLRWSelectorUnsetStatusDefault = 1,
	DssXMLRWSelectorUnsetStatusFirst = 2,
	DssXMLRWSelectorUnsetStatusLast = 4
} EnumDSSXMLRWSelectorUnsetStatus;

typedef enum
{
	DssUnitTransactionReserved    = 0,
    DssUnitTransactionEditable    = 1,
	DssUnitTransactionRequired = 0x10000000
} EnumUnitTransactionFlags;

typedef enum
{
	DssAccessEntryTypeReserved = 0,
	DssAccessEntryTypeObject = 1,
	DssAccessEntryTypeAudit = 2,
} EnumDSSAccessEntryType;

typedef enum
{
	DssAccessRightBrowseFlag		= 0x00000001,
	DssAccessRightUseExecuteFlag	= 0x00000002,
	DssAccessRightReadFlag			= 0x00000004,
	DssAccessRightWriteFlag			= 0x00000008,
	DssAccessRightDeleteFlag		= 0x00000010,
	DssAccessRightControlFlag		= 0x00000020,
	DssAccessRightUseFlag			= 0x00000040,
	DssAccessRightExecuteFlag		= 0x00000080,
	DssAccessRightFullControlFlag	= 0x000000ff,
	DssAccessRightInheritable = 0x20000000,
	DssAccessRightAudit = 0x40000000,
	DssAccessRightAuditInheritable = 0x60000000,
}EnumDSSAccessRightFlags;


typedef enum
{
    DssExpressionReserved = 0,
	DssExpressionGeneric= 1,
	DssFilterSingleBaseFormQual= 2,
	DssFilterMultiBaseFormQual= 3,
	DssFilterJointFormQual= 4,
	DssFilterListQual= 5,
	DssFilterListFormQual= 6,
	DssFilterJointListQual= 7,  
	DssFilterJointListFormQual= 8,
	DssFilterSingleBaseFormExpression = 9,
	DssFilterSingleMetricQual = 10,
	DssFilterMultiMetricQual = 11,
	DssFilterMetricExpression =12,
	DssFilterEmbedQual = 13,
	DssFilterBranchQual = 14,
	DssFilterRelationshipQual = 15,
	DssFilterAllAttributeQual = 16,
	DssFilterAttributeIDQual = 17,
	DssFilterAttributeDESCQual = 18,
	DssExpressionAggMetric = 19,
	DssExpressionBanding = 20,          // Banding expression
	DssFilterReportQual = 21,           // Report as filter
	DssExpressionMDXSAPVariable = 22,   // SAP Variable answer expression
	DssExpressionSQLQueryQual = 23,     // Qualification in SQLQuery   
	DssExpressionCanceledPrompt = 24,  // Expression holding a canceled prompt
	DssExpressionElementList = 25,      // Derived Element: an ordered list of elements like [2000:int; 2003:int; 2001:int; 2002]
	DssExpressionElementSingle = 26,    // Derived Element: a single element like [2000] or [2000:int; NY]
}EnumDSSExpressionType;

typedef enum
{
    DssFunctionCustom                = -1, // 2/22/2012 tehe
    DssFunctionReserved              = 0,
    DssFunctionPlus					 = 1,
    DssFunctionMinus				 = 2,
    DssFunctionTimes				 = 3,
    DssFunctionDivide				 = 4,
    DssFunctionUnaryMinus			 = 5,
    DssFunctionEquals				 = 6,
    DssFunctionNotEqual				 = 7,
    DssFunctionGreater				 = 8,
    DssFunctionLess					 = 9,
    DssFunctionGreaterEqual			 = 10,
    DssFunctionLessEqual			 = 11,
    DssFunctionSum					 = 12,
    DssFunctionCount				 = 13,
    DssFunctionAvg					 = 14,
    DssFunctionMin					 = 15,
    DssFunctionMax					 = 16,
    DssFunctionBetween				 = 17,
    DssFunctionLike					 = 18,
    DssFunctionAnd					 = 19,
    DssFunctionOr					 = 20,
    DssFunctionNot					 = 21,
    DssFunctionIn					 = 22,
    DssFunctionRank					 = 23,
    DssFunctionAbs					 = 24,
    DssFunctionRunningSum			 = 25,
    DssFunctionRunningAvg			 = 26,
    DssFunctionMovingSum			 = 28,
    DssFunctionMovingAvg			 = 27,
    DssFunctionProduct				 = 29,
    DssFunctionMedian				 = 30,
    DssFunctionMode					 = 31,
    DssFunctionStdev				 = 32,
    DssFunctionVar					 = 33,
    DssFunctionGeomean				 = 34,
    DssFunctionEqualEnhanced		 = 35,
    DssFunctionNotEqualEnhanced		 = 36,
    DssFunctionGreaterEqualEnhanced	 = 37,
    DssFunctionLessEqualEnhanced	 = 38,
    DssFunctionBetweenEnhanced		 = 39,
    DssFunctionBanding				 = 40,
    DssFunctionBandingC				 = 41,
    DssFunctionBandingP				 = 42,
    DssFunctionNotLike				 = 43,
    DssFunctionNotBetween			 = 44,
    DssFunctionIntersect			 = 45,
    DssFunctionIntersectIn			 = 46,
    DssFunctionNullToZero			 = 47,
    DssFunctionZeroToNull			 = 48,
    DssFunctionApplySimple			 = 49,
    DssFunctionApplyAggregation		 = 50,
    DssFunctionApplyLogic			 = 51,
    DssFunctionApplyComparison		 = 52,
    DssFunctionApplyRelative		 = 53,
    DssFunctionIsNull				 = 54,
    DssFunctionIsNotNull			 = 55,
    DssFunctionUcase				 = 56,
    DssFunctionNotIn				 = 57,
    DssFunctionNTile				 = 58,
    DssFunctionPercentile			 = 59,
    DssFunctionMovingMax			 = 60,
    DssFunctionMovingMin			 = 61,
    DssFunctionMovingDifference		 = 62,
    DssFunctionMovingStdev			 = 63,
    DssFunctionExpWghMovingAvg		 = 64,
    DssFunctionMovingCount			 = 65,
    DssFunctionRunningMax			 = 66,
    DssFunctionRunningMin			 = 67,
    DssFunctionRunningStdev			 = 68,
    DssFunctionRunningCount			 = 69,
    DssFunctionExpWghRunningAvg		 = 70,
    DssFunctionNotBetweenEnhanced	 = 71,
    DssFunctionConcat				 = 72,
    DssFunctionFirstInRange			 = 73,
    DssFunctionLastInRange			 = 74,
    DssFunctionValueSegment			 = 75,
    DssFunctionContains				 = 76,
    DssFunctionBeginsWith			 = 77,
    DssFunctionEndsWith				 = 78,
    DssFunctionNotContains			 = 79,
    DssFunctionNotBeginsWith		 = 80,
    DssFunctionNotEndsWith			 = 81,
    DssFunctionCase					 = 82,
    DssFunctionCaseV				 = 83,
    DssFunctionStdevP				 = 84,
    DssFunctionRunningStdevP		 = 85,
    DssFunctionMovingStdevP			 = 86,
    DssFunctionNTileS				 = 87,
    DssFunctionNTileVS				 = 88,
    DssFunctionVarP					 = 89,
    DssFunctionCurrentDate			 = 90,
    DssFunctionDayOfMonth			 = 91,
    DssFunctionDayOfWeek			 = 92,
    DssFunctionDayOfYear			 = 93,
    DssFunctionWeek					 = 94,
    DssFunctionMonth				 = 95,
    DssFunctionQuarter				 = 96,
    DssFunctionYear					 = 97,
    DssFunctionCurrentDateTime		 = 98,
    DssFunctionCurrentTime			 = 99,
    DssFunctionHour					 = 100,
    DssFunctionMinute				 = 101,
    DssFunctionSecond				 = 102,
    DssFunctionMilliSecond			 = 103,
    DssFunctionConcatNoBlank		 = 104,
    DssFunctionLength				 = 105,
    DssFunctionLower				 = 106,
    DssFunctionLTrim				 = 107,
    DssFunctionPosition				 = 108,
    DssFunctionRTrim				 = 109,
    DssFunctionSubStr				 = 110,
    DssFunctionInitCap				 = 111,
    DssFunctionTrim					 = 112,
    DssFunctionRightStr				 = 113,
    DssFunctionLeftStr				 = 114,
    DssFunctionGreatest				 = 115,
    DssFunctionLeast				 = 116,
    DssFunctionFirst				 = 117,
    DssFunctionLast					 = 118,
    DssFunctionDate					 = 119,
    DssFunctionDaysBetween			 = 120,
    DssFunctionMonthsBetween		 = 121,
    DssFunctionAddDays				 = 122,
    DssFunctionAddMonths			 = 123,
    DssFunctionMonthStartDate		 = 124,
    DssFunctionMonthEndDate			 = 125,
    DssFunctionYearStartDate		 = 126,
    DssFunctionYearEndDate			 = 127,    
    DssFunctionIF					 = 128,
    DssFunctionApplyOptional		 = 129,
    DssFunctionApplyCSSecurityFilter  = 130,
    DssFunctionUnion				 = 131,
    DssFunctionExcept				 = 132,
    DssFunctionCoalesce				 = 133,
    DssFunctionAdd					 = 134,
    DssFunctionAverage				 = 135,
    DssFunctionMultiply				 = 136,
    DssFunctionBandingM				 = 137,  // lcui 7/18/2006 TQMS222462 Banding on metric value
    DssFunctionOLAPSum				 = 138,  // qxue, 07/12/2006, new olap functions
    DssFunctionOLAPAvg				 = 139,  // qxue, 07/12/2006, new olap functions
    DssFunctionOLAPCount			 = 140,  // qxue, 07/12/2006, new olap functions
    DssFunctionOLAPMax				 = 141,  // qxue, 07/12/2006, new olap functions
    DssFunctionOLAPMin				 = 142,  // qxue, 07/12/2006, new olap functions
    DssFunctionLag					 = 143,  // qxue, 07/12/2006, new olap functions
    DssFunctionLead					 = 144,  // qxue, 07/12/2006, new olap functions
    DssFunctionOLAPRank				 = 145,  // gliang, 10/03/2008, new olap functions
    DssFunctionFirstTrue             = 146,   // jimzhang July/27/2011
    DssFunctionLcase                 = 147,          //zhyang, 27/07/2011
    DssFunctionConcatAgg             = 177,
    DssFunctionTuple                 = 1000
   
} EnumDSSFunction;

typedef enum EnumSimpleOperatorType
{
    DssFuncUnknown = 0,
    DssFuncPlus = 1,
    DssFuncMinus = 2,
    DssFuncTimes = 3,
    DssFuncDivide = 4,
    DssFuncUnaryMinus = 5,
    DssFuncEquals = 6,
    DssFuncNotEqual = 7,
    DssFuncGreater = 8,
    DssFuncLess = 9,
    DssFuncGreaterEqual = 10,
    DssFuncLessEqual = 11,
    DssFuncSum = 12,
    DssFuncCount = 13,
    DssFuncAvg = 14,
    DssFuncMin = 15,
    DssFuncMax = 16,
    DssFuncBetween = 17,
    DssFuncLike = 18,
    DssFuncAnd = 19,
    DssFuncOr = 20,
    DssFuncNot = 21,
    DssFuncIn = 22,
    DssFuncRank = 23,
    DssFuncAbs = 24,
    DssFuncRunningSum = 25,
    DssFuncRunningAvg = 26,
    DssFuncMovingAvg = 27,
    DssFuncMovingSum = 28,
    DssFuncProduct = 29,
    DssFuncMedian = 30,
    DssFuncMode = 31,
    DssFuncStdev = 32,
    DssFuncVar = 33,
    DssFuncGeomean = 34,
    DssFuncEqualEnhanced = 35,
    DssFuncNotEqualEnhanced = 36,
    DssFuncGreaterEqualEnhanced = 37,
    DssFuncLessEqualEnhanced = 38,
    DssFuncBetweenEnhanced = 39,
    DssFuncBanding = 40,
    DssFuncBandingC = 41,
    DssFuncBandingP = 42,
    DssFuncNotLike = 43,
    DssFuncNotBetween = 44,
    DssFuncIntersect = 45,
    DssFuncIntersectIn = 46,
    DssFuncNullToZero = 47,
    DssFuncZeroToNull = 48,
    DssFuncApplySimple = 49,
    DssFuncApplyAggregation = 50,
    DssFuncApplyLogic = 51,
    DssFuncApplyComparison = 52,
    DssFuncApplyRelative = 53,
    DssFuncIsNull = 54,
    DssFuncIsNotNull = 55,
    DssFuncN_Tile = 56,
    DssFuncPercentile = 57,
    DssFuncMovingMax = 58,
    DssFuncMovingMin = 59,
    DssFuncMovingDifference = 60,
    DssFuncMovingStdev = 61,
    DssFuncExpWghMovingAvg = 62,
    DssFuncMovingCount = 63,
    DssFuncRunningMax = 64,
    DssFuncRunningMin = 65,
    DssFuncRunningStdev = 66,
    DssFuncRunningCount = 67,
    DssFuncExpWghRunningAvg = 68,
    DssFuncNotIn = 69,
    DssFuncNotBetweenEnhanced = 70,
    DssFuncUCase = 71,
    DssFuncConcat = 72,
    DssFuncFirstInRange = 73,
    DssFuncLastInRange = 74,
    DssFuncNTile_V = 75,
    DssFuncStdevP = 76,
    DssFuncRunningStdevP = 77,
    DssFuncMovingStdevP = 78,
    DssFuncNTile_S = 79,
    DssFuncVarP = 80,
    DssFuncNTile_VS = 81,
    DssFuncCase = 82,
    DssFuncCaseV = 83,
    DssFuncBeginsWith = 84,
    DssFuncContains = 85,
    DssFuncEndsWith = 86,
    DssFuncNotBeginsWith = 87,
    DssFuncNotContains = 88,
    DssFuncNotEndsWith = 89,
    DssFuncFirstTrue = 90,
    DssFuncMax2 = 91,
    DssFuncMin2 = 92,
    DssFuncFirst = 93,
    DssFuncLast = 94,
    DssFuncIF = 95,
    DssFuncCurrentDate = 100,
    DssFuncDayOfMonth = 101,
    DssFuncDayOfWeek = 102,
    DssFuncDayOfYear = 103,
    DssFuncWeek = 104,
    DssFuncMonth = 105,
    DssFuncQuarter = 106,
    DssFuncYear = 107,
    DssFuncCurrentDateTime = 108,
    DssFuncCurrentTime = 109,
    DssFuncHour = 110,
    DssFuncMinute = 111,
    DssFuncSecond = 112,
    DssFuncMilliSecond = 113,    
    DssFuncConcatNoBlank = 114,  // 7/4/2012 tehe: string calculation
    DssFuncLength = 115,         // 7/4/2012 tehe: string calculation
    DssFuncLCase = 116,
    DssFuncLTrim = 117,         // 7/4/2012 tehe: string calculation
    DssFuncPosition = 118,      // 7/4/2012 tehe: string calculation
    DssFuncRTrim = 119,         // 7/4/2012 tehe: string calculation
    DssFuncSubStr = 120,        // 7/4/2012 tehe: string calculation
    DssFuncInitCap = 121,       // 7/4/2012 tehe: string calculation
    DssFuncTrim = 122,          // 7/4/2012 tehe: string calculation
    DssFuncRightStr = 123,      // 7/4/2012 tehe: string calculation
    DssFuncLeftStr = 124,       // 7/4/2012 tehe: string calculation
    DssFuncDate = 125,
    DssFuncDaysBetween = 126,
    DssFuncMonthsBetween = 127,
    DssFuncAddDays = 128,
    DssFuncAddMonths = 129,
    DssFuncMonthStartDate = 130,
    DssFuncMonthEndDate = 131,
    DssFuncYearStartDate = 132,
    DssFuncYearEndDate = 133,
    DssFuncApplyOptional = 134,
    DssFuncApplyCSSecurityFilter = 135,
    DssFuncAdd = 139,
    DssFuncAverage = 140,
    DssFuncMultiply = 141,
    DssFuncBandingM = 142,
    DssFuncOLAPSum = 143,
    DssFuncOLAPAvg = 144,
    DssFuncOLAPCount = 145,
    DssFuncOLAPMax = 146,
    DssFuncOLAPMin = 147,
    DssFuncLag = 148,
    DssFuncLead = 149,
    DssFuncOLAPRank = 150,
    DssFuncRepeat = 151,
    
    DssFuncPercentRankRelative=162,// 6/20/2014 xzhu: funancial function
    DssFuncPercentRank=171,
    DssFuncWeightStdevP=180,
    DssFuncWeightMeanAve=181,
    DssFuncConcatAgg=182,
    DssFuncWeightCov=183,
    DssFuncWeightCorr=184,
    
    // jimzhang, Nov/13/2012, this is hard coding to support Mod(). On server side, Mod() is in StdMathSTOS package, and its function index is 33. But at current, only function type (CDSSFunction::mFunctionType on server side) is serialized to client, and function type of Mod() is -1. So, to support Mod() on client, treat Mod() as a built-in function and hard code its function type to a valid value here.
    DssFunctionMathMod   = 1033
} EnumSimpleOperatorType;


/*const char* FunctionString[] =	{
 "",  // Reserved
 "+", // Plus
 "-", // Minus
 "*", // Times
 "/", // Divide
 "-", // UnaryMinus ??
 "=", // Equals
 "!=", // Not Equals
 ">", // Greater
 "<", // Less
 ">=", // GreaterEqual
 "<=", // LessEqual
 "SUM", // Sum
 "COUNT", // Count
 "AVG", // Average
 "MIN", // Minimun
 "MAX", // Maximum
 "BETWEEN", // Between
 "LIKE", // Like
 "AND", // And
 "OR", // Or
 "NOT", // Not
 "IN", // In
 "RANK", // Rank
 "ABS" // Abs
 };*/

typedef enum
{
    DssPrivilegesReserved = 0,
    DssPrivilegesCreateAppObj = 1,
    DssPrivilegesCreateConfigObj = 2,
    DssPrivilegesCreateSchemaObj = 3,
    DssPrivilegesScheduleRequest = 4,
    DssPrivilegesUseArchitect = 5,
    DssPrivilegesWebReportDetails = 6,
    DssPrivilegesUseServerAdmin = 7,
    DssPrivilegesUseMetricEditor = 8,
    DssPrivilegesUseFilterEditor = 9,
    DssPrivilegesUseTemplateEditor = 10,
    DssPrivilegesUseReportEditor = 11,
    DssPrivilegesUseConsolidationEditor = 12,
    DssPrivilegesUseCustomGroupEditor = 13,
    DssPrivilegesUseDocumentEditor = 14,
    DssPrivilegesUsePromptEditor = 15,
    DssPrivilegesBypassAccessChecks = 16,
    DssPrivilegesWebAdministrator = 17,
    DssPrivilegesWebUser = 18,
    DssPrivilegesWebViewHistoryList = 19,
    DssPrivilegesWebReportManipulations = 20,
    DssPrivilegesWebPivotReport = 20,
    DssPrivilegesWebCreateNewReport = 21,
    DssPrivilegesWebObjectSearch = 22,
    DssPrivilegesWebChangeUserOptions = 23,
    DssPrivilegesWebSaveReport = 24,
    DssPrivilegesWebDrilling = 25,
    DssPrivilegesWebAdvancedDrilling = 25,
    DssPrivilegesWebExport = 26,
    DssPrivilegesWebPrintMode = 27,
    DssPrivilegesWebDeleteObject = 28,
    DssPrivilegesWebManageObject = 28,
    DssPrivilegesWebPublish = 29,
    DssPrivilegesUseServerCache = 30,
    DssPrivilegesWebReportSQL = 31,
    DssPrivilegesReserved2 = 32,
    DssPrivilegesCreateDatamart = 33,
    DssPrivilegesUseDatamartEditor = 34,
    DssPrivilegesUseObjectManager = 35,
    DssPrivilegesWebAddToHistoryList = 36,
    DssPrivilegesWebChangeViewMode = 37,
    DssPrivilegesWebNormalDrilling = 38,
    DssPrivilegesWebDrillOnMetrics = 39,
    DssPrivilegesWebFormatGridAndGraph = 40,
    DssPrivilegesWebScheduling = 41,
    DssPrivilegesWebSubscriptions = 41,
    DssPrivilegesWebScheduleReport = 41,
    DssPrivilegesWebSimultaneousExecution = 42,
    DssPrivilegesWebSwitchPageByElements = 43,
    DssPrivilegesWebSort = 44,
    DssPrivilegesUseVLDBPropertyEditor = 45,
    DssPrivilegesWebSaveTemplatesAndFilters = 46,
    DssPrivilegesWebFilterOnSelections = 47,
    DssPrivilegesScheduleAdministration = 48,
    DssPrivilegesReserved3 = 49,
    DssPrivilegesWebScheduleEMail = 49,
    DssPrivilegesUseUserManager = 50,
    DssPrivilegesUseJobMonitor = 51,
    DssPrivilegesUseUserConnectionMonitor = 52,
    DssPrivilegesUseDatabaseConnectionMonitor = 53,
    DssPrivilegesUseScheduleMonitor = 54,
    DssPrivilegesUseCacheMonitor = 55,
    DssPrivilegesUseClusterMonitor = 56,
    DssPrivilegesUseDatabaseInstanceManager = 57,
    DssPrivilegesUseScheduleManager = 58,
    DssPrivilegesUseProjectMonitor = 59,
    DssPrivilegesReserved4 = 60,
    DssPrivilegesWebSendNow = 60,
    DssPrivilegesBypassSchemaAccessChecks = 61,
    DssPrivilegesModifySchemaObjects = 61,
    DssPrivilegesViewETLInformation = 62,
    DssPrivilegesUseReportObjectsWindow = 63,
    DssPrivilegesUseThresholdsEditor = 64,
    DssPrivilegesUseFormattingEditor = 65,
    DssPrivilegesSaveCustomAutoStyle = 66,
    DssPrivilegesUseReportFilterEditor = 67,
    DssPrivilegesCreateDerivedMetrics = 68,
    DssPrivilegesModifySubtotals = 69,
    DssPrivilegesModifyReportObjectList = 70,
    DssPrivilegesWebCreateDerivedMetrics = 71,
    DssPrivilegesWebModifySubtotals = 72,
    DssPrivilegesWebModifyReportList = 73,
    DssPrivilegesWebUseReportObjectsWindow = 74,
    DssPrivilegesWebUseReportFilterEditor = 75,
    DssPrivilegesWebUseDesignMode = 76,
    DssPrivilegesWebAliasObjects = 77,
    DssPrivilegesWebConfigureToolbars = 78,
    DssPrivilegesWebUseQueryFilterEditor = 79,
    DssPrivilegesWebReExecuteReportAgainstWH = 80,
    DssPrivilegesSetAttributeDisplay = 81,
    DssPrivilegesChooseAttributeFromDisplay = 81,
    DssPrivilegesUseHistoryList = 82,
    DssPrivilegesConfigureToolbars = 83,
    DssPrivilegesChangeUserPreferences = 84,
    DssPrivilegesUseReportDataOptions = 85,
    DssPrivilegesUseDataExplorer = 86,
    DssPrivilegesFormatGraph = 87,
    DssPrivilegesModifySorting = 88,
    DssPrivilegesViewSQL = 89,
    DssPrivilegesCreateNewFolder = 90,
    DssPrivilegesPivotReport = 91,
    DssPrivilegesUseDesignMode = 92,
    DssPrivilegesAliasObjects = 93,
    DssPrivilegesUseGridOptions = 94,
    DssPrivilegesSecondChuckStart = 95,
    DssPrivilegesUseSearchEditor = 95,
    DssPrivilegesReExecuteReportAgainstWH = 96,
    DssPrivilegesUseDrillMapEditor = 97,
    DssPrivilegesSendToEMail = 98,
    DssPrivilegesUseFunctionPlugInEditor = 99,
    DssPrivilegesWebSimpleGraphFormatting = 100,
    DssPrivilegesWebUseLockedHeaders = 101,
    DssPrivilegesWebSetColumnWidths = 102,
    DssPrivilegesUseSecurityFilterManager = 103,
    DssPrivilegesUseProjectStatusEditor = 104,
    DssPrivilegesUseProjectDocumentation = 105,
    DssPrivilegesWebExecuteDatamartReport = 106,
    DssPrivilegesUseSubtotalEditor = 107,
    DssPrivilegesUseFindAndReplaceDialog = 108,
    DssPrivilegesUseCommandManager = 109,
    DssPrivilegesUseRWDocumenEditor = 110,
    DssPrivilegesExecuteRWDocument = 111,
    DssPrivilegesWebExecuteRWDocument = 112,
    DssPrivilegesUseOfficeIntelligence = 113,
    DssPrivilegesWebCreateEmailAddress = 114,
    DssPrivilegesWebPrintNow = 115,
    DssPrivilegesWebScheduledPrinting = 116,
    DssPrivilegesWebCreatePrintLocation = 117,
    DssPrivilegesWebExportToFileNow = 118,
    DssPrivilegesWebScheduledExportToFile = 119,
    DssPrivilegesWebCreateFileLocation = 120,
    DssPrivilegesServerMonitoring = 121,
    DssPrivilegesDefineCustomSQLReport = 122,
    DssPrivilegesDefineFreeformSQLReport = 122,
    DssPrivilegesDefineOLAPCubeReport = 123,
    DssPrivilegesImportOLAPCube = 124,
    DssPrivilegesWebDocumentDesign = 125,
    DssPrivilegesWebManageDocumentDatasets = 126,
    DssPrivilegesWebModifyGridLevelInDocument = 127,
    DssPrivilegesWebModifyUnitInGridInDocument = 127,
    DssPrivilegesWebSetAttributeFormDisplay = 128,
    DssPrivilegesWebChooseAttributeFormDisplay = 128,
    DssPrivilegesWebExecuteBulkExportReport = 129,
    DssPrivilegesWebExecuteBulkExport = 129,
    DssPrivilegesWebNumberFormatting = 130,
    DssPrivilegesUseDesktop = 131,
    DssPrivilegesCreateShortcut = 132,
    DssPrivilegesWebEnableMMTAccess = 133,
    DssPrivilegesDrillWithIntelligentCube = 134,
    DssPrivilegesDefineQueryBuilderReport = 135,
    DssPrivilegesWebDefineQueryBuilderReport = 136,
    DssPrivilegesUseMstrMobile = 137,
    DssPrivilegesMobileViewDocument = 138,
    DssPrivilegesUseIntegrityManager = 139,
    DssPrivilegesCacheAdministration = 140,
    DssPrivilegesUseInboxMonitor = 141,
    DssPrivilegesInboxAdministration = 142,
    DssPrivilegesUseCubeMonitor = 143,
    DssPrivilegesThirdChuckStart = 144,
    DssPrivilegesCubeAdministration = 144,
    DssPrivilegesFireEvent = 145,
    DssPrivilegesUseTransmitterDeviceManager = 146,
    DssPrivilegesUseContactManager = 147,
    DssPrivilegesUseSecurityRoleManager = 148,
    DssPrivilegesAssignSecurityRoles = 149,
    DssPrivilegesConfigureContactDataProfile = 150,
    DssPrivilegesResetUserPasswords = 151,
    DssPrivilegesLinkToExternalAccounts = 152,
    DssPrivilegesGrantPrivilege = 153,
    DssPrivilegesConfigureGroupMembership = 154,
    DssPrivilegesUseDatabaseLoginManager = 155,
    DssPrivilegesDefineSecurityFilter = 156,
    DssPrivilegesClusterAdministration = 157,
    DssPrivilegesLoadProject = 158,
    DssPrivilegesIdleProject = 159,
    DssPrivilegesDefineViewReport = 160,
    DssPrivilegesUseCubeReportEditor = 161,
    DssPrivilegesExecuteCubeReport = 162,
    DssPrivilegesWebDefineViewReport = 163,
    DssPrivilegesWebExecuteCubeReport = 164,
    DssPrivilegesConfigureServerBasic = 165,
    DssPrivilegesConfigureStatistics = 166,
    DssPrivilegesConfigureSecurity = 167,
    DssPrivilegesConfigureInbox = 168,
    DssPrivilegesConfigureGoverning = 169,
    DssPrivilegesConfigureCaches = 170,
    DssPrivilegesConfigureProjectBasic = 171,
    DssPrivilegesConfigureConnectionMap = 172,
    DssPrivilegesConfigureProjectDataSource = 173,
    DssPrivilegesEnableUser = 174,
    DssPrivilegesConfigureSubscriptionSettings = 175,
    DssPrivilegesConfigureAuditing = 176,
    DssPrivilegesConfigureLanguageSettings = 177,
    DssPrivilegesMonitorChangeJournal = 178,
    DssPrivilegesUseBulkTranslationTool = 179,
    DssPrivilegesUseBulkExportEditor = 180,
    DssPrivilegesJobAdministration = 181,
    DssPrivilegesUserConnectionAdministration = 182,
    DssPrivilegesDatabaseConnectionAdministration = 183,
    DssPrivilegesDuplicateProject = 184,
    DssPrivilegesSubscribeOthers = 185,
    DssPrivilegesSubscribeToEmailAlert = 186,
    DssPrivilegesSendLinkInEmail = 187,
    DssPrivilegesExportToPDF = 188,
    DssPrivilegesExportToHTML = 189,
    DssPrivilegesExportToExcel = 190,
    DssPrivilegesExportToText = 191,
    DssPrivilegesExportToFlash = 192,
    DssPrivilegesViewNotes = 193,
	DssPrivilegesAddNotes = 194,
    DssPrivilegesEditNotes = 195,
    DssPrivilegesUseDistributionServices = 196,
    DssPrivilegesUseImmediateDelivery = 197,
    DssPrivilegesEnableAdministrationFromWeb = 198,
    DssPrivilegesUseTranslationEditor = 199,
    DssPrivilegesWebUsePromptEditor = 200,
    DssPrivilegesCreateHTMLContainer = 201,
    DssPrivilegesImportTableFromMultipleSources = 202,
    DssPrivilegesExecuteMultipleSourceReport = 203,
    DssPrivilegesSavePersonalPromptAnswers = 204,
    DssPrivilegesWebDefineDerivedElements = 205,
    DssPrivilegesWebUseBasicThresholdEditor = 206,
    DssPrivilegesWebDashboardDesign = 207,
    DssPrivilegesWebSaveDerivedElements = 208,
    DssPrivilegesWebUseAdvancedThresholdEditor = 209,
    DssPrivilegesWebEditReportLinks = 210,
    DssPrivilegesWebEditDrillingAndLinks = 210,
    DssPrivilegesDefineDerivedElements = 211,
    DssPrivilegesDashboardDesign = 212,
    DssPrivilegesSaveDerivedElements = 213,
    DssPrivilegesEditReportLinks = 214,
    DssPrivilegesUseLinkEditor = 214,
    DssPrivilegesWebEditNotes = 215,
    DssPrivilegesDrillAndLink = 216,
    DssPrivilegesWebDefineAdvancedReportOptions = 217,
    DssPrivilegesWebCreateHTMLContainer = 218,
    DssPrivilegesUseOLAPServices = 219,
    DssPrivilegesUseDynamicSourcing = 220,
    DssPrivilegesUseSQLTabInExportEditors = 221,
    DssPrivilegesWebUseCustomGroupEditor = 222,
    DssPrivilegesWebImportData = 223,
    DssPrivilegesWebImportDatabase = 224,
	DssPrivilegesDefineTransactionReport = 225,	// This is the first privilege for 9.2
	DssPrivilegesWebConfigureTransaction = 226,
	DssPrivilegesExecuteTransaction = 227,
	DssPrivilegesWebVisualExploration = 228,
	DssPrivilegesUseObjectManagerReadOnly = 229,
	DssPrivilegesUseObjectManagerImportOnly = 230,
	DssPrivilegesUseTranslationEditorBypass = 231,
    DssPrivilegesMaximum = 232,
	DssPrivilegesWebCreateAnalysis = 233,
	DssPrivilegesWebSaveAnalysis = 234,
	DssPrivilegesPrintFromDevice = 235,	// This is the first privilege for 9.3
	DssPrivilegesEmailScreenFromDevice = 236
    
}EnumDSSPrivilegeType;

typedef enum EnumDSSViewMedia
{
	DssViewMediaAvailableViewStatic = 0x00000001, 
	DssViewMediaAvailableViewInteractive = 0x00000002,
	DssViewMediaAvailableViewEditable = 0x00000004,
	DssViewMediaAvailableViewFlash = 0x00000008,
	DssViewMediaAvailableExportExcel = 0x00000010,
	DssViewMediaAvailableExportPDF = 0x00000020,
	DssViewMediaAvailableExportHTML = 0x00000040,
	DssViewMediaAvailableExportFlash = 0x00000080,
	DssViewMediaAvailableExportExcelPlainText = 0x00000100,
	DssViewMediaAvailableExportCSV = 0x00000200,
	DssViewMediaAvailableExportPlainText = 0x00000400,
	DssViewMediaAvailableViewAnalysis = 0x00000800,
    DssViewMediaAvailableViewAnalysisHTML5 = 0x00002000,
    DssViewMediaAvailablePrivate  = 0x00008000,
	
	// Mask for available view media
	DssViewMediaAvailableMediaMask = 0x07ffffff,
	
	// For default view media--value changed from bit vector to seq# using high 5 bits, for value 0 to 31. 
	DssViewMediaReserved = 0, 
	DssViewMediaViewStatic = 0x08000000, 
	DssViewMediaViewInteractive = 0x10000000,
	DssViewMediaViewEditable = 0x18000000,
	DssViewMediaViewFlash = 0x20000000,
	DssViewMediaExportExcel = 0x28000000,
	DssViewMediaExportPDF = 0x30000000,
	DssViewMediaExportHTML = 0x38000000,
	DssViewMediaExportFlash = 0x40000000, 
	DssViewMediaExportExcelPlainText = 0x48000000,
	DssViewMediaExportCSV = 0x50000000,
	DssViewMediaExportPlainText = 0x58000000,
	DssViewMediaViewAnalysis = 0x60000000,
    DssViewMediaViewAnalysisHTML5 = 0x70000000,
	
	// Mask for default view media
	DssViewMediaDefaultMask = 0xf8000000
}EnumDSSViewMedia;

typedef enum EnumDSSDeviceOrientation
{
	DssDeviceOrientationReserved = 0,
	DssDeviceOrientationLandscape = 1,
	DssDeviceOrientationPortrait = 2
}EnumDSSDeviceOrientation;

typedef enum EnumDSSMonth
{
    DssMonthJanuary = 1,
    DssMonthFebruary = 2,
    DssMonthMarch = 3,
    DssMonthApril = 4,
    DssMonthMay = 5,
    DssMonthJune = 6,
    DssMonthJuly = 7,
    DssMonthAugust = 8,
    DssMonthSeptember = 9,
    DssMonthOctober = 10,
    DssMonthNovember = 11,
    DssMonthDecember = 12
} EnumDSSMonth;

typedef enum EnumDSSXMLDocSaveAsFlags
{
    DssXmlDocSaveAsReserved = 0,
    DssXmlDocSaveAsDefault = 0x00,
    DssXmlDocSaveAsOverwrite = 0x01,
    DssXmlDocSaveAsWithAnswersOpen = 0x02,
    DssXmlDocSaveAsWithAnswersClosed = 0x04
} EnumDSSXMLDocSaveAsFlags;

typedef enum EnumDSSSortFlag
{
    DssSortFlagReserved             = 0,
    DssSortFlagDefault              = 1,
    DssSortFlagMicroChart           = 2,
    DssSortFlagMicroChartTreeMode   = 3
} EnumDSSSortFlag;

typedef enum EnumDSSTransactionAction
{
    DssTransactionActionReserved =      0,
    DssTransactionActionDisable =       0x1,
    DssTransactionActionHide =          0x2,
    DssTransactionActionMarkRequired =  0x4,
} EnumDSSTransactionAction;

typedef enum EnumDSSCSIPartitionEvaluationMode
{
	DssCSIModeInvalid = 0,
	DssCSIModeSingleNodeNonPartitioned = 1,
	DssCSIModeSingleNodePartitioned = 2,
	DssCSIModeMultiNodePartitioned = 3
} EnumDSSCSIPartitionEvaluationMode;

typedef enum EnumDSSTransactionErrorCode
{
    DSSTXN_ERR_NONE = 0,
    DSSTXN_ERR_INSERT_INVALID_FORM = 1,
    DSSTXN_ERR_INSERT_DUPLICATED_ROW = 2,
    DSSTXN_ERR_WRONG_CSI_INVALID_UNIT = 3,
    DSSTXN_ERR_WRONG_CSI_UPDATE_LM = 4,
    DSSTXN_ERR_WRONG_CSI_UPDATE_LMC = 5, //update component metric of level metric
    DSSTXN_ERR_WRONG_CSI_UPDATE_COMPID = 6, //update incomplete compound ID
    DSSTXN_ERR_DATA_TYPE_CONVERSION = 7,
    DSSTXN_ERR_UPDATE_DUPLICATED_ROW = 8,
    DSSTXN_ERR_INSERT_BREAK_RELATION = 9,
    DSSTXN_ERR_UPDATE_BREAK_RELATION = 10,
    DSSTXN_ERR_DELETE_LEVEL_METRIC = 11,
    DSSTXN_ERR_INPUT_DATA_OVERFLOW = 12,
    DSSTXN_ERR_MISSING_OBJECT = 13,
    DSSTXN_ERR_UPDATE_RELATION_LM = 14,
    DSSTXN_ERR_CONSOLIDATION = 15
} EnumDSSTransactionErrorCode;

typedef enum EnumDSSTimeRole
{
    DssTimeRoleUndefined = -1,
    DssTimeRoleNone = 0,
    DssTimeRoleDate = 1,
    DssTimeRoleDateTime = 2,
    DssTimeRoleTime = 3,
    DssTimeRoleSecond = 4,
    DssTimeRoleMinute = 5,
    DssTimeRoleHour = 6,
    DssTimeRoleDay = 7,
    DssTimeRoleWeek = 8,
    DssTimeRoleMonth = 9,
    DssTimeRoleQuarter = 10,
    DssTimeRoleYear = 11,
    DssTimeRoleSeasonalRoot = 0x0100,
    DssTimeRoleYearOfDecade = 0xc0b,
    DssTimeRoleQuarterOfYear = 0xb0a,
    DssTimeRoleMonthOfYear = 0xb09,
    DssTimeRoleMonthOfQuarter = 0xa09,
    DssTimeRoleWeekOfYear = 0xb08,
    DssTimeRoleWeekOfQuarter = 0xa08,
    DssTimeRoleWeekOfMonth = 0x908,
    DssTimeRoleDayOfYear = 0xb07,
    DssTimeRoleDayOfQuarter = 0xa07,
    DssTimeRoleDayOfMonth = 0x907,
    DssTimeRoleDayOfWeek = 0x807,
    DssTimeRoleHourOfDay = 0x706,
    DssTimeRoleMinuteOfDay = 0x705,
    DssTimeRoleMinuteOfHour = 0x605,
    DssTimeRoleSecondOfDay = 0x704,
    DssTimeRoleSecondOfHour = 0x604,
    DssTimeRoleSecondOfMinute = 0x504,
    DssTimeRoleSeasonalEnd = 0xffff,
    DssTimeRoleLastOne
} EnumDSSTimeRole;

typedef enum EnumDSSRecursiveDefnColumns
{
    DssRecursiveDefnColumnReserved = 0,
    DssRecursiveDefnColumnParent = 1,
    DssRecursiveDefnColumnJoinID = 2,
    DssRecursiveDefnColumnFilterID = 3,
    DssRecursiveDefnColumnDistance = 4,
    DssRecursiveDefnColumnLastOne = 5
} EnumDSSRecursiveDefnColumns;
#endif
