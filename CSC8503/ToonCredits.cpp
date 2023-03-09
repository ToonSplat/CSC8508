#include "ToonCredits.h"

ToonCredits::ToonCredits(GameTechRenderer* renderer, ToonGameWorld* world, Window* win)
{
	m_Renderer = renderer;
	m_World	   = world;
	m_Window   = win;
	m_FileHandler = new ToonFileHandling(m_FileName);
	FetchData();
}

ToonCredits::~ToonCredits()
{
	delete m_FileHandler;
}

PushdownState::PushdownResult ToonCredits::OnUpdate(float dt, PushdownState** newState)
{
	m_Renderer->SetWorld(m_World);
	m_Renderer->Update(dt);
	m_Renderer->Render();
	Debug::UpdateRenderables(dt);
	DisplayText();
	if (InputManager::GetInstance().GetInputs()[1]->IsBack()) { return PushdownResult::Pop; }
	return PushdownResult::NoChange;
}

void ToonCredits::OnAwake()
{
}

void ToonCredits::OnSleep()
{
}

void ToonCredits::FetchData()
{
	m_FileHandler->ReadDataIn(m_FetchedText);
	if (!m_FetchedText.empty()) { ParseData(); }
}

void ToonCredits::ParseData()
{
	int characterCount = 0;
	std::string singleLineString = "";
	for (char character : m_FetchedText)
	{
		if (characterCount && (!(characterCount % m_MaxCharactersPerLine) || character == '\n'))
		{
			characterCount = 0;
			m_DisplayTextVector.push_back(singleLineString);
			singleLineString = "";
		}
		singleLineString += character;
		characterCount++;
	}
}

void ToonCredits::DisplayText()
{
	float initialVerticalPosition = 10.0f;
	for (std::string str : m_DisplayTextVector)
	{
		Debug::Print(str, Vector2(5.0f, initialVerticalPosition), Debug::WHITE);
		initialVerticalPosition += 10.0f;
	}
}