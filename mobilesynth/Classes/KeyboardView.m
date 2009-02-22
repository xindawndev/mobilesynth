//
//  KeyboardView.m
//  mobilesynth
//
//  Created by Allen Porter on 12/13/08.
//  Copyright 2008 thebends. All rights reserved.
//

#import "KeyboardView.h"


@implementation KeyboardView

@synthesize keyboardDelegate;

- (id)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
      currentNote = 0;
    }
    return self;
}

- (void)dealloc {
    [super dealloc];
}


- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
  UITouch* touch = [touches anyObject];
  CGPoint point = [touch locationInView:self];
  currentNote = [self noteAtPoint:point];
  [keyboardDelegate noteBegin:currentNote]; 
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
  UITouch* touch = [touches anyObject];
  CGPoint point = [touch locationInView:self];
  int note = [self noteAtPoint:point];
  if (note != currentNote) {
    currentNote = note;
    [keyboardDelegate noteChange:note];
  }
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
  currentNote = 0;
  [keyboardDelegate noteEnd];
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
  currentNote = 0;
  [keyboardDelegate noteEnd];
}


//
// Determines the key at the specified point in the image.  The keyboard is
// laid out vertically.
//

// TODO(allen): The keyboard range has been limited temporarily to workaround
// the fact that it is drawn as a single image which is too large to render
// when extended with more octaves.  Fix!

// The "width" of each white key
static const float kKeySize = 57.14;

// An octive is 7 white keys
static const float kOctaveSize = 57.14 * 7;
static const float kKeysPerOctave = 12;

// The X position that marks the start of the black keys
static const int kBlackKeyEndY = 102;

// Offsets of the double black keys
static const int kBlackKeyPairStartX = 39;
static const int kBlackKeyPairEndX = 152;

// Offsets of the triple black keys
static const int kBlackKeyTrioStartX = 211;
static const int kBlackKeyTrioEndX = 382;

static const int kLowC = 28;

- (int)octaveAtPoint:(CGPoint)point {
  return point.x / kOctaveSize;
}

- (int)positionInOctaveAtPoint:(CGPoint)point {
  return point.x - [self octaveAtPoint:point] * kOctaveSize;
}

// Note C is 0
- (int)keyInFirstOctave:(CGPoint)point {
  if (point.y < kBlackKeyEndY &&
      point.x > kBlackKeyPairStartX &&
      point.x <= kBlackKeyPairStartX + kKeySize) {
    return 1;
  }
  if (point.x <= kKeySize) {
    return 0;
  }
  if (point.y < kBlackKeyEndY &&
      point.x > kBlackKeyPairStartX + kKeySize &&
      point.x <= kBlackKeyPairStartX + 2 * kKeySize) {
    return 3;
  }
  if (point.x <= 2 * kKeySize) {
    return 2;
  }
  if (point.x <= 3 * kKeySize) {
    return 4;
  }  
  if (point.y < kBlackKeyEndY &&
      point.x > kBlackKeyTrioStartX &&
      point.x <= kBlackKeyTrioStartX + kKeySize) {
    return 6;
  }
  if (point.x <= 4 * kKeySize) {
    return 5;
  }  
  if (point.y < kBlackKeyEndY &&
      point.x > kBlackKeyTrioStartX + kKeySize &&
      point.x <= kBlackKeyTrioStartX + 2 * kKeySize) {
    return 8;
  }
  if (point.x <= 5 * kKeySize) {
    return 7;
  }  
  if (point.y < kBlackKeyEndY &&
      point.x > kBlackKeyTrioStartX + 2 * kKeySize &&      
      point.x <= kBlackKeyTrioStartX + 3 * kKeySize) {
    return 10;
  }
  if (point.x <= 6 * kKeySize) {
    return 9;
  }  
  return 11;
}

- (int)noteAtPoint:(CGPoint)point {
  int octave = [self octaveAtPoint:point];  
  int keys_below_octave = octave * kKeysPerOctave;
  // Adjust the point so that the x-coordinate is relative to the octave, then
  // get the key within the octave.
  point.x = [self positionInOctaveAtPoint:point] - 1;
  int key = [self keyInFirstOctave:point];
  // The lowest key on
  return kLowC + keys_below_octave + key;
}

@end
