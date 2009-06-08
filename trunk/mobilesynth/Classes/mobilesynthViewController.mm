//
//  mobilesynthViewController.m
//  mobilesynth
//
//  Created by Allen Porter on 12/7/08.
//  Copyright thebends 2008. All rights reserved.
//

#import "mobilesynthViewController.h"
#import "ArpeggioView.h"
#import "AudioOutput.h"
#import "EnvelopeView.h"
#import "FilterView.h"
#import "KeyboardView.h"
#import "ModulationView.h"
#import "OscillatorView.h"
#import "OscillatorDetailView.h"
#import "TouchForwardingUIScrollView.h"
#include "synth/controller.h"
#include "synth/envelope.h"
#include "synth/modulation.h"
#include "synth/oscillator.h"
#include "synth/filter.h"
#include "synth/parameter.h"


@implementation mobilesynthViewController

@synthesize keyboardScrollView;
@synthesize controlScrollView;
@synthesize controlPageControl;

@synthesize oscillatorView;
@synthesize oscillatorDetailView;
@synthesize modulationView;
@synthesize filterView;
@synthesize envelopeView;
@synthesize filterEnvelopeView;
@synthesize arpeggioView;

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

- (void)syncControls {
  @synchronized(self) {
    [oscillatorView changed:self];
    [oscillatorDetailView changed:self];
    [modulationView changed:self];
    [filterView changed:self];
    [envelopeView changed:self];
    [filterEnvelopeView changed: self];
    [arpeggioView changed: self];
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
  [controlViews addObject:arpeggioView];

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

- (void)startLoadAnimations {
  //
  // Attempt some visual cues that will hopefully let the user notice that they
  // can scroll the control view and keyboard view.
  //
  
  // Start at the bottm and scroll to the top
  CGRect controlViewStartPosition;
  controlViewStartPosition.origin.x = controlScrollView.contentSize.width - 5;
  controlViewStartPosition.origin.y = controlScrollView.contentSize.height - 5;
  controlViewStartPosition.size.width = 5;
  controlViewStartPosition.size.height = 5;
  [controlScrollView scrollRectToVisible:controlViewStartPosition animated:NO];
  controlViewStartPosition.origin.y = 0;
  [controlScrollView scrollRectToVisible:controlViewStartPosition animated:YES];
  
  // Scroll the keyboard all the way to the far end.
  CGRect keybaordStartPosition;
  keybaordStartPosition.origin.x = [keyboardScrollView contentSize].width - 5;
  keybaordStartPosition.origin.y = 0;
  keybaordStartPosition.size.width = 5;
  keybaordStartPosition.size.height = 5;
  [keyboardScrollView scrollRectToVisible:keybaordStartPosition animated:YES];
  
  // Flash as a visual indicator to the user
  [controlScrollView flashScrollIndicators];
  [keyboardScrollView flashScrollIndicators];   
}

- (void)viewDidLoad {
  [super viewDidLoad];
  [self loadControlViews];

  CGRect keyboardViewFrame;
  keyboardViewFrame.origin.x = 0;
  keyboardViewFrame.origin.y = 0;
  keyboardViewFrame.size.width = 800;
  // Leave some empty space for scrolling
  keyboardViewFrame.size.height = keyboardScrollView.frame.size.height - 20;
  keyboardView = [[KeyboardView alloc] initWithFrame:keyboardViewFrame
                                     withOctaveCount:2];
  [keyboardView setKeyboardDelegate:self];
  [keyboardScrollView addSubview:keyboardView];  
  [keyboardScrollView setContentSize:keyboardView.frame.size];
  [keyboardScrollView setScrollEnabled:YES];
  
  // Forward touch events to the keyboard
  [keyboardScrollView setTouchView:keyboardView];
  
  controller_ = new synth::Controller;
  [oscillatorView setController:controller_];
  [oscillatorDetailView setController:controller_];
  [modulationView setController:controller_];
  [filterView setController:controller_];
  [envelopeView setEnvelope:controller_->volume_envelope()];
  [filterEnvelopeView setEnvelope:controller_->filter_envelope()];
  [arpeggioView setController:controller_];
    
  [self syncControls];
  
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
    
  [self startLoadAnimations];
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
