#pragma once

#include <SDL3/SDL.h>

namespace Renderer3D
{
	class Window
	{
	public:
		Window();
		~Window();
		bool InitializeWindow();
		void DestroyWindow();
	public:
		char* m_Title = "3DRenderer";
		SDL_Window* m_Window = nullptr;
		SDL_Renderer* m_Renderer = nullptr;
		uint32_t m_Width = 1080;
		uint32_t m_Height = 720;
	};
}

