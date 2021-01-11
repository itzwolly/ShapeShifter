#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "glm.hpp"
#include "mge/core/GameObject.hpp"

/**
 * Exercise for the student: implement the Light class...
 * Imagine things like setLightType, setLightIntensity, setFalloffAngle etc, see the corresponding lectures
 */
class ShadowGen;
class Light : public GameObject
{
	public:
		Light(const std::string& aName = nullptr, float c1 = 1, float c2=0, float c3 = 0, const glm::vec3& aPosition = glm::vec3( 2.0f, 10.0f, 5.0f), int type = 0,float pMaxReach=3, float pAngleFallOffStart=0,
			float pAngleFallOffEnd=5, glm::vec3 pAmbientLightColor = glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3 pLightColor = glm::vec3(1, 1, 1), 
			glm::vec3 pLightDirection = glm::vec3(0,-1,0));
		virtual ~Light();
        //override set parent to register/deregister light...
        virtual void _setWorldRecursively (World* pWorld) override;

		glm::vec3 GetDirection();
		int GetType();
		float GetC1();
		float GetC2();
		float GetC3();
		void SwitchType();
		float GetMaxReach();
		glm::vec3 GetAmbientLightColor();
		glm::vec3 GetLightColor();
		float GetSpotAngleStart();
		float GetSpotAngleEnd();
		void setShadowGen(ShadowGen* pMaterial);
		ShadowGen* getShadowGen() const;
private:
	float _c1;
	float _c2;
	float _c3;
	int _type;
	glm::vec3 _ambientLightColor;
	glm::vec3 _lightColor;
	glm::vec3 _direction;
	float _maxReach;
	float _angleFallOffStart;
	float _angleFallOffEnd;
	ShadowGen* _shadowGen;
};

#endif // LIGHT_HPP
