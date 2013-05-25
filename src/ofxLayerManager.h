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

#ifndef OFXLAYERMANAGER
#define OFXLAYERMANAGER

#include "ofxLayer.h"
#include <map>

class ofxLayerManager
{
public: 
    
    typedef map<string, ofxLayer*>::iterator layerIt;    
    
	ofxLayerManager()
    {
        enableAppEventCallbacks();
#ifdef TARGET_OPENGLES
        enableTouchCallbacks();
#else
        enableMouseEventCallbacks();
        enableKeyEventCallbacks();
        enableWindowEventCallbacks(); 
#endif 
    }
	
	~ofxLayerManager()
	{
        //exit function handles the dynamic memory...gets called by openframeworks before quiting
	}
    
    void enable()
    {
        enableAppEventCallbacks();        
    }
    
    void disable()
    {
        disableAppEventCallbacks();
    }

    
    //App Data 
    void setSharedAppData(ofxSharedAppData* sharedAppData) { this->sharedAppData = sharedAppData; }
    
    //Layer Adding & Setup

    void addLayer(ofxLayer* newlayer)
    {
        newlayer->setManager(this); 
        newlayer->setSharedAppData(sharedAppData);
//        newlayer->setup();
        layers[newlayer->getLayerName()] = newlayer;
        ofAddListener(newlayer->switchLayerEvent, this, &ofxLayerManager::onSwitchLayer);
        ofAddListener(newlayer->activateLayerEvent, this, &ofxLayerManager::onActivateLayer);
        ofAddListener(newlayer->deactivateLayerEvent, this, &ofxLayerManager::onDeactivateLayer);
        ofAddListener(newlayer->deleteLayerEvent, this, &ofxLayerManager::onDeleteLayer);        
    }
    
    void onActivateLayer(ofxLayerEventArgs &args)
    {
        layerIt it = layers.find(args.layerName);
        if (it != layers.end())
        {
        	ofxLayer *l = (ofxLayer *) it->second;
            if(!l->isSetup())
            {
                l->setup();
                l->setSetup(true);
            }
        	l->activate();
        }
    }

    void onDeleteLayer(ofxLayerEventArgs &args)
    {
        layerIt it = layers.find(args.layerName);
        if (it != layers.end())
        {
            ofxLayer *l = (ofxLayer *) it->second;
            deleteLayer(l);
        }
    }
    
    void onDeactivateLayer(ofxLayerEventArgs &args)
    {
        layerIt it = layers.find(args.layerName);
        if (it != layers.end())
        {
            ofxLayer *l = (ofxLayer *) it->second; 
            l->deactivate(); 
        }        
    }

    void onSwitchLayer(ofxLayerEventArgs &args)
    {        
        layerIt it = layers.find(args.layerName);
        if (it != layers.end()) 
        {            
            args.sender->deactivate(); 
            ofxLayer *l = (ofxLayer *) it->second;
            if(!l->isSetup())
            {
                l->setup();
                l->setSetup(true);
            }            
            l->activate();
        }          
    }
    
    void deleteLayer(ofxLayer *_layer)
    {
        _layer->setDead(true);
    }
    
	void activateLayer(ofxLayer *_layer)
	{
        layerIt it = layers.find(_layer->getLayerName());
        if (it != layers.end())
        {
            ofxLayer *l = (ofxLayer *) it->second;
            if(!l->isSetup())
            {
                l->setup();
                l->setSetup(true);
            }            
            l->activate(); 
        }		
	}
	
	void switchLayer(ofxLayer *_layer)
	{
		layerIt it = layers.find(_layer->getLayerName());        
		if (it != layers.end())
        {
            for (layer= layers.begin(); layer != layers.end(); ++layer)
            {
                ofxLayer *last = (ofxLayer *) layer->second;                
                if(last->isSetup())
                {
                    last->deactivate(); 
                }
            }
            ofxLayer *l = (ofxLayer *) it->second;
            if(!l->isSetup())
            {
                l->setup();
                l->setSetup(true);
            }            
            l->activate();             
        }
    }
    
	void switchLayer(string name)
	{
		layerIt it = layers.find(name);        
		if (it != layers.end())
        {
            for (layer= layers.begin(); layer != layers.end(); ++layer)
            {
                ofxLayer *last = (ofxLayer *) layer->second;                
                if(last->isSetup())
                {
                    last->deactivate();
                }
            }
            ofxLayer *l = (ofxLayer *) it->second;
            if(!l->isSetup())
            {
                l->setup();
                l->setSetup(true);
            }            
            l->activate();             
        }
    }
    
	ofxLayer *getActiveLayer()
    {
        for (layer= layers.begin(); layer != layers.end(); ++layer)
        {
            ofxLayer *l = (ofxLayer *) layer->second;
            if(l->isActive())
            {
                return l;
            }
        }
        return NULL;
    }
    
    ofxSharedAppData* getSharedAppData()
    {
        return sharedAppData;
    }
    
    map<string, ofxLayer*> getLayers() const
    {
        return layers;
    }
    
    vector<string> getLayerNames()
    {
        vector<string> layerNames; 
        for (layer= layers.begin(); layer != layers.end(); ++layer)
        {
            ofxLayer *last = (ofxLayer *) layer->second;   
            layerNames.push_back(last->getLayerName());
        }
        return layerNames; 
    }
    
    bool containsLayer(string layerName)
    {
        layerIt it = layers.find(layerName);
		if (it == layers.end())
        {
            return false;
        }
        return true;
    }
    
    //App Callbacks
    void enableAppEventCallbacks()
    {
        ofAddListener(ofEvents().update, this, &ofxLayerManager::onUpdate);
        ofAddListener(ofEvents().draw, this, &ofxLayerManager::onDraw);
        ofAddListener(ofEvents().exit, this, &ofxLayerManager::onExit);
    }

    void disableAppEventCallbacks()
    {
        ofRemoveListener(ofEvents().update, this, &ofxLayerManager::onUpdate);
        ofRemoveListener(ofEvents().draw, this, &ofxLayerManager::onDraw);
        ofRemoveListener(ofEvents().exit, this, &ofxLayerManager::onExit);
    }

    void onUpdate(ofEventArgs &data) 
    {
        update();
    }
    void onDraw(ofEventArgs &data) 
    { 
        draw();
    } 
    void onExit(ofEventArgs &data) 
    { 
        exit();
    } 
    
    void update()
    {
        for (layer= layers.begin(); layer != layers.end(); ++layer )
        {
            if(layer->second->isDead())
            {
                ofxLayer *l = layer->second;

                if(l->isSetup())
                {
                    if(l->isActive())
                    {
                        l->deactivate();
                    }
                    l->exit();
                }
                delete l;
                layers.erase(layer);                
            }
            else if(layer->second->isActive())
            {
                layer->second->update();
            }
        }
    }
    
    void draw()
    {
        for (layer= layers.begin(); layer != layers.end(); ++layer )
        {
            if(layer->second->isActive()) { layer->second->draw(); }
        }
    }
    
    void exit()
    {
        cout << "Exiting LayerManager" << endl;

        disable();
        for (layer = layers.begin(); layer != layers.end(); ++layer )
        {
            ofxLayer *l = layer->second;
            if(l->isSetup())
            {
                l->exit();
            }
            delete l;
        }
		layers.clear();
    }
    
#ifdef TARGET_OPENGLES
    //Touch Callbacks
    void enableTouchCallbacks()
    {
        ofAddListener(ofEvents().touchUp, this, &ofxLayerManager::onTouchUp);
        ofAddListener(ofEvents().touchDown, this, &ofxLayerManager::onTouchDown);
        ofAddListener(ofEvents().touchMoved, this, &ofxLayerManager::onTouchMoved);
        ofAddListener(ofEvents().touchCancelled, this, &ofxLayerManager::onTouchCancelled);
        ofAddListener(ofEvents().touchDoubleTap, this, &ofxLayerManager::onTouchDoubleTap);
    }
    
    void onTouchUp(ofTouchEventArgs &data) 
    {
        for (layer= layers.begin(); layer != layers.end(); ++layer )
            if(layer->second->isActive()) { layer->second->touchUp(data); }        
    }
    
    void onTouchDown(ofTouchEventArgs &data)
    {
        for (layer= layers.begin(); layer != layers.end(); ++layer )
            if(layer->second->isActive()) { layer->second->touchDown(data); }
    }
    
    void onTouchMoved(ofTouchEventArgs &data) 
    {
        for (layer= layers.begin(); layer != layers.end(); ++layer )
            if(layer->second->isActive()) { layer->second->touchMoved(data); }        
    }
    void onTouchCancelled(ofTouchEventArgs &data)
    {
        for (layer= layers.begin(); layer != layers.end(); ++layer )
            if(layer->second->isActive()) { layer->second->touchCancelled(data); }
    }
    void onTouchDoubleTap(ofTouchEventArgs &data)
    {
        for (layer= layers.begin(); layer != layers.end(); ++layer )
            if(layer->second->isActive()) { layer->second->touchDoubleTap(data); }
    }
#else
    //Keyboard Callbacks
    void enableKeyEventCallbacks()
    {
        ofAddListener(ofEvents().keyPressed, this, &ofxLayerManager::onKeyPressed);
        ofAddListener(ofEvents().keyReleased, this, &ofxLayerManager::onKeyReleased);
    }
    
    void onKeyPressed(ofKeyEventArgs& data)
    {
        for (layer= layers.begin(); layer != layers.end(); ++layer )
            if(layer->second->isActive()) { layer->second->keyPressed(data.key); }
        
    }
    
    void onKeyReleased(ofKeyEventArgs& data)
    {
        for (layer= layers.begin(); layer != layers.end(); ++layer )
            if(layer->second->isActive()) { layer->second->keyReleased(data.key); }
    }    
    //Mouse Callbacks
    void enableMouseEventCallbacks()
    {
        ofAddListener(ofEvents().mouseReleased, this, &ofxLayerManager::onMouseReleased);
        ofAddListener(ofEvents().mousePressed, this, &ofxLayerManager::onMousePressed);
        ofAddListener(ofEvents().mouseMoved, this, &ofxLayerManager::onMouseMoved);
        ofAddListener(ofEvents().mouseDragged, this, &ofxLayerManager::onMouseDragged);
    }
    
    void onMouseReleased(ofMouseEventArgs& data) 
    { 
        for (layer= layers.begin(); layer != layers.end(); ++layer )
            if(layer->second->isActive()) { layer->second->mouseReleased(data); }         
    }
    
    void onMousePressed(ofMouseEventArgs& data) 
    { 
        for (layer= layers.begin(); layer != layers.end(); ++layer )
            if(layer->second->isActive()) { layer->second->mousePressed(data); }         
    }
    
    void onMouseMoved(ofMouseEventArgs& data) 
    { 
        for (layer= layers.begin(); layer != layers.end(); ++layer )
            if(layer->second->isActive()) { layer->second->mouseMoved(data); }         
    }
    
    void onMouseDragged(ofMouseEventArgs& data) 
    { 
        for (layer= layers.begin(); layer != layers.end(); ++layer )
            if(layer->second->isActive()) { layer->second->mouseDragged(data); }             
    }
    
    //Window Resize Callback
    void enableWindowEventCallbacks()
    {
        ofAddListener(ofEvents().windowResized, this, &ofxLayerManager::onWindowResized);        
    }
    
    void onWindowResized(ofResizeEventArgs& data) 
    { 
        for (layer= layers.begin(); layer != layers.end(); ++layer )
        {
            if(layer->second->isSetup())
            {   
                layer->second->windowResized(data.width, data.height); 
            }
        }
    }       
#endif      
    
#ifdef TARGET_ANDROID
    
    void urlResponse(ofHttpResponse & response)
    {
    	for (layer= layers.begin(); layer != layers.end(); ++layer )
			if(layer->second->isActive()) { layer->second->urlResponse(response); }
    }

    void imageSelected(string imageURL)
    {
    	for (layer= layers.begin(); layer != layers.end(); ++layer )
    		if(layer->second->isActive()) { layer->second->imageSelected(imageURL); }
    }

    void gotFile(string url, string filename)
	{
		for (layer= layers.begin(); layer != layers.end(); ++layer )
			if(layer->second->isActive()) { layer->second->gotFile(url, filename); }
	}

    void savePressed(string title, string tags)
	{
		for (layer= layers.begin(); layer != layers.end(); ++layer )
			if(layer->second->isActive()) { layer->second->savePressed(title, tags); }
	}

    void pause()
    {
        for (layer= layers.begin(); layer != layers.end(); ++layer)
            layer->second->pause();         
    }
    
	void stop()
    {
        for (layer= layers.begin(); layer != layers.end(); ++layer)
            layer->second->stop();         
    }
    
    void resume()
    {
        for (layer= layers.begin(); layer != layers.end(); ++layer)
            layer->second->resume();         
    }
    
    void reloadTextures()
    {
        for (layer= layers.begin(); layer != layers.end(); ++layer)
            layer->second->reloadTextures();             
    }
    
    void onKeyDown(int keyCode)
    {
        for (layer= layers.begin(); layer != layers.end(); ++layer )
            if(layer->second->isActive()) { layer->second->onKeyDown(keyCode); }             
    }    
    
	void onKeyUp(int keyCode)
    {
        for (layer= layers.begin(); layer != layers.end(); ++layer )
            if(layer->second->isActive()) { layer->second->onKeyUp(keyCode); }             
    }
    
	bool backPressed()
    { 
        for (layer= layers.begin(); layer != layers.end(); ++layer )
            if(layer->second->isActive()) { return layer->second->backPressed(); }             
    }

	void menuPressed()
    {
        for (layer= layers.begin(); layer != layers.end(); ++layer )
            if(layer->second->isActive()) { layer->second->menuPressed(); }             
    }
    
	bool menuItemSelected(string menu_id_str)
    {
        for (layer= layers.begin(); layer != layers.end(); ++layer )
            if(layer->second->isActive()) { return layer->second->menuItemSelected(menu_id_str); }             
    }    
	
    bool menuItemChecked(string menu_id_str, bool checked)
    { 
        for (layer= layers.begin(); layer != layers.end(); ++layer )
            if(layer->second->isActive()) { return layer->second->menuItemChecked(menu_id_str, checked); }             
    }
    
	void okPressed()
    {
        for (layer= layers.begin(); layer != layers.end(); ++layer )
            if(layer->second->isActive()) { return layer->second->okPressed(); }                 
    }
	
    void cancelPressed()
    {
        for (layer= layers.begin(); layer != layers.end(); ++layer )
            if(layer->second->isActive()) { return layer->second->cancelPressed(); }                 
    }        

#endif      
    
    
#ifdef TARGET_OF_IPHONE
    
    void lostFocus()
    {
        for (layer= layers.begin(); layer != layers.end(); ++layer )
        {
            if(layer->second->isSetup())
            {
                layer->second->lostFocus();
            }
        }    
    }
    void gotFocus()
    {
        for (layer= layers.begin(); layer != layers.end(); ++layer )
        {
            if(layer->second->isSetup())
            {
                layer->second->gotFocus();
            }
        }
    }
    
    void gotMemoryWarning()
    {
        for (layer= layers.begin(); layer != layers.end(); ++layer )
        {
            if(layer->second->isSetup())
            {
                layer->second->gotMemoryWarning();
            }
        }
    }
    
    void deviceOrientationChanged(int newOrientation)
    {
        for (layer= layers.begin(); layer != layers.end(); ++layer )
        {
            if(layer->second->isSetup())
            {
                layer->second->deviceOrientationChanged(newOrientation);
            }
        }
    }
    
#endif
    
private:
    layerIt layer; 
    map<string, ofxLayer*> layers;    
    ofxSharedAppData *sharedAppData; 
};

#endif 
