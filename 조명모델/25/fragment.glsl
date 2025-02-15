#version 330 core

in vec3 FragPos;			//--- 위치값
in vec3 Normal;				//--- 버텍스 세이더에서 받은 노멀값

out vec4 FragColor;			//--- 최종 객체의 색 저장

uniform vec3 lightPos;		//--- 조명의 위치
uniform vec3 lightColor;	//--- 조명의 색
uniform vec3 objectColor;	//--- 객체의 색

uniform vec3 viewPos;		//--- 카메라 위치

void main()
{
	float ambientLight = 0.5;										//--- 주변 조명 계수: 0.0 ≤ ambientLight ≤ 1.
	vec3 ambient = ambientLight * lightColor;						//--- 주변 조명 값

	vec3 normalVector = normalize (Normal);							//--- 노말값을 정규화한다.
	vec3 lightDir = normalize (lightPos - FragPos);					//--- 표면과 조명의 위치로 조명의 방향을 결정한다.

	float diffuseLight = max (dot (normalVector, lightDir), 0.0);	//--- N과 L의 내적 값으로 강도 조절 (음의 값을 가질 수 없게 한다.)
	vec3 diffuse = diffuseLight * lightColor;						//--- 산란반사조명값 = 산란반사값 * 조명색상값


	int shininess = 128;				//--- 광택 계수
										//--- 반짝이는 하이라이트를 생성한다.
										//--- 관찰자가 빛의 입사각과 거의 같은 반사각 부근에 위치할 경우 입사된 빛의 전부를 인식하며 하이라이트가 생긴다.
										//--- 거울 반사 조명은 재질의 shininess (광택 계수) 정도를 추가: shininess 가 높으면 작은 면적의 하이라이트가 생성된다.

	vec3 viewDir = normalize (viewPos - FragPos);					//--- 관찰자의 방향
	vec3 reflectDir = reflect (-lightDir, normalVector);			//--- 반사 방향: reflect 함수 - 입사 벡터의 반사 방향 계산

	float specularLight = max (dot (viewDir, reflectDir), 0.0);		//--- V와 R의 내적값으로 강도 조절: 음수 방지
	specularLight = pow(specularLight, shininess);					//--- shininess 승을 해주어 하이라이트를 만들어준다.
	vec3 specular = specularLight * lightColor;						//--- 거울 반사 조명값: 거울반사값 * 조명색상값

	vec3 result = (ambient + diffuse + specular) * objectColor;		//--- 최종 조명 설정된 픽셀 색상: (주변조명 + 산란반사조명 + 거울반사조명) * 객체 색상


	FragColor = vec4 (result, 1.0);									//--- 픽셀 색을 출력
}