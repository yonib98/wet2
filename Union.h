#ifndef WET2_UNION_H
#define WET2_UNION_H
class Union{
private:
    const int HEAD=-1;
    int k;
    int* parent;
    int* size;
public:
    Union(int k);
    int find(int i);
    int merge(int p,int q);
    ~Union();
};

#endif //WET2_UNION_H
