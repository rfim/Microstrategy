//
//  attrParser.h
//  MSTRRssReader
//
//  Created by Pavel Shatrov on 3/31/11.
//  Copyright 2011 EastBanc Technologies. All rights reserved.
//

#import <Foundation/Foundation.h>


/*
 <fmt>
 <phni value="60"/>
 <it value="10"/>
 <drf value="30"/>
 <asd value="Up"/>
 <ass value="1"/>
 <fid value="true"/>
 <rup value="true"/>
 <updro value="false"/>
 <mitmr value="true"/>
 <le value="Inline"/>
 <ths value="0x0000FF"/>
 <drssf value="http://static.feed.rbc.ru/rbc/internal/rss.rbc.ru/rbc.ru/news.rss??http://news.google.com/news?pz=1&amp;cf=all&amp;ned=us&amp;hl=en&amp;output=rss"/>
 <alc value="true"/>
 <cd value="0x000000"/>
 <ab value="0xd0d0d0"/>
 <ef value="0x000000"/>
 <bc value="0x86c5fb"/>
 <gh value="0xc8d7e5"/>
 <brlc value="0x000000"/>
 <brl value="0xdfdfdf"/>
 <imsh value="0x000000"/>
 <bh value="0x86c5fb"/>
 <iisc value="0xc8d7e5"/>
 <rtc value="0x000000"/>
 <wbdc value="0xa7a7a7"/>
 <rnf value="{[Customer City]}"/>
 <ft value="Tahoma"/>
 <fs value="11"/>
 <fst value="false"/>
 <fw value="false"/>
 <ftd value="Tahoma"/>
 <fsd value="10"/>
 <fstd value="false"/>
 <fwt value="false"/>
 <bcd value="0xffffff"/>
 <fcd value="0x000000"/>
 <wbc value="0xf5f5f2"/>
 <asni value="false"/>
 </fmt>
 */

@interface MSMAttrParser : NSObject<NSXMLParserDelegate> {	
	
}

@property (nonatomic, strong) NSString * asd;
@property (nonatomic, strong) NSString * le;
@property (nonatomic, strong) NSString * ths;
@property (nonatomic, strong) NSString * drssf;
@property (nonatomic, strong) NSString * alc;
@property (nonatomic, strong) NSString * cd;
@property (nonatomic, strong) NSString * ab;
@property (nonatomic, strong) NSString * ef;
@property (nonatomic, strong) NSString * bc;
@property (nonatomic, strong) NSString * gh;
@property (nonatomic, strong) NSString * brlc;
@property (nonatomic, strong) NSString * brl;
@property (nonatomic, strong) NSString * imsh;
@property (nonatomic, strong) NSString * bh;
@property (nonatomic, strong) NSString * iisc;
@property (nonatomic, strong) NSString * rtc;
@property (nonatomic, strong) NSString * wbdc;
@property (nonatomic, strong) NSString * rnf;
@property (nonatomic, strong) NSString * ft;
@property (nonatomic, strong) NSString * ftd;
@property (nonatomic, strong) NSString * bcd;
@property (nonatomic, strong) NSString * fcd;
@property (nonatomic, strong) NSString * wbc;


@end
