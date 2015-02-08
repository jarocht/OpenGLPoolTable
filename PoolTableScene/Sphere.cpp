#include "Sphere.h"
#include <glm/gtx/vector_angle.hpp>

int Sphere::save_point_and_color (glm::vec3 p, glm::vec3 c) {
    /* push_back and return its index into the vector */
    all_points.push_back(p);
    all_colors.push_back(c);
    return all_points.size() - 1;
}

void Sphere::divideTriangle (int a, int b, int c, int level) {
    if (level == 0) {
        all_index.push_back(a);
        all_index.push_back(b);
        all_index.push_back(c);
        return;
    }

    /* interpolate the colors */
    glm::vec3 cAB = 0.5f * (all_colors[a] + all_colors[b]);
    glm::vec3 cAC = 0.5f * (all_colors[a] + all_colors[c]);
    glm::vec3 cBC = 0.5f * (all_colors[b] + all_colors[c]);

    /* find the mid point of each triangle side */
    glm::vec3 AB = glm::normalize(0.5f * (all_points[a] + all_points[b]));
    glm::vec3 AC = glm::normalize(0.5f * (all_points[a] + all_points[c]));
    glm::vec3 BC = glm::normalize(0.5f * (all_points[b] + all_points[c]));

    /* adjust the length to match the radius of the sphere */
    AB *= sphere_radius;
    AC *= sphere_radius;
    BC *= sphere_radius;
    int ab = save_point_and_color(AB, cAB);
    int ac = save_point_and_color(AC, cAC);
    int bc = save_point_and_color(BC, cBC);

    /* be sure the order of these numbers makes a CCW face */
    divideTriangle(a, ab, ac, level - 1);
    divideTriangle(ab, bc, ac, level - 1);
    divideTriangle(ab, b, bc, level - 1);
    divideTriangle(ac, bc, c, level - 1);
}

void Sphere::init_model(int level) {
    /* the following if statements are needed to avoid regenerating the buffers
       when the sphere is recreated at different recursion detail
     */
    if (!glIsBuffer(vertex_buffer))
        glGenBuffers(1, &vertex_buffer);
    if (!glIsBuffer(color_buffer))
        glGenBuffers(1, &color_buffer);
    if (!glIsBuffer(index_buffer))
        glGenBuffers(1, &index_buffer);
    all_points.clear();
    all_colors.clear();
    all_index.clear();
    /* Make sure the order of the initial four vertices are specified
       correctly so the three vertices each face of the tetrahedron are in
       CCW order
     */
#if 1
    all_points.emplace_back(0, 1, 1 / sqrt(2));
    all_points.emplace_back(-1, 0, -1 / sqrt(2));
    all_points.emplace_back(0, -1, 1 / sqrt(2));
    all_points.emplace_back(+1, 0, -1 / sqrt(2));
#endif
#if 0
    all_points.emplace_back (1, 1, 1);
    all_points.emplace_back (-1, -1, 1);
    all_points.emplace_back (1, -1, -1);
    all_points.emplace_back (-1, 1, -1);
#endif
#if 0
    float r1 = static_cast<float>(sqrt(2.0/3.0));
    float r2 = 1.0 / static_cast<float>(2.0 * sqrt(6.0));
    float r3 = 1.0 / static_cast<float>(2.0 * sqrt(3.0));
    float r4 = 1/ static_cast<float>(sqrt(3.0));
    all_points.emplace_back (0, 0, r1 - r2);
    all_points.emplace_back (-r3, -0.5, -r2);
    all_points.emplace_back (r4, 0, -r2);
    all_points.emplace_back (-r3, +0.5, -r2);
#endif
    all_colors.emplace_back(1.0f, 1.0f, 1.0f);
    all_colors.emplace_back(1.0f, 0.2f, 0.2f);
    all_colors.emplace_back(0.2f, 1.0f, 0.2f);
    all_colors.emplace_back(0.2f, 0.2f, 1.0f);

    sphere_radius = glm::l1Norm(all_points[0]);
    /* be sure the order of these numbers makes a CCW face */
    divideTriangle(0, 1, 2, level);
    divideTriangle(0, 2, 3, level);
    divideTriangle(0, 3, 1, level);
    divideTriangle(2, 1, 3, level);

    /* Allocate memory for vertex buffer */
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, all_points.size() * sizeof(float) * 3, NULL, GL_DYNAMIC_DRAW);

    /* map the address from GPU memory to CPU memory */
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

    /* Allocate memory for color buffer */
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    glBufferData(GL_ARRAY_BUFFER, all_colors.size() * sizeof(float) * 3, NULL, GL_DYNAMIC_DRAW);

    /* map the address from GPU memory to CPU memory */
    float *color_ptr = (float *) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    /* Initialize the colors */
    ptr = color_ptr;
    for (auto v : all_colors) {
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

void Sphere::build(void *data) {
    int lev = *((int *) data);
    init_model(lev);

    post_build(); /* must call post_build(), to allow actual rendering */
}
