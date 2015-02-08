#include <vector>
#include <glm/vec3.hpp>
#include "BufferObject.h"
using namespace std;

class Sphere : public BufferObject {
private:
    float sphere_radius;
    int save_point_and_color(glm::vec3, glm::vec3);
    int save_point(glm::vec3);
    void init_model(int level, float r, float g, float b);
    void divideTriangle (int a, int b, int c, int level);
public:
    float radius() const {
        return sphere_radius;
    }
    //void build(void *);
    void build(int level, float r, float g, float b);
};

