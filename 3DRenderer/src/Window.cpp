#include "Window.h"
#include <iostream>

namespace Renderer3D
{
	Window::Window()
		: m_Title{ "3DRenderer" }, m_Window{nullptr}, m_Renderer{ nullptr }, m_Width{ 1080 }, m_Height{ 720 }, m_IsFullscreen{true}
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
		
		if (m_IsFullscreen)
		{
			m_Width = DisplayMode->w;
			m_Height = DisplayMode->h;
		}
		SDL_Window* window;
		SDL_Renderer* renderer;
		if (!SDL_CreateWindowAndRenderer(
			m_Title.c_str(),
			m_Width,
			m_Height,
			SDL_WINDOW_BORDERLESS,
			&window, &renderer
		))
		{
			std::cerr << "ERROR: SDL Window and Renderer Creation Failed!\n";
			std::cerr << SDL_GetError() << "\n";
			return false;
		}
		m_Window.reset(window);
		m_Renderer.reset(renderer);

		SDL_SetWindowFullscreen(m_Window.get(), m_IsFullscreen);
		return true;
	}

	void Window::DestroyWindow()
	{
		m_Window.reset();
		m_Renderer.reset();
	}
}