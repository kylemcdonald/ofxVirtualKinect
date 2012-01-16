#pragma once

#include "ofMain.h"
#include "ofxKinect.h"

class ofxVirtualKinect {
public:
	ofxVirtualKinect();
	~ofxVirtualKinect();
	
	void setup();
	void update();
	bool isFrameNew();
	void draw(float x, float y);
	ofPixels& getPixels();
	
	void setMaxLen(float maxLen);
	void setStepSize(int stepSize);
	void setClipping(float nearClipping, float farClipping);
	void setOrthoScale(float orthoScale);
	void setPosition(ofVec3f position);
	void setCameraRotation(ofVec3f cameraRotation);
	void setSceneRotation(ofVec3f sceneRotation);
	void setHorizontalFlip(bool horizontalFlip);
	
	int getWidth() const;
	int getHeight() const;
private:
	ofxKinect kinect;
	
	vector<ofVec3f> surface;
	vector<unsigned int> indices;
	
	ofFbo fbo;
	ofPixels colorImage, grayImage;
	
	bool newFrame;
	bool horizontalFlip;
	bool needToUpdatePixels;
	
	float maxLen;
	int stepSize;
	float nearClipping, farClipping;
	float orthoScale;
	ofVec3f position, cameraRotation, sceneRotation;
	
	void updateSurface();
	void updateMesh();
	void renderCamera();
	void updatePixels();
};