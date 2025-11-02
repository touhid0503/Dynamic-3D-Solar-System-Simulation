///Dynamic 3D Solar System Simulation using OpenGL/GLUT
#include <GL/glut.h>
#include <math.h>
#include <stdbool.h>

// Angles for rotation
float earthRevolution = 0; // Earth around Sun
float earthRotation = 0;   // Earth self-rotation
float moonRevolution = 0;  // Moon around Earth

// Scaling
float earthScale = 1.0f;
float moonScale = 1.0f;

// Camera
float camAngle = 20.0f;
float zoom = -12.0f;

// Animation control
bool animate = true;

// Draw a sphere
void drawSphere(float r){
    glutSolidSphere(r, 30, 30);
}

// Display scene
void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Camera
    gluLookAt(0, 5, 12 + zoom,  0, 0, 0,  0, 1, 0);
    glRotatef(camAngle, 0, 1, 0);

    // Sun
    glColor3f(1, 1, 0);
    drawSphere(1.2);

    // Earth orbit
    glPushMatrix();
        glRotatef(earthRevolution, 0, 1, 0); // Earth around Sun
        glTranslatef(4, 0, 0);
        glScalef(earthScale, earthScale, earthScale);

        // Earth self rotation
        glPushMatrix();
            glRotatef(earthRotation, 0, 1, 0);
            glColor3f(0, 0, 1);
            drawSphere(0.5);
        glPopMatrix();

        // Moon orbit around Earth
        glRotatef(moonRevolution, 0, 1, 0);
        glTranslatef(1, 0, 0);
        glScalef(moonScale, moonScale, moonScale);
        glColor3f(0.8, 0.8, 0.8);
        drawSphere(0.2);
    glPopMatrix();

    glutSwapBuffers();
}

// Timer function for automatic animation
void update(int value){
    if(animate){
        earthRevolution += 0.5; // Earth orbit speed
        earthRotation += 2;     // Earth spin speed
        moonRevolution += 1.0;  // Moon orbit speed

        if(earthRevolution > 360) earthRevolution -= 360;
        if(earthRotation > 360) earthRotation -= 360;
        if(moonRevolution > 360) moonRevolution -= 360;
    }

    glutPostRedisplay();
    glutTimerFunc(20, update, 0); // Call every 20 ms (~50 FPS)
}

// Keyboard for manual controls
void keyboard(unsigned char key, int x, int y){
    switch(key){
        case 27: exit(0); // ESC
        case 'p': case 'P': animate = false; break; // Pause
        case 's': case 'S': animate = true; break;  // Start / Resume
        // Scaling
        case '+': earthScale += 0.1; break;
        case '-': earthScale -= 0.1; break;
        case '*': moonScale += 0.05; break;
        case '/': moonScale -= 0.05; break;
        // Zoom
        case 'w': zoom += 0.5; break;
        case 'x': zoom -= 0.5; break;
    }
    glutPostRedisplay();
}

// Special keys for camera rotation
void special(int key, int x, int y){
    if(key == GLUT_KEY_LEFT) camAngle -= 5;
    if(key == GLUT_KEY_RIGHT) camAngle += 5;
    glutPostRedisplay();
}

// Handle window resize
void reshape(int w, int h){
    if(h == 0) h = 1;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w/h, 1, 100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Initialization
void init(){
    glEnable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 1);
}

// Main
int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Dynamic Solar System - Interactive GLUT");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutTimerFunc(20, update, 0);

    glutMainLoop();
    return 0;
}
