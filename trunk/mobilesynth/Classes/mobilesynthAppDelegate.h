//
//  mobilesynthAppDelegate.h
//  mobilesynth
//
//  Created by Allen Porter on 12/7/08.
//  Copyright thebends 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@class mobilesynthViewController;

@interface mobilesynthAppDelegate : NSObject <UIApplicationDelegate> {
  UIWindow *window;
  mobilesynthViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet mobilesynthViewController *viewController;

@end

