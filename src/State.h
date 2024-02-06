#ifndef STATE_H
#define STATE_H
#include <vector>
#include "Vertex.h"

template <typename IT>
class State  {
public:    
    State();

    State(size_t N);

    // Other member functions...
    std::vector<Vertex<IT>> vertexVector;
    std::vector<IT> stack;
    std::vector<IT> tree;
};
// Constructor
template <typename IT>
State<IT>::State(){
}
// Constructor
template <typename IT>
State<IT>::State(size_t N): vertexVector(N), tree(N), stack(N){
}

#endif