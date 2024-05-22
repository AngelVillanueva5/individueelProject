#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>

#define CUBE_SIZE 3

struct Color {
    GLfloat r, g, b;
};

struct Cube {
    GLfloat size;
    Color faces[6];
    GLfloat orientation[3]; // rotation around x, y, z axes
};

Cube rubiksCube[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE];

Color colors[6] = {
    {1.0, 0.0, 0.0}, // Red
    {0.0, 1.0, 0.0}, // Green
    {0.0, 0.0, 1.0}, // Blue
    {1.0, 1.0, 0.0}, // Yellow
    {1.0, 0.5, 0.0}, // Orange
    {1.0, 1.0, 1.0}  // White
};

GLfloat angleX = 0.0;
GLfloat angleY = 0.0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void createCube(Cube* cube, GLfloat size) {
    cube->size = size;
    for (int i = 0; i < 6; i++) {
        cube->faces[i] = colors[i];
    }
    cube->orientation[0] = 0.0;
    cube->orientation[1] = 0.0;
    cube->orientation[2] = 0.0;
}

void drawFace(Color color) {
    glColor3f(color.r, color.g, color.b);
    glBegin(GL_QUADS);
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glEnd();
}

void drawCube(Cube* cube) {
    GLfloat size = cube->size;
    glPushMatrix();
    glScalef(size, size, size);
    glRotatef(cube->orientation[0], 1.0, 0.0, 0.0);
    glRotatef(cube->orientation[1], 0.0, 1.0, 0.0);
    glRotatef(cube->orientation[2], 0.0, 0.0, 1.0);

    // Front face
    drawFace(cube->faces[0]);

    // Rotate and draw other faces
    glPushMatrix();
    glRotatef(90.0, 0.0, 1.0, 0.0);
    drawFace(cube->faces[1]);
    glPopMatrix();

    glPushMatrix();
    glRotatef(-90.0, 0.0, 1.0, 0.0);
    drawFace(cube->faces[2]);
    glPopMatrix();

    glPushMatrix();
    glRotatef(180.0, 0.0, 1.0, 0.0);
    drawFace(cube->faces[3]);
    glPopMatrix();

    glPushMatrix();
    glRotatef(90.0, 1.0, 0.0, 0.0);
    drawFace(cube->faces[4]);
    glPopMatrix();

    glPushMatrix();
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    drawFace(cube->faces[5]);
    glPopMatrix();

    glPopMatrix();
}

void drawRubiksCube() {
    for (int x = 0; x < CUBE_SIZE; x++) {
        for (int y = 0; y < CUBE_SIZE; y++) {
            for (int z = 0; z < CUBE_SIZE; z++) {
                glPushMatrix();
                glTranslatef(x - 1, y - 1, z - 1);
                drawCube(&rubiksCube[x][y][z]);
                glPopMatrix();
            }
        }
    }
}

void rotateFaceX(int layer, int clockwise) {
    Cube temp[CUBE_SIZE][CUBE_SIZE];
    for (int y = 0; y < CUBE_SIZE; y++) {
        for (int z = 0; z < CUBE_SIZE; z++) {
            temp[y][z] = rubiksCube[layer][y][z];
        }
    }
    for (int y = 0; y < CUBE_SIZE; y++) {
        for (int z = 0; z < CUBE_SIZE; z++) {
            if (clockwise) {
                rubiksCube[layer][y][z] = temp[CUBE_SIZE - 1 - z][y];
                rubiksCube[layer][y][z].orientation[0] += 90;
            } else {
                rubiksCube[layer][y][z] = temp[z][CUBE_SIZE - 1 - y];
                rubiksCube[layer][y][z].orientation[0] -= 90;
            }
        }
    }
}

void rotateFaceY(int layer, int clockwise) {
    Cube temp[CUBE_SIZE][CUBE_SIZE];
    for (int x = 0; x < CUBE_SIZE; x++) {
        for (int z = 0; z < CUBE_SIZE; z++) {
            temp[x][z] = rubiksCube[x][layer][z];
        }
    }
    for (int x = 0; x < CUBE_SIZE; x++) {
        for (int z = 0; z < CUBE_SIZE; z++) {
            if (clockwise) {
                rubiksCube[x][layer][z] = temp[CUBE_SIZE - 1 - z][x];
                rubiksCube[x][layer][z].orientation[1] += 90;
            } else {
                rubiksCube[x][layer][z] = temp[z][CUBE_SIZE - 1 - x];
                rubiksCube[x][layer][z].orientation[1] -= 90;
            }
        }
    }
}

void rotateFaceZ(int layer, int clockwise) {
    Cube temp[CUBE_SIZE][CUBE_SIZE];
    for (int x = 0; x < CUBE_SIZE; x++) {
        for (int y = 0; y < CUBE_SIZE; y++) {
            temp[x][y] = rubiksCube[x][y][layer];
        }
    }
    for (int x = 0; x < CUBE_SIZE; x++) {
        for (int y = 0; y < CUBE_SIZE; y++) {
            if (clockwise) {
                rubiksCube[x][y][layer] = temp[CUBE_SIZE - 1 - y][x];
                rubiksCube[x][y][layer].orientation[2] += 90;
            } else {
                rubiksCube[x][y][layer] = temp[y][CUBE_SIZE - 1 - x];
                rubiksCube[x][y][layer].orientation[2] -= 90;
            }
        }
    }
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        angleY -= 5.0;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        angleY += 5.0;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        angleX -= 5.0;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        angleX += 5.0;

    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        rotateFaceY(0, 1); // Rotate top layer clockwise
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        rotateFaceY(0, 0); // Rotate top layer counterclockwise
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        rotateFaceY(CUBE_SIZE - 1, 1); // Rotate bottom layer clockwise
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        rotateFaceY(CUBE_SIZE - 1, 0); // Rotate bottom layer counterclockwise
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
        rotateFaceX(0, 1); // Rotate left layer clockwise
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        rotateFaceX(0, 0); // Rotate left layer counterclockwise
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        rotateFaceX(CUBE_SIZE - 1, 1); // Rotate right layer clockwise
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        rotateFaceX(CUBE_SIZE - 1, 0); // Rotate right layer counterclockwise
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        rotateFaceZ(0, 1); // Rotate front layer clockwise
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        rotateFaceZ(0, 0); // Rotate front layer counterclockwise
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
        rotateFaceZ(CUBE_SIZE - 1, 1); // Rotate back layer clockwise
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
        rotateFaceZ(CUBE_SIZE - 1, 0); // Rotate back layer counterclockwise
}

void updateRotation() {
    // Additional update logic can be added here if needed.
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Equivalent of gluLookAt
    GLfloat eyeX = 3.0, eyeY = 4.0, eyeZ = 5.0;
    GLfloat centerX = 0.0, centerY = 0.0, centerZ = 0.0;
    GLfloat upX = 0.0, upY = 1.0, upZ = 0.0;

    GLfloat forward[3], side[3], up[3];
    forward[0] = centerX - eyeX;
    forward[1] = centerY - eyeY;
    forward[2] = centerZ - eyeZ;

    // Normalize forward vector
    GLfloat forwardLength = sqrt(forward[0] * forward[0] + forward[1] * forward[1] + forward[2] * forward[2]);
    forward[0] /= forwardLength;
    forward[1] /= forwardLength;
    forward[2] /= forwardLength;

    // Compute side vector
    side[0] = forward[1] * upZ - forward[2] * upY;
    side[1] = forward[2] * upX - forward[0] * upZ;
    side[2] = forward[0] * upY - forward[1] * upX;

    // Normalize side vector
    GLfloat sideLength = sqrt(side[0] * side[0] + side[1] * side[1] + side[2] * side[2]);
    side[0] /= sideLength;
    side[1] /= sideLength;
    side[2] /= sideLength;

    // Recompute up vector
    up[0] = side[1] * forward[2] - side[2] * forward[1];
    up[1] = side[2] * forward[0] - side[0] * forward[2];
    up[2] = side[0] * forward[1] - side[1] * forward[0];

    GLfloat matrix[16] = {
        side[0],    up[0],    -forward[0],    0.0,
        side[1],    up[1],    -forward[1],    0.0,
        side[2],    up[2],    -forward[2],    0.0,
        0.0,        0.0,        0.0,        1.0
    };

    glMultMatrixf(matrix);
    glTranslatef(-eyeX, -eyeY, -eyeZ);

    glRotatef(angleX, 1.0, 0.0, 0.0);
    glRotatef(angleY, 0.0, 1.0, 0.0);

    drawRubiksCube();
}

int main() {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Rubik's Cube", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glEnable(GL_DEPTH_TEST);

    // Initialize the Rubik's Cube
    for (int x = 0; x < CUBE_SIZE; x++) {
        for (int y = 0; y < CUBE_SIZE; y++) {
            for (int z = 0; z < CUBE_SIZE; z++) {
                createCube(&rubiksCube[x][y][z], 0.9);
            }
        }
    }

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        updateRotation();
        display();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

