//
// Created by dongmin on 18. 7. 19.
//

#ifndef TM2IN_TYPE_CONVERSION_H
#define TM2IN_TYPE_CONVERSION_H

#include "features/Vertex.h"
#include "features/Triangle.h"
#include "detail/cgal_config.h"

namespace TM2IN{
    namespace detail{
        namespace feature{
            Kernel::Point_3 to_CGAL_Point_3(Vertex& vt);
            Kernel::Triangle_3 to_CGAL_Triangle_3(Triangle& tri);
            Segment_3 to_CGAL_Segment_3(HalfEdge* he);

        }
    }
}

#endif //TM2IN_TYPE_CONVERSION_H
