#version 300 es

layout(location = 0) in vec3 vertex_in;  // Вершина базового объекта.
layout(location = 1) in vec3 offset_in;  // Его смещение относительно 0,0,0.
layout(location = 2) in vec3 angles_in;  // Углы поворотов.
layout(location = 3) in vec3 params_in;  // Длина, ширина, здоровье.
layout(location = 4) in vec3 barycentric_in;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 vColor;
out vec3 vBC;
void main() {
  vec3 offset;
  vec3 pos;
  offset = vec3(offset_in.x, offset_in.y, offset_in.z);
  pos = vec3(vertex_in.x, vertex_in.y, vertex_in.z);
  // vec4 offset = vec4(offset_in.x, offset_in.y, offset_in.z, 1.0);
  // float scale = 1.0;  // offset_in.w;
  // vec4 vertex =
  //     vec4(vertex_in.x * scale, vertex_in.y * scale, vertex_in.z *
  //     scale, 1.0);

  vBC = barycentric_in;

  // float angle;
  // if (offset.x == 0.0 && offset.y == 0.0 && offset.z == 0.0) {
  //   vColor = vec3(0, 1, 0);
  //   angle = 45.0;
  // } else {
  vColor = vec3(1, 1, 0);
  // angle = 0.0;
  // }
  // float s = sin(angle);
  // float c = cos(angle);
  // clang-format off
  // mat4 rot = mat4(
  //     c, 0.0,   s, 0,
  //   0.0, 1.0, 0.0, 0,
  //    -s, 0.0,   c, 0,
  //   0.0, 0.0, 0.0, 1.0);
  // mat4 trans_to = mat4(
  //   1.0, 0.0, 0.0, vertex.x,
  //   0.0, 1.0, 0.0, vertex.y,
  //   0.0, 0.0, 1.0, vertex.z,
  //   0.0, 0.0, 0.0, 1
  // );
  // mat4 trans_back = mat4(
  //   1.0, 0.0, 0.0,  vertex.x,
  //   0.0, 1.0, 0.0,  vertex.y,
  //   0.0, 0.0, 1.0,  vertex.z,
  //   0.0, 0.0, 0.0, 1
  // );
  // clang-format on
  // vertex = trans_to * rot * vertex;
  // vertex = trans_back * vertex;
  // vertex = projection * view * model * (vertex + offset);
  // gl_Position = vertex;
  gl_Position = projection * view * model * vec4(pos + offset, 1.0);
}
