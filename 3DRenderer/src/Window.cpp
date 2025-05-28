#include "Window.h"
#include <iostream>

namespace Renderer3D
{
	Window::Window()
		: m_Title{ "3DRenderer" }, m_Window{}, m_Renderer{}, m_Width{ 1080 }, m_Height{ 720 }
	{
		//InitializeWindow();
	}
	Window::~Window()
	{
		DestroyWindow();
	}
	bool Window::InitializeWindow()
	{
		if (!SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
		{
			std::cerr << "ERROR: SDL Initialization Failed!\n";
			std::cerr << SDL_GetError() << "\n";
			return false;
		}

		SDL_DisplayID DisplayID = SDL_GetPrimaryDisplay();
		const SDL_DisplayMode* DisplayMode = SDL_GetCurrentDisplayMode(DisplayID);
		m_Width = DisplayMode->w;
		m_Height = DisplayMode->h;

		if (!SDL_CreateWindowAndRenderer(
			m_Title,
			m_Width,
			m_Height,
			SDL_WINDOW_BORDERLESS | SDL_WINDOW_FULLSCREEN,
			&m_Window, &m_Renderer))
		{
			std::cerr << "ERROR: SDL Window and Renderer Creation Failed!\n";
			std::cerr << SDL_GetError() << "\n";
			return false;
		}

		return true;
	}

	void Window::DestroyWindow()
	{
		SDL_DestroyRenderer(m_Renderer);
		SDL_DestroyWindow(m_Window);
		SDL_Quit();
	}
}