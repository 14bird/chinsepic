#include "chinesepic.hpp"
#include <iostream>
#include <fstream>
#include <string>
bool operator<(tag a, tag b) {
	if (a.name != b.name)return a.name<b.name;
	if (a.fc<b.fc)return a.fc<b.fc;
	if (a.bc<b.bc)return a.bc<b.bc;
	return a.con<b.con;
}
#ifdef __unix__
#include <cstdlib>
#include <unistd.h>
//#include <X11/Xlib.h>
#include <Imlib2.h>
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
cv::Mat getpic(tag tt, std::map<tag, cv::Mat>& ma) {
	if (ma.find(tt) == ma.end()) {
		ma.insert(std::make_pair(tt, drawpic(tt,IMG_SIZ)));
	}
	return ma[tt];
}
#ifdef _WIN32
void GetStringSize(HDC hDC, const char* str, int* w, int* h)
{
	SIZE size;
	GetTextExtentPoint32A(hDC, str, strlen(str), &size);
	if (w != 0) *w = size.cx;
	if (h != 0) *h = size.cy;
}

void putTextZH(cv::Mat &imm, const char* str, cv::Point org, cv::Scalar color, int fontSize, const char* fn, bool italic, bool underline,bool cal)
{
    cv::Mat dst;
    if(cal){
        dst = imm(cv::Rect(org.x,org.y,(int)((strlen(str))*fontSize*1.4),(int)(fontSize*1.4)));
        org.x=0,org.y=0;
    }
    else{
        dst = imm;
    }
	CV_Assert(dst.data != 0 && (dst.channels() == 1 || dst.channels() == 3));

	int x, y, r, b;
	if (org.x > dst.cols || org.y > dst.rows) return;
	x = org.x < 0 ? -org.x : 0;
	y = org.y < 0 ? -org.y : 0;

	LOGFONTA lf;
	lf.lfHeight = -fontSize;
	lf.lfWidth = 0;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = 5;
	lf.lfItalic = italic;   //斜体
	lf.lfUnderline = underline; //下划线
	lf.lfStrikeOut = 0;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = 0;
	lf.lfClipPrecision = 0;
	lf.lfQuality = PROOF_QUALITY;
	lf.lfPitchAndFamily = 0;
	strcpy_s(lf.lfFaceName, fn);

	HFONT hf = CreateFontIndirectA(&lf);
	HDC hDC = CreateCompatibleDC(0);
	HFONT hOldFont = (HFONT)SelectObject(hDC, hf);

	int strBaseW = 0, strBaseH = 0;
	int singleRow = 0;
	char buf[1 << 12];
	strcpy_s(buf, str);
	char *bufT[1 << 12];  // 这个用于分隔字符串后剩余的字符，可能会超出。
						  //处理多行
	{
		int nnh = 0;
		int cw, ch;

		const char* ln = strtok_s(buf, "\n", bufT);
		while (ln != 0)
		{
			GetStringSize(hDC, ln, &cw, &ch);
			strBaseW = max(strBaseW, cw);
			strBaseH = max(strBaseH, ch);

			ln = strtok_s(0, "\n", bufT);
			nnh++;
		}
		singleRow = strBaseH;
		strBaseH *= nnh;
	}

	if (org.x + strBaseW < 0 || org.y + strBaseH < 0)
	{
		SelectObject(hDC, hOldFont);
		DeleteObject(hf);
		DeleteObject(hDC);
		return;
	}

	r = org.x + strBaseW > dst.cols ? dst.cols - org.x - 1 : strBaseW - 1;
	b = org.y + strBaseH > dst.rows ? dst.rows - org.y - 1 : strBaseH - 1;
	org.x = org.x < 0 ? 0 : org.x;
	org.y = org.y < 0 ? 0 : org.y;

	BITMAPINFO bmp = { 0 };
	BITMAPINFOHEADER& bih = bmp.bmiHeader;
	int strDrawLineStep = strBaseW * 3 % 4 == 0 ? strBaseW * 3 : (strBaseW * 3 + 4 - ((strBaseW * 3) % 4));

	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = strBaseW;
	bih.biHeight = strBaseH;
	bih.biPlanes = 1;
	bih.biBitCount = 24;
	bih.biCompression = BI_RGB;
	bih.biSizeImage = strBaseH * strDrawLineStep;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;

	void* pDibData = 0;
	HBITMAP hBmp = CreateDIBSection(hDC, &bmp, DIB_RGB_COLORS, &pDibData, 0, 0);

	CV_Assert(pDibData != 0);
	HBITMAP hOldBmp = (HBITMAP)SelectObject(hDC, hBmp);

	//color.val[2], color.val[1], color.val[0]
	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkColor(hDC, 0);
	//SetStretchBltMode(hDC, COLORONCOLOR);

	strcpy_s(buf, str);
	const char* ln = strtok_s(buf, "\n", bufT);
	int outTextY = 0;
	while (ln != 0)
	{
		TextOutA(hDC, 0, outTextY, ln, strlen(ln));
		outTextY += singleRow;
		ln = strtok_s(0, "\n", bufT);
	}
	uchar* dstData = (uchar*)dst.data;
	int dstStep = dst.step / sizeof(dstData[0]);
	unsigned char* pImg = (unsigned char*)dst.data + org.x * dst.channels() + org.y * dstStep;
	unsigned char* pStr = (unsigned char*)pDibData + x * 3;
	for (int tty = y; tty <= b; ++tty)
	{
		unsigned char* subImg = pImg + (tty - y) * dstStep;
		unsigned char* subStr = pStr + (strBaseH - tty - 1) * strDrawLineStep;
		for (int ttx = x; ttx <= r; ++ttx)
		{
			for (int n = 0; n < dst.channels(); ++n) {
				double vtxt = subStr[n] / 255.0;
				int cvv = vtxt * color.val[n] + (1 - vtxt) * subImg[n];
				subImg[n] = cvv > 255 ? 255 : (cvv < 0 ? 0 : cvv);
			}

			subStr += 3;
			subImg += dst.channels();
		}
	}

	SelectObject(hDC, hOldBmp);
	SelectObject(hDC, hOldFont);
	DeleteObject(hf);
	DeleteObject(hBmp);
	DeleteDC(hDC);
}
#else
void putTextZH(cv::Mat& imm,std::string con,cv::Point pos,cv::Scalar col,int siz,std::string fontdir,bool cal){
    //std::cout<<con.size()<<std::endl;
    cv::Mat im;
    if(cal){
        im = imm(cv::Rect(pos.x,pos.y,(int)(con.size()*siz*1.4),(int)(siz*1.4)));
        pos.x=0,pos.y=0;
    }
    else{
        im = imm;
    }
    Imlib_Image img;
    img = imlib_create_image (im.cols,im.rows);
    imlib_context_set_image(img);
    DATA32* imdata=imlib_image_get_data ();
    Imlib_Font fonts=imlib_load_font(fontdir.c_str());
    imlib_context_set_font(fonts);
     imlib_context_set_color (col[2], col[1], col[0], 227);
    if(fonts==NULL){
        std::cout<<"No font"<<std::endl;
        exit(0);
    }
    int coll[3];
    cv::namedWindow("wi");
    coll[0]=im.data[0],coll[1]=im.data[1],col[2]=im.data[2];
    cv::MatIterator_<cv::Vec3b> it, end;
    int i=0;
    for( it = im.begin<cv::Vec3b>();it!= im.end<cv::Vec3b>();it++){
        imdata[i++]=255u*256u*256u*256u+((unsigned int)((*it)[2]))*256*256+((unsigned int)((*it)[1]))*256+(unsigned int)((*it)[0]);
    }
    int wi=siz*(con.size()+1),he=siz,ze=0;
    int *p_w=&wi,*p_h=&he,*p_z=&ze;
    imlib_text_draw_with_return_metrics(pos.x,pos.y,con.c_str(),p_w,p_h,p_z,p_z);
    imlib_image_put_back_data(imdata);
    i=0;
    for( it = im.begin<cv::Vec3b>();it!= im.end<cv::Vec3b>();it++){
        unsigned int tt=imdata[i++];
        (*it)[0]=tt&255;tt>>=8;
        (*it)[1]=tt&255;tt>>=8;
        (*it)[2]=tt&255;
    }
    imlib_free_image();
}
#endif
unsigned char gs(std::string s) {
	if (s.size() != 2) {
		std::cout << "plese input the correct color of tag!" << std::endl;
		exit(0);
	}
	unsigned char ans = 0;
	ans += s[0] > '9' ? s[0] - 'a' +10: s[0] - '0';
	ans *= 16;
	ans += s[1] > '9' ? s[1] - 'a' +10: s[1] - '0';
	return ans;
}
cv::Scalar stringtoScalar(std::string a) {
	return cv::Scalar(gs(a.substr(0,2)),gs(a.substr(2,2)),gs(a.substr(4,2)));
}
cv::Mat drawpic(tag tt,int siz) {
	cv::Mat im(siz,siz*(tt.con.size()+1),CV_8UC3,stringtoScalar(tt.bc));
    #ifdef _WIN32
	putTextZH(im, tt.con.c_str(), cv::Point(0, 0), stringtoScalar(tt.fc), siz, "Arial");
    #else
    putTextZH(im, tt.con, cv::Point(0, 0), stringtoScalar(tt.fc), IMG_SIZ ,"/usr/share/fonts/TTF/DroidSansFallback/10");
    #endif
	cv::namedWindow("wi");
	imshow("wi", im);
	cv::waitKey(0);
	return im;
}
void rectangle_cir(cv::Mat im,cv::Point topleft,int width,int height,cv::Scalar col,int border,int r){
    width=std::max(3,width);height=std::max(3,height);
    if(width<2*r||height<2*r){
        r=std::min(height/2,width/2);
    }
    if(border<0){
        rectangle(im,cv::Rect(topleft.x,topleft.y+r,width,height-r*2),col,-1);
        rectangle(im,cv::Rect(topleft.x+r,topleft.y,width-r*2,height),col,-1);
        cv::circle(im,cv::Point(topleft.x+r,topleft.y+r),r,col,-1);
        cv::circle(im,cv::Point(topleft.x+width-r,topleft.y+r),r,col,-1);
        cv::circle(im,cv::Point(topleft.x+r,topleft.y+height-r),r,col,-1);
        cv::circle(im,cv::Point(topleft.x+width-r,topleft.y+height-r),r,col,-1);
    }
    else{
        cv::line(im,cv::Point(topleft.x+r,topleft.y),cv::Point(topleft.x+width-r,topleft.y),col,border);
        cv::line(im,cv::Point(topleft.x+r,topleft.y+height),cv::Point(topleft.x+width-r,topleft.y+height),col,border);
        cv::line(im,cv::Point(topleft.x,topleft.y+r),cv::Point(topleft.x,topleft.y+height-r),col,border);
        cv::line(im,cv::Point(topleft.x+width,topleft.y+r),cv::Point(topleft.x+width,topleft.y+height-r),col,border);
        cv::ellipse(im,cv::Point(topleft.x+r,topleft.y+r),cv::Size(r,r),0,180,270,col,border);
        cv::ellipse(im,cv::Point(topleft.x+width-r,topleft.y+r),cv::Size(r,r),0,270,360,col,border);
        cv::ellipse(im,cv::Point(topleft.x+r,topleft.y+height-r),cv::Size(r,r),0,90,180,col,border);
        cv::ellipse(im,cv::Point(topleft.x+width-r,topleft.y+height-r),cv::Size(r,r),0,0,90,col,border);
    }
}
