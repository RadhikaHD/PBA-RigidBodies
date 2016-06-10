#include "rigid.h"
#include "Matrix.h"
#include "Vector.h"
#include "Camera.h"
#include "Quaternion.h"

#define WINDOW_WIDTH    1080
#define WINDOW_HEIGHT   1080

int persp_win;
Camera *camera;

static double WinWidth = WINDOW_WIDTH;
static double WinHeight = WINDOW_HEIGHT;

const double rmass = 0.5;
const double rsize = 2.0;
const double rtimestep = 0.05;


static double cameraX=0.0, cameraY=32.0, cameraZ=27.0;

Rigid rigidBody(rmass, rsize, rtimestep);

void do_material()
{
    
    float mat_ambient[] = {0.0,0.0,0.0,1.0};
    float mat_diffuse[] = {0.8, 1.0, 1.0,1.0};
    float mat_specular[] = {0.03,0.03,0.03,1.0};
    float mat_shininess[] = {1.0};
    
    glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
    glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
    glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
    
}

void do_lights()
{
    
    float light0_ambient[] = {0.0, 0.0, 0.0, 0.0};
    float light0_diffuse[] = {2.0, 2.0, 2.0, 0.0};
    float light0_specular[] = {2.25, 2.25, 2.25, 0.0};
    float light0_position[] = {1.5, 2.0, 2.0, 1.0};
    float light0_direction[] = { -1.5, -2.0, -2.0, -2.0, 1.0};
    
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light0_ambient);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
    
    glLightfv(GL_LIGHT0,GL_AMBIENT,light0_ambient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,light0_diffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR,light0_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0_direction);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
}

void setup_the_viewvolume()
{
    
    Vector3d eye, view, up;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(37.0, 1.0, 0.1, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    eye.x = cameraX;
    eye.y = cameraY;
    eye.z = cameraZ;
    view.x = 0.0;
    view.y = 0.0;
    view.z = 0.0;
    up.x = 0.0;
    up.y = 1.0;
    up.z = 0.0;
    
    camera = new Camera(Vector3d(eye.x, eye.y, eye.z), Vector3d(view.x, view.y, view.z),
                        Vector3d(up.x, up.y, up.z));
    
}

void doReshape(int w, int h)
{
    
    int vpw, vph;
    float aspect = float(WINDOW_WIDTH) / float(WINDOW_HEIGHT);
    
    if(float(w) / float(h) > aspect)
    {
        vph = h;
        vpw = int(aspect * h + 0.5);
    }
    else
    {
        vpw = w;
        vph = int(w / aspect + 0.5);
    }
    
    glViewport(0, 0, vpw, vph);
    WinWidth = w;
    WinHeight = h;
    setup_the_viewvolume();
    
}

void drawDisplay()
{
   
    glEnable(GL_DEPTH_TEST);
    glClearColor(0,0.8,1.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    camera->PerspectiveDisplay(WINDOW_WIDTH, WINDOW_HEIGHT);
    rigidBody.runSimulation();
    glutSwapBuffers();
    
}

void Timer(int unused)
{
    glutPostRedisplay();
    glutTimerFunc(20, Timer, 0);
}

void mouseEventHandler(int button, int state, int x, int y) {
    camera->HandleMouseEvent(button, state, x, y);
    glutPostRedisplay();
}

void motionEventHandler(int x, int y) {
    camera->HandleMouseMotion(x, y);
    glutPostRedisplay();
}

void handleKey(unsigned char key, int x, int y)
{
    
    switch (key) {
        
        case 'r': case 'R':
            camera->Reset();
            rigidBody.initializeRigid();
            rigidBody.runSimulation();
            break;
        case 'f': case 'F':
            camera->SetCenterOfFocus(Vector3d(0, 0, 0));
            break;

        case 27:
            exit(0);
            break;
            
        default:
            return;
            break;
       
    }
    
    glutPostRedisplay();
    
}


int main(int argc, char* argv[])
{
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGBA | GLUT_DEPTH );
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100,50);
    persp_win = glutCreateWindow("Rigid Bodies");
    setup_the_viewvolume();
    glutReshapeFunc(doReshape);
    do_lights();
    do_material();
    rigidBody.initializeRigid();
    rigidBody.runSimulation();
    glutDisplayFunc(drawDisplay);
    Timer(0);
    glutMouseFunc(mouseEventHandler);
    glutMotionFunc(motionEventHandler);
    glutKeyboardFunc(handleKey);
    
    glutMainLoop();
    return 0;
    
}

