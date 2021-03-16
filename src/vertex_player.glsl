#version 300 es

layout(location = 0) in vec3 pos_in;
layout(location = 1) in vec4 instanced_arg;
layout(location = 2) in vec3 bar;

layout(location = 3) in vec3 parent_offset;
// layout(location = 4) in vec3 parent_angles;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 vColor;
out vec3 vBC;
void main() {
  vec4 offset = vec4(instanced_arg.x, instanced_arg.y, instanced_arg.z, 1.0);
  float scale = instanced_arg.w;
  vec4 pos = vec4(pos_in.x * scale, pos_in.y * scale, pos_in.z * scale, 1.0);

  vBC = bar;

  float angle;
  if (offset.y == 0.0) {
    vColor = vec3(1, 1, 1);
    angle = 45.0;
  } else {
    vColor = vec3(0, 1, 1);
    angle = 0.0;
  }
  if (parent_offset.x == 2.0) {
    vColor = vec3(1, 0, 0);
  }
  float s = sin(angle);
  float c = cos(angle);
  // clang-format off
  mat4 rot = mat4(
      c, 0.0,   s, 0, 
    0.0, 1.0, 0.0, 0, 
     -s, 0.0,   c, 0,
    0.0, 0.0, 0.0, 1.0);
  mat4 trans_to = mat4(
    1.0, 0.0, 0.0, pos.x,
    0.0, 1.0, 0.0, pos.y,
    0.0, 0.0, 1.0, pos.z,
    0.0, 0.0, 0.0, 1
  );
  mat4 trans_back = mat4(
    1.0, 0.0, 0.0, -pos.x,
    0.0, 1.0, 0.0, -pos.y,
    0.0, 0.0, 1.0, -pos.z,
    0.0, 0.0, 0.0, 1
  );
  // clang-format on
  pos = trans_to * rot * pos;
  pos = trans_back * pos;
  pos = projection * view * model * (pos + offset);
  gl_Position = pos;
}
