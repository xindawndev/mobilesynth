//
//  OctaveView.h
//  mobilesynth
//
//  Created by Allen Porter on 4/5/09.
//  Copyright 2009 thebends. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface OctaveView : UIView {

}

- (id)initWithFrame:(CGRect)frame withKey:(int)keyNumber;
- (int)keyCount;
- (void)reset;
- (BOOL)isOpaque;

@end
