//
//  wrapper.hpp
//  Huffman
//
//  Created by Vishnu Banna on 12/26/19.
//  Copyright © 2019 Vishnu Banna. All rights reserved.
//

#ifndef wrapper_hpp
#define wrapper_hpp

#include <stdio.h>
#include <iostream>
#include "linkedlist.h"
#include "decoder.h"
#include "encoder.h"
#include "main.hpp"


#ifdef __cplusplus
extern "C" {
#endif
  char * write1(char * filename, int len);
  char * read1(char * filename,  int len);
  int writelen(char * filename,  int len);
  int readlen(char * filename,  int len);
#ifdef __cplusplus
}
#endif

#endif /* wrapper_hpp */
