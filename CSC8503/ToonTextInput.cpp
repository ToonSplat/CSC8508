#include "ToonTextInput.h"

ToonTextInput::ToonTextInput(Coordinates coordinates,
							 GameTechRenderer* renderer,
							 Vector2 windowSize,
							 std::function<PushdownState::PushdownResult(std::string, bool* showError)> doneButtonClosure, 
							 std::string errorMessage,
							 ToonVirtualKeyboard::KeyboardInputType inputType,
							 Vector4 focusColour,
							 Vector4 unfocusColour) : m_Coordinates(coordinates),
													  m_Renderer(renderer),
													  m_WindowSize(windowSize), 
													  m_doneButtonClosure(doneButtonClosure),
													  m_ErrorMessageText(errorMessage),
													  m_InputType(inputType), m_IsFocused(true),
													  m_FocusColour(focusColour),
													  m_UnfocusColour(unfocusColour),
													  m_InputText("")
{
	Coordinates keyboardCoordinates  = m_Coordinates;
	keyboardCoordinates.origin.y	+= keyboardCoordinates.size.y;
	m_VirtualKeyboard				 = new ToonVirtualKeyboard(keyboardCoordinates, m_WindowSize);
	m_ErrorMessageCoordinates		 = Vector2(50 - errorMessage.length(), coordinates.origin.y - 5.0f);
}

ToonTextInput::~ToonTextInput()
{
	delete m_VirtualKeyboard;
}

void ToonTextInput::UpdatePosition(Coordinates newCoordinates)
{
	m_Coordinates = newCoordinates;
}

void ToonTextInput::DrawUserInputText()
{
	m_InputText					 = m_VirtualKeyboard->GetUserInputText();
	Vector2 inputTextCoordinates = m_Coordinates.origin + Vector2(1.0f, (m_Coordinates.size.y / 2) + 1.0f);
	Debug::Print(m_InputText, inputTextCoordinates, Debug::WHITE);
}

std::string ToonTextInput::GetUserInputText()
{
	return m_InputText;
}

PushdownState::PushdownResult ToonTextInput::OnUpdate(float dt, PushdownState** newState)
{
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::ESCAPE)) { return PushdownResult::Pop; }
	if (m_IsFocused) { m_VirtualKeyboard->UpdateAndHandleInputEvents(); }

	m_Renderer->Update(dt);
	m_Renderer->Render();
	Debug::UpdateRenderables(dt);
	Debug::DrawQuad(m_Coordinates.origin, m_Coordinates.size, Debug::BLUE);
	DrawUserInputText();
	if (m_VirtualKeyboard->m_HasUserClickedDoneButton)
	{
		m_VirtualKeyboard->m_HasUserClickedDoneButton = false;
		return m_doneButtonClosure(m_VirtualKeyboard->GetUserInputText(), &m_ShouldShowErrorMessage);
	}
	if (m_ShouldShowErrorMessage) { Debug::Print(m_ErrorMessageText, m_ErrorMessageCoordinates, Debug::RED); }
	return PushdownState::PushdownResult::NoChange;
}

void ToonTextInput::OnAwake() {}
void ToonTextInput::OnSleep() {}