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
@synthesize lfoSrc;
@synthesize lfoDest;
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

- (synth::Controller::ModulationDestination)destForButtonIndex:(int)index {
  switch (index) {
    case 0:
      return synth::Controller::LFO_DEST_FILTER;
    case 1:
      return synth::Controller::LFO_DEST_PITCH;
    case 2:
      return synth::Controller::LFO_DEST_WAVE;
    default:
      NSLog(@"Unknown wave type: %d", index);
      return  synth::Controller::LFO_DEST_WAVE;
  }
}

- (void)changed:(id)sender {
  controller->set_modulation_amount([lfoAmount value]);
  controller->set_modulation_frequency([lfoRate value]);
  controller->set_modulation_source(
      [self sourceForButtonIndex:[lfoSrc selectedSegmentIndex]]);
  controller->set_modulation_destination(
      [self destForButtonIndex:[lfoDest selectedSegmentIndex]]);
}

@end
