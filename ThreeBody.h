#pragma once
#include "Planet.h"
#include <time.h>
#include <math.h>

class ThreeBody
{
public:
	ThreeBody()
	{
		this->gravitationalConstant = 100.0f;
	}

	virtual void reset()
	{
		randomPlanet(this->planetA);
		randomPlanet(this->planetB);
		randomPlanet(this->planetC);
	}

	virtual void setGravitationalConstant(float c)
	{
		this->gravitationalConstant = c;
	}

	virtual void setPlanetAMess(float m)
	{
		this->planetA.setMess(m);
	}

	virtual void setPlanetBMess(float m)
	{
		this->planetB.setMess(m);
	}

	virtual void setPlanetCMess(float m)
	{
		this->planetC.setMess(m);
	}

	Planet& getPlanetA()
	{
		return this->planetA;
	}

	Planet& getPlanetB()
	{
		return this->planetB;
	}

	Planet& getPlanetC()
	{
		return this->planetC;
	}

	virtual void update(float deltaTime) = 0;

protected:
	float gravitationalConstant;
	Planet planetA, planetB, planetC;
	
	void randomPlanet(Planet& p);
	void computeAcceleration(Planet& planet, Planet& p1, Planet& p2, Vec3& acc);
};

