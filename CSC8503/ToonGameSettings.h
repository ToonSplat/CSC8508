#pragma once
#include "PushdownMachine.h"
#include "PushdownState.h"
#include "GameTechRenderer.h"

using namespace NCL;
using namespace CSC8503;

class ToonGameSettings : public PushdownState
{
	private:
		GameTechRenderer* m_Renderer;
		ToonGameWorld*	  m_World;
		Window*			  m_Window = NULL;

	public:
		ToonGameSettings(GameTechRenderer* renderer, ToonGameWorld* world, Window* win);
		~ToonGameSettings();
		PushdownResult OnUpdate(float dt, PushdownState** newState) override;
		void OnAwake() override;
		void OnSleep() override;

	private:
		void DrawScreen();
};