// main.cpp : Defines the entry point for the console application.
//
#include <Windows.h>
#include "stdafx.h"
#include "CHIP8.h"
#include <SDL\SDL.h>
#include <GL\glew.h>
#include <iostream>
#include <string>


//Define CPU object
CHIP8 CPU8;

//Define SDL window varibles
SDL_Window* window;
SDL_GLContext context;
int window_x;
int window_y;
int window_width = 640;
int window_height = 320;
char* window_title;

//Prototype Functions
void SetupInput();




bool
Initialize() 
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		return false;
	}

	//Create Window
	window = SDL_CreateWindow("CHIP8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_OPENGL);
	//Create OpenGL context
    context = SDL_GL_CreateContext(window);
	glewExperimental = GL_TRUE;
	glewInit();

	/*glViewport(0, 0, window_width, window_height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glOrtho(0, window_width, window_height, 0, -1.0, 1.0);
	glClearColor(0, 0, 0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glShadeModel(GL_FLAT);

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DITHER);
	glDisable(GL_BLEND);*/

	//SDL_WM_SetCaption("CodeSlinger - Chip8 Emulator", NULL);
	return true;
}



void
SetupInput()
{

}

void RenderFrame(CHIP8* cpu)
{
	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glRasterPos2i(-1, 1);
	glPixelZoom(1, -1);
	try {
		glDrawPixels(window_width, window_height, GL_RGB, GL_UNSIGNED_BYTE, cpu->gfx);
		SDL_GL_SwapWindow(window);
		glFlush();
		cpu->draw = false;
	}
	catch (int i)
	{
		
	}
	
}


void HandleInput(CHIP8* cpu, SDL_Event event)
{
	if (event.type == SDL_KEYDOWN)
	{
		int key = -1;
		switch (event.key.keysym.sym)
		{
		case SDLK_x: key = 0; break;
		case SDLK_1: key = 1; break;
		case SDLK_2: key = 2; break;
		case SDLK_3: key = 3; break;
		case SDLK_q: key = 4; break;
		case SDLK_w: key = 5; break;
		case SDLK_e: key = 6; break;
		case SDLK_a: key = 7; break;
		case SDLK_s: key = 8; break;
		case SDLK_d: key = 9; break;
		case SDLK_z: key = 10; break;
		case SDLK_c: key = 11; break;
		case SDLK_4: key = 12; break;
		case SDLK_r: key = 13; break;
		case SDLK_f: key = 14; break;
		case SDLK_v: key = 15; break;
		default: break;
		}
		if (key != -1)
		{
			cpu->KeyPressed(key);
		}
	}

	else if (event.type == SDL_KEYUP)
	{
		int key = -1;
		switch (event.key.keysym.sym)
		{
		case SDLK_x: key = 0; break;
		case SDLK_1: key = 1; break;
		case SDLK_2: key = 2; break;
		case SDLK_3: key = 3; break;
		case SDLK_q: key = 4; break;
		case SDLK_w: key = 5; break;
		case SDLK_e: key = 6; break;
		case SDLK_a: key = 7; break;
		case SDLK_s: key = 8; break;
		case SDLK_d: key = 9; break;
		case SDLK_z: key = 10; break;
		case SDLK_c: key = 11; break;
		case SDLK_4: key = 12; break;
		case SDLK_r: key = 13; break;
		case SDLK_f: key = 14; break;
		case SDLK_v: key = 15; break;
		default: break;
		}
		if (key != -1)
		{
			cpu->KeyReleased(key);
		}

		
	}
}

int
main(int argc, char *args[])
{
	std::string s;
	
	int fps = 60;
	bool quit = false;
	bool loaded = false;
	SDL_Event event;
	float interval = 1000;
	interval /= fps;
	unsigned int time2 = SDL_GetTicks();
	int numopcodes = 400;
	int numframe = numopcodes / fps;
	std::cout << "Load rom" << std::endl;
	std::getline(std::cin, s);
	
	while(!loaded){
		try {
			CPU8.initialize();
			CPU8.loadgame(s);
			loaded = true;
		}
		catch (int e) {
			std::cout << "Load failed" << std::endl;
			std::cout << "Load rom" << std::endl;
			std::getline(std::cin, s);


		}
	}
	//Set up OpenGL
	if (!Initialize()) {
		return 1;
	}
	

	while(!quit)
	{
		while (SDL_PollEvent(&event) && !quit)
		{
			HandleInput(&CPU8, event);

			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		unsigned int current = SDL_GetTicks();

		if ((time2 + interval) < current && !quit)
		{
			for (int i = 0; i < numframe; i++)
				CPU8.emulateCycle();

			time2 = current;
			if(CPU8.draw)
			RenderFrame(&CPU8);
			
		}
	}

	//system("PAUSE");
	//Clean up 
	delete context;
	SDL_Quit();
	return 0;
}
