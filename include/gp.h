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
	int m;

	// current training set
	vector<double> x, y;

	// current inverse gram matrix
	vector<vector<double>> L;

public:
	// Create a new GP with given mean and covariance function, and noise parameter
	GP(std::function<double(double)> m, std::function<double(double, double)> k, double sigma);

	// Condition the GP on a new example (x, y)
	void push(double x, double y);

	// Sample the GP for a given point x
	double sample(double x);

	// Get the mean value for a given point x
	double get_mean(double x);

	// Get the variance for a given point x
	double get_var(double x);
};

#endif
