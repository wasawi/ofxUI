#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	//ofBackground(80);
	
	
    float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float dim = 20;
    float length = 320-xInit;
	sliderIndex = 0;
        
    gui = new ofxUIScrollableSliderCanvas(ofGetWidth()/3,50,length+xInit,500);
    gui->setScrollableDirections(false, true);
	addWidgets();
	ofAddListener(gui->newGUIEvent, this, &ofApp::guiEvent);
	
	
	// for debug purposes:
//	gui->drawScrollCanvas(true);

	/* comapare scroll slider to range slider
    guiTest = new ofxUICanvas(700,50,200,500);
	guiTest->addRangeSlider("test ", 80, 120, 90, 100, dim, 200, length-xInit);
	guiTest->addScrollSlider("test ", 80, 120, 90, 120, dim, 200, length-xInit);
	*/
	
}
//--------------------------------------------------------------
void ofApp::addWidgets(){
	
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float dim = 20;
    float length = 320-xInit;
	
    for(int i = 0; i < 10; i++)
    {
        gui->addSlider("SLIDER " +ofToString(sliderIndex), 80, 120, i*3.3+80, length-xInit, dim);
        gui->addSpacer(length-xInit, 2)->setVisible(false);
		sliderIndex++;
    }
	
	// this is needed in order for it to work!!
	// (for now there's no better way to do it, gomenasai!)
	// make sue you use it everytime you create widgets
	gui->autoSizeToFitWidgets();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofBackgroundGradient(150,50);
	ofSetColor(0);
	
	ofDrawBitmapString("press f to toggle fbo", ofPoint(20,60));
	ofDrawBitmapString("press s to toggle scroll bar", ofPoint(20,80));
	ofDrawBitmapString("press a to add widgets", ofPoint(20,100));
}
//--------------------------------------------------------------
void ofApp::exit()
{
    delete gui; 
}
//--------------------------------------------------------------
void ofApp::guiEvent(ofxUIEventArgs &e)
{
    cout << e.widget->getName() << endl; 
    if(e.widget->getName() == "SLIDER 0")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;    
        ofBackground(slider->getScaledValue());
    }
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    switch (key) {
        case 'p':
            gui->setDrawWidgetPadding(true);
            break;
        case 'P':
            gui->setDrawWidgetPadding(false);
            break;
		
        case 'f':
            gui->toggleFBO();
            break;
		case 's':
            gui->toggleScrollBar();
            break;
		case 'a':
			addWidgets();
            break;
		    
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{

}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
	
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}