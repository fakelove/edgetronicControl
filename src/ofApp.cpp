#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    preview.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);
    tempPreview.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);
    httpQueue.registerTaskEvents(this);

}

//--------------------------------------------------------------
void ofApp::update(){
    string ip = CAMIP;
    string imgUri = ip+ "/static/images/displayed_image.jpg?"+ ofToString(ofGetFrameNum());
    string loadUri = ip + "/image2";
    httpQueue.get(loadUri);
    //uri+=  "?" + ofToString(ofGetFrameNum());
    imgLoader.loadFromURL(tempPreview, imgUri);
    
    
    if(tempPreview.getColor(tempPreview.getWidth()-1 , tempPreview.getHeight()-1) != ofColor(0,0,0)){
        preview.setFromPixels(tempPreview);
            }
    
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

void ofApp::onTaskData(const ofx::TaskDataEventArgs<ofx::HTTP::ClientResponseBufferEventArgs>& args)
{
    cout<<args.getTaskId().toString() <<endl;
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

