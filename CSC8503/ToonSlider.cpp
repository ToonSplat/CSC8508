#include "ToonSlider.h"

ToonSlider::ToonSlider(Coordinates coordinates, int sliderLevels, Vector2 windowSize, float textWidth, float sliderHeight, float sliderHeadSize) : m_coordinates(coordinates), m_SliderLevels(sliderLevels), m_WindowSize(windowSize), m_IsActive(false), m_SliderHeadSize(sliderHeadSize)
{
	sliderHeight = 100.0f / m_WindowSize.y;	//Updating height to 1 pixel. TODO: - Change Logic for sliderHeight(bar height)
	AssignCoordinates(textWidth, sliderHeight, sliderHeadSize);
}

ToonSlider::~ToonSlider()
{
}

void ToonSlider::Update(float dt)
{
	DrawSlider();
}

void ToonSlider::DrawSlider()
{
	DrawBar();
	DrawHead();
	Debug::Print(std::to_string(m_CurrentLevel), m_SliderTextCoordinates.origin, Debug::WHITE);
}

void ToonSlider::DrawBar()
{
	Debug::DrawFilledQuad(m_SliderBarCoordinates.origin, m_SliderBarCoordinates.size, /*m_SliderBarCoordinates.size.y*/100.0f / m_WindowSize.y, m_SliderUncoveredColour);
	Debug::DrawFilledQuad(m_SliderBarCoordinates.origin, Vector2(m_SliderLevelCoordinatesMap[m_CurrentLevel].origin.x - m_SliderBarCoordinates.origin.x, m_SliderBarCoordinates.size.y), 100.0f / m_WindowSize.y, Debug::GREEN);
}

void ToonSlider::DrawHead()
{
	Coordinates headCoord = GetHeadCoordinates(m_CurrentLevel);
	m_HeadCoordinates	  = Coordinates(Vector2(headCoord.origin.x, headCoord.origin.y - (m_SliderHeadSize / 2)), Vector2(m_SliderHeadSize, m_SliderHeadSize));
	Debug::DrawFilledQuad(m_HeadCoordinates.origin, m_HeadCoordinates.size, 100.0f / m_WindowSize.y, Debug::WHITE);
}

void ToonSlider::AssignCoordinates(float textWidth, float sliderHeight, float sliderHeadSize)
{
	m_ShouldDisplayLevels	= textWidth > 0.0f;
	m_SliderTextCoordinates = Coordinates(Vector2(m_coordinates.origin.x + m_coordinates.size.x - textWidth + 2.0f, m_coordinates.origin.y + 1.0f), Vector2(textWidth, m_coordinates.size.y));
	m_SliderBarCoordinates  = Coordinates(m_coordinates.origin, Vector2(m_coordinates.size.x - textWidth, sliderHeight));
	//Head
	PopulateSliderCoordinatesMap();
}

void ToonSlider::PopulateSliderCoordinatesMap()
{
	float levelWidth = m_SliderBarCoordinates.size.x / m_SliderLevels;
	m_SliderHeadSize = m_SliderHeadSize > levelWidth ? levelWidth : m_SliderHeadSize;
	for (int i = 0; i < m_SliderLevels; i++)
	{
		float offset = levelWidth * i;
		m_SliderLevelCoordinatesMap[i] = Coordinates(Vector2(m_SliderBarCoordinates.origin.x + offset, m_SliderBarCoordinates.origin.y), Vector2(levelWidth, m_SliderBarCoordinates.size.y));
	}
}

Coordinates ToonSlider::GetHeadCoordinates(int level)
{
	if (level < 0 && level >= m_SliderLevels) { return INVALID_COORDINATE; }
	return m_SliderLevelCoordinatesMap[level];
}


int ToonSlider::GetHeadLevel(Vector2 headPosition)
{
	int index = 0;
	if (headPosition.x < m_SliderLevelCoordinatesMap[0].origin.x) { return 0; }
	else if (headPosition.x >= m_SliderLevelCoordinatesMap[m_SliderLevels - 1].origin.x + m_SliderLevelCoordinatesMap[m_SliderLevels - 1].size.x) { return m_SliderLevels - 1; }
	for (auto& sliderLevelData : m_SliderLevelCoordinatesMap)
	{
		float sliderLevelEndXCoord = sliderLevelData.second.origin.x + sliderLevelData.second.size.x;
		if (sliderLevelData.second.origin.x <= headPosition.x && sliderLevelEndXCoord > headPosition.x)
		{
			return  index;
		}
		index++;
	}
	return -1;
}

int ToonSlider::GetHeadLevelUsingMousePosition(Vector2 mousePosition)
{
	//Edge cases
	if (mousePosition.x < m_SliderLevelCoordinatesMap[0].origin.x)
	{
		return 0; }
	else if (mousePosition.x >= m_SliderLevelCoordinatesMap[m_SliderLevels - 1].origin.x + m_SliderLevelCoordinatesMap[m_SliderLevels - 1].size.x)
	{ 
		return m_SliderLevels - 1; }

	//Calculating for inner cases
	float levelWidth			= m_SliderBarCoordinates.size.x / m_SliderLevels;
	float positionWithoutOffset = mousePosition.x - m_SliderBarCoordinates.origin.x;
	return (positionWithoutOffset / levelWidth);
}

void ToonSlider::HandleKeyboardAndMouseEvents()
{
	if (InputManager::GetInstance().GetInputs()[1]->IsPushingRight()) { m_CurrentLevel = std::min(++m_CurrentLevel, m_SliderLevels - 1); }
	else if (InputManager::GetInstance().GetInputs()[1]->IsPushingLeft()) { m_CurrentLevel = std::max(--m_CurrentLevel, 0); }
	
	if (InputManager::GetInstance().GetInputs()[1]->IsShooting())
	{
		Vector2 mousePosition			  = InputManager::GetInstance().GetInputs()[1]->GetMousePosition();
		float	y						  = ((mousePosition.y / m_WindowSize.y) * 100);
		float	x						  = ((mousePosition.x / m_WindowSize.x) * 100);
		Vector2 mousePositionWithinBounds = Vector2(x, y);
		m_CurrentLevel					  = GetHeadLevelUsingMousePosition(mousePositionWithinBounds);//GetHeadLevel(mousePositionWithinBounds);
	}
}