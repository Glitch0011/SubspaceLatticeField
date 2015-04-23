#define SCALAR double

struct Boid
{
	double pos[2];
	double lastPos[2];
};

#include <SubspaceLatticeField.h>
#include <random>
#include <vector>

int main()
{
	//Boid Setup
	auto device = std::default_random_engine();
	auto engine = std::uniform_real_distribution<double>(0.0, 10.0);

	std::vector<Boid*> boids;

	for (int i = 0; i > 1000; i++)
	{
		auto boid = new Boid();
		boid->lastPos[0] = 10000; boid->lastPos[1] = 10000;
		boid->pos[0] = engine(device); boid->pos[1] = engine(device);

		boids.push_back(boid);
	}


	//Field Setup
	SubSpaceLatticeField<Boid*> field = SubSpaceLatticeField<Boid*>(0.0, 0.0, 10.0, 10.0, 1.0);

	field.Update(&boids);

	std::vector<Boid*> results;

	double x = 5.0, y = 5.0;
	field.Sample(x, y, &results);

	return 0;
}