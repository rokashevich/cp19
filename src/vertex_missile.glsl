#version 300 es

layout(location = 0) in vec3 vertex_in;  // Вершина базового объекта.
layout(location = 1) in vec4 offset_in;  // Его смещение относительно 0,0,0.
layout(location = 2) in vec3 params_in;  // Длина, ширина, здоровье.
layout(location = 3) in vec3 angles_in;  // Углы поворотов.
layout(location = 4) in vec3 barycentric_in;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 vColor;
out vec3 vBC;

void main() {
  vec3 offset = vec3(offset_in.x, offset_in.y, offset_in.z);
  float scale = 0.2;  // offset_in.w;
  vec3 pos =
      vec3(vertex_in.x * scale, vertex_in.y * scale, vertex_in.z * scale);

  if (barycentric_in == vec3(0, 0, 1))
    vColor = vec3(1, 0, 0);
  else
    vColor = vec3(1, 1, 0);
  gl_Position = projection * view * model * vec4(pos + offset, 1.0);

  vBC = barycentric_in;
}
