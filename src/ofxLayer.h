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

#ifndef OFXLAYER
#define OFXLAYER

#include "ofEvents.h"
#include "ofMain.h"

using namespace std; 

class ofxLayer; 
class ofxLayerManager; 
class ofxSharedAppData; 

class ofxLayerEventArgs : public ofEventArgs {
public:
    ofxLayerEventArgs( string layerName , ofxLayer *sender)
    {
        this->layerName = layerName; 
        this->sender = sender; 
    }
    ofxLayerEventArgs( string layerName , ofxLayer *sender, string message)
    {
        this->layerName = layerName; 
        this->sender = sender; 
        this->message = message;         
    }

    string layerName;
    string message; 
    ofxLayer *sender; 
};

class ofxLayer
{
public:
	ofxLayer()
	{
		active = false;
        bSetup = false;
        bDead = false; 
	}

	virtual ~ofxLayer() 
	{
	}
    
    virtual void setup() {} 
    virtual void update() {}
    virtual void draw() {}
    virtual void exit() {}
    
    virtual string getLayerName() {return layerName;}

    bool isActive() { return active; } 
    
    bool isSetup() { return bSetup; } 
    
    void setSetup(bool _bSetup)
    {
        bSetup = _bSetup;
    }

    virtual void activate() 
    {
        active = true; 
    }                              
	
    virtual void deactivate() 
    {
        active = false;     
    } 	
    
    void deleteMe()
    {
        setDead(true); 
        ofxLayerEventArgs args = ofxLayerEventArgs(layerName, this);
        ofNotifyEvent(deleteLayerEvent, args, this);
    }
    
    void deleteLayer(string name)
    {
        if(name == layerName)
        {
            setDead(true);
        }        
        ofxLayerEventArgs args = ofxLayerEventArgs(name, this);
        ofNotifyEvent(deleteLayerEvent, args, this);
    }
    
    void activateLayer(string name)
    {
        ofxLayerEventArgs args = ofxLayerEventArgs(name, this); 
        ofNotifyEvent(activateLayerEvent, args, this);
    }

    void deactivateLayer(string name)
    {
        ofxLayerEventArgs args = ofxLayerEventArgs(name, this); 
        ofNotifyEvent(deactivateLayerEvent, args, this);        
    }
    
    void switchLayer(string name)
    {
        ofxLayerEventArgs args = ofxLayerEventArgs(name, this);         
        ofNotifyEvent(switchLayerEvent, args, this);
    }

    void switchLayer(string name, string message)
    {
        ofxLayerEventArgs args = ofxLayerEventArgs(name, this, message);         
        ofNotifyEvent(switchLayerEvent, args, this);
    }
    
    void setManager(ofxLayerManager *_manager)
    {
        manager = _manager; 
    }

    void setDead(bool _bDead)
    {
        bDead = _bDead;
    }
    
    bool isDead()
    {
        return bDead; 
    }
    
    ofEvent<ofxLayerEventArgs> deleteLayerEvent;
    ofEvent<ofxLayerEventArgs> switchLayerEvent;
    ofEvent<ofxLayerEventArgs> activateLayerEvent;    
    ofEvent<ofxLayerEventArgs> deactivateLayerEvent;    
    
    virtual void touchDown(ofTouchEventArgs& touch) {}
    virtual void touchMoved(ofTouchEventArgs& touch) {}
    virtual void touchUp(ofTouchEventArgs& touch) {}
    virtual void touchDoubleTap(ofTouchEventArgs& touch) {}
    virtual void touchCancelled(ofTouchEventArgs& touch) {}
    
#ifndef TARGET_OPENGLES
    
    virtual void mouseReleased(ofMouseEventArgs& data) {}
    virtual void mousePressed(ofMouseEventArgs& data) {}
    virtual void mouseMoved(ofMouseEventArgs& data) {}
    virtual void mouseDragged(ofMouseEventArgs& data) {}
    
    virtual void keyPressed(int key) {}
    virtual void keyReleased(int key) {}
    virtual void windowResized(int w, int h) {}

#endif
    
    void setSharedAppData(ofxSharedAppData* sharedAppData) { this->sharedAppData = sharedAppData; }
    
#ifdef TARGET_ANDROID
    virtual void savePressed(string title, string tags){};
    virtual void imageSelected(string imageURL){};
    virtual void pause(){};
	virtual void stop(){};
	virtual void resume(){};
	virtual void reloadTextures(){}

    virtual void onKeyDown(int keyCode){}    
	virtual void onKeyUp(int keyCode){}
    
	virtual bool backPressed(){ return false; }
	virtual void menuPressed(){}
    
	virtual bool menuItemSelected(string menu_id_str){ return false; }    
	virtual bool menuItemChecked(string menu_id_str, bool checked){ return false; }
    
	virtual void okPressed(){};
	virtual void cancelPressed(){};
	virtual void gotFile(string url, string filename){};
	virtual void urlResponse(ofHttpResponse & response) {};
    
#endif
    
#ifdef TARGET_OF_IPHONE

    virtual void lostFocus() {}
    virtual void gotFocus() {}
    virtual void gotMemoryWarning() {}
    virtual void deviceOrientationChanged(int newOrientation) {}
    
#endif
    
protected:
    ofxLayerManager *manager;
    ofxSharedAppData* sharedAppData;
	string layerName; 
    bool active;    //means that the layer should be updating and drawing
    bool bSetup;
    bool bDead;
};

#endif



/* Template to copy
 
 #ifndef OFX_
 #define OFX_
 
 #include "ofxLayer.h"
 
 class className : public ofxLayer
 {
 public:
	 //--------------------------------------------------------------
	 className()
	 {
		layerName = "className";
	 }
	 //--------------------------------------------------------------
	 ~className() 
	 {
	 
	 }
	 //--------------------------------------------------------------
	 void setup() 
	 {
	 
	 }
	 //--------------------------------------------------------------
	 void update() 
	 {
	 
	 }
	 //--------------------------------------------------------------
	 void draw() 
	 {
	 
	 }
	 //--------------------------------------------------------------
	 void exit() 
	 {
	 
	 }
	 //--------------------------------------------------------------
	 void activate() 
	 {
		active = true; 
	 }                              
	 
	 void deactivate() 
	 {
		active = false;     
	 } 	
	 //--------------------------------------------------------------
	 void touchDown(ofTouchEventArgs& touch) 
	 {
	 
	 }
	 //-------------------------------------------------------------- 
	 void touchMoved(ofTouchEventArgs& touch) 
	 {
	 
	 }
	 //-------------------------------------------------------------- 
	 void touchUp(ofTouchEventArgs& touch) 
	 {
	 
	 }
	 //-------------------------------------------------------------- 
	 void touchDoubleTap(ofTouchEventArgs& touch) 
	 {
	 
	 }
	 //--------------------------------------------------------------
	 void touchCancelled(ofTouchEventArgs& touch) 
	 {
	 
	 }
	 //--------------------------------------------------------------
	 void mouseReleased(ofMouseEventArgs& data) 
	 {
	 
	 }
	 //--------------------------------------------------------------
	 void mousePressed(ofMouseEventArgs& data) 
	 {
	 
	 }
	 //--------------------------------------------------------------
	 void mouseMoved(ofMouseEventArgs& data) 
	 {
	 
	 }
	 //--------------------------------------------------------------
	 void mouseDragged(ofMouseEventArgs& data) 
	 {
	 
	 }
	 //--------------------------------------------------------------
	 void keyPressed(int key) 
	 {
	 
	 }
	 //--------------------------------------------------------------
	 void keyReleased(int key) 
	 {
	 
	 }
	 //--------------------------------------------------------------
	 void windowResized(int w, int h) 
	 {
	 
	 }
	 //--------------------------------------------------------------
 
 protected:		//	ofxSharedAppData* sharedAppData;	string layerName; 	bool active;    Reference

 };
 
 #endif
 
*/
