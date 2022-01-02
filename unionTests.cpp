#include "Union.h"
int main(){
    Union test1=Union(40);
    int x1 = test1.find(5);
    test1.merge(test1.find(5),test1.find(3));
    x1 = test1.find(5);
    test1.merge(test1.find(5),test1.find(6));
    x1=test1.find(5);
    test1.merge(test1.find(2),test1.find(3));
    x1=test1.find(2);
    test1.merge(test1.find(5),test1.find(2));
    x1 = test1.find(2);
}