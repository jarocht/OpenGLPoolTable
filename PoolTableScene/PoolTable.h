#ifndef POOLTABLE_H
#define POOLTABLE_H

#define GLEW_STATIC
#include <vector>
#include <GL/glew.h>

using namespace std;

class PoolTable {

private:
    GLuint vertex_buffer, index_buffer, color_buffer;
    vector<float> vertices;
    vector<GLushort> indices;
    vector<GLfloat> color;

public:
    ~PoolTable();
    void build (float length, float breadth, float height, float clr1, float clr2, float clr3);
    void render() const;
};
#endif