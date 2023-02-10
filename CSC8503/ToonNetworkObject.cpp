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
	lastFullState.position = object->GetRigidbody()->getTransform().getPosition();
	lastFullState.orientation = object->GetRigidbody()->getTransform().getOrientation();
	lastFullState.linVelocity = object->GetRigidbody()->getLinearVelocity();
	lastFullState.angVelocity = object->GetRigidbody()->getAngularVelocity();
}

//Client objects recieve these packets
bool ToonNetworkObject::ReadDeltaPacket(DeltaPacket& p) {
	ToonObjectState state;
	if (!GetNetworkState(p.fullID, state)) {
		return false; // Can't delta if no existing state
	}
	UpdateStateHistory(p.fullID);

	reactphysics3d::Vector3 fullPos = state.position;
	reactphysics3d::Quaternion fullOrientation = state.orientation;

	fullPos.x += ((float)p.pos[0] / 10.0f);
	fullPos.y += ((float)p.pos[1] / 10.0f);
	fullPos.z += ((float)p.pos[2] / 10.0f);

	fullOrientation.x += ((float)p.orientation[0]) / 127.0f;
	fullOrientation.y += ((float)p.orientation[1]) / 127.0f;
	fullOrientation.z += ((float)p.orientation[2]) / 127.0f;
	fullOrientation.w += ((float)p.orientation[3]) / 127.0f;

	object->SetPosition(fullPos);
	object->SetOrientation(fullOrientation);

	return true;
}

bool ToonNetworkObject::ReadFullPacket(FullPacket& p) {
	if (p.fullState.stateID < lastFullState.stateID)
		return false; // Received old packet
	lastFullState = p.fullState;

	object->SetPosition(lastFullState.position);
	object->SetOrientation(lastFullState.orientation);
	object->GetRigidbody()->setLinearVelocity(lastFullState.linVelocity);
	object->GetRigidbody()->setAngularVelocity(lastFullState.angVelocity);

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

	reactphysics3d::Vector3 currentPos = lastFullState.position;
	reactphysics3d::Quaternion currentOrientation = lastFullState.orientation;
	currentPos -= state.position;
	currentOrientation -= state.orientation;

	dp->pos[0] = (char)(currentPos.x * 10.0f);
	dp->pos[1] = (char)(currentPos.y * 10.0f);
	dp->pos[2] = (char)(currentPos.z * 10.0f);

	dp->orientation[0] = (char)(currentOrientation.x * 127.0f);
	dp->orientation[1] = (char)(currentOrientation.y * 127.0f);
	dp->orientation[2] = (char)(currentOrientation.z * 127.0f);
	dp->orientation[3] = (char)(currentOrientation.w * 127.0f);

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