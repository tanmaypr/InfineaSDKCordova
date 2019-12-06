// IPCIQ

// Apple Frameworks
#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>

// IPCIQ headers
#import <InfineaSDK/InfineaSDK.h>
#import <InfineaSDK/IQExtension.h>
#import <InfineaSDK/IQCustomDevice.h>
#import <InfineaSDK/IQTransactionReceipt.h>

// Service constants
extern NSString * const kIPC_OPTIC_DECODER_SERVICE;

/// Checkin completion block
typedef void(^CompletionBlock)(BOOL isSuccess, NSData *data, NSURLResponse *response, NSError *error);

// ********************************* PROTOCOLS *******************************
@protocol IPCIQDelegate <NSObject>
@optional
/**
 Get IPC Devices info
 @param devicesInfo contains IPC Device infos pass along to delegate
 */
- (void)ipcDevicesInfo:(NSArray *)devicesInfo;

/**
 The response status for Company ID
 @param isSuccess If true, the company ID is successfully added, else the company ID is wrong
 @param error The error message if error, else is nil
 */
- (void)companyIDStatus:(BOOL)isSuccess error:(NSError *)error;

/**
 The current status message of IPCIQ checkin process.
 @param statusMessage The status message
 */
- (void)ipciqStatus:(NSString *)statusMessage;

/**
 The report that gets send back to IPCIQ with the status of each configuration. You can read this to see which config has been changed.
 @param report The dictionary that contains the report
 */
- (void)configsDidUpdateWithReport:(NSDictionary *)report;

/**
 Status of setting device's tag.
 @param isSuccess The BOOL value status
 @param error If there is error, the error will be passed here
 */
- (void)deviceTagStatus:(BOOL)isSuccess error:(NSError *)error;

@end
// ******************************** END PROTOCOLS *******************************


// ******************************* INTERFACES ***********************************
/**
 A class that handles key check, device's diagnostic information, and configuration.
 */
@interface IPCIQ : NSObject

// The state of IPCIQ whether it is checking in or not
@property (nonatomic, readonly) BOOL isCheckingIn;

// Allow location request or skip completely. Default is False.
@property (nonatomic) BOOL locationEnabled;

// Allow check-in or skip completely. Default is True.
@property (nonatomic) BOOL checkInEnabled;

/**
 Get the shared instance of IPCIQ. This will also start IPCIQ timer to check in
 @return IPCIQ shared object
 */
+ (instancetype)registerIPCIQ;

/**
 A developer key is needed in order to use the framework. Please contact IPC for a developer key. This key needs to be set BEFORE calling any other methods, preferrably in application:didFinishLaunchingWithOptions:, or anywhere prior to
 @param key The developer key.
 @param error If there is an error with the developer key, the error object will be returned here.
 @return Boolean A response to indicate the developer key is valid or not.
 */
- (BOOL)setDeveloperKey:(NSString *)key withError:(NSError *__autoreleasing *)error;

/**
 A boolean indicate if the framework has a developer key and is valid.
 @return boolean True if valid, false otherwise
 */
- (BOOL)isValidated;

/**
 Check if your license key is valid for the specific service
 @param license The license key provided by IPC.
 @param service The service that the key was generated for.
 */
- (NSError *)validateLicense:(NSString *)license forService:(NSString *)service;

/**
 Convert NSDate to UTC time string
 @param adate Take in an NSDate
 @return NSString of UTC time
 */
- (NSString *)convertTimeToUTC:(NSDate *)adate;

/**
 Pause IPCIQ from checking in. This only works if the app is Active. When app goes to background, this pause will be released, and IPCIQ will be resumed.
 */
- (void)pause;

/**
 Resume IPCIQ to continue check in
 */
- (void)resume;

/**
 Get all the added extensions
 @return NSArray IQExtension objects
 */
- (NSArray *)getExtensions;

/**
 Get all connected devices info
 @return NSArray The array contains all devices info
 */
- (NSArray *)getConnectedDeviceInfo;

/**
 Add an instance of IQExtension into the checkin data to be sent to IPCIQ
 @param extension An instance of IQExtension
 */
- (void)addExtension:(IQExtension *)extension;

/**
 Remove an instance of IQExtension from the list of checkin data
 @param extension An instance of IQExtension
 */
- (void)removeExtension:(IQExtension *)extension;

/**
 Add an instance of IQCustomDevice into the checkin data to be sent to IPCIQ
 @param customDevice An instance of IQCustomDevice
 */
- (void)addCustomDevice:(IQCustomDevice *)customDevice;

/**
 Remove an instance of IQCustomDevice from the list of checkin data
 @param customDevice An instance of IQCustomDevice
 */
- (void)removeCustomDevice:(IQCustomDevice *)customDevice;

/**
 Set check in timer interval, in seconds.
 @param interval Number of seconds until next check in
 */
- (void)setCheckInInterval:(NSTimeInterval)interval;

/**
 Get check in interval, in seconds
 @return NSInteger of check in interval
 */
- (NSInteger)getCheckInInterval;

/**
 Set config check in timer interval, in seconds.
 @param interval Number of seconds until next config check in
 */
- (void)setConfigCheckInInterval:(NSTimeInterval)interval;

/**
 Get current config check in interval, in seconds
 @return NSInteger Seconds of config check-in interval
 */
- (NSInteger)getConfigCheckInInterval;

/**
 Convert the raw identifier device name to a pretty device name
 @param rawName The raw name read from DTDevice.deviceName
 @return NSString of pretty device name
 */
+ (NSString *)getDeviceNameFromRawName:(NSString *)rawName;

/**
 Get the image of the corresponding device
 @param deviceName the raw device name read from DTDevice.deviceName
 @return UIImage of the corresponding device
 */
+ (UIImage *)getImageOfDevice:(NSString *)deviceName;

/**
 Set device's tag
 @param tag The NSString tag name
 */
- (void)setDeviceTag:(NSString *)tag;

/**
 Set location upload interval in seconds
 @param seconds The upload interval
 */
- (void)setLocationUploadInterval:(NSInteger)seconds;

/**
 Set location upload interval in seconds
 @return seconds The location upload interval
 */
- (NSInteger)getLocationUploadInterval;

/**
 If set to YES, the next checkin will be allowed to upload, regardless of checkin interval
 @param permission State of permission
 */
- (void)allowNextCheckinUpload:(BOOL)permission;

/**
 If set to YES, the next location upload will be allowed to upload with last successful location ping, regardless of location upload interval
 @param permission State of permission
 */
- (void)allowNextLocationUpload:(BOOL)permission;

/**
 Checkin devices
 @param complete The completion block
 */
- (void)checkInWithCompletionBlock:(CompletionBlock)complete;

/**
 Get device UUID assigned by IPCIQ
 @return NSString contains UUID
 */
- (NSString *)getUUID;

/**
 Add an object to be one of IPCIQ's delegates
 @param object The delegate object
 */
- (void)addDelegate:(id)object;

/**
 Remove an object from being IPCIQ's delegate
 @param object The object to be removed
 */
- (void)removeDelegate:(id)object;

/**
 Perform fetch when application wake up by iOS
 @param completionHandler The completion handler pass in from app delegate
 */
- (void)performFetchWithCompletionHandler:(void (^)(UIBackgroundFetchResult))completionHandler;

/**
 IPCIQ handling remote notification registration successful
 @param deviceToken Send from remote server contains the device token to use for push notification
 */
- (void)didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken;

/**
 Register IPCIQ's push notification. Call this in application did finish launch
 */
- (void)registerPushNotification;

/**
 Let IPCIQ handles the didReceiveRemoteNotification. You can continue to implement your custom handling after calling this
 @param userInfo The dictionary passing in from didReceiveRemoteNotification:userInfo app delegate
 @param completionHandler The completion handler pass in from app delegate
 */
- (void)didReceiveRemoteNotification:(NSDictionary *)userInfo fetchCompletionHandler:(void (^)(UIBackgroundFetchResult))completionHandler;

/**
Upload transaction receipt
@param receipt The transaction receipt
*/
- (void)uploadTransactionReceipt:(IQTransactionReceipt *)receipt;



// UNAVAILABLE
- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;


@end
