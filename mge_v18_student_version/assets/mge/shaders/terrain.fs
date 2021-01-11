//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform sampler2D diffuseTexture1;
uniform sampler2D diffuseTexture2;
uniform sampler2D diffuseTexture3;
uniform sampler2D diffuseTexture4;
uniform sampler2D waterTexture;


uniform vec3 diffuseColor;
uniform vec3 ambientLightColor;
uniform vec3 directionalLightColor;

uniform float	shinyness;
uniform vec3	specularColor;
uniform vec3  	cameraPos;
uniform float 	maxReach;
uniform float 	c1;
uniform float 	c2;
uniform float 	c3;
uniform int 	type;
uniform vec3 	lightDirection;
uniform float 	spotlightAngleStart;
uniform float 	spotlightAngleEnd;
uniform vec3  	lightPosition;
uniform float  	scale;

in vec4 textureDecider;
in vec3  vertNormal;
in vec2 texCoord;
in vec3  worldPosition;
in float increaseHeight;
in float timeS;

out vec4 fragment_color;

void main( void ) {
	vec3 fragNormal = normalize(cross(dFdx(worldPosition), dFdy(worldPosition)));
	vec3 blending = abs( fragNormal );
	blending = normalize(max(blending, 0));
	float b = (blending.x + blending.y + blending.z);
	blending /= vec3(b, b, b);//calculating blend for each axis

	vec4 color;
	vec3 actualPos= worldPosition;
	actualPos.y+=increaseHeight;


	vec4 xaxis;
	vec4 yaxis;
	vec4 zaxis;

	if(textureDecider.r!=0)
	{
		//color = texture(diffuseTexture4,texCoord);
		xaxis = texture( diffuseTexture4, worldPosition.yz*scale);
		yaxis = texture( diffuseTexture4, worldPosition.xz*scale);
		zaxis = texture( diffuseTexture4, worldPosition.xy*scale);
	}
	else if(textureDecider.g!=0)
	{
		//color = texture(diffuseTexture1,texCoord);
		xaxis = texture( diffuseTexture1, worldPosition.yz*scale);
		yaxis = texture( diffuseTexture1, worldPosition.xz*scale);
		zaxis = texture( diffuseTexture1, worldPosition.xy*scale);
	}
	else if(textureDecider.b!=0)
	{
		//color = texture(diffuseTexture2,texCoord);
		xaxis = texture( diffuseTexture2, worldPosition.yz*scale);
		yaxis = texture( diffuseTexture2, worldPosition.xz*scale);
		zaxis = texture( diffuseTexture2, worldPosition.xy*scale);
	}
	else
	{
		//color = texture(waterTexture,texCoord);
		xaxis = texture( waterTexture, worldPosition.yz*scale);
		yaxis = texture( waterTexture, vec2(worldPosition.x+cos(timeS),worldPosition.z)*scale);
		zaxis = texture( waterTexture, worldPosition.xy*scale);		
	}

	float lightIntensity;
	vec3 directionalLightVector;
	float distance = length(actualPos-lightPosition);

	if(type==0)
	{
		directionalLightVector = actualPos-(lightPosition);

	    lightIntensity = max(0,(maxReach/distance));
	}
	else if(type==1)
	{
		directionalLightVector=lightDirection;
		lightIntensity=1;
	}
	else
	{
		directionalLightVector = actualPos-(lightPosition);
		lightIntensity =max(0, 1-((dot(normalize(-directionalLightVector),normalize(vertNormal))-spotlightAngleStart)/(spotlightAngleEnd-spotlightAngleStart)));
	}
	lightIntensity = lightIntensity / (c1+c2*distance+c3*distance*distance);

	float diffuseIntensity = max (0,dot(-normalize(directionalLightVector), normalize (vertNormal)));

    vec3 ambientTerm = ambientLightColor * diffuseColor;
    vec3 diffuseTerm = diffuseIntensity * directionalLightColor * diffuseColor;

    vec3 reflectedLight = (actualPos-lightPosition);

	vec3 specularLight = pow(max(0,dot(reflectedLight,normalize(cameraPos-actualPos))), shinyness) * directionalLightColor * specularColor;

	color = xaxis * blending.x + yaxis * blending.y + zaxis * blending.z;

	color = color * 2.0 - 1.0;
	color = normalize( color );
	color = color * lightIntensity+vec4(diffuseColor* lightIntensity,1);

	fragment_color = color;//vec4(directionalLightColor,1);
}
