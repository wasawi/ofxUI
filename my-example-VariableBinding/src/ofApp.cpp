#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    
	position = ofVec3f(ofGetWidth()*.5, ofGetHeight()*.5, 0);
	gui = new ofxUISuperCanvas("Circle position", 0, 0, 200, 0);
	gui->add3DPad("FRONT",
				   ofPoint(0,ofGetWidth()),
				   ofPoint(0, ofGetHeight()),
				   ofPoint(0,ofGetWidth()),
				   &position,
				   OFX_UI_FRONT);
	
	gui->add3DPad("TOP"  ,
				   ofPoint(0, ofGetWidth()),
				   ofPoint(0, ofGetHeight()),
				   ofPoint(0,ofGetWidth()),
				   &position,
				   OFX_UI_TOP);
	
	 gui->add3DPad("LEFT",
				   ofPoint(0, ofGetWidth()),
				   ofPoint(0, ofGetHeight()),
				   ofPoint(0,ofGetWidth()),
				   &position,
				   OFX_UI_LEFT);
	gui->autoSizeToFitWidgets();
	ofAddListener(gui->newGUIEvent,this,&ofApp::guiEvent);
	
	gui2 = new ofxUISuperCanvas("Circle position", 200, 0, 200, 0);
	gui2->add3DPad("BACK",
				  ofPoint(0,ofGetWidth()),
				  ofPoint(0, ofGetHeight()),
				  ofPoint(0,ofGetWidth()),
				  &position,
				  OFX_UI_BACK);
	
	gui2->add3DPad("BOTTOM"  ,
				  ofPoint(0, ofGetWidth()),
				  ofPoint(0, ofGetHeight()),
				  ofPoint(0,ofGetWidth()),
				  &position,
				  OFX_UI_BOTTOM);
	
	gui2->add3DPad("RIGHT",
				  ofPoint(0, ofGetWidth()),
				  ofPoint(0, ofGetHeight()),
				  ofPoint(0,ofGetWidth()),
				  &position,
				  OFX_UI_RIGHT);
	
	gui2->autoSizeToFitWidgets();
	ofAddListener(gui2->newGUIEvent,this,&ofApp::guiEvent);
	
}

//--------------------------------------------------------------
void ofApp::draw()
{    	
	ofBackground(50);
	ofSetColor(200);
	ofPushView();
		ofTranslate(ofVec3f(0,0,-ofGetWidth()));
		ofCircle(position, 50);
	ofPopView();
}
//--------------------------------------------------------------
void ofApp::guiEvent(ofxUIEventArgs &e)
{
	
/*
	string name = e.widget->getName(); 
	int kind = e.widget->getKind();
	
	if (name== "FRONT_XYZ")
	{
		ofxUI2DPad *pad = (ofxUI2DPad *) gui->getWidget("FRONT_XYZ");
		
		// get values
		position.x = pad->getScaledValue().x;
		position.y = pad->getScaledValue().y;
		updatePads();
	}
	else if (name== "TOP_XZY")
	{
		ofxUI2DPad *pad = (ofxUI2DPad *) gui->getWidget("TOP_XZY");
		
		// get values
		position.x = pad->getScaledValue().x;
		position.z = pad->getScaledValue().y;
		updatePads();
	}
	else if (name== "RIGHT_ZYX")
	{
		ofxUI2DPad *pad = (ofxUI2DPad *) gui->getWidget("RIGHT_ZYX");
		
		// get values
		position.z = pad->getScaledValue().x;
		position.y = pad->getScaledValue().y;
		updatePads();
	}
*/


}

//--------------------------------------------------------------
void ofApp::updatePads()
{
	//update pads
	ofxUI2DPad *pad;
	pad = (ofxUI2DPad *) gui->getWidget("FRONT");
	pad -> setValue(ofVec3f(position.x, position.y, 0));
	pad = (ofxUI2DPad *) gui->getWidget("TOP");
	pad -> setValue(ofVec3f(position.x,position.z,0));
	pad = (ofxUI2DPad *) gui->getWidget("RIGHT");
	pad -> setValue(ofVec3f(position.z,position.y,0));
}

//--------------------------------------------------------------
void ofApp::exit()
{
    delete gui; 
}
