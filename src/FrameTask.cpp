//
//  FrameTask.cpp
//  edgertronicControl
//
//  Created by Dan Moore on 8/18/14.
//
//

#include "FrameTask.h"
FrameTask::FrameTask(){
    
}

FrameTask::~FrameTask(){
    
}

void FrameTask::setup(){
    imgBuffer.setup(ofImage());
    imgRefresh.registerAllEvents(this);
    ofAddListener(ofEvents().update, this, &FrameTask::update);
    previewURL = "http://10.11.12.13/static/images/image.jpg";
    Poco::UUID uuid = imgRefresh.get(previewURL);

}

void FrameTask::update(ofEventArgs& args){
    newFrame =imgBuffer.swapFront();
    if(newFrame){
        preview.setFromPixels(imgBuffer.getFront().getPixelsRef());
    }
    FrameMap::iterator iter = frames.begin();
    
    while (iter != frames.end())
    {
        Frame& t = iter->second;
        
        if (t.state == Frame::SUCCESS)
        {
            frames.erase(iter++);
        }
        else
        {
            ++iter;
        }
    }
}

bool FrameTask::isFrameNew(){
    return newFrame;
}

ofTexture* FrameTask::getCurrentFrame(){
    return &preview.getTextureReference();
}

void FrameTask::exit(){
    ofRemoveListener(ofEvents().update, this, &FrameTask::update);
}

void FrameTask::onTaskQueued(const ofx::TaskQueueEventArgs& args)
{
    // Make a record of the task so we can keep track of its progress.
    Frame newFrame;
    newFrame.name = args.getTaskName();
    newFrame.uuid = args.getTaskId();
    frames[newFrame.uuid] = newFrame;
}


void FrameTask::onTaskStarted(const ofx::TaskQueueEventArgs& args)
{
}


void FrameTask::onTaskCancelled(const ofx::TaskQueueEventArgs& args)
{
    
}


void FrameTask::onTaskFinished(const ofx::TaskQueueEventArgs& args)
{
    if (frames[args.getTaskId()].state == Frame::PENDING)
    {
        frames[args.getTaskId()].progress = 1;
        frames[args.getTaskId()].state = Frame::SUCCESS;
        Poco::UUID uuid = imgRefresh.get(previewURL);
    }
}


void FrameTask::onTaskFailed(const ofx::TaskFailedEventArgs& args)
{
    
}


void FrameTask::onTaskProgress(const ofx::TaskProgressEventArgs& args)
{
    
}


void FrameTask::onClientBuffer(const ofx::HTTP::ClientBufferEventArgs& args)
{
    const ofx::IO::ByteBuffer& buffer = args.getData().getByteBuffer();
    try
    {
        ofBuffer buff;
        buff.set(buffer.getCharPtr(), buffer.size());
        ofImage &img = imgBuffer.getBack();
        img.loadImage(buff);
        imgBuffer.swapBack();
    }
    catch (const Poco::Exception& exc)
    {
        ofLogError("FrameTask::onTaskData") << exc.displayText();
    }
}
