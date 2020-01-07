#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;


class writebit{
private:
  ostringstream file;
  unsigned char byte;
  unsigned char word;
  int writen;
public:
  writebit(){
    byte = 0x0;
    word = 0x0;
    writen = 7;
  }

  ~writebit(){}

  string close(){
    this -> write(2);
    return file.str();
  }

  void write(int bit){
    if (bit == 2){
      file << word;
      writen = 7;
      word = 0;
    }
    if (bit == 1){
      byte = 0x1;
    }
    else{
      byte = 0x0;
    }
    byte = byte << writen;
    word |= byte;
    writen -= 1;
    if (writen == -1){
      file << word;
      writen = 7;
      word = 0;
    }
    #ifdef DEBUG
    cout << "writen :" << writen << endl;
    cout << "word :" <<  word << endl;
    #endif
  }

  // this does not work correctly
  void writebyte(unsigned char input){
    unsigned char valfinish;
    unsigned char valnext;

    int fin = 7 - writen;
    int nex = writen + 1;
    valfinish = input >> fin;
    word |= valfinish;

    #ifdef DEBUG
    cout << "input :" << input << endl;
    cout << "writen :" << writen << endl;
    cout << "fin :" << fin << endl;
    cout << "valfinish :" << valfinish << endl;
    cout << "word :" << word << endl;
    #endif

    file << word;
    word = 0;
    valnext = input << nex;
    word |= valnext;

    #ifdef DEBUG
    cout << "nex :" << nex << endl;
    cout << "valnex :" << (int)valnext << endl;
    cout << "word :" << (int)word << endl;
    #endif
  }

  void writeint(int value){
    int arr[32] = {0};
    int start;

    #ifdef BITOUT
    cout << "\n\nlength :: " << value << " | binary :: " ;
    #endif

    start = 31;// - count;
    while(start >= 0){
      arr[start] = value % 2;
      value = value / 2;
      start -= 1;
    }

    for (int i = 0; i < 32; i++){
      #ifdef BITOUT
      cout << arr[i];
      #endif
      this -> write(arr[i]);
    }
    #ifdef BITOUT
    cout << endl << endl;
    #endif
  }

};


typedef struct treenode{
  char * letter;
  struct treenode * left;
  struct treenode * right;
}Tnode;

typedef struct listnode{
  int num;
  Tnode * tree;
  struct listnode * next;
}Lnode;

typedef struct node{
  char letter;
  int num;
}node;

class linkedlist{
// private:
//   Lnode * head;
public:
  Lnode * head;
  linkedlist(){
    head = nullptr;
  }

  ~linkedlist(){
    //cout << head;
    Lnode * pointer;
    pointer = head;
    Lnode * hold;
    hold = nullptr;

    while (pointer != nullptr){
      //cout << pointer << endl;
      hold = pointer;
      pointer = pointer -> next;
      if (hold != nullptr){
        hold -> tree = this -> deletetnode(hold -> tree);
      }
      delete hold;
      hold = nullptr;
    }
  }

  Tnode * deletetnode(Tnode * n){
    //cout << n;

    if (n != nullptr){
      //cout << " " << n -> left << " " << n -> right << endl;
      if (n -> left != nullptr){
        n -> left = deletetnode(n -> left);
      }

      if (n -> right != nullptr){
        n -> right = deletetnode(n -> right);
      }

      if(n -> letter != nullptr){
        delete [] n -> letter;
        n -> letter = nullptr;
      }

      delete n;
      n = nullptr;
    }


    return n;
  }

  void addnode(node * values){
    if (head == nullptr){
      head = new Lnode;
      head -> num = values -> num;
      head -> tree = new Tnode;
      head -> tree -> letter = new char[1];
      head -> tree -> letter[0] = values -> letter;
      head -> tree -> right = nullptr;
      head -> tree -> left = nullptr;
      head -> next = nullptr;;
    }
    else {
      Lnode * pointer = new Lnode;
      pointer -> num = values -> num;
      pointer -> tree = new Tnode;
      pointer -> tree -> letter = new char[1];
      pointer -> tree -> letter[0] = values -> letter;
      pointer -> tree -> right = nullptr;
      pointer -> tree -> left = nullptr;
      pointer -> next = head;
      head = pointer;
    }
  }

  void ordpreserve(){
    if(head == nullptr || head -> next == nullptr){
      return;
    }
    else{
      if (head -> num <= head -> next -> num){
        return;
      }
      else{
        Lnode * nhead;
        Lnode * pointer;
        Lnode * hold;
        pointer = head;
        nhead = head -> next;

        while(pointer -> next != nullptr && pointer -> next -> num < head -> num){
          pointer = pointer -> next;
        }
        hold = head;
        head = nhead;
        hold -> next = pointer -> next;
        pointer -> next = hold;
      }
    }
  }

  void collapse(bool pres = true){
    Tnode * full;
    Lnode * hold;
    Lnode * newhead;

    if(head -> next == nullptr){
      return;
    }

    full = new Tnode;

    full -> letter = nullptr;
    full -> right = head -> tree;

    full -> left = head -> next -> tree;





    hold = head;
    hold -> tree = nullptr;
    head -> tree = nullptr;
    head = head -> next;

    newhead = new Lnode;
    newhead -> tree = full;
    newhead -> num = head -> num + hold -> num;
    delete hold;

    hold = head;
    hold -> tree = nullptr;
    head -> tree = nullptr;
    head = head -> next;
    delete hold;

    newhead -> next = head;

    head = newhead;

    if (pres){
      this -> ordpreserve();
    }
  }

  void printlist(){
    Lnode * pointer;
    pointer = head;
    if (head -> next != nullptr){cout << "broke" << endl;}

    while (pointer != nullptr){
      if (pointer -> tree -> letter == nullptr){
        cout << "nullptr :: " << pointer -> num << endl;
      }
      else if (pointer -> tree -> letter[0] == '\n'){
        cout << "\\n :: " << pointer -> num << endl;
      }
      else{
        cout << pointer -> tree -> letter[0] << " :: " << pointer -> num << endl;
      }
      pointer = pointer -> next;
    }
  }
};



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

void ptreehelper(Tnode * tree, int * byte, int i, writebit * writer){
  if (tree -> letter == nullptr){
    byte[i] = 0;
    ptreehelper(tree -> left, byte, i - 1, writer);
    byte[i] = 1;
    ptreehelper(tree -> right, byte, i - 1, writer);
    writer -> write(0);
  }
  else{
    if (i < 0){return;}
    writer -> write(1);
    writer -> writebyte((unsigned char)tree -> letter[0]);
    // for (int j = 99; j >= i + 1; j--){
    //   //file << //byte[j];
    cout << 1 << tree -> letter[0];
    //}
    return;
  }
}

bool printtree(linkedlist * values, string filename){
  if (values -> head -> next != nullptr){
    return false;
  }
  else{
    Tnode * hold = nullptr;
    hold = values -> head -> tree;
    int byte[100] = {0};
    //int count;

    writebit writer;
    ptreehelper(hold, byte, 99, &writer);
    writer.close();
    //delete writer;

    // ifstream file2;
    // file2.open(filename);
    // file2.seekg(0, file2.end);
    // count = file2.tellg();
    // cout << "\n" << count << endl;
    // file2.close();
    return true;
  }
}
//put into encoder class
bool printtree(linkedlist * values){
  if (values -> head -> next != nullptr){
    return false;
  }
  else{
    Tnode * hold = nullptr;
    hold = values -> head -> tree;
    int byte[100] = {0};
    ptreehelper(hold, byte, 99, 'a');
    return true;
  }
}

void buildtree(linkedlist * values){
  while (values -> head -> next != nullptr){
    values -> collapse();
    #ifdef DEBUG
    cout << "\n";
    values -> printlist();
    #endif
  }
  printtree(values, "tree");
  printtree(values);
}

#endif
