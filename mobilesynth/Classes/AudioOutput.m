//
//  AudioOutput.m
//  mobilesynth
//
//  Created by Allen Porter on 12/20/08.
//  Copyright 2008 thebends. All rights reserved.
//

#import "AudioOutput.h"

static void playCallback(void *inUserData,
                         AudioQueueRef inAQ,
                         AudioQueueBufferRef inputBuffer) {
  AudioOutput* output = (AudioOutput*)inUserData;
  [[output sampleDelegate] generateSamples:inputBuffer];
  OSStatus status = AudioQueueEnqueueBuffer(inAQ, inputBuffer, 0, NULL);
  if (status != noErr) {
    NSLog(@"AudioQueueEnqueueBuffer failed: %d", status);
  }
}

@implementation AudioOutput

@synthesize sampleDelegate;

+ (void)displayErrorAndExit:(NSString*)message
                  errorCode:(OSStatus)status {
  if (status != noErr) {
    NSLog(@"%@: %@", message,
          [[NSError errorWithDomain:NSOSStatusErrorDomain
                               code:status
                           userInfo:nil] localizedDescription]);
  } else {
    NSLog(message);
  }
  exit(1);
}

- (id) init
{
  self = [super init];
  if (self != nil) {
    audioFormat_.mSampleRate = 44100.0;
    audioFormat_.mFormatID = kAudioFormatLinearPCM;
    audioFormat_.mFormatFlags = kAudioFormatFlagIsSignedInteger;
    audioFormat_.mChannelsPerFrame = 1;
    audioFormat_.mBitsPerChannel = 8 * sizeof(short);
    audioFormat_.mBytesPerFrame = sizeof(short);
    audioFormat_.mFramesPerPacket = 1;
    audioFormat_.mBytesPerPacket =
        audioFormat_.mFramesPerPacket * audioFormat_.mBytesPerFrame;
  }
  return self;
}

- (void) start {
  OSStatus status = AudioQueueNewOutput(&audioFormat_, playCallback, self,
                                        NULL, NULL, 0, &queue_);
  if (status != noErr) {
    [AudioOutput displayErrorAndExit:@"AudioQueueNewOutput" errorCode:status];
    return;
  }
  
  status = AudioQueueReset(queue_);
  if (status != noErr) {
    [AudioOutput displayErrorAndExit:@"AudioQueueReset" errorCode:status];
    return;
  }
  // allocate and prime the queue with some data
  int bufferIndex;
  for (bufferIndex = 0; bufferIndex < NUM_BUFFERS; ++bufferIndex) {
    status = AudioQueueAllocateBuffer(queue_, BUFFER_SIZE,
                                      &buffers_[bufferIndex]); 
    if (status != noErr) {
      [AudioOutput displayErrorAndExit:@"AudioQueueAllocateBuffer"
                             errorCode:status];
      return;
    }
    playCallback(self, queue_, buffers_[bufferIndex]);
  }
  status = AudioQueueStart(queue_, NULL);  
  if (status != noErr) {
    [AudioOutput displayErrorAndExit:@"AudioQueueStart" errorCode:status];
    return;
  }
}


@end
