#include "ofxVirtualKinect.h"

const unsigned int camWidth = 640;
const unsigned int camHeight = 480;

ofxVirtualKinect::ofxVirtualKinect() :
newFrame(false),
maxLen(100),
stepSize(2),
nearClipping(0),
farClipping(1024),
orthoScale(10),
position(ofVec3f(0, 0, 0)),
sceneRotation(ofVec3f(0, 0, 0)),
cameraRotation(ofVec3f(0, 0, 0)),
horizontalFlip(true),
needToUpdatePixels(false) {
}

ofxVirtualKinect::~ofxVirtualKinect() {
	kinect.close();
}

void ofxVirtualKinect::setup() {
	surface.resize(camWidth * camHeight);
	indices.resize(camWidth * camHeight * 3);
	
	kinect.init(false, false);
	kinect.open();
	
	fbo.allocate(camWidth, camHeight, GL_RGB);
	colorImage.allocate(camWidth, camHeight, OF_IMAGE_COLOR);
	grayImage.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);
}

void ofxVirtualKinect::updateSurface() {
	float* z = kinect.getDistancePixels();
	for(int y = 0; y < camHeight; y += stepSize) {
		for(int x = 0; x < camWidth; x += stepSize) {
			int i = y * camWidth + x;
			if(z[i] != 0) {
				surface[i] = kinect.getWorldCoordinateAt(x, y);
			}
		}
	}
}

void ofxVirtualKinect::updateMesh() {
	float* z = kinect.getDistancePixels();
	indices.clear();
	for(int y = 0; y < camHeight - stepSize; y += stepSize) {
		for(int x = 0; x < camWidth - stepSize; x += stepSize) {
			int i = y * camWidth + x;
			unsigned int nwi = i;
			unsigned int nei = nwi + stepSize;
			unsigned int swi = nwi + (stepSize * camWidth);
			unsigned int sei = swi + stepSize;
			float nw = z[nwi];
			float ne = z[nei];
			float sw = z[swi];
			float se = z[sei];
			
			if(nw != 0 && ne != 0 && sw != 0 &&
				 abs(nw - ne) < maxLen && abs(nw - sw) < maxLen) {
				indices.push_back(nwi);
				indices.push_back(nei);
				indices.push_back(swi);
			}
			
			if(ne != 0 && se != 0 && sw != 0 &&
				 abs(sw - se) < maxLen && abs(ne - se) < maxLen) {
				indices.push_back(nei);
				indices.push_back(sei);
				indices.push_back(swi);
			}
		}
	}
}

void ofxVirtualKinect::renderCamera() {
	fbo.begin();
	ofClear(0, 255);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_FOG);
	
	glClearColor(0, 0, 0, 1);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	GLfloat fogColor[4]= {0, 0, 0, 1};
	glFogfv(GL_FOG_COLOR, fogColor);
	glHint(GL_FOG_HINT, GL_FASTEST);
	glFogf(GL_FOG_START, nearClipping);
	glFogf(GL_FOG_END, farClipping);
	
	ofPushMatrix();
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-320 * orthoScale, 320 * orthoScale, -240 * orthoScale, 240 * orthoScale, nearClipping, farClipping);
	glMatrixMode(GL_MODELVIEW);
	
	glLoadIdentity();
	
	ofRotateX(sceneRotation.x);
	ofRotateY(sceneRotation.y);
	ofRotateZ(sceneRotation.z);
	ofTranslate(position.x, position.y, position.z);
	ofRotateX(cameraRotation.x);
	ofRotateY(cameraRotation.y);
	ofRotateZ(cameraRotation.z);
	
	ofScale(horizontalFlip ? -1 : 1, 1, -1);
	
	ofSetColor(255);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(ofVec3f), &(surface[0].x));
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, &indices[0]);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	ofPopMatrix();
	
	glDisable(GL_FOG);
	glDisable(GL_DEPTH_TEST);
	
	fbo.end();
}

void ofxVirtualKinect::updatePixels() {
	fbo.readToPixels(colorImage);
	unsigned char* grayPixels = grayImage.getPixels();
	unsigned char* colorPixels = colorImage.getPixels();
	int n = camWidth * camHeight;
	for(int i = 0; i < n; i++) {
		*grayPixels = *colorPixels;
		grayPixels += 1;
		colorPixels += 3;
	}
}

void ofxVirtualKinect::update() {
	kinect.update();
	if(kinect.isFrameNew()) {
		newFrame = true;
		needToUpdatePixels = true;
		updateSurface();
		updateMesh();
		renderCamera();
	}
}

bool ofxVirtualKinect::isFrameNew() {
	bool curNewFrame = newFrame;
	newFrame = false;
	return curNewFrame;
}

unsigned char* ofxVirtualKinect::getPixels() {
	return getPixelsRef().getPixels();
}

ofPixels& ofxVirtualKinect::getPixelsRef() {
	if(needToUpdatePixels) {
		updatePixels();
		needToUpdatePixels = false;
	}
	return grayImage;
}

void ofxVirtualKinect::draw(float x, float y) {
	fbo.draw(x, y);
}

void ofxVirtualKinect::setMaxLen(float maxLen) {
	this->maxLen = maxLen;
}

void ofxVirtualKinect::setStepSize(int stepSize) {
	this->stepSize = stepSize;
}

void ofxVirtualKinect::setClipping(float nearClipping, float farClipping) {
	this->nearClipping = nearClipping;
	this->farClipping = farClipping;
}

void ofxVirtualKinect::setOrthoScale(float orthoScale) {
	this->orthoScale = orthoScale;
}

void ofxVirtualKinect::setPosition(ofVec3f position) {
	this->position = position;
}

void ofxVirtualKinect::setCameraRotation(ofVec3f cameraRotation) {
	this->cameraRotation = cameraRotation;
}

void ofxVirtualKinect::setSceneRotation(ofVec3f sceneRotation) {
	this->sceneRotation = sceneRotation;
}

void ofxVirtualKinect::setHorizontalFlip(bool horizontalFlip) {
	this->horizontalFlip = horizontalFlip;
}

int ofxVirtualKinect::getWidth() const {
	return camWidth;
}

int ofxVirtualKinect::getHeight() const {
	return camHeight;
}