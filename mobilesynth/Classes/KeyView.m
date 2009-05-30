//
//  KeyView.m
//  mobilesynth
//
//  Created by Allen Porter on 4/6/09.
//  Copyright 2009 thebends. All rights reserved.
//

#import "KeyView.h"


@implementation KeyView


- (id)initWithFrame:(CGRect)frame withKey:(int)keyNumber {
  if (self = [super initWithFrame:frame]) {
    [self setBackgroundColor:[UIColor clearColor]];
    [self setMultipleTouchEnabled:YES];
    key = keyNumber;
    keyPressed = FALSE;
  }
  return self;
}

- (void)dealloc {
    [super dealloc];
}

- (void)keyDown {
  if (!keyPressed) {
    keyPressed = TRUE;
    [self setNeedsDisplay]; 
  }
}

- (void)keyUp {
  if (keyPressed) {
    keyPressed = FALSE;
    [self setNeedsDisplay];
  }
}

- (int)keyNumber {
  return key;
}

@end
