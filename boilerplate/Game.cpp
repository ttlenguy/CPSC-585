#include "Game.h"
#include "Resources.h"
#include "InputManager.h"
#include "Aventador.h"
#include "Path.h"
#include "PowerUpList.h"
#include "Skybox.h"
#include "Menu.h"

#include "GameExt.h"

using namespace std;
using namespace glm;
using namespace physx;

namespace Game {
	list<shared_ptr<Entity> > entities;
	list<shared_ptr<Entity> > startGameEntities;
	shared_ptr<Aventador> aventador0;
	shared_ptr<Aventador> aventador1;

	shared_ptr<Path> path;
	shared_ptr<HUDobj> hud;
	vector<PowerUp*> aiPowerUps;

	bool alive;
	bool vs_ai;

	double spawnCoolDown = 10;
	double powerUpSpawnTime = Time::time += spawnCoolDown;

	float impulse = 100;
	double switchRange = 10.0;
	bool inSwtichRange = false;
	bool isGameOver = false;

	// we can customize this function as much as we want for now for debugging
	void init() {
		startMainMenu();
	}

	void update() {
		glfwPollEvents();
		addPowerUp();
		for (auto it = entities.begin(); it != entities.end(); it++) {
			if (it->get()->alive) {
				it->get()->update(mat4(1));
			}
			else {
				it = entities.erase(it);
			}
			if (!alive)break;
		}

		if (!alive) {
			
		}

		alive = 1;

		if (PRINT_ENTITIES) {
			cout << entities.size() << endl;
		}

		//This is where a restart function would go
		//currently doing something wrong as restarting must not actually delete as the program slows down after each restart
		if ((controller1.GetButtonPressed(13)) || (Keyboard::keyPressed(GLFW_KEY_ESCAPE))) {
			startMainMenu();
		}
		addPowerUp();
		checkForSwap();

	}

	//adding more power ups into the scene
	void addPowerUp() {
		if (Time::time > powerUpSpawnTime) {
			powerUpSpawnTime += spawnCoolDown;

			entities.push_back(unique_ptr<Entity>(new PowerUp()));
			entities.push_back(unique_ptr<Entity>(new PowerUp()));
			entities.push_back(unique_ptr<Entity>(new PowerUp()));
			entities.push_back(unique_ptr<Entity>(new PowerUp()));

		}
	}

	//swaps the roles if they are withing range
	void checkForSwap() {
		double dist = getDist();
		if (dist < switchRange && !inSwtichRange) {
			if (!getBack())return;
			PxRigidBodyExt::addLocalForceAtLocalPos(*getBack()->actor,
				PxVec3(0, 0, impulse), PxVec3(0, 0, 0), PxForceMode::eIMPULSE);
			switchRole();
			inSwtichRange = true;
		}
		else if (dist > switchRange && inSwtichRange) {
			inSwtichRange = false;

		}
	}

	void switchRole() {
		aventador0->changeRole();
		aventador1->changeRole();
	}

	/*
	//this is the start screen loop objects or place an image
	int startScreen() {
		//initialization of whatever we want in here for now just a print statement
		//currently can only go to the main game loop because of how i set up loops in the main file

		cout << "In Start Screen" << endl;		

		if ((controller1.GetButtonPressed(12)) || (Keyboard::keyPressed(GLFW_KEY_ENTER))) {
			cout << "ENTERED GAME LOOP" << endl;
			entities.clear();
			init();
			return 1;
		}
	}

	//same as start screen but just after the race is over
	void endScreen() {
		
	}*/

	double getDist() {
		if (!aventador0)return -1;
		if (!aventador1)return -1;
		PxTransform pos0 = aventador0->actor->getGlobalPose();
		PxTransform pos1 = aventador1->actor->getGlobalPose();
		return sqrt(pow((pos0.p.x - pos1.p.x), 2) + pow((pos0.p.z - pos1.p.z), 2));
	}

	bool didSwitchOccur() {
		return inSwtichRange;
	}

	Aventador* getFront() {
		return (aventador0->isFront()) ? aventador0.get() : aventador1.get();
	}

	Aventador* getBack() {
		return (aventador0->isFront()) ? aventador1.get() : aventador0.get();
	}

	void setGameOverFlag(bool flag) {
		isGameOver = flag;
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