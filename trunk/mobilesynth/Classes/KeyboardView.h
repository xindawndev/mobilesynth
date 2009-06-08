//
//  KeyboardView.h
//  mobilesynth
//
//  Created by Allen Porter on 12/13/08.
//  Copyright 2008 thebends. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol KeyboardDelegate
@required
- (void)noteOn:(int)note;
- (void)noteOff:(int)note;
@end

@protocol Key
@required
- (void)keyDown;
- (void)keyUp;
// Returns the midi note represented by this key
- (int)keyNumber;
@end

@interface KeyboardView : UIView {
@private
  id <KeyboardDelegate> keyboardDelegate;
  NSMutableSet* keyDownSet;
}

- (id)initWithFrame:(CGRect)frame withOctaveCount:(int)count;

@property (nonatomic, retain) IBOutlet id <KeyboardDelegate> keyboardDelegate;

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event;

@end
