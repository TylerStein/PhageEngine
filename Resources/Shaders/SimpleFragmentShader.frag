#version 400
uniform sampler2D fTexture;
uniform vec3 diffuse;
uniform vec3 ambient;
uniform vec3 specular;
uniform float shininess;
uniform float alpha;

in vec2 fTexCoord;
in vec4 fColor;
in vec3 fNormal;

in vec3 fVert;

in vec3 fLightSource;
in vec3 fLightPower;

in mat4 fModel;

out vec4 finalColor;

void main(){
	//calculate normal in world coordinates
    mat3 normalMatrix = transpose(inverse(mat3(fModel)));
    vec3 normal = normalize(normalMatrix * fNormal);
    
    //calculate the location of this fragment (pixel) in world coordinates
    vec3 fragPosition = vec3(fModel * vec4(fVert, 1));
    
    //calculate the vector from this pixels surface to the light source
    vec3 surfaceToLight = fLightSource - fVert;

    //calculate the cosine of the angle of incidence
    float brightness = dot(normal, surfaceToLight) / (length(surfaceToLight) * length(normal));
    brightness = clamp(brightness, 0, 1);

    //calculate final color of the pixel plus ambient
    vec4 surfaceColor = texture(fTexture, fTexCoord);
	vec4 finalAmbient = vec4(ambient, 1.0);
    finalColor = finalAmbient + vec4(brightness * fLightPower * surfaceColor.rgb, alpha);
}