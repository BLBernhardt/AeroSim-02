#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <map>

namespace Utility {
  
  const double PI = 3.1415926535;

  double GetMaximumKey(std::map<double, double> &samples);

  double GetMinimumKey(std::map<double, double> &samples);
  
  double NormalizeAngle(double angle);

  double Interpolate(std::map<double, double> &knownSamples, double z);

  std::string GetFileNamePart(std::string name);

};

#endif

