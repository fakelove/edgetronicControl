#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxXmlSettings.h"
#include "FrameTask.h"
#include "StatusTask.h"
#include "DownloaderTask.h"


#define CAMIP "http://10.11.12.13"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void newState(int & i);
    void newFlag(int & i);
    void newLevel(int & i);
    
    void downloadStart(float & i);
    void downloadFinish(string & file);
    void downloadProgress(float & i);
    

    
    void onGuiEvent(ofxUIEventArgs& e);
    
private:


    FrameTask frameTask;
    StatusTask cameraStatus;
    DownloaderTask captureDownloader;
    
    ofVideoPlayer videoPreview;
    void setupUI();
    ofxUISuperCanvas* ui;
    ofxUITextArea* uiStatus;

    deque<string> triggerQue;
    deque<string> saveQue;
    string camState, pCamState;
    float    saveProgress;
    float    loadProgress;
    
    float desiredFramerate, desiredWidth, desiredHeight;
    float desiredISO, desiredShutter, desiredDuration;
    float preTrigger;
    
    bool capture, configure, download;
    bool loaded;
};
