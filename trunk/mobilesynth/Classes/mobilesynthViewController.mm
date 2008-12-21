//
//  mobilesynthViewController.m
//  mobilesynth
//
//  Created by Allen Porter on 12/7/08.
//  Copyright thebends 2008. All rights reserved.
//

#import "mobilesynthViewController.h"
#import "AudioOutput.h"
#import "KeyboardView.h"
#include "synth/combiner.h"
#include "synth/controller.h"
#include "synth/envelope.h"
#include "synth/modulation.h"
#include "synth/oscillator.h"


@implementation mobilesynthViewController

@synthesize keyboardScrollView;
@synthesize keyboardImageView;


// Use A above Middle C as the reference frequency
static const float kNotesPerOctave = 12.0;
static const float kMiddleAFrequency = 440.0;
static const int kMiddleANote = 49;

static float GetFrequencyForNote(int note) {
  return kMiddleAFrequency * powf(2, (note - kMiddleANote) / kNotesPerOctave);
}

- (void)noteBegin:(int)note {
  @synchronized(self) {
    float freq = GetFrequencyForNote(note);
    NSLog(@"Note: %d Freq: %f", note, freq);
    combiner_->set_frequency(freq);
    osc1_->set_frequency(freq);
    osc2_->set_frequency(freq);
    envelope_->NoteOn();
  }
}

- (void)noteEnd {
  @synchronized(self) {
    envelope_->NoteOff();
  }
}

- (void)syncControls {
  @synchronized(self) {
    osc1_->set_level(1.0);
    osc1_->set_wave_type(synth::Oscillator::TRIANGLE);
    osc2_->set_level(0.2);
    osc2_->set_wave_type(synth::Oscillator::SQUARE);
/*
    lfo_->set_amount(0.5);
    lfo_osc_->set_wave_type(synth::Oscillator::SAWTOOTH);
    lfo_osc_->set_frequency(5.0); 
 */
  }
}

- (void)generateSamples:(AudioQueueBufferRef)buffer {
  @synchronized(self) {    
    buffer->mAudioDataByteSize = buffer->mAudioDataBytesCapacity;
    short* data = (short*)buffer->mAudioData;
    int length = buffer->mAudioDataByteSize / sizeof(short);
    for (int i = 0; i < length; ++i) {
      // Float => Signed integer
      data[i] = (short)(controller_->GetSample() * 32767.0f);
    }
  }
}

- (void)viewDidLoad {
  [super viewDidLoad];
  // Setup the inner size of the scroll view to the size of the full keyboard
  // image.  This basically makes the scroll view work.
  [keyboardScrollView setContentSize:[[keyboardImageView image] size]];
  // TODO(allen): Start at the middle of the keyboard (Scroll to key?)
  // TODO(allen): Set this to disable scrolling, and enable sliding.
  //[keyboardScrollView setScrollEnabled:NO];
  
  NSLog(@"viewDidLoad");

  combiner_ = new synth::Combiner;
  combiner_->set_level(1.0);
  osc1_ = new synth::Oscillator;
  osc1_->set_level(0.0);
  osc2_ = new synth::Oscillator;
  osc2_->set_level(0.0);
  combiner_->Add(osc1_);
  combiner_->Add(osc2_);
  
  lfo_osc_ = new synth::Oscillator;
  lfo_osc_->set_level(1.0);
  lfo_osc_->set_frequency(1);
  lfo_ = new synth::LFO;
  lfo_->set_amount(0.0);
  lfo_->set_oscillator(lfo_osc_);
  
  envelope_ = new synth::Envelope;
  envelope_->set_attack(0.0);
  envelope_->set_decay(0.0);
  envelope_->set_sustain(1.0);
  envelope_->set_release(0.0);
  
  controller_ = new synth::Controller;
  controller_->set_sample_rate(44100.0);
  controller_->set_volume(1.0);
  controller_->set_volume_envelope(envelope_);
  controller_->set_oscillator(combiner_);
  controller_->set_lfo(lfo_);
  
  [self syncControls];

  // Initalize all the glue
  [keyboardImageView setKeyboardDelegate:self];
  
  // Calling start will immediately cause samples to be generated
  output = [[AudioOutput alloc] init];
  [output setSampleDelegate:self];
  [output start];
  
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
  return (interfaceOrientation == UIInterfaceOrientationLandscapeRight);
}

- (void)didReceiveMemoryWarning {
  [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
  // Release anything that's not essential, such as cached data
}

- (void)dealloc {
  delete controller_;
  delete envelope_;
  delete lfo_osc_;
  delete lfo_;
  delete combiner_;
  delete osc1_;
  delete osc2_;
  [super dealloc];
}

@end
