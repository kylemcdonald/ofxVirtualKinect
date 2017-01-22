#pragma once

#include "ofMain.h"
#include "ofxKinect.h"

class ofxVirtualKinect : public ofBaseHasPixels {
public:
	ofxVirtualKinect();
	
    void setup();
    void close();
	void update();
	bool isFrameNew();
	void draw(float x, float y);
    ofPixels& getPixels();
    const ofPixels& getPixels() const;
	
	void setMaxLength(float maxLen);
	void setStepSize(int stepSize);
	void setClipping(float nearClipping, float farClipping);
	void setOrthoScale(float orthoScale);
	void setPosition(ofVec3f position);
	void setCameraRotation(ofVec3f cameraRotation);
	void setSceneRotation(ofVec3f sceneRotation);
	void setHorizontalFlip(bool horizontalFlip);
	
	int getWidth() const;
    int getHeight() const;
    
    ofFbo fbo;
    
private:
	ofxKinect kinect;
	
	vector<ofVec3f> surface;
	vector<unsigned int> indices;
	
	ofPixels colorImage, grayImage;
	
	bool newFrame;
	bool horizontalFlip;
	
	float maxLength;
	int stepSize;
	float nearClipping, farClipping;
	float orthoScale;
	ofVec3f position, cameraRotation, sceneRotation;
	
	void updateSurface();
	void updateMesh();
	void renderCamera();
	void updatePixels();
};
