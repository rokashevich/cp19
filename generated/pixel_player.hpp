#pragma once
const char pixel_player[] = R"(#version 300 es

precision mediump float;
in vec3 vColor;
in vec3 vBC;
out vec4 FragColor;
void main()
{
  //http://codeflow.org/entries/2012/aug/02/easy-wireframe-display-with-barycentric-coordinates/
  //FragColor = vec4(vColor, 1.0);
  if(any(lessThan(vBC, vec3(0.01)))){
      FragColor = vec4(0,0,0, 1.0);
  }
  else{
      FragColor = vec4(vColor, 1.0);
  }
  //FragColor = vec4(vColor, 1.0);
}
)";