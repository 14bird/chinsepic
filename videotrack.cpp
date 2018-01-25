#include <iostream>
#include <cstdlib>
#include <algorithm>
#include "videotrack.hpp"
std::vector<int> calrange(cv::Point center,int width,int heigth,int widthrange,int heigthrange){
    int tlx,tly,brx,bry;
    tlx=std::max(0,center.x-width/2);tly=std::max(0,center.y-heigth/2);
    brx=tlx+width,bry=tly+heigth;
    if(brx>=widthrange){
        int tmp=brx-widthrange+1;
        brx-=tmp;tlx-=tmp;
    }
    if(bry>=heigthrange){
        int tmp=bry-heigthrange+1;
        bry-=tmp;tly-=tmp;
    }
    return {tlx,tly,brx,bry};
}
std::vector<int> getrange(cv::Point center,std::vector<int> pre,int width,int heigth,
int rangeofxchange,int rangeofychange,int widthrange,int heigthrange){
    std::vector<int> ans=calrange(center,width,heigth,widthrange,heigthrange);
    if(abs(pre[0]-ans[0])>rangeofxchange){
        ans[0]=pre[0]>ans[0]?pre[0]-rangeofxchange:pre[0]+rangeofxchange;
        ans[2]=ans[0]+width;
    }
    if(abs(pre[1]-ans[1])>rangeofychange){
        ans[1]=pre[1]>ans[1]?pre[1]-rangeofychange:pre[1]+rangeofychange;
        ans[3]=ans[1]+heigth;
    }
    return ans;
}
std::vector<cv::Mat> videoTrack(std::vector<cv::Point> center,std::vector<cv::Mat> Videos,
int width,int heigth,int rangeofxchange,int rangeofychange){
    if(!center.size()){
        std::cout<<"The center of track is empty!"<<std::endl;
        exit(0);
    }
    if(!Videos.size()){
        std::cout<<"The video is empty!"<<std::endl;
        exit(0);
    }
    if(width>Videos[0].cols||heigth>Videos[0].rows){
        std::cout<<"the size of track is over the size of video!"<<std::endl;
        std::cout<<width<<' '<<Videos[0].cols<<' '<<heigth<<' '<<Videos[0].rows<<std::endl;
        exit(0);
    }
    //cv::namedWindow("win");
    std::vector<int> now=calrange(center[0],width,heigth,Videos[0].cols,Videos[0].rows);
    std::vector<cv::Mat> ans;
    for(int i=0;i<std::min(center.size(),Videos.size());i++){
        if(Videos[i].cols==0||Videos[i].rows==0)break;
        now=getrange(center[i],now,width,heigth,rangeofxchange,rangeofychange,Videos[i].cols,Videos[i].rows);
        //for(auto it:now)std::cout<<it<<' ';std::cout<<std::endl;
        //imshow("win",Videos[i]);
        //cv::waitKey(40);
        ans.push_back(Videos[i](cv::Rect(now[0],now[1],width,heigth)));
    }
    //cv::waitKey(0);
    return ans;
}
const char* classNames[] = {"background",
                            "aeroplane", "bicycle", "bird", "boat",
                            "bottle", "bus", "car", "cat", "chair",
                            "cow", "diningtable", "dog", "horse",
                            "motorbike", "person", "pottedplant",
                            "sheep", "sofa", "train", "tvmonitor"};
std::vector<cv::Point> getcenterBykcfssd(std::vector<cv::Mat> vi,int width,
int heigth,int px,int py,std::string condir,std::string bindir){
    cv::String modelConfiguration = condir;
    cv::String modelBinary = bindir;
    //! [Initialize network]
    cv::dnn::Net net = cv::dnn::readNetFromCaffe(modelConfiguration, modelBinary);
    //! [Initialize network]
    if (net.empty())
    {
        std::cerr << "Can't load network by using the following files: " << std::endl;
        std::cerr << "prototxt:   " << modelConfiguration << std::endl;
        std::cerr << "caffemodel: " << modelBinary << std::endl;
        std::cerr << "Models can be downloaded here:" << std::endl;
        std::cerr << "https://github.com/weiliu89/caffe/tree/ssd#models" << std::endl;
        exit(-1);
    }
    cv::Rect2d roi;
    cv::Ptr<cv::Tracker> tracker = cv::TrackerKCF::create();
    std::vector<cv::Point> ans;
    std::vector<int> inipos=calrange(cv::Point(px,py),width/4,heigth/4,vi[0].cols,vi[0].rows);
    roi=cv::Rect2d(inipos[0],inipos[1],inipos[2]-inipos[0],inipos[3]-inipos[1]);
    tracker->init(vi[0], roi);
    //std::cout<<roi.x<<' '<<roi.y<<' '<<roi.width<<' '<<' '<<vi[0].cols<<' '<<vi[0].rows<<std::endl;
    //std::cout<<inipos[0]<<' '<<inipos[1]<<' '<<inipos[2]-inipos[0]<<' '<<inipos[3]-inipos[1]<<std::endl;
    //cv::namedWindow("win");
    int ttt=clock();
    for (int i=0;i<vi.size();i++)
    {
        if (vi[i].empty())
        {
            break;
        }
        if (vi[i].channels() == 4)
            cvtColor(vi[i], vi[i], cv::COLOR_BGRA2BGR);
        //! [Prepare blob]
        std::vector<int> tmp=calrange(cv::Point(px,py),width,heigth,vi[i].cols,vi[i].rows);
        //for(auto it:tmp)std::cout<<it<<' ';std::cout<<std::endl;
        cv::Mat frame=vi[i](cv::Rect(tmp[0],tmp[1],tmp[2]-tmp[0],tmp[3]-tmp[1]));
        cv::Mat inputBlob = cv::dnn::blobFromImage(frame, 1.0f, cv::Size(300, 300), cv::Scalar(104, 117, 123), false, false); //Convert Mat to batch of images
        //! [Prepare blob]
        //! [Set input blob]
        net.setInput(inputBlob, "data"); //set the network input
        //! [Set input blob]
        //! [Make forward pass]
        cv::Mat detection = net.forward("detection_out"); //compute output
        //! [Make forward pass]
        //std::vector<double> layersTimings;
        //double freq = cv::getTickFrequency() / 1000;
        //double time = net.getPerfProfile(layersTimings) / freq;
        cv::Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());
        float confidenceThreshold = 0.5;
        std::vector<cv::Point> vec;
        for(int i = 0; i < detectionMat.rows; i++)
        {
            float confidence = detectionMat.at<float>(i, 2);
            if(confidence > confidenceThreshold)
            {
                size_t objectClass = (size_t)(detectionMat.at<float>(i, 1));
                int xLeftBottom = static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols);
                int yLeftBottom = static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows);
                int xRightTop = static_cast<int>(detectionMat.at<float>(i, 5) * frame.cols);
                int yRightTop = static_cast<int>(detectionMat.at<float>(i, 6) * frame.rows);
                cv::Rect object(xLeftBottom, yLeftBottom,
                            xRightTop - xLeftBottom,
                            yRightTop - yLeftBottom);
                cv::String label = cv::String(classNames[objectClass]) ;
                if(label!=cv::String("person")){
                    continue;
                }
                //cv::rectangle(frame, object, cv::Scalar(0, 255, 0));
                vec.push_back(cv::Point(tmp[0]+xLeftBottom+(xRightTop - xLeftBottom)/2,tmp[1]+yLeftBottom+(yRightTop - yLeftBottom)/2));
            }
        }
        tracker->update(vi[i], roi);
        if(vec.empty()&&roi.width){
            px=roi.x+roi.width/2;
            py=roi.y+roi.height/2;
        }
        else{
            int a=vec[0].x,b=vec[0].y;
            for(int j=1;j<vec.size();j++){
                if(pow(a-px,2)+pow(b-py,2)<pow(vec[j].x-px,2)+pow(vec[j].y-py,2)){
                    a=vec[j].x,b=vec[j].y;
                }
            }
            if(!roi.width){
                px=a,py=b;
            }
            else{
                px=a,py=b;
            }
        }
        //cv::rectangle(vi[i],roi,cv::Scalar(255,0,255));
        //std::cout<<px<<' '<<py<<std::endl;
        /*imshow("win",vi[i]);
        cv::waitKey(0);*/
        ans.push_back(cv::Point(px,py));
    }
    std::cout<<((double)(clock()-ttt))/CLOCKS_PER_SEC<<std::endl;
    return ans;
}
