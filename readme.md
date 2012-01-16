# ofxVirtualKinect is an addon for [openFrameworks](http://openframeworks.cc/)

It uses [ofxKinect](https://github.com/ofTheo/ofxKinect) to create a virtual depth camera by rendering a mesh with fog based from an arbitrary perspective. This allows you to mount the kinect wherever you like, and then get a virtual depth image from a perspective you may not otherwise have access to.

ofxVirtualKinect uses a customized version of [ofxControlPanel](https://github.com/kylemcdonald/ofxControlPanel).

ofxVirtualKinect was originally created for the 3rdi interactive installation in Doha, and was called [ofxVirtualCamera](https://github.com/kylemcdonald/DohaInstallation/tree/master/addons/ofxVirtualCamera) at the time. This new version has been updated to work with openFrameworks 007, taking advantage of the standardized FBO interface in openFrameworks.