#include <iostream>
#include <gl/glew.h>											//--- 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Motion(int x, int y);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid TimerFunction(int value);

int* reSize(int w, int h, int value);

float* SetColor(const char* num);
int hex2num(char hex);

int gl2window(float point, int way);
float window2gl(int point, int way);

int* makeRect(int x, int y, int value);

void main(int argc, char** argv)								//--- 윈도우 출력하고 콜백함수 설정 
{	//--- 윈도우 생성하기
	glutInit(&argc, argv);										// glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);				// 디스플레이 모드 설정
	glutInitWindowPosition(100, 100);								// 윈도우의 위치 지정
	glutInitWindowSize(500, 500);								// 윈도우의 크기 지정
	reSize(500, 500, 1);										// 내가만든 사이즈함수 정의

	glutCreateWindow("Example1");								// 윈도우 생성(윈도우 이름)


	float* color = SetColor("36FFFF");							//색상코드표 색깔 쓸수있음, #빼고넣기
	glClearColor(color[0], color[1], color[2], 1.0f);			//내가원하는 색상을 색상코드로 적용
	glClear(GL_COLOR_BUFFER_BIT);								// 설정된 색으로 전체를 칠하기

	glColor3f(0.0f, 1.0f, 0.0f);
	makeRect(250, 250, 1);

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)									// glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	glutDisplayFunc(drawScene);									// 출력 콜백함수의 지정
	glutReshapeFunc(Reshape);									// 다시 그리기 콜백함수 지정
	glutKeyboardFunc(Keyboard);									// 키보드 입력 콜백함수 지정.
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutMainLoop();												// 이벤트 처리 시작
}

GLvoid drawScene()												//--- 콜백 함수: 그리기 콜백 함수
{
	//glClearColor(0.0f, 0.0f, 1.0f, 1.0f);						// 바탕색을 ‘blue’ 로 지정
	glClear(GL_COLOR_BUFFER_BIT);								// 설정된 색으로 전체를 칠하기
	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다.
	makeRect(0, 0, 2);

	glutSwapBuffers();											// 화면에 출력하기
}

GLvoid Reshape(int w, int h)									//--- 콜백 함수: 다시 그리기 콜백 함수
{
	reSize(w, h, 1);
	glViewport(0, 0, w, h);
}

GLvoid Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		int* loc = makeRect(0, 0, 0);		// 현재 생성된 사각형 좌표만 받아옴

		int x1 = loc[0] - 40;
		int y1 = loc[1] - 40;
		int x2 = loc[0] + 40;
		int y2 = loc[1] + 40;

		if (x <x1 || x>x2 || y<y1 || y>y2) {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<int> dis(0, 100);

			glClearColor((float)dis(gen) / 100, (float)dis(gen) / 100, (float)dis(gen) / 100, 1.0f);

			glutPostRedisplay(); // 화면 재 출력

		}
		else {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<int> dis(0, 100);

			glColor3f((float)dis(gen) / 100, (float)dis(gen) / 100, (float)dis(gen) / 100);

			glutPostRedisplay(); // 화면 재 출력
		}
	}
}

GLvoid Motion(int x, int y)
{
	glClear(GL_COLOR_BUFFER_BIT);								// 설정된 색으로 전체를 칠하기
	makeRect(x, y, 1);
	glutPostRedisplay();
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'R':
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		break; //--- 배경색을 빨강색으로 설정
	case 'G':
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		break; //--- 배경색을 초록색으로 설정
	case 'B':
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		break; //--- 배경색을 파랑색으로 설정
	case 'A': {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dis(0, 3);
		switch (dis(gen)) {
		case 1:
			glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
			break;
		case 2:
			glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
			break;
		case 3:
			glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
			break;
		}
	}
			break;
	case 'W':
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		break;
	case 'K':
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	case 'T':
		glutTimerFunc(100, TimerFunction, 2);
		break;
	case 'S':
		glutTimerFunc(100, TimerFunction, 0);
		break;
	case 'Q':
		exit(0);
		break;
	}

	glutPostRedisplay(); //--- 배경색이 바뀔때마다 출력 콜백함수를 호출하여 화면을 refresh 한다
}

GLvoid TimerFunction(int value)			//최초시작 2, 반복할때 1, 종료할때 0
{
	static int temp = 1;

	if (value == 2)
		temp = 1;
	else if (value == 0)
		temp = 0;

	if (temp == 0)
		return;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 3);

	switch (dis(gen)) {
	case 1:
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		break;
	case 2:
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		break;
	case 3:
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		break;
	}

	glutPostRedisplay(); // 화면 재 출력

	glutTimerFunc(100, TimerFunction, temp); // 타이머함수 재 설정
}

float* SetColor(const char* num)
{
	float color[3]{};

	int r = hex2num(num[0]) * 16 + hex2num(num[1]);
	int g = hex2num(num[2]) * 16 + hex2num(num[3]);
	int b = hex2num(num[4]) * 16 + hex2num(num[5]);

	color[0] = (float)r / 255.0f;
	color[1] = (float)g / 255.0f;
	color[2] = (float)b / 255.0f;

	return color;
}

int hex2num(char hex) {

	if (hex >= 48 && hex <= 57)         // 문자가 0~9이면(ASCII 코드 48~57)
	{
		// 문자에서 0에 해당하는 ASCII 코드 값을 빼
		return hex - 48;
	}
	else if (hex >= 65 && hex <= 70)    // 문자가 A~F이면(ASCII 코드 65~70)
	{                                 // 대문자로 된 16진수의 처리
		// 문자에서 (A에 해당하는 ASCII 코드 값 - 10)을 빼
		return hex - (65 - 10);
	}
	else if (hex >= 97 && hex <= 102)   // 문자가 a~f이면(ASCII 코드 97~102)
	{                                 // 소문자로 된 16진수의 처리
		// 문자에서 (a에 해당하는 ASCII 코드 값 - 10)을 빼
		return hex - (97 - 10);
	}
}

int* reSize(int w, int h, int value)		// value가 1이면 사이즈변경
{
	static int size[2]{};

	if (value == 1) {
		size[0] = w;
		size[1] = h;
	}

	return size;
}

int gl2window(float point, int way)		// 가로는 0 세로는 1
{
	int* size = reSize(0, 0, 0);								

	return (1 + point) * ((float)size[way] / 2);			// 나중에 문제생길수있어 y부호. 확인해보기

}

float window2gl(int point, int way)		// 가로는 0 세로는 1
{
	int* size = reSize(0, 0, 0);

	if (way == 1)
		return -(point - (float)size[way] / 2) / ((float)size[way] / 2);
	return (point - (float)size[way] / 2) / ((float)size[way] / 2);
}

int* makeRect(int x, int y, int value)		// value가 1이면 사각형 생성, value가 2이면 xy무시하고 기존위치에 사각형 생성
{
	static int loc[2]{};

	if (value == 1) {
		loc[0] = x; loc[1] = y;
		glRectf(window2gl(x - 40, 0), window2gl(y - 40, 1), window2gl(x + 40, 0), window2gl(y + 40, 1));
	}
	else if (value == 2) {
		glRectf(window2gl(loc[0] - 40, 0), window2gl(loc[1] - 40, 1), window2gl(loc[0] + 40, 0), window2gl(loc[1] + 40, 1));
	}

	return loc;
}
