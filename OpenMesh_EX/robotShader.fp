
#version 430

out vec4 vFragColor;
uniform vec3 Color;

void main(void)
{ 
	//vFragColor = vec4(1.0,0.8,0.2,1.0);
	if(Color.r != -1.0){
		vFragColor = vec4(Color, 1.0);
		//vFragColor = vec4(vec3((gl_PrimitiveID+1)/12.0), 1.0);
	}
	else{ 
		vFragColor = vec4(float(gl_PrimitiveID+1), float(gl_PrimitiveID+1), float(gl_PrimitiveID+1), 1.0);
		//vFragColor2 = gl_PrimitiveID+1;
		//vFragColor = vec4(vec3((gl_PrimitiveID+1)/12.0), 1.0);
		//vFragColor = vec4(0.1, gl_PrimitiveID+1, gl_PrimitiveID+1, 1.0);
	}
	
	

}
	
    