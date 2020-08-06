#version 300 es

layout (location = 0) in vec3 pos_in;
layout (location = 1) in vec4 instanced_arg;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 vColor;

vec3 color(int health){
  return vec3(1,1,0);
}

void main() {
  vec3 pos = vec3(pos_in.x, pos_in.y, pos_in.z);
  vColor = vec3(1,1,0);
  gl_Position = projection * view * model * vec4(pos, 1.0);

}
