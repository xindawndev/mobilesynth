//
//  ArpeggioView.m
//  mobilesynth
//
//  Created by Allen Porter on 4/19/09.
//  Copyright 2009 thebends. All rights reserved.
//

#import "ArpeggioView.h"
#include "synth/controller.h"

@implementation ArpeggioView

@synthesize rate;
@synthesize enabled;
@synthesize octaves;
@synthesize step;
@synthesize controller;

static const float kSampleRate = 44100.0;
static const long kArpeggioMaxSamples = kSampleRate;  // 1 second
static const long kArpeggioMinSamples = kSampleRate / 8;  // 0.125 seconds

- (long)arpeggioSamples:(float)value {
  return value * (kArpeggioMinSamples - kArpeggioMaxSamples) +
      kArpeggioMaxSamples;
}

- (synth::Arpeggio::Step)stepForButtonIndex:(int)index {
  switch (index) {
    case 0:
      return synth::Arpeggio::UP;
    case 1:
      return synth::Arpeggio::DOWN;
    case 2:
      return synth::Arpeggio::UP_DOWN;
    case 3:
      return synth::Arpeggio::RANDOM;
    default:
      NSLog(@"Unknown step type: %d", index);
      return  synth::Arpeggio::UP;
  }
}

- (void)changed:(id)sender {
  controller->set_arpeggio_enabled([enabled isOn]);
  controller->set_arpeggio_octaves([octaves selectedSegmentIndex] + 1);
  controller->set_arpeggio_samples([self arpeggioSamples:[rate value]]);
  controller->set_arpeggio_step(
      [self stepForButtonIndex:[step selectedSegmentIndex]]);
}

@end
