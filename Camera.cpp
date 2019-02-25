#include "Camera.h"

Camera::~Camera()
{
}

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch)
	: cameraDirection(glm::vec3(0.0f, 0.0f, -1.0f)), moveSpeed(SPEED), mouseSensity(SENSITY), zoom(ZOOM)
{
	this->cameraPosition = position;
	this->up = up;
	this->yaw = yaw;
	this->pitch = pitch;
	this->updateCameraVectors();
}

Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch)
	: cameraDirection(glm::vec3(0.0f, 0.0f, -1.0f)), moveSpeed(SPEED), mouseSensity(SENSITY), zoom(ZOOM)
{
	this->cameraPosition = glm::vec3(posX, posY, posZ);
	this->up = glm::vec3(upX, upY, upZ);
	this->yaw = yaw;
	this->pitch = pitch;
	this->updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(this->cameraPosition, this->cameraPosition + this->cameraDirection, this->cameraUp);
}

void Camera::processKeyboard(cameraShift direction, GLfloat deltaTime)
{
	GLfloat velocity = this->moveSpeed * deltaTime;
	if (direction == FORWARD)
		this->cameraPosition += this->cameraDirection * velocity;
	if (direction == BACKWARD)
		this->cameraPosition -= this->cameraDirection * velocity;
	if (direction == LEFT)
		this->cameraPosition -= this->cameraRight * velocity;
	if (direction == RIGHT)
		this->cameraPosition += this->cameraRight * velocity;
}

void Camera::processMouse(GLfloat xoffset, GLfloat yoffset, GLboolean constrainpitch)
{
	xoffset *= this->mouseSensity;
	yoffset *= this->mouseSensity;

	this->yaw += xoffset;
	this->pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainpitch)
	{
		if (this->pitch > 89.0f)
			this->pitch = 89.0f;
		if (this->pitch < -89.0f)
			this->pitch = -89.0f;
	}

	// Update cameraDirection, cameraRight and Up Vectors using the updated Eular angles
	this->updateCameraVectors();
}

/*
void Camera::ProcessMouseScroll(GLfloat yoffset)
{
	if (this->zoom >= 1.0f && this->zoom <= 45.0f)
		this->zoom -= yoffset;
	if (this->zoom <= 1.0f)
		this->zoom = 1.0f;
	if (this->zoom >= 45.0f)
		this->zoom = 45.0f;
}
*/

void Camera::updateCameraVectors()
{
	// Calculate the new cameraDirection vector
	glm::vec3 direction;
	direction.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	direction.y = sin(glm::radians(this->pitch));
	direction.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	this->cameraDirection = glm::normalize(direction);
	// Also re-calculate the cameraRight and Up vector
	this->cameraRight = glm::normalize(glm::cross(this->cameraDirection, this->up));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	this->cameraUp = glm::normalize(glm::cross(this->cameraRight, this->cameraDirection));
}
