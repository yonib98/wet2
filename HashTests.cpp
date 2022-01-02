#include "HashTable.h"
int main(){
    HashTable<int> t1 = HashTable<int>(5);
    t1.insert(5,5*2);
    t1.insert(6,6*2);
    t1.insert(7,7*2);
}