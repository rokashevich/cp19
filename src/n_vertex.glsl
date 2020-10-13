#version 300 es

layout (location = 4) in vec3 pos_in;
layout (location = 5) in vec4 instanced_arg;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 vColor;

void main() {
  vec3 offset = vec3(instanced_arg.x,instanced_arg.y,instanced_arg.z);
  float size = instanced_arg.w;
  vec3 pos = vec3(pos_in.x*size, pos_in.y*size, pos_in.z*size);

  vColor = vec3(0,1,0);
  gl_Position = projection * view * model * vec4(pos + offset, 1.0);
}
