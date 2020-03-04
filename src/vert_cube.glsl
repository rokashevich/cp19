#version 300 es
layout (location = 0) in vec3 pos_in;
layout (location = 1) in float arg;
uniform int const_params[1];
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 vColor;

void main() {
    vec3 pos = pos_in;
    vec3 offset;
    int cube_dimension = const_params[0];

    offset = vec3(-gl_InstanceID,1,-1);

    pos = vec3(0, pos_in.y, pos_in.x); // поворот в плоскость yz

    int a = floatBitsToInt(arg);
    if (a == 33) {
            vColor = vec3(1,0,0);
    } else {
            vColor = vec3(0,1,0);
    }
    gl_Position = projection * view * model * vec4(pos + offset, 1.0);
}
