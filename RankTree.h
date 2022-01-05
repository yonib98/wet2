#ifndef WET2_GIT_RANKTREE_H
#define WET2_GIT_RANKTREE_H
#include "AVLTree.h"
class RankTree : public AVLTree<int*>{
    int scale;
    int* zero_scores;
public:
    explicit RankTree(int scale);
    void insertAtZero(int score);
};
RankTree::RankTree(int scale): AVLTree<int *>(false), scale(scale) {
    zero_scores = new int[scale+1];
    for(int i=1;i<scale;i++){
        zero_scores[i]=0;
    }
}

void RankTree::insertAtZero(int score) {
   if(score<0){
       //Complete this
       throw int();
   }
    zero_scores[score]++;
}
#endif //WET2_GIT_RANKTREE_H
