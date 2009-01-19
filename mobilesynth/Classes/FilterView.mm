//
//  FilterView.m
//  mobilesynth
//
//  Created by Allen Porter on 1/19/09.
//  Copyright 2009 thebends. All rights reserved.
//

#import "FilterView.h"
#include "synth/filter.h"

@implementation FilterView

@synthesize cutoff;
@synthesize filter;

- (void)changed:(id)sender {
  filter->set_cutoff([cutoff value]);
}

@end