#include "HashTable.h"
int main(){
    HashTable<int> t1 = HashTable<int>(5);
    t1.insert(5,5*2);
    t1.insert(6,6*2);
    t1.insert(7,7*2);
    t1.insert(8,8*2);
    t1.insert(9,18);
    t1.insert(10,20);
    t1.remove(7);
    t1.remove(6);
    t1.remove(8);
    t1.remove(9);
    t1.remove(5);
    t1.remove(10);
    return 0;
}
