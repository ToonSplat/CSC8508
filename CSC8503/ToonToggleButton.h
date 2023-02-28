#pragma once
//#include "Debug.h"
//#include "Toon2DCoordinates.h"


#include "GameTechRenderer.h"
#include "Toon2DCoordinates.h"
#include "InputManager.h"

using namespace NCL;
using namespace CSC8503;


enum ToggleButtonStates
{
	ToggleOff,
	ToggleOn
};


class ToonToggleButtonHead
{
	enum AnimationDirection
	{
		NoAnimation,
		LeftToRight,
		RightToLeft
	};

	private:
		Vector2			   m_StartLocation;
		Vector2			   m_EndLoacation;
		Vector2			   m_CurrentLocation;
		Vector2			   m_Size;
		AnimationDirection m_AnimationDirection;
		const Vector4	   m_HeadColour		= Debug::WHITE;
		const float		   m_AnimationSpeed = 0.5f;
		
	public:
		ToonToggleButtonHead(Coordinates coordinates)
		{
			m_StartLocation		 = coordinates.origin;
			m_EndLoacation		 = coordinates.origin + coordinates.size;
			m_CurrentLocation	 = m_StartLocation;
			m_Size				 = coordinates.size;
			m_AnimationDirection = AnimationDirection::NoAnimation;
		}

		void TriggerAnimation() { UpdateAnimationDirection(); }

		void UpdateHead()
		{
			DrawHead();
		}

	private:
		void UpdateAnimationDirection()
		{
			if (m_CurrentLocation.x == m_StartLocation.x) { m_AnimationDirection = AnimationDirection::LeftToRight; }
			else { m_AnimationDirection = AnimationDirection::RightToLeft; }
		}

		float GetIncrement()
		{
			if		(m_AnimationDirection == AnimationDirection::LeftToRight) { return  m_AnimationSpeed; }
			else if (m_AnimationDirection == AnimationDirection::RightToLeft) { return -m_AnimationSpeed; }
			return 0.0f;
		}

		void DrawHead()
		{
			m_CurrentLocation.x += GetIncrement();
			Debug::DrawFilledQuad(m_CurrentLocation, m_Size, m_HeadColour);
			if (m_CurrentLocation.x <= m_StartLocation.x || m_CurrentLocation.x >= m_EndLoacation.x)
			{
				m_AnimationDirection = AnimationDirection::NoAnimation;
			}
		}
};


class ToonToggleButton
{
	private:
		Coordinates			  m_Coordinates;
		ToggleButtonStates	  m_CurrentState;
		const Vector4		  m_ToggleOnColour   = Debug::GREEN;
		const Vector4		  m_ToggleOffColour  = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
		const Vector4		  m_HeadColour		 = Debug::WHITE;
		Coordinates			  m_HalfSize;
		bool				  m_ShouldDrawValue	 = false;
		bool				  m_IsAnimating		 = false;
		ToonToggleButtonHead* m_ToggleButtonHead = NULL;
		
	public:
		bool m_IsActive = false;

	public:
		ToonToggleButton(Coordinates coordinates, bool shouldDrawValue = false);
		~ToonToggleButton();
		ToggleButtonStates GetButtonState();
		void UpdateButtonDraw();
		void HandleEvents();
	
	private:
		void DrawButton();
		void DrawOffToggle();
		void DrawOnToggle();
		void DrawValue();
		void ToggleCurrentState();
};