#ifndef ArrayListInt_h
#define ArrayListInt_h
// the #include statment and code go here...
#include "Arduino.h"
 
class ArrayListInt {
public:	
  ArrayListInt(int*); //Arraylist(void);
  ~ArrayListInt(void);
  //void display_list();
  void add_item(int* item);
  void set_item(int* item, int index);
  void remove_selected_item(int index);
  void empty_list();
  void set_list(int** list);
  int** get_list();
  int get_size();
private:
  int** list;
  int size;	
};
 
#endif
 
 
/*
 * Arduino Class for ArrayListInt
 * Written: Obed Isai Rios
 */
 
 
#include "Arduino.h"
#include "ArrayListInt.h"
 
ArrayListInt::ArrayListInt(int* init){
  list = (int**)malloc(10*sizeof(int*));
  list[0] = init;
  this->size = 1; 
}
 
ArrayListInt::~ArrayListInt(void)
{
}
 
void ArrayListInt::add_item(int* item){
  int **neulist = (int**)malloc((size+1)*sizeof(int*));
  for(int i=0; i<size; i++){
    neulist[i] = list[i];
   }
   //
   neulist[size] = item;
   list = neulist;
   size = size + 1;
}
 
void ArrayListInt::set_item(int* item, int index){
  list[index] = item;
}
 
void ArrayListInt::remove_selected_item(int index){
  int **neulist = (int**)malloc((size-1)*sizeof(int*));
  //From Begining
  for(int i=0; i<index; i++){
    neulist[i] = list[i]; 
  }
  //From next Index  
  for(int i=index; i<=size-1; i++){
    neulist[i] = list[i+1];
  }
  //free(matrix);
  list = neulist;
  size = size - 1;
}
 
void ArrayListInt::empty_list(){
   size = 1;
   int **neulist = (int**)malloc((size)*sizeof(int*));   
   list = neulist;
   list[0] = 0;
}
 
/*void ArrayListInt::display_list(){
   Serial.begin(9600);
  for(int i=0; i<size; i++){
    Serial.println(list[i]);
   }
}*/
 
int** ArrayListInt::get_list(){
  return this->list;
}
 
void ArrayListInt::set_list(int** list){
  this->list = list;
}
 
int ArrayListInt::get_size(){
  return this->size;
}
 
