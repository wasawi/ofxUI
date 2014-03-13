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

/*
 *  ofxUIScrollSlider.h
 *  Scroll bar added by Carles on 20/01/14.
 *
 */

#include "ofxUIScrollSlider.h"
#include "ofxUI.h"

ofxUIScrollSlider::ofxUIScrollSlider(string _name, float _min, float _max, float _valuelow, float _valuehigh, float w, float h,
                 float x, float y, int _size) : ofxUIWidgetWithLabel()
{
    useReference = false;
    init(_name, _min, _max, &_valuelow, &_valuehigh, w, h, x, y, _size);
}

ofxUIScrollSlider::ofxUIScrollSlider(string _name, float _min, float _max, float *_valuelow, float *_valuehigh, float w, float h,
                 float x, float y, int _size) : ofxUIWidgetWithLabel()
{
    useReference = true;
    init(_name, _min, _max, _valuelow, _valuehigh, w, h, x, y, _size);
}
/*
ofxUIScrollSlider::ofxUIScrollSlider(float x, float y, float w, float h, float _min, float _max, float _valuelow, float _valuehigh, string _name, int _size) : ofxUIWidgetWithLabel()
{
    useReference = false;
    init(_name, _min, _max, &_valuelow, &_valuehigh, w, h, x, y, _size);
    //        ofLogWarning("ofxUIScrollSlider: DON'T USE THIS CONSTRUCTOR. THIS WILL BE REMOVED ON FUTURE RELEASES.");
}

ofxUIScrollSlider::ofxUIScrollSlider(float w, float h, float _min, float _max, float _valuelow, float _valuehigh, string _name, int _size) : ofxUIWidgetWithLabel()
{
    useReference = false;
    init(_name, _min, _max, &_valuelow, &_valuehigh, w, h, 0, 0, _size);
    //        ofLogWarning("ofxUIScrollSlider: DON'T USE THIS CONSTRUCTOR. THIS WILL BE REMOVED ON FUTURE RELEASES.");
}

ofxUIScrollSlider::ofxUIScrollSlider(float x, float y, float w, float h, float _min, float _max, float *_valuelow, float *_valuehigh, string _name, int _size) : ofxUIWidgetWithLabel()
{
    useReference = true;
    init(_name, _min, _max, _valuelow, _valuehigh, w, h, x, y, _size);
    //        ofLogWarning("ofxUIScrollSlider: DON'T USE THIS CONSTRUCTOR. THIS WILL BE REMOVED ON FUTURE RELEASES.");
}

ofxUIScrollSlider::ofxUIScrollSlider(float w, float h, float _min, float _max, float *_valuelow, float *_valuehigh, string _name, int _size) : ofxUIWidgetWithLabel()
{
    useReference = true;
    init(_name, _min, _max, _valuelow, _valuehigh, w, h, 0, 0, _size);
    //        ofLogWarning("ofxUIScrollSlider: DON'T USE THIS CONSTRUCTOR. THIS WILL BE REMOVED ON FUTURE RELEASES.");
}
*/
ofxUIScrollSlider::~ofxUIScrollSlider()
{
    if(!useReference)
    {
        delete valuelowRef;
        delete valuehighRef;
    }
}

void ofxUIScrollSlider::init(string _name, float _min, float _max, float *_valuelow, float *_valuehigh, float w, float h,
          float x, float y, int _size)
{
    rect = new ofxUIRectangle(x,y,w,h);
    name = string(_name);
    if(w > h)
    {
        kind = OFX_UI_WIDGET_SSLIDER_H;
    }
    else
    {
        kind = OFX_UI_WIDGET_SSLIDER_V;
    }
    
    paddedRect = new ofxUIRectangle(-padding, -padding, w+padding*2.0, h+padding);
    paddedRect->setParent(rect);
    
    draw_fill = true;
    
    valuelow = *_valuelow;                                                  //the widget's value
    valuehigh = *_valuehigh;                                                //the widget's value
    
    if(useReference)
    {
        valuelowRef = _valuelow;
        valuehighRef = _valuehigh;
    }
    else
    {
        valuelowRef = new float();
        valuehighRef = new float();
        *valuelowRef = valuelow;
        *valuehighRef = valuehigh;
    }
    
    max = _max;
    min = _min;
    hitHigh = false;
    hitLow = false;
    hitCenter = false;
    
    if(valuelow > max)
    {
        valuelow = max;
    }
    if(valuelow < min)
    {
        valuelow = min;
    }
    
    if(valuehigh > max)
    {
        valuehigh = max;
    }
    if(valuehigh < min)
    {
        valuehigh = min;
    }
    
    valuelow = ofxUIMap(valuelow, min, max, 0.0, 1.0, true);
    valuehigh = ofxUIMap(valuehigh, min, max, 0.0, 1.0, true);
    labelPrecision = 2;
    
    if(kind == OFX_UI_WIDGET_RSLIDER_H)
    {
        label = new ofxUILabel(0,h+padding,(name+" LABEL"), (name + ": " + ofxUIToString(getScaledValueLow(),labelPrecision) + " " + ofxUIToString(getScaledValueHigh(),labelPrecision)), _size);
    }
    else
    {
        label = new ofxUILabel(0,h+padding,(name+" LABEL"), name, _size);
    }
    
    label->setParent(label);
    label->setRectParent(rect);
    label->setEmbedded(true);
    
    increment = fabs(max - min) / 10.0;

	//j
	rangeval = valuehigh - valuelow;
}

void ofxUIScrollSlider::update()
{
    if(useReference)
    {
        valuelow = ofxUIMap(*valuelowRef, min, max, 0.0, 1.0, true);
        valuehigh = ofxUIMap(*valuehighRef, min, max, 0.0, 1.0, true);
    }
}

void ofxUIScrollSlider::setDrawPadding(bool _draw_padded_rect)
{
    draw_padded_rect = _draw_padded_rect;
    label->setDrawPadding(false);
}

void ofxUIScrollSlider::setDrawPaddingOutline(bool _draw_padded_rect_outline)
{
    draw_padded_rect_outline = _draw_padded_rect_outline;
    label->setDrawPaddingOutline(false);
}

void ofxUIScrollSlider::drawFill()
{
    if(draw_fill)
    {
        ofxUIFill();
        ofxUISetColor(color_fill);
        if(kind == OFX_UI_WIDGET_RSLIDER_H)
        {
            ofxUIDrawRect(rect->getX()+rect->getWidth()*valuelow, rect->getY(), rect->getWidth()*(valuehigh-valuelow), rect->getHeight());
        }
        else
        {
            ofxUIDrawRect(rect->getX(), rect->getY()+(1.0-valuehigh)*rect->getHeight(), rect->getWidth(), rect->getHeight()*(valuehigh-valuelow));
        }
    }
}

void ofxUIScrollSlider::drawFillHighlight()
{
	
    if(draw_fill_highlight)
    {
        ofxUIFill();
        ofxUISetColor(color_fill_highlight);
        if(kind == OFX_UI_WIDGET_SSLIDER_H)
        {
            ofxUIDrawRect(rect->getX()+rect->getWidth()*valuelow, rect->getY(), rect->getWidth()*(valuehigh-valuelow), rect->getHeight());
        }
        else
        {
            ofxUIDrawRect(rect->getX(), rect->getY()+(1.0-valuehigh)*rect->getHeight(), rect->getWidth(), rect->getHeight()*(valuehigh-valuelow));
        }
		if(kind == OFX_UI_WIDGET_SSLIDER_V)
		{
//            label->drawString(rect->getX()+rect->getWidth()+padding, label->getRect()->getHeight()/2.0+rect->getY()+rect->getHeight()-rect->getHeight()*valuehigh, valuehighString);
//            label->drawString(rect->getX()+rect->getWidth()+padding, label->getRect()->getHeight()/2.0+rect->getY()+rect->getHeight()-rect->getHeight()*valuelow, valuelowString);
			
//			float middleval = getPercentValueMiddle();
//			float mapmiddleval = getScrollPosition();
			
			//if(ofGetLogLevel()== OF_LOG_VERBOSE)label->drawString(rect->getX()+rect->getWidth()+padding, middleval, ofxUIToString(mapmiddleval,labelPrecision)); 
		}
    }
}

void ofxUIScrollSlider::mouseMoved(int x, int y )
{
    if(rect->inside(x, y))
    {
        state = OFX_UI_STATE_OVER;
    }
    else
    {
        state = OFX_UI_STATE_NORMAL;
    }
    stateChange();
}

void ofxUIScrollSlider::mouseDragged(int x, int y, int button)
{
	/*
	if(rect->inside(x, y))
    {
        hit = true;
	}else hit = false;
	*/
	
    if(hit)
    {
        state = OFX_UI_STATE_DOWN;
        hitValueHigh = valuehigh;
        hitValueLow = valuelow;
        input(x, y);
        triggerEvent(this);
    }
    else
    {
        state = OFX_UI_STATE_NORMAL;
    }
    stateChange();
}

void ofxUIScrollSlider::mousePressed(int x, int y, int button)
{
    if(rect->inside(x, y))
    {
        hit = true;
        state = OFX_UI_STATE_DOWN;
        hitValueHigh = valuehigh;
        hitValueLow = valuelow;
        input(x, y);
        triggerEvent(this);
    }
    else
    {
        state = OFX_UI_STATE_NORMAL;
    }
    stateChange();
}

void ofxUIScrollSlider::mouseReleased(int x, int y, int button)
{
    if(hit)
    {
#ifdef TARGET_OPENGLES
        state = OFX_UI_STATE_NORMAL;
#else
        state = OFX_UI_STATE_OVER;
#endif
        input(x, y);
        triggerEvent(this);
    }
    else
    {
        state = OFX_UI_STATE_NORMAL;
    }
    stateChange();
    hit = false;
    hitHigh = false;
    hitLow = false;
    hitCenter = false;
}

void ofxUIScrollSlider::keyPressed(int key)
{
	/*
    if(state == OFX_UI_STATE_OVER)
    {
        switch (key)
        {
            case OF_KEY_RIGHT:
                setValueHigh(getScaledValueHigh()+increment);
                setValueLow(getScaledValueLow()+increment);
                triggerEvent(this);
                break;
                
            case OF_KEY_LEFT:
                setValueHigh(getScaledValueHigh()-increment);
                setValueLow(getScaledValueLow()-increment);
                triggerEvent(this);
                break;
                
                
            case OF_KEY_UP:
                setValueHigh(getScaledValueHigh()+increment);
                setValueLow(getScaledValueLow()-increment);
                triggerEvent(this);
                break;
                
                
            case OF_KEY_DOWN:
                setValueHigh(getScaledValueHigh()-increment);
                setValueLow(getScaledValueLow()+increment);
                triggerEvent(this);
                break;
                
            default:
                break;
        }
    }
	 */
}

void ofxUIScrollSlider::keyReleased(int key)
{
    
}

void ofxUIScrollSlider::windowResized(int w, int h)
{
    
}

void ofxUIScrollSlider::setIncrement(float _increment)
{
    increment = _increment;
}

void ofxUIScrollSlider::input(float x, float y)
{
    float v = 0;
	
    if(kind == OFX_UI_WIDGET_SSLIDER_H)
    {
        v = rect->percentInside(x, y).x;
    }
    else
    {
        v = 1.0-rect->percentInside(x, y).y;
    }
	
	//Set limits and values
	rangeval = valuehigh - valuelow;
	float halfrange = rangeval*0.5;
	valuehigh = (v+halfrange); 
	valuelow = (v-halfrange);

	// if inside limits
	if((valuehigh < 1) && (valuelow > 0)){ 
		hitPoint = v; 
	}
	// if hitBottom
	else if(valuehigh >= 1){
		valuehigh = 1; 
		valuelow= 1 - rangeval;
		hitPoint = 1 - halfrange; 
	}
	// if hitTop
	else if(valuelow <= 0){
		valuehigh = rangeval; 
		valuelow = 0;
		hitPoint = halfrange; 
	}
	else {
		if(ofGetLogLevel()== OF_LOG_VERBOSE)cout << "Error: ofxUISCrollSlider::input: Out of Limits" << endl;
	}
	
	//Set right hitPoint depending range size
	if(v > 0.5){
		hitPoint = ofxUIMap(valuehigh, 0.5, 1, 0.5, 0, true); // invert v values // to 0
	}
	else if(v < 0.5){
		hitPoint = ofxUIMap(valuelow, 0.5, 0, 0.5, 1, true); // invert v values // to 1
	}
	
    updateValueRef();
    updateLabel();
}

void ofxUIScrollSlider::updateValueRef()
{
    (*valuelowRef) = getScaledValueLow();
    (*valuehighRef) = getScaledValueHigh();
}

void ofxUIScrollSlider::updateLabel()
{
    valuelowString = ofxUIToString(getValueLow(),labelPrecision);
    valuehighString = ofxUIToString(getValueHigh(),labelPrecision);
    
    if(kind == OFX_UI_WIDGET_RSLIDER_H)
    {
        label->setLabel(name + ": " + valuelowString + " " + valuehighString);
    }
}

void ofxUIScrollSlider::stateChange()
{
    if(valuehigh > 0)
    {
        draw_fill = true;
    }
    else
    {
        draw_fill = false;
    }
    
    switch (state) {
        case OFX_UI_STATE_NORMAL:
        {
            draw_fill_highlight = false;
            draw_outline_highlight = false;
            label->unfocus();
        }
            break;
        case OFX_UI_STATE_OVER:
        {
            draw_fill_highlight = false;
            draw_outline_highlight = true;
            label->unfocus();
        }
            break;
        case OFX_UI_STATE_DOWN:
        {
            draw_fill_highlight = true;
            draw_outline_highlight = true;
            label->focus();
        }
            break;
        case OFX_UI_STATE_SUSTAINED:
        {
            draw_fill_highlight = false;
            draw_outline_highlight = false;
            label->unfocus();
        }
            break;
            
        default:
            break;
    }
}

void ofxUIScrollSlider::setVisible(bool _visible)
{
	visible = _visible;
	label->setVisible(visible);
}

void ofxUIScrollSlider::setValueLow(float _value)
{
    valuelow = ofxUIMap(_value, min, max, 0.0, 1.0, true);
    updateValueRef();
    updateLabel();
}

void ofxUIScrollSlider::setValueHigh(float _value)
{
    valuehigh = ofxUIMap(_value, min, max, 0.0, 1.0, true);
    updateValueRef();
    updateLabel();
}

float ofxUIScrollSlider::getValueLow()
{
    return (*valuelowRef);
}

float ofxUIScrollSlider::getValueHigh()
{
    return (*valuehighRef);
}

float ofxUIScrollSlider::getNormalizedValueLow()
{
    return valuelow;
}

float ofxUIScrollSlider::getNormalizedValueHigh()
{
    return valuehigh;
}

float ofxUIScrollSlider::getPercentValueLow()
{
    return valuelow;
}

float ofxUIScrollSlider::getPercentValueHigh()
{
    return valuehigh;
}

float ofxUIScrollSlider::getScaledValueLow()
{
    return ofxUIMap(valuelow, 0.0, 1.0, min, max, true);
}

float ofxUIScrollSlider::getScaledValueHigh()
{
    return ofxUIMap(valuehigh, 0.0, 1.0, min, max, true);
}

void ofxUIScrollSlider::setLabelVisible(bool _labelVisible)
{
    label->setVisible(_labelVisible);
}

void ofxUIScrollSlider::setLabelPrecision(int _precision)
{
    labelPrecision = _precision;
    updateValueRef();
    updateLabel();
}

void ofxUIScrollSlider::setParent(ofxUIWidget *_parent)
{
    parent = _parent;
    calculatePaddingRect();
}

void ofxUIScrollSlider::setMax(float _max)
{
    setMaxAndMin(_max, min);
}

void ofxUIScrollSlider::setMin(float _min)
{
    setMaxAndMin(max, _min);
}

void ofxUIScrollSlider::setMaxAndMin(float _max, float _min)
{
    max = _max;
    min = _min;
    
    valuelow= ofxUIMap(valuelow, 0, 1.0, min, max, true);
    valuelow = ofxUIMap(valuelow, min, max, 0.0, 1.0, true);
    
    valuehigh = ofxUIMap(valuehigh, 0, 1.0, min, max, true);
    valuehigh = ofxUIMap(valuehigh, min, max, 0.0, 1.0, true);
    updateValueRef();
    updateLabel();
}

bool ofxUIScrollSlider::isDraggable()
{
    return true;
}



float ofxUIScrollSlider::getMax()
{
    return max;
}

float ofxUIScrollSlider::getMin()
{
    return min;
}

float ofxUIScrollSlider::getScrollPosition()
{
	currentPosition=0;
	
/*	cout << "***********"<< endl;
	cout << "valuelow = "<< valuelow << endl;
	cout << "valuehigh = "<< valuehigh << endl;
	cout << "hitValueLow = "<< hitValueLow << endl;
	cout << "hitValueHigh = "<< hitValueHigh << endl;
	cout << "hitPoint = "<< hitPoint << endl;
	cout << "max = "<< max << endl;
	cout << "min = "<< min << endl;
	cout << "rangeval = "<< rangeval << endl;
	cout << "min = "<< min << endl;
	cout << "-----------"<< endl;
	cout << "x = "<< rect->x << endl;
	cout << "y = "<< rect->y << endl;
	cout << "w = "<< rect->width << endl;
	cout << "h = "<< rect->height << endl;
*/
	currentPosition = ofMap(valuelow, 0, 1-rangeval, 0, 1);
	
	return currentPosition;
}

void ofxUIScrollSlider::setScrollPosition(float _y)
{
	rangeval = valuehigh - valuelow;
/*	cout << "*************************"<< endl;
	cout << "goto y = "<< _y << endl;
	cout << "rangeval = "<< rangeval << endl;
*/
	float _valuelow = ofMap(_y, 0, 1, 0, 1-rangeval, true);
	setValueLow(_valuelow);
//	cout << "valuelow = "<< _valuelow << endl;
	
	// make sure it falls inside the slider, so clamp it
	float _valuehigh = ofMap(_y, 0, 1, rangeval, 1, true);
//	cout << "valuehigh = "<< _valuehigh << endl;
	setValueHigh(_valuehigh);
	rangeval = valuehigh - valuelow;
}





