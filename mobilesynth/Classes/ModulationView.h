//
//  ModulationView.h
//  mobilesynth
//
//  Created by Allen Porter on 12/23/08.
//  Copyright 2008 thebends. All rights reserved.
//

#import <UIKit/UIKit.h>

namespace synth { class Controller; }

@interface ModulationView : UIView {
  UISlider* lfoRate;
  UISlider* lfoAmount;
  UISegmentedControl* lfoWave;
  
  synth::Controller* controller;
}

@property (nonatomic, retain) IBOutlet UISlider *lfoRate;
@property (nonatomic, retain) IBOutlet UISlider *lfoAmount;
@property (nonatomic, retain) IBOutlet UISegmentedControl *lfoWave;
@property (nonatomic) IBOutlet synth::Controller *controller;

- (void)changed:(id)sender;

@end
