#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <limits>

#include "utils.h"
#include "inputHandlers.h"
#include "graphicsBuffer.h"
#include "shaders.h"
#include "bricks.h"
#include "player.h"
#include "ball.h"

/*********************
* -- GLOBAL VARS  -- *
**********************/
bool isRunning = false;

void reset(Ball* ball, size_t x, size_t y) {
	ball->x = x;
	ball->y = y;
	ball->direction_x = 1;
	ball->direction_y = 1;
	ball->speed = 1;
}

int main() {
	// game objects setup
	Player player = {
		0, 0, // starting x and y
		28, 6, // width and height
		0, // direction
		2, // speed
		rgbToUint32(187, 187, 187) //color
	};

	Ball ball = {
		0, 0, // starting x and y
		0, 0, // direction x and y
		1, // speed
		rgbToUint32(195, 175, 205) // color
	};

	// GLFW Setup
	glfwSetErrorCallback(errorCallback);

	if(!glfwInit()) return -1;

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(640, 480, "BOC", NULL, NULL);
	if(!window) {
		glfwTerminate();
		return -1;
	}

	// Provide a pointer to the player instance to the keyboard handler
	glfwSetWindowUserPointer(window, &player);

	glfwSetKeyCallback(window, keyboardCallback);
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	// GLEW SETUP
	GLenum err = glewInit();
	if(err != GLEW_OK) {
		fprintf(stderr, "Error initializing GLEW.\n");
		glfwTerminate();
		return -1;
	}

	int glVersion[2] = {-1, 1};
	glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);


	// GENERAL SETUP
	printf("Using OpenGL: %d.%d\n", glVersion[0], glVersion[1]);
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Shading Language: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));


	// Graphics Buffer
	const size_t bufferWidth = 240;
	const size_t bufferHeight = 256;
	Buffer buffer;
	buffer.width = bufferWidth;
	buffer.height = bufferHeight;
	buffer.data = new uint32_t[buffer.width * buffer.height];

	bufferClear(&buffer, 0);

	// Create texture for presenting buffer to OpenGL
	GLuint bufferTexture;
	glGenTextures(1, &bufferTexture);
	glBindTexture(GL_TEXTURE_2D, bufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, buffer.width, buffer.height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, buffer.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Create vao for generating fullscreen triangle VAO
	// VAO is a Vertex Array Object, using a triange is more efficient than using a quad or another shape
	// VAOs are necessary in modern OpenGL for defining vertex attribute configurations
	GLuint fullscreenTriangeVAO;
	glGenVertexArrays(1, &fullscreenTriangeVAO);

	// Set up shaders
	GLuint shaderId = glCreateProgram();
	initShaders(shaderId);

	glLinkProgram(shaderId);

	if(!validateProgram(shaderId)) {
		fprintf(stderr, "Error validating shader.\n");
		glfwTerminate();
		glDeleteVertexArrays(1, &fullscreenTriangeVAO);
		delete buffer.data;
		return -1;
	}

	glUseProgram(shaderId);

	GLint location = glGetUniformLocation(shaderId, "buffer");
	glUniform1i(location, 0);

	/**********************
	 * -- OpenGL Setup -- *
	 **********************/
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(fullscreenTriangeVAO);

	// Prepare game
	isRunning = true;
	uint32_t clearColor = rgbToUint32(0, 0, 0);
	player.x = buffer.width / 2;
	player.y = 40;

	ball.x = buffer.width / 2;
	ball.y = buffer.height / 2;
	ball.direction_y = 1;
	ball.direction_x = 1;
	/******************
	* -- MAIN LOOP -- *
	*******************/
	while(!glfwWindowShouldClose(window) && isRunning) {
		bufferClear(&buffer, clearColor);

		/* -- Draw -- */
		drawBricks(&buffer, buffer.height, buffer.width, 10, 10);

		// Draw Player
		bufferDrawRect(&buffer, player.height, player.width, player.x, player.y, player.color);

		// Draw Ball
		bufferDrawBall(&buffer, ball.x, ball.y, ball.color);

		/* -- RENDERING -- */
		glTexSubImage2D(
			GL_TEXTURE_2D, 0, 0, 0,
			buffer.width, buffer.height,
			GL_RGBA, GL_UNSIGNED_INT_8_8_8_8,
			buffer.data
		);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glfwSwapBuffers(window);

		/* -- SIMULATION -- */
		handleKeyboardEvents(window);
		// Player
		if(player.direction != 0) {
			size_t new_x = player.x + player.direction * player.speed;
			// Keep player within screen bounds
			if(new_x >= std::numeric_limits<size_t>::max()) {
				new_x = 1;
			} else if(new_x + player.width >= buffer.width) {
				new_x = buffer.width - player.width - 1;
			}
			player.x = new_x;
		}

		// Ball
		if(ball.direction_x != 0 && ball.direction_y != 0) {
			size_t new_x = ball.x + ball.direction_x * ball.speed;
			size_t new_y = ball.y + ball.direction_y * ball.speed;

			// Left and right wall collisions
			if(new_x >= std::numeric_limits<size_t>::max()) {
				new_x = 1;
				ball.direction_x *= -1;
			} else if (new_x + 6 >= buffer.width) {
				new_x = buffer.width - 6 - 1;
				ball.direction_x *= -1;
			}

			// Ceiling and floor collisions
			if(new_y + 6 >= buffer.height) {
				new_y = buffer.height - 6 - 1;
				ball.direction_y *= -1;
			} else if(new_y >= std::numeric_limits<size_t>::max()) {
				reset(&ball, buffer.width / 2, buffer.height / 2);
			}

			// Player collisions
			if(new_y <= player.y) {
				if(new_x + 6 >= player.x && new_x + 6 <= player.x + player.width) {
					new_y = player.y + 6 + 1;
					ball.direction_y *= -1;
					if(ball.direction_x == player.direction) {
						ball.speed += player.speed / 2;
					} else if (ball.direction_x != player.direction) {
						if (ball.speed > player.speed) {
							ball.speed -= player.speed / 2;
						}
						ball.direction_x *= -1;
					}
				}
			}

			// Update ball position
			ball.x = new_x;
			ball.y = new_y;
		}

		// Bricks

		/* -- Poll Events -- */
		glfwPollEvents();
	} /* - END MAIN LOOP - */

	glfwDestroyWindow(window);
	glfwTerminate();

	glDeleteVertexArrays(1, &fullscreenTriangeVAO);

	return 0;
}
