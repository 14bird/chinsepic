#include "chinesepic.hpp"
#include <string>
#include <iostream>
#include <locale>
#include <codecvt>
using namespace std;
int main() {
#ifdef _WIN32
	makepic(tag("行人", "tmp", "007f00", "ffffff"), capturePostion());
#else
	//makepic(tag("行人", "tmp", "007f00", "ffffff"), capturePostion(10, 98, 40, 16, 4000));
    map<tag,cv::Mat> ma;
    cv::namedWindow("win");
    imshow("win",getpic(tag("行人", "tmp", "007f00", "ffffff"),ma));
    cv::waitKey(0);
    imshow("win",getpic(tag("行人", "tmp", "007f00", "ffffff"),ma));
    cv::waitKey(0);
	//std::wcout<<con<<endl;
#endif

}
