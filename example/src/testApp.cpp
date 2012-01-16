#include "testApp.h"


void testApp::setup() {
	kinect.setup();
	
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	
	panel.setup(280, 800);
	panel.addPanel("virtual camera");
	float maxPosition = 4000;
	panel.addSlider("x", 0, -maxPosition, maxPosition);
	panel.addSlider("y", 0, -maxPosition, maxPosition);
	panel.addSlider("z", 0, -maxPosition, maxPosition);
	panel.addSlider("xcr", 0, -180, 180);
	panel.addSlider("ycr", 0, -180, 180);
	panel.addSlider("zcr", 0, -180, 180);
	panel.addSlider("xsr", 0, -180, 180);
	panel.addSlider("ysr", 0, -180, 180);
	panel.addSlider("zsr", 0, -180, 180);
	panel.addSlider("near", 0, 0, maxPosition);
	panel.addSlider("far", maxPosition, 0, maxPosition);
	panel.addSlider("maxLen", 100, 0, maxPosition);
	panel.addSlider("stepSize", 2, 1, 16, true);
	panel.addSlider("orthoScale", 10, 0, 32);
	panel.addToggle("horizontalFlip", false);
}

float testApp::_(string name) {
	return panel.getValueF(name);
}

void testApp::update() {
	ofVec3f position(_("x"), _("y"), _("z"));
	ofVec3f cameraRotation(_("xcr"), _("ycr"), _("zcr"));
	ofVec3f sceneRotation(_("xsr"), _("ysr"), _("zsr"));
	kinect.setPosition(position);
	kinect.setCameraRotation(cameraRotation);
	kinect.setSceneRotation(sceneRotation);
	kinect.setClipping(_("near"), _("far"));
	kinect.setStepSize(_("stepSize"));
	kinect.setMaxLen(_("maxLen"));
	kinect.setOrthoScale(_("orthoScale"));
	kinect.setHorizontalFlip(_("horizontalFlip"));

	kinect.update();
	
	if(kinect.isFrameNew()) {
	}
}

void testApp::draw() {
	ofBackground(127);
	ofSetColor(255, 255, 255);
	kinect.draw(300, 100);
}

