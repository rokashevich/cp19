#version 300 es
precision mediump float;
in vec3 vColor;
in vec3 vBC;
out vec4 FragColor;
void main()
{
  if(any(lessThan(vBC, vec3(0.05)))){
      FragColor = vec4(0,0,0, 1.0);
  }
  else{
      FragColor = vec4(vColor, 1.0);
  }
  FragColor = vec4(vColor, 1.0);
}
