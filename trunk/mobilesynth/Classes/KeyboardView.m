//
//  KeyboardView.m
//  mobilesynth
//
//  Created by Allen Porter on 12/13/08.
//  Copyright 2008 thebends. All rights reserved.
//

#import "KeyboardView.h"
#import "OctaveView.h"
#import "KeyView.h"

@implementation KeyboardView

@synthesize keyboardDelegate;

// The MIDI note of the lowest note on the keyboard
static const int kLowC = 16;

- (id)initWithFrame:(CGRect)frame withOctaveCount:(int)count {
  if (self = [super initWithFrame:frame]) {    
    [self setMultipleTouchEnabled:YES];
    
    // Slice the view into horizontal octaves
    CGRect octaveFrame;
    octaveFrame.origin.y = 0;
    octaveFrame.size.width = frame.size.width / count;
    octaveFrame.size.height = frame.size.height;
    int key = kLowC;
    for (int i = 0; i < count; ++i) {
      octaveFrame.origin.x = i * octaveFrame.size.width;
      OctaveView* octave = [[OctaveView alloc] initWithFrame:octaveFrame
                                                withKey:key];
      key += [octave keyCount];
      [self addSubview:octave];
    }
  }
  return self;
}

- (void)dealloc
{
  [super dealloc];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
  NSArray* touchArray = [touches allObjects];
  for (int i = 0; i < [touchArray count]; ++i) {
    UITouch* touch = [touchArray objectAtIndex:i];  
    CGPoint point = [touch locationInView:self];
    
    KeyView *key = (KeyView*)[self hitTest:point withEvent:event];
    [key keyDown];
    [keyboardDelegate noteOn:[key keyNumber]];
  }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
  NSArray* touchArray = [touches allObjects];
  for (int i = 0; i < [touchArray count]; ++i) {
    UITouch* touch = [touchArray objectAtIndex:i];  
    CGPoint point = [touch locationInView:self];
    CGPoint previousPoint = [touch previousLocationInView:self];
    
    KeyView *key = (KeyView*)[self hitTest:point withEvent:event];
    KeyView *previousKey = (KeyView*)[self hitTest:previousPoint withEvent:event];    
    if (key == previousKey) {
      // Nothing to do, movement within a key
      continue;
    }
    if (key) {
      [key keyDown];    
      [keyboardDelegate noteOn:[key keyNumber]];
    }
    if (previousKey) {
      [previousKey keyUp];
      [keyboardDelegate noteOff:[previousKey keyNumber]];
    }
  }
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
  NSArray* touchArray = [touches allObjects];
  for (int i = 0; i < [touchArray count]; ++i) {
    UITouch* touch = [touchArray objectAtIndex:i];  
    CGPoint point = [touch locationInView:self];
    KeyView *key = (KeyView*)[self hitTest:point withEvent:event];
    [keyboardDelegate noteOff:[key keyNumber]];
    [key keyUp];
  }
  if ([[event touchesForView:self] count] == 0) {
    // Sometimes we don't always get notified about all of the touches ending.
    // This is a failsafe to just shut everything off.
    [keyboardDelegate allOff];
    // Reset the UI
    int count = [[self subviews] count];
    for (int i = 0; i < count; ++i) {
      OctaveView* octaveView = (OctaveView*)[[self subviews] objectAtIndex:i];
      [octaveView reset];
    }
  }
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
  [self touchesEnded:touches withEvent:event];
}

- (BOOL)isOpaque {
  return YES;
}

@end
