#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <random>
#include <utility>
#include <gl/glew.h>											//--- 필요한 헤더파일 include
#include <gl/freeglut.h>
//#include <gl/freeglut_ext.h>
//#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
//#include <gl/glm/gtc/matrix_transform.hpp>

#include "Draw.h"
#include "Keyboard.h"
#include "KeyboardUP.h"
#include "Mouse.h"
#include "Timer.h"

#include "Light.h"
#include "Sphere.h"

class Light;
class Sphere;

extern Light LIGHT;
extern Sphere SPHERE[3];

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

using namespace std;


extern int TimerValue;

extern GLint winWidth, winHeight;
extern GLuint cubeVAO, sphereVAO;
extern GLuint shaderID; //--- 세이더 프로그램 이름

extern vector<glm::vec3> cubevertex;
extern vector<glm::vec3> spherevertex;

extern bool isDepTest;

extern float L_RotYAngle;
extern int isLRotate;

extern int colorValue;
