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
};
Union::Union(int k): k(k){
    parent=new int[k+1];
    size = new int[k+1];
    for(int i=1;i<k;i++){
        parent[i]=HEAD;
        size[i]=1;
    }
}
int Union::find(int i){
    int copy_i=i;
    if(parent[i]==HEAD){
        return i;
    }
    while(parent[i]!=HEAD){
        i=parent[i];
    }
    while(parent[copy_i]!=HEAD){
        int tmp=parent[copy_i];
        parent[copy_i]=i;
        copy_i=tmp;
    }
    return i;
}
int Union::merge(int p,int q){
    if(p==q){
        return p;
    }
    if(size[p]<size[q]){
        parent[p]=q;
        size[q]+=size[p];
        size[p]=0;
        return q;
    }else{
        parent[q]=p;
        size[p]+=size[q];
        size[q]=0;
        return p;
    }
}

#endif //WET2_UNION_H
