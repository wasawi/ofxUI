/********************************************************************************** 
 
 Copyright (C) 2012 Syed Reza Ali (www.syedrezaali.com)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 
 **********************************************************************************/

#pragma once

#include "ofxUICanvas.h"

class ofxUIScrollableSliderCanvas : public ofxUICanvas
{    
public:    
    ~ofxUIScrollableSliderCanvas();
    ofxUIScrollableSliderCanvas(float x, float y, float w, float h, float sliderW);
//    ofxUIScrollableSliderCanvas(float x, float y, float w, float h, ofxUICanvas *sharedResources);
    ofxUIScrollableSliderCanvas();
    ofxUIScrollableSliderCanvas(ofxUICanvas *sharedResources);
    void initScrollable();
	void setDamping(float _damping);
    void setSnapping(bool _snapping);
    void setScrollArea(float x, float y, float w, float h);
    void setScrollAreaToScreen();
    void setScrollAreaToScreenWidth();
    void setScrollAreaToScreenHeight();
    void setScrollAreaHeight(float _height);
    void setScrollAreaWidth(float _width);
    void setScrollableDirections(bool _scrollX, bool _scrollY);
    void setStickDistance(float _stickyDistance);
	void setVisible(bool _visible);
    void dampenX();
    void dampenY();
    void update();
    virtual void drawBack();
    virtual void drawOutline();
    virtual void drawOutlineHighlight();
    virtual void drawFill();
    virtual void drawFillHighlight();
    virtual void drawPadded();
    virtual void drawPaddedOutline();
    virtual void draw();
    virtual void setPosition(int x, int y);
    virtual void setDimensions(float _width, float _height);
	void drawScrollableRect();
	//c & j
	void setFBOArea(float x, float y, float w, float h);
	void setScrollingDimensions(float _width, float _height, float _widthInternal, float _heightInternal);
    void setMappedScrollPos(float _posScroll);
	ofVec2f calcHeightContents(vector<ofxUIWidget*> _auxwidgets);
	void updateScrollPosition(int max);
	void updateScrollBarSize(vector<ofxUIWidget*> _auxwidgets, float maxrange, float minrange);
	void setupScrollBar(string _name, float _min, float _max, int _lowvalue, int _highvalue, int _w, int _h, int _x, int _y, int _size);


#ifdef OFX_UI_TARGET_TOUCH
    void touchDown(float x, float y, int id);
    void touchMoved(float x, float y, int id);
    void touchUp(float x, float y, int id);
    void touchCancelled(float x, float y, int id);
#endif
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    ofxUIRectangle *getSRect();
    virtual bool isHit(int x, int y);
	
	//c & j
	void guiEvent(ofxUIEventArgs &e);
	ofxUICanvas* getScroll(); // TODO try to find a better name
	void adjustContentstoGui(bool bsnap = false);


protected:
    ofxUIRectangle *sRect;
    bool isScrolling;
    bool snapping; 
    bool scrollX, scrollY; 
    bool nearTop, nearBot, nearRight, nearLeft;
    bool hitWidget; 
    ofPoint pos; 
    ofPoint ppos; 
    ofPoint vel; 
    ofPoint acc; 
    float damping;
    float stickyDistance;    
	
	//c & j
	ofFbo fbo;    //experimental
    ofxUIRectangle *FBORect;
	bool bFBO;
	float posScrollbar;//Direct scrolling
	int heightContents;
	
	ofxUICanvas *gui_slider;
};