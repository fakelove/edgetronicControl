//
//  DownloaderTask.h
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

class Capture
{
public:
    enum State
    {
        PENDING,
        SUCCESS,
        FAILURE
    };
    
    Capture():
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

class DownloaderTask{
public:
    DownloaderTask();
    ~DownloaderTask();
    void setup();
    void triggerDownload();
    void update(ofEventArgs& args);
    void exit();
    void onTaskQueued(const ofx::TaskQueuedEventArgs& args);
    void onTaskStarted(const ofx::TaskStartedEventArgs& args);
    void onTaskCancelled(const ofx::TaskCancelledEventArgs& args);
    void onTaskFinished(const ofx::TaskFinishedEventArgs& args);
    void onTaskFailed(const ofx::TaskFailedEventArgs& args);
    void onTaskProgress(const ofx::TaskProgressEventArgs& args);
    void onTaskData(const ofx::TaskDataEventArgs<ofx::HTTP::ClientResponseBufferEventArgs>& args);
    typedef std::map<Poco::UUID, Capture> CaptureMap;
    ofEvent<float> downloadProgress;
    ofEvent<float> downloadStarted;
	ofEvent<string> downloadFinished;
private:
    CaptureMap captures;
    ofxHTTP::DefaultClientTaskQueue captureQueue;
    Synchronized<ofBuffer> buffers;
    string downloadURL;
    string lastFileName;
    float progress;
};