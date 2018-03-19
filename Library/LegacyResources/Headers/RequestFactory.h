//
//  RequestFactory.h
//  MSTR Foundation
//
//  Created by Amirali Charania on 2/20/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import"MSINetworkRequest.h"

typedef enum _NetworkRequestTypes{
	EnumLoginRequest = 0, 
	EnumCloseSessionsRequest, 
	EnumIsSessionAliveRequest, 
	EnumReportListRequest, 
	EnumPageByRequest, 
	EnumReportGraphRequest, 
	EnumReportGridRequest, 
	EnumRWTaskRequest, 
	EnumRefreshSubscriptionRequest, 
	EnumPollStatusRequest, 
	EnumInboxMessageTimeRequest,
	EnumFolderBrowseRequest, 
	EnumBrowseHierarchyRequest, 
	EnumNamedFolderIDRequest, 
	EnumGridDrillRequest, 
	EnumGraphDrillRequest, 
	EnumLinkedDrillRequest, 
	EnumDuplicateMessageRequest,
	EnumPromptsRequest, 
	EnumSaveSubscriptionRequest, 
	EnumBrowseElementsRequest, 
	EnumChartDataRequest, 
	EnumObjectInfoRequest, 
	EnumRWGridDrillRequest,
	EnumSortGridRequest, 
	EnumPivotGridRequest, 
	EnumRWDBinaryRequest, 
	EnumRWDManipulationRequest, 
	EnumXMLAPIRequest, 
	EnumXMLAPIRequest2,
	EnumExecuteReportRequest, 
	EnumExecuteDocumentRequest, 
	EnumRWDImageServiceRequest, 
	EnumPreloadRWTaskRequest, 
	EnumSubscribeRequest, 
	EnumUnsubscribeRequest, 
	EnumLatencyRequest,
	EnumAnswerPromptsTaskRequest,
	EnumFileDownloadRequest,
	EnumAuthentiationFormRequest,
	EnumConfigurationAutoupdateCheckRequest,
	EnumInitializeApplicationRequest,
	EnumChangePasswordRequest,
	EnumGetDimensionTaskRequest,
	EnumSearchMetaDataTaskRequest,
	EnumWCDataRequest,
    EnumWCDataRequestForNK,
    EnumGetFolderURLsRequest,
    EnumRWDBinaryDataRequestForNK,
    EnumRWDExecuteAndBinaryTaskRequestForNK,
	EnumGetConnectionInformationRequest,
	EnumDeleteObjectTaskRequest,
	EnumUploadImageTaskRequest,
	EnumWisdomConnectionRequest,
	EnumStartingDocumentRequest,
    EnumRWDExecuteAndBinaryTaskRequest,
    EnumCreateFolderTaskRequest,
    EnumHTMLViewerRequest,
    EnumGetProjectsTaskRequest,
    EnumGetMapShapesTaskRequest,
    EnumGetMapShapeConfigTaskRequest,
    EnumUsherQRImageRequest,
    EnumUsherPollStatusRequest,
    EnumMSISubscribeRequest,
    EnumMSIUnsubscribeRequest,
    EnumMSIGetMobileSubscriptionRequest,
    EnumMSIGetSchedulesRequest,
    EnumMSILoadPromptAnswerRequest,
    EnumMSILoadSubscriptionRequest,
    EnumMSIUpdateSubscriptionRequest,
    EnumGetPluginVisListRequest,
	EnumGetD3WidgetFilesListRequest,
    EnumMSIExportReportRequest,
    EnumMSIExportDocumentRequest
} NetworkRequestTypes;

@interface RequestFactory : NSObject {
}

+(RequestFactory*) getRequestFactory;
+(BOOL) isMainRequestOnExecution:(NSObject*) ipRequest;
-(id<MSINetworkRequest>) newRequest:(NetworkRequestTypes)requestType;

@end
