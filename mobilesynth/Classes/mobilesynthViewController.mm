//
//  mobilesynthViewController.m
//  mobilesynth
//
//  Created by Allen Porter on 12/7/08.
//  Copyright thebends 2008. All rights reserved.
//

#import "mobilesynthViewController.h"
#import "AudioOutput.h"
#import "EnvelopeView.h"
#import "KeyboardView.h"
#import "ModulationView.h"
#import "OscillatorView.h"
#import "OscillatorDetailView.h"
#include "synth/combiner.h"
#include "synth/controller.h"
#include "synth/envelope.h"
#include "synth/modulation.h"
#include "synth/oscillator.h"


@implementation mobilesynthViewController

@synthesize keyboardScrollView;
@synthesize keyboardImageView;
@synthesize controlScrollView;
@synthesize controlPageControl;

@synthesize oscillatorView;
@synthesize oscillatorDetailView;
@synthesize modulationView;
@synthesize envelopeView;


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
    [oscillatorView changed:self];
    [oscillatorDetailView changed:self];
    [modulationView changed:self];
    [envelopeView changed:self];
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

// Setup the scrolable control panel
- (void)loadControlViews {
  CGRect frame = [keyboardScrollView frame];
  frame.origin.x = frame.size.width / 2;
  frame.origin.y = 0;
  frame.size.width = 10;
  frame.size.height = 10;
  [keyboardScrollView scrollRectToVisible:frame animated:YES];
  
  NSMutableArray *controlViews = [[NSMutableArray alloc] init]; 
  
  // New controls panels should be added here
  [controlViews addObject:oscillatorView];
  [controlViews addObject:oscillatorDetailView];
  [controlViews addObject:modulationView];
  [controlViews addObject:envelopeView];
  
  for (int i = 0; i < [controlViews count]; ++i) {
    UIView* view = [controlViews objectAtIndex:i];
    CGRect frame = controlScrollView.frame;
    frame.origin.x = frame.size.width * i;
    frame.origin.y = 0;
    view.frame = frame;
    [controlScrollView addSubview:view];
  }
  [controlViews release];
  
  int subviews = [[controlScrollView subviews] count];
  CGSize frameSize = [controlScrollView frame].size;
  [controlScrollView setContentSize:CGSizeMake(frameSize.width * subviews,
                                               frameSize.height)];
  [controlPageControl setNumberOfPages:subviews];
}

- (void)viewDidLoad {
  [super viewDidLoad];
  [self loadControlViews];

  // Setup the inner size of the scroll view to the size of the full keyboard
  // image.  This basically makes the scroll view work.
  [keyboardScrollView setContentSize:[[keyboardImageView image] size]];
  // TODO(allen): Start at the middle of the keyboard (Scroll to key?)
  // TODO(allen): Set this to disable scrolling, and enable sliding.
  //[keyboardScrollView setScrollEnabled:NO];
  
  osc1_ = new synth::Oscillator;
  osc2_ = new synth::Oscillator;
  
  // Create a combiner that merges osc1 and osc2.  Its volume is set to the
  // max and is never changed.  Instead, the volumes of the child oscillators
  // can be adjusted.
  combiner_ = new synth::Combiner;
  combiner_->set_level(1.0);  
  combiner_->Add(osc1_);
  combiner_->Add(osc2_);
  
  lfo_osc_ = new synth::Oscillator;
  lfo_ = new synth::LFO;
  lfo_->set_oscillator(lfo_osc_);
  
  envelope_ = new synth::Envelope;
  
  // Tie all of the components together with the controller. 
  controller_ = new synth::Controller;
  controller_->set_sample_rate(44100.0);
  controller_->set_volume(1.0);
  controller_->set_volume_envelope(envelope_);
  controller_->set_oscillator(combiner_);
  controller_->set_lfo(lfo_);
  
  
  // Link the synth objects to their UI controllers
  [oscillatorView setOsc1:osc1_];
  [oscillatorView setOsc2:osc2_];
  [oscillatorDetailView setOsc2:osc2_];
  [modulationView setLfo:lfo_osc_];
  
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

- (void)syncPageControl {
  // Switch the indicator when more than 50% of the previous/next page is visible
  CGFloat pageWidth = controlScrollView.frame.size.width;
  int page = floor((controlScrollView.contentOffset.x - pageWidth / 2) / pageWidth) + 1;
  [controlPageControl setCurrentPage:page];
}

- (void)scrollViewDidScroll:(UIScrollView *)sender {
  // We don't want a "feedback loop" between the UIPageControl and the scroll delegate in
  // which a scroll event generated from the user hitting the page control triggers updates from
  // the delegate method. We use a boolean to disable the delegate logic when the page control is used.
  if (pageControlUsed) {
    // do nothing - the scroll was initiated from the page control, not the user dragging
    return;
  }
  [self syncPageControl];
}

// At the end of scroll animation, reset the boolean used when scrolls originate from the UIPageControl
- (void)scrollViewDidEndDecelerating:(UIScrollView *)scrollView {
  pageControlUsed = NO;
  [self syncPageControl];
}

- (IBAction)changePage:(id)sender {
  int page = [controlPageControl currentPage];
  // update the scroll view to the appropriate page
  CGRect frame = controlScrollView.frame;
  frame.origin.x = frame.size.width * page;
  frame.origin.y = 0;

  [controlScrollView scrollRectToVisible:frame animated:YES];
  // Set when scrolls originate from the UIPageControl. See scrollViewDidScroll: above.
  pageControlUsed = YES;
}


@end
