#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.1415926535

float px, py, pdx, pdy, pa; // player angle
int mapX = 8, mapY = 8, mapS = 64;

int map[8][8] =
    {
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1},
};

void castRay3D()
{
}

void draw2DMap()
{
    int x, y, xo, yo;
    for (y = 0; y < mapY; y++)
    {
        for (x = 0; x < mapX; x++)
        {
            if (map[y][x] == 1)
            {
                glColor3f(1, 1, 1);
            }
            else
            {
                glColor3f(0, 0, 0);
            }
            xo = x * mapS;
            yo = y * mapS;
            glBegin(GL_QUADS);
            glVertex2i(xo + 1, yo + 1);
            glVertex2i(xo + 1, yo + mapS - 1);
            glVertex2i(xo + mapS - 1, yo + mapS - 1);
            glVertex2i(xo + mapS - 1, yo + 1);
            glEnd();
        }
    }
}

void drawPlayer()
{
    glColor3f(0, 1, 1);
    glPointSize(8);
    glBegin(GL_POINTS);
    glVertex2i(px, py);
    glEnd();

    glLineWidth(1);
    glBegin(GL_LINES);
    glVertex2f(px, py);
    glVertex2f(px + cos(pa) * 20, py + sin(pa) * 20);
    glEnd();
}

// micro stepping & radius-based collision
static int blocked(float x, float y, float R)
{
    int left = (int)((x - R) / mapS);
    int right = (int)((x + R) / mapS);
    int top = (int)((y - R) / mapS);
    int bottom = (int)((y + R) / mapS);

    if (left < 0 || right >= mapX || top < 0 || bottom >= mapY)
        return 1;

    return map[top][left] || map[top][right] || map[bottom][left] || map[bottom][right];
}

void button(unsigned char key, int x, int y)
{
    float dx = 0, dy = 0; // desire move
    int movementSpeed = 12, step = 3;
    float R = 1.0f; // player radius

    if (key == 'a')
    {
        pa += 0.1f;
        if (pa >= 2 * PI)
            pa -= 2 * PI;
        pdx = cos(pa) * 5;
        pdy = sin(pa) * 5;
    }
    if (key == 'd')
    {
        pa -= 0.1f;
        if (pa < 0)
            pa += 2 * PI;
        pdx = cos(pa) * 5;
        pdy = sin(pa) * 5;
    }
    if (key == 'w' || key == 's')
    {
        int direction = (key == 'w') ? 1 : -1;
        dx = direction * movementSpeed * ((fabsf(pdx) < 1e-4f) ? 0.0f : pdx / 5.0f);
        dy = direction * movementSpeed * ((fabsf(pdy) < 1e-4f) ? 0.0f : pdy / 5.0f);
    }

    int steps = movementSpeed / step;
    for (int i = 0; i < steps; i++)
    {
        float stepX = dx / steps;
        float stepY = dy / steps;
        float newX = px + stepX;
        float newY = py + stepY;

        if (!blocked(newX, newY, R))
        {
            px = newX;
            py = newY;
        }
        else
            break;
    }
    glutPostRedisplay();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw2DMap();
    drawPlayer();
    glutSwapBuffers();
}

void init()
{
    glClearColor(0.3, 0.3, 0.3, 0);
    gluOrtho2D(0, 1024, 512, 0);
    px = 96;
    py = 128;
    pa = 0;
    pdx = cos(pa) * 5;
    pdy = sin(pa) * 5;
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1024, 512);
    glutCreateWindow("Escape-Paragon");
    init();
    glutKeyboardFunc(button);
    glutDisplayFunc(display);
    glutMainLoop();
}