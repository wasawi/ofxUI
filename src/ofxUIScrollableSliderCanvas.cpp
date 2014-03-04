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

#include "ofxUIScrollableSliderCanvas.h"
#include "ofxUI.h"

ofxUIScrollableSliderCanvas::~ofxUIScrollableSliderCanvas()
{
    delete sRect;
	delete FBORect;
}

ofxUIScrollableSliderCanvas::ofxUIScrollableSliderCanvas(float x, float y, float w, float h, float sliderW) : ofxUICanvas(x,y,w,h)
{
    initScrollable();
	setupScrollBar("S",		// string _name,
				   0,			// float _min,
				   h,			// float _max,
				   0,			// int _lowvalue,
				   h,			// int _highvalue,
				   sliderW,		// int _w,
				   h,			// int _h,
				   x+w,			// int _x,
				   y,			// int _y,
				   OFX_UI_FONT_SMALL);
	adjustContentstoGui(false);
}

ofxUIScrollableSliderCanvas::ofxUIScrollableSliderCanvas() : ofxUICanvas()
{
    initScrollable();
	//TODO 
	//setupScrollBar("S", 0, h, h-20, h, 26, h, x+w, 0, OFX_UI_FONT_SMALL);
}

ofxUIScrollableSliderCanvas::ofxUIScrollableSliderCanvas(ofxUICanvas *sharedResources) : ofxUICanvas(sharedResources)
{
    initScrollable();
	//TODO 
	//setupScrollBar("S", 0, h, h-20, h, 26, h, x+w, 0, OFX_UI_FONT_SMALL);
}

void ofxUIScrollableSliderCanvas::initScrollable()
{
    kind = OFX_UI_WIDGET_SCROLLABLECANVAS;
    sRect = new ofxUIRectangle(rect->x, rect->y, rect->getWidth(), rect->getHeight());	
	FBORect = new ofxUIRectangle();
	
    paddedRect->setParent(sRect);
    isScrolling = false;
    vel.set(0);
    pos.set(0);
    ppos.set(0);
    acc.set(0);
    damping = .75;
    scrollX = false;
    scrollY = true;
    
    nearTop = false;
    nearBot = false;
    nearRight = false;
    nearLeft = false;
	
    hitWidget = false;
    stickyDistance = 32;
    hit = false;
    snapping = true;
	
	bFBO=false;
#ifdef OFX_UI_TARGET_TOUCH
    touchId = -1;
#endif
}

void ofxUIScrollableSliderCanvas::setupScrollBar(string _name, float _min, float _max, int _lowvalue, int _highvalue, int _w, int _h, int _x, int _y, int _size){

	gui_slider = new ofxUICanvas(_x, _y, _w, _h);
	float spacing = 0.1;
	gui_slider->setWidgetSpacing(spacing);
	gui_slider->addWidgetRight(new ofxUIScrollSlider(_name,
													 _min,
													 _max,
													 _lowvalue,
													 _highvalue,
													 _w - spacing*2,
													 _h - spacing*2,
													 _x,
													 _y,
													 OFX_UI_FONT_SMALL));

	gui_slider->setDrawPaddingOutline(false);
	
	ofAddListener(gui_slider->newGUIEvent,this,&ofxUIScrollableSliderCanvas::guiEvent);
}

void ofxUIScrollableSliderCanvas::setDamping(float _damping)
{
    damping = _damping;
}

void ofxUIScrollableSliderCanvas::setSnapping(bool _snapping)
{
    snapping = _snapping;
}

void ofxUIScrollableSliderCanvas::setScrollArea(float x, float y, float w, float h)
{
    sRect->x = x;
    sRect->y = y;
    sRect->setWidth(w);
    sRect->setHeight(h);
    paddedRect->setWidth(w+padding*2);
    paddedRect->setHeight(h+padding*2);
}

void ofxUIScrollableSliderCanvas::setFBOArea(float x, float y, float w, float h)
{
	bFBO=true;
    FBORect->x = x;
    FBORect->y = y;
    FBORect->setWidth(w);
    FBORect->setHeight(h);
	
	fbo.allocate(FBORect->getWidth(),  FBORect->getHeight(), GL_RGBA,0);
	gui_slider->setPosition(x+w, y);
	
	// draw canvas(inside fbo) on 0,0 because the fbo will draw on position.
	sRect->x = 0;
    sRect->y = 0;
	rect->x=0;
	rect->y=0;
}

void ofxUIScrollableSliderCanvas::setScrollAreaToScreen()
{
    sRect->x = 0;
    sRect->y = 0;
    sRect->setWidth(ofGetWidth());
    sRect->setHeight(ofGetHeight());
}

void ofxUIScrollableSliderCanvas::setScrollAreaToScreenWidth()
{
    sRect->x = 0;
    sRect->setWidth(ofGetWidth());
}

void ofxUIScrollableSliderCanvas::setScrollAreaToScreenHeight()
{
    sRect->y = 0;
    sRect->setHeight(ofGetHeight());
}

void ofxUIScrollableSliderCanvas::setScrollAreaHeight(float _height)
{
    sRect->setHeight(_height);
    paddedRect->setHeight(_height+padding*2);
}

void ofxUIScrollableSliderCanvas::setScrollAreaWidth(float _width)
{
    sRect->setWidth(_width);
    paddedRect->setWidth(_width+padding*2);
}

void ofxUIScrollableSliderCanvas::setScrollableDirections(bool _scrollX, bool _scrollY)
{
    scrollX = _scrollX;
    scrollY = _scrollY;
}

void ofxUIScrollableSliderCanvas::setStickDistance(float _stickyDistance)
{
    stickyDistance = _stickyDistance;
}

void ofxUIScrollableSliderCanvas::dampenX()
{
    if(nearRight || nearLeft)
    {
        acc.x -=vel.x*(.5);
    }
    else
    {
        acc.x -=vel.x*(1.0-damping);
    }
}

void ofxUIScrollableSliderCanvas::dampenY()
{
    if(nearTop || nearBot)
    {
        acc.y -=vel.y*(.5);
    }
    else
    {
        acc.y -=vel.y*(1.0-damping);
    }
}

void ofxUIScrollableSliderCanvas::update()
{
    if(!isScrolling)
    {
        if(scrollX && snapping)
        {
            float dxLeft = rect->x - sRect->x;
            float dxRight = (sRect->x+sRect->getWidth()) - (rect->x+rect->getWidth());
            
            if(fabs(dxLeft) < stickyDistance)
            {
                nearLeft = true;
            }
            else if(fabs(dxRight) < stickyDistance)
            {
                nearRight = true;
            }
            else
            {
                nearLeft = false;
                nearRight = false;
            }
            
            if(dxLeft > 0)
            {
                acc.x += (-dxLeft)/10.0;
                dampenX();
            }
            else if(nearLeft)
            {
                acc.x += (-dxLeft)/10.0;
                dampenX();
            }
            else if(dxRight > 0)
            {
                acc.x += (dxRight)/10.0;
                dampenX();
            }
            else if(nearRight)
            {
                acc.x += (dxRight)/10.0;
                dampenX();
            }
        }
        
        if(scrollY && snapping)
        {
            float dyTop = rect->y - sRect->y;
            float dyBot = (sRect->y+sRect->getHeight()) - (rect->y+rect->getHeight());
			
			if(fabs(dyBot) < stickyDistance)
            {
                nearTop = false;
                nearBot = true;
            }
            if(fabs(dyTop) < stickyDistance)
            {
                nearTop = true;
                nearBot = false;
            }
            
            if(dyTop > 0)
            {
                acc.y += (-dyTop)/10.0;
                dampenY();
            }
            else if(nearTop)
            {
                acc.y += (-dyTop)/10.0;
                dampenY();
            }
            
            if(dyBot > 0)
            {
                acc.y += (dyBot)/10.0;
                dampenY();
            }
            else if(nearBot)
            {
                acc.y += (dyBot)/10.0;
                dampenY();
            }
            
            nearTop = false;
            nearBot = false;
        }
        
        acc.limit(50);
        vel +=acc;
        vel.limit(100);
        if(scrollX && fabs(vel.x) > 1.0) rect->x += floor(vel.x);
        if(scrollY && fabs(vel.y) > 1.0) rect->y += floor(vel.y);
        
        vel *=damping;
        acc.set(0);
    }
    
    for(vector<ofxUIWidget *>::iterator it = widgets.begin(); it != widgets.end(); ++it)
    {
        (*it)->update();
    }
}

void ofxUIScrollableSliderCanvas::drawBack()
{
    if(draw_back)
    {
        ofxUIFill();
        ofxUISetColor(color_back);
        sRect->draw();
    }
}

void ofxUIScrollableSliderCanvas::drawOutline()
{
    if(draw_outline)
    {
        ofNoFill();
        ofxUISetColor(color_outline);
        sRect->draw();
    }
}

void ofxUIScrollableSliderCanvas::drawOutlineHighlight()
{
    if(draw_outline_highlight)
    {
        ofNoFill();
        ofxUISetColor(color_outline_highlight);
        sRect->draw();
    }
}

void ofxUIScrollableSliderCanvas::drawFill()
{
    if(draw_fill)
    {
        ofxUIFill();
        ofxUISetColor(color_fill);
        sRect->draw();
    }
}

void ofxUIScrollableSliderCanvas::drawFillHighlight()
{
    if(draw_fill_highlight)
    {
        ofxUIFill();
        ofxUISetColor(color_fill_highlight);
        sRect->draw();
    }
}

void ofxUIScrollableSliderCanvas::drawPadded()
{
    if(draw_padded_rect && !embedded)
    {
        ofxUIFill();
        ofxUISetColor(color_padded_rect);
        paddedRect->draw();
    }
}

void ofxUIScrollableSliderCanvas::drawPaddedOutline()
{
    if(draw_padded_rect_outline && !embedded)
    {
        ofNoFill();
        ofxUISetColor(color_padded_rect_outline);
        paddedRect->draw();
    }
}

void ofxUIScrollableSliderCanvas::draw()
{
	
	if (bFBO) {
		fbo.begin();
		/*
		 OK this is a very ugly hack..
		 this color is half of the double of the back color (50) which is 75.
		 it is very hard to get around this problem:
		 http://forum.openframeworks.cc/t/fbo-problems-with-alpha/1643/10
		 http://forum.openframeworks.cc/t/weird-problem-rendering-semi-transparent-image-to-fbo/2215/4
		 
		 Some tests:
		 left value: OFX_UI_COLOR_BACK_ALPHA 100
		 right value: equivalent in ofClear();
		 (this only works when ofBackground is default)
		 0      200
		 20     184
		 50     158
		 100    108
		 150    59
		 180    35
		 200    20
		 255    20
		 */
		
		
        ofClear(40);
        ofClearAlpha();
	}
	
    ofxUIPushStyle();
	
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofxUISetRectMode(OFX_UI_RECTMODE_CORNER);
    ofSetLineWidth(1.0);
    
    drawPadded();
    
    drawPaddedOutline();
    
    drawBack();
    
    drawFill();
    
    drawFillHighlight();
    
    drawOutline();
    
    drawOutlineHighlight();
    
    for(vector<ofxUIWidget *>::reverse_iterator it = widgets.rbegin(); it != widgets.rend(); ++it)
    {
		if (bFBO) {
			// Do not test if inside.. otherwise the widgets will disapear again.
			if((*it)->isVisible())// && (*it)->getRect()->rInside(*sRect))
			{
				(*it)->draw();
			}
		}else{
			if((*it)->isVisible() && (*it)->getRect()->rInside(*sRect))
			{
				(*it)->draw();
			}
		}
    }

    ofxUIPopStyle();
	
	if (bFBO) {
		fbo.end();

		ofDisableAlphaBlending();
		ofSetColor(255);
		fbo.draw(FBORect->getX(), FBORect->getY());
	}
	ofEnableAlphaBlending();
}

void ofxUIScrollableSliderCanvas::setPosition(int x, int y)
{
    rect->x = x;
    rect->y = y;
    sRect->x = x;
    sRect->y = y;
}

void ofxUIScrollableSliderCanvas::setDimensions(float _width, float _height)
{
    sRect->setWidth(MIN(_width, ofGetWidth() - sRect->getX()));
    sRect->setHeight(MIN(_height, ofGetHeight() - sRect->getY()));
    rect->setWidth(_width);
    rect->setHeight(_height);
    paddedRect->width = rect->width+padding*2;
    paddedRect->height = rect->height+padding*2;
}


void ofxUIScrollableSliderCanvas::drawScrollableRect()
{
    sRect->draw();
}

#ifdef OFX_UI_TARGET_TOUCH

void ofxUIScrollableSliderCanvas::touchDown(float x, float y, int id)
{
    if(sRect->inside(x, y))
    {
        for(vector<ofxUIWidget *>::iterator it = widgets.begin(); it != widgets.end(); ++it)
        {
            if((*it)->isHit(x, y))
            {
                if((*it)->isDraggable())
                {
                    hitWidget = true;
                }
                (*it)->touchDown(x, y, id);
            }
        }
    }
    
    if(sRect->inside(x, y) && id == 0)
    {
        hit = true;
        isScrolling = false;
        vel.set(0);
    }
}

void ofxUIScrollableSliderCanvas::touchMoved(float x, float y, int id)
{
    for(vector<ofxUIWidget *>::iterator it = widgets.begin(); it != widgets.end(); ++it)
    {
        if((*it)->isVisible())	(*it)->touchMoved(x, y, id);
    }
    
    if(hit && id == 0)
    {
        if(!hitWidget)
        {
            if(isScrolling != true)
            {
                isScrolling = true;
                ppos = ofPoint(x, y);
                vel.set(0);
            }
            else
            {
                pos = ofPoint(x, y);
                vel = pos-ppos;
                if(scrollX) rect->x +=vel.x;
                if(scrollY) rect->y +=vel.y;
                ppos = pos;
            }
        }
    }
}

void ofxUIScrollableSliderCanvas::touchUp(float x, float y, int id)
{
    for(vector<ofxUIWidget *>::iterator it = widgets.begin(); it != widgets.end(); ++it)
    {
        if((*it)->isVisible())	(*it)->touchUp(x, y, id);
    }
    
    hit = false;
    hitWidget = false;
    if(isScrolling)
    {
        isScrolling = false;
        pos = ofPoint(x, y);
    }
}

void ofxUIScrollableSliderCanvas::touchCancelled(float x, float y, int id)
{
    for(vector<ofxUIWidget *>::iterator it = widgets.begin(); it != widgets.end(); ++it)
    {
        if((*it)->isVisible())	(*it)->touchUp(x, y, id);
    }
    
    hit = false;
    hitWidget = false;
    if(isScrolling)
    {
        isScrolling = false;
        pos = ofPoint(x, y);
    }
}

#endif

void ofxUIScrollableSliderCanvas::mouseDragged(int x, int y, int button)
{
    for(vector<ofxUIWidget *>::iterator it = widgets.begin(); it != widgets.end(); ++it)
    {
        if((*it)->isVisible())	(*it)->mouseDragged(x, y, button);
    }
    
    if(hit)
    {
        if(!hitWidget)
        {
            if(isScrolling != true)
            {
                isScrolling = true;
                ppos = ofPoint(x,y);
                vel.set(0,0);
            }
            else
            {
                pos = ofPoint(x, y);
                vel = pos-ppos;
                if(scrollX) rect->x +=vel.x;
                if(scrollY) rect->y +=vel.y;
                ppos = pos;
            }
        }
    }
}

void ofxUIScrollableSliderCanvas::mousePressed(int x, int y, int button)
{
    if(sRect->inside(x, y))
    {
        hit = true;
        for(vector<ofxUIWidget *>::iterator it = widgets.begin(); it != widgets.end(); ++it)
        {
            if((*it)->isVisible())
            {
                if((*it)->isHit(x, y))
                {
                    if((*it)->isDraggable())
                    {
                        hitWidget = true;
                    }
                    (*it)->mousePressed(x, y, button);
                }
            }
        }
    }
    
    isScrolling = false;
    vel.set(0,0);
}

void ofxUIScrollableSliderCanvas::mouseReleased(int x, int y, int button)
{
    for(vector<ofxUIWidget *>::iterator it = widgets.begin(); it != widgets.end(); ++it)
    {
        if((*it)->isVisible()) (*it)->mouseReleased(x, y, button);
    }
    
    hit = false;
    hitWidget = false;
    if(isScrolling)
    {
        isScrolling = false;
        pos = ofPoint(x,y);
    }
}

ofxUIRectangle *ofxUIScrollableSliderCanvas::getSRect()
{
    return sRect;
}


bool ofxUIScrollableSliderCanvas::isHit(int x, int y)
{
    if(isEnabled() && sRect->inside(x, y))
    {
        return true;
    }
    else
    {
        for (map<string, ofxUIWidget*>::iterator it=widgetsAreModal.begin() ; it != widgetsAreModal.end(); it++ )
        {
            if((*it).second->isVisible() && (*it).second->isHit(x, y))
            {
                return true;
            }
        }
    }
    return false;
}

void ofxUIScrollableSliderCanvas::setMappedScrollPos(float _posScroll){
	
	if(_posScroll >= 0 && _posScroll <= 1){
		posScrollbar = _posScroll;
	}
}


ofVec2f ofxUIScrollableSliderCanvas::calcHeightContents(vector<ofxUIWidget*> _auxwidgets)
{
	ofVec2f maximums = ofVec2f(0,0);
    float maxWidth = 0;
    float maxHeight = 0;
    
    for(vector<ofxUIWidget *>::iterator it = _auxwidgets.begin(); it != _auxwidgets.end(); ++it)
    {
        if((*it)->isVisible())
        {
            ofxUIRectangle* wr = (*it)->getRect();
            ofxUIRectangle* wrp = (*it)->getPaddingRect();
            float widgetwidth = wr->getX()+wrp->getWidth() - rect->getX();
            float widgetheight = wr->getY()+wrp->getHeight() - rect->getY();
            
            float widgetpaddingwidth = wrp->getX()+wrp->getWidth() - rect->getX();
            float widgetpaddingheight = wrp->getY()+wrp->getHeight() - rect->getY();
            
            
            if(widgetwidth > maxWidth)
            {
                maxWidth = widgetwidth;
            }
            else if(widgetpaddingwidth > maxWidth)
            {
                maxWidth = widgetpaddingwidth;
            }
            
            if(widgetheight > maxHeight)
            {
                maxHeight = widgetheight;
            }
            else if(widgetpaddingheight > maxHeight)
            {
                maxHeight = widgetpaddingheight;
            }
        }
    }
	maximums = ofVec2f(maxWidth, maxHeight);	
	return maximums;
}
//--------------------------------------------------------------
void ofxUIScrollableSliderCanvas::updateScrollBarSize(vector<ofxUIWidget*> _auxwidgets, float maxrange, float minrange){
	
	if(scrollY){
		
		float sizeScrollbar = -1;
		int sizeHContent = calcHeightContents(getWidgets()).y;// y is heigh, x is width
		sizeScrollbar = ofxUIMap(sizeHContent, 0, maxrange, minrange, 10, true);
		ofxUIScrollSlider* scrollSlider = (ofxUIScrollSlider*)gui_slider->getWidget("S");
		
		scrollSlider->setValueHigh(scrollSlider->getScaledValueHigh()+sizeScrollbar*0.5);
		scrollSlider->setValueLow(scrollSlider->getScaledValueHigh()-sizeScrollbar*0.5);
	}
	
}

//--------------------------------------------------------------
void ofxUIScrollableSliderCanvas::updateScrollPosition(int max){
	if(scrollY){
		
		vector<ofxUIWidget*> auxwidgets = getWidgets();
		ofVec2f maxims = calcHeightContents(auxwidgets); 
		int sizeHContent = maxims.y;
		
		//Find real canvas position. From 'y' position to 'maxY + y'
		float posmap = ofxUIMap(posScrollbar, 0, 1, +sRect->y, -sizeHContent+max+sRect->y, true); 
		
		//finally move the canvas to direct pos finded between the maximum and minimum
		rect->y = posmap;//floor(posmap); //force widgets to draw in pixel
	}
}

//c
//--------------------------------------------------------------
void ofxUIScrollableSliderCanvas::adjustContentstoGui(bool _bsnap){
	
	if(_bsnap){
		autoSizeToFitWidgets(); 
	}
	else {
		setSnapping(_bsnap); //Auto damping levels only works for full size window
		updateScrollBarSize(getScroll()->getWidgets(), 3000 , 500); // set new default size depending content inside // max , min
	}
}

//--------------------------------------------------------------
void ofxUIScrollableSliderCanvas::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	int kind = e.widget->getKind();
	
	if(name == "S"){
		ofxUIScrollSlider* scrollSlider =  (ofxUIScrollSlider *)e.widget;
		float mapvalscroll = scrollSlider->getPosScrollBar();
		setMappedScrollPos(mapvalscroll);
		updateScrollPosition(scrollSlider->getMax());		
	}
}

//--------------------------------------------------------------
ofxUICanvas* ofxUIScrollableSliderCanvas::getScroll(){
	// TODO try to find a better name
	return gui_slider;
}


void ofxUIScrollableSliderCanvas::setVisible(bool _visible)
{
	
	ofxUIScrollSlider* scrollSlider	=  (ofxUIScrollSlider *) gui_slider->getWidget("S");
    visible = _visible;
    if(visible)
    {
        enable();
		scrollSlider->setVisible(true);
		gui_slider->setVisible(true);
    }
    else
    {
        disable();
		scrollSlider->setVisible(false);
		gui_slider->setVisible(false);
    }
}


