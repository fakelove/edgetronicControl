#pragma once

#include "ofMain.h"
#include "ofxHTTP.h"
#include "ofxUI.h"
#include "ofxThreadedImageLoader.h"
#include "ofxXmlSettings.h"
#include "ofxJSONElement.h"
#include "ofxHttpUtils.h"

#define CAMIP "http://10.11.12.13"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void onTaskQueued(const ofx::TaskQueuedEventArgs& args);
        void onTaskStarted(const ofx::TaskStartedEventArgs& args);
        void onTaskCancelled(const ofx::TaskCancelledEventArgs& args);
        void onTaskFinished(const ofx::TaskFinishedEventArgs& args);
        void onTaskFailed(const ofx::TaskFailedEventArgs& args);
        void onTaskProgress(const ofx::TaskProgressEventArgs& args);
        void onTaskData(const ofx::TaskDataEventArgs<ofx::HTTP::ClientResponseBufferEventArgs>& args);
    
        void onGuiEvent(ofxUIEventArgs& e);
    
    private:
        ofxHTTP::DefaultClientTaskQueue httpQueue, imgRefresh;
    
        ofxThreadedImageLoader imgLoader;

        void trigger();
        void saveStop();
        void download();
        void configure();
        void getCamSettings();
    
        ofImage preview, tempPreview;
    
        void setupUI();
        ofxUISuperCanvas* ui;
        ofxUISuperCanvas* camInfo;
    
        Poco::UUID downloadID, statusID, infoID, configureID;

        ofxHttpUtils httpUtils;
    
        //camera options
        ofxJSONElement desiredSettings, actualSettings;
        float desiredFramerate, desiredWidth, desiredHeight, desiredShutter;
        float actualFramerate, actualWidth, actualHeight, actualShutter;
};
