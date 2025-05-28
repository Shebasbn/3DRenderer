#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <SDL3/SDL.h>


namespace Renderer3D::detail
{
	struct SDLWindowDeleter
	{
		void operator()(SDL_Window* window) const {
			if (window) {
				SDL_DestroyWindow(window);
			}
		}
	};
	struct SDLRendererDeleter
	{
		void operator()(SDL_Renderer* renderer) const {
			if (renderer) {
				SDL_DestroyRenderer(renderer);
			}
		}
	};
}

namespace Renderer3D
{
	class Window
	{
	public:
		Window();
		~Window();
		bool InitializeWindow();
		void DestroyWindow();

		SDL_Window* GetWindow() const { return m_Window.get(); }
		SDL_Renderer* GetRenderer() const { return m_Renderer.get(); }

		uint32_t GetWidth() const { return m_Width; }
		void SetWidth(uint32_t width) { m_Width = width; }

		uint32_t GetHeight() const { return m_Height; }
		void SetHeight(uint32_t height) { m_Height = height; }

		const std::string& GetTitle() const { return m_Title; }
		void SetTitle(const std::string& title) { m_Title = title; }


		bool IsFullscreen() const { return m_IsFullscreen; }
		
		void SetFullscreen(bool enabled) 
		{ 
			if (enabled != m_IsFullscreen)
			{
				m_IsFullscreen = enabled;
				SDL_SetWindowFullscreen(GetWindow(), m_IsFullscreen);
			}
		}

	private:
		std::unique_ptr<SDL_Window, detail::SDLWindowDeleter> m_Window;
		std::unique_ptr<SDL_Renderer, detail::SDLRendererDeleter> m_Renderer;
		std::string m_Title;
		uint32_t m_Width;
		uint32_t m_Height;
		bool m_IsFullscreen;
	};
}


