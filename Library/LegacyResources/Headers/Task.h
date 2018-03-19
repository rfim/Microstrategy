//
//  Task.h
//  IPadMain
//
//  Created by Liu Liang on 5/7/10.
//  Copyright 2010 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface Task : NSObject {
	id __weak mReceiver;
	SEL mSelector;
	id mParam;
}

@property (atomic, weak) id mReceiver;
@property (nonatomic) SEL mSelector;
@property (nonatomic, strong) id mParam;

@end
