//
//  FilterView.m
//  mobilesynth
//
//  Created by Allen Porter on 1/19/09.
//  Copyright 2009 thebends. All rights reserved.
//

#import "FilterView.h"
#include "synth/controller.h"

@implementation FilterView

@synthesize cutoff;
@synthesize resonance;
@synthesize controller;
@synthesize frequencyLabel;

- (void)changed:(id)sender {
  // TODO(allen): It seems like it would be more natural to make this log scale
  controller->set_filter_cutoff([cutoff value]);
  [frequencyLabel
      setText:[NSString stringWithFormat:@"%d Hz", (int)[cutoff value]]];
  
  controller->set_filter_resonance([resonance value]);
}

@end