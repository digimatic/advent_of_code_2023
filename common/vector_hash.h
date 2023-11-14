#include <functional>
#include <vector>

namespace westerstrom
{
	namespace HashCombine
	{
		// Code from boost
		// Reciprocal of the golden ratio helps spread entropy
		//     and handles duplicates.
		// See Mike Seymour in magic-numbers-in-boosthash-combine:
		//     https://stackoverflow.com/questions/4948780

		template <class T> inline void hash_combine(std::size_t& seed, T const& v)
		{
			seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}
	} // namespace HashCombine

	struct VectorHash
	{
		template <class VecElem>
		std::size_t operator()(const std::vector<VecElem>& vec) const noexcept
		{
			size_t seed = 0;
			for(auto& v : vec)
			{
				auto strhash = std::hash<VecElem>{}(v);
				HashCombine::hash_combine(seed, strhash);
			}
			return seed;
		}
	};

} // namespace westerstrom
