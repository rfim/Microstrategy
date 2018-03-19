
#import "Service.h"

//Services used by Dashboard
typedef enum _DashboardServiceTypes{
	 
    //Reserve 400 Service Type for MSIServiceFactory.
	EnumRWDBinaryDataService=400, 
	EnumRWManipulationService, 
	EnumRWManipulationAndBinaryDataService, 
	EnumRWDrillService, 
	EnumRWDrillAndBinaryDataService, 
	EnumRWDImageService, 
	EnumPreloadRWDBinaryDataService, 
	EnumCacheCheckService, 
	EnumSaveToInboxService, 
	EnumRemoveInboxMsgsService, 
	EnumGetInboxMsgsService, 
	EnumRWLinkDrillAndBinaryDataService, 
	EnumSubscriptionCheckService, 
	EnumUserRuntimeService,
	EnumRWDDataServiceManagerService,
	EnumSaveAsSegmentService,
    EnumRemoveJobService,
    EnumCheckStatusService,
    EnumSaveAsDocumentService,
    EnumPollStatusService,
    EnumCreateFolderService
} DashboardServiceTypes;

@interface DashboardServiceFactory :NSObject {
}

+(DashboardServiceFactory*)getServiceFactory;
-(id<Service>)newService:(NSInteger) serviceType;

@end
