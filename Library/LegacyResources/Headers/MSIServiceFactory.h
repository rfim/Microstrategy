//
//  MSIServiceFactory.h
//  MSTR Foundation
//
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Service.h"
#import "MSISessionConnectionManager.h"

#define MSTRCancelAllServicesNotification @"MSTRCancelAllServicesNotification"

@class MSIProjectInfo, MSTRError, WebServerInfo, MSIServiceImpl, MSIProjectConfiguration;

/**
 There are many types of services available to SDK users. The BrowseElements and ObjectInfo services are two of the more common and useful ones.
 */
typedef enum _ServiceTypes{
	EnumSessionService = 0, 
	EnumCloseSessionsService, 
	EnumReconcileService, 
	EnumPageByService, 
	EnumRWService, 
	EnumRefreshService, 
	EnumDataExplorerService, 
	EnumRegularDrillService, 
	EnumLinkedDrillService, 
	EnumPromptsService, 
	EnumSaveSubscriptionService, 
	EnumImageService, 
	EnumBrowseElementsService, 
	EnumObjectInfoService, 
	EnumSortGridService, 
	EnumPivotGridService, 
	EnumInitializeApplicationService, 
	EnumPreloadRWDataService, 
	EnumPreloadReportDataService, 
    EnumObjectService,
	EnumRegisterDeviceTokenService, 
	EnumRemoveDeviceTokenService,
	EnumSubscribeService,
	EnumUnsubscribeService, 
	EnumSQLWritebackService, 
	EnumLatencyService, 
	EnumLatencyPingService,
	EnumCertificateLoginFieldsService, 
	EnumGetCertificateService,
	EnumSignCertificateService,
	EnumDefaultProjPropService, 
	EnumFileDownloadService,
	EnumConfigurationAutoupdateCheckService,
	EnumChangePasswordService,
	EnumCreateSessionWithProjectService,
	EnumGetDimensionService,
	EnumSearchMetaDataService,
	EnumDeleteObjectService,
	EnumWisdomConnectionService,
	EnumStartingDocumentService,
	EnumWCDataService,
    EnumGetFolderURLsService,
    EnumWCDataServiceForNK,
    EnumRWDBinaryDataServiceForNK,
    EnumRWDExecuteAndBinaryService,
    EnumMobileStatsService,
    EnumHTMLViewerService,
    EnumGetProjectsService,
    EnumUsherQRImageService,
    EnumUsherPollStatusService,
    EnumMSMWCDataServiceForBT,
    EnumMSMRWDBinaryDataServiceForBT,
    EnumMSISubscribeService,
    EnumMSIUnsubscribeService,
    EnumMSIGetMobileSubscriptionService,
    EnumMSIGetSchedulesService,
    EnumMSILoadPromptAnswerService,
    EnumMSILoadSubscriptionService,
    EnumMSIUpdateSubscriptionService,
    EnumMSIGetAssociatedUsersOfReport,
    EnumMSICheckSubscriptionACL,
    EnumMSIGetSchedulesOfReport,
    EnumGetPluginVisListService,
    EnumReportExecuteXMLService,
    EnumMSIDocumentExecuteXMLService,
    EnumMSIExportReportService,
    EnumMSIGetPreferencesService,
    EnumMSIGetBinaryDataService,
    EnumMSIExportDocumentService
}ServiceTypes;

/**
 * MSIServiceFactory is the factory class to create all kinds of services in SDK. It also helps to manage session states, stop and resume services when device is locked or unlocked.
 */
@interface MSIServiceFactory : NSObject{
@private
	MSTRError* currentError;
    NSMutableArray* restartServicesList;
}

/**
 * Static method to create a singleton MSIServiceFactory object.
 * @return a singleton MSIServiceFactory object
 */
+(MSIServiceFactory *)getServiceFactory;

/**
 * Factory method to create a service depending on the service type. Client is responsible for releasing the service.
 * @param serviceType an enum value of ServiceTypes
 * @return a Service instance
 * @see ServiceTypes
 */
-(id<Service>)newService:(NSInteger) serviceType;

/**
 * get a copy of the section table.
 * @return Returns a copy of the section table
 */
-(NSDictionary*)getSessionTable;

/**
 * Connects to the I-Server based on the project information provided.
 * @param Target project for session creation 
 * @returnReturns session state string if successful, throws MSTRException if unsuccessful
 * @throws MSTRException when lacking a session
 */
-(NSString*)getSessionState:(MSIProjectInfo*)project __attribute__((deprecated)); //Deprecated, Replaced by getConnection: of MSISessionConnectionManager.

/**
 * Connects to the I-Server based on the project information provided. 
 * @param Target project for session creation 
 * @param Whether alerts resulting out of network errors should be suppressed (default is NO)
 * @return Returns session state string if successful, throws MSTRException if unsuccessful
 * @throws MSTRException when lacking a session
*/
-(NSString*)getSessionState:(MSIProjectInfo*)project suppressNetworkErrorAlerts:(BOOL)suppressNetworkErrorAlerts __attribute__((deprecated)); //Deprecated, Replaced by getConnection: of MSISessionConnectionManager.

/**
 * Connects to the I-Server based on the project information provided. 
 * @param Target project for session creation 
 * @param Whether alerts resulting out of network errors should be suppressed (default is NO)
 * @param Whether errors accessing projects should be ignored (default is NO)
 * @return Returns session state string if successful, throws MSTRException if unsuccessful
 * @throws MSTRException when lacking a session
 */
-(NSString*)getSessionState:(MSIProjectInfo*)aProject suppressNetworkErrorAlerts:(BOOL)suppressNetworkErrorAlerts ignoreAccessProjectError:(BOOL)ignoreAccessProjectError __attribute__((deprecated)); //Deprecated, Replaced by getConnection: of MSISessionConnectionManager.

/**
 * Connects to the I-Server based on the project information provided. 
 * @param Target project for session creation 
 * @param Whether alerts resulting out of network errors should be suppressed (default is NO)
 * @param Whether errors accessing projects should be ignored (default is NO)
 * @param The parent service calling the session service
 * @return Returns session state string if successful, throws MSTRException if unsuccessful
 * @throws MSTRException when lacking a session
*/
-(NSString*)getSessionState:(MSIProjectInfo*)aProject suppressNetworkErrorAlerts:(BOOL)suppressNetworkErrorAlerts ignoreAccessProjectError:(BOOL)ignoreAccessProjectError parentService:(MSIServiceImpl *)parentService __attribute__((deprecated)); //Deprecated, Replaced by getConnection: of MSISessionConnectionManager.

/**
 * Connects to the I-Server based on the project information provided. This method is used for the cases when the project has the possibility to be removed from device preferences during the service or the project has not been added to device preferences. In other scenarios, the getSessionState should be used.
 * @param projectConfig target project for session creation 
 * @param _suppressNetworkErrorAlerts whether alerts resulting out of network errors should be suppressed (default is NO)
 * @param _ignoreAccessProjectError whether errors accessing projects should be ignored (default is NO)
 * @return returns session state string if successful, throws MSTRException if unsuccessful
 * @throws MSTRException when lacking a session
 */
-(NSString*)getSessionStateByProjectConfiguration:(MSIProjectConfiguration*)projectConfig suppressNetworkErrorAlerts:(BOOL)_suppressNetworkErrorAlerts ignoreAccessProjectError:(BOOL)_ignoreAccessProjectError __attribute__((deprecated)); //Deprecated, Replaced by getConnection: of MSISessionConnectionManager.

/**
 * Get whether mobile read write privilege is granted for the project in the active session.
 * If there is no active session created for this project, it returns YES.
 * @param project project to be used to find a session
 * @return whether the current active project session has mobile read write privilege
 */
-(BOOL)hasMobileRWPrivilege:(MSIProjectInfo*)project __attribute__((deprecated)); //Deprecated, Replaced by the same method of MSISessionConnectionManager.

/**
 * Get the list of web servers that the client is currently connected with.
 * @return list of WebServerInfo
 * @see WebServerInfo
 */
-(NSArray*)connectedServers __attribute__((deprecated)); //Deprecated, Replaced by the same method of MSISessionConnectionManager.

/**
 * Get all the session states created from a web server
 * @param webServer web server to be used to looked for session states
 * @return list of session state strings
 * @see WebServerInfo
 */
-(NSArray*)sessionListForServer:(WebServerInfo*)webServer __attribute__((deprecated)); //Deprecated, Replaced by the same method of MSISessionConnectionManager.

/**
 * Remove all session states in client memory.
 * Note: These session states are not disconnected from server by calling this method.
 * @see CloseSessionService
 */
-(void)clearSessions __attribute__((deprecated)); //Deprecated, Please use getConnectionInformation: and clearConnection: of MSISessionConnectionManager.

/**
 * Remove session states of a particular project in client memory. 
 * Note: These session states are not disconnected from server by calling this method.
 * @param project project used to find session states
 */
-(void)clearSessionsForProject:(MSIProjectInfo*)project __attribute__((deprecated)); //Deprecated, Please use getConnectionInformation: and clearConnection: of MSISessionConnectionManager.

/**
 * Remove session states created from a particular web server in memory. These session states are not disconnected from server by calling this method.
 * @param webServer web server used to find session states
 */
-(void)clearSessionsForServer:(WebServerInfo*)webServer __attribute__((deprecated)); //Deprecated, Please use getConnectionInformation: and clearConnection: of MSISessionConnectionManager.

/**
 * Invalid a session state. The session state can't be used by client any more after it's invalidated.
 * Note: The session state is not invalidated in server side by calling this method.
 * @param sessionState session state to be invalidated
 */
-(void)invalidateSessionForState:(NSString*) sessionState __attribute__((deprecated)); //Deprecated, Replaced by the same method of MSISessionConnectionManager.

/**
 * Invalid session state of a project. The session state can't be used by client any more if it's invalided. 
 * Note: The session state is not invalidated in server side by calling this method.
 * @param project project used to find the session state
 */
-(void)invalidateSessionForProject:(MSIProjectInfo*)project __attribute__((deprecated)); //Deprecated, Replaced by the same method of MSISessionConnectionManager.

/**
 * Add a service to the restart list.
 * When UIApplicationProtectedDataWillBecomeUnavailable notification is received, services in the restart list will be stopped.
 * When UIApplicationProtectedDataDidBecomeAvailable notification is received, services in the restart list wil be restarted.
 * @param service service to be added to the restart list
 */
-(void)addToRestartServicesList:(id<Service>)service;

-(void)cancelAllServices;

@end