//
//  TouchForwardingUIScrollView.h
//  mobilesynth
//
//  Created by Allen Porter on 6/5/09.
//  Copyright 2009 thebends. All rights reserved.
//

#import <UIKit/UIKit.h>

// This scroll view forwards all touch events to the specified touchView as a
// workardound for some odd behavior where the scroll view does not notify
// subviews on all touch end events.
@interface TouchForwardingUIScrollView : UIScrollView {
@private
  UIView* touchView;
}

@property (nonatomic, retain) UIView* touchView;

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event;

@end
