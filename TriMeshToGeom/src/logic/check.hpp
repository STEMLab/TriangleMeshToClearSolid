//
//  check.hpp
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 9..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//

#ifndef check_hpp
#define check_hpp

#include <stdio.h>
#include <vector>

#include "model/vertex.h"
#include "logic/CGALCalculation.h"

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Vector_3.h>
#include <CGAL/Point_3.h>
#include <CGAL/Kernel/global_functions.h>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_3 Point_3;
typedef Kernel::Vector_3 Vector_3;

class Checker{
private:



public:
    double threshold_vertex = 0.0;
    double ori_degree = 0.0;
    double collinear_degree = 0.1;

    Checker(double vertex, double degree){
        threshold_vertex = vertex;
        ori_degree = degree;
    }

    void setCollinearDegree(double p){
        collinear_degree = p;
    }
    void setSameOrientationDegree(double s){
        ori_degree = s;
    }
    bool isExistDuplication(std::vector<Vertex*> vertex);

    bool isSameX(Vertex* a, Vertex* b);
    bool isSameY(Vertex* a, Vertex* b);
    bool isSameZ(Vertex* a, Vertex* b);

    bool isSameX(Vertex* a, Vertex* b, Vertex* c);
    bool isSameY(Vertex* a, Vertex* b, Vertex* c);
    bool isSameZ(Vertex* a, Vertex* b, Vertex* c);

    bool isSameVertex(Vertex& v1, Vertex& v2);
    bool isSameVertex(Vertex* v1, Vertex* v2);

    bool isSameOrientation(Vector_3& nv1, Vector_3& nv2);
    bool isSameOrientation(Vertex* origin, Vertex* v1, Vertex* v2);
    bool isSimilarOrientation(Vector_3& nv1, Vector_3& nv2);
    bool isSamePlanar(Vector_3& big, Vector_3& small, double degree);
    bool isSamePlanar(Vertex* origin, Vertex* v1, Vertex* v2);

    bool isCollinear(Vertex* origin, Vertex* v1, Vertex* v2);

    int compare_vertex(Vertex* v1, Vertex* v2);
};
#endif /* check_hpp */
