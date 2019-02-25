#pragma once
#include <math.h>
#include "Vec3.h"

struct Planet
{	
	Vec3 planetCoordinate, planetVelocity;
	float planetMess, planetRadius;

	Planet() {
		this->planetMess = 0.0f;
		this->planetRadius = 0.0f;
	}

	float getKineticEnergy()
	{
		float velocitySquare = this->planetVelocity.MagnitudeSquared();
		return 0.5f*this->planetMess*velocitySquare;
	}

	void setVelocity(float energy)
	{
		float v = sqrt(2 * energy / this->planetMess);
		float w = this->planetVelocity.Magnitude();
		if (w < 0.00001f)
		{
			this->planetVelocity.x = v;
		}
		else
		{
			this->planetVelocity *= v / w;
		}
	}

	void setVelocity(Vec3 c)
	{
		this->planetVelocity = c;
	}

	void setMess(float mess)
	{
		this->planetMess = mess;
		this->planetRadius = powf(mess, 1.0f / 3.0f);
	}
};