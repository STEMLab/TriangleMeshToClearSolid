#include <string>
#include <detail/feature/type_conversion.h>

#include "features/Vertex.h"
#include "detail/io/JsonWriter.h"

using namespace std;

std::string Vertex::asJsonText()
{
    return TM2IN::detail::io::to_json(*this);
}

void Vertex::translate(double diff[]){
    for (int i = 0 ; i < 3 ; i++) coords[i] += diff[i];
}

void Vertex::translateTo(vector<double> dest){
    for (int i = 0 ; i < 3; i++) coords[i] = dest[i];
}


Kernel::Point_3 Vertex::CGAL_point() {
    return TM2IN::detail::feature::to_CGAL_Point_3(*this);
}


Vertex::Vertex(double px, double py, double pz) {
    coords[0] = CGAL::to_double(px);
    coords[1] = CGAL::to_double(py);
    coords[2] = CGAL::to_double(pz);
}

Vertex::Vertex(Vertex &vt) {
    coords[0] = vt.x();
    coords[1] = vt.y();
    coords[2] = vt.z();
}

void Vertex::setCoords(double x, double y, double z) {
    coords[0] = x;
    coords[1] = y;
    coords[2] = z;
}

std::ostream &operator<<(std::ostream &ou, Vertex *pVertex) {
    ou << pVertex->asJsonText() << std::endl;
    return ou;
}
