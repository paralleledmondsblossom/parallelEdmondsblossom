#ifndef BLOSSOM_H
#define BLOSSOM_H

#include "Vertex.h"
#include "Stack.h"
#include "DSU2.h"

class Blossom {
    public:
        // Static method to find the root of a vertex
        template <typename IT>
        static Vertex<IT>* Base(Vertex<IT>* x);
        template <typename IT>
        static Vertex<IT>* Root(Vertex<IT>* x);
        // Static method to find the root of a vertex
        template <typename IT, typename VT>
        static void Shrink(const Graph<IT, VT>& graph, 
                            const IT stackEdge, 
                            DisjointSetUnion<IT> &dsu,
                            std::vector<Vertex<IT>> & vertexVector, 
                            std::vector<IT> &stack);

        template <typename IT, typename VT, template <typename> class StackType=std::vector>
        static bool Shrink_Concurrent(const Graph<IT, VT>& graph, 
                            const IT stackEdge, 
                            std::vector<Vertex<IT>> & vertexVector, 
                            StackType<IT> &stack);

        template <typename IT, typename VT>
        static void Shrink(const Graph<IT, VT>& graph, 
                            const IT stackEdge, 
                            std::vector<Vertex<IT>> & vertexVector, 
                            std::vector<IT> &stack);
    private:

        // Helper function for path compression
        template <typename IT>
        static Vertex<IT>* FindSet(Vertex<IT>* x);

        // Static method for Union-Find with path compression
        template <typename IT>
        static Vertex<IT>* SetUnion(Vertex<IT>* x, Vertex<IT>* y);
};

template <typename IT>
Vertex<IT>* Blossom::Root(Vertex<IT>* x) {
    return FindSet(x);
}

template <typename IT>
Vertex<IT>* Blossom::Base(Vertex<IT>* x) {
    return FindSet(x)->BaseField;
}

template <typename IT, typename VT>
void Blossom::Shrink(const Graph<IT, VT>& graph, 
                    const IT stackEdge, 
                    DisjointSetUnion<IT> &dsu,
                    std::vector<Vertex<IT>> & vertexVector, 
                    std::vector<IT> &stack){
    // V,W
    IT EdgeFromVertexID,EdgeToVertexID;
    Vertex<IT> *EdgeToVertex;
    // A,B
    IT FromBaseID,ToBaseID,OriginalBaseID;
    Vertex<IT> *FromBase,*ToBase;
    // E
    IT nextEdge;

    nextEdge = stackEdge;
    // V = EdgeFrom(E);
    EdgeFromVertexID = Graph<IT,VT>::EdgeFrom(graph,nextEdge);
    // W = EdgeTo(E);
    EdgeToVertexID = Graph<IT,VT>::EdgeTo(graph,nextEdge);
    // B = Base(X);
    #ifndef NDEBUG
    FromBaseID = dsu[EdgeFromVertexID];
    auto FromBaseIDTest = DisjointSetUnionHelper<IT>::getBase(EdgeFromVertexID,vertexVector);  
    assert(FromBaseID==FromBaseIDTest);
    #else
    FromBaseID = DisjointSetUnionHelper<IT>::getBase(EdgeFromVertexID,vertexVector);  
    #endif

    FromBase = &vertexVector[FromBaseID];

    // A = Base(Y);
    #ifndef NDEBUG
    ToBaseID = dsu[EdgeToVertexID];
    auto ToBaseIDTest = DisjointSetUnionHelper<IT>::getBase(EdgeToVertexID,vertexVector);  
    assert(ToBaseID==ToBaseIDTest);
    #else
    ToBaseID = DisjointSetUnionHelper<IT>::getBase(EdgeToVertexID,vertexVector);  
    #endif

    ToBase = &vertexVector[ToBaseID];


    // if (Age(A) > Age(B))
    if(ToBase->AgeField > FromBase->AgeField){
        std::swap(FromBaseID,ToBaseID);
        std::swap(EdgeFromVertexID,EdgeToVertexID);
    }

    /*
    * Walk up the alternating tree from vertex V to vertex A, shrinking
    * the blossoms into a superblossom.  Edges incident to the odd vertices
    * on the path from V to A are pushed onto stack S, to later search from.
    */
    bool Found = false;
    // while (B != A)
    OriginalBaseID = ToBaseID;
    while(FromBaseID!=OriginalBaseID){
        IT matchedEdge, treeEdge;
        // M = Match(B);
        matchedEdge = graph.GetMatchField(FromBaseID);

        // W = Other(M, B);
        EdgeToVertexID = Graph<IT,VT>::Other(graph,matchedEdge,FromBaseID);
        EdgeToVertex = &vertexVector[EdgeToVertexID];
        
        // Bridge(W) = E;
        EdgeToVertex->BridgeField = nextEdge;

        // Shore(W) = V;
        EdgeToVertex->ShoreField = EdgeFromVertexID;

        // T = Tree(W);
        treeEdge = EdgeToVertex->TreeField;
        if (treeEdge < 0){
            printf("MASSIVE ERROR!!!\n");
            exit(1);
        }
        if (!Found){
            Found = Graph<IT,VT>::pushEdgesOntoStack(graph,vertexVector,EdgeToVertexID,stack,matchedEdge,treeEdge);
        }

        // Little unsure of this logic.
        // Y = Blossom(W);
        #ifndef NDEBUG
        ToBaseID = dsu[EdgeToVertexID];
        ToBaseIDTest = DisjointSetUnionHelper<IT>::getBase(EdgeToVertexID,vertexVector);  
        assert(ToBaseID==ToBaseIDTest);
        #else
        ToBaseID = DisjointSetUnionHelper<IT>::getBase(EdgeToVertexID,vertexVector);  
        #endif


        // X = SetUnion(Y, X);
        #ifndef NDEBUG
        dsu.linkTo(FromBaseID,ToBaseID);
        DisjointSetUnionHelper<IT>::linkTo(FromBaseID,ToBaseID,vertexVector);  
        #else
        DisjointSetUnionHelper<IT>::linkTo(FromBaseID,ToBaseID,vertexVector);  
        #endif
        // E = T;
        nextEdge = treeEdge;
        // V = Other(E, W);
        EdgeFromVertexID = Graph<IT,VT>::Other(graph,nextEdge,ToBaseID);
        // Y = Blossom(V);
        // X = SetUnion(Y, X);
        #ifndef NDEBUG
        dsu.linkTo(ToBaseID,dsu[EdgeFromVertexID]);
        DisjointSetUnionHelper<IT>::linkTo(ToBaseID,DisjointSetUnionHelper<IT>::getBase(EdgeFromVertexID,vertexVector),vertexVector);  
        #else
        DisjointSetUnionHelper<IT>::linkTo(ToBaseID,DisjointSetUnionHelper<IT>::getBase(EdgeFromVertexID,vertexVector),vertexVector);  
        #endif

        // B = Base(X);

        #ifndef NDEBUG
        FromBaseID=dsu[EdgeFromVertexID];
        FromBaseIDTest = DisjointSetUnionHelper<IT>::getBase(EdgeFromVertexID,vertexVector);
        assert(FromBaseID==FromBaseIDTest); 
        #else
        FromBaseID = DisjointSetUnionHelper<IT>::getBase(EdgeFromVertexID,vertexVector);
        #endif
    }
}


template <typename IT, typename VT>
void Blossom::Shrink(const Graph<IT, VT>& graph, 
                    const IT stackEdge, 
                    std::vector<Vertex<IT>> & vertexVector, 
                    std::vector<IT> &stack){
    // V,W
    IT EdgeFromVertexID,EdgeToVertexID;
    Vertex<IT> *EdgeToVertex;
    // A,B
    IT FromBaseID,ToBaseID,OriginalBaseID;
    Vertex<IT> *FromBase,*ToBase;
    // E
    IT nextEdge;

    nextEdge = stackEdge;
    // V = EdgeFrom(E);
    EdgeFromVertexID = Graph<IT,VT>::EdgeFrom(graph,nextEdge);
    // W = EdgeTo(E);
    EdgeToVertexID = Graph<IT,VT>::EdgeTo(graph,nextEdge);
    // B = Base(X);
    FromBaseID = DisjointSetUnionHelper<IT>::getBase(EdgeFromVertexID,vertexVector);  

    FromBase = &vertexVector[FromBaseID];

    // A = Base(Y);
    ToBaseID = DisjointSetUnionHelper<IT>::getBase(EdgeToVertexID,vertexVector);  

    ToBase = &vertexVector[ToBaseID];


    // if (Age(A) > Age(B))
    if(ToBase->AgeField > FromBase->AgeField){
        std::swap(FromBaseID,ToBaseID);
        std::swap(EdgeFromVertexID,EdgeToVertexID);
    }

    /*
    * Walk up the alternating tree from vertex V to vertex A, shrinking
    * the blossoms into a superblossom.  Edges incident to the odd vertices
    * on the path from V to A are pushed onto stack S, to later search from.
    */
    bool Found = false;
    // while (B != A)
    OriginalBaseID = ToBaseID;
    while(FromBaseID!=OriginalBaseID){
        IT matchedEdge, treeEdge;
        // M = Match(B);
        matchedEdge = graph.GetMatchField(FromBaseID);

        // W = Other(M, B);
        EdgeToVertexID = Graph<IT,VT>::Other(graph,matchedEdge,FromBaseID);
        EdgeToVertex = &vertexVector[EdgeToVertexID];
        
        // Bridge(W) = E;
        EdgeToVertex->BridgeField = nextEdge;

        // Shore(W) = V;
        EdgeToVertex->ShoreField = EdgeFromVertexID;

        // T = Tree(W);
        treeEdge = EdgeToVertex->TreeField;
        if (treeEdge < 0){
            printf("MASSIVE ERROR!!!\n");
            exit(1);
        }
        if (!Found){
            Found = Graph<IT,VT>::pushEdgesOntoStack(graph,vertexVector,EdgeToVertexID,stack,matchedEdge,treeEdge);
        }

        // Little unsure of this logic.
        // Y = Blossom(W);
        ToBaseID = DisjointSetUnionHelper<IT>::getBase(EdgeToVertexID,vertexVector);  

        // X = SetUnion(Y, X);
        DisjointSetUnionHelper<IT>::linkTo(FromBaseID,ToBaseID,vertexVector);  
        // E = T;
        nextEdge = treeEdge;
        // V = Other(E, W);
        EdgeFromVertexID = Graph<IT,VT>::Other(graph,nextEdge,ToBaseID);
        // Y = Blossom(V);
        // X = SetUnion(Y, X);
        DisjointSetUnionHelper<IT>::linkTo(ToBaseID,DisjointSetUnionHelper<IT>::getBase(EdgeFromVertexID,vertexVector),vertexVector);  

        // B = Base(X);
        FromBaseID = DisjointSetUnionHelper<IT>::getBase(EdgeFromVertexID,vertexVector);
    }
}


template <typename IT, typename VT, template <typename> class StackType>
bool Blossom::Shrink_Concurrent(const Graph<IT, VT>& graph, 
                    const IT stackEdge, 
                    std::vector<Vertex<IT>> & vertexVector, 
                    StackType<IT> &stack){
    // V,W
    IT EdgeFromVertexID,EdgeToVertexID;
    Vertex<IT> *EdgeToVertex;
    // A,B
    IT FromBaseID,ToBaseID,OriginalBaseID;
    Vertex<IT> *FromBase,*ToBase;
    // E
    IT nextEdge;

    nextEdge = stackEdge;
    // V = EdgeFrom(E);
    EdgeFromVertexID = Graph<IT,VT>::EdgeFrom(graph,nextEdge);
    // W = EdgeTo(E);
    EdgeToVertexID = Graph<IT,VT>::EdgeTo(graph,nextEdge);
    // B = Base(X);
    FromBaseID = DisjointSetUnionHelper<IT>::getBase(EdgeFromVertexID,vertexVector);  
    FromBase = &vertexVector[FromBaseID];

    // A = Base(Y);
    ToBaseID = DisjointSetUnionHelper<IT>::getBase(EdgeToVertexID,vertexVector);  
    ToBase = &vertexVector[ToBaseID];


    // if (Age(A) > Age(B))
    if(ToBase->AgeField > FromBase->AgeField){
        std::swap(FromBaseID,ToBaseID);
        std::swap(EdgeFromVertexID,EdgeToVertexID);
    }

    /*
    * Walk up the alternating tree from vertex V to vertex A, shrinking
    * the blossoms into a superblossom.  Edges incident to the odd vertices
    * on the path from V to A are pushed onto stack S, to later search from.
    */
    bool Found = false;
    // while (B != A)
    OriginalBaseID = ToBaseID;
    while(FromBaseID!=OriginalBaseID){
        IT matchedEdge, treeEdge;
        // M = Match(B);
        matchedEdge = vertexVector[FromBaseID].MatchField;

        // W = Other(M, B);
        EdgeToVertexID = Graph<IT,VT>::Other(graph,matchedEdge,FromBaseID);
        EdgeToVertex = &vertexVector[EdgeToVertexID];
        
        // Bridge(W) = E;
        EdgeToVertex->BridgeField = nextEdge;

        // Shore(W) = V;
        EdgeToVertex->ShoreField = EdgeFromVertexID;

        // T = Tree(W);
        treeEdge = EdgeToVertex->TreeField;
        if (treeEdge < 0){
            return false;
        }
        if (!Found){
            Found = Graph<IT,VT>::pushEdgesOntoStack(graph,vertexVector,EdgeToVertexID,stack,matchedEdge,treeEdge);
        }

        // Little unsure of this logic.
        // Y = Blossom(W);
        ToBaseID = DisjointSetUnionHelper<IT>::getBase(EdgeToVertexID,vertexVector);  

        // X = SetUnion(Y, X);
        DisjointSetUnionHelper<IT>::linkTo(FromBaseID,ToBaseID,vertexVector);  
        // E = T;
        nextEdge = treeEdge;
        // V = Other(E, W);
        EdgeFromVertexID = Graph<IT,VT>::Other(graph,nextEdge,ToBaseID);
        // Y = Blossom(V);
        // X = SetUnion(Y, X);
        DisjointSetUnionHelper<IT>::linkTo(ToBaseID,DisjointSetUnionHelper<IT>::getBase(EdgeFromVertexID,vertexVector),vertexVector);  

        // B = Base(X);
        FromBaseID = DisjointSetUnionHelper<IT>::getBase(EdgeFromVertexID,vertexVector);
    }
    return true;
}

template <typename IT>
Vertex<IT>* Blossom::SetUnion(Vertex<IT>* x, Vertex<IT>* y) {
    // E = SetFind(E);
    x = FindSet(x);
    // F = SetFind(F);
    y = FindSet(y);

    // Check if they are already in the same set
    // if (E == F)
    if (x == y) {
        // return E;
        return x;
    }

    // D = E->Label;
    Vertex<IT>* D = x->BaseField;
    // Perform Union by rank
    // if (E->Rank < F->Rank)
    if (x->RankField < y->RankField) {
        // E->Up = F;
        x->ParentField = y;
        // F->Label = D;
        y->BaseField = D;
        return y;
    // else if (E->Rank > F->Rank)
    } else if (x->RankField > y->RankField) {
        // F->Up = E;
        y->ParentField = x;
        return x;
    } else {
        // If ranks are the same, arbitrarily choose one as the parent and increment its rank
        // E->Rank += 1;
        x->RankField++;
        // F->Up = E;
        y->ParentField = x;
        return x;
    }
}

// Helper function for path compression
template <typename IT>
Vertex<IT>* Blossom::FindSet(Vertex<IT>* x) {
    if (x != x->ParentField) {
        x->ParentField = FindSet(x->ParentField); // Path compression
    }
    return x->ParentField;
}

#endif // BLOSSOM_H
