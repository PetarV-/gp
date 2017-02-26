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

void to_tikz(vector<vector<double>> X, vector<vector<double>> Y, vector<vector<double>> Xmss, vector<vector<double>> mss, string fname)
{
	assert(X.size() == Y.size());
	FILE *f = fopen(fname.c_str(), "w");

	fprintf(f, "\\usepgfplotslibrary{fillbetween}\n");
	
	// Define colours (TODO-someday: make this parametrised...)
	fprintf(f, "\\definecolor{mycolor0}{rgb}{0.00000,0.44700,0.74100}%%\n");
	fprintf(f, "\\definecolor{mycolor1}{rgb}{0.85000,0.32500,0.09800}%%\n");
	fprintf(f, "\\definecolor{mycolor2}{rgb}{0.92900,0.69400,0.12500}%%\n");
	fprintf(f, "\\definecolor{mycolor3}{rgb}{0.49400,0.18400,0.55600}%%\n");
	fprintf(f, "\\definecolor{mycolor4}{rgb}{0.46600,0.67400,0.18800}%%\n");

	// Begin picture
	fprintf(f, "\\begin{tikzpicture}[very thick]\n");
	// Begin axis (with all parameters)
	fprintf(f, "\\begin{axis}[very thick, width=10.028in, height=4.754in,\n");
	fprintf(f, "scale only axis, xmin=-5, xmax=5, ymin=-5, ymax=5,\n");
	fprintf(f, "axis background/.style={fill=white}]\n");

		// plot mean function
	assert(Xmss[0].size() == mss[0].size());
	fprintf(f, "\\addplot[color=black, solid] table[row sep=crcr]{%%\n");
	for (uint j=0;j<Xmss[0].size();j++)
	{
		fprintf(f, "%.10lf %.10lf\\\\\n", Xmss[0][j], mss[0][j]);
	}
	fprintf(f, "};\n");

	// plot +- two sigma
	assert(Xmss[1].size() == mss[1].size());
	fprintf(f, "\\addplot[color=gray, dashed, name path=F] table[row sep=crcr]{%%\n");
	for (uint j=0;j<Xmss[1].size();j++)
	{
		fprintf(f, "%.10lf %.10lf\\\\\n", Xmss[1][j], mss[1][j]);
	}
	fprintf(f, "};\n");

	assert(Xmss[2].size() == mss[2].size());
	fprintf(f, "\\addplot[color=gray, dashed, name path=G] table[row sep=crcr]{%%\n");
	for (uint j=0;j<Xmss[2].size();j++)
	{
		fprintf(f, "%.10lf %.10lf\\\\\n", Xmss[2][j], mss[2][j]);
	}
	fprintf(f, "};\n");

	// fill in between
	fprintf(f, "\\addplot[gray] fill between[of=F and G];\n");

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
	// use zero-mean, squared-exponential covariance (hyperparam l = 0.75), no noise.
	auto m = [](double) { return 0; };
	auto k = [](double x, double y) { return exp(-(x - y) * (x - y) / (2.0 * 0.75 * 0.75)); };
	GP gp(m, k, 0.0);

	gp.push(-3.0, 0.0);
	//gp.push(0.0, 1.0);
	gp.push(2.0, -1.0);
	//gp.push(4.0, 2.0);

	// points to be used to plot lines
	vector<double> xs;
	for (double x=-5.0;x<=5.0;x+=0.01) xs.push_back(x);

	// Sample the prior
	vector<double> mu = gp.get_means(xs);
	vector<vector<double>> Sigma = gp.get_covar(xs);

	MultiGaussian N(gen, mu, Sigma);

	vector<vector<double>> Xs(n_samp, xs);
	vector<vector<double>> Ys(n_samp);

	for (int i=0;i<n_samp;i++)
	{
		Ys[i] = N.sample();
	}

	vector<vector<double>> X(3, xs);
	vector<vector<double>> mss(3);

	for (uint i=0;i<xs.size();i++)
	{
		double mu = gp.get_means({xs[i]})[0];
		double std = sqrt(gp.get_covar({xs[i]})[0][0]);
		mss[0].push_back(mu);
		mss[1].push_back(mu - 2 * std);
		mss[2].push_back(mu + 2 * std);
	}

	to_tikz(Xs, Ys, X, mss, "test1.tex");

	// Running into numerical issues for sampling more conditioned posteriors... just plot the mean +- 2 sigma :)

	gp.push(0.0, 1.0);
	gp.push(4.0, 2.0);

	for (uint i=0;i<xs.size();i++)
	{
		double mu = gp.get_means({xs[i]})[0];
		double std = sqrt(gp.get_covar({xs[i]})[0][0]);
		mss[0][i] = mu;
		mss[1][i] = mu - 2 * std;
		mss[2][i] = mu + 2 * std;
	}

	to_tikz({}, {}, X, mss, "test2.tex");

	return 0;
}
