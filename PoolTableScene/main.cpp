#include <iostream>
#include <cmath>
#include <algorithm>

#define PI 3.1415926535897932384626433832795
#define GLEW_STATIC
#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/io.hpp>
#include "Sphere.h"
#include "PoolTable.h"
#undef GLFW_DLL
#include </GLFW/include/glfw3.h>
//#include <GLFW/glfw3.h>
#include <vector>
#include <glfw3.h>

using namespace std;

Sphere *Balls[15];
//Sphere one;
PoolTable poolTable;
PoolTable leg1;
PoolTable leg2;
PoolTable leg3;
PoolTable leg4;
PoolTable side1;
PoolTable side2;
PoolTable side3;
PoolTable side4;
PoolTable pock1;
PoolTable pock2;
PoolTable pock3;
PoolTable pock4;
PoolTable pock5;
PoolTable pock6;


//HexNut two;
void init_model();
void win_refresh(GLFWwindow*);
float arc_ball_rad_square;
int screen_ctr_x, screen_ctr_y;
int selected_ball = -1;
bool show_wire_frame;
void renderBall(int selectedBall, int ballNum);

glm::mat4 camera_cf; // {glm::translate(glm::mat4(1.0f), glm::vec3{0,0,-5})};

void err_function (int what, const char *msg) {
    cerr << what << " " << msg << endl;
}


void win_resize (GLFWwindow * win, int width, int height)
{
#ifdef DEBUG
    cout << __FUNCTION__ << " " << width << "x" << height << endl;
#endif
    int w, h;
    glfwGetWindowSize(win, &w, &h);
    screen_ctr_x = w / 2.0;
    screen_ctr_y = h / 2.0;
    float rad = min(h,w)/2;
    arc_ball_rad_square = rad * rad;
    /* Use the entire window for our view port */
    glViewport(0, 0, width, height);
    /* Use GL_PROJECTION to select the type of synthetic camera */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    /* near-plane(1) & far-plane(10) are always POSITIVE and they measure
     * the distances along the Z-axis in front of the camera */
    /* This Controls the Camera View Volume */
     gluPerspective(60.0, static_cast<float> (width)/ static_cast<float> (height), 1, 1000);
}

void renderBall(int selectedBall, int ballNum, float dist)
{
    if (selectedBall == ballNum)
    {
        glPushMatrix();
        glTranslatef(0, dist / 2, 0);
        Balls[selectedBall]->render(show_wire_frame);
        glPopMatrix();
    }
    else
    {
        Balls[selectedBall]->render(show_wire_frame);
    }
}

void win_refresh (GLFWwindow *win) {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();
    /* place the camera using the camera coordinate frame */
    glMultMatrixf (glm::value_ptr(camera_cf));

    //const float& S = one.radius();
    const float& S = Balls[0]->radius();
    /* draw the axes */
    glBegin(GL_LINES);
    glColor3ub (255, 0, 0);
    glVertex2i (0, 0);
    glVertex2f (S * 1.1, 0);
    glColor3ub (0, 255, 0);
    glVertex2i (0, 0);
    glVertex2f (0, S * 1.1);
    glColor3ub (0, 0, 255);
    glVertex2i (0, 0);
    glVertex3f (0, 0, S * 1.1);
    glEnd();

    //Rack of Pool Balls
    int ballNum = 0;
    float d = Balls[0]->radius() * 2;
    glPushMatrix();
    glTranslatef(0, d / 2, 0); //Set on top of X-Z Plane, DO NOT TRANSLATE Y anymore
    renderBall(ballNum, selected_ball, d);
    ballNum++;
    for (int i = 2; i < 6; i++)
    {
        glTranslatef(sin(PI/6) * d, 0, -cos(PI/6) * d); //Row Increment
        renderBall(ballNum, selected_ball, d);
        ballNum++;
        glPushMatrix();
        for (int j = i - 1; j > 0; j--)
        {
            glTranslatef(-d, 0, 0);
            renderBall(ballNum, selected_ball, d);
            ballNum++;
        }
        glPopMatrix();
    }
    glPopMatrix();
    //End Rack of Pool Balls

    //Cue ball
    glPushMatrix();
    glTranslatef(0, -2, 70);
    renderBall(11, 11, 11);
    glPopMatrix();

    //PoolTable
    glPushMatrix();
    glRotatef(270, 1, 0, 0);
    glTranslatef(0, -30, -1.5);
    poolTable.build(80, 130, 5, 0, .5, 0);
    poolTable.render();
    glPopMatrix();

    //Leg 1
    glPushMatrix();
    glTranslatef(-35, -25, -30);
    leg1.build(8, 50, 8, 2, .5,.1);
    leg1.render();
    glPopMatrix();

    //Leg 2
    glPushMatrix();
    glTranslatef(35, -25, -30);
    leg2.build(8, 50, 8,2, .5,.1);
    leg2.render();
    glPopMatrix();

    //Leg 3
    glPushMatrix();
    glTranslatef(-35, -25, 90.5);
    leg3.build(8, 50, 8, 2, .5,.1);
    leg3.render();
    glPopMatrix();

    //Leg 4
    glPushMatrix();
    glTranslatef(35, -25, 90.5);
    leg4.build(8, 50, 8, 2, .5,.1);
    leg4.render();
    glPopMatrix();

    //Side 1
    glPushMatrix();
    glRotatef(270, 1, 0, 0);
    glTranslatef(37, -30, 3);
    side1.build(6, 130, 6, 0, .5, 1);
    side1.render();
    glPopMatrix();

    //Side 2
    glPushMatrix();
    glRotatef(270, 1, 0, 0);
    glTranslatef(-37, -30, 3);
    side2.build(6, 130, 6, 0, .5, 1);
    side2.render();
    glPopMatrix();

    //Side 3
    glPushMatrix();
    glRotatef(270, 1, 0, 0);
    glTranslatef(0, -92, 3);
    side3.build(80, 6, 6, 0, .5, 1);
    side3.render();
    glPopMatrix();

    //Side 4
    glPushMatrix();
    glRotatef(270, 1, 0, 0);
    glTranslatef(0, 32, 3);
    side4.build(80, 6, 6, 0, .5, 1);
    side4.render();
    glPopMatrix();

    //Pock 1
    glPushMatrix();
    glTranslatef(32, 0, -24);
    pock1.build(6, 3, 6, 0, 0, 0);
    pock1.render();
    glPopMatrix();

    //Pock 2
    glPushMatrix();
    glTranslatef(32, 0, 30);
    pock2.build(6, 3, 6, 0, 0, 0);
    pock2.render();
    glPopMatrix();

    //Pock 3
    glPushMatrix();
    glTranslatef(32, 0, 85);
    pock3.build(6, 3, 6, 0, 0, 0);
    pock3.render();
    glPopMatrix();

    //Pock 4
    glPushMatrix();
    glTranslatef(-32, 0, -24);
    pock4.build(6, 3, 6, 0, 0, 0);
    pock4.render();
    glPopMatrix();

    //Pock 5
    glPushMatrix();
    glTranslatef(-32, 0, 30);
    pock5.build(6, 3, 6, 0, 0, 0);
    pock5.render();
    glPopMatrix();

    //Pock 6
    glPushMatrix();
    glTranslatef(-32, 0, 85);
    pock6.build(6, 3, 6, 0, 0, 0);
    pock6.render();
    glPopMatrix();





    /* must swap buffer at the end of render function */
    glfwSwapBuffers(win);
}

/* action: GLFW_PRESS, GLFW_RELEASE, or GLFW_REPEAT */
void key_handler (GLFWwindow *win, int key, int scan_code, int action, int mods)
{
    if (action == 0) return;

    if (mods == GLFW_MOD_SHIFT) {
        switch (key) {
            case GLFW_KEY_D: /* Uppercase 'D' */
                /* pre mult: trans  Z-ax of the world */
                //hex1_cf = glm::translate(glm::vec3{0, +0.5f, 0}) * hex1_cf;
                break;
        }
    }
    else {
        switch (key) {
            case GLFW_KEY_COMMA:
                selected_ball--;
                if (selected_ball == -2)
                    selected_ball = 14;
                break;
            case GLFW_KEY_PERIOD:
                selected_ball++;
                if (selected_ball == 15)
                    selected_ball = -1;
                break;
            case GLFW_KEY_W:
                    show_wire_frame = !show_wire_frame;
                break;
            case GLFW_KEY_D: /* lowercase 'd' */
                /* pre mult: trans  Z-ax of the world */
                //hex1_cf = glm::translate(glm::vec3{0, -0.5f, 0}) * hex1_cf;
                break;
            case GLFW_KEY_MINUS:
                /* post mult: rotate around Z-ax of the hex nut */
                //hex1_cf = hex1_cf * glm::rotate(1.0f, glm::vec3{0, 0, 1});
                break;
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(win, true);
                break;
            case GLFW_KEY_0:
            case GLFW_KEY_1:
                //Camera pos 1 -Default
                break;
            case GLFW_KEY_2:
                //Camera pos 2
                break;
            case GLFW_KEY_3:
                //Camera pos 3
                break;
            case GLFW_KEY_4:
                //Camera pos 4
                break;
        }
    }
    win_refresh(win);
}

/*
    The virtual trackball technique implemented here is based on:
    https://www.opengl.org/wiki/Object_Mouse_Trackball
*/
void cursor_handler (GLFWwindow *win, double xpos, double ypos) {
    int state = glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT);
    static glm::vec3 first_click;
    static glm::mat4 current_cam;
    static bool is_tracking = false;

    glm::vec3 this_vec;
    if (state == GLFW_PRESS) {
        /* TODO: use glUnproject? */
        float x = (xpos - screen_ctr_x);
        float y = -(ypos - screen_ctr_y);
        float hypot_square = x * x + y * y;
        float z;

        /* determine whether the mouse is on the sphere or on the hyperbolic sheet */
        if (2 * hypot_square < arc_ball_rad_square)
            z = sqrt(arc_ball_rad_square - hypot_square);
        else
            z = arc_ball_rad_square / 2.0 / sqrt(hypot_square);
        if (!is_tracking) {
            /* store the mouse position when the button was pressed for the first time */
            first_click = glm::normalize(glm::vec3{x, y, z});
            current_cam = camera_cf;
            is_tracking = true;
        }
        else {
            /* compute the rotation w.r.t the initial click */
            this_vec = glm::normalize(glm::vec3{x, y, z});
            /* determine axis of rotation */
            glm::vec3 N = glm::cross(first_click, this_vec);

            /* determine the angle of rotation */
            float theta = glm::angle(first_click, this_vec);

            /* create a quaternion of the rotation */
            glm::quat q{cos(theta / 2), sin(theta / 2) * N};
            /* apply the rotation w.r.t to the current camera CF */
            camera_cf = current_cam * glm::toMat4(glm::normalize(q));
        }
        win_refresh(win);
    }
    else {
        is_tracking = false;
    }
}

void scroll_handler (GLFWwindow *win, double xscroll, double yscroll) {
    /* translate along the camera Z-axis */
    glm::mat4 z_translate = glm::translate((float)yscroll * glm::vec3{0, 0, 1});
    camera_cf =  z_translate * camera_cf;
    win_refresh(win);

}

void init_gl() {
    glEnable (GL_DEPTH_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnable(GL_CULL_FACE);
    glLineWidth(3.0);

    /* place the camera at Z=+5 (notice that the sign is OPPOSITE!) */
    camera_cf *= glm::translate(glm::vec3{0, -30, -200});
}

void make_model() {

    float colors[15][3]  = {
            {1.0, 1.0, 0.0}, //Yellow
            {1.0, 1.0, 0.0},
            {0.4, 1.0, 0.0}, //Green
            {0.2, 0.2, 0.8}, //Blue
            {0.0, 0.0, 0.0}, //Black
            {0.4, 1.0, 0.0}, //Green
            {0.2, 0.2, 0.8}, //Blue
            {1.0, 0.0, 0.0}, //Red #1
            {1.0, 0.0, 0.0}, //Red #1
            {0.8, 0.4, 0.0}, //Orange
            {0.8, 0.0, 0.2}, //Red #2
            {0.8, 0.0, 0.2}, //Red #2
            {0.6, 0.2, 0.8}, //Purple
            {0.6, 0.2, 0.8}, //Purple
            {0.8, 0.4, 0.0} //Orange
    };

    int N = 4;
    for (int i = 0; i < 15; i++)
    {
        Balls[i] = new Sphere;
        Balls[i]->build(N, colors[i][0], colors[i][1], colors[i][2]);
    }

    //poolTable.build(20, 40, 1, 0.0, 1.0, 0);
    //one.build((void *)&N);
    //two.build(nullptr);

    //hex1_cf = glm::rotate(30.0f, glm::vec3{0, 1, 0});   /* rotate 30 degs around Y-axis */
}

int main() {
    if(!glfwInit()) {
        cerr << "Can't initialize GLFW" << endl;
        glfwTerminate();
        exit (EXIT_FAILURE);
    }

    glfwSetErrorCallback(err_function);
    GLFWwindow * win;
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    win = glfwCreateWindow(1000, 500, "Pool Table Scene", NULL, NULL);
    if (!win) {
        cerr << "Can't create window" << endl;
        exit (EXIT_FAILURE);
    }

    glfwSetWindowRefreshCallback(win, win_refresh);
    /* On Mac with Retina display there is a discrepancy between units measured in
     * "screen coordinates" and "pixels" */
    glfwSetWindowSizeCallback(win, win_resize);  /* use this for non-retina displays */
    //glfwSetFramebufferSizeCallback(win, win_resize); /* use this for retina displays */
    glfwSetKeyCallback(win, key_handler);
    glfwSetCursorPosCallback(win, cursor_handler);
    glfwSetScrollCallback(win, scroll_handler);
    glfwMakeContextCurrent(win);

    /* glewInit must be invoked AFTER glfwMakeContextCurrent() */
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf (stderr, "GLEW init error %s\n", glewGetErrorString(err));
        exit (EXIT_FAILURE);
    }

    /* GL functions can be called, only AFTER the window is created */
    const GLubyte *version = glGetString (GL_VERSION);
    printf ("GL Version is %s\n", version);


    glfwSetWindowSize(win, 1920, 1080);
    glfwSwapInterval(1);
    init_gl();
    make_model();

    int ev_num = 0;
    win_refresh(win);
    while (!glfwWindowShouldClose(win)) {
        glfwWaitEvents();
    }
    glfwDestroyWindow(win);
    glfwTerminate();
    return 0;
}
