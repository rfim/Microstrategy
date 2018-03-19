//
//  File.swift
//  MicroStrategyMobile
//
//  Created by Ju, Wenting on 12/13/16.
//  Copyright © 2016 MicroStratgy Inc. All rights reserved.
//


import XCTest
import Foundation

class UITestServer: NSObject {
    
}

extension XCTestCase {
    fileprivate struct SessionData {
        static var uiTestServerAddress = "http://localhost:5000"
        
        static var session: URLSession?
    }
    
    public var uiTestServerAddress: String {
        get { return SessionData.uiTestServerAddress }
        set { SessionData.uiTestServerAddress = newValue }
    }
    
    var session: URLSession {
        get {
            if SessionData.session == nil {
                let sessionConfig = URLSessionConfiguration.ephemeral
                //SessionData.session = NSURLSession(configuration: sessionConfig, delegate: nil, delegateQueue: NSOperationQueue.mainQueue())
                SessionData.session = URLSession(configuration: sessionConfig)
            }
            return SessionData.session!
        }
    }
    
    func urlForEndpoint(_ endpoint: String, args: [String]) -> URL? {
        var urlString = "\(SessionData.uiTestServerAddress)/\(endpoint)"
        for arg in args {
            urlString += "/"
            urlString += arg.urlEncode()
            
        }
        let endpoint = URL(string: urlString)
        guard let url = endpoint else {
            XCTFail("Invalid URL: \(urlString)")
            return nil
        }
        return url
    }
    
    func dataFromRemoteEndpoint(_ endpoint: String, method: String, args: [String]) -> Data? {
        guard let url = urlForEndpoint(endpoint, args: args) else {
            return nil
        }
        
        let request = NSMutableURLRequest(url: url)
        request.httpMethod = method
        
        var result: Data? = nil
        let expectation = self.expectation(description: "dataTask")
        let dataTask = session.dataTask(with: request as URLRequest, completionHandler: { data, response, error in
            // WARNING: NOT a main queue
            if error != nil {
                XCTFail("dataTaskWithRequest error (please check if UITestServer is running): \(error)")
                return
            }
            if let httpResponse = response as? HTTPURLResponse {
                if httpResponse.statusCode != 200 {
                    XCTFail("dataTaskWithRequest: status code \(httpResponse.statusCode) received, please check if UITestServer is running")
                    return
                }
            }
            guard let responseData = data else {
                XCTFail("No data received (UITestServer not running?)")
                return
            }
            result = responseData
            expectation.fulfill()
        })
        dataTask.resume()
        waitForExpectations(timeout: 10.0, handler: nil)
        return result
    }
    
    func dataFromRemoteEndpoint(_ endpoint: String, method: String = "GET", args: String...) -> Data? {
        return dataFromRemoteEndpoint(endpoint, method: method, args: args)
    }
    
    func stringFromRemoteEndpoint(_ endpoint: String, method: String, args: [String]) -> String {
        let data = dataFromRemoteEndpoint(endpoint, method: method, args: args)
        if let stringData = data {
            let resolution = NSString(data: stringData, encoding: String.Encoding.utf8.rawValue) ?? ""
            return resolution as String
        }
        return ""
    }
    
    func stringFromRemoteEndpoint(_ endpoint: String, method: String = "GET", args: String...) -> String {
        return stringFromRemoteEndpoint(endpoint, method: method, args: args)
    }
    
    func callRemoteEndpoint(_ endpoint: String, method: String, args: [String]) {
        let _ = dataFromRemoteEndpoint(endpoint, method: method, args: args)
    }
    
    func callRemoteEndpoint(_ endpoint: String, method: String = "GET", args: String...) {
        callRemoteEndpoint(endpoint, method: method, args: args)
    }
    
    
}

extension String{
    func urlEncode() -> String {
        
        var result=String()
        
        let source = Array(self.characters)
        
        for currentChar in source {
            if currentChar == " " {
                
                result.append("+" as Character)
                
            } else if currentChar == "." || currentChar == "-" || currentChar == "_" || currentChar == "~" || (currentChar >= "a" && currentChar <= "z") || (currentChar >= "A" && currentChar <= "Z" ) || (currentChar >= "0" && currentChar <= "9") {
                
                result.append(currentChar)
                
            } else {
                
                result = result + (appendingFormat("%%%02X","\(currentChar)" ))
                
            }
            
        }
        
        
        return result
    }
    
}





