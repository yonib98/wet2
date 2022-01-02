#ifndef WET2_HASHTABLE_H
#define WET2_HASHTABLE_H
template<class T>
class HashTable{
private:
    class HashTableItem {
        int key;
        T value;
        HashTableItem* next;
    }
    const int expand_parameter=2;
    HashTableItem** lists_array;
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
    lists_array = new HashTableItem*[table_size];
    for(int i=0;i<table_size;i++){
        lists_array[i]=nullptr;
    }
}
template<class T>
void HashTable<T>::insert(int key,const T& value){
    int index = hash(key);
    HashTableItem* list = data[index];
    while(list>getNext()!= nullptr) {
        if(*list->key==key) {
            throw int();//AlreadyExists
        }
        list = list->getNext();
    }
    if(*list->key==key) {
        throw int();
    }
    HashTableItem* new_item = new HashTableItem<T>(key,data);
    list->setNext(new_item);
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
    HashTableItem<T>** old_lists_array = lists_array;
    HashTableItem<T>** new_lists_array = new HashTableItem<T>*[table_size];
    lists_array=new_lists_array;
    for(int i=0;i<current_size;i++) {
        HashTableItem *list = old_lists_array[i];
        HashTableItem* tmp = list;
        while (tmp != nullptr) {
            int key = tmp->key;
            T value = tmp->value;
            insert(key, value);
            tmp++;
        }
        delete list;
    }
    delete [] old_lists_array;
}

#endif //WET2_HASHTABLE_H
