//
//  mobilesynthViewController.m
//  mobilesynth
//
//  Created by Allen Porter on 12/7/08.
//  Copyright thebends 2008. All rights reserved.
//

#import "mobilesynthViewController.h"

@implementation mobilesynthViewController

@synthesize keyboardScrollView;
@synthesize keyboardImageView;

- (void)viewDidLoad {
  [super viewDidLoad];
  // Setup the inner size of the scroll view to the size of the full keyboard
  // image.  This basically makes the scroll view work.
  [keyboardScrollView setContentSize:[[keyboardImageView image] size]];
  // TODO(allen): Start at the middle of the keyboard (Scroll to key?)
  
  // TODO(allen): Set this to disable scrolling, and enable sliding.
  //[keyboardScrollView setScrollEnabled:NO];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
  NSLog(@"auto");
  return (interfaceOrientation == UIInterfaceOrientationLandscapeRight);
}

- (void)didReceiveMemoryWarning {
  [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
  // Release anything that's not essential, such as cached data
}

- (void)dealloc {
  [super dealloc];
}

@end
