#include <EGL/egl.h>
#include <GLES3/gl32.h>
#include <GLES3/gl3ext.h>
#include "SDL.h"
//#include "SDL_opengl.h"
#include <time.h>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

const char *vertexShaderSource = "#version 300 es\n"
    "layout (location = 0) in vec2 pos;"
    "layout (location = 1) in vec3 color;"
    "//uniform mat4 model;"
    "//uniform mat4 view;\n"
    "//uniform mat4 projection;\n"
    "out vec3 vColor;"
    "void main()\n"
    "{"
    "   gl_Position = vec4(pos, 0.0, 1.0f);\n"
    "   vColor = color;"
    "}\0";
const char *fragmentShaderSource = "#version 300 es\n"
    "precision mediump float;\n"
    "in vec3 fColor;"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(fColor, 1.0);"
    "}\n\0";

const char *geometryShaderSource = "#version 310 es\n"
    "#extension GL_EXT_geometry_shader : enable\n"
    "layout(points) in;\n"
    "layout(triangle_strip, max_vertices = 4) out;\n"
    "in vec3 vColor[];"
    "out vec3 fColor;"
    "uniform mat4 model;"
    "uniform mat4 view;"
    "uniform mat4 projection;"
    "void main(){\n"
        "fColor = vColor[0];"
        "gl_Position = projection*view*model*(gl_in[0].gl_Position + vec4(-0.1, -0.1, 0.0, 0.0));"
        "EmitVertex();\n"
        "gl_Position = projection*view*model*(gl_in[0].gl_Position + vec4( 0.1, -0.1, 0.0, 0.0));"
        "EmitVertex();\n"
        "gl_Position = projection*view*model*(gl_in[0].gl_Position + vec4(-0.1,  0.1, 0.0, 0.0));"
        "EmitVertex();\n"
        "gl_Position = projection*view*model*(gl_in[0].gl_Position + vec4( 0.1,  0.1, 0.0, 0.0));"
        "EmitVertex();\n"
        "EndPrimitive();\n"
    "}\n\0";

// camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov   =  45.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

void render(){
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glClearColor(1.f, 0.f, 1.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);
}

int main(int argc, char *argv[]) {
    SDL_Window *window;
    SDL_Renderer *renderer;
    int done;
    SDL_Event event;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL\n");
        return 1;
    }
    srand(time(NULL));
    window = SDL_CreateWindow(NULL, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    if (!window) {
        printf("Could not initialize Window\n");
        return 1;
    }
    SDL_GLContext Context = SDL_GL_CreateContext(window);

    // vertext shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success){
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      printf("ERROR: %s\n", infoLog);
    }

    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("ERROR: %s\n", infoLog);
    }

    // geometry shader
    int geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometryShader, 1, &geometryShaderSource, nullptr);
    glCompileShader(geometryShader);
    glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
        printf("ERROR: %s\n", infoLog);
    }

    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glAttachShader(shaderProgram, geometryShader);
    glLinkProgram(shaderProgram);

    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      printf("ERROR: %s\n", infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(geometryShader);

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    float points[] = {
        -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);


    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLint posAttrib = glGetAttribLocation(shaderProgram, "pos");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE,
                           6 * sizeof(float), (void*) (3 * sizeof(float)));

    done = 0;
    while (!done) {
        float currentFrame = SDL_GetTicks();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = 1;
            } else if( event.type == SDL_KEYDOWN ) {
              float cameraSpeed = 0.01 * deltaTime;
              switch( event.key.keysym.sym ) {
                case SDLK_UP:
                  cameraPos += cameraSpeed * cameraFront;
                  break;
                case SDLK_DOWN:
                  cameraPos -= cameraSpeed * cameraFront;
                  break;
                case SDLK_LEFT:
                  cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
                  break;
                case SDLK_RIGHT:
                  cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
                  break;
                default:
                  break;
                }
            } else if (event.type == SDL_MOUSEMOTION) {
              int xpos, ypos;
              SDL_GetMouseState( &xpos, &ypos );
              if (firstMouse)
                  {
                      lastX = xpos;
                      lastY = ypos;
                      firstMouse = false;
                  }

                  float xoffset = xpos - lastX;
                  float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
                  lastX = xpos;
                  lastY = ypos;

                  float sensitivity = 0.1f; // change this value to your liking
                  xoffset *= sensitivity;
                  yoffset *= sensitivity;

                  yaw += xoffset;
                  pitch += yoffset;

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

        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

        // camera/view transformation
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);

        //glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized


        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        model = glm::translate(model, glm::vec3( 0.0f,  0.0f,  0.0f));
        float angle = 0.0f;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);

        glDrawArrays(GL_POINTS, 0, 4);

        SDL_GL_SwapWindow(window);
    }

    SDL_Quit();

    return 0;
}
