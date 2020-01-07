#ifndef ENCODER_H
#define ENCODER_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "linkedlist.h"
#include "decoder.h"
#include <bitset>

void mapprint(int ** map);
void mapdestroy(int ** map);

int compare(const void * p1, const void * p2){
  return  (*(node**)p2) -> num - (*(node**)p1) -> num;
}

class encode{
private:
  node ** occurances;
  int length;

public:
  linkedlist list;

  encode (){
    occurances = nullptr;
    length = 0;
    this -> buildoccurances();
  }

  ~encode(){
    this -> destroyoccuraces();
  }

  void buildoccurances(){
    occurances = new node * [256];
    length = 256;
    for(int i = 0; i < length; i++){
      occurances[i] = new node;
      occurances[i] -> num = 0;
      occurances[i] -> letter = 0;
    }
    return;
  }

  void destroyoccuraces(){
    if (occurances == nullptr){return;}
    for(int i = 0; i < length; i++){
      delete this -> occurances[i];
      //occurances[i] = nullptr;
    }
    delete [] this -> occurances;
    length = 0;
    occurances = nullptr;
    return;
  }

  bool countchar(string data){
    stringstream inFile;
    int len;
    inFile << data;
    inFile.seekg(0, inFile.end);
    len = inFile.tellg();
    inFile.seekg(0, inFile.beg);

    unsigned char * file = new unsigned char[len];
    inFile.read((char *)file, len);

    for (int i = 0; i < len; i++){
      #ifdef DEBUG
        cout << file[i];
      #endif
      occurances[(int)file[i]] -> num += 1;
    }

    #ifdef DEBUG
      cout << "here" << endl;
    #endif
    this -> placeletter();
    qsort (occurances, length, sizeof(node *), compare);
    this -> buildlist();
    this -> destroyoccuraces();
    delete [] file;
    return true;
  }

  void placeletter(){
    for(int i = 0; i < length; i++){
      if (occurances[i] -> num){
        occurances[i] -> letter = i;
        if (occurances[i] -> letter == '\n'){
          occurances[i] -> num -= 1;
        }
      }

    }
  }

  void printoccurances(){
    for(int i = 0; i < length; i++){
      if (occurances[i] -> num){
        cout << occurances[i] -> num << " :: ";
        cout << occurances[i] -> letter << endl;
      }
    }
    return;
  }

  void buildlist(){
    for (int i = 0; i < length; i++){
      if (occurances[i] -> num){
        list.addnode(occurances[i]);
      }
    }
    #ifdef DEBUG
      list.printlist();
    #endif
  }

  void ptreehelper(Tnode * tree, int * byte, int i, char debug){
    if (tree -> letter == nullptr){
      byte[i] = 0;
      ptreehelper(tree -> left, byte, i - 1, 'a');
      byte[i] = 1;
      ptreehelper(tree -> right, byte, i - 1, 'a');
    }
    else{
      if (i < 0){return;}
      if (tree -> letter == nullptr){
        return;
      }
      else if (tree -> letter[0] == '\n'){
        cout << "\\n :: ";
      }
      else {
        cout << " " << tree -> letter[0] << " :: ";
      }
      int limit = max(30, i);
      for (int j = 99; j >= limit + 1; j--){
        cout << byte[j];
      }
      cout << " " << 99 - i << endl;
      return;
    }
  }

  void ptreehelper(Tnode * tree, int * byte, int i, ostream& file){
    if (tree -> letter == nullptr){
      byte[i] = 0;
      ptreehelper(tree -> left, byte, i - 1, file);
      byte[i] = 1;
      ptreehelper(tree -> right, byte, i - 1, file);
      file << 0;
    }
    else{
      if (i < 0){return;}
      file << 1;
      file << tree -> letter[0];
      for (int j = 99; j >= i + 1; j--){
        //file << //byte[j];
        cout << byte[j];
      }
      return;
    }
  }

  bool printtree(string filename){
    if (list.head -> next != nullptr){
      return false;
    }
    else{
      Tnode * hold = nullptr;
      hold = list.head -> tree;
      int byte[100] = {0};

      ofstream file;
      file.open(filename);
      if (!file.is_open()){return false;}
      ptreehelper(hold, byte, 99, file);
      file << 0;
      file.close();
      return true;
    }
  }

  void rtreehelper(Tnode * tree, int * byte, int i, writebit * writer, int ** map = nullptr){
    if (tree -> letter == nullptr){
      byte[i] = 0;
      rtreehelper(tree -> left, byte, i - 1, writer, map);
      //byte[i] = 1;
      byte[i] = 1;
      rtreehelper(tree -> right, byte, i - 1, writer, map);
      //byte[i] = 0;
      writer -> write(0);
      if (map == nullptr){
        cout << 0;
      }
      cout << " " << 0 << "";
    }
    else{
      if (i < 0){return;}
      writer -> write(1);
      writer -> writebyte((unsigned char)tree -> letter[0]);

      bitset<8> p((unsigned char)tree -> letter[0]);
      //cout << " " << 1 << " " << p;
      cout << " " << 1 << "" << tree -> letter[0];
      if (map == nullptr){
        cout << 1 << tree -> letter[0];
      }
      else {
        delete [] map[int((unsigned char)tree -> letter[0])]; // posible memory error
        map[int((unsigned char)tree -> letter[0])] = new int [99 - i + 1];
        map[int((unsigned char)tree -> letter[0])][0] = 99 - i;
        for (int j = 99; j >= i + 1; j--){
          map[int((unsigned char)tree -> letter[0])][99 - j + 1] = byte[j];
        }
      }
      return;
    }
  }

  int ** writetree(string filename, writebit * main = nullptr){
    if (list.head -> next != nullptr){
      return nullptr;
    }
    else{
      Tnode * hold = nullptr;
      hold = list.head -> tree;
      int byte[100] = {0};
      int ** map;
      map = new int * [256]; // memory error "Mismatched free() / delete / delete []"
      for (int i = 0; i < 256; i++){
        map[i] = new int[1];
        map[i][0] = 0;
      }

      if (main == nullptr){
        writebit writer;
        rtreehelper(hold, byte, 99, &writer, map);
        writer.close();
        cout << endl;
      }
      else{
        rtreehelper(hold, byte, 99, main, map);
        //cout << endl;
      }

      return map;
    }
  }

  //put into encoder class
  bool printtree(){
    if (list.head -> next != nullptr){
      return false;
    }
    else{
      Tnode * hold = nullptr;
      hold = list.head -> tree;
      int byte[100] = {0};
      ptreehelper(hold, byte, 99, 'a');
      return true;
    }
  }

  int ** buildtree(writebit * writer = nullptr){
    if (list.head == nullptr){return nullptr;}
    while (list.head -> next != nullptr){// we need to make sure the final pointer is nullptr
      list.collapse();
      #ifdef DEBUG
      cout << "\n";
      list.printlist();
      #endif
    }
    int ** map;
    map = writetree("tree", writer);
    #ifdef DEBUG
    printtree();
    #endif
    return map;
  }

  void writefile(int ** map, writebit * writer, string filename){
    stringstream file;
    int len;
    int bitlen;

    file << filename;
    file.seekg(0, file.end);
    len = file.tellg();
    file.seekg(0, file.beg);

    char * full = new char[len];
    file.read(full, len);

    writer -> write(0);
    writer -> write(0);
    //writer -> write(0);
    cout << " " << 0;

    writer -> writeint(len);

    //cout << "\n\n" ;
    int count = 1;
    for(int i = 0; i < len; i++){
      bitlen = map[int((unsigned char)full[i])][0];
      for(int j = 1; j <= bitlen; j++){
        writer -> write(map[int((unsigned char)full[i])][j]);

        #ifdef BITOUT
        cout << map[int((unsigned char)full[i])][j];
        #endif
        count += 1;
      }
    }
    cout << "\n\n" ;
    cout << count << " :: " << len * 8 << " | ratio : " << float(count)/(len * 8.0) * 100;
    cout << "\n\n" ;


    delete [] full;

  }
};


void mapprint(int ** map){
  int length;
  //cout << map << endl;
  for (int i = 0; i < 256; i ++){
    //cout << "here" << endl;
    if (map[i][0] != 0){
      cout << char(i) << " " << i << " :: ";
      length = map[i][0];
      for(int j = 1; j <= length; j++){
        cout << map[i][j];
      }
      cout << endl;
    }
  }
  return;
}

void mapdestroy(int ** map){
  for (int i = 0; i < 256; i++){
    delete [] map[i];
  }
  delete [] map;
}


string write(string filename){
  encode * f = new encode;
  if (f == nullptr){
    return "ERROR";
  }
  writebit writer;
  f -> countchar(filename);

  int ** map = nullptr;
  map = f -> buildtree(&writer);
  if (map == nullptr){
    delete f;
    return "ERROR";;
  }
  f -> writefile(map, &writer, filename);
  string item = writer.close();

  mapprint(map);
  mapdestroy(map);
  delete f;
  cout << item; 
  return item;
}

#endif
