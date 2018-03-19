//
//  RectangleView.h
//  IPhoneChart
//
//  Created by Liu Liang on 4/27/10.
//  Copyright 2010 MSTR. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

typedef enum
{
	OT_Rectangle = 0,
	OT_Marker = 1,	
}EnumObjectType;

@interface RectangleView : UIView 
@property (nonatomic, assign) EnumObjectType mObjectType;
@property (nonatomic, assign) void* mpObject;

@end
