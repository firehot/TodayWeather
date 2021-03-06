//
//  TodayViewController.h
//  widget
//
//  Created by KwangHo Kim on 2016. 5. 28..
//
//

/********************************************************************
 Declare include files
 ********************************************************************/
#import <UIKit/UIKit.h>

#import	<CoreLocation/CoreLocation.h>
#import "TodayWeatherShowMore.h"

@interface CityInfo : NSObject
@property (nonatomic) id identifier;
@property (nonatomic) NSString *address;
@property (nonatomic) BOOL currentPosition;
@property (nonatomic) int index;
@property (nonatomic) NSDictionary *weatherData;
- (void) encodeWithCoder : (NSCoder *)encode ;
- (id) initWithCoder : (NSCoder *)decode;
@end

@class TodayViewController;
@class TodayWeatherShowMore;

/********************************************************************
 Declare Class Definitions
 ********************************************************************/
@interface TodayViewController : UIViewController <CLLocationManagerDelegate>
{
    IBOutlet UILabel        *addressLabel;
    IBOutlet UILabel        *updateTimeLabel;
    IBOutlet UIActivityIndicatorView    *loadingIV;
    
    IBOutlet UILabel        *curTempLabel;              // current temperature title
    IBOutlet UILabel        *curDustLabel;            // current Dust

    IBOutlet UIImageView    *curWTIconIV;            // current weather status
    
    IBOutlet UILabel        *todayMaxMinTempLabel;         // today Max/Min Temperature
        
    IBOutlet UIButton       *editWidgetBtn;
    IBOutlet UIButton       *updateDataBtn;
    
    __weak IBOutlet UIView *locationView;
    IBOutlet UIView         *noLocationView;                // No Location View
    IBOutlet UILabel        *descLabel;                     // descLabel
    IBOutlet UIButton       *noLocEditWidgetBtn;
    
    __weak IBOutlet UIButton *nextCityBtn;

    NSMutableArray          *mCityList;
    NSMutableArray          *mCityDictList;                 // for sealization을 위한 
    CityInfo                *mCurrentCity;
    int                     mCurrentCityIdx;
    BOOL                    bIsReqComplete;
    
    // current postion
    double								gMylatitude;
    double								gMylongitude;
    
    CLLocationManager					*locationManager;
    CLLocation							*startingPoint;
    
    NSMutableData                       *responseData;
    
    
    BOOL                                bIsDateView;
    __weak IBOutlet UIView *showMoreView;
    
    TodayWeatherShowMore        *todayWSM;
    NSDictionary                *curJsonDict;
    
@public
    __weak IBOutlet UILabel *time1Label;
    __weak IBOutlet UILabel *time2Label;
    __weak IBOutlet UILabel *time3Label;
    __weak IBOutlet UILabel *time4Label;
    __weak IBOutlet UILabel *time5Label;
    __weak IBOutlet UILabel *time6Label;
    
    __weak IBOutlet UILabel *temp1Label;
    __weak IBOutlet UILabel *temp2Label;
    __weak IBOutlet UILabel *temp3Label;
    __weak IBOutlet UILabel *temp4Label;
    __weak IBOutlet UILabel *temp5Label;
    __weak IBOutlet UILabel *temp6Label;
    
    __weak IBOutlet UIImageView *showMore1IV;
    __weak IBOutlet UIImageView *showMore2IV;
    __weak IBOutlet UIImageView *showMore3IV;
    __weak IBOutlet UIImageView *showMore4IV;
    __weak IBOutlet UIImageView *showMore5IV;
    __weak IBOutlet UIImageView *showMore6IV;
}

/********************************************************************
 Declare Class properties
 ********************************************************************/
@property (retain, nonatomic) CLLocationManager					*locationManager;
@property (retain, nonatomic) CLLocation						*startingPoint;
@property (retain, nonatomic) NSMutableData						*responseData;
@property (retain, nonatomic) IBOutlet UIActivityIndicatorView    *loadingIV;

/********************************************************************
 Declare Class functions
 ********************************************************************/
+ (TodayViewController *)sharedInstance;

- (IBAction) editWidget:(id)sender;
- (IBAction) updateData:(id)sender;
- (IBAction)toggleShowMore:(id)sender;
- (IBAction)moveMainApp:(id)sender;


- (IBAction)nextCity:(id)sender;

- (void) initWidgetDatas;
- (void) initLocationInfo;
- (void) refreshDatas;
- (void) getAddressFromDaum:(double)latitude longitude:(double)longitude;
- (void) requestAsyncByURLSession:(NSString *)nssURL reqType:(NSUInteger)type;
- (void) makeJSONWithData:(NSData *)jsonData reqType:(NSUInteger)type;
- (void) parseJSONData:(NSDictionary *)jsonDict;
- (void) processWeatherResultsWithShowMore:(NSDictionary *)jsonDict;
- (NSString *) makeRequestURL:(NSString *)nssAddr1 addr2:(NSString*)nssAddr2 addr3:(NSString *)nssAddr3;
- (BOOL) setCityInfo:(CityInfo *)nextCity;

- (void) processPrevData:(int)idx;
- (void) processRequestIndicator:(BOOL)isComplete;
- (void) processShowMore;

- (NSDictionary *) getCurJsonDict;
- (void) setCurJsonDict:(NSDictionary *)dict;

@end
