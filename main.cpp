#include "chinesepic.hpp"
#include "videotrack.hpp"
#include <string>
#include <iostream>
#include <locale>
#include <codecvt>
using namespace std;
int main() {
    /*cv::Mat im = cv::imread("/home/bird14/wp/c++/learnopencv/images.jpg");
    //cv::line(im,cv::Point(0,0),cv::Point(100,100),cv::Scalar(255,0,255),5);
    //cv::circle(im,cv::Point(50,50),30,cv::Scalar(0,255,0),-1);
    rectangle_cir(im,cv::Point(100,100),50,80,cv::Scalar(255,0,111),2);
    rectangle_cir(im,cv::Point(100,100),50,80,cv::Scalar(0,111,111));
    cv::namedWindow("win");
    imshow("win",im);
    cv::waitKey(0);*/
    cv::VideoCapture cap;
    cv::Mat          src;
    cap.open("input.avi");
    cv::Mat im ;
    do{
        cap>>src;
        if(!src.empty())break;
    }while(1);
    int width=360,heigth=360;
    int poix=980,poiy=360;
    cv::VideoWriter writer;
    writer.open("outtmp.avi",CV_FOURCC('M', 'J', 'P', 'G'),20,src.size(),1);
    std::vector<cv::Point> pois;
    std::vector<cv::Mat> vec;
    vec.push_back(src.clone());
    while(1){
        cap>>src;
        if(src.empty())break;
        vec.push_back(src.clone());
    }
    cout<<vec.size()<<endl;
    repeatsol(vec,20,130,putTextZH,"你好",cv::Point(200,200),cv::Scalar(0,111,111),18,"/usr/share/fonts/wps-office/FZHTK.TTF/18");
    repeatsol(vec,20,130,putTextZH,"ab",cv::Point(300,300),cv::Scalar(111,111,111),18,"/usr/share/fonts/wps-office/FZHTK.TTF/18");
    //repeatsol(vec,20,130,rectangle_cir,cv::Point(100,100),100,100,cv::Scalar(111,0,133),5,20);
    //repeatsol(vec,20,130,cv::rectangle,cv::Rect(20,20,200,200),cv::Scalar(255,111,0),-1);
    for(auto it:vec){
        writer.write(it);
    }
    /*
    cv::dnn::Net net = cv::dnn::readNetFromCaffe(
    "/home/bird14/Documents/models/models/VGGNet/VOC0712/SSD_300x300/deploy.prototxt",
    "/home/bird14/Documents/models/models/VGGNet/VOC0712/SSD_300x300/VGG_VOC0712_SSD_300x300_iter_120000.caffemodel");
    cv::Mat inputBlob = cv::dnn::blobFromImage(src, 1.0f, cv::Size(300, 300), cv::Scalar(104, 117, 123), false, false); //Convert Mat to batch of images
    net.setInput(inputBlob, "data");
    cv::Mat detection = net.forward("detection_out"); //compute output
    cv::Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());
    while(1){
        cap >> src;
        //cv::Mat inputBlob = cv::dnn::blobFromImage(src, 1.0f, cv::Size(300, 300), cv::Scalar(104, 117, 123), false, false); //Convert Mat to batch of images
        net.setInput(inputBlob, "data");
        cv::Mat detection = net.forward("detection_out");
        cv::Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());
        for(int i = 0; i < detectionMat.rows; i++)
        {
            float confidence = detectionMat.at<float>(i, 2);
            if(confidence > 0.5)
            {
                size_t objectClass = (size_t)(detectionMat.at<float>(i, 1));
                int xLeftBottom = static_cast<int>(detectionMat.at<float>(i, 3) * src.cols);
                int yLeftBottom = static_cast<int>(detectionMat.at<float>(i, 4) * src.rows);
                int xRightTop = static_cast<int>(detectionMat.at<float>(i, 5) * src.cols);
                int yRightTop = static_cast<int>(detectionMat.at<float>(i, 6) * src.rows);
                cv::Rect object(xLeftBottom, yLeftBottom,
                            xRightTop - xLeftBottom,
                            yRightTop - yLeftBottom);
                cout<<(xLeftBottom+xRightTop)/2<<' '<<(yLeftBottom+yRightTop)/2<<endl;
            }
        }
        if(src.empty()){
            break;
        }
        vec.push_back(src.clone());
    }/*
    std::vector<cv::Point> anss=getcenterBykcfssd(vec,width,heigth,poix,poiy,
    "/home/bird14/Documents/models/models/VGGNet/VOC0712/SSD_300x300/deploy.prototxt",
    "/home/bird14/Documents/models/models/VGGNet/VOC0712/SSD_300x300/VGG_VOC0712_SSD_300x300_iter_120000.caffemodel");
    std::vector<cv::Mat> ans=videoTrack(anss, vec, width, heigth,25,3);
    for(auto it:ans){
        writer.write(it);
    }*/
    //cap.release();
    //writer.release();
    return 0;
}
