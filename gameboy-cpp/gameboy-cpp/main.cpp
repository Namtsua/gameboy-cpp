#include "common.hpp"
#include "gameboy.hpp"

using namespace Constants;

SDL_Window* window = NULL;
SDL_Surface* screen_surface = NULL;
SDL_Texture* texture = NULL;
SDL_Renderer* renderer = NULL;
SDL_Event event;
GPU* m_gpu = NULL;

// Initialize SDL and create window
bool init_SDL();
void close_SDL();
void draw_frame();

int main(int argc, char* argv[])
{
	if (!init_SDL())
		exit(1);

	GameBoy* gb = new GameBoy(renderer);
	gb->start();
	bool quit = false;
	while (!quit)
	{
		gb->cycle();

		//while (SDL_PollEvent(&event) != 0)
		//{
		//	if (event.type == SDL_QUIT)
		//	{
		//		quit = true;
		//	}
		//	else if (event.type == SDL_KEYDOWN)
		//	{
		//		// switch case to handle key presses
		//	}
		//}
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
		window = SDL_CreateWindow("GBC++", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DISPLAY_WIDTH, DISPLAY_HEIGHT, SDL_WINDOW_SHOWN);
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
				texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, DISPLAY_WIDTH, DISPLAY_HEIGHT);
				if (texture == NULL)
				{
					fprintf(stderr, "SDL texture could not be created!");
					success = false;
				}
			}
		}
	}

	return success;
}

void close_SDL()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyTexture(texture);
	SDL_Quit();
}