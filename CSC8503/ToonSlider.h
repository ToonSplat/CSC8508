#pragma once

#include <unordered_map>
#include "GameTechRenderer.h"
#include "Toon2DCoordinates.h"
#include "InputManager.h"

class ToonSlider
{
	private:
		Coordinates							 m_coordinates;	//Coordinates of just slider with text space
		int									 m_SliderLevels;
		bool								 m_IsActive;
		bool								 m_ShouldDisplayLevels;
		Coordinates							 m_SliderBarCoordinates;
		Coordinates							 m_SliderTextCoordinates;
		Coordinates							 m_HeadCoordinates;
		const Vector4&						 m_SliderCoveredColour   = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
		const Vector4&						 m_SliderUncoveredColour = Debug::GREEN;
		int									 m_CurrentLevel		     = 0;
		std::unordered_map<int, Coordinates> m_SliderLevelCoordinatesMap;	// Using it, in order to avoid repetitive calculations.
		const Coordinates					 INVALID_COORDINATE		 = Coordinates(Vector2(-1.0f, -1.0f), Vector2(-1.0f, -1.0f));

	public:
		ToonSlider(Coordinates coordinates, int sliderLevels, float textWidth = 10.0f, float sliderHeight = 2.0f, float sliderHeadSize = 5.0f);	//Set text width to 0 if Slider text is not needed
		~ToonSlider();

		void Update(float dt);

	private:
		void DrawSlider();
		void DrawBar();
		void DrawHead();
		void AssignCoordinates(float textWidth, float sliderHeight, float sliderHeadSize);
		void PopulateSliderCoordinatesMap();
		Coordinates GetHeadCoordinates(int level);
		int GetHeadLevel(Coordinates headCoordinates);
};