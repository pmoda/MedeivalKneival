#include "SmokeManager.h"

#include <time.h>
#include <Windows.h>
#include <GL/glut.h>

// Code gotten and adapted from http://snipplr.com/view/54306/opengl-particle-system-smoke-stream/
struct Particle
{
    float posX;
    float posY;
    float posZ;
    float alpha;
};

const int MAX_PARTICLES = 50;
const int MIN_PARTICLES = 50;
const float limits = .8;
Particle particles[MAX_PARTICLES];

SmokeManager::SmokeManager()
{
    X = 0;
    Y = 0;
    Z = 0;
    currentParticle = 1;

    for (int i = 0; i < MAX_PARTICLES; ++i)
    {
        particles[i].alpha = 1.0f;
    }
}


SmokeManager::~SmokeManager()
{
}

void SmokeManager::draw(float alphaDec)
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glAlphaFunc(GL_GREATER,0.2);
    glEnable(GL_ALPHA_TEST);

    float R, G, B;
    glPushMatrix();
        for (int i = 0; i < MAX_PARTICLES; i++)
        {
            R = 0.6;
            G = 0.6;
            B = 0.6;
            glColor4f(R, G, B, particles[i].alpha);
            glPushMatrix();
                glTranslatef(0, -0.25, -0.7);
                glRotatef(-45, 1, 0, 0);
                glTranslatef(X * 6, Y * 2 - 1, Z * 6);
                glScalef(1 + Y, 1 + Y, 1 + Y);
                glRotatef(45, 1, 0, 0);
                glutSolidSphere(0.04, 10, 10);
            glPopMatrix();
            particles[i].alpha -= alphaDec;
            X = particles[i].posX;
            Y = particles[i].posY;
            Z = particles[i].posZ;
        }

    glPopMatrix();
}

void SmokeManager::moveParticles(int amount_of_particles)
{
    float myX, myY;
    glColor3d(2, .5, 0);
    for (int i = 0; i < amount_of_particles; i++) {		
        myX = rand() % 3 + 1;
        if(myX==1 && particles[i].posX<=limits ){
            int mytemp = rand() % 100 + 1;
            int temp = rand() % 5 + 1;
            particles[i].posX+=temp*.001;
            particles[i].posZ+=temp*.001;
            particles[i].posY+=mytemp*.0004;
        }
        if(myX==2){particles[i].posX+=.00;particles[i].posY+=.01;}
        if(myX==3 && particles[i].posX>=-limits){
            int temp = rand() % 5 + 1;
            int mytemp = rand() % 100 + 1;
            particles[i].posX-=temp*.001;
            particles[i].posZ+=temp*.001;
            particles[i].posY+=mytemp*.0004;
        }
        ///////////////////////////////////////////
        if(particles[i].alpha <= 0){	
            particles[i].posX=0;
            particles[i].posY=0;
            particles[i].posZ=0;
            particles[i].alpha = 1;
        }
    }
}

void SmokeManager::idle()
{
    moveParticles(currentParticle);

    if (currentParticle != MAX_PARTICLES) {
        currentParticle++;
    }
}