#include <cstdlib>
#include <iostream>

#include "inc/Player.h"
#include <gl/glut.h>

void Player::move() {

    this->pos.x += 0.001f;
    this->pos.y += 0.001f;
    //this->pos.z += 0.1f;
    

}

void GameObject::draw() {

    static float theta = 0;
    

    glPushMatrix();

        glLoadIdentity();
        glTranslatef(this->pos.x, this->pos.y, this->pos.z );
        glRotatef(theta, 1.0f, 1.0f, 1.0f);
        glColor4f(0.0, .5, .4, 1.0f);
        glutSolidSphere(.1, 32, 32);

    glPopMatrix();

    theta += .5f;

}
