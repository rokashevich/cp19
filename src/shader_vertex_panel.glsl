#version 300 es

layout (location = 0) in vec3 pos_in;
layout (location = 1) in vec4 instanced_arg;

uniform int panels_permanent[7];  // Массив готовится в GameWorld.
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 vColor;

vec3 color(int health){
  if (health == 10) return vec3(1,0,0);
  else if (health == 9) return vec3(0,1,0);
  else if (health == 8) return vec3(0,0,1);
  else if (health == 7) return vec3(1,1,0);
  else if (health == 6) return vec3(0,1,1);
  else if (health == 5) return vec3(1,0,1);
  else if (health == 4) return vec3(0.5,0,0);
  else if (health == 3) return vec3(0,0.5,0);
  else if (health == 2) return vec3(0,0,0.5);
  else if (health == 1) return vec3(0.5,0.5,0.5);
  else return vec3(1,1,1);
}

void main() {
  int param = int(instanced_arg.w);
  int surface = param / 100;  // Плоскость: 1-xy, 2-yz, 3-xz.
  int health = param % 100; // Здоровье 1-10.

  vec3 offset;
  vec3 pos;

  if (surface == 0) { // xy
    offset = vec3(instanced_arg.x,instanced_arg.y,instanced_arg.z);
    pos = vec3(pos_in.x, pos_in.y, pos_in.z);
    vColor = color(health);
  }
  else if (surface == 1) { // yz
    offset = vec3(instanced_arg.x,instanced_arg.y,instanced_arg.z);
    pos = vec3(pos_in.z, pos_in.y, pos_in.x);
    vColor = color(health);
  }
  else if (surface == 2) { // xz
    offset = vec3(instanced_arg.x,instanced_arg.y,instanced_arg.z);
    pos = vec3(pos_in.y, pos_in.z, pos_in.x);
    vColor = color(health);
  } else {
    offset = vec3(0,0,0);
    pos = vec3(pos_in.y, pos_in.z, pos_in.x);
    vColor = vec3(1,1,1);
  }

  gl_Position = projection * view * model * vec4(pos + offset, 1.0);
 /*
  float health = instanced_arg.w;
  int resolution = panels_permanent[0];
  float component = float(instanced_arg.x) / float(resolution+1);
  vColor = vec3(1,component,component);




  int slab_panels_count = panels_permanent[2];
  int storey_coherent_walls_count = panels_permanent[3];
  int storey_panels_total_ = panels_permanent[5];

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
  gl_Position = projection * view * model * vec4(pos + offset, 1.0);
*/

//  switch (material) {
//  case 1: vColor = vec3(0,0,0); break; // Пол.
//  case 2: vColor = vec3(1,0,0); break;
//  case 3: vColor = vec3(0,0,1); break;
//  default:vColor = vec3(1,1,1);
//  }

}
