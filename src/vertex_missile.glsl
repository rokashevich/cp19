#version 300 es

layout(location = 0) in vec3 pos_in;
layout(location = 1) in vec4 instanced_arg;
layout(location = 4) in vec3 bar;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 vColor;
out vec3 vBC;

void main() {
  vec3 offset = vec3(instanced_arg.x, instanced_arg.y, instanced_arg.z);
  float scale = instanced_arg.w;
  vec3 pos = vec3(pos_in.x * scale, pos_in.y * scale, pos_in.z * scale);

  if (bar == vec3(0, 0, 1))
    vColor = vec3(1, 0, 0);
  else
    vColor = vec3(1, 1, 0);
  gl_Position = projection * view * model * vec4(pos + offset, 1.0);

  vBC = bar;
}
