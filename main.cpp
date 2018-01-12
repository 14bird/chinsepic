#include "chinesepic.hpp"
#include <string>
#include <iostream>
#include <locale>
#include <codecvt>
using namespace std;
int main() {
#ifdef _WIN32
	makepic("行人", "tmp", "007f00", "ffffff", capturePostion());
#else
	makepic("行人", "tmp", "007f00", "ffffff", capturePostion(10, 98, 40, 16, 4000));
	//std::wcout<<con<<endl;
#endif
}
