#ifndef __CHINESEPIC_HPP
    #define __CHINESEPIC_HPP 1
    #include<string>
struct capturePostion{
	int startx, starty, width, heigth,tim;
	capturePostion() {
		startx = 10;//左上角坐标
		starty = 76;
		width = 40;//图片大小
		heigth = 16;
		tim = 1000;//浏览器启动时间
	}
	capturePostion(int a,int b) {
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
	capturePostion(int a,int b,int c,int d,int e) {
		startx = a;
		starty = b;
		width = c;
		heigth = d;
		tim = e;
	}
};
    #ifdef __unix__
        void makepic(std::string con,std::string name,std::string bc,std::string fc,capturePostion cap);
    #else
		void makepic(std::wstring con, std::string name, std::string bc, std::string fc,capturePostion cap);
    #endif
#endif
