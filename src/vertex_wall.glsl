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

vec3 color(int health) {
  if (health == 10)
    return vec3(1, 0, 0);
  else if (health == 9)
    return vec3(1, 1, 0);
  else if (health == 8)
    return vec3(0, 0, 1);
  else if (health == 7)
    return vec3(1, 1, 0);
  else if (health == 6)
    return vec3(0, 1, 1);
  else if (health == 5)
    return vec3(1, 0, 1);
  else if (health == 4)
    return vec3(0.5, 0, 0);
  else if (health == 3)
    return vec3(0, 0.5, 0);
  else if (health == 2)
    return vec3(0, 0, 0.5);
  else if (health == 1)
    return vec3(0.5, 0.5, 0.5);
  else
    return vec3(0, 1, 1);
}

void main() {
  // int param = int(offset_in.w);
  int surface = 1;  // param / 100;  // Плоскость: 1-xy, 2-yz, 3-xz.
  int health = 10;  // param % 100;   // Здоровье 1-10.

  vec3 offset;
  vec3 pos;

  offset = vec3(offset_in.x, offset_in.y, offset_in.z);

  if (angles_in.y == 90.0) {  // yz стена ребром
    pos = vec3(vertex_in.z, vertex_in.y, vertex_in.x);
    vColor = vec3(1, 0, 0);
  } else if (angles_in.x == 90.0) {  // xz пол-потолок
    pos = vec3(vertex_in.y, vertex_in.z, vertex_in.x);
    vColor = vec3(0, 1, 0);
  } else {  // фронтальная стена xy
    pos = vec3(vertex_in.x, vertex_in.y, vertex_in.z);
    vColor = vec3(0, 0, 1);
  }
  gl_Position = projection * view * model * vec4(pos + offset, 1.0);
  vBC = barycentric_in;
}
