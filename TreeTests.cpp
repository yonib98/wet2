#include "AVLTree.h"
int main(){
    AVLTree t = AVLTree(false,10);
    t.insert(100,0,3);
    t.insert(200,0,4);
//    t.insert(300,0,6);
//    t.insert(400,0,6);
//    t.insert(350,0,6);
//    t.insert(370,0,7);
//    t.insert(380,0,8);
//    t.insert(390,0,8);
//    t.insert(410,0,2);
    t.insert(500,0,2);
    t.insert(420,0,2);
    t.insert(415,0,2);
    t.remove(420,0,2);
    t.remove(390,0,8);

}