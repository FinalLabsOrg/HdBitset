#pragma once
#include <iostream>

#if defined _OPENMP
#include <omp.h>
#endif

namespace hyperdimensional {

	class hdthread
	{
		public:
			static unsigned getMyThreadId();
	};

	/*
	Numeric thread/rank ID either from OpenMP or MPI
	*/
	inline unsigned hdthread::getMyThreadId()
	{
	#if defined _OPENMP
		return omp_get_thread_num();
	#endif
	#if defined MPI_VERSION
		int rank;
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		return rank;
	#endif
		std::cout << "Nor OpenMP nor Open MPI found. Exiting." << std::endl;
		exit(-1);
	}

}