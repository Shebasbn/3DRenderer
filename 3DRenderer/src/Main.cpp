#include <iostream>
#include <stdint.h>
#include <string>
#include <SDL3\SDL.h>
#include "Window.h"
#include "vector.h"

struct Vec3d
{
	float x, y, z;
};

struct Triangle
{
	Vec3d vertex[3];
};

struct Mesh
{
	std::vector<Triangle> triangles;
};

struct Mat4x4
{
	float m[4][4]{};
};

void MultiplyMatrixVector(Vec3d& input, Vec3d& output, Mat4x4& matrix)
{
	output.x = input.x * matrix.m[0][0] + input.y * matrix.m[1][0] + input.z * matrix.m[2][0] + matrix.m[3][0];
	output.y = input.x * matrix.m[0][1] + input.y * matrix.m[1][1] + input.z * matrix.m[2][1] + matrix.m[3][1];
	output.z = input.x * matrix.m[0][2] + input.y * matrix.m[1][2] + input.z * matrix.m[2][2] + matrix.m[3][2];
	float w = input.x * matrix.m[0][3] + input.y * matrix.m[1][3] + input.z * matrix.m[2][3] + matrix.m[3][3];

	if (w != 0.0f)
	{
		output.x /= w;
		output.y /= w;
		output.z /= w;
	}
}

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

Renderer3D::Vec3_t CameraPosition{ 0, 0, -5 };
Renderer3D::Vec3_t CubeRotation{};

Renderer3D::Vec3_t CubeVertices[8]
{
	{-1.0f, -1.0f, -1.0f}, 
	{-1.0f, 1.0f, -1.0f}, 
	{-1.0f, -1.0f, 1.0f}, 
	{1.0f, -1.0f, -1.0f}, 
};

Mesh CubeMesh;
Mat4x4 ProjectionMatrix{};




float FovFactor = 648;

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

void DrawLine(float startX, float startY, float endX, float endY, uint32_t color)
{
	float deltaX = endX - startX;
	float deltaY = endY - startY;

	float absDeltaX = fabsf(deltaX);
	float absDeltaY = fabsf(deltaY);

	float stepX = (deltaX > 0.0f) ? 1.0f : -1.0f;
	float stepY = (deltaY > 0.0f) ? 1.0f : -1.0f;

	float x = startX;
	float y = startY;

	if (absDeltaX > absDeltaY)
	{
		float slope = deltaY / deltaX;
		float error = 0.5f;
		for (int i = 0; i <= (int)absDeltaX; ++i)
		{
			DrawPixel(roundf(x), roundf(y), color);
			x += stepX;
			error += fabsf(slope);
			if (error >= 1.0f)
			{
				y += stepY;
				error -= 1.0f;
			}
		}
	}
	else
	{
		float invSlope = deltaX / deltaY;
		float error = 0.5f;

		for (int i = 0; i <= (int)absDeltaY; ++i)
		{
			DrawPixel(roundf(x), roundf(y), color);
			y += stepY;
			error += fabsf(invSlope);
			if (error >= 1.0f)
			{
				x += stepX;
				error -= 1.0f;
			}
		}
	}

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
	return Renderer3D::Vec2_t(
		((point.X * FovFactor) / point.Z) + (Window.GetWidth() / 2.0f),
		((point.Y * FovFactor) / point.Z) + (Window.GetHeight() / 2.0f)
	);
}

void Update(float dt)
{
	std::cout << "Delta-Time: " << dt << std::endl;
	CubeRotation.X += 0.0f * dt;
	CubeRotation.Y += 0.0f * dt;
	CubeRotation.Z += 0.1f * dt;

	for (int i = 0; i < N_POINTS; i++)
	{
		Renderer3D::Vec3_t point = CubePoints[i];

		Renderer3D::Vec3_t transformedPoint = point.RotateX(CubeRotation.X);
		transformedPoint = transformedPoint.RotateY(CubeRotation.Y);
		transformedPoint = transformedPoint.RotateZ(CubeRotation.Z);

		transformedPoint.Z -= CameraPosition.Z;
		ProjectedPoints[i] = Project(transformedPoint);
	}


	for (const auto& triangle: CubeMesh.triangles)
	{
		Triangle triProjected, triTranslated;

		triTranslated = triangle;
		for (int i = 0; i < 3; i++)
		{
			triTranslated.vertex[i].z = triangle.vertex[i].z + 3.0f;
		}

		MultiplyMatrixVector(triTranslated.vertex[0], triProjected.vertex[0], ProjectionMatrix);
		MultiplyMatrixVector(triTranslated.vertex[1], triProjected.vertex[1], ProjectionMatrix);
		MultiplyMatrixVector(triTranslated.vertex[2], triProjected.vertex[2], ProjectionMatrix);

		// Scale into view
		for (int i = 0; i < 3; i++)
		{
			triProjected.vertex[i].x = (triProjected.vertex[i].x + 1.0f) * 0.5f * (float)Window.GetWidth();
			triProjected.vertex[i].y = (triProjected.vertex[i].y + 1.0f) * 0.5f * (float)Window.GetHeight();
		}

		DrawLine(triProjected.vertex[0].x, triProjected.vertex[0].y, triProjected.vertex[1].x, triProjected.vertex[1].y, 0xFFFFFFFF);
		DrawLine(triProjected.vertex[1].x, triProjected.vertex[1].y, triProjected.vertex[2].x, triProjected.vertex[2].y, 0xFFFFFFFF);
		DrawLine(triProjected.vertex[2].x, triProjected.vertex[2].y, triProjected.vertex[0].x, triProjected.vertex[0].y, 0xFFFFFFFF);
	}


}

void Render()
{
	DrawGrid(10, 0xFFFFFFFF);

	/*for (int i = 0; i < N_POINTS; i++)
	{
		Renderer3D::Vec2_t projectedPoint = ProjectedPoints[i];
		float width = 4.0f;
		float height = 4.0f;
		DrawRectangle(
			projectedPoint.X - (width/2.0f),
			projectedPoint.Y - (height/2.0f),
			width, 
			height, 
			0xFFFFFF00);
	}*/

	DrawLine(100, 100, 200, 200, 0xFFFFFFFF);
	DrawLine(200, 200, 100, 200, 0xFFFFFFFF);
	DrawLine(100, 200, 100, 100, 0xFFFFFFFF);

	//DrawLine(50.0f, 100.0f, 200.0f, 150.0f, 0xFFFFFFFF);  // shallow slope
	//DrawLine(200.0f, 150.0f, 50.0f, 100.0f, 0xFF00FF00);  // reversed
	//DrawLine(100.0f, 200.0f, 100.0f, 50.0f, 0xFFFF0000);  // vertical

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

	CubeMesh.triangles =
	{
		// SOUTH
		{ 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

		// EAST                                                      
		{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

		// NORTH                                                     
		{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

		// WEST                                                      
		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

		// TOP                                                       
		{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

		// BOTTOM                                                    
		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },

	};

	float Near = 0.1f;
	float Far = 1000.0f;
	float Fov = 90.0f;
	float AspectRatio = (float)Window.GetWidth() / (float)Window.GetHeight();
	float FovRad = 1.0f / tanf(Fov * 0.5f / 180.0f * 3.14159f);

	ProjectionMatrix.m[0][0] = AspectRatio * FovRad;
	ProjectionMatrix.m[1][1] = FovRad;
	ProjectionMatrix.m[2][2] = Far / (Far - Near);
	ProjectionMatrix.m[3][2] = (-Far * Near) / (Far - Near);
	ProjectionMatrix.m[2][3] = 1.0f;
	ProjectionMatrix.m[3][3] = 0.0f;

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