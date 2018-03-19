//
//  ExpressionNode.h
//  MSTRMobile
//
//  Created by Ilia Baskin on 2/16/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSTRHandler.h"

@class Dimty, SAXSupport, MSIExpression, XMLBuilder, MSIObjectInfo;
@class PromptInstances;

typedef enum _NodeTypes{
	NodeTypeFormShortcut = 1, NodeTypeElements, NodeTypeConstant, NodeTypeOperator, NodeTypeShortcut, 
	NodeTypeTime, NodeTypeRelationship, NodeTypeResidue, NodeTypeMe, NodeTypeBigDecimal, NodeTypeGroup, 
	NodeTypeRef
}NodeTypes;

typedef enum _FunctionTypes{
	FunctionTypeGeneric = 1, FunctionTypeRankQual, FunctionTypePercentQual
}FunctionTypes;
	
@interface ExpressionNode : NSObject {
	
@protected
	NSInteger expressionType;
	NSInteger nodeType;
	NSInteger nodeDimtyType;
	NSInteger dataType;
	NSInteger key;
	NSString* displayName;
	NSString* emet;
    NSInteger cpValue;
	Dimty* dimtyObject;
	BOOL isIndependent;
	
	ExpressionNode* parentNode;
	MSIExpression* __weak ownerExpression;
	NSMutableArray* childNodes;
	BOOL isModified;
    PromptInstances* promptinstances;
}

@property NSInteger expressionType;
@property NSInteger nodeType;
@property NSInteger nodeDimtyType;
@property NSInteger dataType;
@property NSInteger key;
@property NSInteger cpValue;
@property (nonatomic, copy) NSString* displayName;
@property (nonatomic, copy) NSString* emet;
@property (nonatomic, strong) Dimty* dimtyObject;
@property (nonatomic, readonly, strong) ExpressionNode* parentNode;
@property BOOL isIndependent;
@property BOOL isModified;

-(id)initWithExpression: (MSIExpression*)expr;
-(id)copyWithZone:(NSZone *)zone expression:(MSIExpression*)expression;
-(id)copyWithZone:(NSZone *)zone;
-(NSInteger)childCount;
-(ExpressionNode*)childAtIndex:(NSInteger)i;
-(ExpressionNode*)firstChild;
-(ExpressionNode*)nextSibling;
-(NSInteger)subExpressionCount;

-(BOOL)addChild:(ExpressionNode*)newNode;
-(BOOL)insertChild:(ExpressionNode*)newNode before:(ExpressionNode*)node;
-(void)removeChild:(ExpressionNode*)child;
-(BOOL)replaceChild:(ExpressionNode*)child with:(ExpressionNode*)newNode;
-(void)clear;
-(BOOL)containsChild:(ExpressionNode*)child;

-(void)handleXml:(NSString*)elementName attributes:(NSDictionary*)attributes  support:(SAXSupport*)support;
-(void)handleXmlAttributes:(NSString*)elementName attributes:(NSDictionary*)attributes;
-(void)buildXml:(XMLBuilder*)builder;
-(void)buildTypeSpecificXml:(XMLBuilder*)builder;


@end

@interface ExpressionNode (MSTRHandler) <MSTRHandler>
@end

//====================================================================
@interface OperatorNode : ExpressionNode {
	
@private
	NSInteger function;
	NSMutableDictionary* properties;
	int mrpFunction;
	NSInteger functionType;
}

@property NSInteger function;
@property (nonatomic, strong) NSMutableDictionary* properties;
@property NSInteger functionType;
-(int) mappedFunction;
-(void) setMappedFunction:(NSInteger)function functionType:(NSInteger)functionType;
-(void) setMappedFunction:(NSInteger)mappedFunction functionType:(NSInteger)newFunctionType ascending:(BOOL) ascending;
@end


//====================================================================
@interface ShortcutNode : ExpressionNode  {
	
@private
	MSIObjectInfo* target;
}

@property (nonatomic, strong) MSIObjectInfo* target;

@end

//====================================================================
@interface ValueNode : ExpressionNode {
	
@protected
	NSInteger valueDataType;
	NSString* value;
}

@property (nonatomic, copy) NSString* value;
@property NSInteger valueDataType;

@end

//====================================================================
@interface ConstantNode : ValueNode {
}

@end

//====================================================================
@interface BigDecimalNode : ValueNode {
}


@end

//====================================================================
@interface TimeNode: ValueNode {
	
@private
	NSString* dynamicDateXML;
}

-(BOOL) isDynamic;

@end

//====================================================================
@interface FormShortcutNode : ExpressionNode {
	
@private
	MSIObjectInfo* attribute;
	MSIObjectInfo* form;
}

@property (nonatomic, strong) MSIObjectInfo* attribute;
@property (nonatomic, strong) MSIObjectInfo* form;

@end

//====================================================================
@interface ElementsObjectNode: ExpressionNode {
@private
	MSIObjectInfo* attribute;
	NSMutableArray* elements;
}

@property (nonatomic, strong) NSMutableArray* elements;
@property (nonatomic, strong) MSIObjectInfo* attribute;

@end

//====================================================================
@interface UnsupportedExpressionNode : ExpressionNode {
	@private
	XMLBuilder* currentBuilder;
	NSMutableString* xml;
}

@end