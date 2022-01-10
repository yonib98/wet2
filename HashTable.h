#ifndef WET2_HASHTABLE_H
#define WET2_HASHTABLE_H
#include "Exceptions.h"
template<class T>
class HashTable{
private:
    class HashTableItem {
        friend class HashTable;
        int key;
        T value;
        HashTableItem* next;
        HashTableItem(int key,const T& value): key(key),value(value),next(nullptr){};
    };
    const int expand_parameter=2;
    HashTableItem** lists_array;
    int current_size;
    int table_size;//Prime?
    int hash(int key) const;
    void shrink();
    void expand();
public:

    explicit HashTable(int initial_size);
    ~HashTable();
    void clear();
    T* search(int key) const;
    void insert(int key,const T&);
    void remove(int key);
};
template<class T>
int HashTable<T>::hash(int key) const {
    return key%table_size;
}
template<class T>
HashTable<T>::HashTable(int initial_size): current_size(0),table_size(initial_size){
    lists_array = new HashTable<T>::HashTableItem*[table_size];
    for(int i=0;i<table_size;i++){
        lists_array[i]=nullptr;
    }
}
template<class T>
void HashTable<T>::insert(int key,const T& value){
    int index = hash(key);
    HashTable<T>::HashTableItem* list = lists_array[index];
    if(list==nullptr){
        lists_array[index]=new HashTable<T>::HashTableItem(key,value);
        current_size++;
        if(current_size==table_size){
            expand();
        }
        return;
    }
    while(list->next!= nullptr) {
        if(list->key==key) {
            throw AlreadyExists();//AlreadyExists
        }
        list = list->next;
    }
    if(list->key==key) {
        throw AlreadyExists();
    }
    HashTable<T>::HashTableItem* new_item =new HashTable<T>::HashTableItem(key,value);
    list->next=new_item;
    current_size++;
    if(current_size==table_size){
        expand();
    }
}

template<class T>
void HashTable<T>::remove(int key){
    int index = hash(key);
    HashTable<T>::HashTableItem* list = lists_array[index];
    HashTable<T>::HashTableItem* temp=list;
    if(temp->key==key){
        lists_array[index]=list->next;
        delete(temp);
        current_size--;
        if(current_size<=table_size/4){
            shrink();
        }
        return;
    }
    while(temp->next!=nullptr){
        if(temp->next->key==key){
            HashTableItem* to_delete = temp->next;
            temp->next=temp->next->next;
            delete to_delete;
            break;
        }
        temp=temp->next;
    }
    current_size--;
    if(current_size<=table_size/4){
        shrink();
    }
}
template<class T>
T* HashTable<T>::search(int key) const{
    int index = hash(key);
    HashTable<T>::HashTableItem* list = lists_array[index];
    HashTable<T>::HashTableItem* temp=list;
    if(temp==nullptr){
        return nullptr;
    }
    while(temp->next!= nullptr){
        if(temp->key==key){
            return &temp->value;
        }
        temp=temp->next;
    }
    if(temp->key==key){
        return &temp->value;
    }
    return nullptr;//does not exist
}

//AddPlayer 1511996719 5 52 ->>> 4 ->> 9
//AddPlayer 976756301 29 22 ->> 1 ->> 1
//AddPlayer 1000848759 16 98 ->> 4 ->> 4
//AddPlayer 1819072310 13 51 ->>> 0 -> 0
//AddPlayer 660622207 15 74 ->>> 2 -> 7
template<class T>
void HashTable<T>::expand() {
    int old_table_size=table_size;
    table_size*=expand_parameter;
    HashTable<T>::HashTableItem** old_lists_array = lists_array;
    HashTable<T>::HashTableItem** new_lists_array = new HashTable<T>::HashTableItem*[table_size];
    for(int i=0;i<table_size;i++){
        new_lists_array[i]= nullptr;
    }
    lists_array=new_lists_array;
    current_size=0;
    for(int i=0;i<old_table_size;i++) {
        HashTable<T>::HashTableItem* list = old_lists_array[i];
        HashTable<T>::HashTableItem* tmp = list;
        while (tmp != nullptr) {
            int key = tmp->key;
            T value = tmp->value;
            insert(key, value);
            HashTable<T>::HashTableItem* to_delete=tmp;
            tmp = tmp->next;
            delete to_delete;
        }
    }
    delete [] old_lists_array;
}

template <class T>
void HashTable<T>::shrink() {
    int old_table_size=table_size;
    table_size/=expand_parameter;
    HashTable<T>::HashTableItem** old_lists_array = lists_array;
    HashTable<T>::HashTableItem** new_lists_array = new HashTable<T>::HashTableItem*[table_size];
    for(int i=0;i<table_size;i++){
        new_lists_array[i]= nullptr;
    }
    lists_array=new_lists_array;
    current_size=0;
    for(int i=0;i<old_table_size;i++) {
        HashTable<T>::HashTableItem* list = old_lists_array[i];
        HashTable<T>::HashTableItem* tmp = list;
        while (tmp != nullptr) {
            int key = tmp->key;
            T value = tmp->value;
            insert(key, value);
            HashTable<T>::HashTableItem* to_delete=tmp;
            tmp = tmp->next;
            delete to_delete;

        }
    }
    delete [] old_lists_array;
}

template<class T>
HashTable<T>::~HashTable(){
    clear();
    delete [] lists_array;
}

template<class T>
void HashTable<T>::clear(){
    for(int i=0;i<table_size;i++){
        HashTable<T>::HashTableItem* list=lists_array[i];
        while(list!= nullptr){
            HashTable<T>::HashTableItem* temp=list;
            list=list->next;
            delete temp;
        }
    }
}
#endif //WET2_HASHTABLE_H
