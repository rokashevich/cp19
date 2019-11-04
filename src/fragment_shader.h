// header file generated by txt2h, 2003-2006 by ScottManDeath
#ifndef  TXT_HEADER_fragment_shader
#define  TXT_HEADER_fragment_shader


const char fragment_shader [] =
"#version 300 es\n"
"precision mediump float;\n"
"in vec3 vColor;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	if (vColor == vec3(0,0,0)) {\n"
"		discard;\n"
"	}\n"
"	FragColor = vec4(vColor, 1.0);\n"
"}\n"
;


#endif  // #ifdef TXT_HEADER_fragment_shader
