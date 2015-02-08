#ifndef DULIMARTA_LOCKNUT_H
#define DULIMARTA_LOCKNUT_H

#include "BufferObject.h"
class HexNut : public BufferObject {
private:
    const int SEGMENT_SUBDIV = 3;
    const float OUTER_RAD = 1.0;
    const float INNER_RAD = 0.5;
    const float THICKNESS = 0.4; /* half thick */
    int top_count, pts_count;

protected:
public:
    void build (void*);
    void render(bool) const;
};
#endif