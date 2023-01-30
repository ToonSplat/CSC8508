#include "Player.h"

using namespace NCL;
using namespace CSC8503;

Player::Player() {
	team = nullptr;
}

Player::Player(Team* chosenTeam) {
	team = chosenTeam;
}

Player::~Player() {

}

void Player::Update(Matrix4 inverseView, float yaw, float pitch, float dt) {
	return;
}

void Player::Shoot() {
	return;
}