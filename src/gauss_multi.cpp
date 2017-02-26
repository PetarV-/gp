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
	auto L = vector<vector<double>>(mu.size(), vector<double>(mu.size(), 0.0));
	auto D = vector<vector<double>>(mu.size(), vector<double>(mu.size(), 0.0));

	C = vector<vector<double>>(mu.size(), vector<double>(mu.size(), 0.0));

	for (uint j=0;j<mu.size();j++)
	{
		for (uint i=j;i<mu.size();i++)
		{
			double s = 0.0;
			for (uint k=0;k<j;k++)
			{
				s += L[i][k] * L[j][k] * D[k][k];
			}
			if (i == j) D[j][j] = sqrt(Sigma[j][j] - s);
			L[i][j] = (Sigma[i][j] - s) / D[j][j];
		}
	}

	for (uint i=0;i<mu.size();i++)
	{
		for (uint j=0;j<mu.size();j++)
		{
			C[i][j] = inner_product(L[i].begin(), L[i].end(), D[j].begin(), 0.0);
	//		printf("%.2lf ", L[i][j]);
		}
	//	printf("\n");
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
		ret[i] = mu[i] + inner_product(C[i].begin(), C[i].end(), z.begin(), 0.0);
	}

	return ret;
}