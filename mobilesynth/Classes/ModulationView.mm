//
//  ModulationView.m
//  mobilesynth
//
//  Created by Allen Porter on 12/23/08.
//  Copyright 2008 thebends. All rights reserved.
//

#import "ModulationView.h"
#include "synth/oscillator.h"

@implementation ModulationView

@synthesize lfoRate;
@synthesize lfoAmount;
@synthesize lfoWave;
@synthesize lfo;

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
  lfo->set_level([lfoAmount value]);
  lfo->set_frequency([lfoRate value]);
  lfo->set_wave_type(
      [self waveTypeForButtonIndex:[lfoWave selectedSegmentIndex]]);
}

@end
