//
//  FilterView.h
//  mobilesynth
//
//  Created by Allen Porter on 1/19/09.
//  Copyright 2009 thebends. All rights reserved.
//

#import <UIKit/UIKit.h>

namespace synth { class Controller; }

@interface FilterView : UIView {
  UISlider* cutoff;
  UISlider* resonance;
  UILabel* frequencyLabel;
  synth::Controller* controller;
}

@property (nonatomic, retain) IBOutlet UISlider *cutoff;
@property (nonatomic, retain) IBOutlet UISlider *resonance;

@property (nonatomic, retain) IBOutlet UILabel *frequencyLabel;

@property (nonatomic) IBOutlet synth::Controller *controller;

- (void)changed:(id)sender;

@end
