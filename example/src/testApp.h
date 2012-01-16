#pragma once

#include "ofMain.h"
#include "ofxVirtualKinect.h"
#include "ofxAutoControlPanel.h"

class testApp : public ofBaseApp {
public:
	
	void setup();
	void update();
	void draw();
	
	float _(string name);
	
	ofxVirtualKinect kinect;
	ofxAutoControlPanel panel;
};
