//
//  EnvelopeView.h
//  mobilesynth
//
//  Created by Allen Porter on 12/23/08.
//  Copyright 2008 thebends. All rights reserved.
//

#import <UIKit/UIKit.h>

namespace synth { class Envelope; }

@interface EnvelopeView : UIView {
  UISlider* attack;
  UISlider* decay;
  UISlider* sustain;
  UISlider* release;
  synth::Envelope* envelope;
}

@property (nonatomic, retain) IBOutlet UISlider *attack;
@property (nonatomic, retain) IBOutlet UISlider *decay;
@property (nonatomic, retain) IBOutlet UISlider *sustain;
@property (nonatomic, retain) IBOutlet UISlider *release;
@property (nonatomic) IBOutlet synth::Envelope *envelope;

- (void)changed:(id)sender;

@end
