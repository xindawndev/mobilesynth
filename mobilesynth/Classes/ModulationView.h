//
//  ModulationView.h
//  mobilesynth
//
//  Created by Allen Porter on 12/23/08.
//  Copyright 2008 thebends. All rights reserved.
//

#import <UIKit/UIKit.h>

namespace synth { class Oscillator; }

@interface ModulationView : UIView {
  UISlider* lfoRate;
  UISlider* lfoAmount;
  UISegmentedControl* lfoWave;
  
  synth::Oscillator* lfo;
}

@property (nonatomic, retain) IBOutlet UISlider *lfoRate;
@property (nonatomic, retain) IBOutlet UISlider *lfoAmount;
@property (nonatomic, retain) IBOutlet UISegmentedControl *lfoWave;
@property (nonatomic) IBOutlet synth::Oscillator *lfo;

- (void)changed:(id)sender;

@end
