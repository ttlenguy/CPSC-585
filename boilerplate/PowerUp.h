#pragma once

#include "Entity.h"
#include "Resources.h"
#include "Graphics.h"
#include "FilterGroup.h"

class PowerUp :public Entity {
	physx::PxRigidDynamic *actor;
	int* id;

public:
	PowerUp(int id);
	void update(glm::mat4 parentTransform)override;
	void render(glm::mat4 parentTransform)override;
	physx::PxRigidDynamic *const getActor();
	PxVec3 PowerUp::getRandLocation();

	/*
	bool operator==(const PowerUp& p) const {
		if (id < p.id) {
			return false;
		}
		else if (id > p.id) {
			return false;
		}
		return true;
	}
	*/
};