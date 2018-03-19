/*
 *  SimpleXMLParser.h
 *  ReportServiceCore
 *
 *  Created by dong shi on 12/24/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef SIMPLE_XML_H
#define SIMPLE_XML_H
#import <libxml/tree.h>
#import <libxml/parser.h>
#import <libxml/xmlwriter.h>
#import <libxml/encoding.h>
#import "string"



class SimpleXMLWriter
{
public:
	SimpleXMLWriter();
	~SimpleXMLWriter();
	
	void StartDocument(const char *version, const char *encoding, const char *standalone = NULL);
	void EndDocument();
	
	void StartElement(const char * name);
	void WriteAttribute(const char * name, const char * content);
	void EndElement();
	
	std::string ToString();
	bool IsValid();
	
private:
	xmlTextWriterPtr mpWriter;
    xmlBufferPtr mpBuffer;
	bool mIsValid;
};




//wrapper of xmlNodePtr for easier use.
class  SimpleXMLNode{
public:
	SimpleXMLNode(xmlNodePtr ipNode);
	SimpleXMLNode();
	~SimpleXMLNode();
	
	xmlNodePtr AddTextChild(const char* ipName, const char* ipContent);
	xmlNodePtr AddChild(xmlNodePtr ipChidNode);
	xmlNodePtr AddChild(SimpleXMLNode* ipChidNode);
	xmlNodePtr AddChild(SimpleXMLNode& irChidNode);
	void SetNode(xmlNodePtr ipNode);
	xmlNodePtr GetNode() const;
	bool IsValid();
	
	void AddProperty(const char* ipPropName, const char* ipPropValue);
	bool HasProperty(const char* ipPropertyName);
	void SetProperty(const char* ipPropName, const char* ipPropValue);
	
	// added by bxia for parsing xml
	void GetProperty(const char* ipPropertyName, std::string& istrPropertyValue);
	void GetContent(std::string& istrContent);
	std::string GetName();
	SimpleXMLNode GetNextSibling();
	SimpleXMLNode GetFirstChild();
	bool IsElementNode();
	
private:
	xmlNodePtr mpNode;
};


class SimpleXMLBuilder
{
public:
	SimpleXMLBuilder();
	~SimpleXMLBuilder();
	void Test();
	
	xmlNodePtr GetRoot();
	std::string ToString(bool iIncludeRootTag);
	
	static xmlNodePtr CreateNodePtr(const char* ipContent, bool iIsNewText = false);
	bool IsValid();
private:
	xmlDocPtr mpDoc;
	xmlNodePtr mpRoot;
};

class SimpleXMLParser
{
public:
	SimpleXMLParser();
	SimpleXMLParser(std::string iXMLString);
	~SimpleXMLParser();
	
	SimpleXMLNode GetRoot();
	bool IsValid();
	bool Parse(std::string iXMLString);
private:
	xmlDocPtr mpDoc;
	xmlNodePtr mpRoot;
};

#endif

