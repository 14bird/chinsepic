#ifndef __VIDEOTRACK_HPP
#define __VIDEOTRACK_HPP 1
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/dnn/shape_utils.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/tracking/tracker.hpp>
#include <opencv2/tracking/tldDataset.hpp>
std::vector<cv::Mat> videoTrack(std::vector<cv::Point> center,std::vector<cv::Mat> Videos,
int width,int heigth,int rangeofxchange=3,int rangeofychange=3);
std::vector<cv::Point> getcenterBykcfssd(std::vector<cv::Mat> vi,int width,
int heigth,int px,int py,std::string condir,std::string bindir);
#endif
