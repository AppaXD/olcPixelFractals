#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

long double posX = -0.5;
long double posY = 0;
int max = 256;
long double zoom = 0.005;
bool update = true;
long double xt;

void mandelbrot(long double& zx, long double& zy, long double& x, long double& y)
{
	xt = zx * zx - zy * zy + x;
	zy = (2 * zx * zy) + y;
	zx = xt;
}

void burningShip(long double& zx, long double& zy, long double& x, long double& y)
{
	xt = zx * zx - zy * zy + x;
	zy = abs((2 * zx * zy) + y);
	zx = abs(xt);
}

double cx = -0.7;
double cy = 0.27015;

void julia(long double& zx, long double& zy, long double& x, long double& y)
{
	xt = zx * zx - zy * zy;
	zy = 2 * zx * zy + cy;
	zx = xt + cx;
}

int current = 0; // 0 = mandelbrot, 1 = julia, 2 = burning ship
std::function<void(long double&, long double&, long double&, long double&)> fractal = mandelbrot;

class Fractal : public olc::PixelGameEngine
{
public:
	Fractal()
	{
		sAppName = "Fractals";
	}

public:
	bool OnUserCreate() override
	{
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		if (IsFocused())
		{
			if (GetKey(olc::Key::W).bPressed)
			{
				posY -= zoom * 20;
				update = true;
			}
			else if (GetKey(olc::Key::A).bPressed)
			{
				posX -= zoom * 20;
				update = true;
			}
			else if (GetKey(olc::Key::S).bPressed)
			{
				posY += zoom * 20;
				update = true;
			}
			else if (GetKey(olc::Key::D).bPressed)
			{
				posX += zoom * 20;
				update = true;
			}
			else if (GetKey(olc::Key::UP).bPressed)
			{
				zoom *= 0.9;
				update = true;
			}
			else if (GetKey(olc::Key::DOWN).bPressed)
			{
				zoom *= 1.1;
				update = true;
			}
			else if (GetKey(olc::Key::SPACE).bPressed)
			{
				switch (current)
				{
				case 0:
					fractal = julia;
					current = 1;
					break;
				case 1:
					fractal = burningShip;
					current = 2;
					break;
				case 2:
					fractal = mandelbrot;
					current = 0;
					break;
				}

				update = true;
			}
		}

		if (update == true)
		{
			Clear(olc::BLACK);
			for (int i = 0; i < ScreenWidth(); i++)
			{
				for (int j = 0; j < ScreenHeight(); j++)
				{
					int iteration = 0;
					long double x = (i - ScreenWidth() / 2.0) * zoom + posX;
					long double y = (j - ScreenHeight() / 2.0) * zoom + posY;
					long double zx = x;
					long double zy = y;
					long double xt;
					while (iteration++ < max && zx * zx + zy * zy < 4.0)
					{
						fractal(zx, zy, x, y);
					}

					if (iteration < max)
					{
						//iteration = pow((float)iteration / (float)max, 0.75) * (float)max; // for lighter image
						float modulus = sqrt(zx*zx + zy * zy);
						float mu = iteration - (log(log(modulus))) / log(2.f);
						float t = mu / max;
						float t2 = t * t;
						olc::Pixel pixel;
						pixel.b = (9 * (1 - t) * t2 * t * 255);
						pixel.g = (15 * (1 - t) * (1 - t) * t2 * 255);
						pixel.r = (8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
						Draw(i, j, pixel);
					}
				}
			}

			update = false;
		}

		return true;
	}
};

int main()
{
	Fractal fractal;
	if (fractal.Construct(800, 600, 1, 1))
		fractal.Start();

	return 0;
}
