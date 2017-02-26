#include <assert.h>
#include <math.h>
#include <numeric>
#include <random>
#include <vector>

#include <gauss_multi.h>

using namespace std;
typedef unsigned int uint;

MultiGaussian::MultiGaussian(default_random_engine gen, vector<double> mu, vector<vector<double>> Sigma) : gen(gen), mu(mu)
{
	// Check dimensionalities match!
	assert(mu.size() == Sigma.size());
	assert(Sigma.size() == Sigma[0].size());

	// LDL^T-decompose Sigma = L * D * L^T
	L = vector<vector<double>>(mu.size(), vector<double>(mu.size(), 0.0));
	D = vector<double>(mu.size(), 0.0);

	for (uint i=0;i<mu.size();i++)
	{
		for (int j=i;j>=0;j--)
		{
			double s = 0.0;
			for (int k=0;k<j;k++)
			{
				s += L[i][k] * L[j][k] * D[k];
			}
			if (i == (uint)j) D[j] = Sigma[j][j] - s;
			L[i][j] = (Sigma[i][j] - s) / D[j];
		}
	}
}

vector<double> MultiGaussian::sample()
{
	normal_distribution<double> N(0.0, 1.0);

	// Compute mu + A * z, where z ~ N(0, I)
	vector<double> z(mu.size());
	for (uint i=0;i<mu.size();i++)
	{
		z[i] = N(gen);
	}

	vector<double> ret(mu.size());
	for (uint i=0;i<mu.size();i++)
	{
		ret[i] = mu[i] + sqrt(D[i]) * inner_product(L[i].begin(), L[i].end(), z.begin(), 0.0);
	}

	return ret;
}