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
@synthesize controller;

- (void)changed:(id)sender {
  controller->set_filter_cutoff([cutoff value]);
}

@end