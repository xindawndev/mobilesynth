//
//  ModulationView.m
//  mobilesynth
//
//  Created by Allen Porter on 12/23/08.
//  Copyright 2008 thebends. All rights reserved.
//

#import "ModulationView.h"
#include "synth/controller.h"

@implementation ModulationView

@synthesize lfoRate;
@synthesize lfoAmount;
@synthesize lfoWave;
@synthesize controller;

- (synth::Controller::ModulationSource)sourceForButtonIndex:(int)index {
  switch (index) {
    case 0:
      return synth::Controller::LFO_SRC_SQUARE;
    case 1:
      return synth::Controller::LFO_SRC_TRIANGLE;
    case 2:
      return synth::Controller::LFO_SRC_SAWTOOTH;
    case 3:
      return synth::Controller::LFO_SRC_REVERSE_SAWTOOTH;   
    default:
      NSLog(@"Unknown wave type: %d", index);
      return  synth::Controller::LFO_SRC_SQUARE;
  }
}

- (void)changed:(id)sender {
  controller->set_modulation_amount([lfoAmount value]);
  controller->set_modulation_frequency([lfoRate value]);
  controller->set_modulation_source(
      [self sourceForButtonIndex:[lfoWave selectedSegmentIndex]]);
  // Tremelo
  controller->set_modulation_destination(synth::Controller::LFO_DEST_WAVE);
}

@end
