//
//  StatusTask.h
//  edgertronicControl
//
//  Created by Dan Moore on 8/18/14.
//
//

#pragma once
#include "ofMain.h"
#include "ofEvents.h"
#include "ofxHTTP.h"
#include "ofxIO.h"
#include "Synchronized.h"
#include "ofxJSON.h"

class Status
{
public:
    enum STATE
    {
        PENDING,
        SUCCESS,
        FAILURE
    };
    
    Status():
    progress(0),
    state(PENDING)
    {
    }
    
    std::string name;
    Poco::UUID uuid;
    float progress;
    std::string message;
    STATE state;
};

class StatusTask{
public:
    enum CAMERA_STATE{
        CAMAPI_STATE_UNCONFIGURED = 1,
        CAMAPI_STATE_CALIBRATING = 2,
        CAMAPI_STATE_RUNNING = 3,
        CAMAPI_STATE_TRIGGERED = 4,
        CAMAPI_STATE_SAVING  = 5,
        CAMAPI_STATE_RUNNING_PRETRIGGER_FULL = 6,
        CAMAPI_STATE_TRIGGER_CANCELED = 7,
        CAMAPI_STATE_SAVE_CANCELED = 8
    };
    enum CAMERA_STATUS{
        CAMAPI_STATUS_OKAY = 1,
        CAMAPI_STATUS_INVALID_STATE = 2,
        CAMAPI_STATUS_STORAGE_ERROR = 3,
        CAMAPI_STATUS_CODE_OUT_OF_DATE = 3
    };
    enum CAMERA_FLAGS{
        CAMAPI_FLAG_STORAGE_FULL = 0,
        CAMAPI_FLAG_STORAGE_MISSING = 1,
        CAMAPI_FLAG_USB_STORAGE_INSTALLED = 2,
        CAMAPI_FLAG_SD_CARD_STORAGE_INSTALLED = 3,
        CAMAPI_FLAG_USB_STORAGE_FULL = 4,
        CAMAPI_FLAG_SD_CARD_STORAGE_FULL = 5,
        CAMAPI_FLAG_STORAGE_BAD = 6,
        CAMAPI_FLAG_FPGA_OUT_OF_DATE = 16,
        CAMAPI_FLAG_SUPPORTED_WIFI_ATTACHED = 17,
    };
    StatusTask();
    ~StatusTask();
    void setup();
    void update(ofEventArgs& args);
    void exit();
    void onTaskQueued(const ofx::TaskQueueEventArgs& args);
    void onTaskStarted(const ofx::TaskQueueEventArgs& args);
    void onTaskCancelled(const ofx::TaskQueueEventArgs& args);
    void onTaskFinished(const ofx::TaskQueueEventArgs& args);
    void onTaskFailed(const ofx::TaskFailedEventArgs& args);
    void onTaskProgress(const ofx::TaskProgressEventArgs& args);
    void onClientBuffer(const ofx::HTTP::ClientBufferEventArgs& args);
    typedef std::map<Poco::UUID, Status> StatusMap;
    
    ofEvent<int> newStateEvent;
    ofEvent<int> newFlagEvent;
	ofEvent<int> newLevelEvent;
    
private:
    StatusMap statusMap;
    ofx::HTTP::DefaultClientTaskQueue statusRefresh;
    string statusURL;
    Synchronized<ofxJSONElement> jsonData;
    int state, flags, level;
    int pState, pFlags, pLevel;
};