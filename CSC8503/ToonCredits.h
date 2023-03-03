#pragma once

#include <vector>
#include "GameTechRenderer.h"
#include "PushdownState.h"
#include "ToonFileHandling.h"
#include "Assets.h"

using namespace NCL;
using namespace CSC8503;

class ToonCredits : public PushdownState
{
	private:
		std::string				 m_FetchedText;
		std::vector<std::string> m_DisplayTextVector;
		GameTechRenderer*		 m_Renderer;
		ToonGameWorld*			 m_World;
		Window*					 m_Window;
		ToonFileHandling*		 m_FileHandler;
		const std::string		 m_FileName = NCL::Assets::DATADIR + "Credits.txt";
		const int				 m_MaxCharactersPerLine = 50;

	public:
		ToonCredits(GameTechRenderer* renderer, ToonGameWorld* world, Window* win);
		~ToonCredits();
		PushdownResult OnUpdate(float dt, PushdownState** newState) override;
		void OnAwake() override;
		void OnSleep() override;

	private:
		void FetchData();
		void ParseData();
		void DisplayText();
};