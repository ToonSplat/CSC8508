#pragma once

#include <unordered_map>
#include "GameTechRenderer.h"
#include "Toon2DCoordinates.h"
#include "InputManager.h"

class ToonSlider
{
	private:
		Coordinates							 m_coordinates;	//Coordinates of just slider with text space
		Vector2								 m_WindowSize;
		int									 m_SliderLevels;
		bool								 m_IsActive;
		bool								 m_ShouldDisplayLevels;
		Coordinates							 m_SliderBarCoordinates;
		Coordinates							 m_SliderTextCoordinates;
		Coordinates							 m_HeadCoordinates;
		const Vector4& m_SliderCoveredColour = Debug::GREEN;
		const Vector4& m_SliderUncoveredColour = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
		int									 m_CurrentLevel = 0;
		std::unordered_map<int, Coordinates> m_SliderLevelCoordinatesMap;	// Using it, in order to avoid repetitive calculations.
		float								 m_SliderHeadSize;
		const Coordinates					 INVALID_COORDINATE = Coordinates(Vector2(-1.0f, -1.0f), Vector2(-1.0f, -1.0f));

	public:
		ToonSlider(Coordinates coordinates, int sliderLevels, Vector2 windowSize, float textWidth = 5.0f, float sliderHeight = 1.0f, float sliderHeadSize = 5.0f);	//Set text width to 0 if Slider text is not needed
		~ToonSlider();

		void Update(float dt);
		void HandleKeyboardAndMouseEvents();
		int GetCurrentVolumeLevel();
		void SetCurrentVolumeLevel(int volumeLevel);

	private:
		void DrawSlider();
		void DrawBar();
		void DrawHead();
		void AssignCoordinates(float textWidth, float sliderHeight, float sliderHeadSize);
		void PopulateSliderCoordinatesMap();
		Coordinates GetHeadCoordinates(int level);
		int GetHeadLevel(Vector2 headPosition);
		int GetHeadLevelUsingMousePosition(Vector2 mousePosition);	//Calculation taking constant computation
};