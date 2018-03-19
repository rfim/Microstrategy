//  $COM\SourceCode\DSSCOMMaster\DSSCOM_TLB_INC.h

//	Header file to import the DSSCommaster master type library

//	Copyright (C) 1997-99 MicroStrategy Incorporated
//	All rights reserved

#ifndef _DSSCOM_TLB_INCLUDED_
#define _DSSCOM_TLB_INCLUDED_

#if defined(WIN32) && !defined(_USE_MSI_COM) && !defined(_USE_PARTIAL_MSI_COM)

	// Disable the warning about the typelibrary using a symbol that is defined as a macro
	// The warning cannot be avoided, since the #import includes some Windows headers that
	// defines the macros.  At this time the symbols 'AddForm' and 'ReplaceText' are affected
	#pragma warning(disable:4278)

	#import "DSSComMaster\DSSCOMMaster.tlb" \
		raw_interfaces_only no_namespace named_guids \
		rename ("GUID", "_GUID") \
		exclude ("IStream", "tagSTATSTG", "_LARGE_INTEGER", "_ULARGE_INTEGER") \
		exclude ("ISequentialStream", "IErrorInfo", "_FILETIME", "_GUID") \
		inject_statement("#pragma message(\"Importing \" __FILE__)") \
		inject_statement("#pragma warning(disable:4146) // unary minus operator applied to unsigned type, result still unsigned")

	// Remove the Windows defined macros, defined within the #import that affect subsequent compilation
	#include "PDCHeader/ProtectedSource/UndefineWin32APIAliases.h"

#else
	#include "PDCHeader/PDCatlbase.h"
	#include "PDCHeader/PDCobjbase.h"
	#include "DSSCOMMaster/DSSCOMMaster_i.h"
#endif // WIN32 && !_USE_MSI_COM

// hurwood 2nd Oct 1999
// We have changed several hundred names in the type library
// These macros make the old names into synonyms into the new names
// Eventually we hope to use only the new names in the code
#define DSSAttribute_LockType                          EnumDSSAttributeLockType
#define DSSAttributeRel_ERType                         EnumDSSERType
#define DSSAttributeRel_PartialType                    EnumDSSPartialType
#define DSSAxis_Name                                   EnumDSSAxisName
#define DSSBaseAttributeForm_Type                      EnumDSSBaseFormType
#define DSSCreateProject_Flags                         EnumDSSCreateFlags
#define DSSDataFlags                                   EnumDSSCellInfoFlags
#define DSSDbc_Restrictions                            EnumDSSCatalogRestriction
#define DSSDbcUpdate_Flags                             EnumDSSCatalogUpdateFlags
#define DSSDelete_Flags                                EnumDSSDeleteFlags
#define DSSDimension_Relationships                     EnumDSSRelationship
#define DSSDimension_Type                              EnumDSSDimensionType
#define DSSDirectedAttribute_Direction                 EnumDSSAttributeDirection
#define DSSElement_Type                                EnumDSSElementType
#define DSSExecution_Status                            EnumDSSExecutionStatus
#define DSSFolder_Type                                 EnumDSSFolderType
#define DSSFunction_Type                               EnumDSSFunction
#define DSSMetricLimit_Type                            EnumDSSMetricLimitType
#define DSSObjectInfo_State                            EnumDSSObjectState
#define DSSObjectInfo_Type                             EnumDSSObjectType
#define DSSParser_Flags                                EnumDSSParserFlags
#define DSSParser_State                                EnumDSSParserState
#define DSSPrivileges_Type                             EnumDSSPrivilegeType
#define DSSPropertyGroup_Types                         EnumDSSPropertyGroupTypes
#define DSSPropertySet_Type                            EnumDSSPropertySet
#define DSSRelation_ExtnType                           EnumDSSExtnType
#define DSSRelation_JoinType                           EnumDSSJoinType
#define DSSReportInstance_State                        EnumDSSReportStates
#define DssReportServerAllCacheActions                 DssActionAllCache
#define DSSReportSource_Actions                        EnumDSSReportActions
#define DSSReportSource_Flags                          EnumDSSReportFlags
#define DSSReportSource_Next                           EnumDSSReportNext
#define DSSRoot_Type                                   EnumDSSRootType
#define DSSSort_Subtotals                              EnumDSSSubtotalsPosition
#define DSSSort_Type                                   EnumDSSSortType
#define DSSSource_Flags                                EnumDSSSourceFlags
#define DSSSubtotalStyles                              EnumDSSSubtotalStyle
#define DSSTemplateSubtotal_Type                       EnumDSSTemplateSubtotalType
#define DSSTemplateUnit_Type                           EnumDSSTemplateUnitType
#define DSSToken_Level                                 EnumDSSTokenLevel
#define DSSToken_State                                 EnumDSSTokenState
#define DSSToken_Type                                  EnumDSSTokenType
#define EnumAutoCommitOptions                          EnumDSSAutoCommit
#define NOT_AUTO_COMMIT                                DssAutoCommitNo
#define AUTO_COMMIT                                    DssAutoCommitYes
#define EnumCachedReportStatus                         EnumDSSCachedReportStates
#define EnumConnection_Status                          EnumDSSWebConnectionStatus
#define DssAgentApplication                            DssApplicationAgent
#define DssWebApplication                              DssApplicationWeb
#define DssServerScheduler                             DssApplicationScheduler
#define DssCustomApplication                           DssApplicationCustom
#define EnumDSSApplication_Type                        EnumDSSApplicationType
#define EnumDSSAttributeTypes                          EnumDSSAttributeType
#define EnumDSSAuthMode                                EnumDSSAuthModes
#define EnumDSSCacheSetting_Type                       EnumDSSCacheSetting
#define EnumDSSCacheType                               EnumDSSCacheTypes
#define EnumDSSDataSource_Flags                        EnumDSSDataSourceFlags
#define DssRoleMD6Server                               DssRoleMD7Server
#define DssDataSourceTypeProjectMD6                    DssDataSourceTypeProjectMD7
#define DssSunday                                      DssDaySunday
#define DssMonday                                      DssDayMonday
#define DssTuesday                                     DssDayTuesday
#define DssWednesday                                   DssDayWednesday
#define DssThursday                                    DssDayThursday
#define DssFriday                                      DssDayFriday
#define DssSaturday                                    DssDaySaturday
#define EnumDSSDBConnectionCache_Option                EnumDSSDBConnectionCache
#define DssDBConnectionCache_Reserved                  DssDBConnectionCacheReserved
#define DssDBConnectionCache_NoCaching                 DssDBConnectionCacheNone
#define DssDBConnectionCache_ConnectionReuse           DssDBConnectionCacheReuse
#define DssDBConnectionCache_SerialConnectionReuse     DssDBConnectionCacheSerialReuse
#define EnumDSSDBConnectionDriver_Type                 EnumDSSDBConnectionDriver
#define DssDBConnectionDriver_Reserved                 DssDBConnectionDriverReserved
#define DssDBConnectionDriver_ODBC                     DssDBConnectionDriverODBC
#define DssDBConnectionDriver_NATIVE_DB_API            DssDBConnectionDriverNativeDBAPI
#define EnumDSSDBConnectionExecution_Mode              EnumDSSDBConnectionExecutionMode
#define DssDBConnectionExecution_Reserved              DssDBConnectionExecutionReserved
#define DssDBConnectionExecution_AsynchConnection      DssDBConnectionExecutionAsynchConnection
#define DssDBConnectionExecution_AsynchStatement       DssDBConnectionExecutionAsynchStatement
#define DssDBConnectionExecution_Synchronus            DssDBConnectionExecutionSynchronous
#define EnumDSSDBConnectionMultiProcess_Option         EnumDSSDBConnectionMultiProcessOption
#define DssDBConnectionMultiProcess_Reserved           DssDBConnectionMultiProcessReserved
#define DssDBConnectionMultiProcess_MultiThreadMode    DssDBConnectionMultiProcessMultiThreadMode
#define DssDBConnectionMultiProcess_MultiProcessMode   DssDBConnectionMultiProcessMultiProcessMode
#define EnumDSSFilterTypes                             EnumDSSFilterType
#define DssElementRequest                              DssJobRequestTypeElement
#define DssMetadataRequest                             DssJobRequestTypeObject
#define DssReportRequest                               DssJobRequestTypeReport
#define DssDocumentRequest                             DssJobRequestTypeDocument
#define DssMDVersionMD6                                DssMDVersionMD7
#define EnumDSSMetricTypes                             EnumDSSMetricType
#define DssJanuary                                     DssMonthJanuary
#define DssFebruary                                    DssMonthFebruary
#define DssMarch                                       DssMonthMarch
#define DssApril                                       DssMonthApril
#define DssMay                                         DssMonthMay
#define DssJune                                        DssMonthJune
#define DssJuly                                        DssMonthJuly
#define DssAugust                                      DssMonthAugust
#define DssSeptember                                   DssMonthSeptember
#define DssOctober                                     DssMonthOctober
#define DssNovember                                    DssMonthNovember
#define DssDecember                                    DssMonthDecember
#define EnumDSSNodeDimtys                              EnumDSSNodeDimty
#define EnumDSSOuterJoinTypes                          EnumDSSOuterJoinType
#define DssProjectLoad_Active                          DssProjectLoadActive
#define DssProjectLoad_FullIdle                        DssProjectLoadFullIdle
#define DssProjectLoad_Offline                         DssProjectLoadOffline
#define EnumDSSPromptActions                           EnumDSSPromptAction
#define DssPropagateACL_DirectChildren                 DssPropagateACLDirectChildren
#define DssPropagateACL_Temporary                      DssPropagateACLTemporary
#define DssPropagateACL_Reserved                       DssPropagateACLReserved
#define DssDisplayInfoCollectionPeriodicity            DssPUDisplayInfoCollectionPeriodicity
#define DssMaintenancePeriodicity                      DssPUMaintenancePeriodicity
#define EnumDSSReportCacheInfoOption                   EnumDSSReportCacheInfoOptions
#define DssUserSuppliedCost                            DssReportCostUserSupplied
#define DssEstimatedCost                               DssReportCostEstimated
#define DssHistoricalCost                              DssReportCostHistorical
#define DssResultSetSize                               DssReportCostResultSetSize
#define DssReportType                                  DssReportCostReportType
#define EnumDSSRoleTypes                               EnumDSSRoleType
#define DssEventTriggerType                            DssTriggerEvent
#define DssTimePointSeriesType                         DssTriggerTimePointSeries
#define EnumDSSServerCommands                          EnumDSSServerCommand
#define DssMaxResultRowCount                           DssServerProjectMaxResultRowCount
#define DssMaxReportExecutionTime                      DssServerProjectMaxReportExecutionTime
#define DssMaxScheduledReportExecutionTime             DssServerProjectMaxScheduledReportExecutionTime
#define DssScheduledReportExecutionRetryCount		   DssServerProjectScheduledReportExecutionRetryCount
#define DssMaxMessageRetentionTime                     DssServerProjectMaxMessageRetentionTime
#define DssMaxUserConnectionIdleTime                   DssServerMaxUserConnectionIdleTime
#define DssMaxJobPerUserAccount                        DssServerProjectMaxJobPerUserAccount
#define DssMaxJobPerUserConnection                     DssServerProjectMaxJobPerUserConnection
#define DssMaxJobPerProject                            DssServerProjectMaxJobPerProject
#define DssMaxInteractiveJobPerProject                 DssServerProjectMaxInteractiveJobPerProject
#define DssMaxUserConnectionPerProject                 DssServerProjectMaxUserConnectionPerProject
#define DssCollectStatisticsFlag                       DssServerProjectCollectStatisticsFlag
#define DssMaxReportCacheMemConsumption                DssServerProjectMaxReportCacheMemConsumption
#define DssMaxReportCacheCount                         DssServerProjectMaxReportCacheCount
#define DssMaxReportCacheDiskSpace                     DssServerProjectMaxReportCacheDiskSpace
#define DssReportCacheLookupTableSize                  DssServerProjectReportCacheLookupTableSize
#define DssMaxObjectCacheMemConsumption                DssServerProjectMaxObjectCacheMemConsumption
#define DssMaxObjectCacheCount                         DssServerProjectMaxObjectCacheCount
#define DssObjectCacheLookupTableSize                  DssServerProjectObjectCacheLookupTableSize
#define DssMaxElementCacheMemConsumption               DssServerProjectMaxElementCacheMemConsumption
#define DssMaxElementCacheCount                        DssServerProjectMaxElementCacheCount
#define DssElementCacheLookupTableSize                 DssServerProjectElementCacheLookupTableSize
#define DssMaxJobsPerServer                            DssServerMaxJobsPerServer
#define DssMaxInteractiveJobsPerServer                 DssServerMaxInteractiveJobsPerServer
#define DssMaxUserConnectionPerServer                  DssServerMaxUserConnectionPerServer
#define DssBackupPeriodicity                           DssServerBackupPeriodicity
#define DssBackupDirectory                             DssServerBackupDirectory
#define DssMSIPerformanceMonitoringFlag                DssServerMSIPerformanceMonitoringFlag
#define DssStartupDiagnosticsFlag                      DssServerStartupDiagnosticsFlag
#define DssMSILockCheckingFlag                         DssServerMSILockCheckingFlag
#define DssBackupIgnoreFlag                            DssServerBackupIgnoreFlag
#define DssSchedulerFlag                               DssServerSchedulerFlag
#define StatisticsUserSessionID                        DssStatisticUserSessionID
#define EnumDSSTableTypes                              EnumDSSTableType
#define DssResultReady                                 DssUserRequestResultReady
#define DssWaitForPrompt                               DssUserRequestWaitForPrompt
#define DssJobExecutionError                           DssUserRequestJobExecutionError
#define DssSqlGeneration                               DssUserRequestSqlGeneration
#define DssSqlExecution                                DssUserRequestSqlExecution
#define DssInAnalyticalEngine                          DssUserRequestInAnalyticalEngine
#define DssInResolutionServer                          DssUserRequestInResolutionServer
#define DssWaitingForCache                             DssUserRequestWaitingForCache
#define DssUpdatingCache                               DssUserRequestUpdatingCache
#define DssWaiting                                     DssUserRequestWaiting
#define DssWaitingOnGovernor                           DssUserRequestWaitingOnGovernor
#define DssWaitingForProject                           DssUserRequestWaitingForProject
#define DssWaitingForChildren                          DssUserRequestWaitingForChildren
#define DssPackingSendingOutput                        DssUserRequestPackingSendingOutput
#define DssRequestExecutionError                       DssUserRequestExecutionError
#define EnumIDSSCloseJobOption                         EnumDSSCloseJobOption
#define DSSCloseJobNormal                              DssCloseJobNormal
#define DSSCloseJobImmediate                           DssCloseJobImmediate
#define EnumIDSSClusterMemberStatus                    EnumDSSClusterMemberStatus
#define DSSClusterMemberStatusEXECUTING                DssClusterMemberStatusExecuting
#define DSSClusterMemberStatusPAUSE                    DssClusterMemberStatusPause
#define DSSClusterMemberStatusSTOPPED                  DssClusterMemberStatusStopped
#define EnumIDSSGetDBConnOption                        EnumDSSGetDBConnOption
#define DSSGetDBConnOptionDetail                       DssGetDBConnOptionDetail
#define DSSGetDBConnOption                             DssGetDBConnOption
#define EnumIDSSGetJobOption                           EnumDSSGetJobOptions
#define DSSGetJobOptionBrowsing                        DssGetJobOptionBrowsing
#define DSSGetJobOptionDetail                          DssGetJobOptionDetail
#define DSSGetJobOptionSQL                             DssGetJobOptionSQL
#define DSSGetJobOptionDiagnostics                     DssGetJobOptionDiagnostics
#define EnumIDSSJobStatus                              EnumDSSJobStatus
#define DSSJobStatusEXECUTING                          DssJobStatusExecuting
#define DSSJobStatusWAITING                            DssJobStatusWaiting
#define DSSJobStatusCOMPLETED                          DssJobStatusCompleted
#define DSSJobStatusERROR                              DssJobStatusError
#define DSSJobStatusCANCELED                           DssJobStatusCanceled
#define EnumIDSSProjectStatus                          EnumDSSProjectStatus
#define DssProjectStatus_Offline                       DssProjectStatusOffline
#define DssProjectStatus_OfflinePending                DssProjectStatusOfflinePending
#define DssProjectStatus_ErrorState                    DssProjectStatusErrorState
#define DssProjectStatus_OnlinePending                 DssProjectStatusOnlinePending
#define DssProjectStatus_Active                        DssProjectStatusActive
#define DssProjectStatus_ExecIdle                      DssProjectStatusExecIdle
#define DssProjectStatus_RequestIdle                   DssProjectStatusRequestIdle
#define DssProjectStatus_MetadataIdle                  DssProjectStatusMetadataIdle
#define DssProjectStatus_WarehouseIdle                 DssProjectStatusWarehouseIdle
#define DssProjectStatus_FullIdle                      DssProjectStatusFullIdle
#define EnumIDSSRefreshOption                          EnumDSSRefreshOption
#define DssServerRefrshNo                              DssServerRefreshNo
#define EnumIDSSScheduleAdminOption                    EnumDSSScheduleAdminOption
#define DSSScheduleAdminNormal                         DssScheduleAdminNormal
#define DSSScheduleAdminDetail                         DssScheduleAdminDetail
#define EnumIDSSServerDiagFlag                         EnumDSSServerDiagnosticsFlag
#define DSSDiag_User                                   DssDiagnosticsUser
#define DSSDiag_System                                 DssDiagnosticsSystem
#define EnumIDSSServerIdleOption                       EnumDSSServerIdleOption
#define DSSServerIdleNormal                            DssServerIdleNormal
#define EnumIDSSServerStatus                           EnumDSSServerStatus
#define EnumIDSSServerStopOption                       EnumDSSServerStopOption
#define DSSServerStopNormal                            DssServerStopNormal
#define DSSServerStopImmediately                       DssServerStopImmediately
#define ServerStopAbort                                DssServerStopAbort
#define IDSSServerNet_Protocol                         EnumDSSNetProtocol
#define IDSSServerNet_PROTOCOL_DEFAULT                 DssNetProtocolDefault
#define IDSSServerNet_PROTOCOL_TCP                     DssNetProtocolTCP
#define IDSSServerNet_PROTOCOL_IPX                     DssNetProtocolIPX
#define IDSSServerNet_PROTOCOL_NB                      DssNetProtocolNB
#define IDSSServerNet_Status                           EnumDSSServerNetStatus
#define IDSSServerNet_Init                             DssServerNetInit
#define IDSSServerNet_Disconnect                       DssServerNetDisconnect
#define IDSSServerNet_Connect_Async                    DssServerNetConnectAsync
#define IDSSServerNet_Connect_Sync                     DssServerNetConnectSync
#define IDSSDbTable                                    IDSSDBTable
#define IDSSDbTables                                   IDSSDBTables
#define EnumDSSDbTableType                             EnumDSSDBTableType
#define DssDrillInstance							   DssReportDrillInstance

// Two of these renames apply to interfaces
// We need to rename the IDD_ symbol for these interfaces as well
#define IID_IDSSDbTable                                IID_IDSSDBTable
#define IID_IDSSDbTables                               IID_IDSSDBTables


// The symbol DSSDataType_Type is also defined in a seperate header file
// So there is a risk that we might find it already defined
#ifndef DSSDataType_Type
#define DSSDataType_Type                               EnumDSSDataType
#endif

// xue 1/21/2000 - Changed DssStateCorrupted to DssStateCorrupt per issue 86404
#define DssStateCorrupted								DssStateCorrupt

// hurwood 17th Feb 2000
// Renamed some memory constants that don't follow the conventions
#define	DssServerAverageReportQueueTime					DssServerSummaryAverageReportQueueTime
#define	DssAverageReportResponseTime					DssServerSummaryAverageReportResponseTime
#define	DssServerOpenReports							DssServerSummaryOpenReports
#define	DssServerExecutingReports						DssServerSummaryExecutingReports
#define	DssServerConnectedUserSessions					DssServerSummaryConnectedUserSessions
#define	DssserverOpenDatabaseConnections				DssServerSummaryOpenDatabaseConnections
#define	DssServerActiveDatabaseConnections				DssServerSummaryActiveDatabaseConnections
#define EnumNTCounter									EnumDSSNTCounter

// xue 3/8/00 - corrected a type
#define DssCmdRequestExecut			DssCmdRequestExecute
#define DssReportCachedFlag			EnumDSSReportCachedFlag

#endif // _DSSCOM_TLB_INCLUDED_
