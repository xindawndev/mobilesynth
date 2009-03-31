//
//  ControlScrollView.h
//  mobilesynth
//
//  Created by Allen Porter on 3/30/09.
//  Copyright 2009 thebends. All rights reserved.
//

#import <Foundation/Foundation.h>

// This class exists only to fix bugs in UIScrollView event tracking
@interface ControlScrollView : UIScrollView {
  // Keep track of touch events, and only forward them to our parent
  // if our notion of state matches the event state.
  BOOL touchDown;
}

@end
