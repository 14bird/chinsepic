#ifndef __CHINESEPIC_HPP
#define __CHINESEPIC_HPP 1
#include<string>
#include<map>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
struct capturePostion {
	int startx, starty, width, heigth, tim;
	capturePostion() {
		startx = 10;//左上角坐标
		starty = 76;
		width = 40;//图片大小
		heigth = 16;
		tim = 1000;//浏览器启动时间
	}
	capturePostion(int a, int b) {
		startx = a;
		starty = b;
		width = 40;
		heigth = 16;
		tim = 1000;
	}
	capturePostion(int a) {
		startx = 10;
		starty = 76;
		width = 40;
		heigth = 16;
		tim = a;
	}
	capturePostion(int a, int b, int c, int d, int e) {
		startx = a;
		starty = b;
		width = c;
		heigth = d;
		tim = e;
	}
};
struct tag {
	std::string con, name, bc, fc;
	tag(std::string a, std::string b, std::string c, std::string d) {
		con = a; name = b; bc = c; fc = d;
	}
};
bool operator<(tag a, tag b);
cv::Mat makepic(tag tt, capturePostion cap);
cv::Mat drawpic(tag tt);
cv::Mat getpic(tag tt, std::map<tag, cv::Mat>& ma);
cv::Scalar stringtoScalar(std::string a);
#ifdef _WIN32
#include<Windows.h>
void GetStringSize(HDC hDC, const char* str, int* w, int* h);
void putTextZH(cv::Mat &dst, const char* str, cv::Point org, cv::Scalar color, int fontSize,
	const char *fn = "Arial", bool italic = false, bool underline = false);

#endif
#endif
