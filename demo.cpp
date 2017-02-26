#include <math.h>
#include <functional>
#include <random>

#include <gauss_multi.h>
#include <gp.h>

using namespace std;
typedef unsigned int uint;

// Random engine to use throughout
default_random_engine gen;

// How many functions to sample at each step
int n_samp = 5;

int main()
{
	// Create a new Gaussian process... 
	// use zero-mean, squared-exponential covariance (hyperparam l = 1.0), no noise.
	auto m = [](double) { return 0; };
	auto k = [](double x, double y) { return exp((x - y) * (x - y) / 2.0 * 1.0); };
	GP gp(m, k, 0.0);

	// points to be used to plot lines
	vector<double> xs;
	for (double x=-5.0;x<=5.0;x+=0.01) xs.push_back(x);

	// Sample the prior
	vector<double> mu = gp.get_means(xs);
	vector<vector<double>> Sigma = gp.get_covar(xs);

	MultiGaussian N(gen, mu, Sigma);

	for (int i=0;i<n_samp;i++)
	{
		vector<double> curr = N.sample();
		for (uint j=0;j<curr.size();j++)
		{
			printf("%.2lf ", curr[j]);
		}
		printf("\n");
	}

	return 0;
}
