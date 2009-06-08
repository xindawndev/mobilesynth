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

@interface KeyDownInfo : NSObject {
  UITouch* touch;
  KeyView* keyView;
}
@property (nonatomic, retain) UITouch* touch;
@property (nonatomic, retain) KeyView* keyView;
@end

@implementation KeyDownInfo
@synthesize touch;
@synthesize keyView;
@end

@implementation KeyboardView

@synthesize keyboardDelegate;

// The MIDI note of the lowest note on the keyboard
static const int kLowC = 16;

- (id)initWithFrame:(CGRect)frame withOctaveCount:(int)count {
  if (self = [super initWithFrame:frame]) {    
    [self setMultipleTouchEnabled:YES];
    [self setOpaque:YES];
    
    keyDownSet = [[NSMutableSet setWithCapacity:0] retain];
    
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

- (void)dealloc {
  [super dealloc];
  [keyDownSet release];
}

- (KeyDownInfo*)findKey:(UITouch*)touch {
  NSArray* keyDownArray = [keyDownSet allObjects];
  for (int i = 0; i < [keyDownArray count]; ++i) {
    KeyDownInfo* keyDown = [keyDownArray objectAtIndex:i];
    if (keyDown.touch == touch) {
      return keyDown;
    }
  }
  return NULL;
}

- (KeyView*)keyViewForTouch:(UITouch*)touch withEvent:(UIEvent *)event{
  CGPoint point = [touch locationInView:self];
  return (KeyView*)[self hitTest:point withEvent:event];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
  NSArray* touchArray = [touches allObjects];
  for (int i = 0; i < [touchArray count]; ++i) {
    UITouch* touch = [touchArray objectAtIndex:i];  
    KeyDownInfo* keyPress = [self findKey:touch];
    if (keyPress) {
      NSLog(@"Unexpected; Touch already began: %@", touch);
      continue;
    }
    
    KeyView *keyView = [self keyViewForTouch:touch withEvent:event];
    if (!keyView) {
      continue;
    }
    [keyView keyDown];    
    [keyboardDelegate noteOn:[keyView keyNumber]];

    // Store the key for later access
    keyPress = [[KeyDownInfo alloc] init];
    keyPress.touch = touch;
    keyPress.keyView = keyView;
    [keyDownSet addObject:keyPress];
  }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
  NSArray* touchArray = [touches allObjects];
  for (int i = 0; i < [touchArray count]; ++i) {
    UITouch* touch = [touchArray objectAtIndex:i];  
    KeyDownInfo* keyPress = [self findKey:touch];
    if (!keyPress) {
      // We got forwarded a touch that did not start on the keyboard.  It might
      // be worth handling this (and the touch moved off a key case below)
      continue;
    }
    KeyView *keyView = [self keyViewForTouch:touch withEvent:event];
    if (keyPress.keyView == keyView) {
      // The touch moved, but did not change keys
      continue;
    }
    if (!keyView) {
      // The touch moved off of a key.  Do not update the current key pressed
      // and continue to play the same note.  The "off" event will be handled
      // in touchesEnded.
      continue;
    }

    // Press the new key, release the old key
    [keyView keyDown];
    [keyboardDelegate noteOn:[keyView keyNumber]];
    KeyView* previousKeView = keyPress.keyView;
    [previousKeView keyUp];
    [keyboardDelegate noteOff:[previousKeView keyNumber]];

    // Record the new key that is being pressed
    keyPress.keyView = keyView;
  }
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {  
  NSArray* touchArray = [touches allObjects];
  for (int i = 0; i < [touchArray count]; ++i) {    
    UITouch* touch = [touchArray objectAtIndex:i];  
    KeyDownInfo* keyPress = [self findKey:touch];
    if (!keyPress) {
      // The TouchForwardingUIScrollView may invoke us multiple times for the
      // same event as a workaround for its parent UIScrollView not always
      // invoking touchesEnded.      
      continue;
    }
    KeyView* previousKeyView = keyPress.keyView;
    [previousKeyView keyUp];
    [keyboardDelegate noteOff:[previousKeyView keyNumber]];
    
    // Stop tracking the touch event
    [keyDownSet removeObject:keyPress];
  }
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
  [self touchesEnded:touches withEvent:event];
}

@end
