#include "common.hpp"
#include "gameboy.hpp"

using namespace Contants;

SDL_Window* window = NULL;
SDL_Surface* screen_surface = NULL;
SDL_Texture* texture = NULL;
SDL_Renderer* renderer = NULL;
SDL_Event event;

// Initialize SDL and create window
bool init_SDL();
void close_SDL();

int main(int argc, char* argv[])
{
	if (!init_SDL())
		exit(1);

	GameBoy* gb = new GameBoy();
	bool quit = false;

	while (!quit)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				// switch case to handle key presses
			}
		}
	}

	close_SDL();
	return EXIT_SUCCESS;
}

bool init_SDL()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "SDL could not be initialized!");
		success = false;
	}
	else
	{
		window = SDL_CreateWindow("GBC++", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			fprintf(stderr, "SDL window could not be created!");
			success = false;
		}
		else
		{
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (renderer == NULL)
			{
				fprintf(stderr, "SDL renderer could not be created!");
				success = false;
			}
			else
			{
				screen_surface = SDL_GetWindowSurface(window);
				SDL_FillRect(screen_surface, NULL, SDL_MapRGB(screen_surface->format, 0x00, 0x23, 0xFF));
				SDL_UpdateWindowSurface(window);
			}
		}
	}

	return success;
}

void close_SDL()
{
	SDL_FreeSurface(screen_surface);
	screen_surface = NULL;

	SDL_DestroyWindow(window);
	window = NULL;

	SDL_Quit();
}