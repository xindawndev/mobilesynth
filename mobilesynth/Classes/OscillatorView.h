//
//  OscillatorView.h
//  mobilesynth
//
//  Created by Allen Porter on 12/23/08.
//  Copyright 2008 thebends. All rights reserved.
//

#import <UIKit/UIKit.h>

namespace synth { class Controller; }

@interface OscillatorView : UIView {
  UISlider* osc1Level;
  UISegmentedControl* osc1Wave;
  UISegmentedControl* osc1Octave;
  UISlider* osc2Level;
  UISegmentedControl* osc2Wave;
  UISegmentedControl* osc2Octave;

  UISlider* glideAmount;

  synth::Controller* controller;
}

@property (nonatomic, retain) IBOutlet UISlider *osc1Level;
@property (nonatomic, retain) IBOutlet UISegmentedControl *osc1Wave;
@property (nonatomic, retain) IBOutlet UISegmentedControl *osc1Octave;

@property (nonatomic, retain) IBOutlet UISlider *osc2Level;
@property (nonatomic, retain) IBOutlet UISegmentedControl *osc2Wave;
@property (nonatomic, retain) IBOutlet UISegmentedControl *osc2Octave;

@property (nonatomic, retain) IBOutlet UISlider *glideAmount;

@property (nonatomic) IBOutlet synth::Controller *controller;

- (void)changed:(id)sender;

@end
