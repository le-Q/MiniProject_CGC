#pragma once

#include<iostream>

#include<glew.h>
#include<glfw3.h>

#include<glm.hpp>
#include<vec3.hpp>
#include<mat4x4.hpp>
#include<gtc\matrix_transform.hpp>

enum direction { FORWARD = 0, BACKWARD, LEFT, RIGHT };

class Camera
{
private:
	glm::mat4 ViewMatrix;

	GLfloat movementSpeed;
	GLfloat sensitivity;

	glm::vec3 cameraUp;
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;

	GLfloat pitch;
	GLfloat yaw;

	void updateCameraVectors()
	{
		this->front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		this->front.y = sin(glm::radians(this->pitch));
		this->front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

		this->front = glm::normalize(this->front);
		this->right = glm::normalize(glm::cross(this->front, this->cameraUp));
		this->up = glm::normalize(glm::cross(this->right, this->front));
	}

public:
	Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 cameraUp)
	{
		this->ViewMatrix = glm::mat4(1.f);

		this->movementSpeed = 3.f;
		this->sensitivity = 5.f;

		this->cameraUp = cameraUp;
		this->position = position;
		this->right = glm::vec3(0.f);
		this->up = cameraUp;

		this->pitch = 0.f;
		this->yaw = -90.f;

		this->updateCameraVectors();
	}

	~Camera() {}

	//Accessors
	const glm::mat4 getViewMatrix()
	{
		this->updateCameraVectors();

		this->ViewMatrix = glm::lookAt(this->position, this->position + this->front, this->up);

		return this->ViewMatrix;
	}

	const glm::vec3 getPosition() const
	{
		return this->position;
	}

	// Functions
	void move(const float& deltaTime, const int direction)
	{
		// Update position vector
		switch (direction)
		{
		case FORWARD:
			this->position += this->front * this->movementSpeed * deltaTime;
			break;
		case BACKWARD:
			this->position -= this->front * this->movementSpeed * deltaTime;
			break;
		case LEFT:
			this->position -= this->right * this->movementSpeed * deltaTime;
			break;
		case RIGHT:
			this->position += this->right * this->movementSpeed * deltaTime;
			break;
		default:
			break;
		}
	}

	void updateMouseInput(const float& deltaTime, const double& offsetX, const double& offsetY)
	{
		// Update pitch yaw and roll
		this->pitch += static_cast<GLfloat>(offsetY) * this->sensitivity * deltaTime;
		this->yaw += static_cast<GLfloat>(offsetX) * this->sensitivity * deltaTime;

		if (this->pitch > 80.f)
			this->pitch = 80.f;
		else if (this->pitch < -80.f)
			this->pitch = -80.f;

		if (this->yaw > 360.f || this->yaw < -360.f)
			this->yaw = 0.f;
	}

	void updateInput(const float& deltaTime, const int direction, const double& offsetX, const double& offsetY)
	{
		this->updateMouseInput(deltaTime, offsetX, offsetY);
	}

};