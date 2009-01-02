//
//  AudioOutput.m
//  mobilesynth
//
//  Created by Allen Porter on 12/20/08.
//  Copyright 2008 thebends. All rights reserved.
//

#import "AudioOutput.h"
#import <AudioUnit/AudioUnitProperties.h>
#import <AudioUnit/AudioOutputUnit.h>
#import <AudioToolbox/AudioServices.h>


@implementation AudioOutput

static const float kSampleRate = 44100.0;
static const int kOutputBus = 0;

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

static OSStatus playCallback(void *inRefCon,
                             AudioUnitRenderActionFlags *ioActionFlags,
                             const AudioTimeStamp *inTimeStamp,
                             UInt32 inBusNumber,
                             UInt32 inNumberFrames,
                             AudioBufferList *ioData) {
  assert(inBusNumber == kOutputBus);
  AudioOutput* output = (AudioOutput*)inRefCon;
  return [[output sampleDelegate] generateSamples:ioData];
}

- (id)init {
  AudioStreamBasicDescription desc;
  return [self initWithAudioFormat:&desc];
}

- (id)initWithAudioFormat:(const AudioStreamBasicDescription*)streamDescription {
  if ((self = [super init])) {
    memcpy(&audioFormat, streamDescription, sizeof(AudioStreamBasicDescription));
  }
  return self;
}

- (void) start {
  OSStatus status;
  // Describe audio component
  AudioComponentDescription desc;
  desc.componentType = kAudioUnitType_Output;
  desc.componentSubType = kAudioUnitSubType_RemoteIO;
  desc.componentFlags = 0;
  desc.componentFlagsMask = 0;
  desc.componentManufacturer = kAudioUnitManufacturer_Apple;
  
  // Get component
  AudioComponent outputComponent = AudioComponentFindNext(NULL, &desc);
  if (outputComponent == NULL) {
    [AudioOutput displayErrorAndExit:@"AudioComponentFindNext"
                           errorCode:0];
  }
  
  // Get audio units
  status = AudioComponentInstanceNew(outputComponent, &audioUnit);
  if (status) {
    [AudioOutput displayErrorAndExit:@"AudioComponentInstanceNew"
                           errorCode:status];
  }
  
  // Enable playback
  UInt32 enableIO = 1;
  status = AudioUnitSetProperty(audioUnit,
                                kAudioOutputUnitProperty_EnableIO,
                                kAudioUnitScope_Output,
                                kOutputBus,
                                &enableIO,
                                sizeof(UInt32));
  if (status) {
    [AudioOutput displayErrorAndExit:@"AudioUnitSetProperty EnableIO (out)"
                           errorCode:status];
  }
   
  // Apply format
  status = AudioUnitSetProperty(audioUnit,
                                kAudioUnitProperty_StreamFormat,
                                kAudioUnitScope_Input,
                                kOutputBus,
                                &audioFormat,
                                sizeof(AudioStreamBasicDescription));
  if (status) {
    [AudioOutput displayErrorAndExit:@"AudioUnitSetProperty StreamFormat"
                           errorCode:status];
  }
  
  AURenderCallbackStruct callback;
  callback.inputProc = &playCallback;
  callback.inputProcRefCon = self;

  // Set output callback
  status = AudioUnitSetProperty(audioUnit,
                                kAudioUnitProperty_SetRenderCallback,
                                kAudioUnitScope_Global,
                                kOutputBus,
                                &callback,
                                sizeof(AURenderCallbackStruct));
  if (status) {
    [AudioOutput displayErrorAndExit:@"AudioUnitSetProperty SetRenderCallback"
                           errorCode:status];
  } 
 
  status = AudioUnitInitialize(audioUnit);
  if (status) {
    [AudioOutput displayErrorAndExit:@"AudioUnitInitialize"
                           errorCode:status];
  }
  
  status = AudioOutputUnitStart(audioUnit);
  if (status) {
    [AudioOutput displayErrorAndExit:@"AudioOutputUnitStart"
                           errorCode:status];
  }
}

- (void) dealloc
{
  AudioUnitUninitialize(audioUnit);
  [super dealloc];
}



@end
