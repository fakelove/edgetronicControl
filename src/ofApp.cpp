#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    preview.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);
    tempPreview.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);
    httpQueue.registerTaskEvents(this);
    
    setupUI();
    
    getCamSettings();

}

//--------------------------------------------------------------
void ofApp::update(){
    string ip = CAMIP;
    string imgUri = ip+ "/static/images/displayed_image.jpg?"+ ofToString(ofGetFrameNum());
    string loadUri = ip + "/image2";
    if(ofGetFrameNum()%30==0){
        imgRefresh.get(loadUri);
    }
    //uri+=  "?" + ofToString(ofGetFrameNum());
    imgLoader.loadFromURL(tempPreview, imgUri);
    
    
    if(tempPreview.getColor(tempPreview.getWidth()-1 , tempPreview.getHeight()-1) != ofColor(0,0,0)){
        preview.setFromPixels(tempPreview);
            }
    
    
    ui->update();
    camInfo->update();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0,0);
    preview.draw(0, 0, ofGetWidth(), ofGetHeight());
}

void ofApp::trigger(){
    string uri = CAMIP;
    uri += "/trigger";

    httpQueue.get(uri);
}

void ofApp::saveStop(){
    string uri = CAMIP;
    uri += "/save_stop";

    httpQueue.get(uri);
    
}

void ofApp::download(){
    string uri = CAMIP;
    uri += "/download";
    
    downloadID = httpQueue.get(uri);
    cout<< "download requested! "<< downloadID.toString()<<endl;
    //httpQueue.
}

void ofApp::configure(){
    string uri = CAMIP;
    uri += "/configure_camera?run=1";
    
    Poco::Net::NameValueCollection nameVal;
    vector<string> members = desiredSettings.getMemberNames();
    for(int i=0; i<members.size(); i++){
        nameVal.set(members[i], desiredSettings.get(members[i],"0").toStyledString()  );
    }
    
//    ofxHTTP::PostRequest::FormParts f;
//    ofxHTTP::PostRequest *post = new ofxHTTP::PostRequest(uri,
//                            nameVal, f,
//                                                
//                            "HTTP/1.1",
//                            configureID);
//
//    
//    configureID = httpQueue.request(post);
    
    ofBuffer buf = desiredSettings.toStyledString();
    httpUtils.postData(uri, buf , "application/json; charset=utf-8");
    
}

void ofApp::getCamSettings(){
    string uri = CAMIP;
    uri += "/get_current_settings";
    infoID = httpQueue.get(uri);
    
    uri = CAMIP;
    uri += "/get_camstatus";
    statusID = httpQueue.get(uri);

    
}

void ofApp::onTaskData(const ofx::TaskDataEventArgs<ofx::HTTP::ClientResponseBufferEventArgs>& args)
{
    //cout<<args.getTaskId().toString() <<endl;
    // Note: Saving to disk could / should also be done in the task's thread.
    
    // This is useful if you want to load the bytes into a GL texture.
    const ofx::IO::ByteBuffer& buffer = args.getData().getByteBuffer();
    
    if(args.getTaskId() == downloadID){
        cout<<"Downloaded!"<<endl;
    
        std::string path = ofToDataPath(args.getTaskId().toString() + ".mov");
        
        try
        {
            ofx::IO::ByteBufferUtils::saveToFile(buffer, path);
        }
        catch (const Poco::Exception& exc)
        {
            ofLogError("ofApp::onTaskData") << exc.displayText();
        }
    } else if(args.getTaskId() == infoID){
        ofx::IO::ByteBuffer buffer = args.getData().getByteBuffer();
        actualSettings.parse(buffer.toString());
        
        Json::Value v = actualSettings.get("frame_rate",0) ;
        actualFramerate = ofToFloat(v.toStyledString());
        v = actualSettings.get("horizontal",0) ;
        actualWidth = ofToFloat(v.toStyledString());
        v = actualSettings.get("vertical",0) ;
        actualHeight = ofToFloat(v.toStyledString());
        
        camInfo->removeWidget("Info");
        camInfo->addTextArea("Info",
                             "Actual Framerate: "+ ofToString(actualFramerate)+
                             "\nActual Width: "+ ofToString(actualWidth)+
                             "\nActual Height: "+ ofToString(actualHeight)+
                             "\nActual Shutter: "+ ofToString(actualShutter), OFX_UI_FONT_SMALL);
        //camInfo->addTextArea("Details", actualSettings.toStyledString());
        camInfo->setDimensions(200, 400);
        
        desiredSettings = actualSettings;

        cout<< actualSettings.toStyledString() <<endl;
    } else if(args.getTaskId() == configureID){
        cout<<args.getData().getByteBuffer().toString()<<endl;
        getCamSettings();

    }
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    switch (key){
            case 't':
                trigger();
                break;
            case 's':
                saveStop();
                break;
            case 'd':
                download();
                break;
            default:
            break;
            
            
    }
}

void ofApp::setupUI(){
    
    ui = new ofxUISuperCanvas("edgertronic control");
    ui->setPosition(10, 10);
    ui->addSlider("Desired Framerate", 12, 14000, desiredFramerate);
    ui->addSlider("Desired Width", 320, 1920, desiredWidth);
    ui->addSlider("Desired Height", 240, 1080, desiredHeight);
    ui->addSpacer();
    ui->addButton("Start", false);
    ui->addButton("Save", false);
    ui->addButton("Configure", false);
    ui->addButton("Download", false);
    ui->addButton("Get Settings", false);

    ui->ofxUICanvas::autoSizeToFitWidgets();
    
    camInfo = new ofxUISuperCanvas("Camera Info");
    camInfo->setPosition(ofGetWidth()-300, 10);
    camInfo->setFontSize(OFX_UI_FONT_SMALL, 7);
    camInfo->addTextArea("Info",
                         "Actual Framerate: "+ ofToString(actualFramerate)+
                         "\nActual Width: "+ ofToString(actualWidth)+
                         "\nActual Height: "+ ofToString(actualHeight)+
                         "\nActual Shutter: "+ ofToString(actualShutter), OFX_UI_FONT_SMALL);
    camInfo->autoSizeToFitWidgets();
    
    ofAddListener(ui->newGUIEvent,this,&ofApp::onGuiEvent);
    
}

void ofApp::onGuiEvent(ofxUIEventArgs& e){
    string name = e.getName();
    int kind = e.getKind();
    
    cout<<name<<endl;
    if (name == "Start") {
        trigger();
    } else if(name == "Save"){
        saveStop();
    } else if(name=="Download"){
        download();
    } else if (name=="Configure"){
        configure();
    } else if (name=="Get Settings"){
        getCamSettings();
    }
    
    if(kind == OFX_UI_WIDGET_SLIDER_H){
        desiredSettings["requested_frame_rate"]= desiredFramerate;
    }
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


void ofApp::onTaskQueued(const ofx::TaskQueuedEventArgs& args)
{

}


void ofApp::onTaskStarted(const ofx::TaskStartedEventArgs& args)
{
}


void ofApp::onTaskCancelled(const ofx::TaskCancelledEventArgs& args)
{
}


void ofApp::onTaskFinished(const ofx::TaskFinishedEventArgs& args)
{
    cout<<"finished " << args.getTaskId().toString()<<endl;
}


void ofApp::onTaskFailed(const ofx::TaskFailedEventArgs& args)
{

}


void ofApp::onTaskProgress(const ofx::TaskProgressEventArgs& args)
{
    cout<<"progress "<< args.getProgress()<<endl;
}

