#include "MyThreeBody.h"
#include <iostream>


MyThreeBody::MyThreeBody()
{
	this->potentialEnergyAB = 0.0f;
	this->potentialEnergyAC = 0.0f;
	this->potentialEnergyBC = 0.0f;
	this->totalEnergy = 0.0f;
}

void MyThreeBody::updatePlanet(Planet & p, float t, Vec3 & acc)
{
	p.planetCoordinate.x = p.planetCoordinate.x + p.planetVelocity.x*t + 0.5*acc.x*t*t;
	p.planetCoordinate.y = p.planetCoordinate.y + p.planetVelocity.y*t + 0.5*acc.y*t*t;
	p.planetCoordinate.z = p.planetCoordinate.z + p.planetVelocity.z*t + 0.5*acc.z*t*t;

	p.planetVelocity.x += acc.x*t;
	p.planetVelocity.y += acc.y*t;
	p.planetVelocity.z += acc.z*t;
}

void MyThreeBody::computePotentialEnergy()
{
	Vec3 vectorAB = this->planetA.planetCoordinate - this->planetB.planetCoordinate;
	float distanceAB = vectorAB.Magnitude();
	if (distanceAB < 0.00001f)
		distanceAB = 0.00001f;
	this->potentialEnergyAB = -this->gravitationalConstant*this->planetA.planetMess*this->planetB.planetMess / distanceAB;

	Vec3 vectorBC = this->planetC.planetCoordinate - this->planetB.planetCoordinate;
	float distanceBC = vectorBC.Magnitude();
	if (distanceBC < 0.00001f)
		distanceBC = 0.00001f;
	this->potentialEnergyBC = -this->gravitationalConstant*this->planetC.planetMess*this->planetB.planetMess / distanceBC;

	Vec3 vectorAC = this->planetA.planetCoordinate - this->planetC.planetCoordinate;
	float distanceAC = vectorAC.Magnitude();
	if (distanceAC < 0.00001f)
		distanceAC = 0.00001f;
	this->potentialEnergyAC = -this->gravitationalConstant*this->planetA.planetMess*this->planetC.planetMess / distanceAC;
}

void MyThreeBody::computeTotalEnergy()
{
	float planetAKineticEnergy = this->planetA.getKineticEnergy();
	float planetBKineticEnergy = this->planetB.getKineticEnergy();
	float planetCKineticEnergy = this->planetC.getKineticEnergy();

	this->computePotentialEnergy();

	this->totalEnergy = this->potentialEnergyAB + this->potentialEnergyAC + this->potentialEnergyBC + planetAKineticEnergy + planetBKineticEnergy + planetCKineticEnergy;
}

void MyThreeBody::reset()
{
	ThreeBody::reset();
	computeTotalEnergy();
}

void MyThreeBody::setGravitationalConstant(float c)
{
	ThreeBody::setGravitationalConstant(c);
	computeTotalEnergy();
}

void MyThreeBody::setPlanetAMess(float m)
{
	this->planetA.setMess(m);
	computeTotalEnergy();
}

void MyThreeBody::setPlanetBMess(float m)
{
	this->planetA.setMess(m);
	computeTotalEnergy();
}

void MyThreeBody::setPlanetCMess(float m)
{
	this->planetA.setMess(m);
	computeTotalEnergy();
}

void MyThreeBody::update(float deltaTime)
{
	if (this->planetA.planetCoordinate.Magnitude() > 400.0f)
		this->planetA.planetVelocity = -0.8 * this->planetA.planetVelocity;
	if (this->planetB.planetCoordinate.Magnitude() > 400.0f)
		this->planetB.planetVelocity = -0.8 * this->planetB.planetVelocity;
	if (this->planetC.planetCoordinate.Magnitude() > 400.0f)
		this->planetC.planetVelocity = -0.8 * this->planetC.planetVelocity;

	Vec3 accA, accB, accC;
	this->computeAcceleration(this->planetA, this->planetB, this->planetC, accA);
	this->computeAcceleration(this->planetB, this->planetC, this->planetA, accB);
	this->computeAcceleration(this->planetC, this->planetA, this->planetB, accC);
	//std::cout << this->planetA.planetVelocity.Magnitude() << std::endl;

	this->updatePlanet(this->planetA, deltaTime, accA);
	this->updatePlanet(this->planetB, deltaTime, accB);
	this->updatePlanet(this->planetC, deltaTime, accC);

	float planetAKineticEnergy = this->planetA.getKineticEnergy();
	float planetBKineticEnergy = this->planetB.getKineticEnergy();
	float planetCKineticEnergy = this->planetC.getKineticEnergy();

	computePotentialEnergy();

	float planetKineticEnergy = this->totalEnergy - (this->potentialEnergyAB + this->potentialEnergyAC + this->potentialEnergyBC);
	if (planetKineticEnergy < 0.0f)
		planetKineticEnergy = 0.0;

	float planetKineticEnergy1 = planetAKineticEnergy + planetBKineticEnergy + planetCKineticEnergy;
	float error = planetKineticEnergy / planetKineticEnergy1;
	if (error > 0.00001f)
	{
		planetAKineticEnergy *= error;
		planetBKineticEnergy *= error;
		planetCKineticEnergy *= error;
	}
	else 
	{
		planetAKineticEnergy = 1.0f;
		planetBKineticEnergy = 1.0f;
		planetCKineticEnergy = 1.0f;
	}
	//computeTotalEnergy();
	this->planetA.setVelocity(planetAKineticEnergy);
	this->planetB.setVelocity(planetBKineticEnergy);
	this->planetC.setVelocity(planetCKineticEnergy);
}

MyThreeBody::~MyThreeBody()
{
}
