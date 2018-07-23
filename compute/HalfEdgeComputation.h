//
// Created by dongmin on 18. 1. 19.
//

#ifndef TRIANGLEMESHTOCLEARSOLID_HALFEDGECOMPUTATION_H
#define TRIANGLEMESHTOCLEARSOLID_HALFEDGECOMPUTATION_H


#include "detail/cgal_config.h"
#include <features/HalfEdge.h>

class HalfEdgeComputation {
public:
    static Line_3 getCGALLine_3(HalfEdge *he);

    static void setParent(std::vector<HalfEdge *> edges, Surface *pSurface);
    static std::vector<Vertex*> getFirstVertexList(std::vector<HalfEdge*> halfEdges);
};


#endif //TRIANGLEMESHTOCLEARSOLID_HALFEDGECOMPUTATION_H
