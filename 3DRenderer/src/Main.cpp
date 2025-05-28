#include <iostream>
#include <stdint.h>
#include <string>
#include <SDL3\SDL.h>
#include "Window.h"


// Utility Functions
int min(int a, int b)
{
	return a < b ? a : b;
}
int max(int a, int b)
{
	return a < b ? a : b;
}


bool Running = false;
static Renderer3D::Window Window;
uint32_t* FrameBuffer = nullptr;
SDL_Texture* FrameBufferTexture = nullptr;



void DrawGrid(uint32_t space, uint32_t color)
{
	for (uint32_t y = 0; y < Window.GetHeight(); y += space)
	{
		//int step = y % space == 0 ? 1 : space;
		for (uint32_t x = 0; x < Window.GetWidth(); x += space)
		{
			FrameBuffer[(Window.GetWidth() * y) + x] = color;
		}
	}
}

void DrawPixel(uint32_t x, uint32_t y, uint32_t color)
{
	if (x < Window.GetWidth() && y < Window.GetHeight())
		FrameBuffer[(Window.GetWidth() * y) + x] = color;
}

void DrawRectangle(uint32_t posX, uint32_t posY, uint32_t width, uint32_t height, uint32_t color)
{
	for (uint32_t y = posY; y <= min((posY + height), Window.GetHeight()); y++)
	{
		//int step = ((y == posY) || (y == (posY + height))) ? 1 : width;
		for (uint32_t x = posX; x <= min((posX + width), Window.GetWidth()); x++)
		{
			FrameBuffer[(Window.GetWidth() * y) + x] = color;
		}
	}
}

void RenderFrameBuffer()
{
	if (!SDL_UpdateTexture(FrameBufferTexture, NULL, FrameBuffer, (int)(Window.GetWidth() * sizeof(uint32_t))))
	{
		std::cerr << "ERROR:SDL UpdateTexture failed!\n";
		std::cerr << SDL_GetError() << "\n";
		return;
	}
	if (!SDL_RenderTexture(Window.GetRenderer(), FrameBufferTexture, NULL, NULL))
	{
		std::cerr << "ERROR:SDL RenderTexture has failed!\n";
		std::cerr << SDL_GetError() << "\n";
		return;
	}
}

void UpdateFrameBuffer(uint32_t color = 0)
{
	for (uint32_t i = 0; i < Window.GetHeight() * Window.GetWidth(); i++)
	{
		FrameBuffer[i] = color;
	}
}


bool SetUp()
{
	FrameBuffer = (uint32_t*)malloc(Window.GetWidth() * Window.GetHeight() * sizeof(uint32_t));

	if (!FrameBuffer)
	{
		std::cerr << "ERROR: FrameBuffer memory allocation has failed!\n";
		return false;
	}

	FrameBufferTexture = SDL_CreateTexture(
		Window.GetRenderer(),
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		Window.GetWidth(),
		Window.GetHeight()
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

void Render()
{
	SDL_SetRenderDrawColor(Window.GetRenderer(), 30, 30, 30, 255);
	SDL_RenderClear(Window.GetRenderer());

	DrawGrid(10, 0xFFFFFFFF);
	
	DrawPixel(25, 25, 0xFFFFFF00);
	
	DrawRectangle(100, 100, 400, 200, 0xFFAA55AA);


	RenderFrameBuffer();
	UpdateFrameBuffer(0xFF0F0F0F);

	SDL_RenderPresent(Window.GetRenderer());
}

void FreeResources()
{
	free(FrameBuffer);
	SDL_DestroyTexture(FrameBufferTexture);
	SDL_Quit();
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