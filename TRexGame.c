#include <GL/glut.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

float trexY = 0.0f;
bool isJumping = false;
float jumpVelocity = 0.0f;
float gravity = -0.008f;
float obstacleX = 1.0f;
int score = 0;
bool gameOver = false;
bool gameStarted = false;
bool choosingDifficulty = true;
float obstacleSpeed = 0.015f;
float trexScale = 1.0f;
float trexRotation = 0.0f;

#define NUM_STARS 100
float stars[NUM_STARS][2];

void initStars() {
    srand(time(0));
    for (int i = 0; i < NUM_STARS; ++i) {
        stars[i][0] = (float)rand() / RAND_MAX * 2 - 1; // x [-1,1]
        stars[i][1] = (float)rand() / RAND_MAX * 2 - 1; // y [-1,1]
    }
}

void drawStars() {
    glColor3f(1.0f, 1.0f, 1.0f);
    for (int i = 0; i < NUM_STARS; ++i) {
        glBegin(GL_POINTS);
        glVertex2f(stars[i][0], stars[i][1]);
        glEnd();
    }
}

void displayText(float x, float y, char *string, void* font) {
    glRasterPos2f(x, y);
    for (char *c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void drawTrex() {
    glPushMatrix();
    glTranslatef(-0.8f, trexY, 0.0f);
    glScalef(trexScale, trexScale, 1.0f);
    glRotatef(trexRotation, 0.0f, 0.0f, 1.0f);
    glColor3f(0.0f, 1.0f, 0.0f);

    // Head
    glRectf(-0.03f, 0.07f, 0.03f, 0.12f);
    // Eye
    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    glVertex2f(0.015f, 0.11f);
    glEnd();

    glColor3f(0.0f, 1.0f, 0.0f);
    // Body
    glRectf(-0.04f, -0.02f, 0.04f, 0.07f);
    // Arms
    glRectf(-0.045f, 0.03f, -0.035f, 0.04f);
    glRectf(0.035f, 0.03f, 0.045f, 0.04f);
    // Legs
    glRectf(-0.03f, -0.05f, -0.01f, -0.02f);
    glRectf(0.01f, -0.05f, 0.03f, -0.02f);
    // Tail
    glRectf(-0.06f, -0.01f, -0.04f, 0.01f);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
    drawStars();

    if (choosingDifficulty) {
        glColor3f(1.0f, 1.0f, 0.0f);
        displayText(-0.6f, 0.6f, "Welcome to Inchara's T-Rex Game", GLUT_BITMAP_TIMES_ROMAN_24);
        glColor3f(0.5f, 1.0f, 1.0f);
        displayText(-0.3f, 0.2f, "Press E for Easy Mode", GLUT_BITMAP_HELVETICA_18);
        displayText(-0.3f, -0.1f, "Press H for Hard Mode", GLUT_BITMAP_HELVETICA_18);
        glutSwapBuffers();
        return;
    }

    if (!gameStarted) {
        glColor3f(1.0f, 1.0f, 0.0f);
        displayText(-0.3f, 0.1f, "Press SPACE to Start", GLUT_BITMAP_TIMES_ROMAN_24);
        glutSwapBuffers();
        return;
    }

    if (gameOver) {
        char finalScore[30];
        sprintf(finalScore, "Final Score: %d", score);
        glColor3f(1.0f, 0.5f, 0.0f);
        displayText(-0.3f, 0.2f, "GAME OVER! Press R to Restart", GLUT_BITMAP_TIMES_ROMAN_24);
        displayText(-0.2f, 0.1f, finalScore, GLUT_BITMAP_HELVETICA_18);
        glutSwapBuffers();
        return;
    }

    // Draw Scoreboard
    char scoreText[20];
    sprintf(scoreText, "Score: %d", score);
    glColor3f(1.0f, 1.0f, 0.0f);
    displayText(-0.95f, 0.9f, scoreText, GLUT_BITMAP_HELVETICA_18);

    // Draw Ground
    glColor3f(0.0f, 0.8f, 0.0f);
    glRectf(-1.0f, -0.1f, 1.0f, -0.05f);

    // Draw T-Rex
    drawTrex();

    // Draw Obstacle
    glPushMatrix();
    glTranslatef(obstacleX, -0.05f, 0.0f);
    glColor3f(1.0f, 0.0f, 0.0f);
    glRectf(-0.05f, 0.0f, 0.05f, 0.1f);
    glPopMatrix();

    glutSwapBuffers();
}

void update(int value) {
    if (!gameStarted || gameOver) return;

    if (isJumping) {
        trexY += jumpVelocity;
        jumpVelocity += gravity;
        trexRotation += 5.0f;
        if (trexY <= 0.0f) {
            trexY = 0.0f;
            isJumping = false;
            trexRotation = 0.0f;
        }
    }

    obstacleX -= obstacleSpeed;
    if (obstacleX < -1.0f) {
        obstacleX = 1.0f;
        score++;
    }

    if (obstacleX < -0.75f && obstacleX > -0.85f && trexY < 0.05f) {
        gameOver = true;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int x, int y) {
    if (choosingDifficulty) {
        if (key == 'e') {
            obstacleSpeed = 0.01f;
            choosingDifficulty = false;
        } else if (key == 'h') {
            obstacleSpeed = 0.02f;
            choosingDifficulty = false;
        }
        glutPostRedisplay();
        return;
    }
    if (key == ' ' && !gameStarted) {
        gameStarted = true;
        glutTimerFunc(16, update, 0);
    }
    if (key == ' ' && !isJumping && !gameOver) {
        isJumping = true;
        jumpVelocity = 0.08f;
    }
    if (key == 'r' && gameOver) {
        trexY = 0.0f;
        trexRotation = 0.0f;
        obstacleX = 1.0f;
        score = 0;
        gameOver = false;
        gameStarted = false;
        choosingDifficulty = true;
        glutPostRedisplay();
    }
}

void init() {
    glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
    initStars();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("T-Rex Game");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
}
