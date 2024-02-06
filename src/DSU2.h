#ifndef DSU2_H
#define DSU2_H
#include <vector>
#include <cassert>
#include "Vertex.h"
template<typename IT>
class DisjointSetUnionHelper {
public:
    static void reset(IT n, std::vector<Vertex<IT>> & vv);
    static IT find(IT a, std::vector<Vertex<IT>> & vv);
    static IT getBase(const IT& a, std::vector<Vertex<IT>> & vv);
    static void linkTo(IT a, IT b, std::vector<Vertex<IT>> & vv);
};

template<typename IT>
void DisjointSetUnionHelper<IT>::reset(IT n, std::vector<Vertex<IT>> & vv) {
}

/*
template<typename IT>
IT DisjointSetUnionHelper<IT>::find(IT a) {
    return link[a] = (a == link[a] ? a : find(link[a]));
}
*/
template<typename IT>
IT DisjointSetUnionHelper<IT>::find(IT a, std::vector<Vertex<IT>> & vv) {
    // return link[a] = (a == link[a] ? a : find(link[a]));
    return vv[a].LinkField = (a == vv[a].LinkField ? a : find(vv[a].LinkField,vv));
}

/*
template<typename IT>
IT DisjointSetUnionHelper<IT>::operator[](const IT& a) {
    return groupRoot[find(a)];
}
*/

template<typename IT>
IT DisjointSetUnionHelper<IT>::getBase(const IT& a, std::vector<Vertex<IT>> & vv) {
    return vv[find(a,vv)].GroupRootField;
}

/*
template<typename IT>
void DisjointSetUnionHelper<IT>::linkTo(IT a, IT b) {
    assert(directParent[a] == -1);
    assert(directParent[b] == -1);
    directParent[a] = b;
    a = find(a);
    b = find(b);
    IT gr = groupRoot[b];
    assert(a != b);

    if (size[a] > size[b])
        std::swap(a, b);
    link[b] = a;
    size[a] += size[b];
    groupRoot[a] = gr;
}
*/

template<typename IT>
void DisjointSetUnionHelper<IT>::linkTo(IT a, IT b, std::vector<Vertex<IT>> & vv) {
    // assert(directParent[a] == -1);
    assert(vv[a].DirectParentField == -1);
    // assert(directParent[b] == -1);
    assert(vv[b].DirectParentField == -1);
    // directParent[a] = b;
    vv[a].DirectParentField = b;
    // a = find(a);
    a = find(a,vv);
    // b = find(b);
    b = find(b,vv);
    // IT gr = groupRoot[b];
    IT gr = vv[b].GroupRootField;
    assert(a != b);

    // if (size[a] > size[b])
    if (vv[a].SizeField > vv[b].SizeField)
        std::swap(a, b);
    // link[b] = a;
    vv[b].LinkField = a;
    // size[a] += size[b];
    vv[a].SizeField += vv[b].SizeField;
    // groupRoot[a] = gr;
    vv[a].GroupRootField = gr;
}
#endif