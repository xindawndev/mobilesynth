//
//  FilterView.h
//  mobilesynth
//
//  Created by Allen Porter on 1/19/09.
//  Copyright 2009 thebends. All rights reserved.
//

#import <UIKit/UIKit.h>

namespace synth { class Filter; }

@interface FilterView : UIView {
  UISlider* cutoff;
  synth::Filter* filter;
}

@property (nonatomic, retain) IBOutlet UISlider *cutoff;
@property (nonatomic) IBOutlet synth::Filter *filter;

- (void)changed:(id)sender;

@end
