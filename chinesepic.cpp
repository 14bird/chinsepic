#include "chinesepic.hpp"
#include <iostream>
#include <fstream>
#include <string>
#ifdef __unix__
    void makepic(std::string con,std::string name,std::string bc,std::string fc){
        std::ofstream oo("tmp.html");
        oo<<"<html><body>"<<std::endl;
        oo<<"<button type=\"button\" style=\"background:#";
        oo<<bc<<";color:#"<<fc;
        oo<<" \">"<<con<<"</button>"<<std::endl;
        oo<<"</body></html>"<<std::endl;
        oo.close();
        //remove("tmp.html");
    }
#else
#endif
