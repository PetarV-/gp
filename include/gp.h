/*
 Petar 'PetarV' Velickovic
 Gaussian Process (GP)
*/

#ifndef _GAUSSIAN_PROCESS
#define _GAUSSIAN_PROCESS

#include <functional>
#include <vector>

/*
 TODO: Provide a nice description of GPs
*/

class GP
{
private:
	// mean and covariance functions
	std::function<double(double)> m;
	std::function<double(double, double)> k;

	// Assumed noise hyperparameter
	double sigma;

	// number of inputs currently conditioning the posterior
	int n;

	// current training set
	vector<double> xt, yt;

	// current inverse gram matrix
	vector<vector<double>> L;

public:
	// Create a new GP with given mean and covariance function, and noise parameter
	GP(std::function<double(double)> m, std::function<double(double, double)> k, double sigma);

	// Condition the GP on a new example (x, y)
	void push(double x, double y);

	// Get the mean values at given points
	vector<double> get_means(vector<double> xs);

	// Get the covariances at all pairs of given points
	vector<vector<double>> get_covar(vector<double> xs);
};

#endif
