// To Do :
// 
// 1. Set Precision of output
//    Precision depends on the resolution provided
//    Default : 0.00 (2 Digits)
//
// 2. Clean up Usage() function
//
// Author : Rahul Rajan (rahul.rajan@ansys.com)
// histo 1.1

#include <cmath>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <getopt.h>

using std::cin;     using std::cout;
using std::endl;    using std::vector;
using std::sort;    using std::domain_error;
using std::ceil;    using std::istream; 
using std::map;     using std::istringstream;
using std::string;  using std::ifstream;
using std::cerr;

istream& rd(istream&, vector<double>&, const double&, const int&);
void print_usage (void);

int main(int argc, char **argv) {

    int VERBOSE_FLAG = 0;

    int c;
    double res = 1.0;
    double mul = 1.0;
    int col = 1;
    static struct option long_options[] =
    {
        {"res",     required_argument, 0, 'r'},
        {"mul",     required_argument, 0, 'm'},
        {"col",     required_argument, 0, 'c'},
	{"verbose", no_argument,       0, 'v'}, 
        {0, 0, 0, 0}
    };
    int option_index = 0;
    while ((c = getopt_long (argc, argv, "r:m:c:v", long_options, &option_index)) != -1) {
        switch (c) {
            case 'r': {
                res = atof (optarg);
                break; }

            case 'm': {
                mul = atof(optarg);
                break;}

            case 'c': {
                col = atoi(optarg);
                break; }
	
	    case 'v':
	        VERBOSE_FLAG = 1;
		break;
            
            case '?' : {
		print_usage ();
		return (1);
                return (1);
                break;
                       }

            default: abort();
        }
    }

    typedef vector<double>::size_type vec_sz;
    vector<double> data;

    if (argc > optind) {
        ifstream in(argv[optind]);
        if (! in.is_open() ) {
            cerr << "Cannot open " << argv[optind] << endl;
            return 1;
        }
        rd (in, data, mul, col);
    } else 
        rd (cin, data, mul, col);

    vec_sz size = data.size();
    if (size == 0)
        throw domain_error ("Cannot create histogram from NULL data");
    sort (data.begin(), data.end());
    double xmin = data.front();
    double xmax = data.back();

    // Create the container for the histogram
    map<double, int> histo;
    
    // Initialise the bins. Set the bins to be 0
    for (double ii = xmin; ii <= xmax; ii += res)
        histo[ii] = 0;
    //for(map<double, int>::iterator it = histo.begin(); it != histo.end(); ++it) 
    //    cout << "[" << it->first << ", " << (it->first + res) << ") => " << it -> second << endl;


    // Binning !!
    for (vector<double>::iterator ii = data.begin(); ii != data.end(); ++ii) {
        static map<double, int>::iterator jj = histo.begin();
        if ( (*ii >= jj->first) && (*ii < jj->first+res))
            ++jj->second;
        else if ( *ii >= jj->first+res) {
            while ( *ii >= (jj->first+res) )
                ++jj;
            ++jj->second;
        }
    }

    for(map<double, int>::iterator it = histo.begin(); it != histo.end(); ++it)
	    if (VERBOSE_FLAG) 
		    cout << "[" << it->first << ", " << (it->first + res) << ") => " << it -> second << endl;
	    else
		    cout << it->first << " " << it->second << endl;

}

istream& rd(istream& in, vector<double>& x, const double& m, const int& c) {

    if (in) {
        x.clear();
        string line;
        while (getline(in, line)) {
            if (line.empty())
                continue;
            istringstream ss(line);
            for (int ii = 0; ii <= c; ++ii)
                ss >> line;
            x.push_back(atof(line.c_str()) * m);
        }
    }
    return in;
}

void print_usage (void) {
       // cout << "Usage : histo [-r | -m | -c] [file | STDIN]\n" 
       //      << endl;
	cout << "Usage : histo [OPTIONS] [file | STDIN] \n"
	     << endl
	     << "OPTIONS" << endl
	     << "-r, --res <resolution>" << endl
	     << "-m, --mul <multiplication_factor>" << endl
	     << "-c, --col <column number>" << endl
	     << "-v, --verbose" << endl ;
}
