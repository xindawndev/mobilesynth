//
//  ModulationView.h
//  mobilesynth
//
//  Created by Allen Porter on 12/23/08.
//  Copyright 2008 thebends. All rights reserved.
//

#import <UIKit/UIKit.h>

namespace synth { class Oscillator; }
namespace synth { class LFO; }

@interface ModulationView : UIView {
  UISlider* lfoRate;
  UISlider* lfoAmount;
  UISegmentedControl* lfoWave;
  
  synth::LFO* lfo;
  synth::Oscillator* osc;
}

@property (nonatomic, retain) IBOutlet UISlider *lfoRate;
@property (nonatomic, retain) IBOutlet UISlider *lfoAmount;
@property (nonatomic, retain) IBOutlet UISegmentedControl *lfoWave;
@property (nonatomic) IBOutlet synth::LFO *lfo;
@property (nonatomic) IBOutlet synth::Oscillator *osc;

- (void)changed:(id)sender;

@end
