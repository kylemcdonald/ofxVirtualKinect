#pragma once

#include "ofAppRunner.h"
#include "ofGraphics.h"
#include "ofTexture.h"

/*
        push/pop to be renamed

        // push
        glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &previousFBO);
        glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING_EXT, &previousReadFBO);
        glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING_EXT, &previousDrawFBO);

        // pop
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, previousFBO);
        glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, previousReadFBO);
        glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, previousDrawFBO);

        add warning if you try to use the wrong resolutions

        do a glGet to find the current fbo before pushing and popping
        add multisampling support:
        need to be able to load results back to an ofImage maybe using glGetTexImage
        add all the checks for warnings and errors (just before begin())
        add OpenGLes support

        should ofFbo override the screen width and screen height returned by ofGetWidth() and ofGetHeight()?
                ofImageGrabScreen() or ofTextureLoadScreenData() etc.
*/

class ofxFbo {
public:
        ofxFbo();
        void setup(int width, int height, bool useDepth = true, bool useStencil = false);
        ~ofxFbo();

        void setFov(float fov);
        void setBackground(float r, float g, float b, float a = 255);
        void clearAlpha();

        void attach(ofTexture& target, int position = 0);
        void detach(int position = 0);

        void begin();
        void end();

        void draw(float x, float y);
        void draw(float x, float y, float width, float height);

        int getWidth();
        int getHeight();

protected:
        int width, height;
        int levels;
        float fov;
        GLuint fboId, depthId, stencilId;
        ofTexture internalColor;
        vector<ofTexture*> attachments;

        void push();
        void pop();
        ofTexture* getAttachment();
        void checkAttachment();
        void setupScreenForFbo();
        void setupScreenForWindow();
};


