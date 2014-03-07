#pragma once 

#include "ofMain.h"
#include "ofxUI.h"

class ofApp : public ofBaseApp 
{
	public:
	void setup();
	void draw();
	void exit(); 

	void updatePads();
	ofxUISuperCanvas *gui;
	ofxUISuperCanvas *gui2;
	void guiEvent(ofxUIEventArgs &e);
   
	ofVec3f position;

};
