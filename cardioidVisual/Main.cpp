#include "olcPixelGameEngine.h"
#include <algorithm>

const double pi = 3.14159265358979323846;
const double two_pi = 2 * pi;


class CardioidPattern : public olc::PixelGameEngine
{
private:
	const float tmax = 0.04f;
	const float max = 200.0f;
	float total = max;
	float factor = 0;
	olc::vf2d center;

	float tick = tmax;

	int r;

public:
	float map(float value, float start1, float stop1, float start2, float stop2)
	{
		// prevent a divide by zero
		if (start1 == 0 and stop1 == 0) return 0.0f;

		return (value * (stop2 - start2)) / (stop1 - start1);
	}

	olc::vf2d getVector(float index)
	{
		float angle = map(fmod(index, total), 0.0f, total, 0.0f, static_cast<float>(two_pi));
		olc::vf2d v(r * cos(angle + static_cast<float>(pi)) + center.x, r * sin(angle + static_cast<float>(pi)) + center.y);

		return v;
	}

	bool OnUserCreate() override
	{
		auto screen = GetScreenSize();
		center = screen/2;
		
		r = std::min<int32_t>(screen.x,screen.y) / 2 - 20;
		
		return true;
	}

	olc::Pixel GetRandColorPixel()
	{
		return olc::Pixel(rand() % 255, rand() % 255, rand() % 255, 255);
	}

	olc::Pixel GetColor(float index)
	{
		return olc::WHITE;
		///////////////////////////////////////////////////*
		//Just switch out white for colors 
		//float result = fmod(trunc(index), 3.0f);
		//if (result == 0)
		//{
		//	return olc::Pixel(255, 0, 0, 255);
		//}
		//else if (result == 1)
		//{
		//	return olc::Pixel(0, 255, 0, 255);
		//}

		//return olc::Pixel(0, 0, 255, 255);

		///////////////////////////////////////////////////
		// or get very random
		
		// return GetRandColorPixel();
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		tick -= fElapsedTime;
		if (tick > 0) return true;

		tick = tmax;		// reset the tick
		Clear(olc::BLACK);
		
		factor += 0.01f; // pattern
		if (factor > 10.0f)
		{
			factor = 0.01f;
		}

		DrawCircle(center, r, olc::WHITE);

		for (int i = 0; i < total; i++)
		{
			//float angle = map(i, 0, total, 0, two_pi);
			auto p = getVector(static_cast<float>(i));
			FillCircle(p, 4);
		}

		for (int i = 0; i < total; i++)
		{
			auto a = getVector(static_cast<float>(i));
			auto b = getVector(i * factor);

			DrawLine(a, b, GetColor(i * factor));
		}

		
		return true;
	}
};

int main()
{
	CardioidPattern main;
	if (main.Construct(1000,1000,1,1,false, false))
	{
		main.Start();
	}

	return 0;
}