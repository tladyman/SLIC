#include <iostream>
#include <boost/program_options.hpp>

#include "Slic.h"

using namespace std;
namespace po = boost::program_options;

int main (int ac, char *av[]){
  string inputFile;
  string outputFile;
  bool saveOutput = false;

  // Declare the supported options.
  po::options_description desc("Allowed options");
  desc.add_options()
  ("help", "Produce this help message")
  ("input", po::value<string>(), "Input file")
  ("output", po::value<string>(), "Output file")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(ac, av, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    cout << desc << "\n";
    return 1;
  }

  if (vm.count("input")) {
    inputFile = vm["input"].as<string>();
    cout << "Input file is: "
    << inputFile << ".\n";
  } else {
    cout << "Input filename was not set.\n";
  }

  if (vm.count("output")) {
    outputFile = vm["output"].as<string>();
    cout << "Output file is: "
    << outputFile << ".\n";
    saveOutput = true;
  }

  // cout<<inputFile<<endl;

  Slic slic(inputFile, 1479, 1);

  cout<<"****Ending program****"<<endl;
  return 0;
}
