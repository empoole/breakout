#ifndef SHADERS
#define SHADERS

#include <GL/glew.h>

static const char* fragmentShader =
"\n"
"#version 330\n"
"\n"
"uniform sampler2D buffer;\n"
"noperspective in vec2 TexCoord;\n"
"\n"
"out vec3 outColor;\n"
"\n"
"void main(void){\n"
"	outColor = texture(buffer, TexCoord).rgb;\n"
"}\n";

static const char* vertexShader =
"\n"
"#version 330\n"
"\n"
"noperspective out vec2 TexCoord;\n"
"\n"
"void main(void){\n"
"\n"
"   TexCoord.x = (gl_VertexID == 2)? 2.0: 0.0;\n"
"   TexCoord.y = (gl_VertexID == 1)? 2.0: 0.0;\n"
"	\n"
"	gl_Position = vec4(2.0 * TexCoord - 1.0, 0.0, 1.0);\n"
"}\n";


bool validateShader(GLuint shader, const char *file = 0);

bool validateProgram(GLuint program);

void initShaders(GLuint shaderId);

#endif // SHADERS
