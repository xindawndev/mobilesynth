//
//  ;
//  mobilesynth
//
//  Created by Allen Porter on 4/5/09.
//  Copyright 2009 thebends. All rights reserved.
//

#import "OctaveView.h"
#import "BlackKeyView.h"
#import "WhiteKeyView.h"


@implementation OctaveView

// Number of white and black keys per octave
static const int kKeysPerOctave = 12;

// Size to scale from the height of a white key
static const float kBlackKeyHeightScale = 0.62;

// Offset in the X direction, within a white key
static const float kBlackKeyOffset = 0.68;

static const int kWhiteKeyNumbers[] = { 0, 2, 4, 5, 7, 9, 11 };
static const int kWhiteKeyCount = sizeof(kWhiteKeyNumbers) / sizeof(int);
static const int kBlackKey1Numbers[] = { 1, 3 };
static const int kBlackKey1Count = sizeof(kBlackKey1Numbers) / sizeof(int);
static const int kBlackKey2Numbers[] = { 6, 8, 10 };
static const int kBlackKey2Count = sizeof(kBlackKey2Numbers) / sizeof(int);

- (id)initWithFrame:(CGRect)frame withKey:(int)keyNumber {
  if (self = [super initWithFrame:frame]) {
    // The lowest level of the frame are the 7 white keys
    CGRect keyFrame;
    keyFrame.origin.y = 0;
    keyFrame.size.width = frame.size.width / kWhiteKeyCount;
    keyFrame.size.height = frame.size.height;
    
    // Add all of the white keys
    for (int i = 0; i < kWhiteKeyCount; ++i) {
      keyFrame.origin.x = i * keyFrame.size.width;
      WhiteKeyView* key =
        [[WhiteKeyView alloc] initWithFrame:keyFrame
                                    withKey:keyNumber + kWhiteKeyNumbers[i]];
      [self addSubview:key];
    }
    
    // Add two black keys for C# and D#
    keyFrame.origin.x = kBlackKeyOffset * keyFrame.size.width;
    keyFrame.size.height = kBlackKeyHeightScale * frame.size.height;
    for (int i = 0; i < kBlackKey1Count; ++i) {
      BlackKeyView* key =
          [[BlackKeyView alloc] initWithFrame:keyFrame
                                      withKey:keyNumber + kBlackKey1Numbers[i]];
      [self addSubview:key];
      keyFrame.origin.x += keyFrame.size.width;
    }

    // Add the next 3 black keys (F#, G#, A#)
    keyFrame.origin.x += keyFrame.size.width;
    for (int i = 0; i < kBlackKey2Count; ++i) {
      BlackKeyView* key =
          [[BlackKeyView alloc] initWithFrame:keyFrame
                                      withKey:keyNumber + kBlackKey2Numbers[i]];
      [self addSubview:key];
      keyFrame.origin.x += keyFrame.size.width;
    }
  }
  return self;
}

- (void)dealloc {
  [super dealloc];
}

- (int)keyCount {
  return kKeysPerOctave;
}

- (void)reset {
  int count = [[self subviews] count];
  for (int i = 0; i < count; ++i) {
    KeyView* keyView = (KeyView*)[[self subviews] objectAtIndex:i];
    [keyView keyUp];
  }
}

- (BOOL)isOpaque {
  return YES;
}

@end
