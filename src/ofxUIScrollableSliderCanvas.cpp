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
	if (bScrollBar) disableScrollBar();
	if (bFBO) disableFBO();
}

ofxUIScrollableSliderCanvas::ofxUIScrollableSliderCanvas(float x, float y, float w, float h, float sliderW) : ofxUICanvas(x,y,w,h)
{
	scrollRect.width	= sliderW;
	initScrollable();
}

ofxUIScrollableSliderCanvas::ofxUIScrollableSliderCanvas() : ofxUICanvas()
{
    initScrollable();
}

ofxUIScrollableSliderCanvas::ofxUIScrollableSliderCanvas(ofxUICanvas *sharedResources) : ofxUICanvas(sharedResources)
{
    initScrollable();
}

void ofxUIScrollableSliderCanvas::initScrollable()
{
    kind = OFX_UI_WIDGET_SCROLLABLECANVAS;
    sRect = new ofxUIRectangle(rect->x, rect->y, rect->getWidth(), rect->getHeight());
	
    paddedRect->setParent(sRect);
    isScrolling = false;
    vel.set(0);
    pos.set(0);
    ppos.set(0);
    acc.set(0);
    damping = .75;
    scrollX = false;
    scrollY = true;

	scrollRect.width = 20;
	bScrollBar=false;
	
    nearTop = true;
    nearBot = true;
    nearRight = false;
    nearLeft = false;
	
    hitWidget = true;
    stickyDistance = 32;
    hit = true;
    snapping = true;
	draw_scrollCanvas = false;
	
	counter=0;
	bFBO=false;
#ifdef OFX_UI_TARGET_TOUCH
    touchId = -1;
#endif
}

void ofxUIScrollableSliderCanvas::setupScrollBar(string _name, float _min, float _max, float _lowvalue, float _highvalue, int _w, int _h, int _x, int _y, int _size){

	sliderCanvas = new ofxUICanvas(_x, _y, _w, _h);
	sliderCanvas->setDrawBack(true);
	float spacing = 0.1;
	sliderCanvas->setWidgetSpacing(spacing);
	sliderWidget = (ofxUIScrollSlider*)sliderCanvas->addWidgetRight(new ofxUIScrollSlider(_name,
													 _min,
													 _max,
													 _lowvalue,
													 _highvalue,
													 _w - spacing*2,
													 _h - spacing*2,
													 _x,
													 _y,
													 OFX_UI_FONT_SMALL));
	
	sliderCanvas->setDrawPaddingOutline(false);
	
	ofAddListener(sliderCanvas->newGUIEvent,this,&ofxUIScrollableSliderCanvas::guiEvent);
}

void ofxUIScrollableSliderCanvas::setDamping(float _damping)
{
    damping = _damping;
}

void ofxUIScrollableSliderCanvas::setSnapping(bool _snapping)
{
    snapping = _snapping;
}

void ofxUIScrollableSliderCanvas::enableScrollBar()		////j ***********************
{
	bScrollBar=true;
	
	// how big is the scrollbar?
	scrollRect.height = getVisibleCanvasPercent().y;
//	cout << "scrollRect.height = "<< scrollRect.height<< endl;
	
	setupScrollBar("_Scroll",					// string _name,
				   0,							// float _min,
				   1,							// float _max,
				   0,							// int _lowvalue,
				   scrollRect.height,			// int _highvalue,
				   scrollRect.width,			// int _w,
				   sRect->getHeight(),			// int _h,
				   sRect->x+sRect->getWidth(),	// int _x,
				   sRect->y,					// int _y,
				   OFX_UI_FONT_SMALL);

	updateScrollBarPosition();

//	this must be called in your application after creating widgets
//	autoSizeToFitWidgets();
// TODO: (now it is not possible to modify widgets width from canvas)
// modify sRect width changes widgets width so that widgets will be drawn
}

void ofxUIScrollableSliderCanvas::disableScrollBar()	////j ***********************
{
	bScrollBar	= false;
	delete sliderCanvas;
}

void ofxUIScrollableSliderCanvas::toggleScrollBar()		////j ***********************
{
	if (bScrollBar) disableScrollBar(); else enableScrollBar();
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

void ofxUIScrollableSliderCanvas::enableFBO()		////j ***********************
{
	bFBO=true;
	fboRect = new ofxUIRectangle;
	fbo = new ofFbo;
	fboRect->set(sRect->x, sRect->y, sRect->getWidth(), sRect->getHeight());
	fbo->allocate(fboRect->getWidth(),  fboRect->getHeight(), GL_RGBA,0);
}

void ofxUIScrollableSliderCanvas::disableFBO()		////j ***********************
{
	bFBO=false;
	delete fboRect;
	delete fbo;
}

void ofxUIScrollableSliderCanvas::toggleFBO()		////j ***********************
{
	
	if (bFBO)
	{
		disableFBO();
	}
	else
	{
		enableFBO();
		
		//useful for debugging blendings
//		cout << counter<< endl;
		if (counter<5) counter++; else counter=0;

	}

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
        
        if(scrollY && snapping)						// if the scroll is vertical
        {
            float dyTop = rect->y - sRect->y;
            float dyBot = (sRect->y+sRect->getHeight()) - (rect->y+rect->getHeight());
            //---------------
			if(fabs(dyBot) < stickyDistance)		// if its near top
            {
                nearTop = false;
                nearBot = true;
            }
            if(fabs(dyTop) < stickyDistance)		// if its near bottom
            {
                nearTop = true;
                nearBot = false;
            }
            //---------------
            if(dyTop > 0)							// it it is below top
            {
//				cout<<"dyTop ="<<dyTop<<endl;
                acc.y += (-dyTop)/10.0;
                dampenY();
            }
            else if(nearTop)						// if it is above top
            {
//                acc.y += (-dyTop)/10.0;
//                dampenY();
            }
            //---------------
            if(dyBot > 0)							// if it is above bottom
            {
                acc.y += (dyBot)/10.0;
                dampenY();
            }
            else if(nearBot)						// if it is below bottom
            {
//                acc.y += (dyBot)/10.0;
//                dampenY();
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
	else // if scrolling
	{
		updateScrollBarPosition();
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
	if(draw_scrollCanvas)
	{   // use this for debugging
		ofNoFill();
        ofxUISetColor(color_outline_highlight);
		rect->draw();
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
		
		fbo->begin();
		
		// move the canvas to fbo coordinates
		rect->x = 0;
		rect->y = rect->y - fboRect->y;
		sRect->x = 0;
		sRect->y = sRect->y - fboRect->y;
		
		
		/*
		 keep this for debugging:
		 
		 OK this is very ugly..
		 It is very hard to get around this problem..
		 we cant apply the alpha two times so we will draw with aplha blending disabled
		 and we will draw the fbo with		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		 http://forum.openframeworks.cc/t/fbo-problems-with-alpha/1643/10
		 http://forum.openframeworks.cc/t/weird-problem-rendering-semi-transparent-image-to-fbo/2215/4
		 */

		// keep this for debugging:
		ofEnableBlendMode(OF_BLENDMODE_DISABLED);	//2 //3
//		ofEnableBlendMode(OF_BLENDMODE_ALPHA);
//		ofEnableBlendMode(OF_BLENDMODE_ADD);
//		ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
//		ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
//		ofEnableBlendMode(OF_BLENDMODE_SCREEN);

		// use this to try them all
//		ofEnableBlendMode(static_cast<ofBlendMode>(counter));

//		glEnable(GL_BLEND);
//		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
//		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);

		
//		ofClear(OFX_UI_COLOR_BACK_ALPHA);
//		ofClear(0, 100);		//1
//		ofClearAlpha();			//2 works but font is too transparent
//		ofClear(0,255);			//2
		ofClear(0,0);			//3 same result as 2
//		ofSetColor(0);
		
	}
	else
	{
		ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	}
	
    ofxUIPushStyle();
	
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
	
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
		fbo->end();
//		ofEnableAlphaBlending();
//		ofEnableBlendMode(static_cast<ofBlendMode>(counter));
//		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

		ofSetColor(255);
		fbo->draw(fboRect->getX(), fboRect->getY());
		
		// put the canvas back
		rect->x = fboRect->x;
		rect->y = rect->y + fboRect->y;
		sRect->x = fboRect->x;
		sRect->y = sRect->y + fboRect->y;
	}
	//this has to be enabled for sure
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
//	sRect->setWidth(MIN(_width, ofGetWidth() - sRect->getX()));
//	sRect->setHeight(MIN(_height, ofGetHeight() - sRect->getY()));
	rect->setWidth(_width);
	rect->setHeight(_height);
//	paddedRect->width = rect->width+padding*2;
//	paddedRect->height = rect->height+padding*2;
	if (bScrollBar) updateScrollBarSize();
	if (bScrollBar) updateScrollBarPosition();
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
    if(isEnabled())
    {
		if(sRect->inside(x, y) || sliderCanvas->isHit(x, y)){
        return true;
		}
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

//--------------------------------------------------------------
ofVec2f ofxUIScrollableSliderCanvas::getVisibleCanvasPercent()
{
	ofVec2f percent = ofVec2f(0,0);
	
	// how much canvas is not visible?
	percent.x = sRect->getWidth()/rect->getWidth();
	percent.y = sRect->getHeight()/rect->getHeight();
	
	return percent;
}

//--------------------------------------------------------------
ofVec2f ofxUIScrollableSliderCanvas::getCanvasPosition()
{
	ofVec2f position = ofVec2f(0,0);
	
	//relative position in pixels
	float currentPosition = rect->y-sRect->y;
	
	//maximum movement in pixels
	float maximumMovement = (rect->getHeight() - sRect->getHeight()) *-1;
	
	float postitionPercent = ofMap(currentPosition,
								   0,
								   maximumMovement,
								   1,
								   0,
								   true);
	
	position.y = postitionPercent;
	
	return position;
}

//--------------------------------------------------------------
void ofxUIScrollableSliderCanvas::setCanvasPosition(ofVec2f _position)
{
	// move canvas vertically (for now)
	float postitionInPixels = ofMap(_position.y,
									1 ,
									0 ,
									0 ,
									rect->getHeight() - sRect->getHeight());

	//cout << "rect->getHeight() = " << rect->getHeight() << endl;
	//cout << "_position.y = " << _position.y << endl;
	//cout << "postitionInPixels = " << postitionInPixels << endl;
	
	// in of you move negatively to move things up, so invert
	postitionInPixels *= -1;
	
	rect->y =postitionInPixels+sRect->y;
	
}


//--------------------------------------------------------------
void ofxUIScrollableSliderCanvas::updateScrollBarSize(){
	
	if(scrollY){
		
		// get the size of the canvas rect out of bounds in %
		scrollRect.height = getVisibleCanvasPercent().y;
//		cout << "scrollRect.height*** = "<<scrollRect.height<< endl;
		if(bScrollBar){
			sliderWidget->setValueHigh(scrollRect.height);
			sliderWidget->setValueLow(0);
		}
	}
}

//--------------------------------------------------------------
void ofxUIScrollableSliderCanvas::updateScrollBarPosition(){
	if(scrollY){
		// find the current canvas position and apply it to the scroll
		scrollRect.y = getCanvasPosition().y;
		
		if(bScrollBar){
			sliderWidget->setScrollPosition(scrollRect.y);
//			cout << "setScrollPosition = "<<scrollRect.y<< endl;
		}
	}
}


//--------------------------------------------------------------
void ofxUIScrollableSliderCanvas::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	int kind = e.widget->getKind();
	
	if(name == "_Scroll"){
		float sliderValue = sliderWidget->getScrollPosition();
//		cout << "** slider value = "<<sliderValue <<endl;
//		cout << "** canvas value = "<<getCanvasPosition().y <<endl;

		ofVec2f _position;
		_position.y=sliderValue;

		setCanvasPosition(_position);
	}
}

//--------------------------------------------------------------
void ofxUIScrollableSliderCanvas::setVisible(bool _visible)
{
    visible = _visible;
    if(visible)
    {
        enable();
		sliderWidget->setVisible(true);
		sliderCanvas->setVisible(true);
    }
    else
    {
        disable();
		sliderWidget->setVisible(false);
		sliderCanvas->setVisible(false);
    }
}

//--------------------------------------------------------------
void ofxUIScrollableSliderCanvas::drawScrollCanvas(bool _visible)
{
    draw_scrollCanvas = _visible;
}


//--------------------------------------------------------------
void ofxUIScrollableSliderCanvas::setSliderWidth(float _sliderW)
{
	// TODO
}



