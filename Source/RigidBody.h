#pragma once

#include "Mesh.hpp"
#include "Math.hpp"
#include "Component.hpp"
#include <string>
#include <unordered_map>
#include <vector>


class RigidBody : public Component
{
	glm::vec3 m_v3Position;
	glm::vec3 m_v3Scale;
	glm::vec3 m_v3Velocity;
	glm::vec3 m_v3Acceleration;

	glm::quat m_qOrientation;

	float m_fMass;
	float m_fMaxAcc;

	Mesh* m_pMesh = nullptr;

public:

	RigidBody( GameObject* gameObject );

	~RigidBody();

	void Update(void);

	void SetMaxAcc(float a_fMaxAcc);
	void SetMass(float a_fMass);
	void SetPosition(glm::vec3 a_v3Position);
	void SetVelocity(glm::vec3 a_v3Velocity);
	void SetAcceleration(glm::vec3 a_v3Acceleration);
	void AddForce(const glm::vec3& force);
};

