//
//  EnvelopeView.m
//  mobilesynth
//
//  Created by Allen Porter on 12/23/08.
//  Copyright 2008 thebends. All rights reserved.
//

#import "EnvelopeView.h"
#include "synth/envelope.h"

@implementation EnvelopeView

@synthesize attack;
@synthesize decay;
@synthesize sustain;
@synthesize release;
@synthesize envelope;

static const float kSampleRate = 44100.0;

// Convert seconds to number of samples
- (long)samples:(float)seconds {
  float result = kSampleRate * seconds;
  return (long)result;
}

- (void)changed:(id)sender {
  envelope->set_attack([self samples:[attack value]]);
  envelope->set_decay([self samples:[decay value]]);
  envelope->set_sustain([sustain value]);
  envelope->set_release([self samples:[release value]]);
}

@end
