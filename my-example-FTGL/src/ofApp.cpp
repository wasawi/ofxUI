#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetVerticalSync(true);
	ofEnableSmoothing();

	//set some sketch parameters

	drawPadding = false;
	backgroundColor = ofColor(100);
//___________________________________________________________________________________________________________________________

	gui = new ofxUICanvas();
	gui->setGlobalCanvasWidth(320);
	gui->setHeight(700);
	gui->setPosition(0, 50);
//	gui->setFont("GUI/Arial Unicode.ttf");                     //This loads a new font and sets the GUI font
//	gui->setFontSize(OFX_UI_FONT_LARGE, 18);            //These call are optional, but if you want to resize the LARGE, MEDIUM, and SMALL fonts, here is how to do it.
//	gui->setFontSize(OFX_UI_FONT_MEDIUM, 14);
//	gui->setFontSize(OFX_UI_FONT_SMALL, 10);            //SUPER IMPORTANT NOTE: CALL THESE FUNTIONS BEFORE ADDING ANY WIDGETS, THIS AFFECTS THE SPACING OF THE GUI

	gui->addLabel("ONE LINE TEST", OFX_UI_FONT_MEDIUM);
	gui->addSpacer();
	gui->addLabel("Label FONT_MEDIUM いいです！！", OFX_UI_FONT_MEDIUM);
	gui->addSpacer();
	gui->addTextArea("TextArea", "Text area FONT_MEDIUM いいです！！", OFX_UI_FONT_MEDIUM);
	gui->addTextInput("TextInput", "Text input FONT_MEDIUM",OFX_UI_FONT_MEDIUM);
	
//	gui->addTextArea("TextArea", "日本語版　this is a text area OFX_UI_FONT_LARGE いいです！！", OFX_UI_FONT_LARGE);
//	gui->addTextInput("TextInput", "日本語版　this is a text input OFX_UI_FONT_MEDIUM いいです！！",OFX_UI_FONT_MEDIUM);
	
	gui->addSpacer();
	gui->addLabelToggle("DRAW PADDING", drawPadding);
	gui->addSpacer();
	gui->setDrawWidgetPadding(true);
	ofAddListener(gui->newGUIEvent,this,&ofApp::guiEvent);

//___________________________________________________________________________________________________________________________
	gui2 = new ofxUICanvas();
//	gui2->setGlobalCanvasWidth(320);
	gui2->setHeight(700);
	gui2->setPosition(330, 50);
	//	gui->setFont("GUI/Arial Unicode.ttf");                     //This loads a new font and sets the GUI font
	//	gui->setFontSize(OFX_UI_FONT_LARGE, 18);            //These call are optional, but if you want to resize the LARGE, MEDIUM, and SMALL fonts, here is how to do it.
	//	gui->setFontSize(OFX_UI_FONT_MEDIUM, 14);
	//	gui->setFontSize(OFX_UI_FONT_SMALL, 10);            //SUPER IMPORTANT NOTE: CALL THESE FUNTIONS BEFORE ADDING ANY WIDGETS, THIS AFFECTS THE SPACING OF THE GUI
	gui2->addLabel("MULTILINE", OFX_UI_FONT_MEDIUM);
	gui2->addSpacer();
	gui2->addLabel("MULTILINE TEST aaaaaaaa aa aaaaaaaa", OFX_UI_FONT_MEDIUM);
	gui2->addSpacer();
	gui2->addLabel("Label", "Label FONT_MEDIUM ذخثتشرهدجαβγδεζηθικλμνξοπρσςτυφχจชซฌฎฏฐฑฒดตทธศαβγδεζηθικλμνいいです！！", OFX_UI_FONT_MEDIUM);
	gui2->addSpacer();
	gui2->addTextArea("TextArea", "Text area FONT_MEDIUM ଊଋୠଌୡଏ غظضذخثتشرهدج αβγδεζηθικλμνξοπρσςτυφいいです！！", OFX_UI_FONT_MEDIUM);
//	gui2->addTextInput("TextInput", "Text input FONT_MEDIUM",OFX_UI_FONT_MEDIUM);
	//	gui->addTextInput("TextInput", "ññ କୈଊଋୠଌୡଏ غظضذخثتشرهدج αβγδεζηθικλμνξοπρσςτυφχ จชซฌฎฏฐฑฒดตทธศษ αβγδεζηθικλμνξοπρσςτυφχ จชซฌฎฏฐฑฒดตทธศษ αβγδεζηθικλμνξοπρσςτυφχ จชซฌฎฏฐฑฒดตทธศษ αβγδεζηθικλμνξοπρσςτυφχ จชซฌฎฏฐฑฒดตทธศษ αβγδεζηθικλμνξοπρσςτυφχ จชซฌฎฏฐฑฒดตทธศษ αβγδεζηθικλμνξοπρσςτυφχ จชซฌฎฏฐฑฒดตทธศษ ...   " ,OFX_UI_FONT_MEDIUM);
//	gui->addSpacer();
//	gui2->addTextArea("TextInput", "ññ କୈଊଋୠଌୡଏغظضذخثتشرهدجαβγδεζηθικλμνξοπρσςτυφχจชซฌฎฏฐฑฒดตทธศαβγδεζηθικλμνξοπρσςτυφχจชซฌฎฏฐฑฒดตทธศษ αβγδεζηθικλμνξοπρσςτυφχ จชซฌฎฏฐฑฒดตทธศษ αβγδεζηθικλμνξοπρσςτυφχ จชซฌฎฏฐฑฒดตทธศษ αβγδεζηθικλμνξοπρσςτυφχ จชซฌฎฏฐฑฒดตทธศษ αβγδεζηθικλμνξοπρσςτυφχ จชซฌฎฏฐฑฒดตทธศษ いいです！！  " ,OFX_UI_FONT_MEDIUM);

	
	//	gui->addTextArea("TextArea", "日本語版　this is a text area OFX_UI_FONT_LARGE いいです！！", OFX_UI_FONT_LARGE);
	//	gui->addTextInput("TextInput", "日本語版　this is a text input OFX_UI_FONT_MEDIUM いいです！！",OFX_UI_FONT_MEDIUM);

	gui2->addSpacer();
	gui2->addLabelToggle("DRAW PADDING", drawPadding);
	gui2->addSpacer();
		
//	gui2->autoSizeToFitWidgets();
	gui2->setDrawWidgetPadding(true);
	ofAddListener(gui2->newGUIEvent,this,&ofApp::gui2Event);
	ofBackground(backgroundColor);
	
}

//--------------------------------------------------------------
void ofApp::update()
{
//	gui->setWidth(ofGetMouseX());
//	gui->setHeight(ofGetMouseY());

}

//--------------------------------------------------------------
void ofApp::draw()
{    	
	ofBackground(backgroundColor);     
	ofPushStyle(); 
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	
    
	ofPopStyle(); 
}
//--------------------------------------------------------------
void ofApp::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName(); 
	int kind = e.widget->getKind(); 
    
	if(name == "DRAW PADDING")
    {
        ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget; 
        drawPadding = toggle->getValue();     
        gui->setDrawWidgetPadding(drawPadding);             
    }    
    else if(name == "SPACER COLOR")
    {
        ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;         
        
        vector<ofxUIWidget*> spacers = gui->getWidgetsOfType(OFX_UI_WIDGET_SPACER); 

        for(int i = 0; i < spacers.size(); i++)        
        {
            ofxUISpacer *spacer = (ofxUISpacer *) spacers[i]; 
            spacer->setToggleColor(toggle->getValue()); 
        }
    }
    
}
//--------------------------------------------------------------
void ofApp::gui2Event(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	int kind = e.widget->getKind();
	
	if(name == "DRAW PADDING")
    {
        ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
        drawPadding = toggle->getValue();
        gui2->setDrawWidgetPadding(drawPadding);
    }
    else if(name == "SPACER COLOR")
    {
        ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
        
        vector<ofxUIWidget*> spacers = gui2->getWidgetsOfType(OFX_UI_WIDGET_SPACER);
		
        for(int i = 0; i < spacers.size(); i++)
        {
            ofxUISpacer *spacer = (ofxUISpacer *) spacers[i];
            spacer->setToggleColor(toggle->getValue());
        }
    }
    
}
//--------------------------------------------------------------
void ofApp::exit()
{
    delete gui;
	delete gui2;
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    switch (key) 
    {            
        case 'g':
        {
            gui->toggleVisible(); 
        }
            break;        
            
        default:
            break;
    }
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y )
{ 
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
 
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{

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