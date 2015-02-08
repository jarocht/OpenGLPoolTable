#define _USE_MATH_DEFINES
#include <cmath>
#include "HexNut.h"

using glm::vec3;
void HexNut::build(void* data) {

    glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1, &index_buffer);
    /* A mechanical nut has 6 identical segments */
    float d_angle = 2 * M_PI / (6 * SEGMENT_SUBDIV);
    const float D60 = M_PI / 3;
    float angle = 0;
    vec3 c1, c2;
    float Z = THICKNESS;
    for (int s = 0; s < 6; s++) {
        c1 = vec3{OUTER_RAD * cos(angle),
            OUTER_RAD * sin(angle), Z};
        c2 = vec3{OUTER_RAD * cos(angle + D60),
                OUTER_RAD * sin(angle + D60), Z};
        for (int k = 0; k < SEGMENT_SUBDIV; k++) {
            float t = (float) k / SEGMENT_SUBDIV;
            vec3 v1{INNER_RAD * cos(angle),
                INNER_RAD * sin(angle), Z};
            vec3 v2{t*c2 + (1-t)*c1};
            all_index.push_back(all_points.size());
            all_points.push_back(v1);
            all_index.push_back(all_points.size());
            all_points.push_back(v2);
            angle  += d_angle;
        }
    }
    /* repeat the first two vertices to complete the quad */
    pts_count = all_index.size();
    all_index.push_back(0);
    all_index.push_back(1);
    top_count = all_index.size();

    Z = -THICKNESS;
    for (int s = 0; s < 6; s++) {
        c1 = vec3{OUTER_RAD * cos(angle),
                OUTER_RAD * sin(angle), Z};
        c2 = vec3{OUTER_RAD * cos(angle + D60),
                OUTER_RAD * sin(angle + D60), Z};
        for (int k = 0; k < SEGMENT_SUBDIV; k++) {
            float t = (float) k / SEGMENT_SUBDIV;
            vec3 v1{INNER_RAD * cos(angle),
                    INNER_RAD * sin(angle), Z};
            vec3 v2{t*c2 + (1-t)*c1};
            all_index.push_back(all_points.size());
            all_points.push_back(v1);
            all_index.push_back(all_points.size());
            all_points.push_back(v2);
            angle  += d_angle;
        }
    }
    all_index.push_back(pts_count);
    all_index.push_back(pts_count + 1);

    /* indices for the outer walls */
    for (int k = 0; k < 6 * SEGMENT_SUBDIV; k++) {
        all_index.push_back(2*k + 1);
        all_index.push_back(2*k + 1 + pts_count);
    }
    all_index.push_back(1);
    all_index.push_back(pts_count + 1);

    /* indices for the inner walls */
    for (int k = 0; k < 6 * SEGMENT_SUBDIV; k++) {
        all_index.push_back(2*k + pts_count);
        all_index.push_back(2*k);
    }
    all_index.push_back(pts_count);
    all_index.push_back(0);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, all_points.size() * sizeof(float) * 3, NULL, GL_DYNAMIC_DRAW);
    float *vertex_ptr = (float *) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    /* Initialize the vertices */
    float *ptr = vertex_ptr;
    for (auto v : all_points) {
        ptr[0] = v.x;
        ptr[1] = v.y;
        ptr[2] = v.z;
        ptr += 3;
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /* Initialize the indices */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, all_index.size() * sizeof(GLushort), all_index.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void HexNut::render(bool outline) const {
    /* bind vertex buffer */
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDisableClientState(GL_COLOR_ARRAY);
//    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
//    glColorPointer(3, GL_FLOAT, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    /* render the polygon */
    glPolygonMode(GL_FRONT, GL_FILL);
    glFrontFace(GL_CCW);
    glColor3ub (255, 0, 0);
    glDrawRangeElements(GL_QUAD_STRIP, 0        ,   0, top_count, GL_UNSIGNED_SHORT, 0);
    glFrontFace(GL_CW);
    glColor3ub (255, 0, 0);
    glDrawRangeElements(GL_QUAD_STRIP, 0, 0, top_count, GL_UNSIGNED_SHORT,
            (void *) (sizeof(GLushort) * (top_count)));
    glFrontFace(GL_CCW);
    glColor3ub (200, 30, 0);
    glDrawRangeElements(GL_QUAD_STRIP, 0, 0, top_count, GL_UNSIGNED_SHORT,
            (void *) (sizeof(GLushort) * (2*top_count)));
    glFrontFace(GL_CCW);
    glColor3ub (120, 120, 120);
    glDrawRangeElements(GL_QUAD_STRIP, 0, 0, top_count, GL_UNSIGNED_SHORT,
            (void *) (sizeof(GLushort) * (3*top_count)));
    /* unbind the buffers */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glEnableClientState(GL_COLOR_ARRAY);

}