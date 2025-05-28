#include <iostream>
#include <stdint.h>
#include <string>
#include <SDL3\SDL.h>
#include "Window.h"



bool Running = false;
static Renderer3D::Window Window;
uint32_t* FrameBuffer = nullptr;
SDL_Texture* FrameBufferTexture = nullptr;



bool SetUp()
{
	FrameBuffer = (uint32_t*)malloc(Window.m_Width * Window.m_Height * sizeof(uint32_t));

	if (!FrameBuffer)
	{
		std::cerr << "ERROR: FrameBuffer memory allocation has failed!\n";
		return false;
	}

	FrameBufferTexture = SDL_CreateTexture(
		Window.m_Renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		Window.m_Width,
		Window.m_Height
	);

	if (!FrameBufferTexture)
	{
		std::cerr << "ERROR: SDL_Texture FrameBufferTexture Creation Failed!\n";
		std::cerr << SDL_GetError() << "\n";
		return false;
	}

	return true;
}

void ProcessInput()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_EVENT_QUIT:
		Running = false;
		break;
	case SDL_EVENT_KEY_DOWN:
		if (event.key.key == SDLK_ESCAPE)
			Running = false;
		break;
	default:
		break;
	}
}

void Update(float dt)
{

}

void RenderFrameBuffer()
{
	if (!SDL_UpdateTexture(FrameBufferTexture, NULL, FrameBuffer, (int)(Window.m_Width * sizeof(uint32_t))))
	{
		std::cerr << "ERROR:SDL UpdateTexture failed!\n";
		std::cerr << SDL_GetError() << "\n";
		return;
	}
	if (!SDL_RenderTexture(Window.m_Renderer, FrameBufferTexture, NULL, NULL))
	{
		std::cerr << "ERROR:SDL RenderTexture has failed!\n";
		std::cerr << SDL_GetError() << "\n";
		return;
	}
}

void UpdateFrameBuffer(uint32_t color = 0)
{
	for (uint32_t i = 0; i < Window.m_Height * Window.m_Width; i++)
	{
		FrameBuffer[i] = color;
	}
}

void DrawGrid(uint32_t space, uint32_t color)
{
	for (uint32_t y = 0; y < Window.m_Height; y += space)
	{
		//int step = y % space == 0 ? 1 : space;
		for (uint32_t x = 0; x < Window.m_Width; x += space)
		{
			FrameBuffer[(Window.m_Width * y) + x] = color;
		}
	}
}

int min(int a, int b)
{
	return a < b ? a : b;
}
int max(int a, int b)
{
	return a < b ? a : b;
}

void DrawRectangle(uint32_t posX, uint32_t posY, uint32_t width, uint32_t height, uint32_t color)
{
	for (uint32_t y = posY; y <= min((posY + height), Window.m_Height); y++)
	{
		//int step = ((y == posY) || (y == (posY + height))) ? 1 : width;
		for (uint32_t x = posX; x <= min((posX + width), Window.m_Height); x++)
		{
			FrameBuffer[(Window.m_Width * y) + x] = color;
		}
	}
}

void Render()
{
	SDL_SetRenderDrawColor(Window.m_Renderer, 30, 30, 30, 255);
	SDL_RenderClear(Window.m_Renderer);

	UpdateFrameBuffer(0xFF0F0F0F);
	DrawGrid(10, 0xFFFFFFFF);
	DrawRectangle(100, 100, 400, 200, 0xFFAA55AA);
	RenderFrameBuffer();


	SDL_RenderPresent(Window.m_Renderer);
}

void FreeResources()
{
	delete FrameBuffer;
	SDL_DestroyTexture(FrameBufferTexture);
}

int main(int argc, char* args[])
{
	std::cout << "Hello World!\n";
	Running = Window.InitializeWindow();

	float LastTime{};
	if (!SetUp()) 
	{ 
		std::cerr << "ERROR: Failed to Set up App!\n";
		return EXIT_FAILURE; 
	}

	while (Running)
	{
		float CurrentTime = SDL_GetTicks() / 1000.0f;
		float dt = CurrentTime - LastTime;
		LastTime = CurrentTime;

		ProcessInput();
		Update(dt);
		Render();
	}

	FreeResources();
}