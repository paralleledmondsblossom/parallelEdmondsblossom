#ifndef GRAPH2_H
#define GRAPH2_H
#include <filesystem>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>
#include <fast_matrix_market/fast_matrix_market.hpp>
#include <chrono>
#include <signal.h>

using namespace std::chrono;
namespace fmm = fast_matrix_market;
#include "FileReader.h"
#include "Vertex.h"
#include "Stack.h"
#include <list>

template <typename IT, typename VT>
class Graph {
public:
    //Graph(const std::filesystem::path& in_path);
    Graph(  std::vector<IT> &_indptr,
            std::vector<IT> &_indices,
            std::vector<IT> &_original_rows,
            std::vector<IT> &_original_cols,
            std::vector<VT> &_original_vals,
            size_t N,
            size_t M);
    /*
    Graph(  std::vector<IT> &indptr,
            std::vector<IT> &indices,
            std::vector<IT> &original_rows,
            std::vector<IT> &original_cols,
            std::vector<IT> &original_vals,
            std::size_t N,
            std::size_t M);*/
    size_t getN() const;
    size_t getM() const;
    bool IsMatched(size_t index) const;
    IT GetMatchField(size_t index) const;
    void SetMatchField(size_t index,IT edge);
    template <template <typename> class StackType = std::vector>
    static bool pushEdgesOntoStack(const Graph<IT, VT>& graph, 
                                        std::vector<Vertex<IT>> & vertexVector, 
                                        IT V_index, 
                                        StackType<IT> &stack,
                                        IT optionalEdge1=-1,
                                        IT optionalEdge2=-1);
    static inline IT Other(const Graph<IT, VT>& graph, const IT edgeIndex, const IT vertexId);
    static IT EdgeFrom(const Graph<IT, VT>& graph, const IT edgeIndex);
    static IT EdgeTo(const Graph<IT, VT>& graph, const IT edgeIndex);
    // Other member functions...
    const std::vector<IT> &indptr;
    const std::vector<IT> &indices;
    const std::vector<IT> &original_rows;
    const std::vector<IT> &original_cols;
    const std::vector<VT> &original_vals;
    const size_t N,M;
    std::vector<std::atomic<IT>> matching;

private:    
    void read_file(const std::filesystem::path& in_path);
    void generateCSR(const std::vector<IT>& rows, const std::vector<IT>& columns, IT numVertices, std::vector<IT>& rowPtr, std::vector<IT>& colIndex);
};

// Constructor
/*
template <typename IT, typename VT>
Graph<IT,VT>::Graph(const std::filesystem::path& in_path) {
    read_file(in_path);
}
*/

template <typename IT, typename VT>
Graph<IT,VT>::Graph(std::vector<IT> &_indptr,
                    std::vector<IT> &_indices,
                    std::vector<IT> &_original_rows,
                    std::vector<IT> &_original_cols,
                    std::vector<VT> &_original_vals,
                    size_t _N,
                    size_t _M):
                    indptr(_indptr),
                    indices(_indices),
                    original_rows(_original_rows),
                    original_cols(_original_cols),
                    original_vals(_original_vals),
                    N(_N),
                    M(_M),
                    matching(_N) {
    // Assign all elements in the vector to false
    for (auto& atomicBool : matching) {
        atomicBool.store(-1);
    }
}


// Constructor
/*
template <typename IT, typename VT>
Graph<IT,VT>::Graph(std::vector<IT> &_indptr,
            std::vector<IT> &indices,
            std::vector<IT> &original_rows,
            std::vector<IT> &original_cols,
            std::vector<IT> &original_vals,
            std::size_t N,
            std::size_t M) :
            matching(N) {
    //indptr = std::move(_indptr);
}
*/
template <typename IT, typename VT>
size_t Graph<IT,VT>::getN() const{
    return N;
}

template <typename IT, typename VT>
size_t Graph<IT,VT>::getM() const{
    return M;
}


template <typename IT, typename VT>
bool Graph<IT,VT>::IsMatched(size_t index) const{
    return matching[index].load()>-1;
    //return matching[index]>-1;
}

template <typename IT, typename VT>
IT Graph<IT,VT>::GetMatchField(size_t index) const{
    return matching[index].load();
    //return matching[index];
}

template <typename IT, typename VT>
void Graph<IT,VT>::SetMatchField(size_t index,IT edge){
    matching[index].store(edge);
    //matching[index]=edge;
}

// Constructor
template <typename IT, typename VT>
void Graph<IT,VT>::read_file(const std::filesystem::path& in_path) {
    fmm::matrix_market_header header;
    fmm::read_options options;
    options.generalize_symmetry = false;
    auto f_to_el_start = high_resolution_clock::now();
    std::ifstream f(in_path);
    fmm::read_matrix_market_triplet(f, header, original_rows, original_cols, original_vals, options);
    auto f_to_el_end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(f_to_el_end - f_to_el_start);
    std::cout << "MTX to EdgeList conversion time: "<< duration.count() << " milliseconds" << std::endl;
    auto el_to_csr_start = high_resolution_clock::now();
    generateCSR(original_rows, original_cols, header.ncols, indptr, indices);
    auto el_to_csr_end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(el_to_csr_end - el_to_csr_start);
    // To get the value of duration use the count()
    // member function on the duration object
    std::cout << "EdgeList to CSR conversion time: "<< duration.count() << " milliseconds" << std::endl;
    std::cout << "Undirected general graph |V|: "<< indptr.size()-1 << " |E|: " << indices.size()/2 << std::endl;
    N = header.ncols;
    M = header.nnz;
}

// Constructor
template <typename IT, typename VT>
void Graph<IT,VT>::generateCSR(const std::vector<IT>& rows, const std::vector<IT>& columns, IT numVertices, std::vector<IT>& rowPtr, std::vector<IT>& colIndex) {
    rowPtr.resize(numVertices + 1, 0);
    colIndex.resize(2*rows.size());
    #pragma omp parallel for
    for (size_t i = 0; i < rows.size(); ++i) {
        #pragma omp atomic
        rowPtr[rows[i] + 1]++;
        #pragma omp atomic
        rowPtr[columns[i] + 1]++;
    }

    for (size_t i = 1; i < rowPtr.size(); ++i) {
        rowPtr[i] += rowPtr[i - 1];
    }

    auto rowPtr_duplicate = rowPtr;
    /*
    #pragma omp parallel for
    for (size_t i = 0; i < rows.size(); ++i) {
        IT source = rows[i];
        IT destination = columns[i];

        IT index;
        #pragma omp atomic capture
        index = rowPtr_duplicate[source]++;
        colIndex[index] = i;
        #pragma omp atomic capture
        index = rowPtr_duplicate[destination]++;
        colIndex[index] = i;
    }
    */

    //#pragma omp parallel for
    for (size_t i = 0; i < rows.size(); ++i) {
        IT source = rows[i];
        IT destination = columns[i];

        IT index;
        //#pragma omp atomic capture
        index = rowPtr_duplicate[source]++;
        colIndex[index] = i;
        //#pragma omp atomic capture
        index = rowPtr_duplicate[destination]++;
        colIndex[index] = i;
    }
}


template <typename IT, typename VT>
template <template <typename> class StackType>
bool Graph<IT,VT>::pushEdgesOntoStack(const Graph<IT, VT>& graph, 
                                    std::vector<Vertex<IT>> & vertexVector, 
                                    IT V_index, 
                                    StackType<IT> &stack,
                                    IT optionalEdge1,
                                    IT optionalEdge2){
    IT nextVertexIndex;
    Vertex<IT>* nextVertex;
    // Push edges onto stack, breaking if that stackEdge is a solution.
    for (IT start = graph.indptr[V_index]; start < graph.indptr[V_index + 1]; ++start) {
        // For blossom contraction, need to skip repushing the matched & tree edges
        if (graph.indices[start] == optionalEdge1 || graph.indices[start] == optionalEdge2)
            continue;
        stack.push_back(graph.indices[start]);

        nextVertexIndex = Graph<IT, VT>::Other(graph, graph.indices[start], V_index);

        nextVertex = &vertexVector[nextVertexIndex];
        if (!nextVertex->IsReached() && !graph.IsMatched(nextVertexIndex))
            return true;
    }
    return false;
}
/*
// Static method to find the other endpoint of an edge
template <typename IT, typename VT>
IT Graph<IT,VT>::Other(const Graph<IT, VT>& graph, const IT edgeIndex, const IT vertexId) {
    if (edgeIndex < 0 || edgeIndex >= static_cast<IT>(graph.original_rows.size())) {
        // Handle invalid edge index
        std::cerr << "Error: Invalid edge index " << edgeIndex<< " vertexId " << vertexId << " graph.original_rows.size() " << graph.original_rows.size() << std::endl;
        raise(SIGSEGV);
        return -1; // or throw an exception, depending on your error handling strategy
    }
    IT source = graph.original_rows[edgeIndex];
    IT destination = graph.original_cols[edgeIndex];
    if (vertexId == source) {
        return destination;
    } else {
        return source;
    }
}
*/
// Static method to find the other endpoint of an edge
template <typename IT, typename VT>
inline IT Graph<IT,VT>::Other(const Graph<IT, VT>& graph, const IT edgeIndex, const IT vertexId) {
    // Using XOR to find the value that doesn't equal the third
    return graph.original_rows[edgeIndex] ^ graph.original_cols[edgeIndex] ^ vertexId;
}

// Static method to find the other endpoint of an edge
template <typename IT, typename VT>
IT Graph<IT,VT>::EdgeFrom(const Graph<IT, VT>& graph, const IT edgeIndex) {
    IT source = graph.original_rows[edgeIndex];
    return source;
}

// Static method to find the other endpoint of an edge
template <typename IT, typename VT>
IT Graph<IT,VT>::EdgeTo(const Graph<IT, VT>& graph, const IT edgeIndex) {
    IT destination = graph.original_cols[edgeIndex];
    return destination;
}

#endif // GRAPH2_H
