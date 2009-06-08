//
//  TouchForwardingUIScrollView.m
//  mobilesynth
//
//  Created by Allen Porter on 6/5/09.
//  Copyright 2009 thebends. All rights reserved.
//

#import "TouchForwardingUIScrollView.h"


@implementation TouchForwardingUIScrollView

@synthesize touchView;

- (UIView*)hitSubview:(NSSet *)touches withEvent:(UIEvent *)event {
  NSArray* touchArray = [touches allObjects];
  for (int i = 0; i < [touchArray count]; ++i) {
    UITouch* touch = [touchArray objectAtIndex:i];  
    CGPoint point = [touch locationInView:self];
    UIView *subview = [self hitTest:point withEvent:event];
    if (subview) {
      return subview;
    }
  }
  return nil;
}
/*
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
  [super touchesBegan:touches withEvent:event];
  UIView* subview = [self hitSubview:touches withEvent:event];
  if (subview) {
    [subview touchesBegan:touches withEvent:event];
  }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
  [super touchesMoved:touches withEvent:event];
  UIView* subview = [self hitSubview:touches withEvent:event];
  if (subview) {
    [touchView touchesMoved:touches withEvent:event];
  }
}
*/

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
  [super touchesEnded:touches withEvent:event];
  UIView* subview = [self hitSubview:touches withEvent:event];
  if (subview) {
    [touchView touchesEnded:touches withEvent:event];
  }
}

@end
