#include "chinesepic.hpp"
#include <string>
#include <iostream>
#include <locale>
#include <codecvt>
using namespace std;
int main() {
    std::map<tag, cv::Mat> ma;
	getpic(tag("行人", "tmp", "007f00", "ffffff"), ma);
}
