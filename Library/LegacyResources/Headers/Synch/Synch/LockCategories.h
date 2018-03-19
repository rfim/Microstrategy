//==============================================================================================
//	FILENAME	:	MSILockCategories.h
//	AUTHOR		:	Nick Pratt
//	CREATION	:	03/01/1999
//	Copyright (C) MicroStrategy Incorporated 1999
//==============================================================================================
#ifndef MSynch_Lock_Categories_h
#define MSynch_Lock_Categories_h
//
// RRRRR   EEEEEE  AAAAA  DDDD     MMM     MMM EEEEEE
// RR  RR  EEEEEE AA   AA DD DD    MMMM   MMMM EEEEEE
// RR  RR  EE     AA   AA DD  D    MM MM MM MM EE
// RRRRRR  EEEEEE AAAAAAA DD  DD   MM  MMM  MM EEEEEE
// RR RR   EE     AA   AA DD  D    MM       MM EE
// RR  RR  EEEEEE AA   AA DD D     MM       MM EEEEEE
// RR   RR EEEEEE AA   AA DDD      MM       MM EEEEEE

// *****************************************************
// *****************************************************
// *****************************************************
// THE VALUES OF THE ENUMERATION SHOULD NEVER BE CHANGED
// IF YOU NEED TO ADD NEW CATEGORIES, ADD THEM AT THE
// END OF THE ENUMERATION. NEVER, EVER, EVER, UNTIL THE
// END OF TIME ADD THEM IN THE MIDDLE, EVEN IF THERE ARE
// UNUSED VALUES
// *****************************************************
// *****************************************************
// *****************************************************

namespace MSynch
{
	typedef enum EnumDSSLockStackCategory
	{
		DssLkStkCatIgnoreCallStack			= -4, // ** OBSOLETE OBSOLETE NEVER USE THIS CATEGORY OR WE'LL IMPROVE YOUR VOCABULARY (WE WILL EXPLAIN TO YOU A NEW MEANING FOR "PAIN")
		DssLkStkCatNotNestingLock			= -3, // Use this category if your lock is never held while calling another lock	
		DssLkStkCatNoLock					= -2, // *** NEVER USE THIS TO INIT LOCK *** used by CComObjectRootEx to identify a multi-threaded object that does not need its own lock
		DssLkStkCatUnset					= -1, // ** NEVER EVER USE THIS **
		DssLkStkCatThread					=  0, // 0
		DssLkStkCatThreadPool,				      // 1
		DssLkStkCatThreadMgr =2,			      // 2
		//DssLkStkCatJobManager,				  // 3 REMOVED:npratt:102999
		DssLkStkCatJob = 4,					      // 4
		DssLkStkCatJobExec,					      // 5
		DssLkStkCatUserManager,				      // 6
		DssLkStkCatUserRunTimeStruct,		      // 7
		DssLkStkCatDSSUserStruct,			      // 8
		DssLkStkCatConfigManager =9,			  // 9
		//DssLkStkCatProject,					  //10 REMOVED:npratt:102999
		DssLkStkCatProjectInstance = 11,  		  //11
		DssLkStkCatTransactionLog,			      //12
		DssLkStkCatDisplayControllerBase,	   	  //13
		DssLkStkCatTimerTaskList,			      //14
		DssLkStkCatTimerList,				      //15
		DssLkStkCatDBMgrConn,				      //16
		DssLkStkCatDBMgrDatabase = 17,		      //17
		//DssLkStkCatDBMgrRoleTable,			  //18 REMOVED:npratt:102999
		//DssLkStkCatDBMgrPhyConnTable,		      //19 REMOVED:npratt:102999
		DssLkStkCatDBCMgrProvider =20,		      //20
		//DssLkStkCatNetConnection,			      //21 REMOVED:npratt:102999
		//DssLkStkCatChannel,					  //22 REMOVED:npratt:102999
		DssLkStkCatServiceMgr = 23,			      //23
		DssLkStkCatServMgrProjectLock,		   	  //24 inside Service Manager
		DssLkStkCatScheduler,				      //25
		DssLkStkCatDependencyMgr,			      //26
		DssLkStkCatNamedIdsList,			      //27
		DssLkStkCatPURestructure,			      //28
		DssLkStkCatStatTable,				      //29
		DssLkStkCatStatDataTable,			      //30
		DssLkStkCatNetSocketClient,			      //31
		DssLkStkCatConnectSerializer,		      //32
		DssLkStkCatRequestMgr,				      //33
		//DssLkStkCatNetServAccept = 34,		  //34 REMOVED:yxiao:102802
		//DssLkStkCatQNext,					      //35 REMOVED:npratt:102999
		DssLkStkCatQList = 36,				      //36 From MSIBasicQ
		DssLkStkCatFreeList,				      //37 DbConnMgr
		DssLkStkCatUsedList,				      //38 DbConnMgr
		DssLkStkCatDBMgrPhyConn,			      //39
		DssLkStkCatJobSchemeObjMap,			      //40 In MSIPU.h
		DssLkStkCatJobSchemeObjWH,			      //41 In MSIPI.h

		DssLkStkCatClassFactory,			      //42 in CComClassFactory (ATL)
		DssLkStkCatAggObject,				      //43 in CComAggObject (ATL class)

		DssLkStkCatObjectServer,			      //44 in CDSSObjectServer

		// We use DssLkStkCatObjectServer + <DSS Type> for each type of DSS Object
		// BEWARE - We suppose that there are 64 types of objects
		// BEWARE - We fix the number so we don't have to keep reassigning enums

		DssLkStkCatTypeFilter,				      //45
		DssLkStkCatTypeTemplate,
		DssLkStkCatTypeReportDefinition,
		DssLkStkCatTypeMetric,
		DssLkStkCatTypeReserved2,
		DssLkStkCatTypeAgent,				      //50
		DssLkStkCatTypeAggMetric,
		DssLkStkCatTypeFolder,
		DssLkStkCatTypeOwner,
		DssLkStkCatTypePrompt,
		DssLkStkCatTypeFunction,			      //55
		DssLkStkCatTypeAttribute,
		DssLkStkCatTypeFact,
		DssLkStkCatTypeDimension,
		DssLkStkCatTypeTable,
		DssLkStkCatTypeView,				      //60
		DssLkStkCatTypeFactGroup,
		DssLkStkCatTypeShortcut,
		DssLkStkCatTypeResolution,
		DssLkStkCatTypeDriver,
		DssLkStkCatTypeAttributeForm,		      //65
		DssLkStkCatTypeSchema,
		DssLkStkCatTypeFindObject,
		DssLkStkCatTypeCatalog,
		DssLkStkCatTypeCatalogDefn,
		DssLkStkCatTypeColumn,				      //70
		DssLkStkCatTypePropertyGroup,
		DssLkStkCatTypePropertySet,

		// First Class Objects of Resource Server
		DssLkStkCatTypeDBRole,
		DssLkStkCatTypeDBLogin,
		DssLkStkCatTypeDBConnection,		      //75
		DssLkStkCatTypeProject,
		DssLkStkCatTypeServerDef,
		DssLkStkCatTypeUser,
		DssLkStkCatTypeUserGroup,
		DssLkStkCatTypeConfiguration,		      //80

		// First Class Objects added for Request Objects
		DssLkStkCatTypeRequest,
		DssLkStkCatTypeScript,
		DssLkStkCatTypeSearch,

		DssLkStkCatTypeSearchFolder,
		DssLkStkCatTypeFunction2,			      //85

		DssLkStkCatTypeFunctionPackageDefinition,

		DssLkStkCatTypeRole,
		DssLkStkCatTypeSecurityRole,

		DssLkStkCatTypeInBox,
		DssLkStkCatTypeInBoxMsg,			      //90

		DssLkStkCatTypeConsolidation,
		DssLkStkCatTypeConsolidationElement,

		// First Class Objects added for Scheduler Objects
		DssLkStkCatTypeScheduleEvent,
		DssLkStkCatTypeScheduleObject,
		DssLkStkCatTypeScheduleTrigger,		   	  //95

		DssLkStkCatTypeLink,
		DssLkStkCatTypeDbTable,
		DssLkStkCatTypeTableSource,

		// Document Definition
		DssLkStkCatTypeDocumentDefinition,
		DssLkStkCatTypeDrillMap,			      //100

		// First class DBMS object
		DssLkStkCatTypeDBMS,				      //101

		// Resume the enumeration after leaving space for 64 lock categories
		DssLkStkCatObjectServerCache = DssLkStkCatObjectServer + 64,// 44+64 = 108
											         				//    in CTable
		DssLkStkCatObjectCollection,			  //    in CDSSObjectCollection
		DssLkStkCatErrorInfo,					  //110 in CDSSErrorInfo
		DssLkStkCatObjectServerErrors,			  //    in CDSSErrors
		DssLkStkCatBaseAttributeForms,			  //    in CDSSBaseAttributeForms
		DssLkStkCatAttributeForms,				  //    in CDSSAttributeForms
		DssLkStkCatObjectServerAttributes,		  //    in CDSSAttributes
		DssLkStkCatAttributeFromDimension,		  //115 in CDSSAttrFromDimension
		DssLkStkCatAttributeRelationship,		  //
		DssLkStkCatAttributeRelationships,		  //
		DssLkStkCatDimensionElementFilters,		  //
		DssLkStkCatLeafElements,				  //    in CDSSLeafElements
		DssLkStkCatLeafNodes,					  //120 in CDSSLeafNodes
		DssLkStkCatElementsCollection,			  //    in CDSSElementsCollection
		DssLkStkCatObjectServerComment,			  //    in CDSSCommentsEnum
		DssLkStkCatLeafObjectServerElementsP=123, //    in CDSSElementsProxy
		//DssLkStkCatObjectServerExtType,		  // REMOVED:npratt:102999   
		DssLkStkCatObjectServerLinkItem=125,	  //125 in CDSSLinkItem
		DssLkStkCatObjectServerLinkItems,		  //    in CDSSLinkItems
		DssLkStkCatObjectServerCollection,		  //    in CDSSCollection
		DssLkStkCatMetadataServer,				  //    in CMetadataServer
		DssLkStkCatMD4Server,					  //    in CMD4Server
		DssLkStkCatSourceNetClient,				  //130 in CDSSSourceClient
		DssLkStkCatSourceNetServer,				  //    in CDSSSourceSvr
		DssLkStkCatCatalogServer,				  //    in CDSSCatalogServer
		DssLkStkCatReportServer,				  //    in CDSSReportServer
		DssLkStkCatReportInstance,				  //    in CDSSReportInstance
		DssLkStkCatBindingTable,				  //135 in CDSSBindingTable
		DssLkStkCatReportViews,					  //    in CDSSReportViews
		DssLkStkCatReportViewResult,			  //    in CDSSViewResult
		//DssLkStkCatReportServerCachedReport=138,  //    in CDSSCachedReport
		//DssLkStkCatReportServerDataObject,	  // REMOVED:npratt:102999
		DssLkStkCatReportServerDimensionMap=140,  //140 in CDSSDimensionMap
		DssLkStkCatReportServerPropertyLookup,	  //    in CDSSPropertyLookup
		DssLkStkCatReportServerSetLookup,		  //    in CDSSPropertySetLookup
		//DssLkStkCatReportCacheHashTableItem,	  //
		DssLkStkCatReportCacheManager=144,			  //    in CDSSReportCacheManager
		//DssLkStkCatReportCacheListKey,			  //145 in CDSSReportCacheListKey
		DssLkStkCatResolutionServer = 146,			  //    in CDSSResolutionServer
		DssLkStkCatResWorkspace,				  //    in CDSSResWorkspace
		DssLkStkCatReportNetClient,				  //    in CDSSRptNetClient
		DssLkStkCatReportExecutor,				  //    in CDSSReportExec
		DssLkStkCatReportExecutorContext,		  //150 CDSSExecProcessContext
		DssLkStkCatReportNetServer,				  //    in CDSSRptSrcSvr
		DssLkStkCatElementServer = 152,			  //    in CDSSElementServer
		//DssLkStkCatElementServerTableLRULock,	  // REMOVED:npratt:102999
		DssLkStkCatElementServerTableLocks = 154, //    in CAttributeTable,
    											  //       CElementsTable,
												  //       CElementTable
		DssLkStkCatElementServerElement,		  //155 CDSSElement
		DssLkStkCatElementServerElements,		  //    in CDSSElements
		DssLkStkCatElementServerElementTime,	  //    in CDSSElementTime
		DssLkStkCatEnumTemplate,				  //    in CDSSEnumTemplate,
    											  //       CDSSBindingTableEnum
		DssLkStkCatDBElementServer,				  //    in CDBElementServer
		DssLkStkCatElementNetClient,			  //160 CDSSElementNetClient
		DssLkStkCatElementNetServer,			  //    in CDSSElementNetServer
		DssLkStkCatAuthServer,					  //    in CDSSAuthServer
		DssLkStkCatClientConnectionDataSource,	  //    in CDSSDataSource
		DssLkStkCatClientConnectionDataSourceE,   //    in CDSSDataSourceEnumerator
		DssLkStkCatClientConnectionSession,		  //165 CDSSSession
		DssLkStkCatClientServices,				  //    in CDSSClientServices
		DssLkStkCatProjectCreator,				  //    in CProjectCreator
		DssLkStkCatProjectCreatorScript,		  //    in CDSSScriptProcessor
		DssLkStkCatUserRuntime,					  //    in CDSSUserRuntime
		DssLkStkCatLRULock,						  //170 CDSSTable (LRULock)
		DssLkStkCatSessionUserRuntime,			  //    in CDSSSession(mUserRuntime)
		DssLkStkCatParserMD4,                  	  //    in CDSSParser
		DssLkStkCatParserXgi,                     //    in CDSSParserXgi
		DssLkStkCatParserXgiToken,                //    in CDSSParserXgiToken
		DssLkStkCatParserXgiTokenStream,          //175 CDSSParserXgiTokenStream
		DssLkStkCatCComObjectCached,			  //    in CComObjectCached (ATL)
		//DssLkStkCatReportCacheListMapKey,		  //    UNUSED - xtian 08181999
		DssLkStkCatClusterLKUPTableMapItem = 178,		  //
		DssLkStkCatContentServer,				  //    in CDSSContentServer
		DssLkStkCatEmbeddedFolder,				  //180 in CDSSEmbeddedFolder
		//DssLkStkCatReportCacheL1MapKey,			  //    in CDSSReportCacheL1MapKey,
		//DssLkStkCatReportCacheL2MapKey,			  //    in CDSSReportCacheL2MapKey,
		//DssLkStkCatReportCacheL3MapKey,			  //    in CDSSReportCacheL3MapKey,
		DssLkStkCatErrorState = 184,				  //    in RptError
		DssLkStkCatAttributeTableLRULock,		  //185 in CAttributeTable
		DssLkStkCatDSSTime,						  // in CDSSTime,	jgoteti 092399
		DssLkStkCatDSSTimePeriod,				  // in CDSSTime,	jgoteti 092399
		DssLkStkCatMDObjectCache,				  // in CMDAccess, xue 9/27/99
		DssLkStkCatCDSSServerMessage,			  // in CDSSServerMessage
		DssLkStkCatCacheReport2,				  //190
		DssLkStkCatResWorkSpaceLite,			  // Wrong spelling of lite though..
		DssLkStkCatDrillElements,
		DssLkStkCatExpr,
		DssLkStkCatCCollection,				      // Ozgur added this one
		DssLkStkCatReportCacheLRUList,			  // 195;npratt;110899
		DssLkStkCatElementServerTable1Locks,	  // 196;npratt;110899
		DssLkStkCatElementServerTable2Locks,	  // 197;npratt;110899
		DssLkStkCatElementServerTable3Locks,	  // 198;npratt;110899
		DssLkStkCatViewSettings,				  // 199;npratt;111299;For PX.
		// This lock category is used for embedded objects, leave 64 space
		DssLkStkCatEmbeddedObject = 200,		  // in CDSSObjectServer

		// Synch Logger is not checked for LOV for possible infinite loop
		// Categories [201,258] is skipped by LockStack as far as LOV
		DssLkStkCatSynchLogger = 201,

		// leave space for 64 types of objects
		DssLkStkCatWorkingSet		= 265,			// working set 

		//
		// Kernel categories continue here upto 349
		//
		DssLkStkCatProcessContextLock = 300,	  //
		DssLkStkCatUserInbox,					  //
		DssLkStkCatDBMgrConnCount,				  //    in DBConnMgr
		DssLkStkCatDBMgrCacheFlushFlag,			  //    in DCConnMgr
		DssLkStkCatServerSession,				  //    server session in Config
		DssLkStkCatJobPrioritySchemeLock,		  //305 in ConfigManager
		DssLkStkCatDiagLogFlag,					  //    in Session manager for
    											  // protection around set/get
												  // diagnostics log flags
		DssLkStkCatGlobalResource,				  //  in Data Source
		DssLkStkCatGlobalResourceCreation,		  //  in Data Source
		DssLkStkCatGlobalResourceTable, 		  //  in Data Source
		DssLkStkCatXMLAPIClusterNode,			  //310 Ding 8/24/99: XML API
		DssLkStkCatXMLAPIAdmin,					  //    Ding 8/24/99: XML API
		//DssLkStkCatCachedReport,				  //    in CDSSCachedReport
		DssLkStkCatMSIHashBucket = 313,			  //    in MSIHashTable
		DssLkStkCatMyHashTable,					  //    in MSIHashTable
		DssLkStkCatConsoleChannel,				  //315 in ConsoleLoop
		DssLkStkCatBackupChannel,				  //    in DSSBackup.h
		DssLkStkCatMSIRequest,					  //    in MSIRequest,
		DssLkStkCatRepSrvSinkMsh,				  //
		DssLkStkCatNetHttpSinkMsh,				  //
		DssLkStkCatSrvNetSinkMThread,			  //320 in CDSSServerNetSinkMThread
		DssLkStkCatSrvNetSinkMsh,				  //
		DssLkStkCatSrvNetSink,					  //    in CDSSServerNetSink
		DssLkStkCatCDSSMsg,						  //    in CDSSMsg
		DssLkStkCatODBCConnection,				  //    in MSIDbOdbcConnection
		DssLkStkCatODBCTraceConnection,			  //325 in MSIDbOdbcConnection
		DssLkStkCatODBCResult,					  //    in MsiDbOdbcResult
		DssLkStkCatODBCSqlStmt,					  //    in MsiDbOdbcSqlStmt
		DssLkStkCatHashBucket,					  //    in Job Managers HashTable..
		DssLkStkCatMSIHashTable,				  //    in MSIHashTable(Job Manager)
		DssLkStkCatNetConnectionSend,			  // 
		DssLkStkCatNetConnectionReceive,		  // 
		DssLkStkCatNetReactor,					  // 
		DssLkStkCatNetServerI,					  // 
		DssLkStkCatCCluster = 334,				  //    in CCluster
		DssLkStkCatCClusters,					  //335 in CClusters
		DssLkStkCatCConnectionPool,				  //    in CConnectionPool
		DssLkStkCatCServer,						  //    in MSIXML\Server.cpp
		DssLkStkCatClusterManager,				  //    in MSIClusterManager
		DssLkStkCatDBMgrRole,					  //    in DMManagerRole
		DssLkStkCatScriptServer,				  //340 in CDSSScriptServer
		DssLkStkCatScriptServerCleanUp,			  //
		DssLkStkCatScriptEngine,				  //    in CDSSScriptEngine
		DssLkStkCatScriptEngineCleanUp,			  //
		DssLkStkCatRequestInstance,				  //    in CDSSRequestInstance
		DssLkStkCatRoleMembership,				  //345 in CDSSRoleMembership
		DssLkStkCatAuthServerCleanUp,			  //
		DssLkStkCatDocumentInstance,			  // in CDSSDocumentInstance
		DssLkStkCatDocumentServer,                //
		DssLkStkCatDocNetClient,                  //


		//
		// Lock Categories for Engine
		//
		DssLkStkCatSQLEngineDefault = 350,		  //350 beginning for SQL Engine

		DssLkStkCatSQLEngineServer,				  // in CDSSSQLEngineServer
		DssLkStkCatQueryEngineServer,			  // in CDSSQueryEngineServer
		DssLkStkCatQueryEngine,					  // in CDSSQueryEngine
		DssLkStkCatCDSSSQL,						  // in CDSSSQL
		DssLkStkCatCDSSSQLs,					  // in CDSSSQLs
		DssLkStkCatCDSSSQLsServer,				  // in CDSSSQLsServer
		DssLkStkCatCDSSTypeMapping,				  // in CDSSTypeMapping
		DssLkStkCatCDSSSyntaxServer,			  // in CDSSSyntaxServer
		DssLkStkCatVLDBDriverMgr,				  // in VLDBDriverManager
		DssLkStkCatSQLEngineServerMutex,		  // in CDSSSQLEngineServer
		DssLkStkCatEngineLite,					  // in CDSSEngineLite
		DssLkStkCatAEEngine,					  // in CDSSAnalyticalEngine	

		//
		// Kernel continues from 400 upto and including 450
		//

		DssLkStkCatMyMSIHashBucket = 400,		  // in MSIHashTable - bucket lock
		DssLkStkCatCDSSDatabaseManager,	  	  	  // in CDSSDatabaseManager
		DssLkStkCatCDSSGeneralCacheInfo,		  // in Report Server
		DssLkStkCatCDSSReportCacheInfo,			  //
		DssLkStkCatCDSSReportCacheInfos,		  //
		DssLkStkCatDocSrcSvr,					  //405
		DssLkStkCatCDSSNetHttp,					  //
		DssLkStkCatCDSSNetSocket = 407,			  //
		//DssLkStkCatDiagDebugMonitor,			  //REMOVED:npratt:102599
		DssLkStkCatDiagModule = 409,			  //
		DssLkStkCatDiagModules,					  //410
		DssLkStkCatDBAdmin,
		DssLkStkCatCDSSClusterManager,
		DssLkStkCatCDSSClusterMember,
		DssLkStkCatCDSSJob,
		DssLkStkCatCDSSServerMessages,			  //415
		DssLkStkCatCDSSCacheAdministrator,
		DssLkStkCatCDSSJobManager,
		DssLkStkCatCDSSServerAdmin,
		DssLkStkCatCDSSStatistics,
		DssLkStkCatCDSSUserSessions,			  //420
		DssLkStkCatCDSSDBRoleMonitor,
		DssLkStkCatCDSSDBRoleMonitors,
		DssLkStkCatCDSSProjectInstance,
		DssLkStkCatCDSSProjectInstances,
		DssLkStkCatCDSSDatabaseMonitor,			  //425
		DssLkStkCatCDSSDatabaseMonitors,
		DssLkStkCatCDSSDBMonitor,
		DssLkStkCatCDSSDBMonitors,
		DssLkStkCatCDSSScheduleAdministrator,
		DssLkStkCatCDSSScheduleInstance,		  //430
		DssLkStkCatCDSSScheduleInstances,
		DssLkStkCatCDSSServerMemAdmin,
		DssLkStkCatWebServerSharedConnection,
		DssLkStkCatCDSSWebServerAdmin,			  //
		DssLkStkCatAccessTokenMDFilter,			  //435 for MD Security Filter load


		//
		// Kernel continues from 450 upto and including 500
		//

		DssLkStkCatHashBucketDatabase = 450,	  //450 in DSSDatabaseManager
		DssLkStkCatHashBucketDBRole, 		      // in DSSDatabaseManager
		DssLkStkCatHashBucketDBConn,		      // in DSSDatabaseManager
		DssLkStkCatHashBucketDBPhyConn, 	      // in DSSDatabaseManager
		DssLkStkCatHashBucketUser, 		          // in MsiUserSessionManager
		DssLkStkCatHashBucketUserSession,         //455 in MsiUserSessionManager
		DssLkStkCatHashBucketJob, 		  	      // in MsiJobManager
		DssLkStkCatHashBucketStatsRow, 	          // in MsiStatsManager
		DssLkStkCatHashBucketRequest, 	  		  // in MsiRequestManager
		DssLkStkCatHashBucketDbDriverInfo,  	  // in MsiDbcProvider
		DssLkStkCatHashBucketReportCache,   	  //460 in Report Cache Manager
		DssLkStkCatHashBucketReportCacheItem,     // in ReportServer Cache Manager

		DssLkStkCatDbConnectionMap,
		DssLkStkCatDbConnectionManager,
		// pxu; 2/29/00; add lock on UserProjectStruct to protect the jobList which is a non-thread-safe map
		DssLkStkCatUserProjectStruct = 464,		  //464

		// jmuraira:m8comutl breakdown
//		DssLkStkCatMSINumberFormat,
//		DssLkStkCatMSILogRouter,
//		DssLkStkCatMSILog,
//		DssLkStkCatTagMsiLogShareStruct,
//		DssLkStkCatMSILogInit,
//		DssLkStkCatMSILogRouterRegistrar,
//		DssLkStkCatMSIMessageMap,
		DssLkStkCatMSITimerBaseEntryList = 472,
		DssLkStkCatMSIPerfObject,
		DssLkStkCatDFCLog,
		DssLkStkCatCDSSResultSet,
		DssLkStkCatRegistry,

		DssLkStkCatFunctionServer,	// 477

		DssLkStkCatProjectData,

//		DssLkStkCatLoggerDLLCache,	// 479
//		DssLkStkCatLoggerFile,
//		DssLkStkCatLoggerMessageRouter = 481,
//		DssLkStkCatLoggerMessageLoader, // 482

		DssLkStkCatCoDbConnectionManager = 483,		//483  COM wrapper of the connection manager
		DssLkStkCatDbConnectionManagerSingleton,	//484  lock for instantiating and deleting conn manager singleton

		DssLkStkCatDbConnectionMaps,				//485  lock for connection maps collection
		DssLkStkCatReportServices,					//486 in CDSSReportServices

		DssLkStkCatETLServer,						//487 for ETL server
		DssLkStkCatETLManager,						//488 for ETL manager

		DssLkStkCatContractManager,					//489 for memory contract manager

		DssLkStkCatReportInstanceSubset,			//490

		DssLkStkCatWSResultPool,					// 491
		DssLkStkCatWSResult,						// 492
		DssLkStkCatWSResultPoolRSM,					// 493
		DssLkStkCatWSResultPoolBucket,				// 494

		DssLkStkCatMiniCOMLibrary = 495,			//495 for MiniCOM
		DssLkStkCatTimer = 501,						//501 Timer

		DssLkStkCatMessage = 502,					//for the new messages
		DssLkStkCatDSSMessage = 503,					//for CDSSMessage

		DssLkStkCatCubeManager = 504,
		DssLkStkCatCubeInfo = 505,
		DssLkStkCatCubeInfos = 506,
		DssLkStkCatHashBucketCubePool = 507,
		DssLkStkCatHashBucketCubeLKUP = 508,
		DssLkStkCatCubeServer	= 509,
		DssLkStkCatCDSSCubeAdmin	= 510,
		DssLkStkCatCubeNetClient	= 511,
		DssLkStkCatCubeNetServer	= 512,
		DssLkStkCatCubeCluster		= 513,
		 
		DssLkStkCatContentService = 514,
		DssLkStkCatHLContentService,
		DssLkStkCatContentDbRepository,
		DssLkStkCatContentInfoCollection,
		DssLkStkCatContentCollection,
		DssLkStkCatContentInfo,
		DssLkStkCatContent,
		DssLkStkCatBinaryData,

		DssLkStkCatNCSContentService,			// 522 for NCS Content Service
		
		// NCS Objects locks
		DssLkStkCatNCSObjectPersonalization, // this lock will be used below NCS Instance object's lock
		DssLkStkCatNCSObject,
		DssLkStkCatNCSObjectCollection,

		DssLkStkCatNCSEngine,

		DssLkStkCatRWDCache,
		DssLkStkCatRWDCacheInfos,
		DssLkStkCatHashBucketRWDCachePool,
		DssLkStkCatHashBucketRWDCacheLKUP,
		DssLkStkCatRWDCacheManager,
		DssLkStkCatCDSSRWDCacheAdmin,
		DssLkStkCatRWDCacheCluster,   

		DssLkStkCatRWDCacheMap,
		DssLkStkCatSortedList,
		DssLkStkCatCubeInfo2,
		DssLkStkCatSQLEngineCubeInfoLock,

		DssLkStkCatSearchEngineManager,
		DssLkStkCatMultiplexProcComm,
		
		DssLkStkCatSearchService,

		DssLkStkCatLastItem					      // Do not add items after here
    											  // (range checking)
	} 
	EnumDSSLockStackCategory;
}

#endif // MSynch_Lock_Categories_h
