#include "Game.h"
#include "Resources.h"
#include "InputManager.h"
#include "Aventador.h"
#include "Path.h"

using namespace std;
using namespace glm;
using namespace physx;

namespace Game {
	list<shared_ptr<Entity> > entities;
	shared_ptr<Aventador> aventador0;
	shared_ptr<Aventador> aventador1;

	shared_ptr<HUDobj> hud;

	double spawnCoolDown = 3;
	double powerUpSpawnTime = Time::time += spawnCoolDown;
	double switchRange = 25.0;
	bool inSwtichRange = false;

	// we can customize this function as much as we want for now for debugging
	void init() {
		aventador0 = shared_ptr<Aventador>(new Aventador(0));

		aventador1 = shared_ptr<Aventador>(new Aventador(1));
		entities.push_back(aventador0);
		entities.push_back(shared_ptr<Path>(new Path(100, 1, 0)));	//the path that gets drawn under the car
		entities.push_back(aventador1);


		//entities.push_back(unique_ptr<Plane>(new Plane));
	}

	void update() {
		glfwPollEvents();
		double dist = getDist();

		for (auto it = entities.begin(); it != entities.end(); it++) {
			if (it->get()->alive) {
				it->get()->update(mat4(1));
			}
			else {
				it = entities.erase(it);
			}
		}
		//adding more power ups into the scene
		if (Time::time > powerUpSpawnTime) {
			powerUpSpawnTime += spawnCoolDown;
			entities.push_back(shared_ptr<Entity>(new PowerUpManager()));
		}
		//check the distance between the aventators
		if (dist < switchRange) {
			aventador1->setForce(100);
			inSwtichRange = true;
		}
		else if (dist > switchRange && inSwtichRange) { //change the speed/role only once
			aventador1->setForce(45);
			switchRole();	//change the roles of the aventators
			inSwtichRange = !inSwtichRange;
		}

	}

	void switchRole() {
		aventador0->changeRole();
		aventador1->changeRole();
		std::cout << "role switch\n";
	}

	double getDist() {
		PxTransform pos0 = aventador0->getActor()->getGlobalPose();
		PxTransform pos1 = aventador1->getActor()->getGlobalPose();
		return sqrt(pow((pos0.p.x - pos1.p.x), 2) + pow((pos0.p.z - pos1.p.z), 2));
	}
}

namespace Time {
	double time, prevTime, deltaTime, fpsTime;
	int fps, tfps;

	void init() {
		time = glfwGetTime();
		prevTime = 0;
		deltaTime = 0;
		fps = tfps = 0;
		fpsTime = 0;
	}

	void update() {
		time = glfwGetTime();
		deltaTime = time - prevTime;
		prevTime = time;
		if (time > fpsTime + 1) {
			fps = tfps;
			if (PRINT_FPS) {
				cout << "fps: " << fps << endl;
			}
			tfps = 0;
			fpsTime = time;
		}
		else {
			tfps++;
		}
	}
}