#version 300 es

layout (location = 0) in vec3 pos_in;
layout (location = 1) in float instanced_arg;

uniform int ribs_permanent[7];  // Массив готовится в GameWorld.
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 vColor;

void main() {
  int arg = int(instanced_arg);
  int health = arg / 1000;
  int material = arg % 1000;

  vec3 pos;
  vec3 offset;

  int resolution = ribs_permanent[0];
  int slab_panels_count = ribs_permanent[2];
  int storey_coherent_walls_count = ribs_permanent[3];
  int storey_panels_total_ = ribs_permanent[5];

  int storey = gl_InstanceID / storey_panels_total_;  // Текущий этаж.
  int range = gl_InstanceID - storey * storey_panels_total_;

  if (range < slab_panels_count) {
    // Плоскость xz (пол).
    pos = vec3(pos_in.y, 0, pos_in.x); // Поворот панели в плоскость пола (xz).
    int row = range / resolution;
    int col = range - row * resolution;
    offset = vec3(col,storey,row);
    if (range == 0 && storey == 0) vColor = vec3(1,1,1);
  } else if (range >= slab_panels_count + storey_coherent_walls_count) {
    // Плоскость yz.
    pos = vec3(0, pos_in.y, pos_in.x); // Поворот панели в плоскость yz.
    range = range - slab_panels_count - storey_coherent_walls_count;
    int col = range / resolution;
    int row = range - col * resolution;
    offset = vec3(float(col)-0.5,float(storey)+0.5,row);
    if (range == 0 && storey == 0) vColor = vec3(1,1,1);
  } else {
    // Плоскость xy.
    pos = vec3(pos_in.x, pos_in.y, 0);
    range = range - slab_panels_count;
    int row = range / resolution;
    int col = range - row * resolution;
    offset = vec3(col,float(storey)+0.5,float(row)-0.5);
   // offset = vec3(-gl_InstanceID,storey,-1);
    if (range == 0 && storey == 0) vColor = vec3(1,1,1);
  }
  offset.y = -offset.y;
  gl_Position = projection * view * model * vec4(pos + offset, 1.0);


  switch (material) {
  case 1: vColor = vec3(0,0,0); break; // Пол.
  case 2: vColor = vec3(1,1,0); break;
  case 3: vColor = vec3(0,1,1); break;
  default:vColor = vec3(1,0,1);
  }

}
