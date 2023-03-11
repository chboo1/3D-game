#include <iostream>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <fstream>
#define PI 3.1415926



class Player
{
	public:
	double cx;
	double cz;
	int x1;
	int x2;
	int x3;
	int z1;
	int z2;
	int z3;
	int y;
	int rxz;
	int ry;
	int ss;
	int vy;
	Player()
	{
	}
	Player(int s)
	{
		ss = s;
		x1 = 40;
		z1 = 30;
		x2 = 40;
		z2 = 30 +ss / 100;
		x3 = 40 +ss / 50;
		z3 = 30 +ss / 200;
		rxz = 0;
		ry = 0;
		vy = 0;
		cx = 40;
		cz = 30 + ss / 50;
	}
	void init (int s)
	{
		ss = s;
		x1 = 40;
		z1 = 30;
		x2 = 40;
		z2 = 30 + ss / 100;
		x3 = 40 + ss / 50;
		z3 = 30 + ss / 200;
		rxz = 0;
		ry = 0;
		vy = 0;
		cx = 40;
		cz = 30 + ss / 50;
	}
	void getCenter()
	{
		cx = (x1 + x2) / 2;
		cz = (z1 + z2) / 2;
	}
	void setPosFromCenter()
	{
		x3 = ((double)ss / 50.0) * cos((double)rxz / 180.0 * PI) + (double)cx;
		z3 = ((double)ss / 50.0) * sin((double)rxz / 180.0 * PI) + (double)cz;
		rxz -= 90;
		rxz += 360;
		rxz %= 360;
		x1 = ((double)ss / 200.0) * cos((double)rxz / 180.0 * PI) + (double)cx;
		z1 = ((double)ss / 200.0) * sin((double)rxz / 180.0 * PI) + (double)cz;
		rxz += 180;
		rxz += 360;
		rxz %= 360;
		x2 = ((double)ss / 200.0) * cos((double)rxz / 180.0 * PI) + (double)cx;
		z2 = ((double)ss / 200.0) * sin((double)rxz / 180.0 * PI) + (double)cz;
		rxz -= 90;
		rxz += 360;
		rxz %= 360;
	}
	void moveW(int amount)
	{
		cx += (double)amount * cos((double)rxz / 180.0 * PI);
		cz += (double)amount * sin((double)rxz / 180.0 * PI);
		setPosFromCenter();
	}
	void moveS(int amount)
	{
		cx -= (double)amount * cos((double)rxz / 180.0 * PI);
		cz -= (double)amount * sin((double)rxz / 180.0 * PI);
		setPosFromCenter();
	}
	void moveA(int amount)
	{
		rotateL(90);
		cx += (double)amount * cos((double)rxz / 180.0 * PI);
		cz += (double)amount * sin((double)rxz / 180.0 * PI);
		rotateR(90);
		setPosFromCenter();
	}
	void moveD(int amount)
	{
		moveA(-amount);
	}
	void moveWX(int amount)
	{
		cx += (double)amount * cos((double)rxz / 180.0 * PI);
		setPosFromCenter();
	}
	void moveWZ(int amount)
	{
		cz += (double)amount * sin((double)rxz / 180.0 * PI);
		setPosFromCenter();
	}
	void moveAX(int amount)
	{
		rotateL(90);
		cx += (double)amount * cos((double)rxz / 180.0 * PI);
		rotateR(90);
		setPosFromCenter();
	}
	void moveAZ(int amount)
	{
		rotateL(90);
		cz += (double)amount * sin((double)rxz / 180.0 * PI);
		rotateR(90);
		setPosFromCenter();
	}
	void rotateL(int amount)
	{
		rxz -= amount;
		//rxz += 360;
		rxz %= 360;
		setPosFromCenter();
	}
	void rotateR(int amount)
	{
		rxz += amount;
		//rxz += 360;
		rxz %= 360;
		setPosFromCenter();
	}
	void rotateU(int amount)
	{
		ry += amount;
		if (ry > ss)
		{
			ry = ss;
		}
		setPosFromCenter();
	}
	void rotateD(int amount)
	{
		ry -= amount;
		if (ry < -ss)
		{
			ry = -ss;
		}
		setPosFromCenter();
	}


};


class Enemy
{
	public:
	int x = 0;
	int z = 0;
	SDL_Texture* model = nullptr;
	int rxz = 0;
	int fov = 0;
	bool sawPLayer = false;
	int chasePlayerX = -1;
	int chasePlayerY = -1;
	int renderdistance = -1;
	int lowerAngle = 361;
	int higherAngle = -1;
	void reinit(int xx, int zz, SDL_Texture * nmodel, int nfov, bool seePlayer)
	{
		x = xx;
		z = zz;
		model = nmodel;
	}
	Enemy(int xx, int zz, SDL_Texture * nmodel, int nfov, bool seePlayer)
	{
		reinit(xx, zz, nmodel, nfov, seePlayer);
	}
	Enemy()
	{
	}
	void operator*=(int input)
	{
		rxz += input;
		rxz %= 360;
	}
	void operator/=(int input)
	{
		rxz -= input;
		rxz %= 360;
	}
	~Enemy()
	{
		SDL_DestroyTexture(model);
	}
	bool checkSight(int px, int py)
	{
        return false;
	}
	bool checkCollision(int px, int py)
	{
		if (px > x - 10 && px < x + 10 && py > z - 10 && py < z + 10)
		{
			return true;
		}
		return false;
	}
	void prepRender(int distance, int angle)
	{
		renderdistance = distance;
		if (angle > higherAngle)
		{
			higherAngle = angle;
		}
		if (angle < lowerAngle)
		{
			lowerAngle = angle;
		}
	}
	void render(SDL_Renderer*renderer, int width, int height, int castfov, int prxz)
	{
		SDL_Rect temp = {width / 2 + (width / castfov) * lowerAngle, height / 2 - height / renderdistance / 2, (width / castfov) * (higherAngle - lowerAngle), height / renderdistance};
		SDL_Rect temp2 = {0, 0, 10, 30};
	}
};


class Main
{
	//Functions
	bool init();
	void gameloop();
	void closeStuff();
	SDL_Texture* loadTexture(std::string path);
	void raycast(int fov, int speed);
	int checkCollision(int px, int py);
	//SDL shit
	SDL_Window*window = NULL;
	SDL_Renderer*renderer = NULL;
	SDL_Event e;
	SDL_Texture*map = NULL;
	SDL_Surface*maps = NULL;
	SDL_Rect wallsR[20];
	SDL_Color wallcolors[20];
	int width = 1000;
	int height = 1000;
	int mx = 0;
	int my = 0;
	int wallno = 0;
	std::string walls = "";
	//bools
	bool quit = false;
	bool w = false;
	bool a = false;
	bool s = false;
	bool d = false;
	bool u = false;
	bool l = false;
	bool o = false;
	bool r = false;
	bool m = false;
	// Special stuff
	Player player;
	Enemy enemy;
	public:
	Main()
	{
		player.init(width);
		enemy.reinit(width / 2, height / 2, loadTexture("enemy.png"), 90, false);
		init();
		map = loadTexture("3D.png");
		maps = IMG_Load("3D.png");
		std::ifstream inf("rectangles.txt", std::ios::in);
		std::string temp = "";
		while (getline(inf, temp))
		{
			walls = walls + temp + '\n';
			wallno++;
		}
		int rat = 0;
		int at = 0;
		for (int i = 0; i < wallno; i++)
		{
			wallsR[i].x = 0;
			wallsR[i].y = 0;
			wallsR[i].w = 0;
			wallsR[i].h = 0;
			wallcolors[i].r = 0xff / wallno * i;
			wallcolors[i].g = 0x00;
			wallcolors[i].b = 0xff - 0xff / wallno * i;
			wallcolors[i].a = SDL_ALPHA_OPAQUE;
		}
		for (int i = 0; i < walls.length(); i++)
		{
			for (i = i; walls[i] != '\n'; i++)
			{
				int ch = (int)walls[i];
				if (ch >= 48 && ch <= 57)
				{
					switch (at)
					{
						case 0:
						wallsR[rat].x = wallsR[rat].x * 10 + ch - 48;
						break;
						case 1:
						wallsR[rat].y = wallsR[rat].y * 10 + ch - 48;
						break;
						case 2:
						wallsR[rat].w = wallsR[rat].w * 10 + ch - 48;
						break;
						case 3:
						wallsR[rat].h = wallsR[rat].h * 10 + ch - 48;
						break;
					}
				}
				else if (walls[i] == ',')
				{
					at++;
				}
			}
			wallsR[rat].w -= wallsR[rat].x - 25;
			wallsR[rat].h -= wallsR[rat].y - 25;
			at = 0;
			rat++;
		}
		for (int i = 0; i < wallno; i++)
		{
			std::cout << wallsR[i].x << ", " << wallsR[i].y << ", " << wallsR[i].w << ", " << wallsR[i].h << ", " << std::endl;
		}
		inf.close();
		while (!quit)
		{
			gameloop();
		}
		closeStuff();
	}
};


bool Main::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "Failed to initialize SDL" << std::endl;
		return false;
	}
	window = SDL_CreateWindow("Jeu 3D", 0, 0, width, height, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		std::cerr << "Failed to create window" << std::endl;
		return false;
	}
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		std::cerr << "SDL_Image didn't initialize... What an asshole..." << std::endl;
		return false;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		std::cerr << "Renderer died... F in cha          t bois..." << std::endl;
		return false;                                                    
	}
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0);
	return true;
}


SDL_Texture* Main::loadTexture(std::string path)
{
	SDL_Texture* loadedTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		std::cerr << "Failed to load image :" << path << ", Err: " << IMG_GetError();
	       return NULL;	
	}
	loadedTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
	return loadedTexture;
}


int Main::checkCollision(int px, int py)
{
	for (int i = 0; i < wallno; i++)
	{
		if ((px > wallsR[i].x && px < wallsR[i].x + wallsR[i].w && py > wallsR[i].y && py < wallsR[i].y + wallsR[i].h) || px < 0 || px > width || py < 0 || py > height)
		{
			return i;
		}
	}
	return -1;
}


void Main::raycast(int fov, int speed)
{
	double linex = 0.0;
	double liney = 0.0;
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_Rect temp = {0, player.ry / 2, width, height / 2};
	SDL_RenderFillRect(renderer, &temp);
	temp.y += temp.h;
	temp.h = height - temp.y;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &temp);
	for (int i = -fov / 2; i < fov / 2; i ++)
	{
		linex = player.cx;
		liney = player.cz;
		double distance = 1.0;
		int j = (player.rxz + i) % 360;
		int collision = checkCollision(linex, liney);
		while (collision == -1)
		{
			linex += cos((double)j / 180.0 * PI) * speed;
			liney += sin((double)j / 180.0 * PI) * speed;
			distance += 0.2;
			collision = checkCollision(linex, liney);
		}
		SDL_SetRenderDrawColor(renderer, wallcolors[collision].r, wallcolors[collision].g, wallcolors[collision].b, wallcolors[collision].a);
		//SDL_RenderDrawLine(renderer, (int)player.cx, (int)player.cz, (int)linex, (int)liney);
		//distance *= sin(((float)abs(i)) / 180.0 * PI);
		temp.x = width / 2 + (width / fov) * i;
		temp.y = height / 2 - (height / distance) / 2 + (player.ry / 2);
		temp.w = width / fov;
		temp.h = height / distance; 
		SDL_RenderFillRect(renderer, &temp);
	}
}
void Main::gameloop()
{
	int ex = 0;
	int ey = 0;
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			quit = true;
		}
		else if (e.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.sym)
			{
				case SDLK_ESCAPE:
					quit = true;
					break;
				case SDLK_w:
					w = true;
					break;
				case SDLK_a:
					a = true;
					break;
				case SDLK_s:
					s = true;
					break;
				case SDLK_d:
					d = true;
					break;
				case SDLK_UP:
					u = true;
					break;
				case SDLK_LEFT:
					l = true;
					break;
				case SDLK_DOWN:
					o = true;
					break;
				case SDLK_RIGHT:
					r = true;
					break;
				case SDLK_m:
					m = true;
					break;
				case SDLK_r:
					std::ifstream inf("rectangles.txt", std::ios::in);
					std::string temp = "";
					wallno = 0;
					walls = "";
					while (getline(inf, temp))

					{
						walls = walls + temp + '\n';
						wallno++;
					}
					int rat = 0;
					int at = 0;
					for (int i = 0; i < wallno; i++)
					{
						wallsR[i].x = 0;
						wallsR[i].y = 0;
						wallsR[i].w = 0;
						wallsR[i].h = 0;
					}
					for (int i = 0; i < walls.length(); i++)
					{
						for (i = i; walls[i] != '\n'; i++)
						{
							int ch = (int)walls[i];
							if (ch >= 48 && ch <= 57)
							{
								switch (at)
								{
									case 0:
									wallsR[rat].x = wallsR[rat].x * 10 + ch - 48;
									break;
									case 1:
									wallsR[rat].y = wallsR[rat].y * 10 + ch - 48;
									break;
									case 2:
									wallsR[rat].w = wallsR[rat].w * 10 + ch - 48;
									break;
									case 3:
									wallsR[rat].h = wallsR[rat].h * 10 + ch - 48;
									break;
								}
							}
							else if (walls[i] == ',')
							{
								at++;
							}
						}
						wallsR[rat].w -= wallsR[rat].x - 25;
						wallsR[rat].h -= wallsR[rat].y - 25;
						at = 0;
						rat++;
						std::cout << i << ", " << walls.length() << std::endl;
					}
					std::cout << "Here" << std::endl;
					for (int i = 0; i < wallno; i++)
					{
						std::cout << wallsR[i].x << ", " << wallsR[i].y << ", " << wallsR[i].w << ", " << wallsR[i].h << ", " << std::endl;
					}
					std::cout << "Here" << std::endl;
					inf.close();
					break;

			}
		}
		else if (e.type == SDL_KEYUP)
		{
			switch (e.key.keysym.sym)
			{
				case SDLK_w:
					w = false;
					break;
				case SDLK_a:
					a = false;
					break;
				case SDLK_s:
					s = false;
					break;
				case SDLK_d:
					d = false;
					break;
				case SDLK_UP:
					u = false;
					break;
				case SDLK_LEFT:
					l = false;
					break;
				case SDLK_DOWN:
					o = false;
					break;
				case SDLK_RIGHT:
					r = false;
					break;
				case SDLK_m:
					m = false;
					break;
			}
		}
		else if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			SDL_GetMouseState(&mx, &my);
			std::cout << mx << ", " << my << std::endl;
		}
		else if (e.type == SDL_MOUSEBUTTONUP)
		{
			SDL_GetMouseState(&ex, &ey);
			std::cout << mx << ", " << my << std::endl;
			std::cout << ex << ", " << ey << std::endl;
			std::ofstream of("rectangles.txt", std::ios::app | std::ios::out | std::ios::ate);
			mx -= mx % 10;
			my -= my % 10;
			ex -= ex % 10;
			ey -= ey % 10;
			of << mx << ", " << my << ", " << ex << ", " << ey << std::endl;
			of.close();
		}
	}
	if (w)
	{
		player.moveWX(1);
		if (checkCollision(player.cx, player.cz) != -1)
		{
			player.moveWX(-1);
		}
		player.moveWZ(1);
		if (checkCollision(player.cx, player.cz) != -1)
		{
			player.moveWZ(-1);
		}
	}
	if (s)
	{
		player.moveWX(-1);
		if (checkCollision(player.cx, player.cz) != -1)
		{
			player.moveWX(1);
		}
		player.moveWZ(-1);
		if (checkCollision(player.cx, player.cz) != -1)
		{
			player.moveWZ(1);
		}
	}
	if (a)
	{
		player.moveAX(1);
		if (checkCollision(player.cx, player.cz) != -1)
		{
			player.moveAX(-1);
		}
		player.moveAZ(1);
		if (checkCollision(player.cx, player.cz) != -1)
		{
			player.moveAZ(-1);
		}
	}
	if (d)
	{
		player.moveAX(-1);
		if (checkCollision(player.cx, player.cz) != -1)
		{
			player.moveAX(1);
		}
		player.moveAZ(-1);
		if (checkCollision(player.cx, player.cz) != -1)
		{
			player.moveAZ(1);
		}
	}
	if (l)
	{
		player.rotateL(1);
	}
	if (r)
	{
		player.rotateR(1);
	}
	if (u)
	{
		player.rotateU(5);
	}
	if (o)
	{
		player.rotateD(5);
	}
	std::cout << checkCollision(player.cx, player.cz) << std::endl;
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderFillRect(renderer, nullptr);
	if (m)
	{
		SDL_SetRenderDrawColor(renderer, 0xff, 0, 0, 0);
		SDL_RenderDrawLine(renderer, player.x1, player.z1, player.x2, player.z2);
		SDL_RenderDrawLine(renderer, player.x1, player.z1, player.x3, player.z3);
		SDL_RenderDrawLine(renderer, player.x3, player.z3, player.x2, player.z2);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0xff, 0);
		SDL_RenderDrawPoint(renderer, player.x1, player.z1);
		SDL_RenderDrawPoint(renderer, player.x2, player.z2);
		SDL_RenderDrawPoint(renderer, player.x3, player.z3);
		for (int i = 0; i < wallno; i++)
		{
			SDL_SetRenderDrawColor(renderer, wallcolors[i].r,wallcolors[i].g,wallcolors[i].b,wallcolors[i].a);
			SDL_RenderFillRect(renderer, &wallsR[i]);
		}
	}
	else
	{
		raycast(100, 10);
	}
	SDL_RenderPresent(renderer);
}


void Main::closeStuff()
{
	SDL_DestroyTexture(map);
	SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}


int main(int argc, char*args[])
{
	Main mainclass;
    return 0;
}


