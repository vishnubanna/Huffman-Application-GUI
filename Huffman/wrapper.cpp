//
//  wrapper.cpp
//  Huffman
//
//  Created by Vishnu Banna on 12/26/19.
//  Copyright © 2019 Vishnu Banna. All rights reserved.
//

#include "wrapper.hpp"
#include <stdio.h>
#include <sstream>
#include <string>

char * write1(char * filename, int len){
  string values = conduct(filename, len).getcompress();

  char * ret = new char [values.length()];
  for (int i = 0; i < values.length(); i++){
    printf("%d ,",values.c_str()[i]);
    ret[i] = values.c_str()[i];
  }
  cout << endl;
  return ret;
}

int writelen(char * filename, int len){
  string values = conduct(filename, len).getcompress();
  return int(values.length());
}

char * read1(char * filename, int len){
  string values = conduct(filename, len).getdecomp();
  cout << endl << "key: " << values << endl;
  printf("key: %s \n", values.c_str());
  
  char * ret = new char [values.length() + 1];
  for (int i = 0; i < values.length(); i++){
    //cout << values.c_str()[i];
    ret[i] = values.c_str()[i];
  }
  ret[values.length()] = '\0';
  
  printf("key: %s \n", ret);
  cout << endl;
  return ret;
}

int readlen(char * filename, int len){
  string values = conduct(filename, len).getdecomp();
  return int(values.length());
}

