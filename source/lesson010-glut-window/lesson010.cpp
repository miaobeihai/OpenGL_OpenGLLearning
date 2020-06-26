
#include "glut.h"

void display(void)
{
    glClearColor(1,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
    glutSwapBuffers();
}

int     main(int argc,char* argv[])
{
    glutInit(&argc, argv);
    glutInitWindowPosition(10, 10);
    glutInitWindowSize(400, 600);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("lesson010");

    glutDisplayFunc(display);
    glutMainLoop();

    return  0;

}
