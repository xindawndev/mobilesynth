//
//  AudioOutput.h
//  mobilesynth
//
//  Created by Allen Porter on 12/20/08.
//  Copyright 2008 thebends. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AudioUnit/AUComponent.h>

#define NUM_BUFFERS 3
#define BUFFER_SIZE 1024

@protocol SampleGenerator 
@required
- (OSStatus)generateSamples:(AudioBufferList*)buffers;
@end

@interface AudioOutput : NSObject {
 @private
  id <SampleGenerator> sampleDelegate;
  AudioComponentInstance audioUnit;
  AudioStreamBasicDescription audioFormat;
}

- (id)initWithAudioFormat:(const AudioStreamBasicDescription*)streamDescription;

@property (nonatomic, retain) IBOutlet id <SampleGenerator> sampleDelegate;

- (void) start;

+ (void)displayErrorAndExit:(NSString*)message
                  errorCode:(OSStatus)status;

@end
