#define __DEBUG__
#include "TriangulationConverter.h"
#include <compute/SurfacesListComputation.h>
#include <fileio/export/MeshExporter.h>
#include "compute/SurfaceComputation.h"
#include "cgal/Polygon_mesh_processing.h"
#include "Converter.h"

using namespace std;


int TriangulationConverter::pre_process() {
    if (this->initTriangleMesh()) return -1;
//    if (this->handleOpenTriangleMesh()) return -1;
//    if (this->partitionTriangleMeshByComponent()) return -1;
//    if (this->remainStructure()) return -1;
    if (this->convertTriangleMeshToSpace()) return -1;
    return 0;
}

int TriangulationConverter::constructSpace() {
    assert (this->spaceList.size() != 0);

    char doNotMerge;
    cout << "keep Triangle without merging? (y/n)" << endl;

    // cin >> doNotMerge;
    doNotMerge = 'n';
    cout << doNotMerge << endl;

    if (checkAnswer(doNotMerge, 'y')){
        return 0;
    }
    if (this->mergeSurfaces()) return -1;

    if (this->simplifyShareEdge()) return -1;

    this->makeSurfaceGraph();

    this->checkSelfIntersection();

    cout << "\n\nre-triangulation" << endl;
    if (this->triangulation()) return -1;

    return 0;
}


int TriangulationConverter::finish() {
    this->tagID();
    this->exportSpace();



    return 0;
}


void TriangulationConverter::makeSurfaceGraph() {
    for (int i = 0 ; i < spaceList.size() ; i++){
        cout << "\n\n" << i << "th graph" << endl;
        spaceList[i]->surfaceGraph = new SurfaceGraph();
        spaceList[i]->surfaceGraph->makeAdjacentGraph(spaceList[i]->surfacesList);
        if (spaceList[i]->surfaceGraph->isClosedSurface()){
            cout << "this is closed" << endl;
        }
        else{
            cout << "not closed" << endl;
        }
        cout << "------------\n" << endl;
    }
}


int TriangulationConverter::triangulation() {
    for (ull it = 0 ; it < this->spaceList.size() ; it++) {
        cout << "space : " << it << endl;
        Space *space = this->spaceList[it];
        space->triangulateSurfaces();
    }
    return 0;
}


