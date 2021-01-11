//DIFFUSE COLOR FRAGMENT SHADER

#version 330 // for glsl version (12 is for older versions , say opengl 2.1


const int NR_LIGHTS = 16; 

struct Light {
    vec3 	lightPosition;
    vec3 	ambientLightColor;
    vec3 	directionalLightColor;
    float 	maxReach;
    float 	c1;
    float 	c2;
    float 	c3;
    int 	type;
    vec3 	lightDirection;
    float 	spotlightAngleStart;
	float 	spotlightAngleEnd;
};

uniform Light lights[NR_LIGHTS];


uniform vec3 	diffuseColor;
uniform int  	lightNr;
uniform vec3  	cameraPos;
uniform float	shinyness;
uniform vec3	specularColor;


in vec3  vertNormal;
in vec3  worldPosition;

out vec4 fragment_color;

void main( void ) {
	int actualLighNr = 0;
	vec4 color=vec4(0,0,0,0);
  	for (int i = 0; i < lightNr; i++) {
		float lightIntensity;
		vec3 directionalLightVector;
		float distance = length(worldPosition-lights[i].lightPosition);

		if(lights[i].type==0)
		{
			directionalLightVector = worldPosition-(lights[i].lightPosition);

		    lightIntensity = max(0,(lights[i].maxReach/distance));
		}
		else if(lights[i].type==1)
		{
			directionalLightVector=lights[i].lightDirection;
			lightIntensity=1;
		}
		else
		{
			directionalLightVector = worldPosition-(lights[i].lightPosition);
			lightIntensity = 1-((dot(normalize(-directionalLightVector),normalize(vertNormal))-lights[i].spotlightAngleStart)/(lights[i].spotlightAngleEnd-lights[i].spotlightAngleStart));
		}
		lightIntensity = lightIntensity / (lights[i].c1+lights[i].c2*distance+lights[i].c3*distance*distance);
		float diffuseIntensity = max (0,dot(-normalize(directionalLightVector), normalize (vertNormal)));

	    vec3 ambientTerm = lights[i].ambientLightColor * diffuseColor;
	    vec3 diffuseTerm = diffuseIntensity * lights[i].directionalLightColor * diffuseColor;

	    vec3 reflectedLight = (worldPosition-lights[i].lightPosition);

	    //reflectedLight = reflectedLight-2*(dot(reflectedLight,normalize(vertNormal)))*normalize(vertNormal);
	    reflectedLight = normalize(reflect(reflectedLight, normalize(vertNormal)));
		
		vec3 specularLight = pow(max(0,dot(reflectedLight,normalize(cameraPos-worldPosition))), shinyness) * lights[i].directionalLightColor * specularColor;
		vec4 tempColor = vec4 ((diffuseTerm+ambientTerm)*lightIntensity , 1) + vec4 (specularLight*lightIntensity , 0);
		color += tempColor;
		if(tempColor.x!=0&&tempColor.y!=0&&tempColor.z!=0)
		{
			actualLighNr++;
		}
	}
	fragment_color = color/actualLighNr;
}
