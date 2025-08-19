#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.1415926535
#define P2 PI/2
#define P3 3*PI/2
#define DR 0.0174533 //one degree in radians

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

float dist (float ax, float ay, float bx, float by, float ang)
{ 
    return ( sqrt ( (bx-ax) * (bx-ax) + (by-ay) * (by-ay)) );
};

void castRay2D()
{
    int r, mx, my, mp, dof;
    float rx, ry, ra, xo, yo;
    
    ra=pa-DR*30; if(ra<0){ra+=2*PI;} if (ra>2*PI) {ra-=2*PI;}
    for (r = 0; r < 64; r++)
    {
        // // -- Check horizontal -- //
        dof = 0;
        float disH=1000000, hx=px, hy=py;
        float aTan = -1 / tan(ra);
        if (ra > PI){ry = (((int)py >> 6) << 6) - 1e-4f; rx = (py - ry) * aTan + px; yo = -64; xo = -yo * aTan;} //up
        if (ra < PI){ry = (((int)py >> 6) << 6) + 64; rx = (py - ry) * aTan + px; yo = 64; xo = -yo * aTan;} //down
        if (fabs(ra) < 1e-6f || fabs(ra - PI) < 1e-6f) {rx = px; ry = py; dof = 8;} //left or right
        while (dof < 8)
        {
            mx = (int)(rx) >> 6; my = (int)(ry) >> 6; mp = my * mapX + mx;
            if (mx >= 0 && mx < mapX && my >= 0 && my < mapY && mp > 0 && map[my][mx] == 1){ hx = rx; hy = ry; disH = dist(px, py, hx, hy, ra); dof = 8;}
            else {rx += xo; ry += yo; dof += 1;}
        }

        // -- Check vertical -- //
        dof = 0;
        float disV=1000000, vx=px, vy=py;
        float nTan = -tan(ra);
        if (ra>P2 && ra<P3){rx = (((int)px >> 6) << 6) - 1e-4f; ry = (px - rx) * nTan + py; xo = -64; yo = -xo * nTan;} //left
        if (ra<P2 || ra>P3){rx = (((int)px >> 6) << 6) + 64; ry = (px - rx) * nTan + py; xo = 64; yo = -xo * nTan;} //right
        if (fabs(ra) < 1e-6f || fabs(ra - PI) < 1e-6f) {rx = px; ry = py; dof = 8;} //up or down
        while (dof < 8)
        {
            mx = (int)(rx) >> 6; my = (int)(ry) >> 6; mp = my * mapX + mx;
            if (mx >= 0 && mx < mapX && my >= 0 && my < mapY && mp > 0 && map[my][mx] == 1){vx = rx; vy = ry; disV = dist(px, py, vx, vy, ra); dof = 8;}
            else {rx += xo; ry += yo; dof += 1;}
        }
        if (disV<disH) { rx=vx; ry=vy;}
        if (disH<disV) { rx=hx; ry=hy;}
        glColor3f(1, 0, 0);
        glLineWidth(3);
        glBegin(GL_LINES);
        glVertex2i(px, py);
        glVertex2i(rx, ry);
        glEnd();
        ra += DR; if(ra<0){ra+=2*PI;} if (ra>2*PI) {ra-=2*PI;}
    }
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
    castRay2D();
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