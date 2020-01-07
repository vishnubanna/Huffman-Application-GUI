#ifndef DECODER_H
#define DECODER_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "linkedlist.h"

using namespace std;
// you have to run valgrind !!!!!!!!
// check that all news are deleted, and all files are closed;
int readbyte();


class readbit{
private:
  stringstream file;
  //string filename;
  char * ffull;
  unsigned char currbyte;
  unsigned char byteshifted1;
  unsigned char currbyte2;
  unsigned char byteshifted2;
  int bytesread;
  int bitsread;
  int size;
  int bitsize;

public:
  readbit(string data){
    //this -> filename = filename;
    currbyte = 0x0;
    currbyte2 = 0x0;
    byteshifted1 = 0x0;
    byteshifted2 = 0x0;
    bytesread = 0;
    bitsread = 7;

    //file.open(filename, ios::in);
    file << data;
    file.seekg(0, file.end);
    size = file.tellg();
    file.seekg(0, file.beg);

    bitsize = size * 8;

    ffull = new char[size];
    //ffull = data.c_str();
    file.read(ffull, size);
  }
  ~readbit(){
    delete [] ffull;
    //file.close();
  }

  int read(){
    // read 1 bit
    int bit;
    if(bytesread >= size){
      return -1;
    }
    if(bitsread == -1){
      bytesread += 1;
      bitsread = 7;
    }

    currbyte = ffull[bytesread];
    bit = this -> check(bitsread);
    //cout << bit << " :: " << bitsread << " :: " << bytesread << endl;
    bitsread = (bitsread - 1);
    return bit;
  }

  int readbyte(){
    // read 8 bits
    unsigned char byte;
    byte = 0x0;
    int bit;
    //cout << endl;
    for (int i = 0; i < 8; i++){
      bit = this -> read();
      // if(bit == -1){
      //   return -1;
      // }
      //cout << bit;
      bit = (unsigned char) bit;
      //cout << bit << endl;
      byte = byte << 1;
      byte |= bit;
    }
    //cout << endl;
    // if (byte == ' '){
    //   cout << "space";
    // }
    // else{
    //   cout << byte;
    // }
    return byte;
  }

  int readint(){
    int value;
    int output;
    //4byte int
    //value = 0x0;
    output = 0x0;

    for(int i = 0; i < 32; i++){
      value = this -> read();
      output = output << 1;
      output |= value;
      //cout << output << endl;
      cout << value;
    }

    return output;
  }

  int check(int location){
    unsigned char bitcheck = 0x1;
    bitcheck = bitcheck << location;
    bitcheck &= currbyte;
    if (bitcheck != 0){
      return 1;
    }
    else {
      return 0;
    }
    bitcheck = 0x0;
  }
};



class decoder{
private:
  readbit * reader;
  string filename;
  Tnode * holder;
  ostringstream outfile;
public:
  linkedlist * list;
  int count;
  int size;

  decoder(string filename){
    list = new linkedlist;
    reader = new readbit(filename);
    this -> filename = filename;
    size = this -> getsize();
    holder = nullptr;
    count = 0;
  }

  ~decoder(){
    list -> printlist();
    printtree(list);
    delete list;
    delete reader;
  }

  bool buildtree(){
    int bit;
    char byte;
    int nodes = 0;
    node hold;
    //cout << endl << endl;
    cout << "tree: ";
    bit = reader -> read();
    cout << nodes << " :: " << size * 8 << " :: ";
    for (int i = 1; i <= size * 8; i++){
      if (bit == 1){
        byte = reader -> readbyte();
        cout << bit;
        cout << byte << " ";

        hold.letter = byte;
        hold.num = bit;

        // cout << endl;
        list -> addnode(&hold);

        //tree -> printlist();
        bit = reader -> read();
        nodes += 1;
      }
      else if (bit == 0){
        cout << bit << " ";
        list -> collapse(false);
        //tree -> printlist();
        bit = reader -> read();
        nodes -= 1;
      }

      if (nodes == 0){
        break;
      }
    }
    cout << endl << endl;
    holder = list -> head -> tree;

    size = this -> readsize();
    //cout << size;
    return true;
  }

  int readsize(){
    int fsize = 0;
    fsize = reader -> readint();
    return fsize;
  }

  int getsize(){
    int sizek;
    stringstream file;
    file << filename;
    file.seekg(0, file.end);
    sizek = file.tellg();
    file.seekg(0, file.beg);
    return sizek;
  }

  string getstr(){
    return outfile.str();
  }

  bool readfile(){
    int bit;
    if (count == size - 1){return false;}
    if (holder -> right == nullptr && holder -> left == nullptr) {
      cout << holder -> letter[0];
      outfile.write(&holder -> letter[0], 1);
      holder = list -> head -> tree;
      count += 1;
      return true;
    }
    bit = reader -> read();
    //cout << bit;
    if (bit == 1){
      holder = holder -> right;
    }
    else {
      holder = holder -> left;
    }
    return true;
  }

};

string read(string filename){
  cout << "\n\nDECODE :: \n";
  //string filename = filen;
  decoder * decode = new decoder(filename);
  decode -> buildtree();
  cout << " :: " << decode -> size << endl << endl;
  while(decode -> readfile());
  string filetot = decode -> getstr();
  cout << endl << endl;
  delete decode;
  return filetot;
}

#endif
