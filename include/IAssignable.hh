#include "TObject.h"
#ifndef _IAssignable_hh
#define _IAssignable_hh
namespace MyCalorimeter 
{
	class IAssignable 
	{
	public:
		//
		//	Constructor & Destructor
		//
		IAssignable () {};
		virtual ~IAssignable () {};
		//
		//	Methods
		//
		virtual void Assign(TObject & object) = 0;
	private:
		//
		//	Methods
		//
	
		//
		//	Data
		//
		/* data */
	};
} /* MyCalorimeter */
#endif
