#include "cstats.h"

CStats::CStats(QListWidget *output)
{
    out = output;
}

void CStats::runUnivariateTests(CAsset a)
{
    //acf_test(a);
    fitAR(a);
}

AR1 CStats::fitAR(CAsset s)
{
    /*
    dlib::matrix<double> input(s.value.size(),1);
    for (int i = 0; i < s.value.size(); i++)
        input(i,0) = s.value[i];
    dlib::matrix<double, 2, 1> params = dlib::randm(2,1);
    //test abpve lines of code
    //string o = "lag " + to_string(i) + " ACF: " + to_string(t) + " test: " + to_string(t_ratio.back()) + "\n";
    //out->addItem(new QListWidgetItem(QString(o.c_str())));
    //f(list(i),x) must be a valid expression that evaluates to a floating point value.
    //dlib::solve_least_squares_lm(dlib::objective_delta_stop_strategy(1e-7).be_verbose(), ar.e())
    */

    //input
    vector<double> coefficients;
    vector<double> inputseries;
    int length = s.value.size();
    int degree = 2;

    //subtract mean
    double m = mean(s);
    for (int i = 0; i < length; i++)
        inputseries.push_back(s.value[i]-m);

    double **ar = new double*[degree+1];
    for (int i = 0; i < degree+1; i++)
        ar[i] = new double[degree+1];

    double *per = new double[length+1];
    double *pef = new double[length+1];
    double *h = new double[degree+1];
    double *g = new double[degree+2];
    int j,n,nn,jj;
    double sn,sd;
    double t1,t2;

    for (j=1;j<=length;j++) {
        pef[j] = 0;
        per[j] = 0;
    }

    for (nn=2;nn<=degree+1;nn++)
    {
        n  = nn - 2;
        sn = 0.0;
        sd = 0.0;
        jj = length - n - 1;
        for (j=1;j<=jj;j++) {
            t1 = inputseries[j+n] + pef[j];
            t2 = inputseries[j-1] + per[j];
            sn -= 2.0 * t1 * t2;
            sd += (t1 * t1) + (t2 * t2);
        }
        g[nn] = sn / sd;
        t1 = g[nn];
        if (n != 0) {
            for (j=2;j<nn;j++)
                h[j] = g[j] + (t1 * g[n - j + 3]);
            for (j=2;j<nn;j++)
                g[j] = h[j];
            jj--;
        }
        for (j=1;j<=jj;j++) {
            per[j] += (t1 * pef[j]) + (t1 * inputseries[j+nn-2]);
            pef[j]  = pef[j+1] + (t1 * per[j+1]) + (t1 * inputseries[j]);
        }

        for (j=2;j<=nn;j++)
            ar[nn-1][j-1] = g[j];
    }

    string o = "AR(2) model parameters: ";
    for (int i=1;i<=degree;i++)
    {
        coefficients.push_back(-ar[degree][i]);
        o += to_string(coefficients.back()) + ", ";
    }
    o += "\n";

    out->addItem(new QListWidgetItem(QString(o.c_str())));

    for (int i = 0; i < degree+1; i++)
        delete ar[i];
    delete ar;
    delete per, pef, h, g;

    return AR1();
}

double CStats::lag_acf(CAsset s, int lag)
{
    CAsset a, lag_a;
    for (int i = lag; i < s.value.size(); i++)
        a.value.push_back(s.value[i]);

    for (int i = 0; i < s.value.size() - lag; i++)
        lag_a.value.push_back(s.value[i]);

    double acf = cov(a, lag_a) / var(s);

    return acf;
}

vector<double> CStats::acf(CAsset s, int max_lag)
{
    vector<double> acf;
    for (int i = 0; i < max_lag; i++)
        acf.push_back(lag_acf(s, i));

    return acf;
}

//test statistic
//H0: lag-l autocorrelation = 0
//Ha: else
//reject H0 if |t ratio| > 1.96
//rejection means lag is significant
vector<double> CStats::acf_test(CAsset s, int max_lag)
{
    double t = 0;
    vector<double> t_ratio;
    for (int i = 0; i < max_lag; i++)
    {
        double test = 0;

        for (int j = 0; j < i; j++)
            test += pow(lag_acf(s, j),2);
        test = (1+2*test)/s.value.size();

        t = lag_acf(s, i);

        t_ratio.push_back(t/sqrt(test));

        string o = "lag " + to_string(i) + " ACF: " + to_string(t) + " test: " + to_string(t_ratio.back()) + "\n";
        out->addItem(new QListWidgetItem(QString(o.c_str())));
    }
    return t_ratio;
}

double CStats::corr(CAsset a, CAsset b)
{
    return (cov(a,b) / sqrt(var(a) * var(b)));
}

double CStats::cov(CAsset a, CAsset b)
{
    double m1 = mean(a);
    double m2 = mean(b);
    double c = 0;
    double size = a.value.size();
    int i = 0;
    for (i; i < size; i++)
        c += (a.value[i]-m1) * (b.value[i]-m2);
    return c/i;
}

double CStats::var(CAsset s)
{
    double m = mean(s);
    double v = 0;
    int i = 0;
    for (i; i < s.value.size(); i++)
        v += pow(s.value[i]-m, 2);
    return v/i;
}


double CStats::mean(CAsset s)
{
    double m = 0;
    int i = 0;
    for (i; i < s.value.size(); i++)
        m += s.value[i];
    return m/i;
}
