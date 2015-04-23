#define SCALAR double

struct Boid
{
	double pos[2];
	double lastPos[2];
};

#include <SubspaceLatticeField.h>
#include <random>
#include <time.h>
#include <vector>

#include <UnitTest.h>

int main()
{
	//Boid Setup
	auto device = std::default_random_engine(time(0));
	auto engine = std::uniform_real_distribution<double>(0.0, 10.0);

	std::vector<Boid*> boids;

	for (int i = 0; i < 1000; i++)
	{
		auto boid = new Boid();
		boid->lastPos[0] = 10000; boid->lastPos[1] = 10000;
		boid->pos[0] = engine(device); boid->pos[1] = engine(device);

		boids.push_back(boid);
	}

	boids[0]->pos[0] = 5.0;
	boids[0]->pos[1] = 5.0;


	//Field Setup
	SubSpaceLatticeField<Boid*> field = SubSpaceLatticeField<Boid*>(0.0, 0.0, 10.0, 10.0, 1.0);

	field.Update(&boids);

	//Test results exist
	UnitTest::TestGreaterThan<int>([&]
	{
		std::vector<Boid*> results;

		double x = 5.0, y = 5.0;
		field.Sample(x, y, &results);

		return results.size();
	}, 0);

	UnitTest::TestEqual<int>([&]
	{
		std::vector<Boid*> results;

		double x = 50.0, y = 50.0;
		field.Sample(x, y, &results);

		return results.size();
	}, 0);

	return 0;
}