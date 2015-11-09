#pragma once
#include <glew.h>
#include <glm\glm.hpp>
#include <iostream>
#include <fstream>
using namespace std;
using namespace glm;


class Shader
{
public:
	static char* loadTextFile(const char* file);

	static GLuint loadShader(const char* file, GLenum shaderType);

	static GLuint loadShaderProgram(const char* vertexFile, const char* fragmentFile);

private:
	Shader();
	~Shader();
};

