#version 330

// attributes
in vec3	i_position;	// xyz - position
in vec3	i_normal;	// xyz - normal
in vec2	i_texcoord0;	// xy - texture coords

// matrices
uniform mat4 u_modelMat;
uniform mat4 u_viewMat;
uniform mat4 u_projMat;
uniform mat3 u_normalMat;

// position of light and camera
uniform vec3 u_lightPosition;
uniform vec3 u_cameraPosition;

// data for fragment shader
out vec3 o_normal;
out vec3 o_toLight;
out vec3 o_toCamera;
out vec2 o_texcoords;

///////////////////////////////////////////////////////////////////

void main(void)
{
   // position in world space
   vec4 worldPosition = u_modelMat * vec4(i_position, 1);
   

   // normal in world space
   o_normal	= normalize(u_normalMat * i_normal);

   // direction to light
   o_toLight	= normalize(u_lightPosition - worldPosition.xyz);

   // direction to camera
   o_toCamera	= normalize(u_cameraPosition - worldPosition.xyz);

   // texture coordinates to fragment shader
   o_texcoords	= i_texcoord0;

   // screen space coordinates of the vertex
   gl_Position	= u_projMat * u_viewMat * worldPosition;
}