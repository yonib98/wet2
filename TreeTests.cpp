#include "AVLTree.h"
int main(){
    AVLTree t = AVLTree(false,10);
    t.insert(1,0,3);
    t.insert(1,0,4);
    t.insert(1,0,5);
    t.insert(2,0,4);
    t.insert(2,0,5);
    t.insert(2,0,3);
    t.insert(3,0,3);
    t.remove(1,0,3);
    t.remove(1,0,4);
    t.remove(1,0,5);

}