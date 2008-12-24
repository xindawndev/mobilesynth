//
//  OscillatorView.h
//  mobilesynth
//
//  Created by Allen Porter on 12/23/08.
//  Copyright 2008 thebends. All rights reserved.
//

#import <UIKit/UIKit.h>

namespace synth { class Oscillator; }

@interface OscillatorView : UIView {
  UISlider* osc1Level;
  UISegmentedControl* osc1Wave;
  UISegmentedControl* osc1Octave;
  UISlider* osc2Level;
  UISegmentedControl* osc2Wave;
  UISegmentedControl* osc2Octave;

  synth::Oscillator* osc1;
  synth::Oscillator* osc2;
}

@property (nonatomic, retain) IBOutlet UISlider *osc1Level;
@property (nonatomic, retain) IBOutlet UISegmentedControl *osc1Wave;
@property (nonatomic, retain) IBOutlet UISegmentedControl *osc1Octave;

@property (nonatomic, retain) IBOutlet UISlider *osc2Level;
@property (nonatomic, retain) IBOutlet UISegmentedControl *osc2Wave;
@property (nonatomic, retain) IBOutlet UISegmentedControl *osc2Octave;

@property (nonatomic) IBOutlet synth::Oscillator *osc1;
@property (nonatomic) IBOutlet synth::Oscillator *osc2;

- (void)changed:(id)sender;

@end
