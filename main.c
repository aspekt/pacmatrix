

#include <stdlib.h>
#include <windows.h>		// Header File For Windows
#include <stdio.h>			// Header File For Standard Input/Output
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library
#include <gl\glut.h>		// Header File For The Glaux Library

static int slices = 32;
static int stacks = 32;

const float piover180 = 0.0174532925f;
float cam_rot_y =0;
float cam_mov_x = 0;
float cam_mov_z = 0;

float heading = 0;
float xpos;
float zpos;

float desp_player = 0.2f;
float camara_y = 30;
float camara_z = 35;
float camara_x = 0;


GLfloat	yrot;				// Y Rotation
GLfloat walkbias = 0;
GLfloat walkbiasangle = 0;
GLfloat lookupdown = 0.0f;
GLfloat	z=0.0f;				// Depth Into The Screen

int ap = 0; // a pressed
int dp = 0; // d pressed
int wp = 0; // w pressed
int sp = 0; // s pressed
int qp = 0; // q pressed
int ep = 0; // e pressed
int fu_p = 0; // flecha up pressed
int fd_p = 0; // flecha down pressed
int fl_p = 0; // flecha left pressed
int fr_p = 0; // flecha right pressed

#define WAKA_SPACE 9
#define WAKA_FOOD 1
#define WAKA_POWER_UP 3
#define WAKA_SUPER_FOOD 2
#define WAKA_WALL 0

const int MAP_SIZE = 40;

/* Referencias: 0 = pared
     1 = comida
     2 = super comida
     9 (espacio) = espacio
*/


char map[40][40] = {{0,0,0,9,1,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                    {0,9,9,9,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0},
                    {0,9,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,0,9,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,9,0},
                    {0,9,1,9,9,9,1,9,9,9,9,9,9,9,9,9,9,1,9,0,9,1,9,9,1,9,9,9,9,9,9,9,9,9,9,9,9,1,9,0},
                    {0,9,1,9,0,9,1,9,0,0,0,0,0,0,0,0,9,1,9,0,9,1,0,9,1,9,0,0,0,0,0,0,0,0,0,0,9,1,9,0},
                    {0,9,1,9,0,9,1,9,9,9,9,9,9,9,0,0,9,1,9,0,9,1,0,9,1,9,9,9,9,9,9,9,9,9,9,0,9,1,9,0},
                    {0,9,1,9,0,9,1,1,1,1,1,1,1,9,9,9,9,1,9,0,9,1,0,9,1,1,1,1,1,1,1,1,1,1,9,0,9,1,9,0},
                    {0,9,1,9,0,9,9,9,9,9,9,9,1,9,9,9,9,1,9,0,9,1,0,9,1,9,9,9,9,9,9,9,9,1,9,0,9,1,9,0},
                    {0,9,1,9,0,0,0,0,0,0,0,9,1,9,0,0,9,1,9,0,9,1,0,9,1,9,0,0,0,0,0,0,9,1,9,0,9,1,9,0},
                    {0,9,1,9,9,9,9,9,9,9,0,9,1,9,0,0,9,1,9,0,9,1,0,9,1,9,9,9,9,9,9,0,9,1,9,0,9,1,9,0},
                    {0,9,1,1,1,1,1,1,1,9,0,9,1,9,0,0,9,1,9,0,9,1,0,9,1,1,1,1,1,1,9,0,9,1,9,0,9,1,9,0},
                    {0,9,1,9,9,9,9,9,1,9,0,9,1,9,9,9,9,1,9,9,9,1,9,9,9,9,9,9,9,1,9,0,9,1,9,0,9,1,9,0},
                    {0,9,1,9,0,0,0,9,1,9,0,9,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,9,1,9,0,9,1,9,0,9,1,9,0},
                    {0,9,1,9,9,9,0,9,1,9,0,9,1,9,9,9,9,9,9,1,9,9,9,9,9,9,1,9,9,1,9,0,9,1,9,9,9,1,9,0},
                    {0,9,1,9,9,9,0,9,1,9,0,9,1,9,0,0,0,0,9,1,9,0,0,0,0,9,1,9,9,1,9,0,9,1,1,1,1,1,9,0},
                    {0,9,1,1,2,9,0,9,1,9,0,9,1,9,0,9,9,9,9,1,9,9,9,9,0,9,1,9,9,1,9,0,9,9,9,9,9,1,9,0},
                    {0,9,1,9,9,9,0,9,1,9,0,9,1,9,9,9,9,9,9,2,9,9,9,9,9,9,1,9,9,1,9,0,0,0,0,0,9,1,9,0},
                    {0,9,1,9,9,9,0,9,1,9,9,9,1,9,9,9,9,9,9,2,9,9,9,9,9,9,1,9,9,1,9,9,9,9,9,9,9,1,9,9},
                    {0,9,1,9,0,0,0,9,1,1,1,1,1,9,0,9,9,9,9,1,9,9,9,9,0,9,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                    {0,9,1,9,9,9,9,9,1,9,9,9,1,9,0,0,0,0,9,1,9,0,0,0,0,9,1,9,9,9,1,9,9,9,9,9,9,1,9,9},
                    {0,9,1,1,1,1,1,1,1,9,0,9,1,9,9,9,9,9,9,1,9,9,9,9,9,9,1,9,0,9,1,9,0,0,0,0,9,1,9,0},
                    {0,9,1,9,9,9,9,9,1,9,0,9,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,0,9,1,9,0,1,1,0,9,1,9,0},
                    {0,9,1,9,0,0,0,9,1,9,0,9,1,9,9,9,9,9,9,9,9,9,9,9,9,9,1,9,0,9,1,9,0,1,1,0,9,1,9,0},
                    {0,9,1,9,0,9,9,9,1,9,0,9,1,9,9,9,9,9,9,9,9,9,9,9,9,9,1,9,0,9,1,9,0,0,0,0,9,1,9,0},
                    {0,9,1,9,0,9,9,9,1,9,0,9,1,9,9,9,9,9,9,9,9,9,9,9,9,9,1,9,0,9,1,9,0,9,9,9,9,1,9,0},
                    {0,9,1,9,0,2,1,1,1,9,0,9,1,9,0,0,0,0,0,0,0,0,0,0,0,9,1,9,0,9,1,9,0,9,1,1,1,1,9,0},
                    {0,9,1,9,0,9,9,9,1,9,0,9,1,9,9,9,9,9,9,0,9,9,9,9,9,9,1,9,0,9,1,9,0,9,1,9,9,9,9,0},
                    {0,9,1,9,0,9,9,9,1,9,0,9,1,1,1,1,1,1,9,0,9,1,1,1,1,1,1,9,0,9,1,9,0,9,1,9,0,0,0,0},
                    {0,9,1,9,0,0,0,9,1,9,0,9,9,9,9,9,9,1,9,0,9,1,9,9,9,9,9,9,0,9,1,9,0,9,1,9,9,9,9,9},
                    {0,9,1,9,9,9,9,9,1,9,0,0,0,0,0,0,9,1,9,0,9,1,9,0,0,0,0,0,0,9,1,9,0,9,1,1,1,1,1,1},
                    {0,9,1,1,1,1,1,1,1,9,0,9,9,9,9,0,9,1,9,0,9,1,9,9,9,9,9,9,9,9,1,9,0,9,9,9,9,9,9,9},
                    {0,9,1,9,9,9,9,9,1,9,0,2,1,1,9,0,9,1,9,0,9,1,1,1,1,1,1,1,1,1,1,9,0,0,0,0,0,0,0,0},
                    {0,9,1,9,0,0,0,9,1,9,9,9,9,1,9,0,9,1,9,0,9,1,9,9,9,9,1,9,9,9,1,9,9,9,9,9,9,9,9,0},
                    {0,9,1,9,0,2,0,9,1,1,1,1,1,1,9,0,9,1,9,0,9,1,9,0,0,9,1,9,0,9,1,1,1,1,1,1,1,1,9,0},
                    {0,9,1,9,0,2,0,9,1,9,9,9,9,9,9,0,9,1,9,0,9,1,9,0,0,9,1,9,0,9,9,9,9,9,9,9,9,1,9,0},
                    {0,9,1,9,0,0,0,9,1,9,0,0,0,0,0,0,9,1,9,0,9,1,9,0,0,9,1,9,0,0,0,0,0,0,0,0,9,1,9,0},
                    {0,9,1,9,9,9,9,9,1,9,9,9,9,9,9,9,9,1,9,9,9,1,9,9,9,9,1,9,0,9,9,9,9,9,9,9,9,1,9,0},
                    {0,9,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,9,0,9,1,1,1,1,1,1,1,2,9,0},
                    {0,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,9,1,9,9,9,9,9,9,9,9,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,1,9,0,0,0,0,0,0,0,0}};


struct vertex_t {
    float x,y,z,size;
    
};

struct vertex_t map_pos[40][40];

//  las posiciones actuales del player
struct vertex_t g_player;

GLuint	texture[1];			// Storage For One Texture ( NEW )
struct t_waka_food {
    float x,y,z;
};

AUX_RGBImageRec *LoadBMP(char *Filename)				// Loads A Bitmap Image
{
	FILE *File=NULL;									// File Handle

	if (!Filename)										// Make Sure A Filename Was Given
	{
		return NULL;									// If Not Return NULL
	}

	File=fopen(Filename,"r");							// Check To See If The File Exists

	if (File)											// Does The File Exist?
	{
		fclose(File);									// Close The Handle
		return auxDIBImageLoad(Filename);				// Load The Bitmap And Return A Pointer
	}

	return NULL;										// If Load Failed Return NULL
}

int LoadGLTextures()									// Load Bitmaps And Convert To Textures
{
	int Status=FALSE;									// Status Indicator

	AUX_RGBImageRec *TextureImage[1];					// Create Storage Space For The Texture

	memset(TextureImage,0,sizeof(void *)*1);           	// Set The Pointer To NULL

	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	if (TextureImage[0]=LoadBMP("Data/NeHe.bmp"))
	{
		Status=TRUE;									// Set The Status To TRUE

		glGenTextures(1, &texture[0]);					// Create The Texture

		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	}

	if (TextureImage[0])									// If Texture Exists
	{
		if (TextureImage[0]->data)							// If Texture Image Exists
		{
///////////////////////////////////////////////////////////	////////////////////////////////
   	free(TextureImage[0]->data);                            // Free The Texture Image Memory
		}

		free(TextureImage[0]);								// Free The Image Structure
	}

	return Status;										// Return The Status
}

void drawMap() {

  // los waka-waka food
    
    float x = -19.5;
    float y = -0.5;
    float z = -19.5;

    static rot_angle = 0;
    rot_angle++;
    if (rot_angle == 360)
        rot_angle=0;
        
    glColor3f(1,1,0);

    int r, c;
    
  
    glPushMatrix();

    glTranslatef(x,y,z);

      for (r = 0; r < MAP_SIZE; r++) {
            for (c = 0; c < MAP_SIZE; c++) {
               //glRotatef(rot_angle, 1,1,1);
               map_pos[r][c].x = x; map_pos[r][c].y = y; map_pos[r][c].z = z;
               if (map[r][c] == WAKA_FOOD) {
                  glPushMatrix();
                    //glRotatef(rot_angle, 1,1,1);
                    glColor3f(1,1,1);
                    glutSolidCube(0.15f);
                    map_pos[r][c].size = 0.15f;
                  glPopMatrix();
                }
                else if (map[r][c] == WAKA_SUPER_FOOD) {
                    glPushMatrix();
                        glRotatef(rot_angle, 1,1,1);
                        glColor3f(1,1,0);
                        glutSolidCube(0.5);
                        map_pos[r][c].size = 0.5f;
                    glPopMatrix();
                }
                else if (map[r][c] == WAKA_WALL) {
                    glColor3f(1,0,0);
                    glutSolidCube(1);
                    map_pos[r][c].size = 1.0f;
                }
               glTranslatef(1, 0, 0);
               x++;
            }
        x=-19.5;
        z++;
        glTranslatef(-MAP_SIZE, 0, 1);
    }
    glPopMatrix();
 
}

/* GLUT callback Handlers */

static void
resize(int width, int height)
{
    const float ar = (float) width / (float) height;
    
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,1.0f,200.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
    

}

/* binomio al cuadrado */
float binomio2(float a, float b) {
    return (a*a) + 2*(a*b) + (b*b);
}

/* la distancia entre 2 puntos en el espacio */
float dist_puntos(float x0, float y0, float z0, float x1, float y1, float z1) {
    return sqrt(binomio2(x0,-x1) + binomio2(y0,-y1) + binomio2(z0,-z1));
}

/* calcula el movimiento vectorial del player con respecto al objeto colisionado

    es el seno (x1-x0) la distancia entre ls 2 puntos en el eje de las x
    igual para el eje de las z
*/
void calc_movimiento(struct vertex_t* player, struct vertex_t* object) {


   //player->x = player->x;// + (object->x - player->x);
    //player->z = player->z;// + (object->z - player->z);
}

struct cfa_t {
    struct vertex_t center;
    float radius;
};

/* vemos si colisionan el player (cfa) con el bounding-box del objeto */
int get_collision_with_box(struct vertex_t* object, struct vertex_t* player_old, struct vertex_t* player_next) {

    if (
        dist_puntos(player_next->x, player_next->y, player_next->z,
                    object->x, object->y, object->z) <= ((g_player.size + object->size)/2)
        )  {


        g_player.x = player_old->x;
        g_player.y = player_old->y;
        g_player.z = player_old->z;
        
        return 1;
    } else {
        return 0;
    }
    
}


void check_collisions(struct vertex_t* player_old, struct vertex_t* player_next) {

    struct vertex_t* object;
    
    int r, c;
     for (r = 0; r < MAP_SIZE; r++) {
        for (c = 0; c < MAP_SIZE; c++) {

            if (map[r][c] != WAKA_SPACE) {
                object = &map_pos[r][c];
                if (map[r][c] == WAKA_WALL) {
                    if (get_collision_with_box(object, player_old,player_next)) {
                       return;
                    }
                } else {
                    if (map[r][c] == WAKA_FOOD || map[r][c] == WAKA_SUPER_FOOD) {
                        if (dist_puntos(object->x, 0, object->z, player_next->x, player_next->y, player_next->z) < ((g_player.size + object->size)/2)) {
                            map[r][c] = WAKA_SPACE;
                        }
                    }
                }
            }
            
        }
    }
     // si no colisionó con una pared
     g_player.x = player_next->x;
     g_player.y = player_next->y;
     g_player.z = player_next->z;
    
}

float agent_anim = 0;

void draw_waka() {

    // el tronco
    glRotatef(-90,1,0,0);
    //glScalef(1.7,1.7,1.7);
    glutSolidCone(0.5,1,16,16);
    
    // la cabeza
    glTranslatef(0,0,0.8);
    glutSolidSphere(0.4,16,16);

}

void draw_grid() {

    float y = -1;
    float x = 0;
    glPushMatrix();
        //glRotatef(rot_angle++, 0,1,0);
        glColor3f(0,1,0);
        glBegin(GL_LINES);
        // las lineas horizontales
          for (z = -20; z <= 20; z++) {
            glVertex3f(-20,y,z);
            glVertex3f(20,y,z);
          }

        // las verticales
        for (x = -20; x <= 20; x++) {
            glVertex3f(x,y,-20);
            glVertex3f(x,y,20);
          }
        glEnd();
    glPopMatrix();

}


static void renderScene(void)
{

    static next_bola_x = 0;
    static next_bola_z = 0;
    
    static float rot_angle = 0;
    float y = -1;
    int z,x;

    static float zrot = 0;
    // velocidad de desplazamiento de la camara
    static float desp_camara = 2.0f;


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
    glLoadIdentity();

    rot_angle++;
    if (camara_y <= 0)
        camara_y = 0;
    else if (camara_y >= 100)
        camara_y = 100;

    gluLookAt(camara_x, camara_y, camara_z, g_player.x, 0, g_player.z,  0, 1, 0);	// This Determines Where The Camera's Position And View Is

    // el efecto que sube y baja (walking bias)
    static float wb_angle = 0;

	if (wb_angle >= 359.0f)		{
		wb_angle = 0.0f;
	}	else {
		wb_angle+= 10;
	}
	float wb_y = (float)sin(wb_angle * piover180)/20.0;
	
	// guardamos los valores del player originales
    
    struct vertex_t player_old = g_player;
    static struct vertex_t player_next;
    
    heading = 0.1f;

	// check keys -----
    if (wp)
        camara_y += desp_camara;
    if (sp)
        camara_y -= desp_camara;
    if (qp)
        zrot -= desp_camara;
    if (ep)
        zrot += desp_camara;
    if (fu_p) {
        // flecha up
        player_next.z = g_player.z - (float)cos(heading*piover180) * desp_player;
       	player_next.x = g_player.x -  (float)sin(heading*piover180) * desp_player;
    }
    if (fd_p) {
        // flecha down
        player_next.z = g_player.z + (float)cos(heading*piover180) * desp_player;
       	player_next.x = g_player.x + (float)sin(heading*piover180) * desp_player;
    }
    if (fl_p) {
       player_next.x = g_player.x - desp_player;
    }
    if (fr_p) {
       player_next.x = g_player.x + desp_player;
    }

    // fin - check keys
    
    check_collisions(&player_old, &player_next);
    
	GLfloat sceneroty = 360.0f - yrot;
	GLfloat scenerotz = 360.0f - zrot;

	//glRotatef(lookupdown,1.0f,0,0);
	glRotatef(sceneroty,0,1.0f,0);
	glRotatef(scenerotz,0,0.0f,1);

	//glTranslatef(xtrans, ytrans, ztrans);

    draw_grid();

    // la comida
    //draw_wakafood(rot_angle, wb_y);
    
    drawMap();
      
    // el Wakatrix (player)
    glPushMatrix();
        glTranslatef(g_player.x,-wb_y-0.7,g_player.z);
        glColor3f(1.0,0.5,0.0);
        draw_waka();
    glPopMatrix();
    
    //check_collisions(bola_x, 0, bola_z, 1);
    
    // los Agents

    glColor3f(0,0,0.6);
    glPushMatrix();
        glTranslatef(g_player.x+2,wb_y-0.7,g_player.z/2);
        draw_waka();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-g_player.x,wb_y-0.7,4);
        draw_waka();
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(8,wb_y-0.7,g_player.z);
        draw_waka();
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(2,wb_y-0.7,-g_player.z);
        draw_waka();
    glPopMatrix();

/*    glBindTexture(GL_TEXTURE_2D, texture[0]);

	glBegin(GL_QUADS);
		// Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    glEnd();
    */
    
    glutSwapBuffers();
}


void pressKey(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_LEFT : fl_p = 1; break;
		case GLUT_KEY_RIGHT : fr_p = 1; break;
		case GLUT_KEY_UP : fu_p = 1; break;
		case GLUT_KEY_DOWN : fd_p = 1; break;
	}
}

void releaseKey(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_LEFT : fl_p = 0; break;
		case GLUT_KEY_RIGHT : fr_p = 0; break;
		case GLUT_KEY_UP : fu_p = 0; break;
		case GLUT_KEY_DOWN : fd_p = 0; break;

	}
}

void processNormalKeys(unsigned char key, int x, int y) {

    if (key == 'd')
        dp = 1;
    else if (key == 'a')
        ap = 1;
    else if (key == 'w')
        wp = 1;
    else if (key == 's')
        sp = 1;
    else if (key == 'x')
        agent_anim = 1;
    else if (key == 'q')
        qp = 1;
    else if (key == 'e')
        ep = 1;
	else if (key == 27)
		exit(0);
}


void processNormalKeysUp(unsigned char key, int x, int y) {
   if (key == 'd')
        dp = 0;
    else if (key == 'a')
        ap = 0;
    else if (key == 'w')
        wp = 0;
    else if (key == 's')
        sp = 0;
    else if (key == 'q')
        qp = 0;
    else if (key == 'e')
        ep = 0;
}



static void 
idle(void)
{
    glutPostRedisplay();
}

const GLfloat light_ambient[]  = { 0.1f, 0.1f, 0.1f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

const GLfloat mat_ambient[]    = { 1.0f, 1.0f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

/* Program entry point */

int 
main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(1024,768);

    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("WakaWaka 3D - Diego Game Studio");
    glutFullScreen();


	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(processNormalKeys);
	glutKeyboardUpFunc(processNormalKeysUp);
	
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);
    
	
    glutReshapeFunc(resize);
    
    glutDisplayFunc(renderScene);
	glutIdleFunc(idle);
	
/*	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping ( NEW )

    if (!LoadGLTextures())								// Jump To Texture Loading Routine ( NEW )
	{
		return FALSE;									// If Texture Didn't Load Return FALSE
	}
	*/

    glClearColor(0,0,0,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
   
   glLineWidth(0.2f);
   //glEnable(GL_LINE_SMOOTH);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
   	glShadeModel(GL_SMOOTH);							// Enables Smooth Color Shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    
    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
    
    g_player.x = 0;
    g_player.y = 0;
    g_player.z = 0;
    g_player.size = 1;
    
    glutMainLoop();

    return EXIT_SUCCESS;
}

