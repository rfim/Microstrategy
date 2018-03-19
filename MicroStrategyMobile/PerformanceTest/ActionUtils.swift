
//
//  ActionUtils.swift
//  MicroStrategyMobile
//
//  Created by Administrator on 10/13/15.
//  Copyright © 2015 MicroStrategy Inc. All rights reserved.
//

import UIKit
import XCTest

let url: String = "mstr://?url=http%3A%2F%2F10.197.32.131%3A8080%2FMicroStrategyMobile%2Fservlet%2FtaskProc%3FtaskId%3DgetMobileConfiguration%26taskEnv%3Dxml%26taskContentType%3Dxmlanf%26configurationID%3D9baab8fd-d419-44f8-afae-51a9620113f5&authMode=1&dt=2"

extension XCUIElement{
    
    public func elementContainsStringWithType(_ text: String, type: XCUIElementType, index: Int) -> XCUIElement{
        
        let selectedElement: NSMutableArray = NSMutableArray()
        
        let elementQuery = XCUIApplication().descendants(matching: type)
        
        let count = elementQuery.count
        
        for i in 0..<Int(count) {
            let element = elementQuery.element(boundBy: UInt(i))
            if(element.title.contains(text)){
                selectedElement.add(element)
            }
        }
      //  let targetElement: XCUIElement = selectedElement.objectAtIndex(index) as! XCUIElement
        
        return selectedElement.object(at: index) as! XCUIElement
        
    }
}


open class ActionUtils: XCTestCase {

    open class func sendPerformanceLog(IPAddress: String, test: XCTestCase, app: XCUIApplication) {

        self.verifyDisappearWithTimeout("Network connection in progress", application: app, type: .other, test: test, timeout: 900)
        _ = test.dataFromRemoteEndpoint("Performance", args: IPAddress);
        _ = test.dataFromRemoteEndpoint("clearLog", args: "");
    }
    
    open class func getMemory(IPAddress: String, message: String, test: XCTestCase, app: XCUIApplication) {

        self.verifyDisappearWithTimeout("Network connection in progress", application: app, type: .other, test: test, timeout: 900)
        _ = test.dataFromRemoteEndpoint("Memory", args: message)


    }
    
    
    open class func scrollAndTap(_ textName: String, application: XCUIApplication, test:XCTestCase){
        
        let progress = application.activityIndicators["In progress"]
        
        let expectAppear = NSPredicate(format: "exists = 0")
        test.expectation(for: expectAppear, evaluatedWith: progress, handler: nil)
        test.waitForExpectations(timeout: 60, handler: nil)
        
        sleep(2)
        
        let element = application.tables.staticTexts[textName]
        
        let table = application.tables.containing(.staticText, identifier: textName)
        let elementQuery = table.staticTexts
        let lastElement = elementQuery.element(boundBy: elementQuery.count - 1)
        let elementNumber = elementQuery.count
        
        
        var swipeUp: Bool = true
        
        
        while (element.frame.origin.y == 0.0){
            
            if (swipeUp){
                table.element.swipeUp()
            }else{
                table.element.swipeDown()
            }
            
            if (lastElement.frame.origin.y != 0.0){
                swipeUp = false
                
            }
            if (elementQuery.element(boundBy: 0).frame.origin.y != 0.0){
                swipeUp = true
            }
            
            sleep(2)
        }
    
       // self.printDescription(application)
        while (element.exists && elementQuery.count == elementNumber) {
            element.tap()
            sleep(2)
        }
        
    }
    
    open class func verifyAndTapElement(_ name: String, application: XCUIApplication, type: XCUIElementType, test: XCTestCase, tap: Bool){
        let element = application.descendants(matching: .any).matching(type, identifier: name).element
        
        let expectAppear = NSPredicate(format: "exists = 1")
        test.expectation(for: expectAppear, evaluatedWith: element, handler: nil)
        test.waitForExpectations(timeout: 60, handler: nil)
        
        if (tap){
            element.tap()
        }
        
    }
    
    open class func verifyAppearWithTimeout(_ name: String, application: XCUIApplication, type: XCUIElementType, test: XCTestCase, timeout: TimeInterval){
        let element = application.descendants(matching: .any).matching(type, identifier: name).element
        
        let expectAppear = NSPredicate(format: "exists = 1")
        test.expectation(for: expectAppear, evaluatedWith: element, handler: nil)
        test.waitForExpectations(timeout: timeout, handler: nil)
    }
    
    open class func verifyDisappearWithTimeout(_ name: String, application: XCUIApplication, type: XCUIElementType, test: XCTestCase, timeout: TimeInterval){
        let element = application.descendants(matching: .any).matching(type, identifier: name).element
        
        let expectAppear = NSPredicate(format: "exists = 0")
        test.expectation(for: expectAppear, evaluatedWith: element, handler: nil)
        test.waitForExpectations(timeout: timeout, handler: nil)
    }
    
    
    
    open class func quitFullScreenAndClose(_ application: XCUIApplication, test:XCTestCase){
        application.buttons["Exit full screen mode."].tap()
        self.verifyAndTapElement("Close", application: application, type: .button, test: test, tap: true)
    }
    
    //The position CGPoint is the offset x/y of basic element origin. 
    open class func tapAtPosition(position: CGPoint, element: XCUIElement) {
       // let element = app.descendantsMatchingType(.Any).matchingType(type, identifier: name).element
        let coordinate = element.coordinate(withNormalizedOffset: CGVector(dx: 0, dy: 0)).withOffset(CGVector(dx: position.x, dy: position.y))

        coordinate.tap()
    }
    
    open class func clearCache(_ app: XCUIApplication, test:XCTestCase){
        let element = app.staticTexts["SETTINGS"]
        self.verifyAndTapElement("fb_hamburger", application: app, type: .button, test: test, tap: true)
        
        while !element.isHittable {
            self.verifyAndTapElement("fb_hamburger", application: app, type: .button, test: test, tap: true)
            sleep(2)
        }

       // app.swipeRight()
        element.tap()
        self.verifyAndTapElement("Clear Cached Data", application: app, type: .staticText, test: test, tap: true)
        self.verifyAndTapElement("Clear", application: app, type: .button, test: test, tap: true)
        if(!app.alerts.element.exists){
            app.buttons["Close"].tap()
        }
        
    }
    
    open class func resetTransaction(_ app: XCUIApplication, test: XCTestCase){
        
        self.verifyAndTapElement("Transactions(SingleGridSource=False)", application: app, type: .button, test: test, tap: true)
        self.scrollAndTap("Test Suite", application: app, test: test)
        
        self.scrollAndTap("RestoreData", application: app, test: test)
        self.verifyAndTapElement("Switch to Graph View", application: app, type: .button, test: test, tap: false)
        
        self.verifyAndTapElement("Close", application: app, type: .button, test: test, tap: true)
        
        _ = test.dataFromRemoteEndpoint("clearLog", args: "")
        
    }
    
    
    open class func changeOfflineMode(_ app: XCUIApplication, test:XCTestCase){
        self.verifyAndTapElement("fb_hamburger", application: app, type: .button, test: test, tap: true)
        self.verifyAndTapElement("SETTINGS", application: app, type: .staticText, test: test, tap: true)
        
        //self.printDescription(app)
        app.switches["Offline Mode"].tap()
        app.buttons["Close"].tap()
        
    }
    
    open class func transactionQueue(_ app: XCUIApplication, test:XCTestCase){
        self.verifyAndTapElement("fb_hamburger", application: app, type: .button, test: test, tap: true)
        self.verifyAndTapElement("Transaction Queue", application: app, type: .staticText, test: test, tap: true)
        
    }
    
    open class func terminateAndRelaunch(_ app: XCUIApplication){
        app.terminate()
        app.launch()
        
        sleep(7)
    }
    
    open class func printDescription(_ app: XCUIApplication){
        sleep(2)
        let element = app.scrollViews.otherElements["Image View"]
        if (!element.exists){
            print(app.debugDescription)
        }
    }
    
    open class func clearServerCache(_ urlAddress: String, dataString: String){
        
        let url = URL(string: urlAddress)
        let urlRequest = NSMutableURLRequest(url: url!)
        urlRequest.httpMethod = "POST"
        let data = dataString.data(using: .utf8)
        urlRequest.httpBody = data
        
       // let task = URLSession.shared.dataTask(with: urlRequest as URLRequest)
        
        let task1 = URLSession.shared.dataTask(with: urlRequest as URLRequest, completionHandler: { data, response, error in
            // WARNING: NOT a main queue
            if error != nil {
            XCTFail("Error: \(error)")
            return
            }
            
            })

        task1.resume()
        
    }
    
    open class func changeLogLevel(_ from: String, to: String, app: XCUIApplication, test: XCTestCase){
        let element = app.staticTexts["SETTINGS"]
        self.verifyAndTapElement("fb_hamburger", application: app, type: .button, test: test, tap: true)
        
        while !element.isHittable {
            self.verifyAndTapElement("fb_hamburger", application: app, type: .button, test: test, tap: true)
            sleep(2)
        }
        
        // app.swipeRight()
        element.tap()
        
       // self.printDescription(app)
        app.staticTexts["Logging"].tap()
       // let eleString = String(format: "Level, %@", from)
       // self.printDescription(app)
        
        let fromElement = app.staticTexts[from]
        let toElement = app.staticTexts[to]
        
        if (fromElement.exists){
        
            app.staticTexts[from].tap()
            
            app.staticTexts[to].tap()
          //  self.printDescription(app)
            app.buttons.matching(identifier: "Back").element(boundBy: 0).tap()
            app.buttons.matching(identifier: "Back").element(boundBy: 0).tap()
            app.buttons["Close"].tap()
        }else if(toElement.exists){
            app.buttons.matching(identifier: "Back").element(boundBy: 0).tap()
            app.buttons["Close"].tap()
        }
        
        
    }
    
    open class func openGD(_ app: XCUIApplication, pwd: String, test: XCTestCase, email: String, accessKey: String){
        
        let element1 = app.textFields["Email Address"]
        
        //self.printDescription(app)
        
        self.verifyAndTapElement("Go", application: app, type: .button, test: test, tap: false)
        
        if element1.exists {
            element1.tap()
            app.typeText(email)
            app.buttons["Go"].tap()
            
            app.typeText(accessKey)
            app.buttons["Go"].tap()
            
            self.verifyAndTapElement("New Password", application: app, type: .secureTextField, test: test, tap: false)
            
            app.typeText(pwd)
            app.buttons["Go"].tap()
            app.typeText(pwd)
            app.buttons["Go"].tap()
            
            app.terminate()
            app.launchEnvironment["UITestingURL"] = url
            
            app.launch()
            
        }
        
        self.verifyAndTapElement("Enter Password", application: app, type: .secureTextField, test: test, tap: false)
        
        app.typeText(pwd)
        
        app.buttons["Go"].tap()
        
    }

}
