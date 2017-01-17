#include "ofApp.h"

void ofApp::setup() {
	kinect.setup();
	
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
    
    float maxPosition = 4000;
    position.setName("position");
    position.setMin(ofVec3f(1,1,1) * -maxPosition);
    position.setMax(ofVec3f(1,1,1) * +maxPosition);
    
    cameraRotation.setName("camera rotation");
    cameraRotation.setMin(ofVec3f(1,1,1) * -180);
    cameraRotation.setMax(ofVec3f(1,1,1) * +180);
    
    sceneRotation.setName("scene rotation");
    sceneRotation.setMin(ofVec3f(1,1,1) * -180);
    sceneRotation.setMax(ofVec3f(1,1,1) * +180);
    
    near = 0;
    near.setName("near");
    near.setMin(0);
    near.setMax(maxPosition);
    
    far = maxPosition;
    far.setName("far");
    far.setMin(0);
    far.setMax(maxPosition);
	
    maxLength = maxPosition;
    maxLength.setName("max length");
    maxLength.setMin(0);
    maxLength.setMax(maxPosition);
    
    stepSize = 2;
    stepSize.setName("step size");
    stepSize.setMin(1);
    stepSize.setMax(16);
    
    orthoScale = 10;
    orthoScale.setName("ortho scale");
    orthoScale.setMin(0);
    orthoScale.setMax(32);
    
    horizontalFlip = false;
    horizontalFlip.setName("horizontal flip");
    
	panel.setup();
    panel.add(position);
    panel.add(cameraRotation);
    panel.add(sceneRotation);
    panel.add(near);
    panel.add(far);
    panel.add(maxLength);
    panel.add(stepSize);
    panel.add(orthoScale);
    panel.add(horizontalFlip);
}

void ofApp::update() {
	kinect.setPosition(position.get());
	kinect.setCameraRotation(cameraRotation.get());
	kinect.setSceneRotation(sceneRotation.get());
	kinect.setClipping(near, far);
    kinect.setStepSize(stepSize);
    kinect.setMaxLength(maxLength);
    kinect.setOrthoScale(orthoScale);
    kinect.setHorizontalFlip(horizontalFlip);

	kinect.update();
	
	if(kinect.isFrameNew()) {
	}
}

void ofApp::draw() {
	ofBackground(127);
	ofSetColor(255, 255, 255);
	kinect.draw(300, 100);
    panel.draw();
}

void ofApp::exit() {
    kinect.close();
}
