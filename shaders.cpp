#include <cstdio>
#include <GL/glew.h>
#include "shaders.h"

bool validateShader(GLuint shader, const char *file) {
	static const unsigned int BUFFER_SIZE = 512;
	char buffer[BUFFER_SIZE];
	GLsizei length = 0;

	glGetShaderInfoLog(shader, BUFFER_SIZE, &length, buffer);

	if(length > 0) {
		printf("Shader %d(%s) compile error: %s\n", shader, (file? file: ""), buffer);
		return false;
	}
	return true;
}

bool validateProgram(GLuint program) {
	static const GLsizei BUFFER_SIZE = 512;
	GLchar buffer[BUFFER_SIZE];
	GLsizei length = 0;

	glGetProgramInfoLog(program, BUFFER_SIZE, &length, buffer);

	if(length > 0) {
		printf("Program %d link error: %s\n", program, buffer);
		return false;
	}

	return true;
}

void initShaders(GLuint shaderId) {
	GLuint shaderV = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shaderV, 1, &vertexShader, 0);
	glCompileShader(shaderV);
	validateShader(shaderV, vertexShader);
	glAttachShader(shaderId, shaderV);
	glDeleteShader(shaderV);

	GLuint shaderF = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shaderF, 1, &fragmentShader, 0);
	glCompileShader(shaderF);
	validateShader(shaderF, fragmentShader);
	glAttachShader(shaderId, shaderF);
	glDeleteShader(shaderF);
}
