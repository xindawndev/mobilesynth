//
//  ControlScrollView.m
//  mobilesynth
//
//  Created by Allen Porter on 3/30/09.
//  Copyright 2009 thebends. All rights reserved.
//

#import "ControlScrollView.h"


@implementation ControlScrollView

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
  // Don't invoke more than once
  if (!touchDown) {
    touchDown = YES;
    [super touchesBegan:touches withEvent:event];
  }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
  // Don't invoke spuriously
  if (!touchDown) {
    return;
  }
  [super touchesMoved:touches withEvent:event];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
  // Don't invoke spuriously
  if (touchDown) {
    touchDown = NO;
    [super touchesEnded:touches withEvent:event];
  }
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
  // Do not forward these events at all
}


@end
