#ifndef WET2_HASHTABLE_H
#define WET2_HASHTABLE_H
#include "List.h"
template<class T>
class HashTable{
private:
    const int expand_parameter=2;
    List<T>* data;
    int current_size;
    int table_size;//Prime?
    int hash(int key);
    void shrink();
    void expand();
public:

    HashTable(int initial_size);
    T& search(int key) const;
    void insert(int key,const T&);
    void remove(int key);
};
template<class T>
int HashTable<T>::hash(int key){
    return key%table_size;
}
template<class T>
HashTable<T>::HashTable(int initial_size): current_size(0),table_size(initial_size){
    data = new List<T>[initial_size];
    for(int i=0;i<initial_size;i++){
        data[i]= List<T>();
    }
}
template<class T>
void HashTable<T>::insert(int key,const T& value){
    int index = hash(key);
    List<T> list = data[index];
    if(list.isExist(key)){
        throw (int());
    }
    list.insert(key,value);
    current_size++;
    if(current_size==table_size){
        expand();
    }
}

template<class T>
void HashTable<T>::remove(int key){
    int index = hash(key);
    List<T> list = data[index];
    if(list.isExist(key)){
        list.remove(key);
    }
    table_size--;
    if(current_size<=table_size/4){
        shrink();
    }
}
template<class T>
T& HashTable<T>::search(int key) const{
    int index = hash(key);
    List<T> list = data[index];
    if(list.isExist(key)){
        return list.find(key);
    }
    throw int();//does not exist
}

template<class T>
void HashTable<T>::expand() {
    table_size*=expand_parameter;
    List<T>* new_data = new List<T>[table_size];ֿ
    for(int i=0;i<current_size;i++){
        if(data[i].getRoot()== nullptr)
            continue;
        List<T> list=data;
        Node<T>* root=data[i].getRoot();
        while(list.getRoot()!= nullptr){
            int key=list.getRoot()->key;
            int new_index=hash(key);
            T& value=list.getRoot()->getData();
            new_data[new_index].insert(key,value);
            Node<T>* to_remove=list.getRoot();
            list.getRoot()=list.getRoot()->getNext();
            delete (to_remove);
        }

    }
}

#endif //WET2_HASHTABLE_H
