//
//  Expression.h
//  MSTRMobile
//
//  Created by Ilia Baskin on 2/16/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>

@class ExpressionNode, SAXSupport, XMLBuilder, Dimty, FormShortcutNode, OperatorNode, TimeNode, ConstantNode, 
	   BigDecimalNode, ShortcutNode, ElementsObjectNode, ValueNode, MSIObjectInfo;

@interface MSIExpression : NSObject <NSCopying> {
@private
	ExpressionNode* rootNode;
	NSString* complexDisplayName;
    ExpressionNode* rootNodeCopy;
};

@property (nonatomic, strong, readonly) ExpressionNode* rootNode;
@property (nonatomic, strong, readonly) NSString* complexDisplayName;

-(id)init;
-(void)dealloc;
-(int)getRootChildrenNumber;
-(BOOL)isEmpty;
-(void)clear;
-(void)clearAll;
-(void)undoClearAll;
-(ExpressionNode*)newNode:(NSDictionary*)attributes  support:(SAXSupport*)support;
-(void)handleXml:(NSString*)elementName attributes:(NSDictionary*)attributes  support:(SAXSupport*)support;
-(void)buildXml:(XMLBuilder*)builder;
-(NSString*)getXml;

-(BOOL)setRootNode:(ExpressionNode*)newRoot;
-(BOOL)addNode:(ExpressionNode*)newNode;

// all create methods leave the reference count at 1.  It is caller's responsibilty to release them

-(OperatorNode*) newOperatorNodeWithExpressionType:(NSInteger)expressionType function:(NSInteger)_function;
-(OperatorNode*) newAndNode;
-(OperatorNode*) newOrNode;
-(ValueNode*) newValueNode:(NSString*)value dataType:(NSInteger)_dataType;
-(ConstantNode*) newConstantNodeWithValue:(NSString*)value dataType:(NSInteger)_dataType;
-(TimeNode*) newTimeNode:(NSString*)value;
-(BigDecimalNode*) newBigDecimalNode:(NSString*)value;
-(ShortcutNode*) newShortcutNode:(MSIObjectInfo*)object;
-(FormShortcutNode*) newFormShortcutNodeWithAttribute:(MSIObjectInfo*) attributeInfo form:(MSIObjectInfo*) formInfo;
-(OperatorNode*) newMetricRankOperatorNodeWithMetric:(MSIObjectInfo*)metricInfo mrpOperator:(NSInteger)mrpOperator;
-(OperatorNode*) newMetricRankOperatorNodeWithMetric:(MSIObjectInfo*)metricInfo mrpOperator:(NSInteger)mrpOperator ascending:(BOOL)ascending;
-(OperatorNode*) newMetricPercentOperatorNode:(MSIObjectInfo*)metricInfo mrpOperator:(NSInteger)mrpOperator;
-(OperatorNode*) newMetricPercentOperatorNode:(MSIObjectInfo*)metricInfo mrpOperator:(NSInteger)mrpOperator ascending:(BOOL)ascending;
-(ElementsObjectNode*)newElementsObjectNode:(NSMutableArray*)_elements attribute:(MSIObjectInfo*)_attribue;

-(OperatorNode*) newAQSubExpression:(MSIObjectInfo*)attributeInfo form:(MSIObjectInfo*)form function:(NSInteger)function dataType:(NSInteger)dataType value:(NSString*)value;
-(OperatorNode*) newAQSubExpression:(MSIObjectInfo*)attributeInfo form:(MSIObjectInfo*)form function:(NSInteger)function dataType:(NSInteger)dataType firstValue:(NSString*)firstValue secondValue:(NSString*)secondValue;
-(OperatorNode*) newMQSubExpression:(MSIObjectInfo*)metricInfo function:(NSInteger)function dataType:(NSInteger)dataType value:(NSString*)value;

@end


@interface MSIExpression(Helper)

+(NSInteger) resolveMRPFunctionFromFunction:(NSInteger)function property4:(NSString*)pr4;
+(void) updateNodeWithMRPFunction:(NSInteger)mrpFunction outerNode:(OperatorNode*)outerNode innerNode:(OperatorNode*)innerNode ascending:(BOOL)ascending;


+(BOOL)isBranchQual:(ExpressionNode*)node;
+(BOOL)isAndNode:(ExpressionNode*)node;
+(BOOL)isOrNode:(ExpressionNode*)node;
+(BOOL)isNotNode:(ExpressionNode*)node;

-(void)consolidateTree;

@end
