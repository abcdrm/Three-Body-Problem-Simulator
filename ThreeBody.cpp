#include "ThreeBody.h"
#include <iostream>

float myRand(float a, float b)
{
	//srand((int)time(0));
	const float r = (float)(::rand() / ((float)RAND_MAX + 1));
	return r * (b - a) + a;
}

void ThreeBody::randomPlanet(Planet& p)
{
	p.planetCoordinate.x = myRand(-30.0f, 30.0f);
	p.planetCoordinate.y = myRand(-30.0f, 30.0f);
	p.planetCoordinate.z = myRand(-30.0f, 30.0f);

	p.planetVelocity.x = myRand(-1, 1);
	p.planetVelocity.y = myRand(-1, 1);
	p.planetVelocity.z = myRand(-1, 1);
	p.planetVelocity.Normalize();
	p.planetVelocity *= myRand(0.4f, 2.0f);

	p.planetMess = myRand(4.0f, 8.0f);;
	p.planetRadius = powf(p.planetMess, 1.0f / 3.0f);
}

void ThreeBody::computeAcceleration(Planet& planet, Planet& p1, Planet& p2, Vec3& acc)
{
	Vec3 vector1 = p1.planetCoordinate - planet.planetCoordinate;
	float distance1Square = vector1.MagnitudeSquared();
	if (distance1Square < 0.00001f)
		distance1Square = 0.00001f;
	float distance1 = sqrt(distance1Square);
	float acc1 = this->gravitationalConstant*p1.planetMess / distance1Square;

	Vec3 vector2 = p2.planetCoordinate - planet.planetCoordinate;
	float distance2Square = vector2.MagnitudeSquared();
	if (distance2Square < 0.00001f)
		distance2Square = 0.00001f;
	float distance2 = sqrt(distance2Square);
	float acc2 = this->gravitationalConstant*p2.planetMess / distance2Square;

	float acc1X = acc1 * vector1.x / distance1;
	float acc1Y = acc1 * vector1.y / distance1;
	float acc1Z = acc1 * vector1.z / distance1;

	float acc2X = acc2 * vector2.x / distance2;
	float acc2Y = acc2 * vector2.y / distance2;
	float acc2Z = acc2 * vector2.z / distance2;

	acc.x = acc1X + acc2X;
	acc.y = acc1Y + acc2Y;
	acc.z = acc1Z + acc2Z;
}