#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <math.h>
#include <string>
#include <sstream>

#include "inc/common.h"
#include "inc/GameCore.h"


/* GameCore constructor */
GameCore::GameCore()
{
    player = new Player();
    player->pos.x = 0;
    player->pos.y = 0;
    player->pos.z = 0;

    v_enemies.push_back(new Enemy("Conroe", Vec3f(12.0f, 33.0f, 0.0f)));
    v_enemies.push_back(new Enemy("Merom", Vec3f(-12.0f, -4.0f, 0.0f)));
    v_enemies.push_back(new Enemy("Yonah", Vec3f(66.0f, -23.0f, 0.0f)));

    game_running = true;
    
}

/* destructor */
GameCore::~GameCore()
{
    delete (this->player);
}

void GameCore::quit()
{
}

/* setup the game, display, etc*/
void GameCore::setUp()
{

    display.initDisplay();
    cout << player->getLives();
}


/* core game logic */
void GameCore::logic()
{

    player->move();

    for (int i = 0; i < v_enemies.size(); i++)
        v_enemies[i]->move(player);

}

/* input polling */
void GameCore::pollInputs() {

    SDL_PollEvent(&event);

    if(event.key.keysym.sym == SDLK_ESCAPE)
            game_running = false;

}

/* check for any collision between world objects */
void GameCore::checkCollisions()
{
    // iterate over all world objects and check for collision with the
	// player
	bool collision = false;
	int i = 0;
	Enemy* enemy;

    for (int i = 0; i < v_enemies.size(); i++) {
        enemy = v_enemies[i];

		if (abs(player->pos.x - enemy->pos.x) <= Enemy::PLAYER_ENEMY_COLLIDE_DISTANCE
				&& abs(player->pos.y - enemy->pos.y) <= Enemy::PLAYER_ENEMY_COLLIDE_DISTANCE) {

				collision = true;
                cout << "Collision with " + enemy->m_name << "!!" << endl;
				game_running = false;
				cin.get();
				exit(0);
		}
	}

}

/* the render method */
void GameCore::renderScene() {

    static float theta = 0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    
    /* draw the player */
    player->draw();
    
    /*
    glTranslatef(0.0f,0.0f,0.0f);
    glRotatef(theta, 0.0f, 1.0f, 1.0f);

    glBegin(GL_TRIANGLES);
        glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
        glVertex2f(0.0f, 1.0f);
        glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
        glVertex2f(0.87f, -0.5f);
        glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
        glVertex2f(-0.87f, -0.5f);
    glEnd();
    */
    

    theta += .5f;
    
    thisframe=SDL_GetTicks();     // Count the FPS
    ff++;
    if ((thisframe-lastframe) > 1000) {
        fps=ff;
        ff=0;
        lastframe=thisframe;
        ostringstream os;
        os << "FPS: ";
        os << fps;
        string a = os.str();
        
        display.setWindowCaption(a.c_str());
    }

}

/* the game's main loop */
int GameCore::mainLoop() {

    while(game_running) {

        pollInputs();
        logic();
        checkCollisions();
        renderScene();

        display.swapBuffers();
    }

    return(0);

}

