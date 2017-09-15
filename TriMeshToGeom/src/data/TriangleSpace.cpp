#include "data/TriangleSpace.h"

#include <cstdio>

TriangleSpace::TriangleSpace(string pname, Checker* check)
{
    checker = check;
    name = pname;
}

TriangleSpace::~TriangleSpace()
{
    //dtor
}

void TriangleSpace::pushTriangle(Triangle tri){
    this->triangles.push_back(tri);
}

int TriangleSpace::makeSurfacesGreedy(){
    int combined_count = 0;
    vector<Triangle*> p_triangles;
    ull size = this->triangles.size();
    for (ull i = 0 ; i < size; i++){
        p_triangles.push_back(&this->triangles[i]);
    }

    bool* checked = (bool*)malloc(sizeof(bool) * size);
    std::fill(checked, checked + size, false);

    vector<Surface*> c_list = makeSurfacesInList(p_triangles, checked, combined_count);
    this->polygon_list.insert(this->polygon_list.end(), c_list.begin(), c_list.end());

    free(checked);

    this->triangles.clear();

    cout << "\ndone make Surfaces" << endl;
    return 0;
}

int TriangleSpace::makeSurfacesNotJoin(){
    vector<Triangle*> p_triangles;
    ull size = this->triangles.size();
    for (ull i = 0 ; i < size; i++){
        p_triangles.push_back(&this->triangles[i]);
    }

    vector<Surface*> c_list;
    for (ull index = 0 ; index < size ; index++){
        Surface* newcp = new Surface(p_triangles[index]);
        c_list.push_back(newcp);
    }
    this->polygon_list.insert(this->polygon_list.end(), c_list.begin(), c_list.end());
    this->triangles.clear();

    cout << "\ndone make Surfaces" << endl;
    return 0;
}

void TriangleSpace::printProcess(ull index, ull size){
    cout << "\r==========" << (int)((double)index/(double)size * 100) <<"% ========";
}


vector<Surface*> TriangleSpace::makeSurfacesInList(vector<Triangle*>& tri_list, bool* checked, int& combined_count)
{
    vector<Surface*> result_list;
    ull size = tri_list.size();
    cout << "\n number : " << size << endl;

    for (ull index = 0 ; index < size; index++)
    {
        this->printProcess(combined_count, this->triangles.size());
        if (checked[index])
        {
            continue;
        }
        checked[index] = true ;
        Surface* newcp = new Surface(tri_list[index]);

        ll count = -1 ;
        while (count != 0)
        {
            newcp = attachTriangle(tri_list, newcp, checked, count);
            if (newcp == NULL) break;
            if (combined_count % 250 == 1 )
            {
                cout << "\n------------ " << index << " -----------  size : " << size << endl;
                this->printProcess(combined_count, this->triangles.size());
            }
            combined_count += count;
        }
        if (newcp != NULL) result_list.push_back(newcp);
    }
    return result_list;
}


Surface* TriangleSpace::attachTriangle(vector<Triangle*> tri_list, Surface* cp, bool* checked, ll& count)
{
    count = 0;
    if (cp->av_normal == CGAL::NULL_VECTOR) return NULL;
    for (ull id = 0 ; id < tri_list.size() ; id++)
    {
        if (!checked[id])
        {
            //if (cp->attachTriangle(tri_list[id], checker))
            if (TriangleAttacher::attach(cp, tri_list[id], checker))
            {
                checked[id] = true;
                count++;
            }
        }
    }
    return cp;
}


int TriangleSpace::combineSurface(){
    cout << "Combine Surfaces" << endl;

    sort(this->polygon_list.begin(), this->polygon_list.end(), Surface::compareLength);
    ull p_size = this->polygon_list.size();
    bool* checked = (bool*)malloc(sizeof(bool) * p_size);
    std::fill(checked, checked + p_size, false);

    vector<Surface*> new_poly_list;
    int combined_count = 0;
    for (ull i = 0 ; i < this->polygon_list.size() ; i++)
    {
        if (checked[i]) continue;
        checked[i] = true;

        ll count = -1;
        Surface* newcp = new Surface(this->polygon_list[i]);
        while(count != 0){
            newcp = attachSurfaces(newcp, i+1, checked, count);
            if (newcp == NULL) break;
            this->printProcess(combined_count, this->polygon_list.size());
            combined_count += count;
        }
        if (newcp != NULL) new_poly_list.push_back(newcp);
    }

    freeSurfaces();
    this->polygon_list = new_poly_list;
    return 0;
}

Surface* TriangleSpace::attachSurfaces(Surface* cp, ull start, bool* checked, ll& count)
{
    count = 0;
    if (cp->av_normal == CGAL::NULL_VECTOR) return NULL;
    for (ull id = start ; id < this->polygon_list.size() ; id++)
    {
        if (!checked[id])
        {
            if (CleanPolygonMaker::combine(cp, this->polygon_list[id], checker))
            {
                checked[id] = true;
                count++;
            }
        }
    }
    return cp;
}

void TriangleSpace::tagID(){
    sort(this->polygon_list.begin(), this->polygon_list.end(), Surface::compareLength);
    for (ull i = 0 ; i < (ull)this->polygon_list.size() ; i++)
    {
        this->polygon_list[i]->sf_id = i;
    }
}

int TriangleSpace::simplifySegment(){
    cout << "\nsimplifySegment\n" << endl;

    ull p_size = this->polygon_list.size();

    for (ull i = 0 ; i < p_size - 1; i++)
    {
        for (ull j = 0 ; j < p_size ; j++)
        {
            if (i == j) continue;
            if (i ==0 && j == 2){
                debug();
            }
            CleanPolygonMaker::simplifyLineSegment(this->polygon_list[i], this->polygon_list[j] );
        }
    }
    return 0;
}

int TriangleSpace::handleDefect(){
    cout << "\n------------- handle Defect --------------\n" << endl;
    sort(this->polygon_list.begin(), this->polygon_list.end(), Surface::compareArea);
    for (vector<Surface*>::size_type i = 0 ; i < this->polygon_list.size(); )
    {

        this->polygon_list[i]->removeDuplication(this->checker);
        this->polygon_list[i]->setMBB();

        if (this->polygon_list[i]->isValid()){
            i++;
        }
        else{
            this->polygon_list.erase(this->polygon_list.begin() + i);
            cout << "Erase unvalid surface" << endl;
        }
    }
    return 0;
}

void TriangleSpace::freeSurfaces(){
    for (ull id = 0 ; id < this->polygon_list.size() ; id++)
    {
        delete(this->polygon_list[id]);
    }
    this->polygon_list.clear();
}

int TriangleSpace::match00(){
    cout << "\n------------- match00 --------------\n" << endl;


    updateMBB();
    //find surfaces whose normal is z direction.
    for (ull i = 0 ; i < (int)this->polygon_list.size() ; i++)
    {
        if (!this->polygon_list[i]->updateNormal(this->checker))
        {
            cout << "Cannot make Normal" << endl;
            exit(-1);
        }
        Vector_3 normal = this->polygon_list[i]->av_normal;
        if (fabs(normal.z()) > fabs(normal.x()) && fabs(normal.z()) > fabs(normal.y()))
        {
            this->polygon_list[i]->makeCoplanarParallelWithZ();
        }
    }

//
//    updateMBB();
//    double diff[3];
//    for (int i = 0 ; i < 3 ; i++){
//        diff[i] = -this->min_coords[i];
//    }
//
//    for (ull i = 0 ; i < (int)this->polygon_list.size() ; i++)
//    {
//        this->polygon_list[i]->translate(diff);
//    }
//
//    for (ull i = 0 ; i < this->vertex->size() ; i++){
//        this->vertex->at(i)->translate(diff);
//    }
    return 0;
}

void TriangleSpace::updateMBB(){
    for (int i = 0 ; i < 3 ; i++)
    {
        this->max_coords[i] = -1000000000;
        this->min_coords[i] = 1000000000;
    }

    for (ull i = 0 ; i < this->polygon_list.size() ; i++){
        Surface* cp = this->polygon_list[i];
        cp->setMBB();
        for (int j = 0 ; j < 3 ; j++){
            this->max_coords[j] = max(this->max_coords[j], cp->max_coords[j]);
            this->min_coords[j] = min(this->min_coords[j], cp->min_coords[j]);
        }
    }
}











int TriangleSpace::makeSurfacesBySeparation()
{
    // Separation by Normal
    vector<vector<Triangle*>> poly_set;
    poly_set = separateByNormal_6(this->triangles);
    if (poly_set.size() != 6 ) return -1;

    int combined_count = 0;
    for (unsigned int dir = 0 ; dir < 6 ; dir++)
    {
        ull c_size = poly_set[dir].size();
        bool* checked = (bool*)malloc(sizeof(bool) * c_size);
        std::fill(checked, checked + c_size, false);

        vector<Surface*> c_list = makeSurfacesInList(poly_set[dir], checked, combined_count);
        this->polygon_list.insert(this->polygon_list.end(), c_list.begin(), c_list.end());

        free(checked);
    }

    this->triangles.clear();

    cout << "\ndone make Surfaces" << endl;
    return 0;
}

int TriangleSpace::makeSurfacesByCandidator()
{
    ull size = this->triangles.size();
    bool* checked = (bool*)malloc(sizeof(bool) * size);
    std::fill(checked, checked + size, false);

    int combined_count = 0;
    for (ull index = 0 ; index < size; index++)
    {
        if (checked[index])
        {
            continue;
        }
        checked[index] = true ;

        Vector_3 pl_nv = this->triangles[index].getNormal();
        vector<Triangle*> candidates;
        for (ull index2 = 0 ; index2 < size ; index2++){
            if (checked[index2])
            {
                continue;
            }

            Vector_3 normal = this->triangles[index2].getNormal();
            if (checker->isSamePlanar(pl_nv, normal)){
                candidates.push_back(&this->triangles[index2]);
                //pl_nv = pl_nv + normal;
                checked[index2] = true ;
            }
        }

        ull c_size = candidates.size();
        bool* checked2 = (bool*)malloc(sizeof(bool) * c_size);
        std::fill(checked2, checked2 + c_size, false);

        vector<Surface*> c_list = makeSurfacesInList(candidates, checked2, combined_count);
        this->polygon_list.insert(this->polygon_list.end(), c_list.begin(), c_list.end());

        free(checked2);

        candidates.clear();
    }

    this->triangles.clear();

    cout << "\ndone make Surfaces" << endl;
    return 0;
}


vector<vector<Triangle*>> TriangleSpace::separateByNormal_6(vector<Triangle>& triangles)
{
    vector<vector<Triangle*>> ret(6, vector<Triangle*>());

    ull size = triangles.size();
    for (ull index = 0 ; index < size; index++){
        Vector_3 normal = triangles[index].getNormal();
        int type = CGALCalculation::findNormalType(normal);
        ret[type].push_back(&triangles[index]);
    }

    return ret;
}
