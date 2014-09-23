#ifndef _MathOperator_hh_
#define _MathOperator_hh_

#include <vector>
using std::vector;
namespace MyCalorimeter
{
	class MathOperator 
	{
		public:
		//
		//	Constants
		//
	
		//
		//	Constructors
		//
			MathOperator ();
			virtual ~MathOperator ();
		//
		//	Methods
		//
			static float getModule(vector< int > & v);
			static float getModule(vector< float > & v);
			static bool approximatelyEqual(const double * start1, const double * end);
			static float getDistance(const double * start, const double * end);
			static vector< float > * vectorProduct(vector< float > & v1, vector< float > & v2);
			static float getDistanceTo(vector< int > & vectorPoint1, vector< float > & vector1, vector< int > * pointOfLine );
			static float getDistanceTo(const double * vectorPoint1, vector< float > & vector1,const double * pointOfLine );
			static vector< float > getDirection(vector< int > & vectorPoint1, vector< int > & vectorPoint2);
			static vector< float > getAngles(vector< float > & direction);
			static vector< float > getDirection(const double * vectorPoint1, const double * vectorPoint2);
		private:
		//
		//	Data
		//
			/* data */
		//
		//	Private methods
		//
			static double * castIntToDouble(int * array);
	};
}
#endif
