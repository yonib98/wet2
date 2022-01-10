#ifndef HW1_MIVNEY_AVLTREE_H
#define HW1_MIVNEY_AVLTREE_H
#include <cmath>
#include "memory"
#include "string"
#include <iostream>
#include "Exceptions.h"
class AVLTree {
    class Node {
        int key_primary;
        int key_secondary;
        int scale;
        int* sub_tree_scores;
        int* self_scores;
        double average;
        Node *parent;
        Node *left;
        Node *right;
        int height;
        friend class AVLTree;
    public:
        Node(int scale): scale(scale),average(0),parent(nullptr),left(nullptr),right(nullptr),height(0){
            //sub_tree_scores = data;
            sub_tree_scores=new int[scale+1];
            self_scores=new int[scale+1];
            for(int i=1;i<scale+1;i++){
                sub_tree_scores[i]=0;
                self_scores[i]=0;
            }
        };
        int getBf(){
            int left_height = left == nullptr ? -1 : left->height;
            int right_height = right == nullptr ? -1 : right->height;
            return left_height - right_height;
        }
        void updateHeight(){
            int lheight = left==nullptr? -1:left->height;
            int rheight = right ==nullptr? -1:right->height;
            height = std::max(lheight,rheight)+1;
        }
        void updateAverage(){
            int count_left = left == nullptr? 0:left->getSubTreePlayersCount();
            int count_right = right == nullptr? 0:right->getSubTreePlayersCount();
            if(count_left+count_right==0){
                average=key_primary;
            }else{
                average=count_left==0? 0:left->average*count_left;
                average+=count_right==0? 0:right->average*count_right;
                int count_current = getPlayersCount();
                average+=count_current*key_primary;
                average=(double)average/(count_left+count_right+count_current);
            }
        }
        void updateSubTreeScores(){
           for(int i=1;i<scale+1;i++){
               sub_tree_scores[i] = left==nullptr? self_scores[i]:left->sub_tree_scores[i]+self_scores[i];
               sub_tree_scores[i] += right== nullptr? 0:right->sub_tree_scores[i];
           }
        }
        bool isLeaf(){
            return right==nullptr && left == nullptr;
        }
        bool isOnlySingleChild(){
            return (right==nullptr && left!= nullptr) || (right!=nullptr && left==nullptr);
        }
        bool isLeftSonOf(const Node& p) const{
            return p.left==this;
        }
        int getKey(){
            return key_primary;
        }
        int* getData() const{
            return self_scores;
        }
        bool isEmptySelfScores(){
            for(int i=1;i<=scale;i++){
                if(self_scores[i]!=0){
                    return false;
                }
            }
            return true;
        }
        int  getPlayersCount(){
            int count=0;
            for(int i=1;i<=scale;i++){
                count+=self_scores[i];
            }
            return count;
        }
        int getSubTreePlayersCount(){
            int count=0;
            for(int i=1;i<=scale;i++){
                count+=sub_tree_scores[i];
            }
            return count;
        }

        bool operator==(const Node &to_compare) const;

        bool operator>(const Node &to_compare) const;

        Node& operator=(const Node& to_copy);
        ~Node(){
            delete[] sub_tree_scores;
            delete[] self_scores;
            parent=nullptr;
            left=nullptr;
            right=nullptr;
        };

    };

protected:
    int size;
    int scale;
    bool use_secondary_key;
    Node *root;
    Node *biggest;
    Node* smallest;

public:
    Node *innerFind(const Node &to_search) const;

    Node* findSequential(Node* p);

    void rightRotation(Node *current_root, Node *root_left_son);

    void leftRotation(Node *current_root, Node *root_right_son);

    Node* innerRemove(Node* to_find,int score, bool* do_remove);

    Node* sequentialRemove(Node* root);

    void treeClear(Node* root);


    void merge(Node** A,int na,Node** B,int nb, Node** C);

    static Node* createCompleteTree(Node* root, int h, int scale);

public:
    AVLTree()=default;
    AVLTree(bool use_secondary_key,int scale);

    AVLTree& operator=(const AVLTree& tree_to_copy);

    int* find(int key_primary, int key_secondary) const;

    void insert(int key_primary, int key_secondary, int score);

    void remove(int key_primary, int key_secondary, int score);

    void updateSmallest();

    void updateBiggest();

    bool isEmpty() const;

    int* getBiggest() const;

    const int* getSmallest() const;


    int getSize() const;

    void helpExport(Node* root,int** arr, int* count) const;

    void helpIDExport(Node* root,int* arr,int *count) const;

    int** exportToArray(int** arr) const;

    int* exportToIDArray(int* arr) const;

    void exportToNodeArray(Node* root,Node** arr,int* count);

    void mergeWith(AVLTree& another_tree);

    static AVLTree almostCompleteTree(bool use_secondary_key,int n,int scale);

    void makeAlmostComplete(Node* root,int n);

    void pushArrayToTree(Node* root,Node** arr,int* index);

    void deleteTree();

    Node* treeCopy(Node* root);

    template<class Predicate>
            void inOrder(int* count,Predicate p) const;
    template<class Predicate>
            void helpInOrder(int* count,Predicate p,Node* root) const;

    void scoresInInterval(bool lower_bound, int score,int level,int* sum_of_players,int* sum_of_players_with_score);
    void scoresInBounds(int lower_level,int higher_level,int score,int *sum_of_players,int* sum_of_players_with_score);

    void getMaxAverage(int m,int * sum);

    int getTreePlayersCount();

    ~AVLTree();
    int deleteDuplicates(Node** arr,int n);
};

#endif //HW1_MIVNEY_AVLTREE_H