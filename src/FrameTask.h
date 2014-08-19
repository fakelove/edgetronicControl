//
//  FrameTask.h
//  edgertronicControl
//
//  Created by Dan Moore on 8/18/14.
//
//

#pragma once
#include "ofMain.h"
#include "ofxHTTP.h"
#include "ofxIO.h"
#include "Synchronized.h"

class Frame
{
public:
    enum State
    {
        PENDING,
        SUCCESS,
        FAILURE
    };
    
    Frame():
    progress(0),
    state(PENDING)
    {
    }
    
    std::string name;
    Poco::UUID uuid;
    float progress;
    std::string message;
    State state;
};

class FrameTask{
public:
    FrameTask();
    ~FrameTask();
    void setup();
    void update(ofEventArgs& args);
    void exit();
    bool isFrameNew();
    ofTexture* getCurrentFrame();
    void onTaskQueued(const ofx::TaskQueuedEventArgs& args);
    void onTaskStarted(const ofx::TaskStartedEventArgs& args);
    void onTaskCancelled(const ofx::TaskCancelledEventArgs& args);
    void onTaskFinished(const ofx::TaskFinishedEventArgs& args);
    void onTaskFailed(const ofx::TaskFailedEventArgs& args);
    void onTaskProgress(const ofx::TaskProgressEventArgs& args);
    void onTaskData(const ofx::TaskDataEventArgs<ofx::HTTP::ClientResponseBufferEventArgs>& args);
    typedef std::map<Poco::UUID, Frame> FrameMap;
private:
    FrameMap frames;
    ofxHTTP::DefaultClientTaskQueue imgRefresh;
    Synchronized<ofImage> imgBuffer;
    ofImage preview;
    string previewURL;
    bool newFrame;
};