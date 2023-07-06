#pragma once
#ifndef __Game__
#define __Game__

class Game
{
public:

	bool Running;
	int windowx, windowy;
	bool finish;
	bool up, down;

	SDL_Window* Window;
	SDL_Renderer* Renderer;

	SDL_Rect rects[7];
	SDL_Texture* ball;
	SDL_Rect sourceball;
	int ballSpeedx, ballSpeedy;

	Game() {}
	~Game() {}

	bool init(const char* title, int xpos, int ypos, int width, int height, int flags)
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
		{
			std::cout << "SDL init success\n";
			Window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
			if (Window != 0)
			{
				std::cout << "window creation success\n";
				Renderer = SDL_CreateRenderer(Window, -1, 0);
				if (Renderer != 0)
				{
					std::cout << "renderer creation success\n";
					SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
					SDL_GetRendererOutputSize(Renderer, &windowx, &windowy);
				}
				else
				{
					std::cout << "renderer init fail\n";
					return false;
				}
			}
			else
			{
				std::cout << "window init fail\n";
				return false;
			}
		}
		else
		{
			std::cout << "SDL init fail\n";
			return false;
		}
		std::cout << "init success\n";
		Running = true;

		SDL_Surface* pTempSurface = SDL_LoadBMP("assets/ball.bmp");
		ball = SDL_CreateTextureFromSurface(Renderer,pTempSurface);
		SDL_FreeSurface(pTempSurface);
		SDL_QueryTexture(ball, NULL, NULL, &sourceball.w, &sourceball.h);

		start();
		finish = true; 

		return true;
	};
	void start() {
		rects[0].w = rects[1].w = windowx / 50;
		rects[0].h = rects[1].h = windowy / 8;
		rects[0].x = windowx / 15;
		rects[1].x = windowx - rects[0].x;
		rects[0].y = rects[1].y = (windowy / 2) - (rects[0].h / 2);

		rects[2].x = rects[3].x = 0;
		rects[2].w = rects[3].w = windowx;
		rects[2].h = rects[3].h = 5;
		rects[2].y = 0;
		rects[3].y = windowy - 5;

		rects[4].y = rects[5].y = 0;
		rects[4].h = rects[5].h = windowy;
		rects[4].w = rects[5].w = 5;
		rects[4].x = 0;
		rects[5].x = windowx - 5;


		sourceball.x = sourceball.y = 0;
		rects[6].h = rects[6].w = windowy / 30;
		rects[6].x = windowx / 2;
		rects[6].y = windowy / 2;
		ballSpeedx = -10;
		ballSpeedy = 0;
	};

	void render() {
		SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
		SDL_RenderClear(Renderer);
		SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
		SDL_RenderFillRects(Renderer, rects, 6);
		SDL_RenderCopy(Renderer, ball, &sourceball, &rects[6]);
		SDL_RenderPresent(Renderer);
	};
	void update() {
		//bounce
		if (rects[6].x <= (rects[0].x + rects[0].w) && rects[6].x >= rects[0].x && rects[6].y + rects[6].h / 2 >= rects[0].y && rects[6].y + rects[6].h / 2 <= rects[0].y + rects[0].h && ballSpeedx < 0)
		{
			ballSpeedx *= -1;
			ballSpeedy += 2 * ballSpeedx * (rects[6].y + rects[6].h / 2 - (rects[0].y + rects[0].h / 2)) / rects[0].h;
		}
		if (rects[6].x <= (rects[1].x + rects[1].w) && rects[6].x + rects[6].w >= rects[1].x && rects[6].y + rects[6].h / 2 >= rects[1].y && rects[6].y + rects[6].h / 2 <= rects[1].y + rects[1].h && ballSpeedx > 0)
		{
			ballSpeedx *= -1;
			ballSpeedy += -2 * ballSpeedx * (rects[6].y + rects[6].h / 2 - (rects[1].y + rects[1].h / 2)) / rects[1].h;
		}
		//ball wall bounce
		if (rects[6].y + rects[6].h > windowy - 5 || rects[6].y < 5)
			ballSpeedy *= -1;

		//bot movement
		if (rects[1].y + rects[1].h / 2 > rects[6].y + rects[6].h / 2 && ballSpeedx > 0 && rects[1].y > 35)
			rects[1].y -= 10;
		if (rects[1].y + rects[1].h / 2 < rects[6].y + rects[6].h / 2 && ballSpeedx > 0 && rects[1].y + rects[1].h < windowy - 35)
			rects[1].y += 10;

		//player movement
		if(up)
			if (rects[0].y > 15)
				rects[0].y -= 10;
		if(down)
			if (rects[0].y + rects[0].h < windowy - 15)
				rects[0].y += 10;

		//end
		if (rects[6].x + rects[6].h > windowx || rects[6].x < 0)
			finish = true;
		else
			finish = false;

		//ball movement
		rects[6].x += ballSpeedx;
		rects[6].y += ballSpeedy;
	};
	void handleEvents() {
		SDL_Event event;
		if (SDL_PollEvent(&event))
		{
			up = down = false;
			switch (event.type)
			{
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					Running = false;
					break;
				case SDLK_UP:
					up = true;
					break;
				case SDLK_DOWN:
					down = true;
					break;
				case SDLK_SPACE:
					if (finish) {
						start();
						finish = false;
					}
				}
				break;
			case SDL_QUIT:
				Running = false;
				break;
			default:
				break;
			}
		}
	};
	void clean() {
		std::cout << "cleaning game\n";
		SDL_DestroyWindow(Window);
		SDL_DestroyRenderer(Renderer);
		SDL_Quit();
	};

	bool running() { return Running; }
};
#endif /* defined(__Game__) */