#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <functional>
#include <random>
#include <vector>
#include <string>

#include <gauss_multi.h>
#include <gp.h>

using namespace std;
typedef unsigned int uint;

// Random engine to use throughout
default_random_engine gen;

// How many functions to sample at each step
int n_samp = 5;

void to_tikz(vector<vector<double>> X, vector<vector<double>> Y, string fname)
{
	assert(X.size() == Y.size());
	FILE *f = fopen(fname.c_str(), "w");
	
	// Define colours (TODO-someday: make this parametrised...)
	fprintf(f, "\\definecolor{mycolor0}{rgb}{0.00000,0.44700,0.74100}%%\n");
	fprintf(f, "\\definecolor{mycolor1}{rgb}{0.85000,0.32500,0.09800}%%\n");
	fprintf(f, "\\definecolor{mycolor2}{rgb}{0.92900,0.69400,0.12500}%%\n");
	fprintf(f, "\\definecolor{mycolor3}{rgb}{0.49400,0.18400,0.55600}%%\n");
	fprintf(f, "\\definecolor{mycolor4}{rgb}{0.46600,0.67400,0.18800}%%\n");

	// Begin picture
	fprintf(f, "\\begin{tikzpicture}[very thick]\n");
	// Begin axis (with all parameters)
	fprintf(f, "\\begin{axis}[width=6.028in, height=4.754in,\n");
	fprintf(f, "scale only axis, xmin=-1, xmax=1, ymin=-5.0, ymax=5.0,\n");
	fprintf(f, "axis background/.style={fill=white}]\n");

	// Add plots
	for (uint i=0;i<X.size();i++)
	{
		assert(X[i].size() == Y[i].size());
		fprintf(f, "\\addplot[color=mycolor%d, solid] table[row sep=crcr]{%%\n", i);
		for (uint j=0;j<X[i].size();j++)
		{
			fprintf(f, "%.10lf %.10lf\\\\\n", X[i][j], Y[i][j]);
		}
		fprintf(f, "};\n");
	}

	fprintf(f, "\\end{axis}\n");
	fprintf(f, "\\end{tikzpicture}\n");

	fclose(f);
}

int main()
{
	// Create a new Gaussian process... 
	// use zero-mean, squared-exponential covariance (hyperparam l = 1.0), no noise.
	auto m = [](double) { return 0; };
	auto k = [](double x, double y) { return exp(-(x - y) * (x - y) / (2.0 * 1.0)); };
	GP gp(m, k, 0.0);

	// points to be used to plot lines
	vector<double> xs;
	for (double x=-1.0;x<=1.0;x+=0.001) xs.push_back(x);

	// Sample the prior
	vector<double> mu = gp.get_means(xs);
	vector<vector<double>> Sigma = gp.get_covar(xs);

	/*for (uint i=0;i<Sigma.size();i++)
	{
		for (uint j=0;j<Sigma.size();j++)
		{
			printf("%.2lf ", Sigma[i][j]);
		}
		printf("\n");
	}*/

	MultiGaussian N(gen, mu, Sigma);

	vector<vector<double>> Xs(n_samp, xs);
	vector<vector<double>> Ys(n_samp);

	for (int i=0;i<n_samp;i++)
	{
		Ys[i] = N.sample();
	}

	to_tikz(Xs, Ys, "test.tex");

	return 0;
}
