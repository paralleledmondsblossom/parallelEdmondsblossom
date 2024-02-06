#ifndef FRONTIER_H
#define FRONTIER_H
#include <vector>
#include "Vertex.h"

template <typename IT, template <typename> class StackType = std::vector >
class Frontier  {
public:
    Frontier();
    Frontier(size_t _capacity);

    void reinit(std::vector<Vertex<IT>> &vertexVector);
    bool verifyTree(std::vector<Vertex<IT>> &vertexVector,
                    std::vector<std::atomic<IT>> &matching);

    void updateTree(std::vector<Vertex<IT>> &vertexVector);
    void updateVertexVector(std::vector<Vertex<IT>> &vertexVector);
    //void split(Frontier<IT> & f2,IT type=0);
    void clear();

    // Other member functions...
    IT time;
    IT TailOfAugmentingPathVertexIndex;
    IT root;
    StackType<IT> stack;
    //std::vector<IT> stack;
    std::vector<Vertex<IT>> tree;
    size_t capacity;
};

// Constructor
template <typename IT, template <typename> class StackType>
Frontier<IT, StackType>::Frontier():time(0),TailOfAugmentingPathVertexIndex(-1) {
}

// Constructor
template <typename IT, template <typename> class StackType>
Frontier<IT, StackType>::Frontier(size_t _capacity):time(0),TailOfAugmentingPathVertexIndex(-1),capacity(_capacity),stack(_capacity),tree(_capacity) {
    
}

// Constructor
template <typename IT, template <typename> class StackType>
void Frontier<IT, StackType>::reinit(std::vector<Vertex<IT>> &vertexVector){      
    for (auto &V : tree) {
        vertexVector[V.LabelField].TreeField=-1;
        vertexVector[V.LabelField].BridgeField=-1;
        vertexVector[V.LabelField].ShoreField=-1;
        vertexVector[V.LabelField].AgeField=-1;
        vertexVector[V.LabelField].LinkField=V.LabelField;
        vertexVector[V.LabelField].DirectParentField=-1;
        vertexVector[V.LabelField].GroupRootField=V.LabelField;
        vertexVector[V.LabelField].SizeField=1;
    }
}

// Constructor
/*
template <typename IT, template <typename> class StackType>
void Frontier<IT, StackType>::split(Frontier<IT> & f2,IT type){
    f2.tree=tree;
    const size_t original_size = stack.size();
    std::move(stack.begin()+(original_size/2), stack.end(), std::back_inserter(f2.stack));
    stack.erase(stack.begin() + (original_size / 2), stack.end());
    if(original_size != stack.size()+f2.stack.size()){
        printf("MASSIVE ERROR %ld != %ld + %ld\n",original_size,stack.size(),f2.stack.size());
        exit(1);
    }
}
*/

// Constructor
template <typename IT, template <typename> class StackType>
void Frontier<IT, StackType>::updateTree(std::vector<Vertex<IT>> &vertexVector){      
    for (auto &V : tree) {
        V.TreeField=vertexVector[V.LabelField].TreeField;
        V.BridgeField=vertexVector[V.LabelField].BridgeField;
        V.ShoreField=vertexVector[V.LabelField].ShoreField;
        V.AgeField=vertexVector[V.LabelField].AgeField;
        V.LinkField=vertexVector[V.LabelField].LinkField;
        V.DirectParentField=vertexVector[V.LabelField].DirectParentField;
        V.GroupRootField=vertexVector[V.LabelField].GroupRootField;
        V.SizeField=vertexVector[V.LabelField].SizeField;
    }
}


// Constructor
template <typename IT, template <typename> class StackType>
bool Frontier<IT, StackType>::verifyTree(std::vector<Vertex<IT>> &vertexVector,
                            std::vector<std::atomic<IT>> &matching){ 
    bool valid = true;     
    for (auto &V : tree) {
        valid&=V.MatchField==matching[V.LabelField].load();
        if (!valid)
            break;
    }
    return valid;
}


// Constructor
template <typename IT, template <typename> class StackType>
void Frontier<IT, StackType>::updateVertexVector(std::vector<Vertex<IT>> &vertexVector){      
    for (auto &V : tree) {
        vertexVector[V.LabelField] = V;
    }
}

// Constructor
template <typename IT, template <typename> class StackType>
void Frontier<IT, StackType>::clear(){
    time = 0;
    TailOfAugmentingPathVertexIndex = -1;
    stack.clear();
    tree.clear();
}

#endif // FRONTIER_H
