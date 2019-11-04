#version 300 es
layout (location = 0) in vec3 pos_in;
//layout (location = 1) in float arg;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float v[10];
out vec3 vColor;

void main() {
	vec3 pos = pos_in;
	vec3 offset;
	if (gl_InstanceID == 1) {  // 4 red
		offset = vec3(-2,1,-1);
		vColor = vec3(1,0,0);
		pos = vec3(0, pos_in.y, pos_in.x); // поворот в плоскость yz
	} else if (gl_InstanceID == 2) { // 3 green
		offset = vec3(0,1,-1);
		vColor = vec3(0,1,0);
		pos = vec3(0, pos_in.y, pos_in.x); // поворот в плоскость yz
	} else if (gl_InstanceID == 3) { // 2 yellow
		offset = vec3(-1,2,-1);
		vColor = vec3(1,1,0);
		pos = vec3(pos_in.x, 0, pos_in.y); // поворот в плоскость xz
	} else if (gl_InstanceID == 4) { // 1 blue
		offset = vec3(-1,0,-1);
		vColor = vec3(0,0,1);
		pos = vec3(pos_in.x, 0, pos_in.y); // поворот в плоскость xz
	} else {
		offset = vec3(-1,1,-2);
		vColor = vec3(0,0,0);
	}
	//  int a = floatBitsToInt(arg);
	//  if (a == 33) {
	//    vColor = vec3(0,1,0);
	//    gl_Position = projection * view * model * vec4(pos + offset, 1.0);
	//  } else if (a == 11) {
	//    vColor = vec3(1,0,0);
	//    vec3 newpos = vec3(pos.x, pos.y, pos.z);
	//    gl_Position = projection * view * model * vec4(newpos + offset, 1.0);
	//    //pos = ;
	//  }
	gl_Position = projection * view * model * vec4(pos + offset, 1.0);
}
