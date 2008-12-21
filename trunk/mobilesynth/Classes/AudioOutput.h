//
//  AudioOutput.h
//  mobilesynth
//
//  Created by Allen Porter on 12/20/08.
//  Copyright 2008 thebends. All rights reserved.
//

#import <Foundation/Foundation.h>
#include <AudioToolbox/AudioToolbox.h>

#define NUM_BUFFERS 2
#define BUFFER_SIZE 8 * 1024

@protocol SampleGenerator 
@required
- (void)generateSamples:(AudioQueueBufferRef)buffer;
@end

@interface AudioOutput : NSObject {
  id <SampleGenerator> sampleDelegate;
  AudioStreamBasicDescription audioFormat_;
  AudioQueueRef queue_;
  AudioQueueBufferRef buffers_[NUM_BUFFERS];
}

@property (nonatomic, retain) IBOutlet id <SampleGenerator> sampleDelegate;

- (void) start;

@end
