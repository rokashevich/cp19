#version 300 es

layout (location = 1) in vec3 pos_in;
layout (location = 3) in vec4 instanced_arg;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 vColor;

void main() {
  vec3 pos = vec3(pos_in.x, pos_in.y, pos_in.z);
  vec3 offset = vec3(instanced_arg.x,instanced_arg.y,instanced_arg.z);
  vColor = vec3(1,1,0);
  gl_Position = projection * view * model * vec4(pos + offset, 1.0);
}
