#include <iostream>
#include <SOIL/SOIL.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <math.h>

using namespace std;

// Compilar
// g++ main.cpp -o a -Wall -lglfw -lm -lGL -lGLU -lSOIL

#define WINDOW_WIDTH 1300
#define WINDOW_HEIGHT 800

/* Posição da luz (x, y, z, intensidade)*/
GLfloat light_position[] = {0.0, 0.0, 0.0, 1.0};

void resize(int width, int height)
{

  glViewport(0, 0, width, height);

  float aspect = float(width) / float(height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(100.0, aspect, 0.1, 500.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(-1.0, 0.0, -5.0);
}

// Função para carregar o arquivo de textura
GLuint loadTexture(const char *texture_file)
{
  GLuint texture = SOIL_load_OGL_texture(texture_file, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

  if (!texture)
  {
    printf("Erro ao carregar imagem\n");
  }

  return texture;
}

GLUquadricObj *initPlanetObject(GLUquadricObj *planet_object)
{
  planet_object = gluNewQuadric();
  gluQuadricDrawStyle(planet_object, GLU_FILL);
  gluQuadricTexture(planet_object, GL_TRUE);
  gluQuadricNormals(planet_object, GLU_SMOOTH);

  return planet_object;
}

void createObject(GLUquadricObj *obj, float obj_size, float obj_X, float obj_Y, float obj_Z, float obj_rotate, const char *img_directory)
{

  glPushMatrix();
  glTranslatef(obj_X, obj_Z, obj_Y);
  glRotatef(obj_rotate, 0, 1, 0);
  glRotatef(-90, 1, 0, 0);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, loadTexture(img_directory));
  gluSphere(obj, obj_size, 40, 40);
  glPopMatrix();
}

//======================================================================================================================
float sun_size = 2;
float sun_rotate = 0.0;
float sun_X = 0.0;
float sun_Y = 0.0;
float sun_Z = 0.0;
//======================================================================================================================
float mercury_size = 0.5;
float mercury_radius = 5;
float mercury_velocity = -500;

float mercury_angle = 0.0;
float mercury_rotate = 0.0;
float mercury_X = 5;
float mercury_Y = 0;
float mercury_Z = 0;
//======================================================================================================================
float earth_size = 0.5;
float earth_radius = 8;
float earth_velocity = 200;

float earth_angle = 0.0;
float earth_rotate = 0.0;
float earth_X = 8;
float earth_Y = 0;
float earth_Z = 0;
//======================================================================================================================
float moon1_size = 0.1;
float moon1_radius = 1;
float moon1_velocity = 500;

float moon1_angle = 0.0;
float moon1_rotate = 0.0;
float moon1_X = 9;
float moon1_Y = 0;
float moon1_Z = 0;
//======================================================================================================================
float moon2_size = 0.1;
float moon2_radius = 1.5;
float moon2_velocity = 500;

float moon2_angle = 0.0;
float moon2_rotate = 0.0;
float moon2_X = 8;
float moon2_Y = 0;
float moon2_Z = 1.5;

void rotateAndTranslatePlanet(float *obj_angle, float *obj_velocity, float *obj_radius, float *obj_X, float *obj_Y, float *obj_Z, float *obj_rotate, float rotation_value, bool moon1, bool moon2)
{

  *obj_angle -= *obj_velocity / 10000;

  if (moon1)
  {
    *obj_X = *obj_radius * cos(*obj_angle) + earth_X;
    *obj_Y = *obj_radius * sin(*obj_angle) + earth_Y;
  }
  else if (moon2)
  {
    *obj_X = earth_X;
    *obj_Y = *obj_radius * sin(*obj_angle) + earth_Y;
    *obj_Z = *obj_radius * cos(*obj_angle);
  }
  else
  {
    // Translação
    *obj_X = *obj_radius * cos(*obj_angle);
    *obj_Y = *obj_radius * sin(*obj_angle);
  }

  // Rotação
  *obj_rotate += rotation_value;
}

void rotateSun()
{
  sun_rotate += 2.0;
}

void creatAllPlanets()
{

  glTranslatef(0.0, 0.0, -10.0);
  glColor3f(1.0, 1.0, 1.0);

  GLUquadricObj *obj_sun = NULL;
  obj_sun = initPlanetObject(obj_sun);

  GLUquadricObj *obj_mercury = NULL;
  obj_mercury = initPlanetObject(obj_mercury);

  GLUquadricObj *obj_earth = NULL;
  obj_earth = initPlanetObject(obj_earth);

  GLUquadricObj *obj_moon1 = NULL;
  obj_moon1 = initPlanetObject(obj_moon1);

  GLUquadricObj *obj_moon2 = NULL;
  obj_moon2 = initPlanetObject(obj_moon2);

  createObject(obj_sun, sun_size, sun_X, sun_Y, sun_Z, sun_rotate, "img/sol.png");
  createObject(obj_mercury, mercury_size, mercury_X, mercury_Y, mercury_Z, mercury_rotate, "img/mercurio.png");
  createObject(obj_earth, earth_size, earth_X, earth_Y, earth_Z, earth_rotate, "img/terra.png");
  createObject(obj_moon1, moon1_size, moon1_X, moon1_Y, moon1_Z, moon1_rotate, "img/mercurio.png");
  createObject(obj_moon2, moon2_size, moon2_X, moon2_Y, moon2_Z, moon2_rotate, "img/mercurio.png");
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  gluLookAt(
      0.0, 0.0, 1.0,
      0.0, 0.0, 0.0, // centro
      0.0, 1.0, 0.0);

  // Profundidade
  glEnable(GL_DEPTH_TEST);
  creatAllPlanets();
}

void controlRotation(GLFWwindow *window, int key, int scanCode, int action, int mod)
{
  if (key == 89)
  {
    rotateSun();
    rotateAndTranslatePlanet(&mercury_angle, &mercury_velocity, &mercury_radius, &mercury_X, &mercury_Y, &mercury_Z, &mercury_rotate, 14.0, 0, 0);
    rotateAndTranslatePlanet(&earth_angle, &earth_velocity, &earth_radius, &earth_X, &earth_Y, &earth_Z, &earth_rotate, 20.0, 0, 0);
    rotateAndTranslatePlanet(&moon1_angle, &moon1_velocity, &moon1_radius, &moon1_X, &moon1_Y, &moon1_Z, &moon1_rotate, 20.0, 1, 0);
    rotateAndTranslatePlanet(&moon2_angle, &moon2_velocity, &moon2_radius, &moon2_X, &moon2_Y, &moon2_Z, &moon2_rotate, 20.0, 0, 1);
  }
}

int main(int argc, char *argv[])
{
  GLFWwindow *window;

  if (!glfwInit())
  {
    return -1;
  }

  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Sistema solar", NULL, NULL);

  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  while (!glfwWindowShouldClose(window))
  {

    glfwPollEvents();
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHT0);   // Luz
    glEnable(GL_LIGHTING); // Sombra

    int largura, altura;
    glfwGetFramebufferSize(window, &largura, &altura);

    display();
    resize(largura, altura);
    glfwSetKeyCallback(window, controlRotation);

    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}