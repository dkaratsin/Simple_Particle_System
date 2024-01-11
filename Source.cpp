#include <stdlib.h>
#include <glut.h>
#include <vector>
#include <cstdlib>
#include <ctime>

const int windowWidth = 1000, windowHeight = 1000;

struct Particle {
    float x, y, z;    
    float speedX, speedY, speedZ; 
    float radius;     
    float lifespan;  
};

std::vector<Particle> particles;

float gravity = -0.0005; 
float wind = 0.0001;


void initParticles(int numParticles) {
    particles.resize(numParticles);
    for (int i = 0; i < numParticles; ++i) {
        particles[i].x = 0.0f;
        particles[i].y = 0.0f;
        particles[i].z = 0.0f;

        particles[i].speedX = static_cast<float>(rand()) / RAND_MAX * 0.02f - 0.01f;
        particles[i].speedY = static_cast<float>(rand()) / RAND_MAX * 0.02f + 0.01f;
        particles[i].speedZ = static_cast<float>(rand()) / RAND_MAX * 0.02f - 0.01f; 

        particles[i].radius = 0.02f;
        particles[i].lifespan = static_cast<float>(rand()) / RAND_MAX * 10.0f + 1.0f; // Random lifespan between 1 and 11 seconds
    }
}

void updateParticles(float deltaTime) {
    for (auto& particle : particles) {
        particle.speedY += gravity;
        particle.speedX += wind;

        particle.x += particle.speedX;
        particle.y += particle.speedY;
        particle.z += particle.speedZ;

        
        if (particle.y - particle.radius <= -1.0f) {
            particle.y = -1.0f + particle.radius;
            particle.speedY = -particle.speedY * 0.8; 
        }

        // Check if particle has reached the end of its lifespan
        particle.lifespan -= deltaTime;
        if (particle.lifespan <= 0.0f) {
            
            particle.x = 0.0f;
            particle.y = 0.0f;
            particle.z = 0.0f;

            // Reset speed and lifespan
            particle.speedX = static_cast<float>(rand()) / RAND_MAX * 0.02f - 0.01f; 
            particle.speedY = static_cast<float>(rand()) / RAND_MAX * 0.02f + 0.01f;
            particle.speedZ = static_cast<float>(rand()) / RAND_MAX * 0.02f - 0.01f; 

            particle.radius = 0.02f;
            particle.lifespan = static_cast<float>(rand()) / RAND_MAX * 10.0f + 1.0f; 
        }
    }
}

void drawParticles() {
    glColor3f(1.0f, 1.0f, 1.0f); // Set particle color (white in this case)

    for (const auto& particle : particles) {
        glPushMatrix();
        glTranslatef(particle.x, particle.y, particle.z);
        glutSolidSphere(particle.radius, 10, 10); // Draw a sphere for each particle
        glPopMatrix();
    }
}

void handleKeypress(unsigned char key, int x, int y) {
    switch (key) {
    case 'a':
        wind -= 0.00005;
        break;
    case 'd':
        wind += 0.00005;
        break;
    case 'w':
        gravity -= 0.00005;
        break;
    case 's':
        gravity += 0.00005;
        break;
    }
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    updateParticles(0.02); 
    drawParticles();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, static_cast<double>(w) / h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void idle() {
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    srand(static_cast<unsigned>(time(nullptr)));
    glutInitWindowSize(windowWidth, windowHeight); /* 1000 x 1000 pixel window */
    glutInitWindowPosition(0, 0); /* place window top left on display */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("OpenGL Particle System");
    glEnable(GL_DEPTH_TEST);
    initParticles(100);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(handleKeypress);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
