//
//  RWGetInboxMsgs.h
//  iPadMain Library
//
//  Created by Ji Jin on 8/25/10.
//  Copyright 2010 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "XMLAPIService.h"

@interface RWGetInboxMsgs : XMLAPIService {
    //ASYNCEXC
    
   //input message ids 
   NSArray* messages;

   //poll Inbox Message with status only or with all detailed informations 
   bool pollStatusOnlyForCurrentProject_;
    
   //returned messages <InboxMsg>/<InboxDetaileMsg>
   NSMutableArray* allInboxMessages;
   
}

@property bool pollStatusOnlyForCurrentProject;

//if messages is nil, then return all messages inside inbox
@property (nonatomic, strong) NSArray* messages;

@end

@interface InboxMsg :NSObject{
    NSString* msgID_;
    
    //is current message's execution finished (or ready to download)
    bool finished_;

}
@property (readonly, copy) NSString* msgID;
@property (readonly) bool finished;

@end

@interface InboxDetailedMsg:InboxMsg{
    NSString* reportID_;
    NSInteger reportType_; //document or report
    NSString* reportName_;
    NSString* reportDescription_;
    NSString* reportSubscriptionID_;
    NSString* reportTargetName_;
    NSString* projectID_;
    double inboxCompletionTime_;
    int stateID_;
    double targetModificationTime_;
    NSString* reportAbbreviation_;
    int reportViewMedia_;

    //is this message viewed by user
    bool isViewedByUser_;
    
    //is this message comes from a dcument drill-out 
    bool isDrilledOut_;
    
    //is this message comes from a dcument has prompt 
    bool hasPrompt_;
    
    //number of manipulations have been applied (including drill-in manipulation)
    unsigned appliedManipulationsNumber_;
    
    bool intermediateDocument_;
}

@property (readonly, copy) NSString* reportID;

//DL;03/18/2013;580194;931 improvement, messageID's the most original document definition ID
//this property is currently used for drilled out. Its' value is source report ID of the drilling message
@property (readonly, copy) NSString *sourceReportID;

@property (readonly) NSInteger reportType;
@property (readonly, copy) NSString* reportName;
@property (readonly, copy) NSString* reportDescription;
@property (readonly, copy) NSString* reportSubscriptionID;
@property (readonly, copy) NSString* reportTargetName;
@property (readonly, copy) NSString* projectID;
@property (readonly) double inboxCompletionTime;
@property (readonly) int stateID;
@property (readonly) double targetModificationTime;
@property (readonly, copy) NSString* reportAbbreviation;
@property (readonly) int reportViewMedia;
@property (readonly) bool isViewedByUser;
@property (readonly) bool isDrilledOut;
@property (readonly) bool hasPrompt;
@property (readonly) unsigned appliedManipulationsNumber;
@property (readonly) bool intermediateDocument;

@end
