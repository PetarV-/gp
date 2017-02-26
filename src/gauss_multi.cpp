#include <assert.h>
#include <math.h>
#include <numeric>
#include <random>
#include <vector>

using namespace std;

MultiGaussian::MultiGaussian(default_random_engine gen, vector<double> mu, vector<vector<double>> Sigma) : gen(gen), mu(mu)
{
	// Check dimensionalities match!
	assert(mu.size() == Sigma.size());
	assert(Sigma.size() == Sigma[0].size());

	// Cholesky-decompose Sigma = A * A^T
	A = vector<vector<double>>(mu.size(), vector<double>(mu.size(), 0.0));

	for (int i=0;i<mu.size();i++)
	{
		for (int j=0;j<i+1;j++)
		{
			double s = 0.0;
			for (int k=0;k<j;k++)
			{
				s += A[i][k] * A[j][k];
			}
			A[i][j] = (i == j) ? sqrt(Sigma[i][i] - s) : (1.0 / A[j][j] * (Sigma[i][j] - s));
		}
	}
}

vector<double> MultiGaussian::sample()
{
	normal_distribution<double> N(0.0, 1.0);

	// Compute mu + A * z, where z ~ N(0, I)
	vector<double> z(mu.size());
	for (int i=0;i<mu.size();i++)
	{
		z[i] = N(gen);
	}

	vector<double> ret(mu.size());
	for (int i=0;i<mu.size();i++)
	{
		ret[i] = mu[i] + inner_product(A[i].begin(), A[i].end(), z.begin());
	}

	return ret;
}