//
//  DownloaderTask.cpp
//  edgertronicControl
//
//  Created by Dan Moore on 8/18/14.
//
//

#include "DownloaderTask.h"
DownloaderTask::DownloaderTask(){
    
}

DownloaderTask::~DownloaderTask(){
    
}

void DownloaderTask::setup(){
    buffers.setup(ofBuffer());
    captureQueue.registerAllEvents(this);
    ofAddListener(ofEvents().update, this, &DownloaderTask::update);
    downloadURL = "http://10.11.12.13";
    progress = 0;
    
}

void DownloaderTask::update(ofEventArgs& args){
    CaptureMap::iterator iter = captures.begin();
    
    while (iter != captures.end())
    {
        Capture& t = iter->second;
        
        if (t.state == Capture::SUCCESS)
        {
            captures.erase(iter++);
        }
        else
        {
            ++iter;
        }
    }
}

void DownloaderTask::triggerDownload(){
    ofLog(OF_LOG_NOTICE)<<"Download Triggered"<<endl;
    ofHttpResponse response = ofLoadURL("http://10.11.12.13/last_saved_filename");
    ofLog(OF_LOG_NOTICE)<<response.data<<endl;
    
    string newFile = ofToString(response.data);
    if(ofIsStringInString(newFile, "\"")){
        ofStringReplace(newFile, "\"", "");
    }
    if(newFile != lastFileName){
        lastFileName = newFile;
        ofLog(OF_LOG_NOTICE)<<"Download url "<<downloadURL+"/static"+lastFileName<<endl;
        progress = 0;
        Poco::UUID uuid = captureQueue.get(downloadURL+"/static"+lastFileName);
    }
    
}


void DownloaderTask::exit(){
    ofRemoveListener(ofEvents().update, this, &DownloaderTask::update);
}

void DownloaderTask::onTaskQueued(const ofx::TaskQueueEventArgs& args)
{
    // Make a record of the task so we can keep track of its progress.
    Capture newFrame;
    newFrame.name = args.getTaskName();
    newFrame.uuid = args.getTaskId();
    captures[newFrame.uuid] = newFrame;
}


void DownloaderTask::onTaskStarted(const ofx::TaskQueueEventArgs& args)
{
    ofNotifyEvent(downloadStarted, progress, this);
}


void DownloaderTask::onTaskCancelled(const ofx::TaskQueueEventArgs& args)
{
    
}


void DownloaderTask::onTaskFinished(const ofx::TaskQueueEventArgs& args)
{
    if (captures[args.getTaskId()].state == Capture::PENDING)
    {
        captures[args.getTaskId()].progress = 1;
        captures[args.getTaskId()].state = Capture::SUCCESS;
        
        if(buffers.swapFront()){
            string filename = ofGetTimestampString()+".mov";
            ofBuffer & capture = buffers.getFront();
            ofFile file(filename, ofFile::WriteOnly);
            file.writeFromBuffer(capture);
            file.close();
            ofNotifyEvent(downloadFinished, filename, this);
        }
    }
}


void DownloaderTask::onTaskFailed(const ofx::TaskFailedEventArgs& args)
{
    
}


void DownloaderTask::onTaskProgress(const ofx::TaskProgressEventArgs& args)
{
    progress = args.getProgress();
    ofNotifyEvent(downloadProgress, progress, this);
}


void DownloaderTask::onClientBuffer(const ofx::HTTP::ClientBufferEventArgs& args)
{
    const ofx::IO::ByteBuffer& buffer = args.getData().getByteBuffer();
    try
    {
        ofBuffer & buff = buffers.getBack();
        buff.set(buffer.getCharPtr(), buffer.size());
        buffers.swapBack();
    }
    catch (const Poco::Exception& exc)
    {
        ofLogError("DownloaderTask::onTaskData") << exc.displayText();
    }
}
