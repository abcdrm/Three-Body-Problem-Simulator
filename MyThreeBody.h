#pragma once
#include "ThreeBody.h"
class MyThreeBody :
	public ThreeBody
{
public:
	MyThreeBody();
	void update(float deltaTime);
	void reset();
	void setGravitationalConstant(float c);
	void setPlanetAMess(float m);
	void setPlanetBMess(float m);
	void setPlanetCMess(float m);
	~MyThreeBody();

protected:
	void updatePlanet(Planet& p, float t, Vec3& acc);
	void computePotentialEnergy();
	void computeTotalEnergy();

private:
	float potentialEnergyAB;
	float potentialEnergyBC;
	float potentialEnergyAC;
	float totalEnergy;
};

