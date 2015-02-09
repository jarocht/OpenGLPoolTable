#include "PoolTable.h"

PoolTable::~PoolTable() {
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &index_buffer);
    glDeleteBuffers(1, &color_buffer);
}

void PoolTable::build(float length, float breadth, float height, float clr1, float clr2, float clr3) {

    glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1, &index_buffer);
    glGenBuffers(1, &color_buffer);

    vertices.push_back(length/2);
    vertices.push_back(breadth/2);
    vertices.push_back(height/2);

    color.push_back(clr1);
    color.push_back(clr2);
    color.push_back(clr3);

    vertices.push_back(-length/2);
    vertices.push_back(breadth/2);
    vertices.push_back(height/2);

    color.push_back(clr1);
    color.push_back(clr2);
    color.push_back(clr3);

    vertices.push_back(-length/2);
    vertices.push_back(-breadth/2);
    vertices.push_back(height/2);

    color.push_back(clr1);
    color.push_back(clr2);
    color.push_back(clr3);

    vertices.push_back(length/2);
    vertices.push_back(-breadth/2);
    vertices.push_back(height/2);

    color.push_back(clr1);
    color.push_back(clr2);
    color.push_back(clr3);

    /*bottom face*/
    vertices.push_back(length/2);
    vertices.push_back(breadth/2);
    vertices.push_back(-height/2);

    color.push_back(clr1);
    color.push_back(clr2);
    color.push_back(clr3);

    vertices.push_back(-length/2);
    vertices.push_back(breadth/2);
    vertices.push_back(-height/2);

    color.push_back(clr1);
    color.push_back(clr2);
    color.push_back(clr3);

    vertices.push_back(-length/2);
    vertices.push_back(-breadth/2);
    vertices.push_back(-height/2);

    color.push_back(clr1);
    color.push_back(clr2);
    color.push_back(clr3);

    vertices.push_back(length/2);
    vertices.push_back(-breadth/2);
    vertices.push_back(-height/2);

    color.push_back(clr1);
    color.push_back(clr2);
    color.push_back(clr3);

    for (int k = 0; k < 4; k++){
        indices.push_back(k);
        indices.push_back(k+4);
    }

    indices.push_back(0);
    indices.push_back(4);

    vertices.push_back(0);
    vertices.push_back(0);
    vertices.push_back(height/2);

    color.push_back(clr1);
    color.push_back(clr2);
    color.push_back(clr3);

    vertices.push_back(0);
    vertices.push_back(0);
    vertices.push_back(-height/2);

    color.push_back(clr1);
    color.push_back(clr2);
    color.push_back(clr3);

    indices.push_back(8);

    for (int k = 0 ; k < 4; k++){
        indices.push_back(k);
    }

    indices.push_back(0);

    indices.push_back(9);

    for (int k = 3 ; k > -1; k--){
        indices.push_back(k+4);
    }

    indices.push_back(7);


    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer (GL_ARRAY_BUFFER, color_buffer);
    glBufferData (GL_ARRAY_BUFFER,
            color.size() * sizeof(GLfloat), color.data(), GL_STATIC_DRAW);

    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort),indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


}

void PoolTable::render() const {

    glPushAttrib(GL_ENABLE_BIT);
    //glDisableClientState(GL_COLOR_ARRAY);
    glBindBuffer (GL_ARRAY_BUFFER, vertex_buffer);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glBindBuffer (GL_ARRAY_BUFFER, color_buffer);
    glColorPointer(3, GL_FLOAT, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);

    glDrawElements(GL_QUAD_STRIP, 10, GL_UNSIGNED_SHORT, 0);
    glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_SHORT, (void*)(sizeof(GLushort)*10));
    glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_SHORT, (void*)(sizeof(GLushort)*16));

    glBindBuffer (GL_ARRAY_BUFFER, 0);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
    glPopAttrib();

}
