#pragma once
#include <map>
#include <memory>
#include <random>


/*
@todo test these / check which tests are missing
*/
/*
Constants and type definitions
Please change as needed.
*/

namespace flhdc {

	/*
	Precise size of a 10k hdbitset.
	*/
	constexpr unsigned cuHd10k = 64 * 157; // 64 * 157 == 10048; assuming 64 bit underlying storage

	/*
	The type of the underlying collection.
	It is either a map or unordered_map
	Don't forget to also change the include above...
	*/
	template<typename Tcollection_key, typename Tcollected_value>
	using collection_t = std::map<Tcollection_key, Tcollected_value>;

	/*
	hdbitset_collection_key_t is used throughout the system.
	Must be large enough to be able to store the numeric pointer value of a 'shared pointer to hdbitset'.
	Must be signed because the sign indicates if the key is global or local (negative for local).
	We believe that the range of intptr_t is too small in case of a large number of collected values.
	*/
	typedef __int128 hdbitset_collection_key_t;

	/*
	The "MPI_Datatype datatype" used in MPI_Bcast and similar calls.
	@todo untested
	*/
	typedef int mpi_transfer_chunk_t;

}



/*
Shorthands for 10k hdbitsets & related manipulation classes.
Manipulation classes are used directly through their static methods and thus are not named '_t'.
Forward declarations for convenience.
No changes are needed (most likely).
*/

namespace flhdc {

	template<const unsigned cuSize> class hdbitset;
	template<const unsigned cuSize>
	using p_hdbitset_t = std::shared_ptr<hdbitset<cuSize>>;

	template<const unsigned cuSize> class hdbitset;
	typedef hdbitset<cuHd10k> hdbitset10k_t;

	typedef p_hdbitset_t<cuHd10k> p_hdbitset10k_t;

	template<const unsigned cuSize> class hdbitset_factory;
	typedef hdbitset_factory<cuHd10k> hdbitset10k_factory;

	template<const unsigned cuSize> class ops;
	typedef ops<cuHd10k> ops10k;

	template<const unsigned cuSize> class cops;
	typedef cops<cuHd10k> cops10k;

}



/*
Shorthands for collections.
Forward declaration for convenience.
No changes are needed (most likely).
*/

namespace flhdc {

	/*
	Shared pointer to a collection.
	*/
	template<typename Tcollection_key, typename Tcollected_value>
	using p_collection_t = std::shared_ptr<collection_t<Tcollection_key, Tcollected_value>>;

	/*
	std::map<hdbitset_collection_key_t, p_hdbitset10k>
	std::unordered_map<hdbitset_collection_key_t, p_hdbitset10k>
	*/
	typedef collection_t<hdbitset_collection_key_t, p_hdbitset10k_t> collection10k_t;

	/*
	std::shared_ptr<std::map<Tcollection_key, Tcollected_value>>
	std::shared_ptr<std::unordered_map<Tcollection_key, Tcollected_value>>
	*/
	typedef p_collection_t<hdbitset_collection_key_t, p_hdbitset10k_t> p_collection10k_t;

}



/*
Auxiliary shorthands.
No changes are needed (most likely).
*/

namespace flhdc {

	/*
	Shorthand of a shared pointer to a randengine.
	Forward declaration for convenience.
	*/
	class randengine;
	typedef std::shared_ptr<randengine> p_randengine_t;

	/*
	Shorthand for a shared pointer to the mersenne twister
	*/
	typedef std::shared_ptr<std::mt19937> p_mt19937_t;

}