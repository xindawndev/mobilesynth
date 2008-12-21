//
//  mobilesynthViewController.h
//  mobilesynth
//
//  Created by Allen Porter on 12/7/08.
//  Copyright thebends 2008. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AudioOutput.h"
#import "KeyboardView.h"

namespace synth { class Controller; }
namespace synth { class Combiner; }
namespace synth { class Envelope; }
namespace synth { class LFO; }
namespace synth { class Oscillator; }

@interface mobilesynthViewController
      : UIViewController <KeyboardDelegate, SampleGenerator> {
  UIScrollView* keyboardScrollView;
  KeyboardView* keyboardImageView;
  AudioOutput* output;

  synth::Combiner* combiner_;
  synth::Oscillator* osc1_;
  synth::Oscillator* osc2_;
  synth::Envelope* envelope_;
  synth::Oscillator* lfo_osc_;
  synth::LFO* lfo_;
  synth::Controller* controller_;
}

@property (nonatomic, retain) IBOutlet UIScrollView *keyboardScrollView;
@property (nonatomic, retain) IBOutlet KeyboardView *keyboardImageView;

- (void)noteBegin:(int)note;
- (void)noteEnd;
- (void)generateSamples:(AudioQueueBufferRef)buffer;

@end

