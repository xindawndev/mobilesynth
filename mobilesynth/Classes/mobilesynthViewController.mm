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
#import "FilterView.h"
#import "KeyboardView.h"
#import "ModulationView.h"
#import "OscillatorView.h"
#import "OscillatorDetailView.h"
#include "synth/controller.h"
#include "synth/envelope.h"
#include "synth/modulation.h"
#include "synth/oscillator.h"
#include "synth/filter.h"
#include "synth/parameter.h"


@implementation mobilesynthViewController

@synthesize keyboardScrollView;
@synthesize keyboardImageView;
@synthesize controlScrollView;
@synthesize controlPageControl;

@synthesize oscillatorView;
@synthesize oscillatorDetailView;
@synthesize modulationView;
@synthesize filterView;
@synthesize envelopeView;
@synthesize filterEnvelopeView;

#define degreesToRadians(x) (M_PI * x / 180.0)

// Use A above Middle C as the reference frequency
static const float kNotesPerOctave = 12.0;
static const float kMiddleAFrequency = 440.0;
static const int kMiddleANote = 49;

static float GetFrequencyForNote(int note) {
  return kMiddleAFrequency * powf(2, (note - kMiddleANote) / kNotesPerOctave);
}

- (void)noteOn:(int)note {
  controller_->NoteOn(note);
}

- (void)noteOff:(int)note {
  controller_->NoteOff(note);
}

- (void)allOff {
  controller_->NoteOff();
}

- (void)syncControls {
  @synchronized(self) {
    [oscillatorView changed:self];
    [oscillatorDetailView changed:self];
    [modulationView changed:self];
    [filterView changed:self];
    [envelopeView changed:self];
    [filterEnvelopeView changed: self];
  }
}

- (OSStatus)generateSamples:(AudioBufferList*)buffers {
  assert(controller_);
  assert(buffers->mNumberBuffers == 1);  // mono output  
  AudioBuffer* outputBuffer = &buffers->mBuffers[0];
  SInt32* data = (SInt32*)outputBuffer->mData;
  if (controller_->released()) {
    // Silence
    memset(data, 0, outputBuffer->mDataByteSize);
    return noErr;
  }
  int samples = outputBuffer->mDataByteSize / sizeof(SInt32);
  float buffer[samples];
  controller_->GetFloatSamples(buffer, samples);
  for (int i = 0; i < samples; ++i) {
    data[i] = buffer[i] * 16777216L;
  }
  return noErr;
}

// Setup the scrolable control panel
- (void)loadControlViews { 
  [[self view] addSubview:controlPageControl];

  // Put the page control vertically in the top left corner.
  controlPageControl.transform =
    CGAffineTransformRotate(controlPageControl.transform, degreesToRadians(90));
  controlPageControl.transform =
    CGAffineTransformTranslate(controlPageControl.transform, 47, 50);
  
  // New controls panels should be added here
  NSMutableArray *controlViews = [[NSMutableArray alloc] init]; 
  [controlViews addObject:oscillatorView];
  [controlViews addObject:oscillatorDetailView];
  [controlViews addObject:modulationView];
  [controlViews addObject:envelopeView];
  [controlViews addObject:filterView];
  [controlViews addObject:filterEnvelopeView];

  CGRect frame = controlScrollView.frame;
  frame.origin.x = 0;
  for (int i = 0; i < [controlViews count]; ++i) {
    frame.origin.y = frame.size.height * i;  
    UIView* view = [controlViews objectAtIndex:i];
    view.frame = frame;
    [controlScrollView addSubview:view];
  }
  [controlPageControl setNumberOfPages:[controlViews count]];
  
  CGSize scrollSize = controlScrollView.frame.size;
  scrollSize.height *= [controlViews count];
  [controlScrollView setContentSize:scrollSize];
  
  [controlViews release];
}

- (void)viewDidLoad {
  [super viewDidLoad];
  [self loadControlViews];

  // Setup the inner size of the scroll view to the size of the full keyboard
  // image.  This basically makes the scroll view work.
  [keyboardScrollView setContentSize:[[keyboardImageView image] size]];
  // TODO(allen): Start at the middle of the keyboard (Scroll to key?)
  // TODO(allen): Set this to disable scrolling, and enable sliding.
  [keyboardScrollView setScrollEnabled:NO];

  controller_ = new synth::Controller;
  [oscillatorView setController:controller_];
  [oscillatorDetailView setController:controller_];
  [modulationView setController:controller_];
  [filterView setController:controller_];
  [envelopeView setEnvelope:controller_->volume_envelope()];
  [filterEnvelopeView setEnvelope:controller_->filter_envelope()];
    
  [self syncControls];

  // Initalize all the glue
  [keyboardImageView setKeyboardDelegate:self];
  
  
  // Format preferred by the iphone (Fixed 8.24)
  outputFormat.mSampleRate = 44100.0;
  outputFormat.mFormatID = kAudioFormatLinearPCM;
  outputFormat.mFormatFlags  = kAudioFormatFlagsAudioUnitCanonical;
  outputFormat.mBytesPerPacket = sizeof(AudioUnitSampleType);
  outputFormat.mFramesPerPacket = 1;
  outputFormat.mBytesPerFrame = sizeof(AudioUnitSampleType);
  outputFormat.mChannelsPerFrame = 1;
  outputFormat.mBitsPerChannel = 8 * sizeof(AudioUnitSampleType);
  outputFormat.mReserved = 0;
    
  output = [[AudioOutput alloc] initWithAudioFormat:&outputFormat];
  [output setSampleDelegate:self];
  [output start];  // immediately invokes our callback to generate samples
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
  return (interfaceOrientation == UIInterfaceOrientationLandscapeRight);
}

- (void)didReceiveMemoryWarning {
  [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
  // Release anything that's not essential, such as cached data
}

- (void)dealloc {
  [output dealloc];
  delete controller_;
  [super dealloc];
}

- (void)syncPageControl {
  // Switch the indicator when more than 50% of the previous/next page is visible
  CGFloat pageHeight = controlScrollView.frame.size.height;
  int page = floor((controlScrollView.contentOffset.y - pageHeight / 2) / pageHeight) + 1;
  [controlPageControl setCurrentPage:page];
}

// At the end of scroll animation, reset the boolean used when scrolls originate from the UIPageControl
- (void)scrollViewDidEndDecelerating:(UIScrollView *)scrollView {
  [self syncPageControl];
}

- (IBAction)changePage:(id)sender {
  int page = [controlPageControl currentPage];
  // update the scroll view to the appropriate page
  CGRect frame = controlScrollView.frame;
  frame.origin.x = 0;
  frame.origin.y = frame.size.height * page;

  [controlScrollView scrollRectToVisible:frame animated:YES];
}


@end
