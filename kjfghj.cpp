/*resources:
for audio library : https://learnopengl.com/In-Practice/2D-Game/Audio
*/
#ifdef __APPLE__
/* Defined before OpenGL and GLUT includes to avoid deprecation messages */
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLUT/glut.h>      // handle the window-managing operations
#else
#include <GL/glew.h>
#include <GL/freeglut.h>    // handle the window-managing operations
#endif
#include <iostream>
#include<windows.h>
#include<GL/glut.h>
#include <mmsystem.h>

#ifdef GL_ES
precision mediump float;
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string>


//<<<<<<<<<<<<<<<<<<< All the declerations in the game >>>>>>>>>>>>>>>>>>>>>>

using namespace std;
GLint win_width = 500, win_hight = 500;
GLdouble PI = 3.14159265359;
GLint fontNd = 0, eatenNd = 0, highscoreNd = 0;
float ballXSpeed = 0.01f;
float transValueX;
float transValueY;
float ballRadius2 = 0.1f;
float ballX = 0.0;
float ballY = 0.0;
float ballYJump;
float ballYVelocity = 0.0f;
float ballXVelocity = 0.0f;
float gravity = -0.005f;
bool isJumping = false;
float stepSize = 0.01f;  // Step size for movement
const float jumpSpeed = 5.0f;   // Initial jump velocity
float counter;
// Define different game states as integers
bool MAIN_MENU = true;
bool Stage = false;
bool Summerr = false;
bool Springg = false;
bool winterr = false;
bool autumnn = false;
bool PAUSE_MENU = false;
bool GAME_OVER = false;
static int score = 0;//score the gold box
//ANIMATE THE BRICK
float xpos = -0.25;//initail x position
int state = 1; // to move lrft or rigth
GLuint myTexture1;
GLuint myTexture2;
GLuint myTexture3;
char image1Path[] = "C:\\Users\\huawei\\source\\repos\\Project9\\goldenbox1.bmp";
char image2Path[] = "C:\\Users\\huawei\\source\\repos\\Project9\\Dwood.bmp";
char image3Path[] = "C:\\Users\\huawei\\source\\repos\\Project9\\lake.bmp";

GLuint LoadTexture(const char* filename, int width, int height)
{
    GLuint texture;
    unsigned char* data;
    FILE* file;

    //The following code will read in our RAW file
    fopen_s(&file, filename, "rb");

    if (file == NULL)
    {
        cout << "Unable to open the image file" << endl << "Program will exit :(" << endl;
        exit(0);
        return 0;
    }

    data = (unsigned char*)malloc(width * height * 3);
    fread(data, width * height * 3, 1, file);

    fclose(file);


    // reorder the image colors to RGB not BGR
    for (int i = 0; i < width * height; ++i)
    {
        int index = i * 3;
        unsigned char B, R;
        B = data[index];
        R = data[index + 2];

        data[index] = R;
        data[index + 2] = B;

    }


    /////////////////////////////////////////
    // All Exercises TODO: load another texture image
    //////
    glGenTextures(1, &texture);            //generate the texture with the loaded data
    glBindTexture(GL_TEXTURE_2D, texture); //bind the texture to it's array

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); //set texture environment parameters

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    /////////////////////////////////////////

    free(data); //free the texture array

    if (glGetError() != GL_NO_ERROR)
        printf("GLError in genTexture()\n");

    return texture; //return whether it was successfull  
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<  Drawing methods >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

void drawBitmapText(const char* string, float x, float y, float z)
{
    const char* c;
    glRasterPos3f(x, y, z);

    for (c = string; *c != '\0'; c++)
    {
        if (fontNd == 0) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
        else if (fontNd == 1) glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
        else if (fontNd == 2) glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
        else if (fontNd == 3) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius) {
    int i;
    int triangleAmount = 20;
    //GLfloat radius = 0.8f; //radius
    GLfloat twicePi = 2.0f * PI;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // center of circle
    for (i = 0; i <= triangleAmount; i++) {
        glVertex2f(
            x + (radius * cos(i * twicePi / triangleAmount)),
            y + (radius * sin(i * twicePi / triangleAmount))
        );
    }
    glEnd();
}

void drawFilledCircle1(GLfloat x, GLfloat y, GLfloat radius) {
    int i;
    int triangleAmount = 100; //# of triangles used to draw circle

    //GLfloat radius = 0.8f; //radius
    GLfloat twicePi = 1.0f * PI;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // center of circle
    for (i = 0; i <= triangleAmount; i++) {
        glVertex2f(
            x + (radius * cos(i * twicePi / triangleAmount)),
            y + (radius * sin(i * twicePi / triangleAmount))
        );
    }
    glEnd();
}

void drawHollowCircle(GLfloat x, GLfloat y, GLfloat radius) {
    int i;
    int lineAmount = 100; //# of triangles used to draw circle

    //GLfloat radius = 0.8f; //radius
    GLfloat twicePi = 2.0f * PI;

    glBegin(GL_LINE_LOOP);
    for (i = 0; i <= lineAmount; i++) {
        glVertex2f(
            x + (radius * cos(i * twicePi / lineAmount)),
            y + (radius * sin(i * twicePi / lineAmount))
        );
    }
    glEnd();
}

void drawBall(GLfloat x, GLfloat y, GLfloat radius) {
    glPushMatrix();
    glTranslatef(ballX, ballY, 0.0);
    int i;
    int triangleAmount = 20;
    GLfloat twicePi = 2.0f * PI;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // center of circle
    for (i = 0; i <= triangleAmount; i++) {
        glVertex2f(
            x + (radius * cos(i * twicePi / triangleAmount)),
            y + (radius * sin(i * twicePi / triangleAmount))
        );
    }
    glEnd();
    glPopMatrix();
}

void drawSquare(GLfloat ver1_x, GLfloat ver1_y, GLfloat ver2_x, GLfloat ver2_y, GLfloat ver3_x, GLfloat ver3_y, GLfloat ver4_x, GLfloat ver4_y) {
    glBegin(GL_QUADS);
    glVertex2f(ver1_x, ver1_y);
    glVertex2f(ver2_x, ver2_y);
    glVertex2f(ver3_x, ver3_y);
    glVertex2f(ver4_x, ver4_y);
    glEnd();
}

//Method to create an upside-down half of filled circle 
void drawFilledCirclehalfUpsideDown(GLfloat x, GLfloat y, GLfloat radius) {
    int i;
    int triangleAmount = 50; //# of triangles used to draw circle
    //GLfloat radius = 0.8f; //radius
    GLfloat twicePi = 1.0f * PI;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // center of circle
    for (i = 0; i <= triangleAmount; i++) {
        glVertex2f(
            x + (radius * cos(i * twicePi / triangleAmount)),
            y + (radius * sin(i * twicePi / triangleAmount))
        );
    }
    glEnd();
}

//Method to draw rocks 
void rocks() {
    // ⁄ﬁ»…
    glColor3d(0.8, 0.8, 0.9);
    drawFilledCirclehalfUpsideDown(-0.2, -0.7, 0.07);
    drawFilledCirclehalfUpsideDown(-0.3, -0.7, 0.03);
    glColor3d(0.2156862745, 0.3019607843, 0.4470588235);
    drawFilledCirclehalfUpsideDown(-0.25, -0.7, 0.05);
    drawFilledCirclehalfUpsideDown(-0.15, -0.7, 0.08);
    glColor3d(0.1568627451, 0.2352941176, 0.3215686275);
    drawFilledCirclehalfUpsideDown(-0.2, -0.7, 0.045);

}

//Method to draw the blocks
void drawTheGroundBlocks() {
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(-1.7, -0.7);
    glVertex2f(-1.4, -0.7);
    glVertex2f(-1.4, -0.8);
    glVertex2f(-1.7, -0.8);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(-1.4, -0.7);
    glVertex2f(-1.1, -0.7);
    glVertex2f(-1.1, -0.8);
    glVertex2f(-1.4, -0.8);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(-1.1, -0.7);
    glVertex2f(-0.8, -0.7);
    glVertex2f(-0.8, -0.8);
    glVertex2f(-1.1, -0.8);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(-0.8, -0.7);
    glVertex2f(-0.5, -0.7);
    glVertex2f(-0.5, -0.8);
    glVertex2f(-0.8, -0.8);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(-0.5, -0.7);
    glVertex2f(-0.2, -0.7);
    glVertex2f(-0.2, -0.8);
    glVertex2f(-.5, -0.8);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(-0.2, -0.7);
    glVertex2f(0.1, -0.7);
    glVertex2f(0.1, -0.8);
    glVertex2f(-.2, -0.8);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(0.1, -0.7);
    glVertex2f(0.4, -0.7);
    glVertex2f(0.4, -0.8);
    glVertex2f(.1, -0.8);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(0.4, -0.7);
    glVertex2f(0.7, -0.7);
    glVertex2f(0.7, -0.8);
    glVertex2f(.4, -0.8);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(0.7, -0.7);
    glVertex2f(1., -0.7);
    glVertex2f(1., -0.8);
    glVertex2f(.7, -0.8);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(1., -0.7);
    glVertex2f(1.3, -0.7);
    glVertex2f(1.3, -0.8);
    glVertex2f(1., -0.8);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(1.3, -0.7);
    glVertex2f(1.6, -0.7);
    glVertex2f(1.6, -0.8);
    glVertex2f(1.3, -0.8);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(1.6, -0.7);
    glVertex2f(1.9, -0.7);
    glVertex2f(1.9, -0.8);
    glVertex2f(1.6, -0.8);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(1.9, -0.7);
    glVertex2f(2.2, -0.7);
    glVertex2f(2.2, -.8);
    glVertex2f(1.9, -.8);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(2.2, -0.7);
    glVertex2f(2.5, -0.7);
    glVertex2f(2.5, -.8);
    glVertex2f(2.2, -.8);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(2.5, -0.7);
    glVertex2f(2.8, -0.7);
    glVertex2f(2.8, -.8);
    glVertex2f(2.5, -.8);
    glEnd();

    //// «·ÿÊ» «·”›·Ì
    glBegin(GL_QUAD_STRIP);
    glColor3d(0.2, 0.3, 0.5);
    glVertex2f(-1., -0.8); //glVertex2f(-1., -0.8);
    glVertex2f(3, -0.8);   //glVertex2f(3, -0.8);
    glVertex2f(-1., -1); //glVertex2f(-1., -0.9);
    glVertex2f(3, -1);   //glVertex2f(3, -0.9);

    //// ÿ»ﬁ… Ã·Ìœ ⁄·ÊÌ¯Â
    glBegin(GL_QUAD_STRIP);
    glColor3d(1, 1, 1);
    glVertex2f(-1., -0.7); //glVertex2f(-1., -0.8);
    glVertex2f(3, -0.7);   //glVertex2f(3, -0.8);
    glColor3d(0.2, 0.3, 0.5);
    glVertex2f(-1., -0.8); //glVertex2f(-1., -0.9);
    glVertex2f(3, -0.8);

    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(1.7, -0.8);
    glVertex2f(2., -0.8);
    glVertex2f(2., -0.9);
    glVertex2f(1.7, -0.9);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(-1.7, -0.8);
    glVertex2f(-1.3, -0.8);
    glVertex2f(-1.3, -0.9);
    glVertex2f(-1.7, -0.9);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(-1.3, -0.8);
    glVertex2f(-1., -0.8);
    glVertex2f(-1., -0.9);
    glVertex2f(-1.3, -0.9);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(-1., -0.8);
    glVertex2f(-.7, -0.8);
    glVertex2f(-.7, -0.9);
    glVertex2f(-1., -0.9);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(-0.7, -0.8);
    glVertex2f(-.4, -0.8);
    glVertex2f(-.4, -0.9);
    glVertex2f(-.7, -0.9);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(-0.4, -0.8);
    glVertex2f(-.1, -0.8);
    glVertex2f(-.1, -0.9);
    glVertex2f(-.4, -0.9);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(-0.1, -0.8);
    glVertex2f(.2, -0.8);
    glVertex2f(.2, -0.9);
    glVertex2f(-.1, -0.9);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(0.2, -0.8);
    glVertex2f(.5, -0.8);
    glVertex2f(.5, -0.9);
    glVertex2f(.2, -0.9);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(0.5, -0.8);
    glVertex2f(.8, -0.8);
    glVertex2f(.8, -0.9);
    glVertex2f(.5, -0.9);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(0.8, -0.8);
    glVertex2f(1.1, -0.8);
    glVertex2f(1.1, -0.9);
    glVertex2f(.8, -0.9);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(1.1, -0.8);
    glVertex2f(1.4, -0.8);
    glVertex2f(1.4, -0.9);
    glVertex2f(1.1, -0.9);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(1.4, -0.8);
    glVertex2f(1.7, -0.8);
    glVertex2f(1.7, -0.9);
    glVertex2f(1.4, -0.9);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(1.7, -0.8);
    glVertex2f(2., -0.8);
    glVertex2f(2., -0.9);
    glVertex2f(1.7, -0.9);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(2., -0.8);
    glVertex2f(2.3, -0.8);
    glVertex2f(2.3, -0.9);
    glVertex2f(2., -0.9);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(2.3, -0.8);
    glVertex2f(2.6, -0.8);
    glVertex2f(2.6, -0.9);
    glVertex2f(2.3, -0.9);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(2.6, -0.8);
    glVertex2f(2.9, -0.8);
    glVertex2f(2.9, -0.9);
    glVertex2f(2.6, -0.9);
    glEnd();


    /////
    glBegin(GL_LINE_LOOP);
    glColor3d(0.5, 0.5, 0.5);
    glVertex2f(-2., -0.9);
    glVertex2f(-1.7, -0.9);
    glVertex2f(-1.7, -1.);
    glVertex2f(-2., -1.);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(-1.7, -0.9);
    glVertex2f(-1.4, -0.9);
    glVertex2f(-1.4, -1.);
    glVertex2f(-1.7, -1.);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(-1.4, -0.9);
    glVertex2f(-1.1, -0.9);
    glVertex2f(-1.1, -1.);
    glVertex2f(-1.4, -1.);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(-1.1, -0.9);
    glVertex2f(-0.8, -0.9);
    glVertex2f(-0.8, -1.);
    glVertex2f(-1.1, -1.);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(-0.8, -.9);
    glVertex2f(-0.5, -.9);
    glVertex2f(-0.5, -1.);
    glVertex2f(-0.8, -1.);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(-0.5, -0.9);
    glVertex2f(-0.2, -0.9);
    glVertex2f(-0.2, -1.);
    glVertex2f(-.5, -1.);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(-0.2, -0.9);
    glVertex2f(0.1, -0.9);
    glVertex2f(0.1, -1.);
    glVertex2f(-.2, -1.);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(0.1, -0.9);
    glVertex2f(0.4, -0.9);
    glVertex2f(0.4, -1.);
    glVertex2f(.1, -1.);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(0.4, -0.9);
    glVertex2f(0.7, -0.9);
    glVertex2f(0.7, -1.);
    glVertex2f(.4, -1.);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(0.7, -0.9);
    glVertex2f(1., -0.9);
    glVertex2f(1., -1.);
    glVertex2f(.7, -1.);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(1., -0.9);
    glVertex2f(1.3, -0.9);
    glVertex2f(1.3, -1.);
    glVertex2f(1., -1.);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(1.3, -0.9);
    glVertex2f(1.6, -0.9);
    glVertex2f(1.6, -1.);
    glVertex2f(1.3, -1.);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(1.6, -0.9);
    glVertex2f(1.9, -0.9);
    glVertex2f(1.9, -1.);
    glVertex2f(1.6, -1.);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(1.9, -0.9);
    glVertex2f(2.2, -0.9);
    glVertex2f(2.2, -1.);
    glVertex2f(1.9, -1.);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(2.2, -0.9);
    glVertex2f(2.5, -0.9);
    glVertex2f(2.5, -1.);
    glVertex2f(2.2, -1.);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(2.5, -0.9);
    glVertex2f(2.8, -0.9);
    glVertex2f(2.8, -1.);
    glVertex2f(2.5, -1.);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(2.8, -0.9);
    glVertex2f(3., -0.9);
    glVertex2f(3., -1.);
    glVertex2f(2.8, -1.);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(1.7, -1.);
    glVertex2f(2., -1.);
    glVertex2f(2., -1.1);
    glVertex2f(1.7, -1.1);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(-1.7, -1.);
    glVertex2f(-1.3, -1.);
    glVertex2f(-1.3, -1.1);
    glVertex2f(-1.7, -1.1);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(-1.3, -1.);
    glVertex2f(-1., -1.);
    glVertex2f(-1., -1.1);
    glVertex2f(-1.3, -1.1);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(-1., -1.);
    glVertex2f(-.7, -1.);
    glVertex2f(-.7, -1.1);
    glVertex2f(-1., -1.1);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(-0.7, -1.);
    glVertex2f(-.4, -1.);
    glVertex2f(-.4, -1.1);
    glVertex2f(-.7, -1.1);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(-0.4, -1.);
    glVertex2f(-.1, -1.);
    glVertex2f(-.1, -1.1);
    glVertex2f(-.4, -1.1);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(-0.1, -1.);
    glVertex2f(.2, -1.);
    glVertex2f(.2, -1.1);
    glVertex2f(-.1, -1.1);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(0.2, -1.);
    glVertex2f(.5, -1.);
    glVertex2f(.5, -1.1);
    glVertex2f(.2, -1.1);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(0.5, -1.);
    glVertex2f(.8, -1.);
    glVertex2f(.8, -1.1);
    glVertex2f(.5, -1.1);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(0.8, -1.);
    glVertex2f(1.1, -1.);
    glVertex2f(1.1, -1.1);
    glVertex2f(.8, -1.1);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(1.1, -1.);
    glVertex2f(1.4, -1.);
    glVertex2f(1.4, -1.1);
    glVertex2f(1.1, -1.1);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(1.4, -1.);
    glVertex2f(1.7, -1.);
    glVertex2f(1.7, -1.1);
    glVertex2f(1.4, -1.1);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(1.7, -1.);
    glVertex2f(2., -1.);
    glVertex2f(2., -1.1);
    glVertex2f(1.7, -1.1);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(2., -1.);
    glVertex2f(2.3, -1.);
    glVertex2f(2.3, -1.1);
    glVertex2f(2., -1.1);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(2.3, -1.);
    glVertex2f(2.6, -1.);
    glVertex2f(2.6, -1.1);
    glVertex2f(2.3, -1.1);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(2.6, -1.);
    glVertex2f(2.9, -1.);
    glVertex2f(2.9, -1.1);
    glVertex2f(2.6, -1.1);
    glEnd();
    /////
        //////
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(-1.7, -1.1);
    glVertex2f(-1.4, -1.1);
    glVertex2f(-1.4, -1.2);
    glVertex2f(-1.7, -1.2);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(-1.4, -1.1);
    glVertex2f(-1.1, -1.1);
    glVertex2f(-1.1, -1.2);
    glVertex2f(-1.4, -1.2);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(-1.1, -1.1);
    glVertex2f(-0.8, -1.1);
    glVertex2f(-0.8, -1.2);
    glVertex2f(-1.1, -1.2);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(-0.8, -1.1);
    glVertex2f(-0.5, -1.1);
    glVertex2f(-0.5, -1.2);
    glVertex2f(-0.8, -1.2);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(-0.5, -1.1);
    glVertex2f(-0.2, -1.1);
    glVertex2f(-0.2, -1.2);
    glVertex2f(-.5, -1.2);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(-0.2, -1.1);
    glVertex2f(0.1, -1.1);
    glVertex2f(0.1, -1.2);
    glVertex2f(-.2, -1.2);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(0.1, -1.1);
    glVertex2f(0.4, -1.1);
    glVertex2f(0.4, -1.2);
    glVertex2f(.1, -1.2);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(0.4, -1.1);
    glVertex2f(0.7, -1.1);
    glVertex2f(0.7, -1.2);
    glVertex2f(.4, -1.2);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(0.7, -1.1);
    glVertex2f(1., -1.1);
    glVertex2f(1., -1.2);
    glVertex2f(.7, -1.2);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(1., -1.1);
    glVertex2f(1.3, -1.1);
    glVertex2f(1.3, -1.2);
    glVertex2f(1., -1.2);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(1.3, -1.1);
    glVertex2f(1.6, -1.1);
    glVertex2f(1.6, -1.2);
    glVertex2f(1.3, -1.2);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(1.6, -1.1);
    glVertex2f(1.9, -1.1);
    glVertex2f(1.9, -1.2);
    glVertex2f(1.6, -1.2);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(1.9, -1.1);
    glVertex2f(2.2, -1.1);
    glVertex2f(2.2, -1.2);
    glVertex2f(1.9, -1.2);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(2.2, -1.1);
    glVertex2f(2.5, -1.1);
    glVertex2f(2.5, -1.2);
    glVertex2f(2.2, -1.2);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(2.5, -1.1);
    glVertex2f(2.8, -1.1);
    glVertex2f(2.8, -1.2);
    glVertex2f(2.5, -1.2);
    glEnd();

}

void spring_obst() {
    //                 
    glBegin(GL_QUADS);
    glColor3d(0.6, 0.5, 0.);
    glVertex2f(-0.2, -0.7);
    glColor3d(0.6, 0.5, 0.);
    glVertex2f(-0.2, 0.1);
    glColor4d(0.2, 0., 0., 1);
    glVertex2f(-0.3, 0.1);
    glColor4d(0.2, 0., 0., 1);
    glVertex2f(-.3, -0.7);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0.);
    glVertex2f(-0.2, -0.7);
    glVertex2f(-0.2, 0.1);
    glVertex2f(-0.3, 0.1);
    glVertex2f(-.3, -0.7);
    glEnd();
    //  
}

void spring_obst2() {
    //                 
    glBegin(GL_QUADS);
    glColor3d(0.6, 0.5, 0.);
    glVertex2f(0.7, -0.7);
    glColor3d(0.6, 0.5, 0.);
    glVertex2f(0.7, -0.6);
    glColor4d(0.2, 0., 0., 1);
    glVertex2f(-0.2, -0.6);
    glColor4d(0.2, 0., 0., 1);
    glVertex2f(-.2, -0.7);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0.);
    glVertex2f(0.7, -0.7);
    glVertex2f(0.7, -0.6);
    glVertex2f(-0.2, -0.6);
    glVertex2f(-.2, -0.7);
    glEnd();
    //  
}

//Method to draw trees
void trees(GLfloat lowR, GLfloat lowG, GLfloat lowB, GLfloat midR, GLfloat midG, GLfloat midB, GLfloat upR, GLfloat upG, GLfloat upB) {
    // Ã–⁄ 
    glLineWidth(10);
    glBegin(GL_LINES);
    glColor3d(0.2156862745, 0.3019607843, 0.4470588235);
    glVertex2f(-0.83, -0.35);
    glVertex2f(-0.83, -1);
    glEnd();

    // tree
    glBegin(GL_TRIANGLES);
    //glColor3d(0.2745098039, 0.4235294118, 0.6156862745); //(GLfloat lowR, GLfloat lowG, GLfloat lowB)
    glColor3d(lowR, lowG, lowB);
    glVertex2f(-0.97, -0.6);
    glVertex2f(-0.66, -0.6);
    glVertex2f(-0.83, -0.3);

    //glColor3d(0.3176470588, 0.4901960784, 0.7254901961); //(GLfloat midR, GLfloat midG, GLfloat midB)
    glColor3d(midR, midG, midB);
    glVertex2f(-0.96, -0.5);
    glVertex2f(-0.68, -0.5);
    glVertex2f(-0.83, -0.2);

    //glColor3d(0.3843137255, 0.5568627451, 0.7725490196); //(GLfloat upR, GLfloat upG, GLfloat upB)
    glColor3d(upR, upG, upB);
    glVertex2f(-0.95, -0.4);
    glVertex2f(-0.7, -0.4);
    glVertex2f(-0.83, -0.1);

    glEnd();

}

void drawScore() {
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(-0.9, 0.9); // Adjust the position as needed
    std::string scoreStr = "Score: " + std::to_string(score);
    for (char c : scoreStr) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }
}
void flyingBrick() {
    //«·ÿÊ»… «·ÿ«Ì—…
    glBegin(GL_QUAD_STRIP);
    glColor3d(0.6, 0.3, 0);
    glVertex2f(-.4, -0.5);
    glVertex2f(0.3, -0.5);
    glColor3d(0.3, 0.1, 0);
    glVertex2f(-0.4, -0.6);
    glColor3d(0.3, 0.1, 0);
    glVertex2f(0.3, -0.6);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-0.4, -0.5);
    glVertex2f(-0.2, -0.5);
    glVertex2f(-0.2, -0.6);
    glVertex2f(-.4, -0.6);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-0.2, -0.5);
    glVertex2f(0.1, -0.5);
    glVertex2f(0.1, -0.6);
    glVertex2f(-.2, -0.6);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(0.1, -0.5);
    glVertex2f(0.3, -0.5);
    glVertex2f(0.3, -0.6);
    glVertex2f(.1, -0.6);
    glEnd();
}


void summerball() {
    glColor3d(0.8901960784, 0.02745098039, 0.02745098039);
    drawBall(-0.9, -0.1, -0.08);
    // ⁄ÌÊ‰
    glColor3f(1.f, 1.f, 1.f);
    drawBall(-0.94, -0.10, .03);
    glColor3f(1.f, 1.f, 1.f);
    drawBall(-0.86, -0.10, .03);
    glColor3f(0.f, 0.f, 0.f);
    drawBall(-0.93, -0.10, .02);
    glColor3f(0.f, 0.f, 0.f);
    drawBall(-0.87, -0.10, .02);
    ///ŒœÊœ 
    glColor3f(0.9f, 0.6f, 0.6f);
    drawBall(-0.86, -0.146, .010);
    glColor3f(0.9f, 0.6f, 0.6f);
    drawBall(-0.94, -0.146, .010);

    glMatrixMode(GL_PROJECTION);
}
void winterball() {
    glColor4d(0.5490196078, 0.6705882353, 0.7921568627, 0.7);
    drawBall(-0.8, -0.6, -0.08);
    // ⁄ÌÊ‰
    glColor3f(1.f, 1.f, 1.f);
    drawBall(-0.84, -0.6, .03);
    glColor3f(1.f, 1.f, 1.f);
    drawBall(-0.76, -0.6, .03);
    glColor3f(0.f, 0.f, 0.f);
    drawBall(-0.83, -0.6, .02);
    glColor3f(0.f, 0.f, 0.f);
    drawBall(-0.77, -0.6, .02);
    ///ŒœÊœ 
    glColor3f(0.9f, 0.6f, 0.6f);
    drawBall(-0.76, -0.640, .010);
    glColor3f(0.9f, 0.6f, 0.6f);
    drawBall(-0.84, -0.640, .010);

    //glMatrixMode(GL_PROJECTION);
}
void springball() {
    ///
    glColor3d(0.9, 0.5, 0.5);
    drawBall(-0.8, -0.5, -0.08);
    // ⁄ÌÊ‰
    glColor3f(1.f, 1.f, 1.f);
    drawBall(-0.84, -0.5, .03);
    glColor3f(1.f, 1.f, 1.f);
    drawBall(-0.76, -0.5, .03);
    glColor3f(0.f, 0.f, 0.f);
    drawBall(-0.83, -0.5, .02);
    glColor3f(0.f, 0.f, 0.f);
    drawBall(-0.77, -0.5, .02);
    ///ŒœÊœ 
    glColor3f(0.9f, 0.6f, 0.6f);
    drawBall(-0.77, -0.550, .010);
    glColor3f(0.9f, 0.6f, 0.6f);
    drawBall(-0.84, -0.551, .010);

    //glMatrixMode(GL_PROJECTION);

}
void autumnball() {
    ///
    glColor3d(0.9333333333, 0.7137254902, 0.02745098039);
    drawBall(-0.8, -0.4, -0.08);
    // ⁄ÌÊ‰
    glColor3f(1.f, 1.f, 1.f);
    drawBall(-0.84, -0.4, .03);
    glColor3f(1.f, 1.f, 1.f);
    drawBall(-0.76, -0.4, .03);
    glColor3f(0.f, 0.f, 0.f);
    drawBall(-0.83, -0.4, .02);
    glColor3f(0.f, 0.f, 0.f);
    drawBall(-0.77, -0.4, .02);
    ///ŒœÊœ 
    glColor3f(0.9f, 0.6f, 0.6f);
    drawBall(-0.76, -0.440, .010);
    glColor3f(0.9f, 0.6f, 0.6f);
    drawBall(-0.84, -0.440, .010);

    glMatrixMode(GL_PROJECTION);
}




//  <<<<<<<<<<<<<<<<<<< All the sences methods in the game >>>>>>>>>>>>>>>>>>>>>>>

void welcome() {

    //clouds//
    glColor3f(1.0f, 1.0f, 1.0f);
    drawFilledCircle(0.3 - 0.15, 0.8, .12);
    drawFilledCircle(0.3, 0.8, .15);
    drawFilledCircle(0.3 + 0.15, 0.8, .16);
    drawFilledCircle(0.3 + 0.3, 0.8, .13);
    //cloud left
    drawFilledCircle(-0.3 - 0.15, 0.5, .12);
    drawFilledCircle(-0.3, 0.5, .15);
    drawFilledCircle(-0.3 + 0.15, 0.5, .16);
    drawFilledCircle(-0.3 + 0.3, 0.5, .13);
    //little cloud
    drawFilledCircle(-0.8 - 0.15, 0.8, .08);
    drawFilledCircle(-0.8, 0.8, .11);
    drawFilledCircle(-0.8 + 0.15, 0.8, .12);
    drawFilledCircle(-0.8 + 0.3, 0.8, .09);
    //little cloud
    drawFilledCircle(0.7 - 0.15, 0.5, .08);
    drawFilledCircle(0.7, 0.5, .11);
    drawFilledCircle(0.7 + 0.15, 0.5, .12);
    drawFilledCircle(0.7 + 0.3, 0.5, .09);
    //little tiny cloud down
    drawFilledCircle(0.3 - 0.06, 0.2, .05);
    drawFilledCircle(0.3, 0.2, .07);
    drawFilledCircle(0.3 + 0.08, 0.2, .08);
    drawFilledCircle(0.3 + 0.16, 0.2, .06);

    drawFilledCircle(-0.9 - 0.06, 0.3, .05);
    drawFilledCircle(-0.9, 0.3, .07);
    drawFilledCircle(-0.9 + 0.08, 0.3, .08);
    drawFilledCircle(-0.9 + 0.16, 0.3, .06);

    glBegin(GL_QUADS);
    glColor3d(0.5, 0.3, 0.1);
    glVertex2f(1, -0.9);
    glVertex2f(-1., -0.9);
    glColor4d(0.2, 0., 0., 0.7);
    glVertex2f(-1., -1);
    glVertex2f(1, -1);
    glEnd();
    //lines
    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(-1, -.9);
    glVertex2f(-.7, -.9);
    glVertex2f(-.7, -1);
    glVertex2f(-1, -1);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(-.7, -.9);
    glVertex2f(-.4, -.9);
    glVertex2f(-.4, -1);
    glVertex2f(-.7, -1);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(-.4, -.9);
    glVertex2f(-.1, -.9);
    glVertex2f(-.1, -1);
    glVertex2f(-.4, -1);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(-.1, -.9);
    glVertex2f(0.2, -.9);
    glVertex2f(.2, -1);
    glVertex2f(-.1, -1);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(0.2, -.9);
    glVertex2f(0.5, -.9);
    glVertex2f(.5, -1);
    glVertex2f(0.2, -1);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(0.5, -.9);
    glVertex2f(0.8, -.9);
    glVertex2f(.8, -1);
    glVertex2f(0.5, -1);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(0.8, -.9);
    glVertex2f(1, -.9);
    glVertex2f(1, -1);
    glVertex2f(0.8, -1);
    glEnd();
    //mid
    glBegin(GL_QUADS);
    glColor3d(0.5, 0.3, 0.1);
    glVertex2f(1, -0.8);
    glVertex2f(-1., -0.8);
    glColor4d(0.2, 0., 0., 0.7);
    glVertex2f(-1., -0.9);
    glVertex2f(1, -0.9);
    glEnd();
    //lines
    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-1, -0.8);
    glVertex2f(-.8, -0.8);
    glVertex2f(-.8, -0.9);
    glVertex2f(-1, -0.9);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-0.8, -0.8);
    glVertex2f(-.5, -0.8);
    glVertex2f(-.5, -0.9);
    glVertex2f(-0.8, -0.9);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-0.5, -0.8);
    glVertex2f(-.2, -0.8);
    glVertex2f(-.2, -0.9);
    glVertex2f(-0.5, -0.9);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-0.2, -0.8);
    glVertex2f(0.1, -0.8);
    glVertex2f(0.1, -0.9);
    glVertex2f(-0.2, -0.9);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(0.1, -0.8);
    glVertex2f(0.4, -0.8);
    glVertex2f(0.4, -0.9);
    glVertex2f(0.1, -0.9);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(0.4, -0.8);
    glVertex2f(0.7, -0.8);
    glVertex2f(0.7, -0.9);
    glVertex2f(0.4, -0.9);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(0.7, -0.8);
    glVertex2f(1, -0.8);
    glVertex2f(1, -0.9);
    glVertex2f(0.7, -0.9);
    glEnd();
    //up
    glBegin(GL_QUADS);
    glColor3d(0.5, 0.3, 0.1);
    glVertex2f(1, -0.7);
    glVertex2f(-1., -0.7);
    glColor4d(0.2, 0., 0., 0.7);
    glVertex2f(-1., -0.8);
    glVertex2f(1, -0.8);
    glEnd();
    //lines
    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(-1, -.7);
    glVertex2f(-.7, -.7);
    glVertex2f(-.7, -0.8);
    glVertex2f(-1, -0.8);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(-.7, -.7);
    glVertex2f(-.4, -.7);
    glVertex2f(-.4, -0.8);
    glVertex2f(-.7, -.8);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(-.4, -.7);
    glVertex2f(-.1, -.7);
    glVertex2f(-.1, -0.8);
    glVertex2f(-.4, -0.8);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(-.1, -.7);
    glVertex2f(0.2, -.7);
    glVertex2f(.2, -0.8);
    glVertex2f(-.1, -.8);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(0.2, -.7);
    glVertex2f(0.5, -.7);
    glVertex2f(.5, -.8);
    glVertex2f(0.2, -.8);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(0.5, -.7);
    glVertex2f(0.8, -.7);
    glVertex2f(.8, -.8);
    glVertex2f(0.5, -.8);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(0.8, -.7);
    glVertex2f(1, -.7);
    glVertex2f(1, -.8);
    glVertex2f(0.8, -.8);
    glEnd();
    //apove up
    glBegin(GL_QUADS);
    glColor3d(0.5, 0.3, 0.1);
    glVertex2f(1, -0.6);
    glVertex2f(-1., -0.6);
    glColor4d(0.2, 0., 0., 0.7);
    glVertex2f(-1., -0.7);
    glVertex2f(1, -0.7);
    glEnd();
    //lines
    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-1, -0.6);
    glVertex2f(-.8, -0.6);
    glVertex2f(-.8, -0.7);
    glVertex2f(-1, -0.7);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-0.8, -0.6);
    glVertex2f(-.5, -0.6);
    glVertex2f(-.5, -0.7);
    glVertex2f(-0.8, -0.7);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-0.5, -0.6);
    glVertex2f(-.2, -0.6);
    glVertex2f(-.2, -0.7);
    glVertex2f(-0.5, -0.7);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-0.2, -0.6);
    glVertex2f(0.1, -0.6);
    glVertex2f(0.1, -0.7);
    glVertex2f(-0.2, -0.7);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(0.1, -0.6);
    glVertex2f(0.4, -0.6);
    glVertex2f(0.4, -0.7);
    glVertex2f(0.1, -0.7);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(0.4, -0.6);
    glVertex2f(0.7, -0.6);
    glVertex2f(0.7, -0.7);
    glVertex2f(0.4, -0.7);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(0.7, -0.6);
    glVertex2f(1, -0.6);
    glVertex2f(1, -0.7);
    glVertex2f(0.7, -0.7);
    glEnd();

    /////«·«—÷ «Œ÷—
    glBegin(GL_QUADS);
    glColor3d(0.4, 0.6, 0.);
    glVertex2f(1.0, -0.7);
    glVertex2f(-1, -0.7);
    glColor3d(0.4, 0.7, 0.2);
    glVertex2f(-1, -0.6);
    glVertex2f(1, -0.6);
    glEnd();
    ///«·«‘Ã«— «Œ÷— ›« Õ
    glColor3d(0.5, 0.7, 0.3);
    drawFilledCircle1(-0.9, -0.6, 0.2);
    drawFilledCircle1(-0.7, -0.6, 0.3);
    drawFilledCircle1(-0.4, -0.6, 0.17);
    drawFilledCircle1(-0.16, -0.6, 0.35);
    drawFilledCircle1(0.04, -0.6, 0.27);
    drawFilledCircle1(0.6, -0.6, 0.35);
    drawFilledCircle1(0.9, -0.6, 0.27);
    ////«·«‘Ã«— «·«Œ÷— «·€«„ﬁ
    glColor3d(0.4, 0.6, 0.2);
    drawFilledCircle1(-0.9, -0.6, 0.09);
    drawFilledCircle1(-0.7, -0.6, 0.2);
    drawFilledCircle1(-0.4, -0.6, 0.17);
    drawFilledCircle1(-0.16, -0.6, 0.25);
    drawFilledCircle1(0.04, -0.6, 0.17);
    drawFilledCircle1(0.4, -0.6, 0.25);
    drawFilledCircle1(.7, -0.6, 0.17);
    drawFilledCircle1(.9, -0.6, 0.23);

    glBegin(GL_TRIANGLES);
    glColor3d(1, 1, 1);
    glVertex2f(-0.01, -0.32);
    glVertex2f(0.1, -0.39);
    glVertex2f(-0.01, -0.45);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(0, 0, 0);
    glVertex2f(-0.01, -0.32);
    glVertex2f(0.1, -0.39);
    glVertex2f(-0.01, -0.45);
    glEnd();

    glLineWidth(1.0f);
    glColor3d(0.9, 0.5, 0.5);
    drawFilledCircle(0.0, 0.0, 0.2);


    glColor3f(1.f, 1.f, 1.f);
    drawFilledCircle(-0.07, 0.04, .06);

    glColor3f(1.f, 1.f, 1.f);
    drawFilledCircle(0.07, 0.04, .06);

    glColor3f(0.f, 0.f, 0.f);
    drawFilledCircle(-0.06, 0.04, .04);

    glColor3f(0.f, 0.f, 0.f);
    drawFilledCircle(0.06, 0.04, .04);

    glColor3f(0.9f, 0.6f, 0.6f);
    drawFilledCircle(0.1, -0.045, .025);


    glColor3f(0.9f, 0.6f, 0.6f);
    drawFilledCircle(-0.1, -0.045, .025);

    //To write on the screen
    glColor3f(0.5, 0.0, 0.5);
    glRasterPos3f(-0.53, 0.0, 0);
    const char* text1 = "CRAZY     ";
    while (*text1) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *text1);
        text1++;
    }
    glColor3f(0.5, 0.0, 0.5);
    glRasterPos3f(0.16, 0.0, 0);
    const char* text2 = "   BALL";
    while (*text2) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *text2);
        text2++;
    }
    glColor3f(0.54, 0.351, 0.167);
    glRasterPos3f(-0.43, -0.57, 0);
    const char* text3 = "Prees S to start the game";
    while (*text3) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text3);
        text3++;
    }
}


void Stages() {
    //clouds//
    glColor3f(1.0f, 1.0f, 1.0f);
    drawFilledCircle(0.3 - 0.15, 0.8, .12);
    drawFilledCircle(0.3, 0.8, .15);
    drawFilledCircle(0.3 + 0.15, 0.8, .16);
    drawFilledCircle(0.3 + 0.3, 0.8, .13);
    //cloud left
    drawFilledCircle(-0.3 - 0.15, 0.5, .12);
    drawFilledCircle(-0.3, 0.5, .15);
    drawFilledCircle(-0.3 + 0.15, 0.5, .16);
    drawFilledCircle(-0.3 + 0.3, 0.5, .13);
    //little cloud
    drawFilledCircle(-0.8 - 0.15, 0.8, .08);
    drawFilledCircle(-0.8, 0.8, .11);
    drawFilledCircle(-0.8 + 0.15, 0.8, .12);
    drawFilledCircle(-0.8 + 0.3, 0.8, .09);
    //little cloud
    drawFilledCircle(0.7 - 0.15, 0.5, .08);
    drawFilledCircle(0.7, 0.5, .11);
    drawFilledCircle(0.7 + 0.15, 0.5, .12);
    drawFilledCircle(0.7 + 0.3, 0.5, .09);
    //little tiny cloud down
    drawFilledCircle(0.3 - 0.06, 0.2, .05);
    drawFilledCircle(0.3, 0.2, .07);
    drawFilledCircle(0.3 + 0.08, 0.2, .08);
    drawFilledCircle(0.3 + 0.16, 0.2, .06);

    drawFilledCircle(-0.9 - 0.06, 0.3, .05);
    drawFilledCircle(-0.9, 0.3, .07);
    drawFilledCircle(-0.9 + 0.08, 0.3, .08);
    drawFilledCircle(-0.9 + 0.16, 0.3, .06);

    glBegin(GL_QUADS);
    glColor3d(0.5, 0.3, 0.1);
    glVertex2f(1, -0.9);
    glVertex2f(-1., -0.9);
    glColor4d(0.2, 0., 0., 0.7);
    glVertex2f(-1., -1);
    glVertex2f(1, -1);
    glEnd();
    //lines
    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(-1, -.9);
    glVertex2f(-.7, -.9);
    glVertex2f(-.7, -1);
    glVertex2f(-1, -1);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(-.7, -.9);
    glVertex2f(-.4, -.9);
    glVertex2f(-.4, -1);
    glVertex2f(-.7, -1);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(-.4, -.9);
    glVertex2f(-.1, -.9);
    glVertex2f(-.1, -1);
    glVertex2f(-.4, -1);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(-.1, -.9);
    glVertex2f(0.2, -.9);
    glVertex2f(.2, -1);
    glVertex2f(-.1, -1);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(0.2, -.9);
    glVertex2f(0.5, -.9);
    glVertex2f(.5, -1);
    glVertex2f(0.2, -1);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(0.5, -.9);
    glVertex2f(0.8, -.9);
    glVertex2f(.8, -1);
    glVertex2f(0.5, -1);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(0.8, -.9);
    glVertex2f(1, -.9);
    glVertex2f(1, -1);
    glVertex2f(0.8, -1);
    glEnd();
    //mid
    glBegin(GL_QUADS);
    glColor3d(0.5, 0.3, 0.1);
    glVertex2f(1, -0.8);
    glVertex2f(-1., -0.8);
    glColor4d(0.2, 0., 0., 0.7);
    glVertex2f(-1., -0.9);
    glVertex2f(1, -0.9);
    glEnd();
    //lines
    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-1, -0.8);
    glVertex2f(-.8, -0.8);
    glVertex2f(-.8, -0.9);
    glVertex2f(-1, -0.9);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-0.8, -0.8);
    glVertex2f(-.5, -0.8);
    glVertex2f(-.5, -0.9);
    glVertex2f(-0.8, -0.9);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-0.5, -0.8);
    glVertex2f(-.2, -0.8);
    glVertex2f(-.2, -0.9);
    glVertex2f(-0.5, -0.9);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-0.2, -0.8);
    glVertex2f(0.1, -0.8);
    glVertex2f(0.1, -0.9);
    glVertex2f(-0.2, -0.9);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(0.1, -0.8);
    glVertex2f(0.4, -0.8);
    glVertex2f(0.4, -0.9);
    glVertex2f(0.1, -0.9);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(0.4, -0.8);
    glVertex2f(0.7, -0.8);
    glVertex2f(0.7, -0.9);
    glVertex2f(0.4, -0.9);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(0.7, -0.8);
    glVertex2f(1, -0.8);
    glVertex2f(1, -0.9);
    glVertex2f(0.7, -0.9);
    glEnd();
    //up
    glBegin(GL_QUADS);
    glColor3d(0.5, 0.3, 0.1);
    glVertex2f(1, -0.7);
    glVertex2f(-1., -0.7);
    glColor4d(0.2, 0., 0., 0.7);
    glVertex2f(-1., -0.8);
    glVertex2f(1, -0.8);
    glEnd();
    //lines
    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(-1, -.7);
    glVertex2f(-.7, -.7);
    glVertex2f(-.7, -0.8);
    glVertex2f(-1, -0.8);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(-.7, -.7);
    glVertex2f(-.4, -.7);
    glVertex2f(-.4, -0.8);
    glVertex2f(-.7, -.8);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(-.4, -.7);
    glVertex2f(-.1, -.7);
    glVertex2f(-.1, -0.8);
    glVertex2f(-.4, -0.8);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(-.1, -.7);
    glVertex2f(0.2, -.7);
    glVertex2f(.2, -0.8);
    glVertex2f(-.1, -.8);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(0.2, -.7);
    glVertex2f(0.5, -.7);
    glVertex2f(.5, -.8);
    glVertex2f(0.2, -.8);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(0.5, -.7);
    glVertex2f(0.8, -.7);
    glVertex2f(.8, -.8);
    glVertex2f(0.5, -.8);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(0.8, -.7);
    glVertex2f(1, -.7);
    glVertex2f(1, -.8);
    glVertex2f(0.8, -.8);
    glEnd();
    //apove up
    glBegin(GL_QUADS);
    glColor3d(0.5, 0.3, 0.1);
    glVertex2f(1, -0.6);
    glVertex2f(-1., -0.6);
    glColor4d(0.2, 0., 0., 0.7);
    glVertex2f(-1., -0.7);
    glVertex2f(1, -0.7);
    glEnd();
    //lines
    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-1, -0.6);
    glVertex2f(-.8, -0.6);
    glVertex2f(-.8, -0.7);
    glVertex2f(-1, -0.7);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-0.8, -0.6);
    glVertex2f(-.5, -0.6);
    glVertex2f(-.5, -0.7);
    glVertex2f(-0.8, -0.7);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-0.5, -0.6);
    glVertex2f(-.2, -0.6);
    glVertex2f(-.2, -0.7);
    glVertex2f(-0.5, -0.7);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-0.2, -0.6);
    glVertex2f(0.1, -0.6);
    glVertex2f(0.1, -0.7);
    glVertex2f(-0.2, -0.7);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(0.1, -0.6);
    glVertex2f(0.4, -0.6);
    glVertex2f(0.4, -0.7);
    glVertex2f(0.1, -0.7);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(0.4, -0.6);
    glVertex2f(0.7, -0.6);
    glVertex2f(0.7, -0.7);
    glVertex2f(0.4, -0.7);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(0.7, -0.6);
    glVertex2f(1, -0.6);
    glVertex2f(1, -0.7);
    glVertex2f(0.7, -0.7);
    glEnd();

    /////«·«—÷ «Œ÷—
    glBegin(GL_QUADS);
    glColor3d(0.4, 0.6, 0.);
    glVertex2f(1.0, -0.7);
    glVertex2f(-1, -0.7);
    glColor3d(0.4, 0.7, 0.2);
    glVertex2f(-1, -0.6);
    glVertex2f(1, -0.6);
    glEnd();
    ///«·«‘Ã«— «Œ÷— ›« Õ
    glColor3d(0.5, 0.7, 0.3);
    drawFilledCircle1(-0.9, -0.6, 0.2);
    drawFilledCircle1(-0.7, -0.6, 0.3);
    drawFilledCircle1(-0.4, -0.6, 0.17);
    drawFilledCircle1(-0.16, -0.6, 0.35);
    drawFilledCircle1(0.04, -0.6, 0.27);
    drawFilledCircle1(0.6, -0.6, 0.35);
    drawFilledCircle1(0.9, -0.6, 0.27);
    ////«·«‘Ã«— «·«Œ÷— «·€«„ﬁ
    glColor3d(0.4, 0.6, 0.2);
    drawFilledCircle1(-0.9, -0.6, 0.09);
    drawFilledCircle1(-0.7, -0.6, 0.2);
    drawFilledCircle1(-0.4, -0.6, 0.17);
    drawFilledCircle1(-0.16, -0.6, 0.25);
    drawFilledCircle1(0.04, -0.6, 0.17);
    drawFilledCircle1(0.4, -0.6, 0.25);
    drawFilledCircle1(.7, -0.6, 0.17);
    drawFilledCircle1(.9, -0.6, 0.23);



    //winter
    glBegin(GL_QUADS);
    glColor3d(0.6, 0.8, 1.0);
    glVertex2f(-0.6f, -0.0f);
    glVertex2f(-0.1f, -0.0f);
    glVertex2f(-0.1f, 0.4f);
    glVertex2f(-0.6f, 0.4f);
    glEnd();


    //summer
    glBegin(GL_QUADS);
    glColor3d(1.0, 1.0, 0.4);
    glVertex2f(-0.6f, -0.1f);
    glVertex2f(-0.1f, -0.1f);
    glVertex2f(-0.1f, -0.5f);
    glVertex2f(-0.6f, -0.5f);
    glEnd();


    //autummn
    glBegin(GL_QUADS);
    glColor3d(1.0, 0.8, 0.0);
    glVertex2f(0.6f, -0.1f);
    glVertex2f(0.1f, -0.1f);
    glVertex2f(0.1f, -0.5f);
    glVertex2f(0.6f, -0.5f);
    glEnd();

    //spring
    glBegin(GL_QUADS);
    glColor3d(0.7, 0.9, 0.5);
    glVertex2f(0.6f, 0.0f);
    glVertex2f(0.1f, 0.0f);
    glVertex2f(0.1f, 0.4f);
    glVertex2f(0.6f, 0.4f);
    glEnd();

    //autumn
    glBegin(GL_LINE_LOOP);
    glColor3d(0.0, 0.0, 0.0);
    glVertex2f(-0.6f, -0.0f);
    glVertex2f(-0.1f, -0.0f);
    glVertex2f(-0.1f, 0.4f);
    glVertex2f(-0.6f, 0.4f);
    glEnd();


    //summer
    glBegin(GL_LINE_LOOP);
    glColor3d(0.0, 0.0, 0.0);
    glVertex2f(-0.6f, -0.1f);
    glVertex2f(-0.1f, -0.1f);
    glVertex2f(-0.1f, -0.5f);
    glVertex2f(-0.6f, -0.5f);
    glEnd();

    //winter
    glBegin(GL_LINE_LOOP);
    glColor3d(0.0, 0.0, 0.0);
    glVertex2f(0.6f, -0.1f);
    glVertex2f(0.1f, -0.1f);
    glVertex2f(0.1f, -0.5f);
    glVertex2f(0.6f, -0.5f);
    glEnd();

    //spring
    glBegin(GL_LINE_LOOP);
    glColor3d(0.0, 0.0, 0.0);
    glVertex2f(0.6f, 0.0f);
    glVertex2f(0.1f, 0.0f);
    glVertex2f(0.1f, 0.4f);
    glVertex2f(0.6f, 0.4f);
    glEnd();




    //To write on the screen
    glColor3f(0.0, 0.0, 0.0);
    glRasterPos3f(-0.59, 0.16, 0);
    const char* text1 = "  WINTER     ";
    while (*text1) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *text1);
        text1++;
    }
    glColor3f(0.0, 0.0, 0.0);
    glRasterPos3f(0.18, 0.16, 0);
    const char* text2 = "SPRING";
    while (*text2) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *text2);
        text2++;
    }
    glColor3f(0.0, 0.0, 0.0);
    glRasterPos3f(-0.56, -0.33, 0);
    const char* text3 = "SUMMER";
    while (*text3) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *text3);
        text3++;
    }

    glColor3f(0.0, 0.0, 0.0);
    glRasterPos3f(0.13, -0.33, 0);
    const char* text4 = "AUTUMN";
    while (*text4) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *text4);
        text4++;
    }
}

void summer() {
    glBegin(GL_QUADS);
    glColor3d(0.789, 0.937, 0.968);
    glVertex2f(1.0, 0.5);//v0
    glColor4d(0.97, 0.964, 0.949, 0.77);
    glVertex2f(1.0, 1.0);//v1
    glVertex2f(-1.0, 1.0);//v2
    glColor3d(0.789, 0.937, 0.968);
    glVertex2f(-1.0, 0.5);//v3
    glEnd();


    //SUN
    glColor4f(0.99f, 0.79f, 0.20f, 0.9f);
    drawFilledCircle(0.01, 0.7, .20);
    glColor4f(0.99f, 0.9f, 0.39f, 1.0f);
    drawFilledCircle(0.01, 0.7, .16);


    //CLOUDS:
    // right cloud
    glColor3f(1.0f, 1.0f, 1.0f);
    drawFilledCircle(0.43, 0.8, .10);
    drawFilledCircle(0.58, 0.8, .13);
    drawFilledCircle(0.7, 0.8, .14);
    drawFilledCircle(0.85, 0.8, .10);
    // left cloud
    glColor3f(1.0f, 1.0f, 1.0f);
    drawFilledCircle(-0.43, 0.8, .10);
    drawFilledCircle(-0.58, 0.8, .13);
    drawFilledCircle(-0.7, 0.8, .14);
    drawFilledCircle(-0.85, 0.8, .10);
    ////little right cloud
    glColor3f(1.0f, 1.0f, 1.0f);
    drawFilledCircle(0.3, 0.5, .08);
    drawFilledCircle(0.38, 0.5, .09);
    drawFilledCircle(0.5, 0.5, .10);
    drawFilledCircle(0.59, 0.5, .08);
    ////little left cloud
    glColor3f(1.0f, 1.0f, 1.0f);
    drawFilledCircle(-0.3, 0.5, .08);
    drawFilledCircle(-0.38, 0.5, .09);
    drawFilledCircle(-0.5, 0.5, .10);
    drawFilledCircle(-0.59, 0.5, .08);

    //SAND:
    //first layer
    glBegin(GL_POLYGON);
    glColor3f(0.988, 0.796, 0.003);
    glVertex2f(-1.0, -0.1);//v0
    glVertex2f(1.0, -0.1);//v1
    glColor3f(0.996, 0.906, 0.550);
    glVertex2f(1.0, 0.18);//v2
    glVertex2f(0.9, 0.17);//v3
    glVertex2f(0.8, 0.16);//4
    glVertex2f(0.7, 0.16);//5
    glVertex2f(0.6, 0.17);//6
    glVertex2f(0.5, 0.17);//7
    glVertex2f(0.4, 0.18);//8
    glVertex2f(0.3, 0.175);//9
    glVertex2f(0.2, 0.165);//10
    glVertex2f(0.1, 0.15);//11
    glVertex2f(0.0, 0.13);//12
    glVertex2f(-0.1, 0.12);//13
    glVertex2f(-0.2, 0.1);//14
    glVertex2f(-0.3, 0.095);//15
    glVertex2f(-0.4, 0.095);//16
    glVertex2f(-0.5, 0.11);//17
    glVertex2f(-0.6, 0.14);//18
    glVertex2f(-0.7, 0.17);//19
    glVertex2f(-0.8, 0.18);//20
    glVertex2f(-0.9, 0.18);//21
    glVertex2f(-1.0, 0.165);//22
    glEnd();
    //second layer
    glBegin(GL_POLYGON);
    glColor3f(0.988, 0.886, 0.167);
    glVertex2f(-1.0, -0.25);//v0
    glVertex2f(1.0, -0.25);//v1
    glColor3f(0.996, 0.925, 0.601);
    glVertex2f(1.0, 0.055);//v2
    glVertex2f(0.9, 0.055);//v3
    glVertex2f(0.8, 0.07);//4
    glVertex2f(0.7, 0.11);//5
    glVertex2f(0.6, 0.12);//6
    glVertex2f(0.5, 0.11);//7
    glVertex2f(0.4, 0.085);//8
    glVertex2f(0.3, 0.07);//9
    glVertex2f(0.2, 0.07);//10
    glVertex2f(0.1, 0.1);//11
    glVertex2f(0.0, 0.112);//12
    glVertex2f(-0.1, 0.1);//13
    glVertex2f(-0.2, 0.07);//14
    glVertex2f(-0.3, 0.04);//15
    glVertex2f(-0.4, 0.02);//16
    glVertex2f(-0.5, 0.01);//17
    glVertex2f(-0.6, 0.015);//18
    glVertex2f(-0.7, 0.025);//19
    glVertex2f(-0.8, 0.045);//20
    glVertex2f(-0.9, 0.06);//21
    glVertex2f(-1.0, 0.07);//22
    glEnd();
    //third layer 
    glBegin(GL_POLYGON);
    glColor4f(0.964, 0.687, 0.296, 0.6);
    glVertex2f(-1.0, -0.35);//v0
    glVertex2f(1.0, -0.35);//v1
    glColor4f(0.996, 0.875, 0.511, 0.9);
    glVertex2f(1.0, 0.0);//v2
    glVertex2f(0.9, 0.01);//v3
    glVertex2f(0.8, 0.005);//4
    glVertex2f(0.7, -0.01);//5
    glVertex2f(0.6, -0.02);//6
    glVertex2f(0.5, 0.0);//7
    glVertex2f(0.4, 0.015);//8
    glVertex2f(0.3, 0.02);//9
    glVertex2f(0.2, 0.02);//10
    glVertex2f(0.1, 0.01);//11
    glVertex2f(0.0, -0.01);//12
    glVertex2f(-0.1, -0.04);//13
    glVertex2f(-0.2, -0.06);//14
    glVertex2f(-0.3, -0.08);//15
    glVertex2f(-0.4, -0.09);//16
    glVertex2f(-0.5, -0.085);//17
    glVertex2f(-0.6, -0.1);//18
    glEnd();

    //fourth layer 
    glBegin(GL_POLYGON);
    glColor3f(0.867, 0.707, 0.460);
    glVertex2f(-1.0, -0.35);//v0
    glVertex2f(1.0, -0.35);//v1
    glColor3f(0.996, 0.925, 0.601);
    glVertex2f(1.0, -0.16);//v2
    glVertex2f(0.9, -0.12);//v3
    glVertex2f(0.8, -0.09);//4
    glVertex2f(0.7, -0.075);//5
    glVertex2f(0.6, -0.07);//6
    glVertex2f(0.5, -0.075);//7
    glVertex2f(0.4, -0.085);//8
    glVertex2f(0.3, -0.1);//9
    glVertex2f(0.2, -0.14);//10
    glVertex2f(0.1, -0.15);//11
    glVertex2f(0.0, -0.16);//12
    glVertex2f(-0.1, -0.17);//13
    glVertex2f(-0.2, -0.16);//14
    glVertex2f(-0.3, -0.15);//15
    glVertex2f(-0.4, -0.13);//16
    glVertex2f(-0.5, -0.1);//17
    glVertex2f(-0.6, -0.07);//18
    glVertex2f(-0.7, -0.05);//19
    glVertex2f(-0.8, -0.035);//20
    glVertex2f(-0.9, -0.04);//21
    glVertex2f(-1.0, -0.05);//22
    glEnd();

    ////fifth layer 
    glBegin(GL_POLYGON);
    glColor3f(0.996, 0.992, 0.558);
    glVertex2f(-1.0, -0.35);//v0
    glVertex2f(1.0, -0.35);//v1
    glColor3f(0.996, 0.992, 0.75);
    glVertex2f(1.0, -0.24);//v2
    glVertex2f(0.9, -0.21);//v3
    glVertex2f(0.8, -0.22);//4
    glVertex2f(0.7, -0.23);//5
    glVertex2f(0.6, -0.24);//6
    glVertex2f(0.5, -0.2);//7
    glVertex2f(0.4, -0.14);//8
    glVertex2f(0.3, -0.11);//9
    glVertex2f(0.2, -0.1);//10
    glVertex2f(0.1, -0.1);//11
    glVertex2f(0.0, -0.11);//12
    glVertex2f(-0.1, -0.13);//13
    glVertex2f(-0.2, -0.16);//14
    glVertex2f(-0.3, -0.21);//15
    glVertex2f(-0.4, -0.22);//16
    glVertex2f(-0.5, -0.23);//17
    glVertex2f(-0.6, -0.19);//18
    glVertex2f(-0.7, -0.18);//19
    glVertex2f(-0.8, -0.16);//20
    glVertex2f(-0.9, -0.14);//21
    glVertex2f(-1.0, -0.13);//22
    glEnd();


    //PALMS:
    ////right palm
    //// tunks
    //trunk1
    glBegin(GL_POLYGON);
    glColor3d(0.5742, 0.4375, 0.2656);
    glVertex2f(0.79, 0.005);//v0
    glVertex2f(0.86, 0.01);//v1
    glVertex2f(0.87, 0.07);//v2
    glVertex2f(0.78, 0.05);//v3
    glEnd();
    //trunk2
    glBegin(GL_TRIANGLES);
    glColor3d(0.5742, 0.4375, 0.2656);
    glVertex2f(0.82, 0.03);//v0
    glVertex2f(0.85, 0.14);//v1
    glVertex2f(0.78, 0.11);//v2
    glEnd();
    //trunk3
    glBegin(GL_TRIANGLES);
    glColor3d(0.5742, 0.4375, 0.2656);
    glVertex2f(0.81, 0.1);//v0
    glVertex2f(0.83, 0.185);//v1
    glVertex2f(0.77, 0.165);//v2
    glEnd();
    //trunk4
    glBegin(GL_TRIANGLES);
    glColor3d(0.5742, 0.4375, 0.2656);
    glVertex2f(0.8, 0.16);//v0
    glVertex2f(0.81, 0.23);//v1
    glVertex2f(0.75, 0.21);//v2
    glEnd();
    //trunk5
    glBegin(GL_TRIANGLES);
    glColor3d(0.5742, 0.4375, 0.2656);
    glVertex2f(0.79, 0.2);//v0
    glVertex2f(0.8, 0.27);//v1
    glVertex2f(0.73, 0.25);//v2
    glEnd();
    //trunk6
    glBegin(GL_TRIANGLES);
    glColor3d(0.5742, 0.4375, 0.2656);
    glVertex2f(0.775, 0.23);//v0
    glVertex2f(0.78, 0.3);//v1
    glVertex2f(0.72, 0.28);//v2
    glEnd();
    //trunk7
    glBegin(GL_TRIANGLES);
    glColor3d(0.5742, 0.4375, 0.2656);
    glVertex2f(0.76, 0.27);//v0
    glVertex2f(0.76, 0.33);//v1
    glVertex2f(0.71, 0.31);//v2
    glEnd();
    //trunk8
    glBegin(GL_TRIANGLES);
    glColor3d(0.5742, 0.4375, 0.2656);
    glVertex2f(0.75, 0.3);//v0
    glVertex2f(0.74, 0.36);//v1
    glVertex2f(0.7, 0.34);//v2
    glEnd();
    /////right palm leaves
    //leave1
    glBegin(GL_POLYGON);
    glColor3d(0.671, 0.679, 0.199);
    glVertex2f(0.68, 0.32);//v0
    glVertex2f(0.67, 0.34);//v1
    glVertex2f(0.675, 0.36);//v2
    glVertex2f(0.63, 0.34);//v3
    glVertex2f(0.6, 0.32);//v4
    glVertex2f(0.57, 0.28);//v5
    glVertex2f(0.56, 0.25);//v6
    glVertex2f(0.555, 0.2);//v7
    glVertex2f(0.55, 0.17);//v8
    glEnd();
    //leave2
    glBegin(GL_POLYGON);
    glColor3d(0.816, 0.789, 0.339);
    glVertex2f(0.67, 0.36);//v0
    glVertex2f(0.63, 0.36);//v1
    glVertex2f(0.6, 0.37);//v2
    glVertex2f(0.55, 0.335);//v3
    glVertex2f(0.52, 0.3);//v4
    glVertex2f(0.5, 0.275);//v5
    glVertex2f(0.49, 0.24);//v6
    glVertex2f(0.57, 0.28);//v7
    glVertex2f(0.6, 0.32);//v8
    glVertex2f(0.63, 0.34);//v9
    glEnd();
    //leave3
    glBegin(GL_POLYGON);
    glColor3d(0.671, 0.679, 0.199);
    glVertex2f(0.67, 0.36);//v0
    glVertex2f(0.69, 0.38);//v1
    glVertex2f(0.72, 0.38);//v2
    glVertex2f(0.7, 0.4);//v3
    glVertex2f(0.67, 0.42);//v4
    glVertex2f(0.63, 0.44);//v5
    glVertex2f(0.6, 0.44);//v6
    glVertex2f(0.55, 0.425);//v7
    glVertex2f(0.52, 0.41);//v8
    glVertex2f(0.46, 0.39);//v9
    glEnd();
    //leave4
    glBegin(GL_POLYGON);
    glColor3d(0.671, 0.679, 0.199);
    glVertex2f(0.72, 0.38);//v0
    glVertex2f(0.73, 0.37);//v1
    glVertex2f(0.77, 0.4);//v2
    glVertex2f(0.9, 0.54);//v3
    glVertex2f(0.85, 0.54);//v4
    glVertex2f(0.8, 0.53);//v5
    glVertex2f(0.75, 0.49);//v6
    glVertex2f(0.73, 0.45);//v7
    glVertex2f(0.72, 0.4);//v8
    glEnd();
    //leave5
    glBegin(GL_POLYGON);
    glColor3d(0.816, 0.789, 0.339);
    glVertex2f(0.73, 0.37);//v0
    glVertex2f(0.8, 0.36);//v1
    glVertex2f(0.85, 0.35);//v2
    glVertex2f(0.92, 0.34);//v3
    glVertex2f(0.99, 0.35);//v4
    glVertex2f(0.95, 0.37);//v5
    glVertex2f(0.9, 0.4);//v6
    glVertex2f(0.86, 0.41);//v7
    glVertex2f(0.8, 0.405);//v8
    glVertex2f(0.76, 0.4);//v9
    glEnd();
    //leave6
    glBegin(GL_POLYGON);
    glColor3d(0.671, 0.679, 0.199);
    glVertex2f(0.73, 0.34);//v0
    glVertex2f(0.78, 0.31);//v1
    glVertex2f(0.84, 0.27);//v2
    glVertex2f(0.89, 0.24);//v3
    glVertex2f(0.95, 0.19);//v4
    glVertex2f(0.935, 0.22);//v5
    glVertex2f(0.915, 0.27);//v6
    glVertex2f(0.9, 0.3);//v7
    glVertex2f(0.86, 0.34);//v8
    glVertex2f(0.82, 0.36);//v9
    glVertex2f(0.8, 0.365);//10
    glVertex2f(0.76, 0.36);//11
    glVertex2f(0.76, 0.35);//12
    glEnd();
    //coconut1
    glColor3f(0.421f, 0.277f, 0.156f);
    drawFilledCircle(0.72, 0.34, 0.045);
    //lift palm
    //// tunks
    //trunk1
    glBegin(GL_POLYGON);
    glColor3d(0.5742, 0.4375, 0.2656);
    glVertex2f(-0.79, 0.005);//v0
    glVertex2f(-0.86, 0.01);//v1
    glVertex2f(-0.87, 0.07);//v2
    glVertex2f(-0.78, 0.05);//v3
    glEnd();
    //trunk2
    glBegin(GL_TRIANGLES);
    glColor3d(0.5742, 0.4375, 0.2656);
    glVertex2f(-0.82, 0.03);//v0
    glVertex2f(-0.85, 0.13);//v1
    glVertex2f(-0.78, 0.11);//v2
    glEnd();
    //trunk3
    glBegin(GL_TRIANGLES);
    glColor3d(0.5742, 0.4375, 0.2656);
    glVertex2f(-0.81, 0.1);//v0
    glVertex2f(-0.83, 0.185);//v1
    glVertex2f(-0.77, 0.165);//v2
    glEnd();
    //trunk4
    glBegin(GL_TRIANGLES);
    glColor3d(0.5742, 0.4375, 0.2656);
    glVertex2f(-0.8, 0.16);//v0
    glVertex2f(-0.81, 0.23);//v1
    glVertex2f(-0.75, 0.21);//v2
    glEnd();
    //trunk5
    glBegin(GL_TRIANGLES);
    glColor3d(0.5742, 0.4375, 0.2656);
    glVertex2f(-0.79, 0.2);//v0
    glVertex2f(-0.8, 0.27);//v1
    glVertex2f(-0.73, 0.25);//v2
    glEnd();
    //trunk6
    glBegin(GL_TRIANGLES);
    glColor3d(0.5742, 0.4375, 0.2656);
    glVertex2f(-0.775, 0.23);//v0
    glVertex2f(-0.78, 0.3);//v1
    glVertex2f(-0.72, 0.28);//v2
    glEnd();
    //trunk7
    glBegin(GL_TRIANGLES);
    glColor3d(0.5742, 0.4375, 0.2656);
    glVertex2f(-0.76, 0.27);//v0
    glVertex2f(-0.76, 0.33);//v1
    glVertex2f(-0.71, 0.31);//v2
    glEnd();
    //trunk8
    glBegin(GL_TRIANGLES);
    glColor3d(0.5742, 0.4375, 0.2656);
    glVertex2f(-0.75, 0.3);//v0
    glVertex2f(-0.74, 0.36);//v1
    glVertex2f(-0.7, 0.34);//v2
    glEnd();

    /////lift palm leaves
    //leave1
    glBegin(GL_POLYGON);
    glColor3d(0.671, 0.679, 0.199);
    glVertex2f(-0.68, 0.32);//v0
    glVertex2f(-0.67, 0.34);//v1
    glVertex2f(-0.675, 0.36);//v2
    glVertex2f(-0.63, 0.34);//v3
    glVertex2f(-0.6, 0.32);//v4
    glVertex2f(-0.57, 0.28);//v5
    glVertex2f(-0.56, 0.25);//v6
    glVertex2f(-0.555, 0.2);//v7
    glVertex2f(-0.55, 0.17);//v8
    glEnd();
    //leave2
    glBegin(GL_POLYGON);
    glColor3d(0.816, 0.789, 0.339);
    glVertex2f(-0.67, 0.36);//v0
    glVertex2f(-0.63, 0.36);//v1
    glVertex2f(-0.6, 0.37);//v2
    glVertex2f(-0.55, 0.335);//v3
    glVertex2f(-0.52, 0.3);//v4
    glVertex2f(-0.5, 0.275);//v5
    glVertex2f(-0.49, 0.24);//v6
    glVertex2f(-0.57, 0.28);//v7
    glVertex2f(-0.6, 0.32);//v8
    glVertex2f(-0.63, 0.34);//v9
    glEnd();
    //leave3
    glBegin(GL_POLYGON);
    glColor3d(0.671, 0.679, 0.199);
    glVertex2f(-0.67, 0.36);//v0
    glVertex2f(-0.69, 0.38);//v1
    glVertex2f(-0.72, 0.38);//v2
    glVertex2f(-0.7, 0.4);//v3
    glVertex2f(-0.67, 0.42);//v4
    glVertex2f(-0.63, 0.44);//v5
    glVertex2f(-0.6, 0.44);//v6
    glVertex2f(-0.55, 0.425);//v7
    glVertex2f(-0.52, 0.41);//v8
    glVertex2f(-0.46, 0.39);//v9
    glEnd();
    //leave4
    glBegin(GL_POLYGON);
    glColor3d(0.671, 0.679, 0.199);
    glVertex2f(-0.72, 0.38);//v0
    glVertex2f(-0.73, 0.37);//v1
    glVertex2f(-0.77, 0.4);//v2
    glVertex2f(-0.9, 0.54);//v3
    glVertex2f(-0.85, 0.54);//v4
    glVertex2f(-0.8, 0.53);//v5
    glVertex2f(-0.75, 0.49);//v6
    glVertex2f(-0.73, 0.45);//v7
    glVertex2f(-0.72, 0.4);//v8
    glEnd();
    //leave5
    glBegin(GL_POLYGON);
    glColor3d(0.816, 0.789, 0.339);
    glVertex2f(-0.73, 0.37);//v0
    glVertex2f(-0.8, 0.36);//v1
    glVertex2f(-0.85, 0.35);//v2
    glVertex2f(-0.92, 0.34);//v3
    glVertex2f(-0.99, 0.35);//v4
    glVertex2f(-0.95, 0.37);//v5
    glVertex2f(-0.9, 0.4);//v6
    glVertex2f(-0.86, 0.41);//v7
    glVertex2f(-0.8, 0.405);//v8
    glVertex2f(-0.76, 0.4);//v9
    glEnd();
    //leave6
    glBegin(GL_POLYGON);
    glColor3d(0.671, 0.679, 0.199);
    glVertex2f(-0.73, 0.34);//v0
    glVertex2f(-0.78, 0.31);//v1
    glVertex2f(-0.84, 0.27);//v2
    glVertex2f(-0.89, 0.24);//v3
    glVertex2f(-0.95, 0.19);//v4
    glVertex2f(-0.935, 0.22);//v5
    glVertex2f(-0.915, 0.27);//v6
    glVertex2f(-0.9, 0.3);//v7
    glVertex2f(-0.86, 0.34);//v8
    glVertex2f(-0.82, 0.36);//v9
    glVertex2f(-0.8, 0.365);//10
    glVertex2f(-0.76, 0.36);//11
    glVertex2f(-0.76, 0.35);//12
    glEnd();

    //coconut
    glColor3f(0.421f, 0.277f, 0.156f);
    drawFilledCircle(-0.72, 0.34, 0.045);



    //UMBRELLA:
    //righr umbrella 
    //left side
    glBegin(GL_POLYGON);
    glColor3d(0.5898, 0.8281, 0.8906);
    glVertex2f(0.18, 0.11);//v0
    glVertex2f(0.2, 0.08);//v1
    glVertex2f(0.22, 0.05);//v2
    glVertex2f(0.25, 0.09);//v3
    glVertex2f(0.3, 0.14);//v4
    glVertex2f(0.34, 0.15);//v5
    glVertex2f(0.3, 0.165);//v6
    glVertex2f(0.25, 0.158);//v7
    glVertex2f(0.2, 0.13);//v8
    glEnd();
    // middle side
    glBegin(GL_POLYGON);
    glColor3d(0.9843, 0.3398, 0.3398);
    glVertex2f(0.22, 0.05);//v0
    glVertex2f(0.3, 0.03);//v1
    glVertex2f(0.36, 0.0);//v2
    glVertex2f(0.36, 0.05);//v3
    glVertex2f(0.355, 0.1);//v4
    glVertex2f(0.35, 0.145);//v5
    glVertex2f(0.33, 0.16);//v6
    glVertex2f(0.3, 0.14);//v7
    glVertex2f(0.25, 0.09);//v8
    glEnd();
    // right side
    glBegin(GL_POLYGON);
    glColor3d(0.5898, 0.8281, 0.8906);
    glVertex2f(0.36, 0.0);//v0
    glVertex2f(0.4, 0.01);//v1
    glVertex2f(0.43, 0.02);//v2
    glVertex2f(0.47, 0.01);//v3
    glVertex2f(0.45, 0.07);//v4
    glVertex2f(0.43, 0.1);//v5
    glVertex2f(0.39, 0.13);//v6
    glVertex2f(0.34, 0.15);//v7
    glVertex2f(0.355, 0.1);//v8
    glVertex2f(0.36, 0.05);//v9
    glEnd();
    //handdle of umbrella 
    glBegin(GL_QUADS);
    glColor3d(0.5742, 0.4375, 0.2656);
    glVertex2f(0.26, -0.12);//v0
    glVertex2f(0.27, -0.115);//v1
    glVertex2f(0.31, 0.02);//v2
    glVertex2f(0.3, 0.02);//v3
    //pointer of umbrella 
    glBegin(GL_QUADS);
    glColor3d(0.5742, 0.4375, 0.2656);
    glVertex2f(0.325, 0.155);//v0
    glVertex2f(0.344, 0.147);//v1
    glVertex2f(0.345, 0.17);//v2
    glVertex2f(0.345, 0.17);//v3
    glEnd();
    //left umbrella 
    //right side
    glBegin(GL_POLYGON);
    glColor3d(0.5898, 0.8281, 0.8906);
    glVertex2f(-0.18, 0.11);//v0
    glVertex2f(-0.2, 0.08);//v1
    glVertex2f(-0.22, 0.05);//v2
    glVertex2f(-0.25, 0.09);//v3
    glVertex2f(-0.3, 0.14);//v4
    glVertex2f(-0.34, 0.15);//v5
    glVertex2f(-0.3, 0.165);//v6
    glVertex2f(-0.25, 0.158);//v7
    glVertex2f(-0.2, 0.13);//v8
    glEnd();
    // middle side
    glBegin(GL_POLYGON);
    glColor3d(0.9843, 0.3398, 0.3398);
    glVertex2f(-0.22, 0.05);//v0
    glVertex2f(-0.3, 0.03);//v1
    glVertex2f(-0.36, 0.0);//v2
    glVertex2f(-0.36, 0.05);//v3
    glVertex2f(-0.355, 0.1);//v4
    glVertex2f(-0.35, 0.145);//v5
    glVertex2f(-0.33, 0.16);//v6
    glVertex2f(-0.3, 0.14);//v7
    glVertex2f(-0.25, 0.09);//v8
    glEnd();
    // left side
    glBegin(GL_POLYGON);
    glColor3d(0.5898, 0.8281, 0.8906);
    glVertex2f(-0.36, 0.0);//v0
    glVertex2f(-0.4, 0.01);//v1
    glVertex2f(-0.43, 0.02);//v2
    glVertex2f(-0.47, 0.01);//v3
    glVertex2f(-0.45, 0.07);//v4
    glVertex2f(-0.43, 0.1);//v5
    glVertex2f(-0.39, 0.13);//v6
    glVertex2f(-0.34, 0.15);//v7
    glVertex2f(-0.355, 0.1);//v8
    glVertex2f(-0.36, 0.05);//v9
    glEnd();
    //handdle of umbrella 
    glBegin(GL_QUADS);
    glColor3d(0.5742, 0.4375, 0.2656);
    glVertex2f(-0.26, -0.12);//v0
    glVertex2f(-0.27, -0.115);//v1
    glVertex2f(-0.31, 0.02);//v2
    glVertex2f(-0.3, 0.02);//v3
    //pointer of umbrella 
    glBegin(GL_QUADS);
    glColor3d(0.5742, 0.4375, 0.2656);
    glVertex2f(-0.325, 0.155);//v0
    glVertex2f(-0.344, 0.147);//v1
    glVertex2f(-0.345, 0.17);//v2
    glVertex2f(-0.345, 0.17);//v3
    glEnd();

    //BRIDGES:
   //1
    glBindTexture(GL_TEXTURE_2D, myTexture2);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glColor3d(0.738, 0.644, 0.511);
    glTexCoord2f(1.0, 0.0); glVertex2f(-0.63, -0.25);//v0                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                glColor4d(0.824, 0.886, 0.878, 0.8);
    glTexCoord2f(1.0, 1.0); glVertex2f(-0.63, -0.18);//v1
    glTexCoord2f(0.0, 1.0); glVertex2f(-0.99, -0.18);//v2
    glTexCoord2f(0.0, 0.0); glVertex2f(-0.99, -0.25);//v3
    glEnd();
    glDisable(GL_TEXTURE_2D);
    //right leg
    glBegin(GL_QUADS);
    glColor4d(0.253, 0.152, 0.011, 0.8);
    glVertex2f(-0.65, -1.0);//v0                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                glColor4d(0.824, 0.886, 0.878, 0.8);
    glVertex2f(-0.65, -0.25);//v1
    glVertex2f(-0.71, -0.25);//v2
    glVertex2f(-0.71, -1.0);//v3
    glEnd();
    //left leg
    glBegin(GL_QUADS);
    glColor4d(0.253, 0.152, 0.011, 0.8);
    glVertex2f(-0.91, -1.0);//v0                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                glColor4d(0.824, 0.886, 0.878, 0.8);
    glVertex2f(-0.91, -0.25);//v1
    glVertex2f(-0.97, -0.25);//v2
    glVertex2f(-0.97, -1.0);//v3
    glEnd();


    //2
    glBindTexture(GL_TEXTURE_2D, myTexture2);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glColor3d(0.738, 0.644, 0.511);
    glTexCoord2f(1.0, 0.0); glVertex2f(-0.36, -0.25);//v0                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                glColor4d(0.824, 0.886, 0.878, 0.8);
    glTexCoord2f(1.0, 1.0); glVertex2f(-0.36, -0.18);//v1
    glTexCoord2f(0.0, 1.0); glVertex2f(-0.61, -0.18);//v2
    glTexCoord2f(0.0, 0.0); glVertex2f(-0.61, -0.25);//v3
    glEnd();
    glDisable(GL_TEXTURE_2D);
    //right leg
    glBegin(GL_QUADS);
    glColor4d(0.253, 0.152, 0.011, 0.8);
    glVertex2f(-0.38, -1.0);//v0                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                glColor4d(0.824, 0.886, 0.878, 0.8);
    glVertex2f(-0.38, -0.25);//v1
    glVertex2f(-0.43, -0.25);//v2
    glVertex2f(-0.43, -1.0);//v3
    glEnd();
    //left leg
    glBegin(GL_QUADS);
    glColor4d(0.253, 0.152, 0.011, 0.8);
    glVertex2f(-0.54, -1.0);//v0                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                glColor4d(0.824, 0.886, 0.878, 0.8);
    glVertex2f(-0.54, -0.25);//v1
    glVertex2f(-0.59, -0.25);//v2
    glVertex2f(-0.59, -1.0);//v3
    glEnd();

    //3
    glBindTexture(GL_TEXTURE_2D, myTexture2);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glColor3d(0.738, 0.644, 0.511);
    glTexCoord2f(1.0, 0.0); glVertex2f(0.34, -0.25);//v0                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                glColor4d(0.824, 0.886, 0.878, 0.8);
    glTexCoord2f(1.0, 1.0); glVertex2f(0.34, -0.18);//v1
    glTexCoord2f(0.0, 1.0); glVertex2f(-0.2, -0.18);//v2
    glTexCoord2f(0.0, 0.0); glVertex2f(-0.2, -0.25);//v3
    glEnd();
    glDisable(GL_TEXTURE_2D);
    //right leg
    glBegin(GL_QUADS);
    glColor4d(0.253, 0.152, 0.011, 0.8);
    glVertex2f(0.3, -1.0);//v0                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                glColor4d(0.824, 0.886, 0.878, 0.8);
    glVertex2f(0.3, -0.25);//v1
    glVertex2f(0.23, -0.25);//v2
    glVertex2f(0.23, -1.0);//v3
    glEnd();
    //left leg
    glBegin(GL_QUADS);
    glColor4d(0.253, 0.152, 0.011, 0.8);
    glVertex2f(-0.09, -1.0);//v0                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                glColor4d(0.824, 0.886, 0.878, 0.8);
    glVertex2f(-0.09, -0.25);//v1
    glVertex2f(-0.16, -0.25);//v2
    glVertex2f(-0.16, -1.0);//v3
    glEnd();

    //4
    glBindTexture(GL_TEXTURE_2D, myTexture2);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glColor3d(0.738, 0.644, 0.511);
    glTexCoord2f(1.0, 0.0); glVertex2f(0.73, -0.25);//v0                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                glColor4d(0.824, 0.886, 0.878, 0.8);
    glTexCoord2f(1.0, 1.0); glVertex2f(0.73, -0.18);//v1
    glTexCoord2f(0.0, 1.0); glVertex2f(0.5, -0.18);//v2
    glTexCoord2f(0.0, 0.0); glVertex2f(0.5, -0.25);//v3
    glEnd();
    glDisable(GL_TEXTURE_2D);
    //right leg
    glBegin(GL_QUADS);
    glColor4d(0.253, 0.152, 0.011, 0.8);
    glVertex2f(0.71, -1.0);//v0                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                glColor4d(0.824, 0.886, 0.878, 0.8);
    glVertex2f(0.71, -0.25);//v1
    glVertex2f(0.66, -0.25);//v2
    glVertex2f(0.66, -1.0);//v3
    glEnd();
    //left leg
    glBegin(GL_QUADS);
    glColor4d(0.253, 0.152, 0.011, 0.8);
    glVertex2f(0.57, -1.0);//v0                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                glColor4d(0.824, 0.886, 0.878, 0.8);
    glVertex2f(0.57, -0.25);//v1
    glVertex2f(0.52, -0.25);//v2
    glVertex2f(0.52, -1.0);//v3
    glEnd();

    //5
    glBindTexture(GL_TEXTURE_2D, myTexture2);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glColor3d(0.738, 0.644, 0.511);
    glTexCoord2f(1.0, 0.0); glVertex2f(0.99, -0.25);//v0                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                glColor4d(0.824, 0.886, 0.878, 0.8);
    glTexCoord2f(1.0, 1.0); glVertex2f(0.99, -0.18);//v1
    glTexCoord2f(0.0, 1.0); glVertex2f(0.75, -0.18);//v2
    glTexCoord2f(0.0, 0.0); glVertex2f(0.75, -0.25);//v3
    glEnd();
    glDisable(GL_TEXTURE_2D);
    //right leg
    glBegin(GL_QUADS);
    glColor4d(0.253, 0.152, 0.011, 0.8);
    glVertex2f(0.97, -1.0);//v0                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                glColor4d(0.824, 0.886, 0.878, 0.8);
    glVertex2f(0.97, -0.25);//v1
    glVertex2f(0.92, -0.25);//v2
    glVertex2f(0.92, -1.0);//v3
    glEnd();
    //left leg
    glBegin(GL_QUADS);
    glColor4d(0.253, 0.152, 0.011, 0.8);
    glVertex2f(0.82, -1.0);//v0                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 glColor4d(0.824, 0.886, 0.878, 0.8);
    glVertex2f(0.82, -0.25);//v1
    glVertex2f(0.77, -0.25);//v2
    glVertex2f(0.77, -1.0);//v3
    glEnd();

    //SEA:
    glBegin(GL_QUADS);
    glColor4d(1.0, 1.0, 1.0, 0.65);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                glColor4d(0.824, 0.886, 0.878, 0.8);
    glVertex2f(1.0, -0.35);//v1
    glVertex2f(-1.0, -0.35);//v2
    glColor4d(0.125, 0.695, 0.664, 0.7);
    glVertex2f(-1.0, -1.0);//v3
    glVertex2f(1.0, -1.0);//v0
    glEnd();

    //REDFLAG:
    //line
    glBegin(GL_LINES);
    glColor3d(0, 0, 0);
    glVertex2f(0.85, 0.1);
    glVertex2f(0.85, -0.18);
    glEnd();
    //flag
    glBegin(GL_TRIANGLES);
    glColor3d(1, 0, 0);
    glVertex2f(0.85, 0.1);
    glVertex2f(0.95, 0.05);
    glVertex2f(0.85, 0.0);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(0, 0, 0);
    glVertex2f(0.85, 0.1);
    glVertex2f(0.95, 0.05);
    glVertex2f(0.85, 0.0);
    glEnd();

}

void autumn() {
    /////clouds/////
    glColor3f(1.0f, 1.0f, 1.0f);
    drawFilledCircle(0.3 - 0.15, 0.8, .12);
    drawFilledCircle(0.3, 0.8, .15);
    drawFilledCircle(0.3 + 0.15, 0.8, .16);
    drawFilledCircle(0.3 + 0.3, 0.8, .13);
    //cloud left
    glColor3f(1.0f, 1.0f, 1.0f);
    drawFilledCircle(-0.8 - 0.15, 0.8, .12);
    drawFilledCircle(-0.8, 0.8, .15);
    drawFilledCircle(-0.8 + 0.15, 0.8, .16);
    drawFilledCircle(-0.8 + 0.3, 0.8, .13);
    //little cloud
    glColor3f(1.0f, 1.0f, 1.0f);
    drawFilledCircle(-0.3 - 0.15, 0.5, .08);
    drawFilledCircle(-0.3, 0.5, .11);
    drawFilledCircle(-0.3 + 0.15, 0.5, .12);
    drawFilledCircle(-0.3 + 0.3, 0.5, .09);
    //little cloud
    glColor3f(1.0f, 1.0f, 1.0f);
    drawFilledCircle(0.7 - 0.15, 0.5, .08);
    drawFilledCircle(0.7, 0.5, .11);
    drawFilledCircle(0.7 + 0.15, 0.5, .12);
    drawFilledCircle(0.7 + 0.3, 0.5, .09);
    //little cloud down
    glColor3f(1.0f, 1.0f, 1.0f);
    drawFilledCircle(0.3 - 0.06, 0.2, .05);
    drawFilledCircle(0.3, 0.2, .07);
    drawFilledCircle(0.3 + 0.08, 0.2, .08);
    drawFilledCircle(0.3 + 0.16, 0.2, .06);

    glColor3f(1.0f, 1.0f, 1.0f);
    drawFilledCircle(-0.9 - 0.06, 0.3, .05);
    drawFilledCircle(-0.9, 0.3, .07);
    drawFilledCircle(-0.9 + 0.08, 0.3, .08);
    drawFilledCircle(-0.9 + 0.16, 0.3, .06);
    ////trees////
    // mid tree
    //tree leaf back
    glColor4f(0.9f, 0.5f, 0.0f, 0.92f);
    drawFilledCircle(0.0, 0.3, 0.19);
    /////tree leg
    glBegin(GL_POLYGON);
    glColor4f(0.4, 0.3, 0.2, 0.92f); // brown color
    glVertex2f(-0.06, -0.8);
    glVertex2f(-0.02, 0.16);;
    glVertex2f(0.02, 0.16);
    glVertex2f(0.06, -0.8);
    glEnd();
    //tree leaf front
    glColor4f(0.9f, 0.6f, 0.0f, 0.92f);
    drawFilledCircle(0.0, 0.3, 0.14);
    //
    //tree on left
    glColor4f(0.9f, 0.5f, 0.0f, 0.83f);
    drawFilledCircle(-0.67, 0.18, 0.16);
    /////tree leg
    glBegin(GL_POLYGON);
    glColor4f(0.4, 0.3, 0.2, 0.83f); // brown color
    glVertex2f(-0.63, -0.8);
    glVertex2f(-0.66, 0.11);;
    glVertex2f(-0.68, 0.11);
    glVertex2f(-0.75, -0.8);
    glEnd();
    //tree leaf front
    glColor4f(0.9f, 0.6f, 0.0f, 0.88f);
    drawFilledCircle(-0.67, 0.18, 0.13);
    //
    //tree on right
    glColor4f(0.9f, 0.5f, 0.0f, 0.8f);
    drawFilledCircle(0.6, 0.1, 0.14);
    /////tree leg
    glBegin(GL_POLYGON);
    glColor4f(0.4, 0.3, 0.2, 0.8f); // brown color
    glVertex2f(0.55, -1);
    glVertex2f(0.6, 0.06);;
    glVertex2f(0.61, 0.06);
    glVertex2f(0.66, -1);
    glEnd();
    //tree leaf front
    glColor4f(0.9f, 0.6f, 0.0f, 0.85f);
    drawFilledCircle(0.6, 0.1, 0.1);
    //
    //tree on right(little)
    glColor4f(0.9f, 0.5f, 0.0f, 0.65f);
    drawFilledCircle(0.31, -0.025, 0.08);
    /////tree leg
    glBegin(GL_POLYGON);
    glColor4f(0.4, 0.3, 0.0, 0.65f); // brown color
    glVertex2f(0.27, -1);
    glVertex2f(0.3, -0.055);;
    glVertex2f(0.32, -0.055);
    glVertex2f(0.35, -1);
    glEnd();
    //tree leaf front
    glColor4f(0.9f, 0.6f, 0.0f, 0.75f);
    drawFilledCircle(0.31, -0.025, 0.05);
    //
    //tree on left(little)
    glColor4f(0.9f, 0.5f, 0.0f, 0.59f);
    drawFilledCircle(-0.35, -0.075, 0.08);
    /////tree leg
    glBegin(GL_POLYGON);
    glColor4f(0.4, 0.3, 0.2, 0.59f); // brown color
    glVertex2f(-0.32, -1);
    glVertex2f(-0.34, -0.079);;
    glVertex2f(-0.36, -0.079);
    glVertex2f(-0.37, -1);
    glEnd();
    //tree leaf front
    glColor4f(0.9f, 0.6f, 0.0f, 0.65f);
    drawFilledCircle(-0.35, -0.075, 0.05);
    ////«·«⁄‘«»//// 
   //«·«⁄‘«» »— ﬁ«·Ì
    glColor3d(1.0, 0.7, 0.0);
    drawFilledCircle1(-0.68, -0.5, 0.17);
    drawFilledCircle1(-1, -0.5, 0.27);


    glColor3d(1.0, 0.7, 0.2);
    drawFilledCircle1(0.8, -0.5, 0.11);
    drawFilledCircle1(1, -0.5, 0.22);


    //// «·«⁄‘«» «„«„ÌÂ «’›—
    glColor3d(1.0, 0.8, 0.2);
    drawFilledCircle1(-0.68, -0.5, 0.13);
    drawFilledCircle1(-1, -0.5, 0.23);

    glColor3d(1.0, 0.8, 0.0);
    drawFilledCircle1(0.8, -0.5, 0.07);
    drawFilledCircle1(1, -0.5, 0.18);
    ////«·ÿÊ»////
    // Ì„Ì‰//
   //«·’› «·À«·À „‰  Õ  ÿÊ»
    glBegin(GL_QUAD_STRIP);
    glColor3d(0.6, 0.3, 0);
    glVertex2f(0.7, -0.7);
    glVertex2f(1, -0.7);
    glColor3d(0.3, 0.1, 0);
    glVertex2f(0.7, -0.8);
    glColor3d(0.3, 0.1, 0);
    glVertex2f(1, -0.8);
    glEnd();

    //«·’› «·À«·À „‰  Õ  Ì”«— ÿÊ»
    glBegin(GL_QUAD_STRIP);
    glColor3d(0.6, 0.3, 0);
    glVertex2f(-0.5, -0.7);
    glVertex2f(-1, -0.7);
    glColor3d(0.3, 0.1, 0);
    glVertex2f(-0.5, -0.8);
    glColor3d(0.3, 0.1, 0);
    glVertex2f(-1, -0.8);
    glEnd();
    //ŒÿÊÿ
    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-0.8, -0.7);
    glVertex2f(-0.5, -0.7);
    glVertex2f(-0.5, -0.8);
    glVertex2f(-0.8, -0.8);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-1., -0.7);
    glVertex2f(-0.8, -0.7);
    glVertex2f(-0.8, -0.8);
    glVertex2f(-1., -0.8);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(0.7, -0.7);
    glVertex2f(1., -0.7);
    glVertex2f(1., -0.8);
    glVertex2f(.7, -0.8);
    glEnd();
    //«·’› «·À«‰Ì „‰  Õ  Ì„Ì‰ ÿÊ»
    glBegin(GL_QUAD_STRIP);
    glColor3d(0.6, 0.3, 0);
    glVertex2f(0.7, -0.8);
    glVertex2f(1, -0.8);
    glColor3d(0.3, 0.1, 0);
    glVertex2f(0.7, -0.9);
    glColor3d(0.3, 0.1, 0);
    glVertex2f(1, -0.9);
    glEnd();

    //«·’› «·À«‰Ì „‰  Õ  Ì”«— ÿÊ»
    glBegin(GL_QUAD_STRIP);
    glColor3d(0.6, 0.3, 0);
    glVertex2f(-0.5, -0.8);
    glVertex2f(-1, -0.8);
    glColor3d(0.3, 0.1, 0);
    glVertex2f(-0.5, -0.9);
    glColor3d(0.3, 0.1, 0);
    glVertex2f(-1, -0.9);
    glEnd();
    //«·ŒÿÊÿ    
    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-1, -0.8);
    glVertex2f(-.7, -0.8);
    glVertex2f(-.7, -0.9);
    glVertex2f(-1, -0.9);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-0.7, -0.8);
    glVertex2f(-.5, -0.8);
    glVertex2f(-.5, -0.9);
    glVertex2f(-.7, -0.9);
    glEnd();



    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(0.7, -0.8);
    glVertex2f(1.0, -0.8);
    glVertex2f(1., -0.9);
    glVertex2f(.7, -0.9);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(0.8, -0.8);
    glVertex2f(1.0, -0.8);
    glVertex2f(1., -0.9);
    glVertex2f(.8, -0.9);
    glEnd();
    //«·’› «·”›·Ì ﬂ«„·
    glBegin(GL_QUAD_STRIP);
    glColor3d(0.6, 0.3, 0);
    glVertex2f(-1., -0.9);
    glVertex2f(1, -0.9);
    glColor3d(0.3, 0.1, 0);
    glVertex2f(-1., -1);
    glVertex2f(1, -1);
    glEnd();

    ///ŒÿÊÿ
    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-0.8, -0.9);
    glVertex2f(-0.5, -0.9);
    glVertex2f(-0.5, -1);
    glVertex2f(-0.8, -1);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-1., -0.9);
    glVertex2f(-0.8, -0.9);
    glVertex2f(-0.8, -1);
    glVertex2f(-1., -1);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-0.5, -0.9);
    glVertex2f(-0.2, -0.9);
    glVertex2f(-0.2, -1);
    glVertex2f(-.5, -1);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-0.2, -0.9);
    glVertex2f(0.1, -0.9);
    glVertex2f(0.1, -1);
    glVertex2f(-.2, -1);
    glEnd();



    //«·’› «·—«»⁄ „‰  Õ  Ì”«— ÿÊ»
    glBegin(GL_QUAD_STRIP);
    glColor3d(0.6, 0.3, 0);
    glVertex2f(-1., -0.6);
    glVertex2f(-0.5, -0.6);
    glColor3d(0.3, 0.1, 0);
    glVertex2f(-1., -0.7);
    glColor3d(0.3, 0.1, 0);
    glVertex2f(-0.5, -0.7);
    glEnd();

    /////«·ŒÿÊÿ
    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-0.7, -0.6);
    glVertex2f(-0.5, -0.6);
    glVertex2f(-0.5, -0.7);
    glVertex2f(-0.7, -0.7);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-1., -0.6);
    glVertex2f(-0.7, -0.6);
    glVertex2f(-0.7, -0.7);
    glVertex2f(-1., -0.7);
    glEnd();
    //«·’› «·Œ«„” „‰  Õ  Ì”«— ÿÊ»
    glBegin(GL_QUAD_STRIP);
    glColor3d(0.6, 0.3, 0);
    glVertex2f(-1., -0.5);
    glVertex2f(-0.5, -0.5);
    glColor3d(0.3, 0.1, 0);
    glVertex2f(-1., -0.6);
    glColor3d(0.3, 0.1, 0);
    glVertex2f(-0.5, -0.6);
    glEnd();
    //lines
    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-0.5, -0.5);
    glVertex2f(-0.8, -0.5);
    glVertex2f(-0.8, -0.6);
    glVertex2f(-.5, -0.6);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-0.8, -0.5);
    glVertex2f(-1., -0.5);
    glVertex2f(-1., -0.6);
    glVertex2f(-.8, -0.6);
    glEnd();

    //    ÿÊ»  Ì„Ì‰ «·’› «·À«·À 1  
    glBegin(GL_QUAD_STRIP);
    glColor3d(0.6, 0.3, 0);
    glVertex2f(1., -0.6);
    glVertex2f(0.7, -0.6);
    glColor3d(0.3, 0.1, 0);
    glVertex2f(1., -0.7);
    glColor3d(0.3, 0.1, 0);
    glVertex2f(0.7, -0.7);
    glEnd();
    //lines
    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(0.7, -0.6);
    glVertex2f(0.8, -0.6);
    glVertex2f(0.8, -0.7);
    glVertex2f(.7, -0.7);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(0.8, -0.6);
    glVertex2f(1.0, -0.6);
    glVertex2f(1., -0.7);
    glVertex2f(.8, -0.7);
    glEnd();
    //2 upper than 1
    glBegin(GL_QUAD_STRIP);
    glColor3d(0.6, 0.3, 0);
    glVertex2f(1., -0.5);
    glVertex2f(0.7, -0.5);
    glColor3d(0.3, 0.1, 0);
    glVertex2f(1., -0.6);
    glColor3d(0.3, 0.1, 0);
    glVertex2f(0.7, -0.6);
    glEnd();
    //lines
    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(0.7, -0.5);
    glVertex2f(1., -0.5);
    glVertex2f(1., -0.6);
    glVertex2f(.7, -0.6);
    glEnd();
    /////////«·Õ›—/////////
         //Ì„Ì‰//
    glBegin(GL_QUADS);
    glColor4d(0., 0., 0, 1);
    glVertex2f(0.7, -1);
    glVertex2f(-0.5, -1);
    glColor4d(0., 0., 0, 0.8);
    glVertex2f(-0.5, -0.8);
    glVertex2f(0.7, -0.8);
    glEnd();

    //line
    glBegin(GL_LINES);
    glColor3d(0, 0, 0);
    glVertex2f(0.85, -0.5);
    glVertex2f(0.85, -0.3);
    glEnd();
    //flag
    glBegin(GL_TRIANGLES);
    glColor3d(1, 0, 0);
    glVertex2f(0.85, -0.3);
    glVertex2f(0.95, -0.35);
    glVertex2f(0.85, -0.4);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(0, 0, 0);
    glVertex2f(0.85, -0.3);
    glVertex2f(0.95, -0.35);
    glVertex2f(0.85, -0.4);
    glEnd();
}

void winter() {

    //Winter background
    //1 init method: ·Ê‰ «·Œ·›Ì…
    //2 sun:
    glColor4d(0.9, 0.7, 0, 0.5);
    drawFilledCircle(0.7, 0.7, 0.15);
    glColor4d(1, 0.8156862745, 0.1450980392, 0.7);
    drawFilledCircle(0.7, 0.7, 0.12);
    glColor3d(1, 0.8941176471, 0.5058823529);
    drawFilledCircle(0.7, 0.7, 0.09);

    //3 clouds:
    glColor3d(1, 1, 1);
    drawFilledCirclehalfUpsideDown(0.6, 0.6, 0.2);
    glColor3d(1, 1, 1);
    drawFilledCirclehalfUpsideDown(0.8, 0.6, 0.1);
    glColor3d(1, 1, 1);;
    drawFilledCirclehalfUpsideDown(0.4, 0.6, 0.1);
    glColor3d(1, 1, 1);
    drawFilledCirclehalfUpsideDown(0.3, 0.6, 0.05);
    glLineWidth(5);
    glBegin(GL_LINES);
    glColor3d(1, 1, 1);
    glVertex2f(0.8, 0.6);
    glVertex2f(0, 0.6);
    glEnd();
    glColor3d(1, 1, 1);
    drawFilledCirclehalfUpsideDown(-0.6, 0.7, 0.2);
    glColor3d(1, 1, 1);
    drawFilledCirclehalfUpsideDown(-0.8, 0.7, 0.1);
    glColor3d(1, 1, 1);;
    drawFilledCirclehalfUpsideDown(-0.4, 0.7, 0.1);
    glColor3d(1, 1, 1);
    drawFilledCirclehalfUpsideDown(-0.3, 0.7, 0.05);
    glLineWidth(5);
    glBegin(GL_LINES);
    glColor3d(1, 1, 1);
    glVertex2f(-1, 0.7);
    glVertex2f(-0.2, 0.7);
    glEnd();
    // 
    glColor3d(0.6666666667, 0.8549019608, 1);
    drawFilledCircle(-0.6, 0, 0.2);
    glColor3d(0.6666666667, 0.8549019608, 1);
    drawFilledCircle(-0.8, -0.1, 0.1);
    glColor3d(0.6666666667, 0.8549019608, 1);
    drawFilledCircle(-0.9, -0.15, 0.05);
    glLineWidth(10);
    glBegin(GL_LINES);
    glColor3d(0.6666666667, 0.8549019608, 1);
    glVertex2f(-1, -0.2);
    glVertex2f(-0.7, -0.2);
    glEnd();
    glColor3d(0.6666666667, 0.8549019608, 1);
    drawFilledCircle(0.15, -0.05, 0.2);
    glColor3d(0.6666666667, 0.8549019608, 1);
    drawFilledCircle(0.35, -0.15, 0.1);
    glColor3d(0.6666666667, 0.8549019608, 1);
    drawFilledCircle(0.4, -0.3, 0.15);
    glColor3d(0.6666666667, 0.8549019608, 1);
    drawFilledCircle(0.5, -0.15, 0.1);
    glColor3d(0.6666666667, 0.8549019608, 1);
    drawFilledCircle(0.78, -0.05, 0.1);
    glColor3d(0.6666666667, 0.8549019608, 1);
    drawFilledCircle(0.7, -0.05, 0.05);
    glColor3d(0.6666666667, 0.8549019608, 1);
    drawFilledCircle(0.65, -0.07, 0.05);
    glColor3d(0.6666666667, 0.8549019608, 1);
    drawFilledCircle(0.6, -0.1, 0.05);
    glColor3d(0.6666666667, 0.8549019608, 1); // «·€Ì„Â «·„“«ÕÂ ⁄«·Ì„Ì‰
    drawFilledCircle(1, 0, 0.2);
    glColor3d(0.6666666667, 0.8549019608, 1); //”œ «·›—«€
    drawFilledCircle(1, -0.2, 0.2);

    //4 mountains: 
    glBegin(GL_TRIANGLES);
    glColor3d(0.8941176471, 0.8941176471, 0.8941176471); // light gray
    glVertex2f(0, -0.7);
    glVertex2f(1.2, -0.7);
    glVertex2f(0.7, 0);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3d(0.9411764706, 0.9411764706, 0.9411764706); //more white
    glVertex2f(-1.5, -0.7);
    glVertex2f(0.8, -0.7);
    glVertex2f(-0.2, 0.3);
    glEnd();

    //5 Trees
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    // «·‘Ã—… «·Œ·›Ì… «·√»⁄œ Ì„Ì‰«
    glScalef(0.3, 0.7, 1.0);
    glTranslatef(2.7, -0.3, 0.0);
    trees(0.3843137255, 0.5568627451, 0.7725490196, 0.3843137255, 0.5568627451, 0.7725490196, 0.3843137255, 0.5568627451, 0.7725490196);
    glPopMatrix();
    // «·‘Ã—… «·Œ·›Ì… «·√»⁄œ Ì”«—«
    glPushMatrix();
    glScalef(0.2, 0.7, 1.0);
    glTranslatef(-2.5, -0.3, 0.0);
    trees(0.3843137255, 0.5568627451, 0.7725490196, 0.3843137255, 0.5568627451, 0.7725490196, 0.3843137255, 0.5568627451, 0.7725490196);
    glPopMatrix();


    //6  ‘ÃÌ—« :
    glColor3d(0.3176470588, 0.4901960784, 0.7254901961); // «··Ê‰ «·Œ·›Ì √› Õ
    drawFilledCircle(0.5, -0.5, 0.06);
    drawFilledCircle(0.4, -0.45, 0.06);
    glColor3d(0.2745098039, 0.4235294118, 0.6156862745); // «··Ê‰ «·√„«„Ì √€„ﬁ
    drawFilledCircle(0.4, -0.45, 0.04);


    //7 House
    glBegin(GL_TRIANGLES); //front roof
    glColor3d(0.2745098039, 0.4235294118, 0.6156862745);
    glVertex2f(0.9, -0.3);
    glVertex2f(0.8, -0.4);
    glVertex2f(1, -0.4);
    glEnd();
    glBegin(GL_TRIANGLES); //suppose to be wood texture
    glColor3d(0.937254902, 0.7098039216, 0.4235294118);
    glVertex2f(0.9, -0.32);
    glColor3d(0.7215686275, 0.5137254902, 0.2274509804);
    glVertex2f(0.82, -0.39);
    glVertex2f(0.97, -0.39);
    glEnd();
    glBegin(GL_QUADS); //window
    glColor3d(0.7, 0.7, 0.7);
    glVertex2f(0.825, -0.37);
    glVertex2f(0.825, -0.365);
    glVertex2f(0.985, -0.37);
    glVertex2f(0.985, -0.365);
    glEnd();
    glBegin(GL_QUAD_STRIP); // wall
    glColor3d(0.3176470588, 0.4901960784, 0.7254901961);
    glVertex2f(1, -0.5); // «·“«ÊÌÂ «·”›·Ï «·Ì„‰Ï
    glVertex2f(1, -0.4); // «·“«ÊÌÂ «·⁄·Ì« «·Ì„‰Ï
    glVertex2f(0.8, -0.5); // «·“«ÊÌÂ «·”›·Ï «·Ì”—Ï
    glVertex2f(0.8, -0.4);// «·“«ÊÌÂ «·⁄·Ì« «·Ì”—Ï
    glVertex2f(0.7, -0.5);// «·“«ÊÌ… «·⁄·Ì« «·Ã«‰»ÌÂ
    glVertex2f(0.7, -0.4);// «·“«ÊÌ… «·”›·Ï «·Ã«‰»Ì…
    glEnd();
    glBegin(GL_QUADS); //roof
    glColor3d(1, 1, 1);
    glVertex2f(0.69, -0.41); // «·“«ÊÌÂ «·”›·Ï «·Ì”—Ï
    glVertex2f(0.79, -0.41); // «·“«ÊÌÂ «·”›·Ï «·Ì„‰Ï
    glVertex2f(0.9, -0.3); // «·“«ÊÌÂ «·⁄·Ì« «·Ì„‰Ï
    glVertex2f(0.8, -0.3);// «·“«ÊÌÂ «·⁄·Ì« «·Ì”—Ï
    glEnd();
    glLineWidth(5);
    glBegin(GL_LINES); // snow 
    glColor3d(1, 1, 1);
    glVertex2f(0.9, -0.3);
    glVertex2f(1, -0.4);
    glEnd();

    //8  ·«· Ã·Ìœ (Hills) 
    //Left hill
    glColor3d(0.6705882353, 0.7568627451, 0.8470588235);
    drawFilledCirclehalfUpsideDown(-1.1, -1, 0.7);
    //Right hill
    glColor3d(0.6705882353, 0.7568627451, 0.8470588235);
    drawFilledCirclehalfUpsideDown(1, -1, 0.6);
    // »Ì‰ «· ·«·:
    //~~~ ‘ÃÌ—« 
    glColor3d(0.3176470588, 0.4901960784, 0.7254901961); // ‘ÃÌ—«  Ì„‰Ï »·Ê‰ Œ·›Ì √› Õ
    drawFilledCircle(0.54, -0.58, 0.06);
    drawFilledCircle(0.58, -0.635, 0.05);
    glColor3d(0.2745098039, 0.4235294118, 0.6156862745); // ‘ÃÌ—«  Ì„‰Ï »·Ê‰ √„«„Ì √€„ﬁ
    drawFilledCircle(0.5, -0.5, 0.04);
    drawFilledCircle(0.54, -0.58, 0.04);
    drawFilledCircle(0.58, -0.635, 0.025);
    //~~~ «·‘Ã—… «·Œ·›Ì… «·√ﬁ—» Ì”«—«
    glPushMatrix();
    glScalef(0.3, 0.7, 1.0);
    glTranslatef(-1, -0.3, 0.0);
    trees(0.2745098039, 0.4235294118, 0.6156862745, 0.2745098039, 0.4235294118, 0.6156862745, 0.2745098039, 0.4235294118, 0.6156862745);
    glPopMatrix();
    //~~~ ‘ÃÌ—« 
    glColor3d(0.3176470588, 0.4901960784, 0.7254901961); // ‘ÃÌ—«  Ì”—Ï »·Ê‰ Œ·›Ì √› Õ
    drawFilledCircle(-0.54, -0.58, 0.045);
    drawFilledCircle(-0.59, -0.635, 0.06);
    glColor3d(0.2745098039, 0.4235294118, 0.6156862745); // ‘ÃÌ—«  Ì”—Ï »·Ê‰ √„«„Ì √€„ﬁ
    drawFilledCircle(-0.54, -0.58, 0.03);
    drawFilledCircle(-0.59, -0.635, 0.04);
    // Middle hill
    glColor3d(0.831372549, 0.8862745098, 0.937254902);
    drawFilledCirclehalfUpsideDown(0, -1, 0.7);
    //~~~ «·‘Ã—… «·Œ·›Ì… «·√ﬁ—» Ì„Ì‰«
    glPushMatrix();
    glScalef(0.5, 0.9, 1.0);
    glTranslatef(2.1, -0.1, 0.0);
    trees(0.2745098039, 0.4235294118, 0.6156862745, 0.2745098039, 0.4235294118, 0.6156862745, 0.2745098039, 0.4235294118, 0.6156862745);
    glPopMatrix();

    // «·‘Ã—… ›Ì «·„ﬁœ„…
    trees(0.2745098039, 0.4235294118, 0.6156862745, 0.3176470588, 0.4901960784, 0.7254901961, 0.3843137255, 0.5568627451, 0.7725490196);

    //9 ÿ»ﬁ… Ã·Ìœ Œ·›Ì… ﬁ—Ì»… ﬂ—ıﬂ«„
    glColor3d(0.97, 0.97, 0.97);
    drawFilledCirclehalfUpsideDown(-1, -0.95, 0.3);
    drawFilledCirclehalfUpsideDown(-0.73, -0.88, 0.2);
    drawFilledCirclehalfUpsideDown(-0.5, -0.95, 0.3);
    drawFilledCirclehalfUpsideDown(-0.20, -0.88, 0.2);
    drawFilledCirclehalfUpsideDown(0, -0.95, 0.3);
    drawFilledCirclehalfUpsideDown(0.20, -0.88, 0.2);
    drawFilledCirclehalfUpsideDown(0.5, -0.95, 0.3);
    drawFilledCirclehalfUpsideDown(0.73, -0.88, 0.2);
    drawFilledCirclehalfUpsideDown(1, -0.95, 0.3);
    drawFilledCirclehalfUpsideDown(1, -0.95, 0.4);

    //10 Falling snow
    glColor3d(1, 1, 1);
    drawFilledCircle(0.9, 0.1, 0.03);
    drawFilledCircle(0.8, 0.5, 0.02);
    drawFilledCircle(0.9, 0.4, 0.01);
    drawFilledCircle(0.7, 0.3, 0.01);
    drawFilledCircle(0.6, 0.2, 0.02);
    drawFilledCircle(0.2, 0.4, 0.01);
    drawFilledCircle(0.5, -0.2, 0.01);
    drawFilledCircle(0.3, -0.2, 0.02);
    drawFilledCircle(0.8, -0.3, 0.02);
    drawFilledCircle(0, 0, 0.03);
    drawFilledCircle(-0.2, 0.5, 0.02);
    drawFilledCircle(-0.8, 0.5, 0.01);
    drawFilledCircle(-0.9, 0.4, 0.02);
    drawFilledCircle(-0.7, 0.1, 0.02);
    drawFilledCircle(-0.6, 0.2, 0.01);
    drawFilledCircle(-0.3, -0.1, 0.02);
    drawFilledCircle(-0.4, -0.3, 0.01);
    drawFilledCircle(-0.6, -0.32, 0.03);
    drawFilledCircle(-0.8, -0.4, 0.02);
    drawFilledCircle(-0.99, -0.3, 0.01);


    //12 ⁄ﬁ»« 
    rocks();


    // jump block
    glBegin(GL_QUADS);
    //glColor3d(0.3, 0.4, 0.6);
    glColor3d(0.4, 0.5, 0.7);
    glVertex2f(0.1, -0.5);
    glVertex2f(0.4, -0.5);
    //glColor3d(1, 1, 1);
    glColor3d(0.9, 0.9, 0.9);
    glVertex2f(0.4, -0.4);
    glVertex2f(0.1, -0.4);
    glEnd();
    glLineWidth(1);
    glBegin(GL_LINE_LOOP);
    glColor3d(1, 1, 1);
    glVertex2f(0.1, -0.5);
    glVertex2f(0.4, -0.5);
    glVertex2f(0.4, -0.4);
    glVertex2f(0.1, -0.4);
    glEnd();

    //14 The END
    glLineWidth(5);
    glBegin(GL_LINES); // ⁄„Êœ
    glColor3d(0.7, 0.7, 0.7);
    glVertex2f(0.7, -0.6);
    glColor3d(0.9, 0.9, 0.9);
    glVertex2f(0.7, 0.1);
    glEnd();
    glPointSize(5.0f);
    glBegin(GL_POINTS); // —»ÿ… «·⁄·„
    glColor3d(1, 0, 0);
    glVertex2f(0.699, 0);
    glVertex2f(0.699, -0.1);
    glEnd();
    glBegin(GL_TRIANGLES); // «·⁄·„
    glColor3d(1, 0, 0);
    glVertex2f(0.699, 0);
    glVertex2f(0.699, -0.1);
    glVertex2f(0.99, -0.05);
    glEnd();
    glPushMatrix();
    glTranslatef(0.98, 0.23, 0.0);
    glScalef(1.3, 1.3, 1.0);
    rocks();
    glPopMatrix();

    //15 Base
    glLineWidth(1);
    drawTheGroundBlocks();
}

void spring() {
    //clouds//
    glColor3f(1.0f, 1.0f, 1.0f);
    drawFilledCircle(0.3 - 0.15, 0.8, .12);
    drawFilledCircle(0.3, 0.8, .15);
    drawFilledCircle(0.3 + 0.15, 0.8, .16);
    drawFilledCircle(0.3 + 0.3, 0.8, .13);
    //cloud left
    drawFilledCircle(-0.3 - 0.15, 0.5, .12);
    drawFilledCircle(-0.3, 0.5, .15);
    drawFilledCircle(-0.3 + 0.15, 0.5, .16);
    drawFilledCircle(-0.3 + 0.3, 0.5, .13);
    //little cloud
    drawFilledCircle(-0.8 - 0.15, 0.8, .08);
    drawFilledCircle(-0.8, 0.8, .11);
    drawFilledCircle(-0.8 + 0.15, 0.8, .12);
    drawFilledCircle(-0.8 + 0.3, 0.8, .09);
    //little cloud
    drawFilledCircle(0.7 - 0.15, 0.5, .08);
    drawFilledCircle(0.7, 0.5, .11);
    drawFilledCircle(0.7 + 0.15, 0.5, .12);
    drawFilledCircle(0.7 + 0.3, 0.5, .09);
    //little tiny cloud down
    drawFilledCircle(0.3 - 0.06, 0.2, .05);
    drawFilledCircle(0.3, 0.2, .07);
    drawFilledCircle(0.3 + 0.08, 0.2, .08);
    drawFilledCircle(0.3 + 0.16, 0.2, .06);

    drawFilledCircle(-0.9 - 0.06, 0.3, .05);
    drawFilledCircle(-0.9, 0.3, .07);
    drawFilledCircle(-0.9 + 0.08, 0.3, .08);
    drawFilledCircle(-0.9 + 0.16, 0.3, .06);
    //     //
   //left
    glBegin(GL_TRIANGLES);
    glColor3d(0.6, .7, 0.4);
    glVertex2f(-0.3, 0.3);
    glColor3d(0.4, .7, 0.4);
    glVertex2f(-0.8, -0.6);
    glVertex2f(0.2, -0.6);
    glEnd();
    //right
    glBegin(GL_TRIANGLES);
    glColor3d(0.7, 0.8, 0.5);
    glVertex2f(-0.0, 0.2);
    glColor3d(0.4, 0.9, 0.4);
    glVertex2f(-0.4, -0.6);
    glVertex2f(.5, -0.6);
    glEnd();

    //////         down
    glBegin(GL_QUADS);
    glColor3d(0.5, 0.3, 0.1);
    glVertex2f(1, -0.9);
    glVertex2f(-1., -0.9);
    glColor4d(0.2, 0., 0., 0.7);
    glVertex2f(-1., -1);
    glVertex2f(1, -1);
    glEnd();
    //lines
    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(-1, -.9);
    glVertex2f(-.7, -.9);
    glVertex2f(-.7, -1);
    glVertex2f(-1, -1);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(-.7, -.9);
    glVertex2f(-.4, -.9);
    glVertex2f(-.4, -1);
    glVertex2f(-.7, -1);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(-.4, -.9);
    glVertex2f(-.1, -.9);
    glVertex2f(-.1, -1);
    glVertex2f(-.4, -1);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(-.1, -.9);
    glVertex2f(0.2, -.9);
    glVertex2f(.2, -1);
    glVertex2f(-.1, -1);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(0.2, -.9);
    glVertex2f(0.5, -.9);
    glVertex2f(.5, -1);
    glVertex2f(0.2, -1);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(0.5, -.9);
    glVertex2f(0.8, -.9);
    glVertex2f(.8, -1);
    glVertex2f(0.5, -1);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(0.8, -.9);
    glVertex2f(1, -.9);
    glVertex2f(1, -1);
    glVertex2f(0.8, -1);
    glEnd();
    //mid
    glBegin(GL_QUADS);
    glColor3d(0.5, 0.3, 0.1);
    glVertex2f(1, -0.8);
    glVertex2f(-1., -0.8);
    glColor4d(0.2, 0., 0., 0.7);
    glVertex2f(-1., -0.9);
    glVertex2f(1, -0.9);
    glEnd();
    //lines
    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-1, -0.8);
    glVertex2f(-.8, -0.8);
    glVertex2f(-.8, -0.9);
    glVertex2f(-1, -0.9);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-0.8, -0.8);
    glVertex2f(-.5, -0.8);
    glVertex2f(-.5, -0.9);
    glVertex2f(-0.8, -0.9);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-0.5, -0.8);
    glVertex2f(-.2, -0.8);
    glVertex2f(-.2, -0.9);
    glVertex2f(-0.5, -0.9);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-0.2, -0.8);
    glVertex2f(0.1, -0.8);
    glVertex2f(0.1, -0.9);
    glVertex2f(-0.2, -0.9);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(0.1, -0.8);
    glVertex2f(0.4, -0.8);
    glVertex2f(0.4, -0.9);
    glVertex2f(0.1, -0.9);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(0.4, -0.8);
    glVertex2f(0.7, -0.8);
    glVertex2f(0.7, -0.9);
    glVertex2f(0.4, -0.9);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(0.7, -0.8);
    glVertex2f(1, -0.8);
    glVertex2f(1, -0.9);
    glVertex2f(0.7, -0.9);
    glEnd();
    //up
    glBegin(GL_QUADS);
    glColor3d(0.5, 0.3, 0.1);
    glVertex2f(1, -0.7);
    glVertex2f(-1., -0.7);
    glColor4d(0.2, 0., 0., 0.7);
    glVertex2f(-1., -0.8);
    glVertex2f(1, -0.8);
    glEnd();
    //lines
    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(-1, -.7);
    glVertex2f(-.7, -.7);
    glVertex2f(-.7, -0.8);
    glVertex2f(-1, -0.8);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(-.7, -.7);
    glVertex2f(-.4, -.7);
    glVertex2f(-.4, -0.8);
    glVertex2f(-.7, -.8);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(-.4, -.7);
    glVertex2f(-.1, -.7);
    glVertex2f(-.1, -0.8);
    glVertex2f(-.4, -0.8);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(-.1, -.7);
    glVertex2f(0.2, -.7);
    glVertex2f(.2, -0.8);
    glVertex2f(-.1, -.8);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(0.2, -.7);
    glVertex2f(0.5, -.7);
    glVertex2f(.5, -.8);
    glVertex2f(0.2, -.8);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(0.5, -.7);
    glVertex2f(0.8, -.7);
    glVertex2f(.8, -.8);
    glVertex2f(0.5, -.8);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3b(0, 0, 0);
    glVertex2f(0.8, -.7);
    glVertex2f(1, -.7);
    glVertex2f(1, -.8);
    glVertex2f(0.8, -.8);
    glEnd();
    //apove up
    glBegin(GL_QUADS);
    glColor3d(0.5, 0.3, 0.1);
    glVertex2f(1, -0.6);
    glVertex2f(-1., -0.6);
    glColor4d(0.2, 0., 0., 0.7);
    glVertex2f(-1., -0.7);
    glVertex2f(1, -0.7);
    glEnd();
    //lines
    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-1, -0.6);
    glVertex2f(-.8, -0.6);
    glVertex2f(-.8, -0.7);
    glVertex2f(-1, -0.7);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-0.8, -0.6);
    glVertex2f(-.5, -0.6);
    glVertex2f(-.5, -0.7);
    glVertex2f(-0.8, -0.7);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-0.5, -0.6);
    glVertex2f(-.2, -0.6);
    glVertex2f(-.2, -0.7);
    glVertex2f(-0.5, -0.7);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(-0.2, -0.6);
    glVertex2f(0.1, -0.6);
    glVertex2f(0.1, -0.7);
    glVertex2f(-0.2, -0.7);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(0.1, -0.6);
    glVertex2f(0.4, -0.6);
    glVertex2f(0.4, -0.7);
    glVertex2f(0.1, -0.7);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(0.4, -0.6);
    glVertex2f(0.7, -0.6);
    glVertex2f(0.7, -0.7);
    glVertex2f(0.4, -0.7);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(0., 0., 0);
    glVertex2f(0.7, -0.6);
    glVertex2f(1, -0.6);
    glVertex2f(1, -0.7);
    glVertex2f(0.7, -0.7);
    glEnd();

    /////          
    glBegin(GL_QUADS);
    glColor3d(0.4, 0.6, 0.);
    glVertex2f(1.0, -0.7);
    glVertex2f(-1, -0.7);
    glColor3d(0.4, 0.7, 0.2);
    glVertex2f(-1, -0.6);
    glVertex2f(1, -0.6);
    glEnd();
    ///                 
    glColor3d(0.5, 0.7, 0.3);
    drawFilledCircle1(-0.9, -0.6, 0.2);
    drawFilledCircle1(-0.7, -0.6, 0.3);
    drawFilledCircle1(-0.4, -0.6, 0.17);
    drawFilledCircle1(-0.16, -0.6, 0.35);
    drawFilledCircle1(0.04, -0.6, 0.27);
    drawFilledCircle1(0.6, -0.6, 0.35);
    drawFilledCircle1(0.9, -0.6, 0.27);
    ////                     
    glColor3d(0.4, 0.6, 0.2);
    drawFilledCircle1(-0.9, -0.6, 0.09);
    drawFilledCircle1(-0.7, -0.6, 0.2);
    drawFilledCircle1(-0.4, -0.6, 0.17);
    drawFilledCircle1(-0.16, -0.6, 0.25);
    drawFilledCircle1(0.04, -0.6, 0.17);
    drawFilledCircle1(0.4, -0.6, 0.25);
    drawFilledCircle1(.7, -0.6, 0.17);
    drawFilledCircle1(.9, -0.6, 0.23);
    //lake
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, myTexture3);
    glBegin(GL_QUADS);
    glColor4d(0., 0.4, 0.9, 0.8);
    glTexCoord2f(0.0, 0.0);  glVertex2f(0.7, -1);
    glTexCoord2f(1.0, 0.0); glVertex2f(-0.2, -1);
    glColor4d(0., 0.8, 0.9, 0.8);
    glTexCoord2f(1.0, 1.0); glVertex2f(-0.2, -0.7);
    glTexCoord2f(0.0, 1.0); glVertex2f(0.7, -0.7);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //line
    glBegin(GL_LINES);
    glColor3d(0, 0.3, 0);
    glVertex2f(0.9, -0.6);
    glVertex2f(0.9, -0.4);
    glEnd();
    //flag
    glBegin(GL_TRIANGLES);
    glColor3d(1, 0, 0);
    glVertex2f(0.9, -0.4);
    glVertex2f(1, -0.45);
    glVertex2f(0.9, -0.5);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3d(0, 0.3, 0);
    glVertex2f(0.9, -0.4);
    glVertex2f(1, -0.45);
    glVertex2f(0.9, -0.5);
    glEnd();
}

void pausegame() {

    glColor3f(0.8007, 0.0, 0.0);
    drawBitmapText("***** YOU WIN *****      ", -0.48, 0.2, 0);

    //Pause
    glColor3d(0.47, 0.25, 0.21);
    drawFilledCircle(-0.36, -0.19, 0.16);
    //traingle 
    glBegin(GL_TRIANGLES);
    glColor3d(1, 1, 1);
    glVertex2f(-0.41, -0.27);
    glVertex2f(-0.27, -0.18);
    glVertex2f(-0.41, -0.1);
    glEnd();
    //stages 
    glBegin(GL_QUADS);
    glColor3d(0.47, 0.25, 0.21);
    glVertex2f(0.21, -0.33);
    glVertex2f(0.5, -0.33);
    glVertex2f(0.5, -0.05);
    glVertex2f(0.21, -0.05);
    glEnd();
    //white rict
    glBegin(GL_QUADS);
    glColor3d(1, 1, 1);
    glVertex2f(0.27, -0.17);
    glVertex2f(0.43, -0.17);
    glVertex2f(0.43, -0.15);
    glVertex2f(0.27, -0.15);
    glEnd();
    //white 
    glBegin(GL_QUADS);
    glColor3d(1, 1, 1);
    glVertex2f(0.27, -0.22);
    glVertex2f(0.43, -0.22);
    glVertex2f(0.43, -0.2);
    glVertex2f(0.27, -0.2);
    glEnd();


}

void First() {
    winter();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, myTexture1);
    glBegin(GL_QUADS);
    glColor4d(0.5, 0.3, 0.2, 0.8);
    glTexCoord2f(0.0, 0.0);  glVertex2f(0.85, -0.6);
    glTexCoord2f(1.0, 0.0); glVertex2f(0.7, -0.6);
    glTexCoord2f(1.0, 1.0); glVertex2f(0.7, -0.47);
    glTexCoord2f(0.0, 1.0); glVertex2f(0.85, -0.47);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(transValueX, 0.0, 0.0);
    winterball();
    glPopMatrix();
}


void Second() {
    spring();
    spring_obst();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, myTexture1);
    glBegin(GL_QUADS);
    glColor4d(0.5, 0.3, 0.2, 0.8);
    glTexCoord2f(0.0, 0.0);  glVertex2f(0.95, -0.6);
    glTexCoord2f(1.0, 0.0); glVertex2f(0.8, -0.6);
    glTexCoord2f(1.0, 1.0); glVertex2f(0.8, -0.47);
    glTexCoord2f(0.0, 1.0); glVertex2f(0.95, -0.47);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(transValueX, 0.0, 0.0);
    springball();
    glPopMatrix();
}

void Second2() {
    spring();
    ;
    spring_obst2();
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, myTexture1);
    glBegin(GL_QUADS);
    glColor4d(0.5, 0.3, 0.2, 0.8);
    glTexCoord2f(0.0, 0.0);  glVertex2f(0.95, -0.6);
    glTexCoord2f(1.0, 0.0); glVertex2f(0.8, -0.6);
    glTexCoord2f(1.0, 1.0); glVertex2f(0.8, -0.47);
    glTexCoord2f(0.0, 1.0); glVertex2f(0.95, -0.47);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(transValueX, 0.0, 0.0);
    springball();
    glPopMatrix();
}

void Third() {
    summer();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, myTexture1);
    glBegin(GL_QUADS);
    glColor4d(0.5, 0.3, 0.2, 0.8);
    glTexCoord2f(0.0, 0.0);  glVertex2f(0.95, -0.18);
    glTexCoord2f(1.0, 0.0); glVertex2f(0.8, -0.18);
    glTexCoord2f(1.0, 1.0); glVertex2f(0.8, -0.05);
    glTexCoord2f(0.0, 1.0); glVertex2f(0.95, -0.05);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(transValueX, 0.0, 0.0);
    summerball();
    glPopMatrix();
}

void Fourth() {
    autumn();
    glPushMatrix();
    glTranslatef(xpos, 0, 0);
    flyingBrick();
    glPopMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, myTexture1);
    glBegin(GL_QUADS);
    glColor4d(0.5, 0.3, 0.2, 0.8);
    glTexCoord2f(0.0, 0.0);  glVertex2f(0.95, -0.5);
    glTexCoord2f(1.0, 0.0); glVertex2f(0.8, -0.5);
    glTexCoord2f(1.0, 1.0); glVertex2f(0.8, -0.37);
    glTexCoord2f(0.0, 1.0); glVertex2f(0.95, -0.37);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(transValueX, 0.0, 0.0);
    autumnball();
    glPopMatrix();
}

// <<<<<<<<<<<<<<<<<<< ball and sences Movement methods >>>>>>>>>>>>>>>>>>>>>>>


//method brick
void timer(int)
{
    glutPostRedisplay();
    glutTimerFunc(4500 / 60, timer, 0);//update the display frame

    switch (state)
    {
    case 1://to go right
        if (xpos <= 0.35) {
            xpos += 0.05;
        }
        else {
            state = -1;
        }
        break;
    case -1://to go left
        if (xpos >= -0.25) {
            xpos -= 0.05;
        }
        else {
            state = 1;
        }
        break;
    }
}


void balljump(int value) {
    if (isJumping) {
        ballYVelocity += gravity;
        ballY += ballYVelocity;
        ballX += ballXVelocity;

        if (ballY <= -0.1 + ballRadius2) {
            ballY = -0.1 + ballRadius2;
            ballYVelocity = 0.0f;
            isJumping = false;
        }
    }
    glutPostRedisplay();
    glutTimerFunc(10, balljump, 0);
}

void special(int key, int x, int y)
{
    //handle special keys
    switch (key) {
    case GLUT_KEY_LEFT:
        ballXSpeed = -0.01f;
        transValueX += ballXSpeed;
        cout << " translate x:" << transValueX << endl;
        break;

    case GLUT_KEY_RIGHT:
        ballXSpeed = 0.01f;
        transValueX += ballXSpeed;
        cout << " translate x:" << transValueX << endl;
        break;
    }
}

static
void play(void)
{
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case '27': // 27 is the ASCII code for the Escape key
        exit(0);
    case 'q': // 27 is the ASCII code for the Escape key
    case 'Q': // 27 is the ASCII code for the Escape key
        exit(0);
    case 'J':
        if (!isJumping) {
            // Apply an upward force to the ball when the space bar is pressed
            ballYVelocity = 0.07f;
            ballXVelocity = 0.009f;
            PlaySound(TEXT("pl_jump1.wav"), NULL, SND_SYNC);
            isJumping = true;
        }
        break;

    case 'j':
        if (!isJumping) {
            // Apply an upward force to the ball when the space bar is pressed
            ballYVelocity = 0.04f;
            ballXVelocity = 0.0f;
            PlaySound(TEXT("pl_jump1.wav"), NULL, SND_SYNC);
            isJumping = true;
        }
        break;
    case 's':
    case 'S':
        if (MAIN_MENU == true) {
            Springg = false;
            winterr = false;
            Summerr = false;
            autumnn = false;
            PAUSE_MENU = false;
            GAME_OVER = false;
            MAIN_MENU = false;
            Stage = true;
            cout << "Q - q to exit the game " << endl;
            cout << " J  to big jump" << endl;
            cout << " j  to small jump" << endl;
            cout << " B - b to return" << endl;
            cout << " S - s to start the game" << endl;
            cout << " I - i to show the instructions" << endl;
            cout << " '1'  level one" << endl;
            cout << " '2'  level two" << endl;
            cout << " '3' level three" << endl;
            cout << " '4' level four" << endl;
            cout << " 'R - r' replay" << endl;
            cout << "  'L - l' to show the list of levels" << endl;

        }
        break;
    case '1':
        transValueX = 0;
        PlaySound(TEXT("new_level.wav"), NULL, SND_SYNC);
        if (Stage == true) {
            Springg = false;
            winterr = true;
            Summerr = false;
            autumnn = false;
            PAUSE_MENU = false;
            GAME_OVER = false;
            MAIN_MENU = false;
            Stage = false;

        }
        break;
    case '2':
        transValueX = 0;
        PlaySound(TEXT("new_level.wav"), NULL, SND_SYNC);
        if (Stage == true && counter == 1) {
            Springg = true;
            winterr = false;
            Summerr = false;
            autumnn = false;
            PAUSE_MENU = false;
            GAME_OVER = false;
            MAIN_MENU = false;
            Stage = false;
        }
        break;
    case '3':
        transValueX = 0;
        PlaySound(TEXT("new_level.wav"), NULL, SND_SYNC);
        if (Stage == true && counter == 2) {
            Springg = false;
            winterr = false;
            Summerr = true;
            autumnn = false;
            PAUSE_MENU = false;
            GAME_OVER = false;
            MAIN_MENU = false;
            Stage = false;
        }
        break;
    case '4':
        transValueX = 0;
        PlaySound(TEXT("new_level.wav"), NULL, SND_SYNC);
        if (Stage == true && counter == 3) {
            Springg = false;
            winterr = false;
            Summerr = false;
            autumnn = true;
            PAUSE_MENU = false;
            GAME_OVER = false;
            MAIN_MENU = false;
            Stage = false;
        }
        break;
    case 'r':
    case 'R':
        PAUSE_MENU = true;
        if (PAUSE_MENU == true) {
            if (counter == 1) {
                PlaySound(TEXT("new_level.wav"), NULL, SND_SYNC);
                transValueX = 0;
                Springg = false;
                winterr = true;
                Summerr = false;
                autumnn = false;
                PAUSE_MENU = false;
                GAME_OVER = false;
                MAIN_MENU = false;
                Stage = false;
            }
            else if (counter == 2) {
                PlaySound(TEXT("new_level.wav"), NULL, SND_SYNC);
                transValueX = 0;
                Springg = true;
                winterr = false;
                Summerr = false;
                autumnn = false;
                PAUSE_MENU = false;
                GAME_OVER = false;
                MAIN_MENU = false;
                Stage = false;
            }
            else if (counter == 3) {
                PlaySound(TEXT("new_level.wav"), NULL, SND_SYNC);
                transValueX = 0;
                Springg = false;
                winterr = false;
                Summerr = true;
                autumnn = false;
                PAUSE_MENU = false;
                GAME_OVER = false;
                MAIN_MENU = false;
                Stage = false;
            }
            else if (counter == 4) {
                PlaySound(TEXT("new_level.wav"), NULL, SND_SYNC);
                transValueX = 0;
                Springg = false;
                winterr = false;
                Summerr = false;
                autumnn = true;
                PAUSE_MENU = false;
                GAME_OVER = false;
                MAIN_MENU = false;
                Stage = false;
            }
        }
        break;
    case 'l':
    case 'L':
        Stage = true;
        Springg = false;
        winterr = false;
        Summerr = false;
        autumnn = false;
        PAUSE_MENU = false;
        GAME_OVER = false;
        MAIN_MENU = false;
        break;

    }
}

void keyboardUp(unsigned char key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
    case GLUT_KEY_RIGHT:
        // Stop horizontal movement when 'A', 'a', 'D', or 'd' key is released
        ballXVelocity = 0.0;
        break;
    }
}



void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (MAIN_MENU) {
        welcome();
    }
    if (Stage) {
        Stages();
    }
    if (winterr) {
        First();
        if (transValueX >= 1.53) {
            PlaySound(TEXT("get_achievement.wav"), NULL, SND_SYNC);
            pausegame();
            score = 1;
            counter = 1;
        }
    }
    if (Springg) {
        Second();
        if (transValueX >= 0.45) {
            Second2();
            if (transValueX >= 1.66) {
                PlaySound(TEXT("get_achievement.wav"), NULL, SND_SYNC);
                pausegame();
                score = 2;
                counter = 2;
            }
        }


    }
    else if (Summerr) {

        Third();
        if (transValueX >= 1.78) {
            PlaySound(TEXT("get_achievement.wav"), NULL, SND_SYNC);
            pausegame();
            score = 3;
            counter = 3;
        }
    }
    else if (autumnn) {

        Fourth();
        if (transValueX >= 1.66) {
            PlaySound(TEXT("get_achievement.wav"), NULL, SND_SYNC);
            pausegame();
            score = 4;
            counter = 4;
        }
    }
    glDisable(GL_BLEND);
    drawScore();
    glFlush();
    glutSwapBuffers();
}

void reshapeFun(GLint newWidth, GLint newHight)
{
    glViewport(0, 0, newWidth, newHight);
    win_width = newWidth;
    win_hight = newHight;
}

void init()
{
    myTexture1 = LoadTexture(image1Path, 640, 640);
    myTexture2 = LoadTexture(image2Path, 500, 333);
    myTexture3 = LoadTexture(image3Path, 459, 240);

    glClearColor(0.789, 0.937, 0.968, 1.0); // Set background color to blue 
    glMatrixMode(GL_PROJECTION);          // set the projection parameters
    glShadeModel(GL_FLAT);
}



int main(int argc, char** argv)
{

    glutInit(&argc, argv);                          // Initialize GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);   //  Specify the display Mode ñ RGB, RGBA or color //  Index, single or double Buffer
    glutInitWindowSize(win_width, win_hight);       // Set the window's initial width & height
    glutInitWindowPosition(50, 50);                 // Position the window's initial top-left corner
    glutCreateWindow("Crazy Ball");    // Create a window with the given title
    init();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glutIdleFunc(play);
    glutDisplayFunc(display);                        // Register display callback handler for window re-paint
    glutReshapeFunc(reshapeFun);
    glutTimerFunc(0, timer, 0);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(special);
    glutTimerFunc(0, balljump, 0);

    glutMainLoop();                                  // Enter the infinitely event-processing loop
    return 0;
}