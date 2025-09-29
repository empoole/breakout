#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <limits>
#include <random>

#include "utils.h"
#include "inputHandlers.h"
#include "graphicsBuffer.h"
#include "shaders.h"
#include "ball.h"
#include "bricks.h"
#include "player.h"

/*********************
* -- GLOBAL VARS  -- *
**********************/
bool isRunning = false;
Brick bricks[168];

void resetGameState(Player* player, Ball* ball, Buffer* buffer) {
	player->x = buffer->width / 2;
	player->y = 40;
	ball->x = buffer->width / 2;
	ball->y = buffer->height / 2;
	ball->direction_y = 1;
	ball->direction_x = 1;
}

float randomFloat(float min, float max) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(min, max);
	return dis(gen);
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
		0.0f, 0.0f, // direction x and y
		1.0f, // speed
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

	initBricks(bricks, 10, 10, buffer.width, 7);

	/******************
	* -- MAIN LOOP -- *
	*******************/
	while(!glfwWindowShouldClose(window) && isRunning) {
		/* -- Poll Events -- */
		glfwPollEvents();

		bufferClear(&buffer, clearColor);

		/* -- Draw -- */
		drawBricks(&buffer, 10, 10, bricks);

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



		/* -- SIMULATION -- */
		handleKeyboardEvents(window);
		// Player
		float new_x = player.x + player.direction * player.speed;
		// Keep player within screen bounds
		if(new_x <= 0.0f) {
			new_x = 0.0f;
		} else if(new_x + player.width >= buffer.width) {
			new_x = buffer.width - player.width;
		}
		player.x = new_x;


		// Ball
		if(ball.direction_x != 0 && ball.direction_y != 0) {
			float new_x = ball.x + ball.direction_x * ball.speed;
			float new_y = ball.y + ball.direction_y * ball.speed;

			// Left and right wall collisions
			if(new_x <= 0) {
				new_x = 0;
				ball.direction_x *= -1;
			} else if (new_x + 6 >= buffer.width) {
				new_x = buffer.width - 6;
				ball.direction_x *= -1;
			}

			// Ceiling and floor collisions
			if(new_y + 6 >= buffer.height) {
				new_y = buffer.height - 6;
				ball.direction_y *= -1;
			} else if(new_y <= 0) {
				resetGameState(&player, &ball, &buffer);
				initBricks(bricks, 10, 10, buffer.width, 7);
				continue;
			}

			// Player collisions
			if(
				new_y - 6 <= player.y &&
				new_y >= player.y - player.height &&
				new_x + 6 >= player.x &&
				new_x <= player.x + player.width
			) {
				new_y = player.y + 6;
				ball.direction_y = -1 * randomFloat(0.1, 1.0) + player.speed;
			}

			// Brick collisions
			for(size_t i = 0; i < 168; ++i) {
				Brick* brick = &bricks[i];
				if (detectBrickCollision(new_x, new_y, brick)) {
					ball.direction_y *= -1;
					ball.direction_x *= -1;
					brick->isBroken = true;
					break;
				}
			}

			// Update ball position
			ball.x = new_x;
			ball.y = new_y;
		}

		glfwSwapBuffers(window);
	} /* - END MAIN LOOP - */

	glfwDestroyWindow(window);
	glfwTerminate();

	glDeleteVertexArrays(1, &fullscreenTriangeVAO);

	return 0;
}
