#include "ofApp.h"

void ofApp::exit(){
    frameTask.exit();
    cameraStatus.exit();
    captureDownloader.exit();
    ofRemoveListener(cameraStatus.newStateEvent, this, &ofApp::newState);
}

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    
    loaded = false;
    capture = false;
    download = false;
    configure = false;
    setupUI();
    
    frameTask.setup();
    cameraStatus.setup();
    captureDownloader.setup();
    ofAddListener(cameraStatus.newStateEvent, this, &ofApp::newState);
    ofAddListener(cameraStatus.newLevelEvent, this, &ofApp::newLevel);
    ofAddListener(cameraStatus.newFlagEvent, this, &ofApp::newFlag);
    ofAddListener(captureDownloader.downloadStarted, this, &ofApp::downloadStart);
    ofAddListener(captureDownloader.downloadProgress, this, &ofApp::downloadProgress);
    ofAddListener(captureDownloader.downloadFinished, this, &ofApp::downloadFinish);
}

//--------------------------------------------------------------
void ofApp::update(){
    if(loaded){
        videoPreview.update();
    }
    
    if(capture){
        capture = false;
        if(camState == "READY"){
            ofHttpResponse reponse = ofLoadURL("http://10.11.12.13/trigger");
            if(reponse.status < 300){
                ofLog(OF_LOG_NOTICE)<<"Capture Triggered"<<endl;
                ofLog(OF_LOG_VERBOSE)<<"trigger returend "<<reponse.status<<" status code"<<endl;
                saveQue.push_back(ofGetTimestampString());
            }else{
                ofLog(OF_LOG_ERROR)<<"trigger returend "<<reponse.status<<" status code"<<endl;
            }
        }else{
            triggerQue.push_back(ofGetTimestampString());
        }
    }
    
    if(camState == "READY" && camState != pCamState && saveProgress == 100){
        if(saveQue.size() > 0){
            saveQue.pop_front();
            download = true;
        }
    }
    
    
    if(configure){
        configure = false;
    }
    
    if(download){
        download = false;
        captureDownloader.triggerDownload();
    }
    
    
    if(camState == "READY"){
        if(triggerQue.size() > 0){
            ofHttpResponse reponse = ofLoadURL("http://10.11.12.13/trigger");
            if(reponse.status < 300){
                ofLog(OF_LOG_NOTICE)<<"Capture Triggered"<<endl;
                ofLog(OF_LOG_VERBOSE)<<"trigger returend "<<reponse.status<<" status code"<<endl;
            }else{
                ofLog(OF_LOG_ERROR)<<"trigger returend "<<reponse.status<<" status code"<<endl;
            }
            triggerQue.pop_front();
        }
    }
    
    pCamState = camState;
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0,0);
    frameTask.getCurrentFrame()->draw(0, 0, 640, 360);
    if(loaded){
        videoPreview.draw(640, 0, 640, 360);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

void ofApp::setupUI(){
    
    ui = new ofxUISuperCanvas("Edgertronic Control");
    ui->setPosition(10, 10);
    ui->addFPS();
    ui->addSpacer();
    ui->addSlider("Level", 0.0, 100.0, &saveProgress);
    ui->addSlider("Download Progress", 0.0, 1.0, &loadProgress);
    ui->addSpacer();
    ui->addToggle("Trigger", &capture);
    ui->addSpacer();
    uiStatus = ui->addTextArea("Camera Status: ", camState);
    ui->ofxUICanvas::autoSizeToFitWidgets();
    
    
    ofAddListener(ui->newGUIEvent,this,&ofApp::onGuiEvent);
    
}

void ofApp::onGuiEvent(ofxUIEventArgs& e){
    string name = e.getName();
    int kind = e.getKind();
}

void ofApp::newState(int & i){
    pCamState = camState;
    if(i == StatusTask::CAMAPI_STATE_CALIBRATING){
        ofLog(OF_LOG_NOTICE)<<"STATE_CALIBRATING"<<endl;
        camState = "CALIBRATING";
        
    }
    if(i == StatusTask::CAMAPI_STATE_RUNNING){
        ofLog(OF_LOG_NOTICE)<<"CAMAPI_STATE_RUNNING"<<endl;
        camState = "RUNNING";
    }
    if(i == StatusTask::CAMAPI_STATE_TRIGGERED){
        ofLog(OF_LOG_NOTICE)<<"CAMAPI_STATE_TRIGGERED"<<endl;
        camState = "TRIGGERED";
    }
    if(i == StatusTask::CAMAPI_STATE_SAVING){
        ofLog(OF_LOG_NOTICE)<<"CAMAPI_STATE_SAVING"<<endl;
        camState = "SAVING";
    }
    if(i == StatusTask::CAMAPI_STATE_RUNNING_PRETRIGGER_FULL){
        ofLog(OF_LOG_NOTICE)<<"CAMAPI_STATE_RUNNING_PRETRIGGER_FULL"<<endl;
        camState = "READY";
    }
    if(i == StatusTask::CAMAPI_STATE_TRIGGER_CANCELED){
        ofLog(OF_LOG_NOTICE)<<"CAMAPI_STATE_TRIGGER_CANCELED"<<endl;
    }
    if(i == StatusTask::CAMAPI_STATE_SAVE_CANCELED){
        ofLog(OF_LOG_NOTICE)<<"CAMAPI_STATE_SAVE_CANCELED"<<endl;
    }
    
    if(uiStatus != NULL){
        uiStatus->setTextString(camState);
    }
}
void ofApp::newFlag(int & i){
    
}
void ofApp::newLevel(int & i){
    saveProgress = i;
}

void ofApp::downloadStart(float & i){
    loadProgress = i;
    loaded = false;
    videoPreview.closeMovie();
}
void ofApp::downloadFinish(string & file){
    loaded = true;
    videoPreview.loadMovie(file);
    videoPreview.play();
}
void ofApp::downloadProgress(float & i){
    loadProgress = i;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
