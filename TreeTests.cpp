#include "AVLTree.h"
int main(){
    AVLTree t = AVLTree(false,10);
    t.insert(100,0,3);
    t.insert(100,0,4);
    t.insert(100,0,5);
    t.insert(200,0,4);
    t.insert(200,0,5);
    t.insert(200,0,3);
    t.insert(300,0,3);
    AVLTree t2 = AVLTree(false,10);
    t2.insert(100,0,3);
    t2.insert(100,0,4);
    t2.insert(100,0,5);
    t2.insert(200,0,4);
    t2.insert(200,0,5);
    t2.insert(200,0,3);
    t2.insert(300,0,3);
    t.mergeWith(t2);

}