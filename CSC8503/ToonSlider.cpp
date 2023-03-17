#include "ToonSlider.h"

ToonSlider::ToonSlider(Coordinates coordinates, int sliderLevels, Vector2 windowSize, float textWidth, float sliderHeight, float sliderHeadSize) : m_coordinates(coordinates), m_SliderLevels(sliderLevels), m_WindowSize(windowSize), m_IsActive(false), m_SliderHeadSize(sliderHeadSize)
{
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
}

void ToonSlider::DrawBar()
{
	Debug::DrawFilledQuad(m_SliderBarCoordinates.origin, m_SliderBarCoordinates.size, m_SliderBarCoordinates.size.y, m_SliderUncoveredColour);
}

void ToonSlider::DrawHead()
{
	Debug::DrawFilledQuad(m_HeadCoordinates.origin, m_HeadCoordinates.size, 100.0f / m_WindowSize.y, Debug::WHITE);
}

void ToonSlider::AssignCoordinates(float textWidth, float sliderHeight, float sliderHeadSize)
{
	m_ShouldDisplayLevels	= textWidth > 0.0f;
	m_SliderTextCoordinates = Coordinates(Vector2(m_coordinates.origin.x + m_coordinates.size.x - textWidth, m_coordinates.origin.y), Vector2(textWidth, m_coordinates.size.y));
	m_SliderBarCoordinates  = Coordinates(m_coordinates.origin, Vector2(m_coordinates.size.x - textWidth, sliderHeight));
	//Head
	PopulateSliderCoordinatesMap();
	Coordinates headCoord	= GetHeadCoordinates(m_CurrentLevel);
	m_HeadCoordinates = Coordinates(Vector2(headCoord.origin.x, headCoord.origin.y - (sliderHeadSize / 2)), Vector2(sliderHeadSize, sliderHeadSize));
}

void ToonSlider::PopulateSliderCoordinatesMap()
{
	float levelWidth = m_coordinates.size.x / m_SliderLevels;
	m_SliderHeadSize = m_SliderHeadSize > levelWidth ? levelWidth : m_SliderHeadSize;
	for (int i = 0; i < m_SliderLevels; i++)
	{
		float offset = levelWidth * i;
		m_SliderLevelCoordinatesMap[i] = Coordinates(Vector2(m_coordinates.origin.x + offset, m_coordinates.origin.y), Vector2(levelWidth, m_coordinates.size.y));
	}
}

Coordinates ToonSlider::GetHeadCoordinates(int level)
{
	if (level < 0 && level >= m_SliderLevels) { return INVALID_COORDINATE; }
	return m_SliderLevelCoordinatesMap[level];
}


int ToonSlider::GetHeadLevel(Coordinates headCoordinates)
{
	int index = 0;
	for (auto& sliderLevelData : m_SliderLevelCoordinatesMap)
	{
		float sliderLevelEndXCoord = sliderLevelData.second.origin.x + sliderLevelData.second.size.x;
		if (sliderLevelData.second.origin.x >= headCoordinates.origin.x && sliderLevelEndXCoord > headCoordinates.origin.x)
		{
			return  index;
		}
		index++;
	}
	return -1;
}
