//
//  main.hpp
//  StringHuffman
//
//  Created by Vishnu Banna on 12/31/19.
//  Copyright © 2019 Vishnu Banna. All rights reserved.
//

#ifndef main_hpp
#define main_hpp

#include <stdio.h>
#include <string>
#include <sstream>
#include "encoder.h"
#include "decoder.h"
#include "linkedlist.h"

using namespace std;

class conduct{
private:
  string filename1;
public:
  conduct(char * input, int len){
    stringstream ss;
    ss.write(input, len);
    filename1 = ss.str();
    cout << filename1;
  }
  
  string getcompress(){
    cout << filename1;
    return write(filename1);
  }
  
  string getdecomp(){
    cout << filename1; 
    return read(filename1);
  }
};

#endif /* main_hpp */
