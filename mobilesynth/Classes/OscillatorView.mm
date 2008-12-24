//
//  OscillatorView.m
//  mobilesynth
//
//  Created by Allen Porter on 12/23/08.
//  Copyright 2008 thebends. All rights reserved.
//

#import "OscillatorView.h"
#include "synth/oscillator.h"

@implementation OscillatorView

@synthesize osc1Level;
@synthesize osc1Wave;
@synthesize osc1Octave;
@synthesize osc2Level;
@synthesize osc2Wave;
@synthesize osc2Octave;

@synthesize osc1;
@synthesize osc2;

- (synth::Oscillator::WaveType)waveTypeForButtonIndex:(int)index {
  switch (index) {
    case 0:
      return synth::Oscillator::SQUARE;
    case 1:
      return synth::Oscillator::TRIANGLE;
    case 2:
      return synth::Oscillator::SAWTOOTH;
    case 3:
      return synth::Oscillator::REVERSE_SAWTOOTH;
    default:
      NSLog(@"Unknown wave type: %d", index);
      return synth::Oscillator::SQUARE;
  }
}

- (void)changed:(id)sender {
  if (!osc1 || !osc2) {
    return;
  }
  
  // TODO(allen): Set octave
  osc1->set_level([osc1Level value]);
  osc1->set_wave_type(
      [self waveTypeForButtonIndex: [osc1Wave selectedSegmentIndex]]);
  osc2->set_level([osc2Level value]);
  osc2->set_wave_type(
      [self waveTypeForButtonIndex: [osc2Wave selectedSegmentIndex]]);
}

@end
