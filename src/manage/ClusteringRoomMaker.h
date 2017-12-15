#ifndef CLUSTERINGROOMMAKER_H_INCLUDED
#define CLUSTERINGROOMMAKER_H_INCLUDED

#include "RoomMaker.h"

class ClusteringRoomMaker : public RoomMaker{
public:
    int pre_process(double degree);
    int constructSpace(CombineParameter* );
    int makeSimpleSpaces(SpaceMaker* sm);

};

#endif // CLUSTERINGROOMMAKER_H_INCLUDED
