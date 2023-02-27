#include "ToonNetworkObject.h"
#include "ToonGameObject.h"
using namespace NCL;
using namespace CSC8503;

ToonNetworkObject::ToonNetworkObject(ToonGameObject* o, int id, int startState) : object(o) {
	o->SetNetworkObject(this);
	lastFullState.stateID = startState;
	networkID = id;
}

ToonNetworkObject::~ToonNetworkObject() {
}

bool ToonNetworkObject::ReadPacket(GamePacket& p) {
	if (p.type == Delta_State)
		return ReadDeltaPacket((DeltaPacket&)p);
	if (p.type == Full_State)
		return ReadFullPacket((FullPacket&)p);
	return false; //this isn't a packet we care about!
}

bool ToonNetworkObject::WritePacket(GamePacket** p, bool deltaFrame, int stateID) {
	UpdateLastFullState();
	if (deltaFrame) {
		if (!WriteDeltaPacket(p, stateID)) {
			return WriteFullPacket(p);
		}
		return true;
	}
	else
		return WriteFullPacket(p);
}

void ToonNetworkObject::UpdateLastFullState() {
	lastFullState.stateID++;
	reactphysics3d::Vector3 position = object->GetRigidbody()->getTransform().getPosition();
	lastFullState.position[0] = int(position.x * 1000);
	lastFullState.position[1] = int(position.y * 1000);
	lastFullState.position[2] = int(position.z * 1000);

	reactphysics3d::Quaternion orientation = object->GetRigidbody()->getTransform().getOrientation();
	lastFullState.orientation[0] = short(orientation.x * 1000);
	lastFullState.orientation[1] = short(orientation.y * 1000);
	lastFullState.orientation[2] = short(orientation.z * 1000);
	lastFullState.orientation[3] = short(orientation.w * 1000);

	reactphysics3d::Vector3 linVelocity = object->GetRigidbody()->getLinearVelocity();
	lastFullState.linVelocity[0] = int(linVelocity.x * 1000);
	lastFullState.linVelocity[1] = int(linVelocity.y * 1000);
	lastFullState.linVelocity[2] = int(linVelocity.z * 1000);

	reactphysics3d::Vector3 angVelocity = object->GetRigidbody()->getAngularVelocity();
	lastFullState.angVelocity[0] = int(angVelocity.x * 1000);
	lastFullState.angVelocity[1] = int(angVelocity.y * 1000);
	lastFullState.angVelocity[2] = int(angVelocity.z * 1000);
}

//Client objects recieve these packets
bool ToonNetworkObject::ReadDeltaPacket(DeltaPacket& p) {
	ToonObjectState state;
	if (!GetNetworkState(p.fullID, state)) {
		return false; // Can't delta if no existing state
	}
	UpdateStateHistory(p.fullID);

	reactphysics3d::Vector3 fullPos = 
		reactphysics3d::Vector3(state.position[0] / 1000.0f, state.position[1] / 1000.0f, state.position[2] / 1000.0f);

	reactphysics3d::Quaternion fullOrientation =
		reactphysics3d::Quaternion(state.orientation[0] / 1000.0f, state.orientation[1] / 1000.0f, state.orientation[2] / 1000.0f, state.orientation[3] / 1000.0f);

	fullPos.x += (p.pos[0] / 1000.0f);
	fullPos.y += (p.pos[1] / 1000.0f);
	fullPos.z += (p.pos[2] / 1000.0f);

	fullOrientation.x += (p.orientation[0] / 1000.0f);
	fullOrientation.y += (p.orientation[1] / 1000.0f);
	fullOrientation.z += (p.orientation[2] / 1000.0f);
	fullOrientation.w += (p.orientation[3] / 1000.0f);

	object->SetPosition(fullPos);
	object->SetOrientation(fullOrientation);

	reactphysics3d::Vector3 linVel =
		reactphysics3d::Vector3(state.linVelocity[0] / 1000.0f, state.linVelocity[1] / 1000.0f, state.linVelocity[2] / 1000.0f);

	reactphysics3d::Vector3 angVel =
		reactphysics3d::Vector3(state.angVelocity[0] / 1000.0f, state.angVelocity[1] / 1000.0f, state.angVelocity[2] / 1000.0f);

	linVel.x += (p.linVel[0] / 1000.0f);
	linVel.y += (p.linVel[1] / 1000.0f);
	linVel.z += (p.linVel[2] / 1000.0f);

	angVel.x += (p.angVel[0] / 1000.0f);
	angVel.y += (p.angVel[1] / 1000.0f);
	angVel.z += (p.angVel[2] / 1000.0f);

	object->GetRigidbody()->setLinearVelocity(linVel);
	object->GetRigidbody()->setAngularVelocity(angVel);

	return true;
}

bool ToonNetworkObject::ReadFullPacket(FullPacket& p) {
	if (p.fullState.stateID < lastFullState.stateID)
		return false; // Received old packet
	lastFullState = p.fullState;

	object->SetPosition(
		reactphysics3d::Vector3(lastFullState.position[0] / 1000.0f, lastFullState.position[1] / 1000.0f, lastFullState.position[2] / 1000.0f));
	object->SetOrientation(
		reactphysics3d::Quaternion(lastFullState.orientation[0] / 1000.0f, lastFullState.orientation[1] / 1000.0f,
			lastFullState.orientation[2] / 1000.0f, lastFullState.orientation[3] / 1000.0f));
	object->GetRigidbody()->setLinearVelocity(
		reactphysics3d::Vector3(lastFullState.linVelocity[0] / 1000.0f, lastFullState.linVelocity[1] / 1000.0f, lastFullState.linVelocity[2] / 1000.0f));
	object->GetRigidbody()->setAngularVelocity(
		reactphysics3d::Vector3(lastFullState.angVelocity[0] / 1000.0f, lastFullState.angVelocity[1] / 1000.0f, lastFullState.angVelocity[2] / 1000.0f));

	stateHistory.emplace_back(lastFullState);

	return true;
}

bool ToonNetworkObject::WriteDeltaPacket(GamePacket** p, int stateID) {
	DeltaPacket* dp = new DeltaPacket();
	ToonObjectState state;
	if (!GetNetworkState(stateID, state)) {
		return false; // Can't delta if no existing state
	}
	dp->fullID = stateID;
	dp->objectID = networkID;

	reactphysics3d::Vector3 currentPos = 
		reactphysics3d::Vector3(reactphysics3d::decimal(lastFullState.position[0]), 
			reactphysics3d::decimal(lastFullState.position[1]), reactphysics3d::decimal(lastFullState.position[2]));

	reactphysics3d::Quaternion currentOrientation =
		reactphysics3d::Quaternion(lastFullState.orientation[0] / 1000.0f, lastFullState.orientation[1] / 1000.0f,
			lastFullState.orientation[2] / 1000.0f, lastFullState.orientation[3] / 1000.0f);

	currentPos.x -= state.position[0];
	currentPos.y -= state.position[1];
	currentPos.z -= state.position[2];
	currentOrientation.x -= state.orientation[0] / 1000.0f;
	currentOrientation.y -= state.orientation[1] / 1000.0f;
	currentOrientation.z -= state.orientation[2] / 1000.0f;
	currentOrientation.w -= state.orientation[3] / 1000.0f;

	dp->pos[0] = (short)(currentPos.x);
	dp->pos[1] = (short)(currentPos.y);
	dp->pos[2] = (short)(currentPos.z);

	dp->orientation[0] = (short)(currentOrientation.x * 1000.0f);
	dp->orientation[1] = (short)(currentOrientation.y * 1000.0f);
	dp->orientation[2] = (short)(currentOrientation.z * 1000.0f);
	dp->orientation[3] = (short)(currentOrientation.w * 1000.0f);

	reactphysics3d::Vector3 linVel =
		reactphysics3d::Vector3(lastFullState.linVelocity[0] / 1000.0f, lastFullState.linVelocity[1] / 1000.0f, lastFullState.linVelocity[2] / 1000.0f);

	reactphysics3d::Vector3 angVel =
		reactphysics3d::Vector3(lastFullState.angVelocity[0] / 1000.0f, lastFullState.angVelocity[1] / 1000.0f, lastFullState.angVelocity[2] / 1000.0f);

	linVel.x -= state.linVelocity[0] / 1000.0f;
	linVel.y -= state.linVelocity[1] / 1000.0f;
	linVel.z -= state.linVelocity[2] / 1000.0f;
	angVel.x -= state.angVelocity[0] / 1000.0f;
	angVel.y -= state.angVelocity[1] / 1000.0f;
	angVel.z -= state.angVelocity[2] / 1000.0f;

	dp->linVel[0] = (short)(linVel.x * 1000.0f);
	dp->linVel[1] = (short)(linVel.y * 1000.0f);
	dp->linVel[2] = (short)(linVel.z * 1000.0f);
	dp->angVel[0] = (short)(angVel.x * 1000.0f);
	dp->angVel[1] = (short)(angVel.y * 1000.0f);
	dp->angVel[2] = (short)(angVel.z * 1000.0f);

	stateHistory.emplace_back(lastFullState);

	*p = dp;

	return true;
}

bool ToonNetworkObject::WriteFullPacket(GamePacket** p) {
	FullPacket* fp = new FullPacket();

	fp->objectID = networkID;
	fp->fullState = lastFullState;

	*p = fp;

	stateHistory.emplace_back(fp->fullState);

	return true;
}

ToonObjectState& ToonNetworkObject::GetLatestNetworkState() {
	return lastFullState;
}

bool ToonNetworkObject::GetNetworkState(int stateID, ToonObjectState& state) {
	for (auto i = stateHistory.begin(); i < stateHistory.end(); i++) {
		if ((*i).stateID == stateID) {
			state = (*i);
			return true;
		}
	}
	return false;
}

void ToonNetworkObject::UpdateStateHistory(int minID) {
	for (auto i = stateHistory.begin(); i < stateHistory.end();) {
		if ((*i).stateID < minID)
			i = stateHistory.erase(i);
		else
			i++;
	}
}