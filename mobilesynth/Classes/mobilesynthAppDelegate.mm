//
//  mobilesynthAppDelegate.m
//  mobilesynth
//
//  Created by Allen Porter on 12/7/08.
//  Copyright thebends 2008. All rights reserved.
//

#import "mobilesynthAppDelegate.h"
#import "mobilesynthViewController.h"

@implementation mobilesynthAppDelegate

@synthesize window;
@synthesize viewController;


- (void)applicationDidFinishLaunching:(UIApplication *)application {  
  // Override point for customization after app launch    
  [[UIApplication sharedApplication] setStatusBarHidden:YES];
  [window addSubview:viewController.view];
  [window makeKeyAndVisible];
}

- (void)dealloc {
  [viewController release];
  [window release];
  [super dealloc];
}


@end
