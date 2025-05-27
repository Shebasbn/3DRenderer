#include <iostream>
#include <stdint.h>
#include <string>
#include <SDL3\SDL.h>


constexpr char* WINDOW_TITLE = "3DRenderer";
uint32_t WindowWidth = 1080;
uint32_t WindowHeight = 720;
bool Running = false;
SDL_Window* Window = nullptr;
SDL_Renderer* Renderer = nullptr;
uint32_t* ColorBuffer = nullptr;

bool InitializeWindow()
{
	if (!SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
	{
		std::cerr << "ERROR: SDL Initialization Failed\n";
		return false;
	}

	if (!SDL_CreateWindowAndRenderer(
		WINDOW_TITLE, 
		WindowWidth, 
		WindowHeight, 
		SDL_WINDOW_BORDERLESS | SDL_WINDOW_FULLSCREEN, 
		&Window, &Renderer))
	{
		std::cerr << "ERROR: SDL Window and Renderer Creation Failed\n";
		return false;
	}

	return true;
}

void SetUp()
{
	ColorBuffer = (uint32_t*)malloc(WindowWidth * WindowHeight * sizeof(uint32_t));
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
	SDL_SetRenderDrawColor(Renderer, 30, 30, 30, 255);
	SDL_RenderClear(Renderer);

	SDL_RenderPresent(Renderer);
}

int main(int argc, char* args[])
{
	std::cout << "Hello World!\n";
	Running = InitializeWindow();

	float LastTime{};
	SetUp();

	while (Running)
	{
		float CurrentTime = SDL_GetTicks() / 1000.0f;
		float dt = CurrentTime - LastTime;
		LastTime = CurrentTime;

		ProcessInput();
		Update(dt);
		Render();
	}
}