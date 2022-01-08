#include "AVLTree.h"
AVLTree::AVLTree(bool use_secondary_key,int scale):size(0),scale(scale), use_secondary_key(use_secondary_key), root(nullptr),biggest(nullptr){}

AVLTree::~AVLTree() {
    deleteTree();
}
AVLTree& AVLTree::operator=(const AVLTree& tree_to_copy){
    if(this==&tree_to_copy){
        return *this;
    }
    this->deleteTree();
    root = this->treeCopy(tree_to_copy.root);
    size=tree_to_copy.size;
    use_secondary_key=tree_to_copy.use_secondary_key;
    biggest=tree_to_copy.biggest;
    return *this;

}

typename AVLTree::Node* AVLTree::innerFind(const Node& to_search) const {
    Node* closest_parent = nullptr;
    Node* temp=root;
    while(temp!=nullptr){
        if(to_search==*temp) {
            return temp;
        }
        else if (to_search>*temp){
            closest_parent=temp;
            temp=temp->right;
        }
        else{
            closest_parent=temp;
            temp=temp->left;

        }
    }
    return closest_parent;
}

int* AVLTree::find(int key_primary,int key_secondary) const {
    Node to_search(scale);
    to_search.key_primary = key_primary;
    to_search.key_secondary=key_secondary;
    Node* result = innerFind(to_search);
    if(result!= nullptr){
        if(*result==to_search){
            return result->self_scores;
        }
    }
    throw DoesNotExist();

}

void AVLTree::insert(int key_primary,int key_secondary,int score) {
    Node *to_insert = new Node(scale);
    to_insert->key_primary = key_primary;
    if(use_secondary_key) {
        to_insert->key_secondary = key_secondary;
    }
    else{
        to_insert->key_secondary = 0;
    }
    to_insert->self_scores[score]++;
    to_insert->sub_tree_scores[score]++;
    to_insert->average=key_primary;
    Node *result = innerFind(*to_insert);
    if(result==nullptr){
        root=to_insert;
        size++;
        updateBiggest();
        return;
    }
    if (*result == *to_insert) {
        result->sub_tree_scores[score]++;
        result->self_scores[score]++;
        result->updateAverage();
        Node* temp=result;
        while(temp!=root){
            Node* parent = temp->parent;
            if(parent==nullptr){
                break;
            }
            parent->updateSubTreeScores();
            parent->updateAverage();
            temp=temp->parent;
        }
        delete to_insert;
        return;
    }

    if (*to_insert > *result) {
        result->right = (to_insert);
        to_insert->parent = result;
    } else {
        result->left = (to_insert);
        to_insert->parent = result;
    }
    Node *temp = to_insert;
    bool rotation = false;
    Node* second_temp=temp;
    while(second_temp!=root){
        Node* parent = second_temp->parent;
        if(parent==nullptr){
            break;
        }
        parent->updateSubTreeScores();
        parent->updateAverage();
        second_temp=second_temp->parent;
    }
    while (temp != root && !rotation) {
        Node* parent = temp->parent;
        if(parent->height >= temp->height+1){
            break;
        }else{
            parent->height = temp->height+1;
        }
        int current_bf = parent->getBf();
        if (current_bf == 2) {
            rotation = true;
            Node *left_son = parent->left;
            int left_bf = left_son->getBf();
            if (left_bf >= 0) {
                if(parent->parent== nullptr)
                    root=left_son;
                else {
                    if (parent->parent->left == parent) {
                        parent->parent->left = left_son;
                    } else {
                        parent->parent->right = left_son;
                    }
                }
                rightRotation(parent, left_son);
                //LL -> right
            } else {

                Node *left_right_son = left_son->right;
                parent->left = (left_right_son);
                leftRotation(left_son, left_right_son);

                if(parent->parent== nullptr){
                    root=left_right_son;
                }
                else{
                    if(parent->parent->left == parent)
                    {
                        parent->parent->left=left_right_son;
                    }else{
                        parent->parent->right = left_right_son;
                    }
                }
                rightRotation(parent, left_right_son);
                //LR
            }
        } else if (current_bf == -2) {
            rotation = true;
            Node *right_son = parent->right;
            int right_bf = right_son->getBf();
            if (right_bf <= 0) {
                //RR
                if(parent->parent== nullptr)
                    root=right_son;
                else {
                    if (parent->parent->left == parent) {
                        parent->parent->left = right_son;
                    } else {
                        parent->parent->right = right_son;
                    }
                }
                leftRotation(parent, right_son);
            } else {

                Node *right_left_son = right_son->left;
                parent->right = (right_left_son);
                rightRotation(right_son, right_left_son);

                if (parent->parent == nullptr) {
                    root = right_left_son;
                } else {
                    if(parent->parent->left == parent){
                        parent->parent->left = right_left_son;
                    }else{
                        parent->parent->right = right_left_son;
                    }

                }
                leftRotation(parent, right_left_son);
                //RL
            }
        }
        temp=parent;
    }
    size++;
    updateBiggest();
}
typename AVLTree::Node* AVLTree::findSequential( typename AVLTree::Node* p){
    p=p->right;
    while(p->left!= nullptr){
        p=p->left;
    }
    return p;
}

typename AVLTree::Node* AVLTree::innerRemove(Node* to_find,int score,bool* do_remove ) {

    Node *found = innerFind(*to_find);
    Node* parent;
    if(*found==*to_find){
        found->self_scores[score]--;
        found->sub_tree_scores[score]--;
        found->updateAverage();
        if(!found->isEmptySelfScores()){
            *do_remove=false;
            return found;
        }
        parent=found->parent;
    }else{
        throw DoesNotExist();
    }
    *do_remove=true;
    if (found->isLeaf()) {
        if (parent == nullptr) {
            root = nullptr;
        } else {
            if (parent->right == found) {
                parent->right = nullptr;
            } else {
                parent->left = nullptr;
            }
        }
        delete found;
        return parent;
    } else if (found->isOnlySingleChild()) {
        if(parent==nullptr){
            root=found->right!=nullptr? found->right:found->left;
            root->parent=nullptr;
            delete found;
            return parent;
        }
        if (found->right != nullptr) {
            if (parent->right == found) {
                parent->right = found->right;
                found->right->parent=parent;
            } else {
                parent->left = found->right;
                found->right->parent=parent;
            }
        } else {
            if (parent->right == found) {
                parent->right = found->left;
                found->left->parent=parent;
            } else {
                parent->left = found->left;
                found->left->parent=parent;
            }
        }
        if(found->left!=nullptr){
            found->left->parent = parent;
        }
        delete found;
        return parent;
    } else {
        Node *new_node = findSequential(found);
        if (found == root) {
            root = new_node;
        }
        if(parent!=nullptr){
            if(found->isLeftSonOf(*parent)){
                parent->left=new_node;
            }else{
                parent->right=new_node;
            }
        }
        if(new_node==found->right){
            //No left son for found
            new_node->left=found->left;
            found->left->parent=new_node;
            new_node->parent=parent;
            delete found;
            if(parent==nullptr){
                return new_node;
            }
            return parent;

        }
        Node *new_left_son = found->left;
        Node *new_right_son = found->right;
        Node *new_parent = found->parent;

        if(new_node->right!=nullptr){
            new_node->right->parent=found;
        }
        found->left = nullptr;
        found->right = new_node->right;
        found->updateHeight();
        new_node->parent->left=found;
        found->parent = new_node->parent;

        new_node->left = new_left_son;
        new_node->right = new_right_son;
        new_node->updateHeight();
        new_node->parent = new_parent;

        if(new_left_son!= nullptr){
            new_left_son->parent=new_node;
        }
        if(new_right_son!= nullptr){
            new_right_son->parent=new_node;
        }

        Node* p= sequentialRemove(new_node);
        return p;
    }
    return nullptr; //shouldnt get here
}


typename AVLTree::Node* AVLTree::sequentialRemove(Node* root){
    root=root->right;
    while(root->left!=nullptr){
        root=root->left;
    }
    Node* p = root->parent;
    if(root->isLeaf()){
        if(root->isLeftSonOf(*p)){
            p->left=nullptr;
        }else{
            p->right=nullptr;
        }
    }else{
        p->left=root->right;
        root->right->parent=p;
    }
    delete root;
    return p;
}


void AVLTree::remove(int key_primary, int key_secondary,int score){
    Node to_find = Node(scale);
    to_find.key_primary = key_primary;
    if (use_secondary_key) {
        to_find.key_secondary = key_secondary;
    }
    else to_find.key_secondary = 0;
    bool do_remove=false;
    Node* parent = innerRemove(&to_find,score, &do_remove);
    if(do_remove==false){
        while(parent!=nullptr){
            parent->updateSubTreeScores();
            parent->updateAverage();
            parent=parent->parent;
        }
        return;
    }
    while(parent!=nullptr){
        parent->updateHeight();
        parent->updateSubTreeScores();
        parent->updateAverage();
        int current_bf = parent->getBf();
        if(current_bf==2){
            Node* left_son = parent->left;
            int left_bf = left_son->getBf();
            if(left_bf>=0){
                if(parent->parent== nullptr)
                    root=left_son;
                else {
                    if (parent->parent->left == parent) {
                        parent->parent->left = left_son;
                    } else {
                        parent->parent->right = left_son;
                    }
                }
                rightRotation(parent, left_son);
                //LL
            }
            else{
                Node *left_right_son = left_son->right;
                parent->left = (left_right_son);
                leftRotation(left_son, left_right_son);

                if(parent->parent== nullptr){
                    root=left_right_son;
                }
                else{
                    if(parent->parent->left == parent)
                    {
                        parent->parent->left=left_right_son;
                    }else{
                        parent->parent->right = left_right_son;
                    }
                }
                rightRotation(parent, left_right_son);
                //LR
            }
        }
        if(current_bf==-2){
            Node *right_son = parent->right;
            int right_bf = right_son->getBf();
            if (right_bf <= 0) {
                //RR
                if(parent->parent== nullptr)
                    root=right_son;
                else {
                    if (parent->parent->left == parent) {
                        parent->parent->left = right_son;
                    } else {
                        parent->parent->right = right_son;
                    }
                }
                leftRotation(parent, right_son);
            } else {

                Node *right_left_son = right_son->left;
                parent->right = (right_left_son);
                rightRotation(right_son, right_left_son);

                if (parent->parent == nullptr) {
                    root = right_left_son;
                } else {
                    if(parent->parent->left == parent){
                        parent->parent->left = right_left_son;
                    }else{
                        parent->parent->right = right_left_son;
                    }

                }
                leftRotation(parent, right_left_son);
                //RL
            }
        }
        parent=parent->parent;
    }
    updateBiggest();
    size--;
}

bool AVLTree::isEmpty() const {
    return root == nullptr;
}

void AVLTree::rightRotation(Node *current_root, Node *root_left_son) {
    Node *temp = root_left_son->right;
    for(int i=0;i<scale;i++){
        root_left_son->sub_tree_scores[i]=current_root->sub_tree_scores[i];

    }
    if(current_root->right!=nullptr){
        for(int i=0;i<scale;i++){
            current_root->sub_tree_scores[i]=current_root->right->sub_tree_scores[i];
        }
    }else{
        for(int i=0;i<scale;i++){
            current_root->sub_tree_scores[i] = current_root->self_scores[i];
        }
    }
    if(temp!=nullptr){
        for(int i=0;i<scale;i++){
            current_root->sub_tree_scores[i]+=temp->sub_tree_scores[i];
        }
    }
    root_left_son->right = (current_root);
    root_left_son->parent=current_root->parent;
    current_root->parent=root_left_son;
    current_root->left =temp;
    if(temp!= nullptr)
        temp->parent=current_root;
    current_root->updateHeight();
    root_left_son->updateHeight();
    current_root->updateAverage();
    root_left_son->updateAverage();
}

void AVLTree::leftRotation(Node *current_root, Node *root_right_son) {

    Node *temp = root_right_son->left;
    for (int i = 1; i <= scale; i++) {
        root_right_son->sub_tree_scores[i] = current_root->sub_tree_scores[i];

    }
    if (current_root->left != nullptr) {
        for (int i = 1; i <= scale; i++) {
            current_root->sub_tree_scores[i] = current_root->left->sub_tree_scores[i];
        }
    } else {
        for (int i = 1; i <= scale; i++) {
            current_root->sub_tree_scores[i] = current_root->self_scores[i];
        }
    }
    if (temp != nullptr) {
        for (int i = 1; i <= scale; i++) {
            current_root->sub_tree_scores[i] += temp->sub_tree_scores[i];
        }
    }
    root_right_son->left = (current_root);
    root_right_son->parent = current_root->parent;
    current_root->parent = root_right_son;
    current_root->right = temp;
    if (temp != nullptr)
        temp->parent = current_root;
    current_root->updateHeight();
    root_right_son->updateHeight();
    current_root->updateAverage();
    root_right_son->updateAverage();
}

bool  AVLTree::Node::operator==(const AVLTree::Node& to_compare) const {
    return this->key_primary == to_compare.key_primary &&
           this->key_secondary == to_compare.key_secondary;
}
typename AVLTree::Node& AVLTree::Node::operator=(const Node &to_copy) {
    if(this==&to_copy){
        return *this;
    }
    key_primary= to_copy.key_primary;
    key_secondary=to_copy.key_secondary;
    for(int i=1;i<=scale;i++){
        self_scores[i]=to_copy.self_scores[i];
    }
    return *this;
}
bool AVLTree::Node::operator>(const typename AVLTree::Node &to_compare) const {
    if (this->key_primary == to_compare.key_primary) {
        return this->key_secondary < to_compare.key_secondary;
    }
    return this->key_primary > to_compare.key_primary;
}
void AVLTree::updateSmallest(){
    Node* tmp = root;
    smallest=root;
    while(tmp!=nullptr){
        smallest = tmp;
        tmp = tmp->left;
    }
}
void AVLTree::updateBiggest(){
    Node* tmp = root;
    biggest=root;
    while(tmp!=nullptr){
        biggest = tmp;
        tmp = tmp->right;
    }
}

void AVLTree::deleteTree(){
    treeClear(root);
    root= nullptr;
    biggest= nullptr;
}
void AVLTree::treeClear(Node* root) {
    if(root== nullptr){
        return;
    }
    treeClear(root->left);
    treeClear(root->right);
    delete root;
}
typename AVLTree::Node* AVLTree::treeCopy(typename AVLTree::Node* root){
    if(root==nullptr){
        return nullptr;
    }
    typename AVLTree::Node* node =new typename AVLTree::Node(scale);
    *(node)=*root;
    node->left= treeCopy(root->left);
    if(node->left!=nullptr){
        node->left->parent=node;
    }
    node->right= treeCopy(root->right);
    if(node->right!=nullptr){
        node->right->parent=node;
    }
    node->updateHeight();
    return node;
}

int* AVLTree::getBiggest() const  {
    return biggest->self_scores;
}
void AVLTree::helpIDExport(Node* root,int* arr, int* count) const {
    if(root==nullptr || *count==size){
        return;
    }
    helpIDExport(root->right,arr,count);
    if(root->key_secondary==0){
        arr[*count]=root->key_primary;
    }else{
        arr[*count] = root->key_secondary;
    }
    (*count)++;
    helpIDExport(root->left,arr,count);

}
void AVLTree::helpExport(Node* root,int** arr, int* count) const {
    if(root==nullptr || *count==size){
        return;
    }
    helpExport(root->right,arr,count);
    arr[*count] = root->self_scores;
    (*count)++;
    helpExport(root->left,arr,count);

}
int** AVLTree::exportToArray(int** arr) const {
    int count = 0;
    helpExport(root,arr,&count);
    return arr;
}
int* AVLTree::exportToIDArray(int* arr) const {
    int count = 0;
    helpIDExport(root,arr,&count);
    return arr;
}
template<class Predicate>
void AVLTree::helpInOrder(int* count, Predicate p, typename AVLTree::Node* root) const {
    if(root==nullptr || *count==0) {
        return;
    }
    helpInOrder(count,p,root->left);
    if(*count>0){
        p(root->self_scores,count);
        (*count)--;
    }
    helpInOrder(count,p,root->right);
}
template<class Predicate>
void AVLTree::inOrder(int* count,Predicate p) const {
    helpInOrder(count,p,root);
}
int AVLTree::getSize() const {
    return size;
}
void AVLTree::exportToNodeArray(typename AVLTree::Node* root,
                                typename AVLTree::Node** arr,int* count) {
    if(root==nullptr || *count==size){
        return;
    }
    exportToNodeArray(root->left,arr,count);
    arr[*count] = root;
    (*count)++;
    exportToNodeArray(root->right,arr,count);
}
void AVLTree::merge(typename AVLTree::Node** A,int na,
                    typename AVLTree::Node** B,int nb, typename AVLTree::Node** C){
    int ia=0,ib=0,ic=0;
    while(ia<na && ib < nb){
        if(*A[ia]>*B[ib]){
            C[ic++]=B[ib++];
        }else{
            C[ic++]=A[ia++];
        }
    }
    for(;ia<na;ic++,ia++){
        C[ic]=A[ia];
    }
    for(;ib<nb;ib++,ic++){
        C[ic]=B[ib];
    }
}
void AVLTree::mergeWith(AVLTree& another_tree) {
    int second_arr_size = another_tree.getSize();
    Node** my_arr = new Node*[size];
    Node** second_arr = new Node*[second_arr_size];
    int count=0;
    this->exportToNodeArray(this->root,my_arr,&count);

    count=0;
    another_tree.exportToNodeArray(another_tree.root,second_arr,&count);

    int merged_arr_size = size+second_arr_size;
    Node** merge_arr = new Node*[merged_arr_size];
    merge(my_arr,size,second_arr,another_tree.getSize(),merge_arr);

    AVLTree almost_complete_tree = AVLTree::almostCompleteTree(this->use_secondary_key,merged_arr_size,scale);
    int index=0;
    almost_complete_tree.pushArrayToTree(almost_complete_tree.root,merge_arr,&index);
    almost_complete_tree.updateBiggest();
    //O(n) do delete  + o(n_k1+nk_2) for copying
    *this=almost_complete_tree;
    delete[] my_arr;
    delete[] second_arr;
    delete[] merge_arr;
}
typename AVLTree::Node* AVLTree::createCompleteTree(typename AVLTree::Node* root,int h,int scale){
    root= new typename AVLTree::Node(scale);
    if(h==0){
        return root;
    }
    root->left=createCompleteTree(root->left,h-1,scale);
    root->left->parent=root;
    root->right=createCompleteTree(root->right,h-1,scale);
    root->right->parent=root;
    root->updateHeight();
    return root;
}

AVLTree AVLTree::almostCompleteTree(bool use_secondary_key,int n,int scale){
    AVLTree almost_complete_tree = AVLTree(use_secondary_key,scale);
    int h = std::ceil(std::log2(n+1))-1;
    almost_complete_tree.root = AVLTree::createCompleteTree(nullptr,h,scale);
    almost_complete_tree.size=std::pow(2,h+1)-1;

    almost_complete_tree.makeAlmostComplete(almost_complete_tree.root,n);
    //reverse in order -> almost complete tree
    return almost_complete_tree;
}
void AVLTree::makeAlmostComplete(typename AVLTree::Node* root, int n){
    if (root==nullptr || size==n){
        return;
    }
    if(root->isLeaf()){
        if(root->parent!=nullptr){
            Node* parent = root->parent;
            if(root==parent->left){
                parent->left=nullptr;
            }else{
                parent->right=nullptr;
            }
        }
        delete root;
        size--;
        return;
    }
    makeAlmostComplete(root->right,n);
    makeAlmostComplete(root->left,n);

}
void AVLTree::pushArrayToTree(typename AVLTree::Node* root,typename AVLTree::Node** arr,int* index){
    if(*index==size || root==nullptr){
        return;
    }
    pushArrayToTree(root->left,arr,index);
    *root=*(arr[*index]);
    (*index)++;
    pushArrayToTree(root->right,arr,index);
}
void AVLTree::scoresInInterval(bool lower_bound, int score,int level,int* sum_of_players,int* sum_of_players_with_score){
    Node to_search(scale);
    to_search.key_primary = level;
    Node* temp=root;
    while(temp!=nullptr){
        if(to_search==*temp) {
            if(lower_bound==false){
                for(int i=1;i<=scale;i++){
                    *sum_of_players+=temp->self_scores[i];
                }
                *sum_of_players_with_score+=temp->self_scores[score];
            }
            if(temp->left!=nullptr){
                for(int i=1;i<=scale;i++){
                    *sum_of_players+=temp->left->sub_tree_scores[i];
                }
                *sum_of_players_with_score+=temp->left->sub_tree_scores[score];
            }
            break;
        }
        else if (to_search>*temp){
            *sum_of_players_with_score+=temp->self_scores[score];
            if(temp->left!=nullptr){
                *sum_of_players_with_score+=temp->left->sub_tree_scores[score];
            }
            for(int i=1;i<=scale;i++){
                *sum_of_players+=temp->self_scores[i];
                if(temp->left!=nullptr){
                    *sum_of_players+=temp->left->sub_tree_scores[i];
                }
            }
            temp=temp->right;
        }
        else{
            temp=temp->left;

        }
    }
}
void AVLTree::scoresInBounds(int lower_level,int higher_level,int score,int *sum_of_players,int* sum_of_players_with_score){
    *sum_of_players=0;
    *sum_of_players_with_score=0;
    scoresInInterval(false,score,higher_level,sum_of_players,sum_of_players_with_score);
    int sum_not_in_range=0;
    int sum_not_in_range_score=0;
    scoresInInterval(true,score,lower_level,&sum_not_in_range,&sum_not_in_range_score);
    *sum_of_players-=sum_not_in_range;
    *sum_of_players_with_score-=sum_not_in_range_score;
}
void AVLTree::getMaxAverage(int m,int * sum){
    int dup_m=m;
    *sum=0;
    Node* tmp = root;
    while(tmp!=nullptr && m>0) {
        if (tmp->right != nullptr) {
            int right_count = tmp->right->getSubTreePlayersCount();
            if (right_count <= m) {
                *sum += right_count * tmp->right->average;
                m -= right_count;
            } else {
                tmp = tmp->right;
                continue;
            }
        }
        int current_count = tmp->getPlayersCount();
        if (current_count <= m) {
            *sum += current_count * tmp->key_primary;
            m -= current_count;
        } else {
            *sum += m * tmp->key_primary;
            m = 0;

        }
        tmp = tmp->left;
    }
}

int AVLTree::getTreePlayersCount(){
    int count=0;
    if(root== nullptr){
        return count;
    }
    for(int i=1;i<=scale;i++){
        count+=root->sub_tree_scores[i];
    }
    return count;
}