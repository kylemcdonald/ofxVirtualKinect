#include "ofxFbo.h"

void ofxFbo::push() {
	if(levels == 0)
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);
	levels++;
}

void ofxFbo::pop() {
	levels--;
	if(levels == 0)
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

ofTexture* ofxFbo::getAttachment() {
	for(unsigned int i = 0; i < attachments.size(); i++)
		if(attachments[i] != NULL)
			return attachments[i];
	return NULL;
}

void ofxFbo::checkAttachment() {
	if(getAttachment() == NULL) {
		// change this to only allocate if not already allocated
		internalColor.allocate(width, height, GL_RGB);
		attach(internalColor);
	}
}

void ofxFbo::setupScreenForFbo() {
	float eyeX = (float) width / 2;
	float eyeY = (float) height / 2;
	float halfFov = PI * fov / 360;
	float theTan = tanf(halfFov);
	float dist = eyeY / theTan;
	float nearDist = dist / 10;
	float farDist = dist * 10;
	float aspect = (float) width / height;
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, aspect, nearDist, farDist);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, dist, eyeX, eyeY, 0, 0, 1, 0);
	
	glViewport(0, 0, width, height);
}

void ofxFbo::setupScreenForWindow() {
	ofSetupScreen();
	glViewport(0, 0, ofGetWidth(), ofGetHeight());
}

ofxFbo::ofxFbo() :
levels(0),
fov(60),
fboId(0),
depthId(0),
stencilId(0) {
}

void ofxFbo::setup(int width, int height, bool useDepth, bool useStencil) {
	this->width = width;
	this->height = height;
	
	glGenFramebuffersEXT(1, &fboId);
	if(useDepth)
		glGenRenderbuffersEXT(1, &depthId);
	if(useStencil)
		glGenRenderbuffersEXT(1, &stencilId);
	
	push();
	if(useDepth) {
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthId);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthId);
	}
	if(useStencil) {
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthId);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_STENCIL_INDEX, width, height);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthId);
	}
	pop();
	
	int maxAttachments = 0;
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &maxAttachments);
	attachments.assign(maxAttachments, (ofTexture*) NULL);
}

ofxFbo::~ofxFbo() {
	if(fboId != 0)
		glDeleteFramebuffersEXT(1, &fboId);
	if(depthId != 0)
		glDeleteRenderbuffersEXT(1, &depthId);
	if(stencilId != 0)
		glDeleteRenderbuffersEXT(1, &stencilId);
}

void ofxFbo::setFov(float fov) {
	this->fov = fov;
}

void ofxFbo::attach(ofTexture& target, int position) {
	push();
	
	detach(position);
	
	attachments[position] = &target;
	ofTextureData& texData = target.texData;
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + position, texData.textureTarget, texData.textureID, 0);
	
	pop();
}

void ofxFbo::detach(int position) {
	push();
	
	ofTexture* target = attachments[position];
	if(target != NULL) {
		ofTextureData& texData = target->texData;
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + position, texData.textureTarget, 0, 0);
		target = NULL;
	}
	
	pop();
}

void ofxFbo::setBackground(float r, float g, float b, float a) {
	push();
	
	checkAttachment();
	glClearColor(r / 255, g / 255, b / 255, a / 255);
	glClear(GL_COLOR_BUFFER_BIT |
					(depthId != 0 ? GL_DEPTH_BUFFER_BIT : 0) |
					(stencilId != 0 ? GL_STENCIL_BUFFER_BIT : 0));
	
	pop();
}

void ofxFbo::clearAlpha() {
	push();
	
	glColorMask(0, 0, 0, 1);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glColorMask(1, 1, 1, 1);
	
	pop();
}

void ofxFbo::begin() {
	checkAttachment();
	glPushMatrix();
	setupScreenForFbo();
	push();
}

void ofxFbo::end() {
	pop();
	setupScreenForWindow();
	glPopMatrix();
}

void ofxFbo::draw(float x, float y) {
	draw(x, y, width, height);
}

void ofxFbo::draw(float x, float y, float width, float height) {
	checkAttachment();
	getAttachment()->draw(x, y, width, height);
}

int ofxFbo::getWidth() {
	return width;
}

int ofxFbo::getHeight() {
	return height;
}
