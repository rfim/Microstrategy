/*
 *  Enums.h
 *  MSTR Foundation
 *
 *  Created by Amirali Charania on 2/25/09.
 *  Copyright 2009 MicroStrategy. All rights reserved.
 *
 */


#ifndef __MSTR_ENUMS__
#define __MSTR_ENUMS__

//TODO eventually this should be exposed as a setting
#define ROWS_PER_PAGE [MSIGeneric incrementalFetchSize]//100

typedef enum _AxisType{
	ROW_AXIS = 0, COLUMN_AXIS = 1
}AxisType;
typedef enum _ModelType {
	UnrecognizedModel = -1, TabularDataModel = 0, TimeSeriesDataModel = 1, PlainDataModel = 2, CrosstabDataModel = 3, MultipleDataModels = 4
} ModelType;
typedef enum _RequestType {
	HTTP= 0, HTTPS = 1
} RequestType;

typedef enum _WebServerTypes {
	J2EE = 0, DOTNET = 1
} WebServerTypes;

typedef enum _NetworkRequestType {
	NetworkRequestTypeTask = 0, NetworkRequestTypeWeb = 1
} NetworkRequestType;

typedef enum _ReportDataTypes {
	Grid = 1, Graph, RWD
} ReportDataTypes;

typedef enum _ResultSetTypes{
	Report = 1, Document
} ResultSetTypes;

typedef enum _HeaderTypes{
	HeaderTypeUndefined = 0, HeaderTypeMetric, HeaderTypeAttribute, HeaderTypeConsolidation, HeaderTypeCustomGroup,
	HeaderTypeCustomGroupItem, HeaderTypeSubtotal
} HeaderTypes;

typedef enum _ObjectTypes {
    ObjectTypeUnknown = -1, ObjectTypeReportDefinition = 3, ObjectTypeDocumentDefinition = 55, ObjectTypeAttribute = 12, ObjectTypeMetric = 4,
	ObjectTypeAttributeForm = 21, ObjectTypePrompt=10, ObjectTypeFolder = 8, ObjectTypeConsolidation = 47, ObjectTypeCustomGroup = 1,
	ObjectTypeFilter = 1, ObjectTypeDimension=14, ObjectTypeSearch = 39, ObjectTypeExternalLink=67, ObjectTypeExternalObject=68, ObjectTypeFact=13, ObjectTypeTemplate=2, ObjectPromptFilter = 10
} ObjectTypes;

typedef enum _ObjectSubTypes {
    ObjectSubTypeUnknown = -1, ObjectSubTypeReportGraph = 0x301, ObjectSubTypeReportGrid = 0x300, ObjectSubTypeReportGridAndGraph = 0x306,
	ObjectSubTypeFilter = 0x100, ObjectSubTypeMetric = 0x400, ObjectSubTypeFolder = 0x800, ObjectSubTypeAttribute = 0xC00,
	ObjectSubTypeAttributeForm = 0x1500, ObjectSubTypeReportWritingDocument = 0x3701, ObjectSubTypeReportCube = 776, ObjectSubTypeSegment = 0x103
	,ObjectSubTypeExternalLink=17153, ObjectSubTypeExternalObject=17408, ObjectSubTypeCustomGroup = 257

} ObjectSubTypes;

typedef enum _EnumDSSXMLExecutionFlags {
	DssXmlExecutionUseCache = 0x80,
	DssXmlExecutionUpdateCache = 0x100,
	DssXmlExecutionResolve = 0x00010000    
} EnumDSSXMLExecutionFlags;

typedef enum _EnumDSSXMLDocExecutionFlags {
    DssXmlDocExecutionFresh = 0x1,
	DssXmlDocExecutionXML = 0x4,
	DssXmlDocExecutionUseRWDCache = 0x4000,
	DssXmlDocExecutionUpdateRWDCache = 0x8000,
	DssXmlDocExecutionFlash = 0x08000000,
    DssXmlDocExecuteOnBackground = 0x00020000,
    DssXmlDocExecuteForceExec = 0x00040000,
    DssDocumentExecuteForSmartClient = 0x01000000,
} EnumDSSXMLDocExecutionFlags;

typedef enum _EnumDSSXMLObjectFlags {
    
    /** Specifies for object browsing a bit mask indicating to load the object's type specific definition
     in the returning XML. */
     DssXmlObjectDefn = 0x00000001,
    
    /** Specifies for object browsing a bit mask indicating to load the object's Name, Abbreviation, Description, Hidden
     and Path fields in the returning XML. */
    DssXmlObjectBrowser = 0x00000002,
    
    /** Specifies for object browsing a bit mask indicating to load the object's modification time, etc.
     in the returning XML. */
    DssXmlObjectDates = 0x00000004,
    
    /** Specifies for object browsing a bit mask indicating to load the object's comments
     in the returning XML. */
    DssXmlObjectComments = 0x00000008,
    
    /** Specifies for object browsing a bit mask indicating to load the object's dependents list
     in the returning XML. */
    DssXmlObjectDependents = 0x00000010,
    
    /** Specifies for object browsing a bit mask indicating to load the object's properties
     in the returning XML. */
    DssXmlObjectProperties = 0x00000020,
    
    /** Specifies for object browsing a bit mask indicating to load the object's security information
     in the returning XML. */
    DssXmlObjectSecurity = 0x00000040,
    
    /** Specifies for object browsing a bit mask indicating to include the object's change journal comments. */
    DssXmlObjectTotalObject = 0x0000007f,
    
    /** Specifies for object browsing a bit mask indicating to load the object's security information
     in the returning XML. */
    DssXmlObjectChangeJournalComments = 0x00000080,
    
    /** Specifies for object browsing a bit mask indicating to include type specific definition of dependent objects
     in the returning XML. */
    DssXmlObjectDepnDefn = 0x00000100,
    
    /** Specifies for object browsing a bit mask indicating to include browser information for dependent objects
     in the returning XML. */
    DssXmlObjectDepnBrowser = 0x00000200,
    
    /** Specifies for object browsing a bit mask indicating to include date information for dependent objects
     in the returning XML. */
    DssXmlObjectDepnDates = 0x00000400,
    
    /** Specifies for object browsing a bit mask indicating to include comment information for dependent objects
     in the returning XML. */
    DssXmlObjectDepnComments = 0x00000800,
    
    /** Specifies for object browsing a bit mask indicating to include dependents information for dependent objects
     in the returning XML. */
    DssXmlObjectDepnDependents = 0x00001000,
    
    /** Specifies for object browsing a bit mask indicating to include properties for dependent objects
     in the returning XML. */
    DssXmlObjectDepnProperties = 0x00002000,
    
    /** Specifies for object browsing a bit mask indicating to include security information for dependent objects
     in the returning XML. */
    DssXmlObjectDepnSecurity = 0x00004000,
    
    /** Specifies for object browsing a bit mask indicating to include all the information about dependent objects
     in the returning XML. */
    DssXmlObjectTotalDepn = 0x00007f00,
    
    /** Specifies for object browsing a bit mask indicating not to add it to cache if call creates a new local object. */
    DssXmlObjectDoNotCache = 0x00020000,
    
    /** Specifies for object browsing a bit mask indicating to check web server cache first. */
    DssXmlObjectCheckWebCache = 0x00040000,
    
    /** Specifies for object browsing a bit mask indicating to only user web server cache. */
    DssXmlObjectUseWebCacheOnly = 0x00080000,
    
    /**
     * Specifies that a Save should proceed if we find the object was changed on
     * persistent storage.
     *
     * @since MicroStrategy Web 7.5.0
     */
    DssXmlObjectSaveOverwrite = 0x01000000,
    
    /**
     * Specifies that removing non-primary translations in object name
     * @since MicroStrategy Web 9.3.1
     */
    DssXmlObjectRemoveNonPrimaryNameTranslations = 0x02000000,
    
    /** Specifies for object browsing a bit mask indicating to load ancestors of the object in the returning XML. */
    DssXmlObjectAncestors = 0x10000000,
    
    /** Specifies for object browsing a bit mask indicating to find and load the hidden objects in the returning XML. */
    DssXmlObjectFindHidden = 0x20000000,
    
    /** Specifies that an expression node returned from the IServer should contain {@link XMLTags#Att_PlainText} with a parsable value*/
    DssXmlObjectPlainText = 0x40000000,
} EnumDSSXMLObjectFlags;

typedef enum _EnumDSSXMLStatus {
    DssXmlStatusMsgID	= 0,
    DssXmlStatusResult	= 1,
    DssXmlStatusPromptXML	= 2,
    DssXmlStatusErrMsgXML	= 3,
    DssXmlStatusJobRunning	= 4,
    DssXmlStatusInSQLEngine	= 5,
    DssXmlStatusInQueryEngine	= 6,
    DssXmlStatusInAnalyticalEngine	= 7,
    DssXmlStatusInResolution	= 8,
    DssXmlStatusWaitingForCache	= 9,
    DssXmlStatusUpdatingCache	= 10,
    DssXmlStatusWaiting	= 13,
    DssXmlStatusWaitingOnGovernor	= 14,
    DssXmlStatusWaitingForProject	= 15,
    DssXmlStatusWaitingForChildren	= 16,
    DssXmlStatusPreparingOutput	= 17,
    DssXmlStatusRequestExecutionError	= 18,
    DssXmlStatusConstructResult	= 19,
    DssXmlStatusHTMLResult	= 20,
    DssXmlStatusXMLResult	= 21,
    DssXmlStatusRunningOnOtherNode	= 22,
    DssXmlLoadingPrompt	= 23,
    DssXmlInExportEngine	= 24
} EnumDSSXMLStatus;

//ASYNCEXC: add enum for inbox request
typedef enum _EnumDSSXMLInboxFlags {
    DssXmlInboxBrowsingInfo     = 1,
    DssXmlInboxBrowsingDetail   = 2,
    DssXmlInboxCurrentProject   = 3,         //this is deprecated and shouldn't be used anymore
    DssXmlInboxAllProjects      = 4,
    DssXmlInboxKeepLatest       = 5,         // Instructs the inbox to keep the latest report result
    DssXmlInboxKeepAsIs         = 6,         // Instructs the inbox to keep the report result as is
    DssXmlInboxStatusOnly       = 16         // to poll message status only
} EnumDSSXMLInboxFlags;


typedef enum _EnumReportViewModes {
	ReportViewModeExecute = 0, ReportViewModeDesign = 1, ReportViewModeCreate = 4, ReportViewModeSaving = 5
} EnumReportViewModes;

typedef enum _EnumRWExecutionModes {
	RWModeData = 0x1,
    RWModeDesign = 0x2,
    RWModePDF = 0x3,
    RWModeExcel = 0x4,
    RWModeSave = 0x5,
    RWModeDetals = 0x6,
    RWModeFlashExport = 0x7,
    RWModeFlashView = 0x8,
    RWModeFlashBinaryView = 0x9,
    RWModeSubscribe = 0x10,
    RWModeCSV = 0xA,
    RWModeMSTR = 0x12
} EnumRWExecutionModes;

typedef enum _EnumDSSXMLResultFlags {
	DssXmlResultGrid = 0x20000,
	DssXmlResultStatusOnlyIfNotReady = 0x2000000,
    DssXmlResultPreserve = 0x80000000,
    DssXmlResultNoDerivedPromptXML = 0x00000020
} EnumDSSXMLResultFlags;

typedef enum _BaseFormSemanticType {
	SemanticTypeNone = 0, SemanticTypeNumber = 2, SemanticTypeReplaceText = 3, SemanticTypePicture = 4, SemanticTypeUrl = 5, SemanticTypeQuickSymbol = 10
} BaseFormSemanticType;

typedef enum _QuickSymbol {
    SymbolReserved = 0, SymbolBlackCircle = 1, SymbolBlackSquare = 2, SymbolLozenge = 3, SymbolBlackDiamond = 4, SymbolInkBlot = 5,
    SymbolWheelofDharma = 6, SymbolBlackFlorette = 7, SymbolPlaceInterestSign = 8, SymbolHeavyCheckMark = 9,
    SymbolHeavyBallotX = 10,
	SymbolRightArrow = 29, SymbolUpArrow = 30, SymbolDownArrow = 31
} QuickSymbol;

typedef enum _AuthModes {
	AuthModeDefault = 0, AuthModeStandard = 1, AuthModeNTCredential = 2, AuthModePassThrough = 4, AuthModeAnonymous = 8, AuthModeLDAP = 0x10, AuthModeWarehousePassthrough = 0x20, AuthModeSimpleSecurityPlugin = 0x40, AuthModeIntegrated = 0x80, AuthModeFacebook = -1, AuthModeUsher = 5
} AuthModes;
 
typedef enum _WebAuthModes {
    WebAuthModeDefault = 0, WebAuthModeAnonymous = 1, WebAuthModeBasic = 2, WebAuthModeNTCredential = 3, WebAuthModeIntegratedAuthentication = 4, WebAuthModeHTMLForm = 5
} WebAuthModes;

typedef enum _TaskGroupType {
	TaskGroupReconcile = 0, TaskGroupRetrieveData = 1, TaskGroupFolderBrowse = 2, TaskGroupDeferExecReconcile=3
} TaskGroupType;

typedef enum _ServiceStates{
	SERVICE_INITIALIZED = 0, SERVICE_EXECUTING = 1, SERVICE_SUCCESS = 2, SERVICE_FAILED = 3, SERVICE_CANCELLED = 4, SERVICE_INTERRUPTED = 5
} ServiceStates;

/**
 LogLevels are used for controlling the level of detail or volume of entries that get logged. Lower values result in more info being logged. Values range from All (0) to Off (16). It is used for guidance on whether to log something, but is not enforced. For example,

	 if ( [[MSILog getLogger] isLoggable: Severe ] )
	 {
		[[MSILog getLogger] logWithLevel: Severe
							className: @"ApplicationLauncherController"
						   methodName: @"startLaunch"
							  message: @"The URL is not valid." ];
	 }

 */
typedef enum _LogLevels {
	All = 0,
	Finest = 2,
	Finer = 4,
	Fine = 6,
	Config = 8,
	Info = 10,
	Warning = 12,
	Severe = 14,
	Off = 16,
	Debug = 18 // Client Only, controlled by Debug Logging MSIPreference
} LogLevels;

typedef enum _AreaShapes {
	AreaShapePolygon=6, AreaShapeRectangle=7
} AreaShapes;

typedef enum _LinkAnswerModes{
	LinkAnswerSamePrompt = 1, LinkAnswerDoNotAnswer = 2, LinkAnswerClose = 3, LinkAnswerDynamic = 4, LinkAnswerStatic = 5, LinkAnswerCurrentUnit = 6, LinkAnswerAllValidUnits=7, LinkAnswerUseDefaultAnswer = 8
} LinkAnswerModes;

typedef enum _DataTypes{
	DataTypeInteger = 1, DataTypeNumeric = 3, DataTypeDecimal = 4, DataTypeReal = 5,
	DataTypeDouble = 6, DataTypeFloat = 7, DataTypeChar = 8, DataTypeDate = 14, DataTypeLong = 22,
	DataTypeBool = 24, DataTypeBigDecimal = 30, DataTypeObject = 40,

} DataTypes;

typedef enum _FormatPropertySetNames{
	FormattingPatterns = 0, FormattingBorder, FormattingFont, FormattingNumber, FormattingAlignment,
	//more added by stone.
	FormattingPadding,  FormattingSize, FormattingColumnWidth,FormattingAppearance
}FormatPropertySetNames;

typedef enum _PatternFormattingProperties{
	PatternFormattingFillColor = 1,
	PatternFormattingPatternColor = 2,	PatternFormattingPatternStyle = 3,	PatternFormattingFillStyle = 4,
	PatternFormattingApplyToGraphThreshold = 5,		PatternFormattingGradientColor = 6,		PatternFormattingGradientAngle = 7,
	PatternFormattingXOffset = 8,	PatternFormattingYOffset = 9
}PatternFormattingProperties;

typedef enum _BorderFormattingProperties{
	BorderFormattingTopStyle = 3, BorderFormattingLeftStyle = 4, BorderFormattingBottomStyle = 5, BorderFormattingRightStyle = 6,
	BorderFormattingTopColor = 9, BorderFormattingLeftColor = 10, BorderFormattingBottomColor = 11, BorderFormattingRightColor = 12,
	BorderFormattingHInsideStyle = 1, BorderFormattingVInsideStyle = 2, BorderFormattingHInsideColor = 7, BorderFormattingVInsideColor = 8,
	BorderFormatting3DStyle = 13, BorderFormatting3DWeight = 14
}BorderFormattingProperties;

typedef enum _FontFormattingProperties{
	FontFormattingBold = 2, FontFormattingItalic = 3, FontFormattingStrikeout = 5, FontFormattingUnderline = 6, FontFormattingColor = 7,
	FontFormattingName = 1, FontFormattingSize = 4, FontFormattingScript = 8
}FontFormattingProperties;

typedef enum _AlignmentFormattingProperties{
	AlignmentFormattingHorizontal = 1,
	AlignmentFormattingVertical = 2,
	AlignmentFormattingTextWrap = 3,
	AlignmentFormattingTextDirection = 4
}AlignmentFormattingProperties;


typedef enum _NumberFormattingProperties
{
	NumberFormattingCategory = 1,   NumberFormattingDecimalPlaces,    NumberFormattingThousandSeparator,
	NumberFormattingCurrencySymbol, NumberFormattingCurrencyPosition, NumberFormattingFormat,
	NumberFormattingNegativeNumbers
}NumberFormattingProperties;

typedef enum _PaddingFormattingProperties
{
	PaddingFormattingLeftPadding = 1, PaddingFormattingRightPadding = 2,
	PaddingFormattingTopPadding = 3,  PaddingFormattingBottomPadding = 4,
	//PaddingFormattingLineSpacing = 5 //not used.
}PaddingFormattingProperties;

typedef enum _SizeFormattingProperties
{
	SizeFormattingHeightMode = 6
}SizeFormattingProperties;

typedef enum _ColumnWidthFormattingProperties
{
	ColumnWidthFormattingColumnScenario = 1
}ColumnWidthFormattingProperties;

typedef enum _AppearanceFormattingProperties
{
	AppearanceFormattingVisible = 2
}AppearanceFormattingProperties;

typedef enum _Function{
	FunctionUnknown = -2, FunctionCustom = -1, FunctionReserved = 0, FunctionPlus = 1, FunctionMinus = 2, FunctionTimes = 3, FunctionDivide = 4,
	FunctionUnaryMinus = 5, FunctionEquals = 6, FunctionNotEqual = 7, FunctionGreater = 8, FunctionLess = 9, FunctionGreaterEqual = 10,
	FunctionLessEqual = 11, FunctionSum = 12, FunctionCount = 13, FunctionAvg = 14, FunctionMin = 15, FunctionMax = 16,
	FunctionBetween = 17, FunctionLike = 18, FunctionAnd = 19, FunctionOr = 20, FunctionNot = 21, FunctionIn = 22,
	FunctionRank = 23, FunctionAbs = 24, FunctionRunningSum = 25, FunctionRunningAvg = 26, FunctionMovingSum = 28,
	FunctionMovingAvg = 27, FunctionProduct = 29, FunctionMedian = 30, FunctionMode = 31, FunctionStdev = 32, FunctionVar = 33,
	FunctionGeomean = 34, FunctionEqualEnhanced = 35, FunctionNotEqualEnhanced = 36, FunctionGreaterEqualEnhanced = 37, FunctionLessEqualEnhanced = 38,
	FunctionBetweenEnhanced = 39, FunctionBanding = 40, FunctionBandingC = 41, FunctionBandingP = 42, FunctionNotLike = 43, FunctionNotBetween = 44,
	FunctionIntersect = 45, FunctionIntersectIn = 46, FunctionNullToZero = 47, FunctionZeroToNull = 48, FunctionApplySimple = 49,
	FunctionApplyAggregation = 50, FunctionApplyLogic = 51, FunctionApplyComparison = 52, FunctionApplyRelative = 53, FunctionIsNull = 54,
	FunctionIsNotNull = 55, FunctionUcase = 56, FunctionNotIn = 57, FunctionNTile = 58, FunctionPercentile = 59, FunctionMovingMax = 60,
	FunctionMovingMin = 61, FunctionMovingDifference = 62, FunctionMovingStdev = 63, FunctionExpWghMovingAvg = 64, FunctionMovingCount = 65,
	FunctionRunningMax = 66, FunctionRunningMin = 67, FunctionRunningStdev = 68, FunctionRunningCount = 69, FunctionExpWghRunningAvg = 70,
	FunctionNotBetweenEnhanced = 71, FunctionConcat = 72, FunctionFirstInRange = 73, FunctionLastInRange = 74, FunctionValueSegment = 75,
	FunctionContains = 76, FunctionBeginsWith = 77, FunctionEndsWith = 78, FunctionNotContains = 79, FunctionNotBeginsWith = 80,
	FunctionNotEndsWith = 81, FunctionCase = 82, FunctionCaseV = 83, FunctionStdevP = 84, FunctionRunningStdevP = 85, FunctionMovingStdevP = 86,
	FunctionNTileS = 87, FunctionNTileVS = 88, FunctionVarP = 89, FunctionCurrentDate = 90, FunctionDayOfMonth = 91, FunctionDayOfWeek = 92,
	FunctionDayOfYear = 93, FunctionWeek = 94, FunctionMonth = 95, FunctionQuarter = 96, FunctionYear = 97, FunctionCurrentDateTime = 98,
	FunctionCurrentTime = 99, FunctionHour = 100, FunctionMinute = 101, FunctionSecond = 102, FunctionMilliSecond = 103, FunctionConcatNoBlank = 104,
	FunctionLength = 105, FunctionLower = 106, FunctionLTrim = 107, FunctionPosition = 108, FunctionRTrim = 109, FunctionSubStr = 110,
	FunctionInitCap = 111, FunctionTrim = 112, FunctionRightStr = 113, FunctionLeftStr = 114, FunctionGreatest = 115, FunctionLeast = 116,
	FunctionAdd = 134, FunctionAverage = 135, FunctionMultiply = 136, FunctionBandingM = 137, FunctionTuple = 1000
} Function;

typedef enum _MRPFunction{
    MRPFunctionReserved = 0, MRPFunctionTop = 1, MRPFunctionBottom = 2, MRPFunctionBetween = 3, MRPFunctionExcludeTop = 4,
	MRPFunctionExcludeBottom = 5, MRPFunctionNotBetween = 6, MRPFunctionEquals = 7, MRPFunctionDifferentFrom = 8,
	MRPFunctionBanding = 9, MRPFunctionBandingC = 10, MRPFunctionBandingP = 11, MRPFunctionBandingM = 12
} MRPFunction;

typedef enum _ExpressionTypes{
	ExpressionTypeReserved = 0, ExpressionTypeGeneric = 1, ExpressionTypeSingleBaseFormQual = 2, ExpressionTypeMultiBaseFormQual = 3,
	ExpressionTypeJointFormQual = 4, ExpressionTypeListQual = 5, ExpressionTypeListFormQual = 6, ExpressionTypeJointListQual = 7,
	ExpressionTypeJointListFormQual = 8, ExpressionTypeSingleBaseFormExpression = 9, ExpressionTypeSingleMetricQual = 10,
	ExpressionTypeMultiMetricQual = 11, ExpressionTypeMetricExpression = 12, ExpressionTypeEmbedQual = 13, ExpressionTypeBranchQual = 14,
	ExpressionTypeRelationshipQual = 15, ExpressionTypeAllAttributeQual = 16, ExpressionTypeAttributeIDQual = 17, ExpressionTypeAttributeDESCQual = 18,
	ExpressionTypeAggMetric = 19, ExpressionTypeBanding = 20, ExpressionTypeReportQual = 21, ExpressionTypeMDXSAPVariable = 22,
	ExpressionTypeSQLQueryQual = 23, ExpressionTypeCancelledPrompt = 24, ExpressionTypeElementList = 25, ExpressionTypeElementSingle = 26
} ExpressionTypes;

/**
 This is used to tag a subview
 **/
// TagPageByControl is not used now. wangchengwen 2014/7/3
typedef enum _SubviewTag{
	TagPageByControl = 20000, TagLayoutTabControl, TagPaginationControl, TagMSTRView, 
	TagMojoView, TagWebView, TagDataView, TagOverlayImage, TagBaseReportView,
	TagDataContainerView, TagErrorView, TagWaitView, TagMiscView, TagEditControlViewer,TagUIImageView,TagEditedIndicatorView,
	TagMarkRowView,
	TagMaskView,
	TagGridGraphView, TagWidgetView,
    TagMobileVIFilterPanelStackView,TagMobileVIVisualizationContentStackView,TagMobileVIPageByStackView,TagMobileVIVisualizationsStackView,
	TagDataInputControl
} SubviewTag;

typedef enum _Position{
	TOP_LEFT,
	TOP_MIDDLE,
	TOP_RIGHT,
	LEFT_MIDDLE,
	CENTER,
	RIGHT_MIDDLE,
	BOTTOM_LEFT,
	BOTTOM_MIDDLE,
	BOTTOM_RIGHT,
	TOP,
	LEFT,
	RIGHT,
	BOTTOM
} Position;

typedef enum _Direction{
	DIRECTION_UNDEFINED = -1,
	DIRECTION_UP,
	DIRECTION_LEFT,
	DIRECTION_RIGHT,
	DIRECTION_DOWN,
	DIRECTION_DIAGONAL
} Direction;

typedef enum _HomeScreenTypes{
	HomeScreenTypeDefault = 1, HomeScreenTypeCustom = 2, HomeScreenTypeResultSet = 3, HomeScreenTypeFolder = 4
} HomeScreenTypes;

typedef enum _HomeScreenBackgroundTypes{
	HomeScreenBgTypeUnknown = -1, HomeScreenBgTypeFill = 1, HomeScreenBgTypeImage = 2
} HomeScreenBackgroundTypes;

typedef enum _HomeScreenButtonStyles{
	HomeScreenButtonStyleNone = 1, HomeScreenButtonStyleFlat = 2, HomeScreenButtonStyleGlass = 3
} HomeScreenButtonStyles;

typedef enum _IconTypes{
	IconTypeDefault = 1,  IconTypeImage= 2
} IconTypes;

typedef enum _HomeScreenButtonActionTypes{
	HomeScreenBtnActionTypeUnknown = -1, HomeScreenBtnActionTypeFolder = 1, HomeScreenBtnActionTypeResultSet = 2, HomeScreenBtnActionTypeReports = 3, HomeScreenBtnActionTypeSettings = 4, HomeScreenBtnActionTypeSharedLibrary = 5, HomeScreenBtnActionTypeHelp = 6,
/*ASYNCEX:add*/    
     HomeScreenBtnActionTypeAsynch= 8    
    
} HomeScreenButtonActionTypes;

typedef  enum _PreCacheSupportingObjectTypes {
	PreCacheSupportingObjectTypeUnknown = -1, PreCacheSupportingObjectTypeFolder = 1, PreCacheSupportingObjectTypeResultSet = 2
} PreCacheSupportingObjectTypes;

typedef enum _ConfigurationTypes{
	ConfigurationTypeMaster = 1
} ConfigurationType;

typedef enum _HomeScreenTitleBarTypes{
	HomeScreenTitleBarTypeRegular = 1,  HomeScreenTitleBarTypeImage= 2
} HomeScreenTitleBarTypes;

typedef enum _FillTypes {
	FillTypeSolidColor = 0, FillTypeTransparent = 1, FillTypeGradient = 2
} FillTypes;

typedef enum _DeviceTypes{
	DeviceTypeIPhone = 1, DeviceTypeIPad = 2
} DeviceType;

typedef enum _CachePreLoadModes{
	CachePreLoadModeAutomatic = 1, CachePreLoadModeOff = 2
} CachePreLoadModes;

typedef enum _CacheClearModes{
	CacheClearModeAutomatic = 1, CacheClearModeOnClose = 2
} CacheClearModes;

typedef enum _PushNotificationModes{
	PushNotificationOn = 1, PushNotificationOff = 2
} PushNotificationModes;

typedef enum _MobileClientType {
	Reserve = 0, Blackberry = 1, IPhone = 2, IPad = 4
}MobileClientType;

/**
	EventTypes are used in mstr:// and mstripad:// URL's.
	For example, to make the app go Back, this URL would get it done:
		mstr://?evt=3124
	 or
		mstripad://?evt=3124
 */
typedef enum _EventTypes{
	EventTypeFolder = 2001,
    EventTypeWebDavFolder = 2002,
	EventTypeEmailScreen = 3037, 
	EventTypeRequestPDF = 3103,	
	EventTypeBack = 3124, 
	EventTypeAnnotateAndShare = 3175,
    EventTypeSwitchSelectionMode = 3987,
    EventTypeOffline = 3988,
	EventTypeOptionMenu = 3989,
    EventTypeRecentView = 3993,
	EventTypeHelp = 3994, 
	EventTypeHomeScreen = 3995,
	EventTypeSharedLibrary = 3996,
	EventTypeReportList = 3997,   
	EventTypeStatus = 3998,
	EventTypeSettings = 3999,
    EventTypeLogout = 4000
}EventTypes;

typedef enum _ReconcileScopes {
	ReconcileScopeLiveReports = 1,  ReconcileScopeSubscriptions = 2, ReconcileScopeAll = 3, ReconcileScopeCustomize = 4,
} ReconcileScopes;

typedef enum _ReconcileBehavior { // bit or/and
	ReconcileDropInvalid = 0, ReconcileKeepInvalid = 1, ReconcileLightCheck = 2, ReconcileOfflineDisable = 4
} ReconcileBehavior;

typedef	enum _CacheStatus {
	CacheStatusInvalid = 1,
} CacheStatus;

typedef enum _AccessRightFlags {
	AccessRightBrowse = 0x1, AccessRightRead = 0x4, AccessRightWrite = 0x8, AccessRightDelete=0x10, AccessRightControl = 0x20,
	AccessRightUse = 0x40, AccessRightExecute = 0x80, AccessRightFullControl = 0xff

} AccessRightFlags;

typedef enum _UpdateReportDataAction {
	UpdateReportKeepData = 0,
	UpdateReportDeleteData = 1,
	UpdateReportInvalidateData = 2,
    UpdateReportWithoutPersist = 3
} UpdateReportDataAction;

typedef enum _LatencyStatus {
	LatencySevere = 0,
	LatencyBad = 1,
	LatencyOk = 2,
	LatencyUnavailable = 3,
} LatencyStatus;

typedef enum _NetworkStatusLevel {
	NetworkStatusGood = 1,		//network status good
	NetworkStatusFair = 2,		//network status fair
	NetworkStatusPoor = 4,		//network status poor (latency == poor || bandwidth == poor)
	NetworkStatusTooPoor = 8,	//network status too poor (latency == poor && bandwidth == poor)
	NetworkStatusLost = 16,
} NetworkStatusLevel;

typedef enum _NetworkConnectionType {
	NetworkConnectionTypeNotReachable = 0,
	NetworkConnectionTypeWiFi = 1,
	NetworkConnectionType3G = 2,
} NetworkConnectionType;

typedef enum _LatencyRecheckStopType {
	LatencyRecheckStopTypeRequestTimeOut = 0, 
	LatencyRecheckStopTypeReceiveLatencyResponse = 1,
	LatencyRecheckStopTypeReceiveFollowingPacket = 2,
	LatencyRecheckStopTypeUserClose = 3,
} LatencyRecheckStopType;

typedef enum _LatencyCheckType {
	LatencyCheckTypeStartUp = 0,
	LatencyCheckTypeTentative = 1,
	LatencyCheckTypeReCheck = 2,
}LatencyCheckType;

typedef enum _CredentialsInvalidReason {
	CredentialsInvalidReasonUnknown = 0,
	CredentialsInvalidReasonError,
	CredentialsInvalidReasonExpired,
	CredentialsInvalidReasonPasswordMustChange,
	CredentialsInvalidReasonHTMLFormAuthentication,
	CredentialsInvalidReasonPasswordHasExpired,
	CredentialsInvalidReasonUserPrivilegeMissing,
} CredentialsErrorReason;

typedef enum _CredentialsStatus {
	CredentialsValid = 0,
	CredentialsDefaultServerInvalid = 1,
	CredentialsServerInvalid = 2,
	CredentialsDefaultProjectInvalid = 4,
	CredentialsProjectInvalid = 8,
	CredentialsDefaultServerEmpty = 16,
	CredentialsServerEmpty = 32,
	CredentialsDefaultProjectEmpty = 64,
	CredentialsProjectEmpty = 128,
	CredentialsDefaultServerExpired = 256,
	CredentialsServerExpired = 512,
	CredentialsDefaultProjectExpired = 1024,
	CredentialsProjectExpired = 2048,
	CredentialsDefaultProjectPasswordMustChange = 4096,
	CredentialsProjectPasswordMustChange = 8192,
	CredentialsServerHTMLFormAuthentication = 16384,
	CredentialsDefaultProjectPasswordHasExpired = 32768,
	CredentialsProjectPasswordHasExpired = 65536,
	CredentialsDefaultUserPrivilegeMissing = 131072,
	CredentialsUserPrivilegeMissing = 262144,
} CredentialsStatus;

typedef enum _MSTROrientation {
	MSTROrientationNone = 0,
	MSTROrientationPortrait,
	MSTROrientationLandscape,
	MSTROrientationBoth
} MSTROrientation;

typedef enum _NetworkRequestServerType {
	NetworkRequestServerTypeMobileServer = 0, NetworkRequestServerTypeCertificateServer = 1
} NetworkRequestServerType;

//TODO: Remove unused enums
typedef enum _DataControllers{
	EnumInvalidControllerType = -1, EnumRWDataController, EnumGridWidgetDataController, EnumChartWidgetDataController, 
	EnumReportGridController, EnumReportGraphController, EnumMojoGridController, EnumVisualizationController, 
	EnumMapWidgetDataController, EnumRWPDFControllerSinglePage, EnumRWPDFControllerMultiPage, EnumRWMojo
}DataControllers;

typedef enum _WaitViewTypes{ //should technically be in WaitView.h, however, due to iPad we can't do this yet
	EnumBlackScreenWait = 0, EnumFloatingWait, EnumNoWait
} WaitViewTypes;

typedef enum _SettingsLabelType {
	SettingsLabelTypeName = 0,
	SettingsLabelTypeValue,
} SettingsLabelType;

typedef enum _ThemeType{
    THEMETYPE_NOT_ENABLE_TEST_THEME = 0,
	THEMETYPE_BLACK = 1,
    THEMETYPE_WHITE = 2,
    THEMETYPE_UNDEFINED = 3,
}ThemeType;

typedef enum _CredentialsPromptStatus {
	CredentialsPromptWaiting = 0,
	CredentialsPromptAnswered,
	CredentialsPromptCancelled
}CredentialsPromptStatus;

typedef enum _MobileLoginPromptType {
	AuthenticationPromptType = 0,
    ChangePasswordPromptType,
    HTMLFormPromptType,
    CertificatePromptType,
    UserDefinedLoginPromptType,
	UsherQRPromptType,
	OAuthLoginPromptType
}MobileLoginPromptType;

typedef enum _WebServerType {
	WebServerTypeNormal = 0,
} WebServerType;


typedef enum _RotationStaus {
	RotationStausNone = -1,
	RotationStausStarted,
	RotationStausEnded
} RotationStaus;

typedef enum _ProgressBarPosition {
	ProgressBarPositionLeft = 1,
	ProgressBarPositionRight = 2,
} ProgressBarPosition;

typedef enum _SecurityFramework {
	SecurityFrameworkUnknown = 0,
	SecurityFrameworkDefault,
	SecurityFrameworkGoodDynamics,
	SecurityFrameworkFacebook,
    SecurityFrameworkAppConnect,
    SecurityFrameworkAirWatch,
    SecurityFrameworkACE
}SecurityFramework;

typedef enum _MSTRFeature {
    MSTRFeatureDisablePreventDataLeakage,
    MSTRFeatureScreenshot,
    MSTRFeatureEmail,
    MSTRFeatureAnnotation,
    MSTRFeatureHttpFileStream,
    MSTRFeatureSendFile,
    MSTRFeatureDocumentInteraction,
    MSTRFeatureCollaboration,
    MSTRFeatureUsherAuthentication,
    MSTRFeaturePrint,
    MSTRFeatureCopyPaste,
    MSTRFeatureCameraAccess,
    MSTRFeatureLocationServices,
    MSTRFeatureCertificateServer,
    MSTRFeatureSaveToPhotos,
    MSTRFeatureExportExcel
}MSTRFeature;

typedef enum {
    kIPadFolderItemBinaryStatusNone = -1,
    kIPadFolderItemBinaryStatusNoBinary,
    kIPadFolderItemBinaryStatusPrepareDownloading,
    kIPadFolderItemBinaryStatusDownloading,
    kIPadFolderItemBinaryStatusDownloadPaused,
    kIPadFolderItemBinaryStatusDownloadFailed,
    kIPadFolderItemBinaryStatusReady,
    kIPadFolderItemBinaryStatusUnsupported,          // For unsupported webdav file
    kIPadFolderItemBinaryStatusUnsupportedByServer,
    kIPadFolderItemBinaryStatusSizeOverLimit,
} IPadFolderItemBinaryStatus;

typedef enum
{
	ClientRequestExecutionHandlerReserved		= 0x00000000,   //Initial value
    ClientRequestExecutionHandlerLiveDocument	= 0x00000001,   //Live document's first slice (before rendering)
    ClientRequestExecutionHandlerOtherLayout    = 0x00000002,   //Live document's first layout and other layouts (after rendering)
    ClientRequestExecutionHandlerManipulation	= 0x00000004,   //Live document manipulations
	ClientRequestExecutionHandlerPrecache       = 0x00000008,   //Preload, precache
}ClientRequestExecutionHandlerFlags;

typedef enum _EnumBinaryDownloadMode{
    EnumBinaryDownloadFull                      = 0x00000000,
	EnumBinaryDownloadIncrementalDataOnly       = 0x00000001,
	EnumBinaryDownloadIncrementalAll            = 0x00000002,
    EnumBinaryDownloadSeparatlyMask             = 0x10000000,       //mask for separate binary format
    EnumBinaryDownloadIncrementalDefAndDataSeparatly = 0x10000004,  //def-data
    EnumBinaryDownloadCSI                       = 0x10000008,       //smart client CSI
    EnumBinaryDownloadAllDataSet                = 0x10000010,       //smart client all datasets
    EnumBinaryDownloadAllBinaryParts            = 0x10000020,       //smart client subscription / preload
} EnumBinaryDownloadMode;

typedef enum _EnumSeparateBinaryChunkType{
    EnumBinaryChunkTypeReserved         = 0,
    EnumBinaryChunkTypeDefinition       = 1,
    EnumBinaryChunkTypeInitSliceData    = 2,
    EnumBinaryChunkTypeInitLayoutData   = 3,
    EnumBinaryChunkTypeOtherLayoutData  = 4,
    EnumBinaryChunkTypeAllLayoutsCSI    = 5,
    EnumBinaryChunkTypeDataset          = 6,
    EnumBinaryChunkTypeImage            = 7,
	EnumBinaryChunkTypeWidget            = 8,
} EnumSeparateBinaryChunkType;

// Used for Client Statistics
typedef enum _MobileOS {
    MobileOSReserved = 0,
    MobileOSiOS = 1
} MobileOS;

typedef enum _ExecutionType {
    ExecutionReserved = 0,
    ExecutionUserReq = 1,
    ExecutionPreCache = 2,
    ExecutionAppRecovery = 3,
    ExecutionSubscriptionCache = 4,
    ExecutionTransactionSubAction = 5,
    ExecutionReportQueue = 6,
    ExecutionReportQueueRecall = 7,
    ExecutionBackButton = 8
} ExecutionType;

typedef enum _ManipulationStatsType {
	ManipulationStatsReserved = 0,
	ManipulationStatsSelector = 1,
	ManipulationStatsPanelSelector = 2,
	ManipulationStatsActionSelector = 3,
	ManipulationStatsChangeLayout = 4,
	ManipulationStatsChangeView = 5,
	ManipulationStatsSort = 6,
	ManipulationStatsPageBy = 7,
	ManipulationStatsInfoWindow = 8,
	ManipulationStatsAnnotation = 9,
	ManipulationStatsEmailScreenShot = 10,
	ManipulationStatsWidgetVideoPlay = 11,
	ManipulationStatsWidgetVideoPause = 12,
	ManipulationStatsWidgetMultimediaDownload = 20,
	ManipulationStatsWidgetMultimediaOpen = 21,
    ManipulationStatsMacro = 99
} ManipulationStatsType;

typedef enum _CacheType {
    CacheTypeReserved = 0,
    CacheTypeNone = 1,
    CacheTypeDevice = 2,
    CacheTypeServer = 3,
    CacheTypeSubscription = 4,
    CacheTypeReportQueue = 5,
    CacheTypeAppMemory = 6
} CacheType;

typedef enum _NetworkType {
    NetworkTypeReserved = 0x00000000,
    NetworkTypeiOSNone = 0x01000000,
    NetworkTypeiOSWiFi = 0x01000001,
    NetworkTypeiOSWWAN = 0x01000002
} NetworkType;

typedef enum _StatsFormat {
    StatsFormatReserved = 0,
    StatsFormatXML = 1,
    StatsFormatDefault = StatsFormatXML
} StatsFormat;

typedef enum _CarryOverSelectedMode {
    CarryOverReserved = 0,
    CarryOverMatchbyID = 1,
    CarryOverMatchbyName = 2,
} CarryOverSelectedMode;

typedef enum _CustomizedNetworkErrors {
	WebAuthenticationError = -2000001,
	MissingCertificateError = -2000002,
	InvalidCertificateError = -2000003,
	SSOFormAuthenticationError = -2000004,
	UsherAuthenticationError = - 2000005,
	UserNotFoundError = -2147216959,
	NotAllowStandardAuthentication = -2147213792,
	UserDisabledError = -2147216965,
	ProjectLDAPPrivilegeError = -2147214578,//If either login or password is empty, LDAP will treat it as anonymous login, we may get a privilege error for this
	UserPrivilegeMissingError = -2147205002,//This is the SDK_E_PRIVILEGE_MISSING error if user doesn't have the privilege DssXmlPrivilegeUseMstrMobile
	PasswordMustChange = -2147216960,//You must supply a new password
//shliu us51426: remove facebook auth which is not supported, to clear warning
//	FacebookAuthenticationError = -2147483648,//Facebook can't authenticate and authorize this user
	WebDavResourceNotFound = -2147216380, //Web DAV folder browsing fail
    SearchEngineNotStarted = -2147212073,
    ProjectSearchDisabled  = -2147212154,
	PasswordHasExpired = -2147216963, //Password has expired.
	UserNotLinkedError = -2147212488, //This user is not linked to any MicroStrategy user.
	NetworkIsNotReachableError = -3000001,
	ApplicationOfflineError = -3000002,
	KeychainNotAvailableCertificateError = -3000003
} CustomizedNetworkErrors;

typedef enum _EventHandlingResult {
	EventHandlingResultNormal = 0,
	EventHandlingResultSubtreeRecreated = 1
} EventHandlingResult;

typedef enum _SecurityPolicyType {
	SecurityPolicyTypeDefault = 0,
	SecurityPolicyTypeDPW,
    SecurityPolicyTypeTouch
}SecurityPolicyType;

typedef enum _EncryptionMode {
    EncryptionModeSoftware = 0,
    EncryptionModeHardware,
    EncryptionModeSoftwareAndHardware
} EncryptionMode;

typedef enum _MSTRApplicationState {
    MSTRApplicationStateLaunching = 0,
    MSTRApplicationStateWillEnterForeground,
    MSTRApplicationStateDidEnterBackground,
    MSTRApplicationStateDidBecomeActive,
    MSTRApplicationStateWillResignActive
    
} MSTRApplicationState;

typedef enum _SubscriptionEditModes {
    SubscriptionEditModePersonalization = 0x1,
    SubscriptionEditModeDeliverySettings = 0x2,
    SubscriptionEditModeAll = SubscriptionEditModePersonalization | SubscriptionEditModeDeliverySettings
} SubscriptionEditModes;

typedef enum _EnumSubscriptionEventCodes {
    EnumSubscriptionEventCodeAnswerAllPromptsAndSubscribe = 8001,
    EnumSubscriptionEventCodeAnswerIndividualPrompt = 4098001,
    EnumSubscriptionEventCodeSaveSubscription = 256019,
    EnumSubscriptionEventCodeSetSubscriptionProperties = 256020
} EnumSubscriptionEventCodes;

typedef enum _EnumWebReportExecutionModes {
    WebReportExecutionModeDefault = 1,
    WebReportExecutionModePDF,
    WebReportExecutionModeExcel,
    WebReportExecutionModePlainText,
    WebReportExecutionModeFlash
} EnumWebReportExecutionModes;

typedef enum _EnumExcelVersion {
    ExcelVersionUnknown = -1,
    ExcelVersion2000SR1 = 1,
    ExcelVersionXP,
    ExcelVersionXpDup,
    ExcelVersion2007,
    ExcelVersion2007Dup
} EnumExcelVersion;


typedef enum _EnumExportExcelResultType {
    ExportExcelResultTypeDone,
    ExportExcelResultTypeAlreadyExported,
} EnumExportExcelResultType;

typedef enum _EnumRAFilterViewStyle{
    RAFilterViewStyleVI,
    RAFilterViewStyleDossier
}EnumRAFilterViewStyle;

#endif // __MSTR_ENUMS__

