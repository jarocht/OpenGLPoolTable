#ifndef TABLE_TOP_H
#define TABLE_TOP_H
#include <vector>
#include <glm/vec3.hpp>
#include "BufferObject.h"
#include <GL/gl.h>

using namespace std;

class TableTop{
private:
    const int N_POINTS = 8;
    GLuint v_buf, i_buf;
    vector<GLushort> index;
    vector<float> vert;

public:
    ~TableTop();
   // void addPoint(float x, float y, float z, vector<float> vec);
    void build();
    void render() const;

};
#endif
