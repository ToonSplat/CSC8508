/*
Part of Newcastle University's Game Engineering source code.

Use as you see fit!

Comments and queries to: richard-gordon.davison AT ncl.ac.uk
https://research.ncl.ac.uk/game/
*/
#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "GameTimer.h"

#include "Vector2.h"

#include <string>

namespace NCL {
	namespace Rendering {
		class RendererBase;
	};
	using namespace Rendering;
	
	class Window {
	public:
		static Window* CreateGameWindow(std::string title = "NCLGL!", int sizeX = 800, int sizeY = 600, bool fullScreen = false, int offsetX = 100, int offsetY = 100);

		static void DestroyGameWindow() {
			delete window;
			window = nullptr;
		}

		bool		IsMinimised() const { return minimised;	 }

		bool		UpdateWindow();
		void		SetRenderer(RendererBase* r);

		bool		IsFullScreen()		const { return fullScreen; }
		bool		HasInitialised()	const { return init; }

		float		GetScreenAspect()	const {
			return size.x / size.y;
		}

		Vector2		GetScreenSize()		const { return size; }
		Vector2		GetScreenPosition()	const { return position; }

		const std::string&  GetTitle()   const { return windowTitle; }
		void				SetTitle(const std::string& title) {
			windowTitle = title;
			UpdateTitle();
		};

		virtual void	LockMouseToWindow(bool lock) = 0;
		virtual void	ShowOSPointer(bool show) = 0;

		virtual void	SetWindowPosition(int x, int y) {};
		virtual void	SetFullScreen(bool state) {};
		virtual void	SetConsolePosition(int x, int y) {};
		virtual void	ShowConsole(bool state) {};

		static const Keyboard*	 GetKeyboard() { return keyboard; }
		static const Mouse*		 GetMouse() { return mouse; }
		static const GameTimer*	 GetTimer() { return timer; }

		static Window*	const GetWindow() { return window; }
	protected:
		Window();
		virtual ~Window();

		virtual void UpdateTitle() {}

		virtual bool InternalUpdate() = 0;

		void ResizeRenderer();

		RendererBase*	renderer;

		bool				fullScreen;
		bool				minimised;
		bool				init;
		Vector2				position;
		Vector2				size;
		Vector2				defaultSize;

		std::string			windowTitle;

		static Window*		window;
		static Keyboard*	keyboard;
		static Mouse*		mouse;
		static GameTimer*	timer;
	};
}
