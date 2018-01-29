#ifndef __CHINESEPIC_HPP
#define __CHINESEPIC_HPP 1
#include <string>
#include <map>
#include <algorithm>
#include <cstdarg>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#define IMG_SIZ 20 //control the size of picture
struct capturePostion {
	int startx, starty, width, heigth, tim;
	capturePostion() {
		startx = 10;//the pos of top-left point
		starty = 76;
		width = 40;//the size of picture
		heigth = 16;
		tim = 1000;//the time of browser starts up
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
cv::Mat drawpic(tag tt,int siz);
cv::Mat getpic(tag tt, std::map<tag, cv::Mat>& ma);
cv::Scalar stringtoScalar(std::string a);
#ifdef _WIN32
#include<Windows.h>
void GetStringSize(HDC hDC, const char* str, int* w, int* h);
void putTextZH(cv::Mat &imm, const char* str, cv::Point org, cv::Scalar color, int fontSize,
	const char *fn = "Arial", bool italic = false, bool underline = false,bool cal=1);
#else
void putTextZH(cv::Mat& imm,std::string con,cv::Point pos,cv::Scalar col,int siz,std::string fontdir,bool cal=1);
#endif
void rectangle_cir(cv::Mat imm,cv::Point topleft,int width,int heigth,cv::Scalar col,int border=-1,int cir=8);
#define repeatsol(a,b,c,d,...) for(int i=b;i<b+c;i++)d(a[i], ##__VA_ARGS__);
//a means array, b means start time,c means effective time,
//d means function time,... means the parameter of function
//void superchu(void (p_fun*)(int));
#endif
