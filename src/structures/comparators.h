#ifndef COMPARATORS_H
#define COMPARATORS_H

#include "structures/nodeData.h"
#include "graph/vertex.h"

// normal order
class Comparators
{
public:
    inline bool operator()(const edgeData &t1, const edgeData &t2) const
    {
        return (t1.vertexId < t2.vertexId);
    }

    inline bool operator()(const Vertex* t1, const Vertex* t2) const
    {
        return (t1->id() < t2->id());
    }
};

#endif // COMPARATORS_H
