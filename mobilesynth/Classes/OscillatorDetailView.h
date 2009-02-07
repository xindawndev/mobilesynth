//
//  OscillatorDetailView.h
//  mobilesynth
//
//  Created by Allen Porter on 12/23/08.
//  Copyright 2008 thebends. All rights reserved.
//

#import <UIKit/UIKit.h>

namespace synth { class Controller; }

@interface OscillatorDetailView : UIView {
  UISlider* osc2Semitones;
  UISlider* osc2Cents;
  UILabel* osc2SemitonesLabel;
  UILabel* osc2CentsLabel;
  UILabel* osc2TotalLabel;
  
  UISwitch* oscSync;
   
  synth::Controller* controller;
}

@property (nonatomic, retain) IBOutlet UISlider *osc2Semitones;
@property (nonatomic, retain) IBOutlet UISlider *osc2Cents;
@property (nonatomic, retain) IBOutlet UILabel *osc2SemitonesLabel;
@property (nonatomic, retain) IBOutlet UILabel *osc2CentsLabel;
@property (nonatomic, retain) IBOutlet UILabel *osc2TotalLabel;

@property (nonatomic, retain) IBOutlet UISwitch *oscSync;

@property (nonatomic) IBOutlet synth::Controller *controller;

- (void)changed:(id)sender;

@end
