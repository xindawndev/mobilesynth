//
//  KeyView.h
//  mobilesynth
//
//  Created by Allen Porter on 4/6/09.
//  Copyright 2009 thebends. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface KeyView : UIView {
@private
  int key;
@protected
  BOOL keyPressed;
}

- (id)initWithFrame:(CGRect)frame withKey:(int)keyNumber;
- (int)keyNumber;

// Provide visual cues that a key has been pressed.
- (void)keyDown;
- (void)keyUp;

@end
