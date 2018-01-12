#include "chinesepic.hpp"
#include <iostream>
#include <fstream>
#include <string>
bool operator<(tag a,tag b){
    if(a.name!=b.name)return a.name<b.name;
    if(a.fc<b.fc)return a.fc<b.fc;
    if(a.bc<b.bc)return a.bc<b.bc;
    return a.con<b.con;
}
#ifdef __unix__
#include <cstdlib>
#include <unistd.h>
#else
#include <Windows.h>
#include <gdiplus.h>
#pragma comment(lib,"gdiplus.lib")
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes
	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;
	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure
	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure
	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);
	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}
	free(pImageCodecInfo);
	return -1;  // Failure
}
#endif
cv::Mat makepic(tag tt, capturePostion cap) {
    //std::cout<<"make"<<std::endl;
	std::ofstream oo("tmp.html");
	oo << "<html><body>" << std::endl;
	oo << "<script language=\"javascript\">" << std::endl;
	oo << "<!--" << std::endl;
	oo << "function clock(){i=i-1" << std::endl;
	oo << "document.title=";
	oo << "\"this window will be closed after \"+i+\"seconds\"" << std::endl;
	oo << "if(i>0)setTimeout(\"clock();\",1000)" << std::endl;
	oo << "else self.close();}" << std::endl;
	oo << "var i=5" << std::endl;
	oo << "clock();" << std::endl;
	oo << "//-->" << std::endl;
	oo << "</script>" << std::endl;
	oo << "<button type=\"button\" style=\"background:#";
	oo << tt.bc;
	oo << ";color:#";
	oo << tt.fc;
	oo << " \">";
	oo << tt.con;
	oo << "</button>" << std::endl;
	oo << "</body></html>" << std::endl;
	oo.close();
#ifdef __unix__
	if (system("xdg-open tmp.html")) {
		std::cout << "please install xdg-open." << std::endl;
	}
	usleep(cap.tim * 1000);
#ifdef __linux__
	if (system("scrot tmp.bmp")) {
		std::cout << "please install scrot." << std::endl;
		exit(0);
	}
#else
	if (system("screencapture tmp.bmp")) {
		std::cout << "please install screencapture" << std::endl;
	}
#endif
#else
	system("explorer tmp.html");
	Sleep(cap.tim);
	HDC hScreenDC = CreateDC("DISPLAY", NULL, NULL, NULL);
	// and a device context to put it in
	HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
	int width = GetDeviceCaps(hScreenDC, HORZRES);
	int height = GetDeviceCaps(hScreenDC, VERTRES);
	// maybe worth checking these are positive values
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);
	// get a new bitmap
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemoryDC, hBitmap);
	BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY);
	hBitmap = (HBITMAP)SelectObject(hMemoryDC, hOldBitmap);
	// clean up
	DeleteDC(hMemoryDC);
	DeleteDC(hScreenDC);
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	Gdiplus::Bitmap *image = new Gdiplus::Bitmap(hBitmap, NULL);
	CLSID myClsId;
	int retVal = GetEncoderClsid(L"image/bmp", &myClsId);
	image->Save(L"tmp.bmp", &myClsId, NULL);
	delete image;
	Gdiplus::GdiplusShutdown(gdiplusToken);
#endif
	remove("tmp.html");
	cv::Mat im = cv::imread("tmp.bmp");
	remove("output.bmp");
	cv::Mat tmp = im(cv::Rect(cap.startx, cap.starty, cap.width, cap.heigth));
	cv::imwrite(tt.name + ".bmp", tmp);
    return tmp;
}
cv::Mat getpic(tag tt,std::map<tag,cv::Mat>& ma){
    if(ma.find(tt)==ma.end()){
        ma.insert(std::make_pair(tt,makepic(tt,capturePostion(10, 98, 40, 16, 4000))));
    }
    return ma[tt];
}
