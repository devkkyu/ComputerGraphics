#include "World.h"

World::World()
	: dxObjPos(0.f), dzObjPos(0.f)
{
	int widthNum;
	int heightNum;
	cout << "가로 개수 입력 : ";
	cin >> widthNum;

	cout << "세로 개수 입력 : ";
	cin >> heightNum;

	for (int i = 0; i < heightNum; ++i) {
		CUBE.push_back(new vector<Cube*>);
	}

	float widthScale = 50.f / widthNum / 2.f;
	float widthDistance = 50.f / widthNum;
	float heightScale = 50.f / heightNum / 2.f;
	float heightDistance = 50.f / heightNum;

	for (int i = 0; i < CUBE.size(); ++i) {
		for (int j = 0; j < widthNum; ++j) {
			CUBE[i]->push_back(new Cube(glm::vec3(-25. + widthScale + (j * widthDistance), 0, -25. + heightScale + (i * heightDistance)), widthScale, heightScale));
		}
	}

	startObjPos.first = 25.f - widthDistance - widthScale;
	startObjPos.second = 25.f - heightScale;


	ROBOT.setPos(glm::vec3(startObjPos.first, 0.f, startObjPos.second), 0.f);
}

void World::Draw()
{
	GLuint Color = glGetUniformLocation(shaderID, "Color");
	//-----------------------------------------------------바닥그리기-----------------------------------------------------

	glBindVertexArray(FVAO);

	GLuint modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.f))); //--- modelTransform 변수에 변환 값 적용하기

	glUniform3f(Color, 0.5f, 0.5f, 0.5f);
	glDrawArrays(GL_POLYGON, 0, 4);


	//-----------------------------------------------------산 그리기-----------------------------------------------------
	for (vector<Cube*>*& hcube : CUBE) {
		for (Cube*& wcube : *hcube) {
			wcube->Draw();
		}
	}

	//-----------------------------------------------------로봇 그리기-----------------------------------------------------
	ROBOT.Draw();
}

void World::Update()
{
	for (vector<Cube*>*& hcube : CUBE) {
		for (Cube*& wcube : *hcube) {
			wcube->Update();
		}
	}

	ROBOT.Update(startObjPos.first + dxObjPos, startObjPos.second + dzObjPos);
}

void World::MakeMaze()
{
	for (vector<Cube*>*& hcube : CUBE) {
		for (Cube*& wcube : *hcube) {
			wcube->SetRender(true);
		}
	}

	random_device rd;
	default_random_engine dre(rd());
	uniform_int_distribution<int> uid{ 0,1 };

	(*CUBE[0])[1]->SetRender(false);
	(*CUBE[CUBE.size() - 1])[CUBE[0]->size() - 2]->SetRender(false);

	int i = 1, j = 1;
	while (true) {
		(*CUBE[i])[j]->SetRender(false);
		if (i == CUBE.size() - 2 && j == CUBE[0]->size() - 2)
			break;

		if (uid(dre)) {					//1이면 오른쪽 0이면 아래로 갑시다
			if (j + 1 < CUBE[0]->size() - 1)
				++j;
			else
				continue;
		}
		else {
			if (i + 1 < CUBE.size() - 1)
				++i;
			else
				continue;
		}
	}
}

void World::Camera(int personView)
{
	switch (personView) {
	case  -1: {
		// 직각투영
		glm::mat4 projection = glm::ortho(-30.0f * ((float)winWidth / (float)winHeight), 30.0f * ((float)winWidth / (float)winHeight), -30.0f, 30.0f, -30.0f, 30.0f);
		GLuint projectionLocation = glGetUniformLocation(shaderID, "projectionTransform"); //--- 투영 변환 값 설정
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

		// 카메라
		glm::vec3 cameraPos = glm::vec3(C_movX, 0.f, 20.f + C_movZ); //--- 카메라 위치 (어디서 볼건지)
		glm::vec3 cameraDirection = glm::vec3(C_movX, 0.0f, 0.0f); //--- 카메라 바라보는 방향 (어디볼건지 하면될듯)
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향->벡터임(방향만) (음수하면 화면 상하거꾸로보임)

		glm::mat4 cameraRevolution = glm::rotate(glm::mat4(1.f), glm::radians(C_RotYAngle), glm::vec3(0.f, 1.f, 0.f));

		glm::mat4 view = glm::lookAt(cameraPos, cameraDirection, cameraUp) * cameraRevolution;

		GLuint viewLocation = glGetUniformLocation(shaderID, "viewTransform"); //--- 뷰잉 변환 설정
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	}
			break;
	case 0: {
		// 원근투영
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)winWidth / (float)winHeight, 0.1f, 200.f);
		GLuint projectionLocation = glGetUniformLocation(shaderID, "projectionTransform"); //--- 투영 변환 값 설정
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

		// 카메라
		glm::vec3 cameraPos = glm::vec3(C_movX, (70.f + C_movZ) / 7.f * 6.f, 70.f + C_movZ); //--- 카메라 위치 (어디서 볼건지)
		glm::vec3 cameraDirection = glm::vec3(C_movX, 0.0f, 0.0f); //--- 카메라 바라보는 방향 (어디볼건지 하면될듯)
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향->벡터임(방향만) (음수하면 화면 상하거꾸로보임)

		glm::mat4 cameraRevolution = glm::rotate(glm::mat4(1.f), glm::radians(C_RotYAngle), glm::vec3(0.f, 1.f, 0.f));

		glm::mat4 view = glm::lookAt(cameraPos, cameraDirection, cameraUp) * cameraRevolution;

		GLuint viewLocation = glGetUniformLocation(shaderID, "viewTransform"); //--- 뷰잉 변환 설정
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	}
		  break;
	case 1: {
		// 원근투영
		glm::mat4 projection = glm::perspective(glm::radians(60.f), (float)winWidth / (float)winHeight, 0.1f, 200.f);
		GLuint projectionLocation = glGetUniformLocation(shaderID, "projectionTransform"); //--- 투영 변환 값 설정
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

		// 카메라
		glm::vec3 cameraPos = glm::vec3(startObjPos.first + dxObjPos, 2.f, startObjPos.second + dzObjPos); //--- 카메라 위치 (어디서 볼건지)
		glm::vec3 cameraDirection = glm::vec3(startObjPos.first + dxObjPos, 3.0f, 0.0f + dzObjPos); //--- 카메라 바라보는 방향 (어디볼건지 하면될듯)
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향->벡터임(방향만) (음수하면 화면 상하거꾸로보임)

		glm::mat4 cameraRotationX = glm::rotate(glm::mat4(1.f), glm::radians(MouseAngle.first), glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 cameraRotationY = glm::rotate(glm::mat4(1.f), glm::radians(MouseAngle.second), glm::vec3(1.f, 0.f, 0.f));

		glm::mat4 view = cameraRotationY * cameraRotationX * glm::lookAt(cameraPos, cameraDirection, cameraUp);

		GLuint viewLocation = glGetUniformLocation(shaderID, "viewTransform"); //--- 뷰잉 변환 설정
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	}
		  break;
	case 3: {
		// 원근투영
		glm::mat4 projection = glm::perspective(glm::radians(60.f), (float)winWidth / (float)winHeight, 0.1f, 200.f);
		GLuint projectionLocation = glGetUniformLocation(shaderID, "projectionTransform"); //--- 투영 변환 값 설정
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

		// 카메라
		glm::vec3 cameraPos = glm::vec3(startObjPos.first + dxObjPos, 3.f, startObjPos.second + dzObjPos); //--- 카메라 위치 (어디서 볼건지)
		glm::vec3 cameraDirection = glm::vec3(startObjPos.first + dxObjPos, 2.0f, 0.0f + dzObjPos); //--- 카메라 바라보는 방향 (어디볼건지 하면될듯)
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향->벡터임(방향만) (음수하면 화면 상하거꾸로보임)

		glm::mat4 cameraRotationX = glm::rotate(glm::mat4(1.f), glm::radians(MouseAngle.first), glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 cameraRotationY = glm::rotate(glm::mat4(1.f), glm::radians(MouseAngle.second), glm::vec3(1.f, 0.f, 0.f));

		glm::mat4 view = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 1.f, -5.f)) * cameraRotationY * cameraRotationX * glm::lookAt(cameraPos, cameraDirection, cameraUp);

		GLuint viewLocation = glGetUniformLocation(shaderID, "viewTransform"); //--- 뷰잉 변환 설정
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	}
		  break;
	}
}

void World::KeyIn(GLuint key)
{
	switch (key)
	{
	case 'w':
	{
		Robot temp(glm::vec3(startObjPos.first + dxObjPos + glm::sin(glm::radians(MouseAngle.first)) * 0.075f,
			0,
			startObjPos.second + dzObjPos - glm::cos(glm::radians(MouseAngle.first)) * 0.075f
		));
		bool iscollide = false;
		for (vector<Cube*>*& hcube : CUBE) {
			bool brk = false;
			for (Cube*& wcube : *hcube) {
				if (isCollide2D(temp, *wcube) && wcube->GetRender()) {
					brk = true;
					iscollide = true;
					break;
				}
			}
			if (brk)
				break;
		}
		if (!iscollide) {
			dxObjPos += glm::sin(glm::radians(MouseAngle.first)) * 0.075f;
			dzObjPos -= glm::cos(glm::radians(MouseAngle.first)) * 0.075f;
		}
	}
	break;
	case 'a':
	{
		Robot temp(glm::vec3(startObjPos.first + dxObjPos - glm::cos(glm::radians(MouseAngle.first)) * 0.075f,
			0,
			startObjPos.second + dzObjPos - glm::sin(glm::radians(MouseAngle.first)) * 0.075f
		));
		bool iscollide = false;
		for (vector<Cube*>*& hcube : CUBE) {
			bool brk = false;
			for (Cube*& wcube : *hcube) {
				if (isCollide2D(temp, *wcube) && wcube->GetRender()) {
					brk = true;
					iscollide = true;
					break;
				}
			}
			if (brk)
				break;
		}
		if (!iscollide) {
			dxObjPos -= glm::cos(glm::radians(MouseAngle.first)) * 0.075f;
			dzObjPos -= glm::sin(glm::radians(MouseAngle.first)) * 0.075f;
		}
	}
	break;
	case 's':
	{
		Robot temp(glm::vec3(startObjPos.first + dxObjPos - glm::sin(glm::radians(MouseAngle.first)) * 0.075f,
			0,
			startObjPos.second + dzObjPos + glm::cos(glm::radians(MouseAngle.first)) * 0.075f
		));
		bool iscollide = false;
		for (vector<Cube*>*& hcube : CUBE) {
			bool brk = false;
			for (Cube*& wcube : *hcube) {
				if (isCollide2D(temp, *wcube) && wcube->GetRender()) {
					brk = true;
					iscollide = true;
					break;
				}
			}
			if (brk)
				break;
		}
		if (!iscollide) {
			dxObjPos -= glm::sin(glm::radians(MouseAngle.first)) * 0.075f;
			dzObjPos += glm::cos(glm::radians(MouseAngle.first)) * 0.075f;
		}
	}
	break;
	case 'd':
	{
		Robot temp(glm::vec3(startObjPos.first + dxObjPos + glm::cos(glm::radians(MouseAngle.first)) * 0.075f,
			0,
			startObjPos.second + dzObjPos + glm::sin(glm::radians(MouseAngle.first)) * 0.075f
		));
		bool iscollide = false;
		for (vector<Cube*>*& hcube : CUBE) {
			bool brk = false;
			for (Cube*& wcube : *hcube) {
				if (isCollide2D(temp, *wcube) && wcube->GetRender()) {
					brk = true;
					iscollide = true;
					break;
				}
			}
			if (brk)
				break;
		}
		if (!iscollide) {
			dxObjPos += glm::cos(glm::radians(MouseAngle.first)) * 0.075f;
			dzObjPos += glm::sin(glm::radians(MouseAngle.first)) * 0.075f;
		}
	}
	break;
	case 'c':
		dxObjPos = 0.f;
		dzObjPos = 0.f;
		break;
	}
}

bool World::isCollide(Robot r1, Cube r2)
{
	if (r1.getRight() < r2.getLeft() || r1.getLeft() > r2.getRight()) return false;
	if (r1.getFront() < r2.getBehind() || r1.getBehind() > r2.getFront()) return false;
	if (r1.getTop() < r2.getBottom() || r1.getBottom() > r2.getTop()) return false;
	return true;
}

bool World::isCollide2D(Robot r1, Cube r2)
{
	if (r1.getRight() < r2.getLeft() || r1.getLeft() > r2.getRight()) return false;
	if (r1.getFront() < r2.getBehind() || r1.getBehind() > r2.getFront()) return false;
	return true;
}
