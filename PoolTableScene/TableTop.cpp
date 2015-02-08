#define GLEW_STATIC
#include <GL/glew.h>
#include "TableTop.h"
#define _USE_MATH_DEFINES
#include <cmath>

TableTop::~TableTop(){
    glDeleteBuffers(1, &v_buf);
    glDeleteBuffers(1, &i_buf);
}

void TableTop::build(){
    glGenBuffers(1, &v_buf);
    glGenBuffers(1, &i_buf);
    /* push vertices of table into vector CCW (top first, then bottom)*/

    /* top of the table */
    vert.push_back(3.0f);
    vert.push_back(2.0f);
    vert.push_back(0.0f);

    vert.push_back(-3.0f);
    vert.push_back(2.0f);
    vert.push_back(0.0f);

    vert.push_back(-3.0f);
    vert.push_back(-2.0f);
    vert.push_back(0.0f);

    vert.push_back(3.0f);
    vert.push_back(-2.0f);
    vert.push_back(0.0f);

/*    addPoint(3.0f, 2.0f, 0.0f, vert);
    addPoint(-3.0f, 2.0f, 0.0f,vert);
    addPoint(-3.0f, -2.0f, 0.0f,vert);
    addPoint(3.0f, -2.0f, 0.0f,vert);*/

    /* bottom of the table */

    vert.push_back(3.0f);
    vert.push_back(2.0f);
    vert.push_back(-1.0f);

    vert.push_back(-3.0f);
    vert.push_back(2.0f);
    vert.push_back(-1.0f);

    vert.push_back(-3.0f);
    vert.push_back(-2.0f);
    vert.push_back(-1.0f);

    vert.push_back(3.0f);
    vert.push_back(-2.0f);
    vert.push_back(-1.0f);

/*    addPoint(3.0f, 2.0f, -1.0f,vert);
    addPoint(-3.0f, 2.0f, -1.0f,vert);
    addPoint(-3.0f, -2.0f, -1.0f,vert);
    addPoint(3.0f, -2.0f, -1.0f,vert);*/

    /* fill in the vertices */

    for(int i = 0; i < N_POINTS; i++)
    {
        index.push_back(i);
        index.push_back(i + N_POINTS / 2);
    }
    /* close the quad for wall */
    index.push_back(0);
    index.push_back(N_POINTS / 2);

    index.push_back(N_POINTS);

    for (int i = 0; i < N_POINTS; i++) {
        index.push_back(i);
    }
    index.push_back(0);

    /* copy the two CPU arrays to GPU */

    /* select the buffer */
    glBindBuffer (GL_ARRAY_BUFFER, v_buf);
    /* allocate in GPU and copy from CPU */
    glBufferData (GL_ARRAY_BUFFER, vert.size() * sizeof(float),
            vert.data(), GL_STATIC_DRAW);
    /* deselect the buffer */
    glBindBuffer (GL_ARRAY_BUFFER, 0);

    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, i_buf);
    /* allocate in GPU and copy from CPU */
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLushort),
            index.data(), GL_STATIC_DRAW);
    /* deselect the buffer */
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
}

/*void addPoint(float x, float y, float z, vector<float> vec) {
    vec.push_back(x);
    vec.push_back(y);
    vec.push_back(z);
}*/



void TableTop::render() const {
    /* select the buffs */
    glPushAttrib(GL_ENABLE_BIT);
    glDisableClientState(GL_COLOR_ARRAY);
    glBindBuffer (GL_ARRAY_BUFFER, v_buf);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, i_buf);

    int N = N_POINTS + 2;
    glDrawElements(GL_QUADS, N, GL_UNSIGNED_SHORT, 0);

    /* deselect the buffs */
    glBindBuffer (GL_ARRAY_BUFFER, 0);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
    glPopAttrib();
}

