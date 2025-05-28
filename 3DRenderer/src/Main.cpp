#include <iostream>
#include <stdint.h>
#include <string>
#include <SDL3\SDL.h>
#include "Window.h"
#include "vector.h"


// Utility Functions
int min(int a, int b)
{
	return a < b ? a : b;
}
int max(int a, int b)
{
	return a > b ? a : b;
}

constexpr int N_POINTS = 9 * 9 * 9;
Renderer3D::Vec3_t CubePoints[N_POINTS];
Renderer3D::Vec2_t ProjectedPoints[N_POINTS];

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

void DrawPixel(int x, int y, uint32_t color)
{
	if (x >= 0 && x < Window.GetWidth() && y >= 0 && y < Window.GetHeight())
		FrameBuffer[(Window.GetWidth() * y) + x] = color;
}

void DrawRectangle(int posX, int posY, int width, int height, uint32_t color)
{
	for (int y = max(posY, 0); y <= min((posY + height), Window.GetHeight()-1); y++)
	{
		//int step = ((y == posY) || (y == (posY + height))) ? 1 : width;
		for (int x = max(posX, 0); x <= min((posX + width), Window.GetWidth()-1); x++)
		{
			DrawPixel(x, y, color);
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

	int pointCount{};
	for (float x = -1.0f; x <= 1.0f; x += 0.25f)
	{
		for (float y = -1.0f; y <= 1.0f; y += 0.25f)
		{
			for (float z = -1.0f; z <= 1.0f; z += 0.25f)
			{
				Renderer3D::Vec3_t newPoint{ x, y, z };
				CubePoints[pointCount++] = newPoint;
			}
		}
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

Renderer3D::Vec2_t Project(Renderer3D::Vec3_t point)
{
	return Renderer3D::Vec2_t(point.X, point.Y);
}

void Update(float dt)
{
	for (int i = 0; i < N_POINTS; i++)
	{
		ProjectedPoints[i] = Project(CubePoints[i]);
	}
}

void Render()
{
	DrawGrid(10, 0xFFFFFFFF);

	for (int i = 0; i < N_POINTS; i++)
	{
		Renderer3D::Vec2_t projectedPoint = ProjectedPoints[i];
		float newX = (projectedPoint.X * Window.GetWidth() / 2.0f) + Window.GetWidth() / 2.0f;
		float newY = (projectedPoint.Y * Window.GetHeight() / 2.0f) + Window.GetHeight() / 2.0f;
		float width = 4.0f;
		float height = 4.0f;
		DrawRectangle(
			newX - width/2.0f,
			newY - height/2.0f,
			width, 
			height, 
			0xFFFFFF00);
	}

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