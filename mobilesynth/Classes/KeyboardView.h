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
- (void)noteBegin:(int)note;
- (void)noteEnd;
@end

@interface KeyboardView : UIImageView {
  id <KeyboardDelegate> keyboardDelegate;
  int currentNote;
}

@property (nonatomic, retain) IBOutlet id <KeyboardDelegate> keyboardDelegate;

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event;

- (int)noteAtPoint:(CGPoint)point;

@end
