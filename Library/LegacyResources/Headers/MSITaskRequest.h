//
//  MSITaskRequest.h
//  MSTR Foundation
//
//  Created by Amirali Charania on 2/20/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSINetworkRequestImpl.h"


#define ENV_TYPE_KEY @"taskEnv"
#define XML_ENV_TYPE_VALUE @"xml"
#define XHR_ENV_TYPE_VALUE @"xhr"

//if you use taskContentType=xmlanf is in the task request, then the block properties 
//will be attributes in the xml, otherwise the block properties would be child nodes
#define CONTENT_TYPE_KEY @"taskContentType"
#define XML_ANF_CONTENT_TYPE_VALUE @"xmlanf"
#define JSON_CONTENT_TYPE_VALUE @"json"
#define TASK_ID_KEY @"taskId"
#define SESSION_STATE_KEY @"sessionState"

@class AuthenticationFormRequest;

@interface MSITaskRequest : MSINetworkRequestImpl {
@protected
	NSString* envType;
	NSString* taskID;
	AuthenticationFormRequest* liveAuthRequest;
}

@property (nonatomic, copy) NSString* envType;
@property (nonatomic, copy) NSString* taskID;

@end
