#version 300 es
precision mediump float;
in vec3 vColor;
out vec4 FragColor;
in vec3 vBC;
void main()
{
  if(any(lessThan(vBC, vec3(0.01)))){
      FragColor = vec4(1,0.5,0.5, 1.0);
  }
  else{
      FragColor = vec4(vColor, 1.0);
  }
  //FragColor = vec4(vColor, 1.0);
}
