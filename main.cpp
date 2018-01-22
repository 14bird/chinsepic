#include "chinesepic.hpp"
#include "videotrack.hpp"
#include <string>
#include <iostream>
#include <locale>
#include <codecvt>
using namespace std;
int main() {
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
    writer.open("output.avi",CV_FOURCC('M', 'J', 'P', 'G'),20,{width,heigth},1);
    std::vector<cv::Point> pois;
    std::vector<cv::Mat> vec;
    vec.push_back(src);
    while(1){
        cap >> src;
        if(src.empty()){
            break;
        }
        vec.push_back(src.clone());
    }
    std::vector<cv::Point> anss=getcenterBykcfssd(vec,width,heigth,poix,poiy,
    "/home/bird14/Documents/models/VGGNet/VOC0712Plus/SSD_300x300_ft/deploy.prototxt",
    "/home/bird14/Documents/models/VGGNet/VOC0712Plus/SSD_300x300_ft/VGG_VOC0712Plus_SSD_300x300_ft_iter_160000.caffemodel");
    std::vector<cv::Mat> ans=videoTrack(anss, vec, width, heigth,25,3);
    for(auto it:ans){
        writer.write(it);
    }
    cap.release();
    writer.release();
    return 0;
}
