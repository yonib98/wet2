#ifndef WET2_LIST_H
#define WET2_LIST_H
#include "HashTable.h"
template<class T>
class Node{
    int key;
    const T& data;
    Node* next;
    //T יהיה מצביע לPlayer
public:
    Node(int key, T& data): key(key),data(data),next(nullptr){};
    int getKey(){return key;};
    const T& getData(){return data;};
    Node* getNext(){return next;};
    void setNext(Node<T>* next){
        this->next=next;
    }
};
template <class T>
class List{
private:
    Node<T>* root;
public:

    List(): root(nullptr){};
    void insert(int key,const T&);
    T& find(int key) const;
    bool isExist (int key) const;
    void remove(int key);
    Node<T>* getRoot(){return root;}
};
template<class T>
void List<T>::insert(int key,const T& data){
    Node<T>* tmp = root;
    while(tmp->getNext()!= nullptr) {
        tmp = tmp->getNext();
    }
    Node<T>* new_node = new Node<T>(key,data);
    tmp->setNext(new_node);
}
template<class T>
bool List<T>::isExist(int key) const{
    Node<T>* tmp = root;
    while(tmp!=nullptr){
        if(tmp->getKey()==key){
            return true;
        }
        tmp=tmp->getNext();
    }
    return false;
}

template<class T>
T& List<T>::find(int key) const{
    Node<T>* tmp=root;
    while(tmp!= nullptr){
        if(tmp->getKey()==key){
            return tmp->data;
        }
    }
    return nullptr; //shouldnt get here
}


template<class T>
void List<T>::remove(int key){
    Node<T>* tmp = root;
    if(tmp->getKey()==key){
        root=tmp->getNext();
        delete tmp;
        return;
    }
    while(tmp->next!=nullptr){
        if(tmp->next->getKey==key){
            Node<T>* to_remove = tmp->getNext();
            tmp->setNext(to_remove->getNext());
            delete to_remove;
            return;
        }
        tmp=tmp->getNext();
    }
}

#endif //WET2_LIST_H
