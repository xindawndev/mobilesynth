//
//  ArpeggioView.h
//  mobilesynth
//
//  Created by Allen Porter on 4/19/09.
//  Copyright 2009 thebends. All rights reserved.
//

#import <UIKit/UIKit.h>

namespace synth { class Controller; }


@interface ArpeggioView : UIView {
  UISlider* rate;
  UISwitch* enabled;
  UISegmentedControl* octaves;
  UISegmentedControl* step;
  synth::Controller* controller;
}

@property (nonatomic, retain) IBOutlet UISlider *rate;
@property (nonatomic, retain) IBOutlet UISwitch *enabled;
@property (nonatomic, retain) IBOutlet UISegmentedControl *octaves;
@property (nonatomic, retain) IBOutlet UISegmentedControl *step;
@property (nonatomic) IBOutlet synth::Controller *controller;

- (void)changed:(id)sender;

@end
