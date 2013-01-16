#ifndef __SMOKEMANAGER_H__
#define __SMOKEMANAGER_H__

class SmokeManager
{
public:
    SmokeManager();
    virtual ~SmokeManager();

    void draw(float alphaDec);
    void moveParticles(int amount_of_particles);
    void idle();

private:
    float X;
    float Y;
    float Z;
    int currentParticle;
};

#endif // __SMOKEMANAGER_H__