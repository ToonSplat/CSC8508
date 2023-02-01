#pragma once
#include "GameObject.h"

using namespace NCL;
using namespace CSC8503;

class HitSphere : public GameObject {
public:
	HitSphere();
	~HitSphere();

	void OnCollisionBegin(GameObject* otherObject);
};