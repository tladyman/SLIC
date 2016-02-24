#include <iostream>
#include <boost/program_options.hpp>

#include "Slic.h"

using namespace std;
namespace po = boost::program_options;

int main (int ac, char *av[]){
  string inputFile;

  // Declare the supported options.
  po::options_description desc("Allowed options");
  desc.add_options()
  ("help", "Produce this help message")
  ("input", po::value<string>(), "Input file")
  ("output", po::value<string>(), "Output folder")
  ("m", po::value<int>(), "Weighting factor between colour and spatial differences. 5-40 recommended but higher results in larger regions")
  ("k", po::value<int>(), "Number of super pixels to attempt")
  ("gui", po::value<bool>(), "Show GUI output")
  ("save", po::value<bool>(), "Save output")
  ("stages", po::value<bool>(), "Show gui/save each stage")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(ac, av, desc), vm);
  po::notify(vm);

  // Display help
  if (vm.count("help")) {
    cout << desc << "\n";
    return 1;
  }

  // Check required options are there
  if (vm.count("input")) {
    inputFile = vm["input"].as<string>();
    cout << "Input file is: "
    << inputFile << ".\n";
  } else {
    cout << "Input filename was not set.\n";
  }

  // Optional options
  int k = 500;
  int m = 10;

  bool showGUI = true;
  bool computeStages = true;
  bool save = true;
  string outputFolder = "output";

  if (vm.count("k")) {
    k = vm["k"].as<int>();
  }

  if (vm.count("m")) {
    m = vm["m"].as<int>();
  }

  if (vm.count("gui")) {
    showGUI = vm["gui"].as<bool>();
  }

  if (vm.count("stages")) {
    computeStages = vm["stages"].as<bool>();
  }

  if (vm.count("save")) {
    save = vm["save"].as<bool>();
  }

  if (vm.count("output")) {
    outputFolder = vm["output"].as<string>();
  }

  Slic slic(inputFile, outputFolder, k, m, computeStages, save, showGUI);

  cout<<"****Ending program****"<<endl;
  return 0;
}
