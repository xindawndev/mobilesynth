//
//  OscillatorView.m
//  mobilesynth
//
//  Created by Allen Porter on 12/23/08.
//  Copyright 2008 thebends. All rights reserved.
//

#import "OscillatorView.h"
#include "synth/controller.h"

@implementation OscillatorView

@synthesize osc1Level;
@synthesize osc1Wave;
@synthesize osc1Octave;
@synthesize osc2Level;
@synthesize osc2Wave;
@synthesize osc2Octave;

@synthesize glideAmount;

@synthesize controller;

static const float kSampleRate = 44100.0;

- (long)samples:(float)seconds {
  float result = kSampleRate * seconds;
  return (long)result;
}

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

- (synth::Controller::OctaveShift)octaveForButtonIndex:(int)index {
  switch (index) {
    case 0:
      return synth::Controller::OCTAVE_1;
    case 1:
      return synth::Controller::OCTAVE_2;
    case 2:
      return synth::Controller::OCTAVE_4;
    case 3:
      return synth::Controller::OCTAVE_8;
    case 4:
      return synth::Controller::OCTAVE_16;
    default:
      NSLog(@"Unknown wave type: %d", index);
      return synth::Controller::OCTAVE_1;
  }
}


- (void)changed:(id)sender {
  // OSC 1
  controller->set_osc1_level([osc1Level value]);
  controller->set_osc1_wave_type(
      [self waveTypeForButtonIndex:[osc1Wave selectedSegmentIndex]]);
  controller->set_osc1_octave(
      [self octaveForButtonIndex:[osc1Octave selectedSegmentIndex]]);
  
  // OSC 2
  controller->set_osc2_level([osc2Level value]);
  controller->set_osc2_wave_type(
      [self waveTypeForButtonIndex:[osc2Wave selectedSegmentIndex]]);
  controller->set_osc2_octave(
      [self octaveForButtonIndex:[osc2Octave selectedSegmentIndex]]);

  controller->set_glide_samples([self samples:[glideAmount value]]);
}

@end
