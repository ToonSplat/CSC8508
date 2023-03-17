#include "ToonSlider.h"

ToonSlider::ToonSlider(Coordinates coordinates, int sliderLevels, float textWidth, float sliderHeight, float sliderHeadSize) : m_coordinates(coordinates), m_SliderLevels(sliderLevels), m_IsActive(false)
{
	AssignCoordinates(textWidth, sliderHeight, sliderHeadSize);
	PopulateSliderCoordinatesMap();
}

ToonSlider::~ToonSlider()
{
}

void ToonSlider::Update(float dt)
{
}

void ToonSlider::DrawSlider()
{

}

void ToonSlider::DrawBar()
{

}

void ToonSlider::DrawHead()
{
}

void ToonSlider::AssignCoordinates(float textWidth, float sliderHeight, float sliderHeadSize)
{
	m_ShouldDisplayLevels	= textWidth > 0.0f;
	m_SliderTextCoordinates = Coordinates(Vector2(m_coordinates.origin.x + m_coordinates.size.x - textWidth, m_coordinates.origin.y), Vector2(textWidth, m_coordinates.size.y));
	m_SliderBarCoordinates  = Coordinates(m_coordinates.origin, Vector2(m_coordinates.size.x - textWidth, sliderHeight));
	//Head
	m_HeadCoordinates		= GetHeadCoordinates(m_CurrentLevel);
}

void ToonSlider::PopulateSliderCoordinatesMap()
{
	float levelWidth = m_coordinates.size.x / m_SliderLevels;
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
