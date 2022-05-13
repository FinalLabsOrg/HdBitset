#pragma once
#include <map>
#include <memory>
#include <random>

/*
@todo make _t naming consistent
*/
// change these declarations if needed
namespace flhdc {

	/*
	Size of a 10k hdbitset.
	*/
	constexpr unsigned uHd10k = 64 * 157; // 64 * 157 == 10048; assuming 64 bit underlying storage

	/*
	The "MPI_Datatype datatype" used in MPI_Bcast and similar calls.
	@todo untested
	*/
	typedef int mpi_transfer_chunk_t;

	/*
	hdbitset_collection_key_t is used throughout the system.
	Must be large enough to be able to store the numeric pointer value of a 'shared pointer to hdbitset'.
	Must be signed because the sign indicates if the key is global or local (negative for local).
	We believe that the range of intptr_t is too small in case of a large number of collected values.
	*/
	typedef __int128 hdbitset_collection_key_t;

	/*
	The type of the underlying collection. 
	map or unordered_map
	Don't forget to also change the include above...
	*/
	template<typename Tcollection_key, typename Tcollected_value>
	using collection_t = std::map<Tcollection_key, Tcollected_value>;

}

// anyting below will most likely need no changes
namespace flhdc {

	/*
	Shorthand of a shared pointer to a collection template.
	Forward declaration for convenience.
	*/
	template<typename Tcollection_key, typename Tcollected_value>
	using p_collection_t = std::shared_ptr<collection_t<Tcollection_key, Tcollected_value>>;

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

	/*
	Shorthands for 10k hdbitsets & related manipulation classes.
	Forward declarations for convenience.
	*/

	template<const unsigned cuSize> class hdbitset;
	typedef hdbitset<uHd10k> hdbitset10k;

	typedef std::shared_ptr<hdbitset10k> p_hdbitset10k;

	template<const unsigned cuSize> class hdbitset_factory;
	typedef hdbitset_factory<uHd10k> hdbitset10k_factory;

	template<const unsigned cuSize> class ops;
	typedef ops<uHd10k> ops10k;

	template<const unsigned cuSize> class cops;
	typedef cops<uHd10k> cops10k;

	/*
	std::map<hdbitset_collection_key_t, p_hdbitset10k>
	*/
	typedef collection_t<hdbitset_collection_key_t, p_hdbitset10k> collection10k_t;

	/*
	std::shared_ptr<std::map<Tcollection_key, Tcollected_value>>
	*/
	typedef p_collection_t<hdbitset_collection_key_t, p_hdbitset10k> p_collection10k_t;

}