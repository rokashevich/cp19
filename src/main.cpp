#include <EGL/egl.h>
#include <GLES3/gl32.h>
#include "SDL.h"
//#include "SDL_opengl.h"
#include <time.h>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

static const char *vertexShaderSource =
		R"END(
		#version 300 es
		layout (location = 0) in vec3 pos_in;
		//layout (location = 1) in float arg;
		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;
		uniform float v[10];
		out vec3 vColor;
		void main() {
		vec3 pos = pos_in;
		vec3 offset;
		if (gl_InstanceID == 1) {  // 4 red
			offset = vec3(-2,1,-1);
			vColor = vec3(1,0,0);
			pos = vec3(0, pos_in.y, pos_in.x); // поворот в плоскость yz
		} else if (gl_InstanceID == 2) { // 3 green
			offset = vec3(0,1,-1);
			vColor = vec3(0,1,0);
			pos = vec3(0, pos_in.y, pos_in.x); // поворот в плоскость yz
		} else if (gl_InstanceID == 3) { // 2 yellow
			offset = vec3(-1,2,-1);
			vColor = vec3(1,1,0);
			pos = vec3(pos_in.x, 0, pos_in.y); // поворот в плоскость xz
		} else if (gl_InstanceID == 4) { // 1 blue
			offset = vec3(-1,0,-1);
			vColor = vec3(0,0,1);
			pos = vec3(pos_in.x, 0, pos_in.y); // поворот в плоскость xz
		} else {
			offset = vec3(-1,1,-2);
			vColor = vec3(1,1,1);
		}
		//  int a = floatBitsToInt(arg);
		//  if (a == 33) {
		//    vColor = vec3(0,1,0);
		//    gl_Position = projection * view * model * vec4(pos + offset, 1.0);
		//  } else if (a == 11) {
		//    vColor = vec3(1,0,0);
		//    vec3 newpos = vec3(pos.x, pos.y, pos.z);
		//    gl_Position = projection * view * model * vec4(newpos + offset, 1.0);
		//    //pos = ;
		//  }
		gl_Position = projection * view * model * vec4(pos + offset, 1.0);
		}
		)END";

static const char *fragmentShaderSource =
		R"END(
		#version 300 es
		precision mediump float;
		in vec3 vColor;
		out vec4 FragColor;
		void main()
		{
		FragColor = vec4(vColor, 1.0);
		}
		)END";

// camera
static glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
static glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
static glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

static float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
static float pitch =  0.0f;
static float fov   =  45.0f;

// timing
static float deltaTime = 0.0f;	// time between current frame and last frame
static float lastFrame = 0.0f;

void render(){
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glClearColor(1.f, 0.f, 1.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);
}

struct Square {
	float x;
	float y;
	float z;
	float a;
	float t;
};

int main(int, char**) { // аргументы не используются, но с пустым main() падает
	SDL_Window *window;
	int done;
	SDL_Event event;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Could not initialize SDL\n");
		return 1;
	}
	window = SDL_CreateWindow(nullptr, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
	if (!window) {
		printf("Could not initialize Window\n");
		return 1;
	}
	SDL_GL_CreateContext(window);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	// vertext shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		printf("Vertex error: %s\n", infoLog);
	}

	// fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		printf("Fragment error: %s\n", infoLog);
	}

	// link shaders
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		printf("ERROR: %s\n", infoLog);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	int a = 33;
	float aa = *((float*)&a);
	int b = 11;
	float bb = *((float*)&b);
	float c[5] = {aa,aa,aa,aa,bb};


	float quadVertices[] = {
		-1.0f,  1.0f,
		1.0f, -1.0f,
		-1.0f, -1.0f,
		-1.0f,  1.0f,
		1.0f, -1.0f,
		1.0f,  1.0f
	};

	unsigned int instanceVBO;
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*5, &c[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0 * sizeof(float), nullptr);


	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); // this attribute comes from a different vertex buffer
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(1, 1);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	int res;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS,&res);
	std::cout << "GL_MAX_VERTEX_ATTRIBS = " << res << std::endl;
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &res);
	std::cout << "GL_MAX_VERTEX_UNIFORM_VECTORS = " << res << std::endl;


	done = 0;
	while (!done) {
		float currentFrame = SDL_GetTicks();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				done = 1;
			} else if( event.type == SDL_KEYDOWN ) {
				float cameraSpeed = 0.005f * deltaTime;
				switch( event.key.keysym.sym ) {
				case SDLK_w:
					cameraPos += cameraSpeed * cameraFront;
					break;
				case SDLK_s:
					cameraPos -= cameraSpeed * cameraFront;
					break;
				case SDLK_a:
					cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
					break;
				case SDLK_d:
					cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
					break;
				default:
					break;
				}
			} else if (event.type == SDL_MOUSEMOTION) {


				static float sensitivity = 0.1f; // change this value to your liking
				yaw += event.motion.xrel * sensitivity;
				pitch -= event.motion.yrel * sensitivity;

				// make sure that when pitch is out of bounds, screen doesn't get flipped
				if (pitch > 89.0f)
					pitch = 89.0f;
				if (pitch < -89.0f)
					pitch = -89.0f;

				glm::vec3 front;
				front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
				front.y = sin(glm::radians(pitch));
				front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
				cameraFront = glm::normalize(front);
			}
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shaderProgram);




		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

		// camera/view transformation
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);

		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		model = glm::translate(model, glm::vec3( 0.0f,  0.0f,  0.0f));
		float angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);

		GLfloat v[10] = {1,2,3.5f,4,5,6,7,8,9,10};
		glUniform1fv(glGetUniformLocation(shaderProgram, "v"), 10, v);

		glBindVertexArray(quadVAO);
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 6, 5);
		//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);

		SDL_GL_SwapWindow(window);
	}

	SDL_Quit();

	return 0;
}
