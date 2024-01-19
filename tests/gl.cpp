#include <GL/glut.h>

// a function called by GLUT everytime the window needs to be painted
void renderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
        glVertex3f(-2, -2, -5.0);
        glVertex3f(2, 0.0, -5.0);
        glVertex3f(0.0, 2, -5.0);
    glEnd();

    glutSwapBuffers();
}

// w: window width, h: window height
void changeSize(int w, int h) {
    if (h == 0) h = 1;
    float ratio = 1.0*w / h;

    // set the current matrix to be the projection matrix
    // - a matrix defines the viewing volume
    glMatrixMode(GL_PROJECTION);
    // load the identity matrix
    glLoadIdentity();
    // set the viewport to be the whole window
    // bottom left corner, w, h
    glViewport(0, 0, w, h);
    // from GLU lib, establishes the perspective parameters
    // param: field-of-view, relation between the width and height of the viewport,
    //        near cutting plane, far cutting plane
    gluPerspective(45, ratio, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {

    // init GLUT and create a window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("A Window");

    // register callbacks
    // call renderScene everytime the GLUT wants to paint the window
    glutDisplayFunc(renderScene);

    // re-paint when we resize the window
    glutReshapeFunc(changeSize);

    // enter GLUT event processing cycle
    glutMainLoop();

    return 1;
}
