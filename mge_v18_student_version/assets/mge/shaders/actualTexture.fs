//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform sampler2D diffuseTexture1;
uniform sampler2D diffuseTexture2;
uniform sampler2D diffuseTexture3;
uniform sampler2D normalMap;
uniform sampler2D glossMap;

uniform bool useNormalMap;
uniform bool useGlossMap;
uniform float useCompleteReflection;

uniform float   transparency1;
uniform float   transparency2;
uniform float   transparency3;

uniform vec3 diffuseColor;
//uniform vec3 ambientLightColor;
//uniform vec3 directionalLightColor;

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


uniform float	shinyness;
uniform vec3	specularColor;
uniform vec3  	cameraPos;
uniform float  	scale;
uniform int  	lightNr;
uniform float   scrollMultiplier1;
uniform float   scrollMultiplier2;
uniform float   scrollMultiplier3;

in vec4 textureDecider;
in vec3  vertNormal;
in vec2 texCoord;
in vec3  worldPosition;
in float timeS;
in vec4 vertTangent;
//in vec4 vertBitangent;
//in mat3 TBN;
//varying mat3 TBN

out vec4 fragment_color;

void main( void ) {
	mat3 TBN;
	vec4 color= vec4(0,0,0,0);
	//vec3 actualPos= worldPosition;

	vec4 tex1= texture( diffuseTexture1, vec2(texCoord.x,texCoord.y+timeS*scrollMultiplier1))*textureDecider.r;
	vec4 tex2= texture( diffuseTexture2, vec2(texCoord.x,texCoord.y+timeS*scrollMultiplier2))*textureDecider.g;
	vec4 tex3= texture( diffuseTexture3, vec2(texCoord.x,texCoord.y+timeS*scrollMultiplier3))*textureDecider.b;
	//vec4 tex4= texture( diffuseTexture4, vec2(texCoord.x,texCoord.y+timeS*scrollMultiplier))*textureDecider.a;
	vec3 actualNormal;
	if(useNormalMap)
	{ 
		vec3 Normal = normalize(vertNormal);
	    vec3 Tangent = normalize(vec3(vertTangent));
	    Tangent = normalize(Tangent - dot(Tangent, Normal) * Normal);
	    vec3 Bitangent = cross(Tangent, Normal);
	    vec3 BumpMapNormal = texture(normalMap, texCoord).xyz;
	    BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0);
	  
	    TBN = mat3(Tangent, Bitangent, Normal);
	    actualNormal = BumpMapNormal;
	    normalize(actualNormal);
	}
	else
	{
		actualNormal=normalize(vertNormal);
		TBN = mat3(1,0,0,0,1,0,0,0,1);
	}
	
	
	//vec4 fragCol = tex1+tex2+tex3;//+tex4;
	//fragCol.a = transparency1*textureDecider.r +transparency2*textureDecider.g +transparency3*textureDecider.b ;//+transparency4*textureDecider.a; 

	int actualLighNr = 0;
	vec3 actualLightPos;
	vec3 actualCamPos = cameraPos*TBN;
	vec3 actualFragPos = worldPosition*TBN;
	float actualShyniness;
	float completeShyniness=0;//vec3(0,0,0);

  	for (int i = 0; i < lightNr; i++) {
  		actualLightPos = lights[i].lightPosition*TBN;
		float lightIntensity;
		vec3 directionalLightVector;
		float distance = length(actualFragPos-actualLightPos);

		if(lights[i].type==0)
		{
			directionalLightVector = (actualFragPos-(actualLightPos));

		    lightIntensity = max(0,(lights[i].maxReach/distance));
		}
		else if(lights[i].type==1)
		{
			directionalLightVector=lights[i].lightDirection;
			lightIntensity=1;
		}
		else
		{
			directionalLightVector = actualFragPos-(lights[i].lightPosition);
			lightIntensity = 1-((dot(normalize(-directionalLightVector),(actualNormal))-lights[i].spotlightAngleStart)/(lights[i].spotlightAngleEnd-lights[i].spotlightAngleStart));
		}

		lightIntensity = (lightIntensity * (dot(actualNormal, normalize(-directionalLightVector)))) / (lights[i].c1+lights[i].c2*distance+lights[i].c3*distance*distance);
		float diffuseIntensity = max (0,dot(-normalize(directionalLightVector),  (actualNormal)));

	    vec3 ambientTerm = lights[i].ambientLightColor * diffuseColor;
	    vec3 diffuseTerm = lights[i].directionalLightColor * diffuseColor;
	    vec3 ambpdif = normalize(diffuseTerm+ambientTerm);

	    vec3 reflectedLight = (actualFragPos-actualLightPos);

	    //reflectedLight = reflectedLight-2*(dot(reflectedLight,(actualNormal)))*(actualNormal);
	    reflectedLight = normalize(reflect(reflectedLight, (actualNormal)));
		
		actualShyniness = shinyness;
		if(useGlossMap)
		{
			actualShyniness = actualShyniness*(1-texture(glossMap,texCoord).x);
		}
		vec3 specularLight = pow(max(0,dot(reflectedLight,normalize(actualCamPos-actualFragPos))), actualShyniness)  * specularColor;
		if(actualShyniness==0)
		{
			specularLight = vec3(0,0,0);
		}
		vec4 tempColor = vec4 (ambpdif*lightIntensity , 1) + vec4 ((specularLight)*lightIntensity , 0);
		color += (tempColor);
		if(tempColor.x!=0&&tempColor.y!=0&&tempColor.z!=0)
		{
			actualLighNr++;
		}
		completeShyniness+=length((specularLight)*lightIntensity)/3;
	}
	completeShyniness = completeShyniness/actualLighNr;
	float val =  (completeShyniness*(useCompleteReflection));
	vec4 nColor = (tex1+tex2+tex3);
	
	if (nColor.a < 0.9) {
		discard;
	}
	
	fragment_color = nColor * (2*color-1)/actualLighNr;
	fragment_color.a = (transparency1 * textureDecider.r + transparency2 * textureDecider.g + transparency3 * textureDecider.b); //+transparency4*textureDecider.a;	
}