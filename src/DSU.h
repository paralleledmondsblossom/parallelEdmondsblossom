#include <vector>
#include <algorithm>
#include <cassert>
#ifndef DSU_H
#define DSU_H
template<typename T>
class DisjointSetUnion {
public:
    void reset(T n);
    T find(T a);
    T operator[](const T& a);
    void linkTo(T a, T b);

//private:
    std::vector<T> link;
    std::vector<T> directParent;
    std::vector<T> size;
    std::vector<T> groupRoot;
};

template<typename T>
void DisjointSetUnion<T>::reset(T n) {
    link = std::vector<T>(n);
    size = std::vector<T>(n, 1);
    std::iota(link.begin(), link.end(), 0);
    groupRoot = link;
    directParent = std::vector<T>(n, -1);
}

template<typename T>
T DisjointSetUnion<T>::find(T a) {
    return link[a] = (a == link[a] ? a : find(link[a]));
}

template<typename T>
T DisjointSetUnion<T>::operator[](const T& a) {
    return groupRoot[find(a)];
}

template<typename T>
void DisjointSetUnion<T>::linkTo(T a, T b) {
    assert(directParent[a] == -1);
    assert(directParent[b] == -1);
    directParent[a] = b;
    a = find(a);
    b = find(b);
    T gr = groupRoot[b];
    assert(a != b);

    if (size[a] > size[b])
        std::swap(a, b);
    link[b] = a;
    size[a] += size[b];
    groupRoot[a] = gr;
}
#endif