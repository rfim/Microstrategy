

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import "BarCodeCB.h"
#import "KeypadInputController.h"
//#import "ZBarImageScanner.h"

/* whether we are using an actual device or not. Simulator doesn't have the AVFoundation libraries for camera */
#if !TARGET_OS_EMBEDDED
	#define SIMULATOR_COMPILATION 1
#else
	#define SIMULATOR_COMPILATION 0
#endif

#if (SIMULATOR_COMPILATION || __IPHONE_OS_VERSION_MAX_ALLOWED < 40000)	
	#define CAN_USE_AVCAPTURE  0	
#else
	#define CAN_USE_AVCAPTURE 1
#endif

@class BarcodeScannerController;
@class ZBarImageScanner;
@class ZBarSymbol;

@protocol BarcodeScannerControllerDelegate <UINavigationControllerDelegate, UIImagePickerControllerDelegate>
- (void)barcodeScannerController:(BarcodeScannerController*)self didFinishScanning:(BarCodeCB *)result;
@end

#if CAN_USE_AVCAPTURE == 0
@interface BarcodeScannerController : UIViewController
<KeypadInputControllerDelegate>
#else

@interface BarcodeScannerController : UIViewController
<KeypadInputControllerDelegate, AVCaptureVideoDataOutputSampleBufferDelegate>
#endif
{
	BarCodeCB *readerCB;
	NSTimer *timer;
	NSString *codes[10];
	int codeIndex;
	BOOL isVertical;
	UIView *originalView;
	BOOL cameraActive;
	UIToolbar *toolBar;
	UIButton *cancelButton;
	UIImageView *scanView;
	UIImageView *lensCover;
	UITextField *textField;
	KeypadInputController *vwKeypadInput;
	UISegmentedControl *toggleCamera;
    UIBarButtonItem *segmentButton;
    UIView *keypadView;
	NSString *currentValue;
	id<BarcodeScannerControllerDelegate> __weak delegate;
	CGRect scanCrop;
	ZBarImageScanner *scanner;
	NSInteger maxScanDimension;
	ZBarSymbol *symbol;
	CALayer *boxLayer;
	UIView *boxView;
	BOOL scanSuccess;
	BOOL isOS4Available;
	BOOL statusBarHidden;
    CGRect customFrame;
    
#if CAN_USE_AVCAPTURE
	AVCaptureSession *session;
	AVCaptureVideoDataOutput *captureOutput;
	AVCaptureDeviceInput *captureInput;
	AVCaptureDevice *device;
	AVCaptureVideoPreviewLayer *prevLayer; 
#endif	
}

@property(nonatomic,weak) id<BarcodeScannerControllerDelegate> delegate;
@property(nonatomic, copy) NSString *currentValue;
@property CGRect customFrame;

#if CAN_USE_AVCAPTURE
// access to configure image scanner
@property (readonly, nonatomic) ZBarImageScanner *scanner;
#endif	

- (void) cancelScan;

+ (BOOL) isAvailable;

@end

