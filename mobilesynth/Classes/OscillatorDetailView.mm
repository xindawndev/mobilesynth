//
//  OscillatorDetailView.m
//  mobilesynth
//
//  Created by Allen Porter on 12/23/08.
//  Copyright 2008 thebends. All rights reserved.
//

#import "OscillatorDetailView.h"
#include "synth/oscillator.h"

@implementation OscillatorDetailView

@synthesize osc2Semitones;
@synthesize osc2Cents;
@synthesize osc2TotalLabel;
@synthesize osc2SemitonesLabel;
@synthesize osc2CentsLabel;
@synthesize osc2;

- (void)changed:(id)sender {
  int total = (int)[osc2Cents value] + 100 * (int)[osc2Semitones value];
  // Adjust the "cents" label in the UI
  [osc2SemitonesLabel
      setText:[NSString stringWithFormat:@"%d", (int)[osc2Semitones value]]];
  [osc2CentsLabel
      setText:[NSString stringWithFormat:@"%d", (int)[osc2Cents value]]];
  [osc2TotalLabel
      setText:[NSString stringWithFormat:@"%d CENTS", total]];
  osc2->set_frequency_shift(total);
}

@end
