
#include <limits>

#include "utility.h"

double Utility::GetMaximumKey(std::map<double, double> &samples)
{
  double maxValue = std::numeric_limits<double>::min();

  	for( auto &x: samples )
    {
      	if( x.first > maxValue )
		{
	  		maxValue = x.first;
		}
    }

  return maxValue;
}

double Utility::GetMinimumKey(std::map<double, double> &samples)
{

  double minValue = std::numeric_limits<double>::max();

  for(auto &x: samples)
    {
      if (x.first < minValue)
	{
	  minValue = x.first;
	}
    }

  return minValue;
}

double Utility::NormalizeAngle(double angle)
{
  if (angle < -Utility::PI)
    {
      int nPi = (int)((angle + Utility::PI) / (2 * Utility::PI)) - 1;
      return angle - 2 * nPi * Utility::PI;
    }
  else if (angle >= Utility::PI)
    {
      int nPi = (int)((angle + Utility::PI) / (2 * Utility::PI));
      return angle - 2 * nPi * Utility::PI;
    }
  else
    return angle;
};


//===============================================================================================================
#if 1
    double Utility::Interpolate( std::map<double, double> &DataPairs, double z ) 
    {
        double result;
    
        double last_x = DataPairs.begin()->first;
        double last_y = DataPairs.begin()->second;

        for( const std::pair<const double, double> &x : DataPairs ) 
        {
            if(( z >= last_x ) && ( z < x.first )) 
            {
                double factor = ( z - last_x) / ( x.first - last_x );
				result = last_y + factor * ( x.second - last_y );
                return result;
            }
            last_x = x.first;
            last_y = x.second;
        }

		if( z < DataPairs.begin()->first )  return DataPairs.begin()->second;
		else								return DataPairs.rbegin()->second;
	}
#endif


#if 0
double Utility::Interpolate( std::map<double, double> &knownSamples, double z )
{
  	double last_x = knownSamples.begin()->first;
  	double last_y = knownSamples.begin()->second;
  	
  //   printf(">>><<><>>< last_x %f  last_y %f  z %f \n", last_x, last_y, z );
      //		printf(" 3------->   knownSamples[0.2600]  = %lf \n ", knownSamples[0.2600]);   
      		
      		
  	for( auto &x : knownSamples )
  	{
      	if(( z >= last_x ) && ( z < x.first ))
		{
	  		double factor = ( z - last_x) / ( x.first - last_x );
	  		return factor * x.second + factor * ( x.second - last_y );
		}
		last_x = x.first;
		last_y = x.second;
 	}
  
  	if( z < knownSamples.begin()->first ) 	return knownSamples.begin()->second;
  	else									return knownSamples.end()->second;
}
#endif

//==================================================================================================================



std::string Utility::GetFileNamePart(std::string name)
{
  std::string result;

  return result;
}
