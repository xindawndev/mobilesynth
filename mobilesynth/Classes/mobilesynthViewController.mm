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


// Use A above Middle C as the reference frequency
static const float kNotesPerOctave = 12.0;
static const float kMiddleAFrequency = 440.0;
static const int kMiddleANote = 49;

static float GetFrequencyForNote(int note) {
  return kMiddleAFrequency * powf(2, (note - kMiddleANote) / kNotesPerOctave);
}

- (void)noteBegin:(int)note {
  [self noteChange:note];
  controller_->NoteOn(note);
}

- (void)noteChange:(int)note {
  controller_->NoteChange(note);
}

- (void)noteEnd {
  controller_->NoteOff();
}


- (void)syncControls {
  @synchronized(self) {
    [oscillatorView changed:self];
    [oscillatorDetailView changed:self];
    [modulationView changed:self];
    [filterView changed:self];
    [envelopeView changed:self];
  }
}

- (OSStatus)generateSamples:(AudioBufferList*)buffers {
  assert(controller_);
  assert(buffers->mNumberBuffers == 1);  // mono output
  AudioBuffer* outputBuffer = &buffers->mBuffers[0];
  SInt32* data = (SInt32*)outputBuffer->mData;
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
  [controlViews addObject:filterView];
  [controlViews addObject:filterEnvelopeView];
  
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
