# pragma once
# include <cstdint>
# include <algorithm>
# include <array>
# ifdef __cpp_concepts
# if __has_include(<concepts>)
# include <concepts>
# endif
# endif
# include <iterator>
# include <numeric>
# include <random>
# include <type_traits>

namespace siv
{
# ifdef __cpp_lib_concepts
	template <std::floating_point Float>
# else
	template <class Float>
# endif
	class BasicPerlinNoise
	{
	public:

		using value_type = Float;

	private:

		std::uint8_t p[512];

		[[nodiscard]]
		static constexpr value_type Fade(value_type t) noexcept
		{
			return t * t * t * (t * (t * 6 - 15) + 10);
		}

		[[nodiscard]]
		static constexpr value_type Lerp(value_type t, value_type a, value_type b) noexcept
		{
			return a + t * (b - a);
		}

		[[nodiscard]]
		static constexpr value_type Grad(std::uint8_t hash, value_type x, value_type y, value_type z) noexcept
		{
			const std::uint8_t h = hash & 15;
			const value_type u = h < 8 ? x : y;
			const value_type v = h < 4 ? y : h == 12 || h == 14 ? x : z;
			return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
		}

		[[nodiscard]]
		static constexpr value_type Weight(std::int32_t octaves) noexcept
		{
			value_type amp = 1;
			value_type value = 0;

			for (std::int32_t i = 0; i < octaves; ++i)
			{
				value += amp;
				amp /= 2;
			}

			return value;
		}

	public:

	# if __has_cpp_attribute(nodiscard) >= 201907L
		[[nodiscard]]
	# endif
		explicit BasicPerlinNoise(std::uint32_t seed = std::default_random_engine::default_seed)
		{
			reseed(seed);
		}

	# ifdef __cpp_lib_concepts
		template <std::uniform_random_bit_generator URNG>
	# else
		template <class URNG, std::enable_if_t<!std::is_arithmetic_v<URNG>>* = nullptr>
	# endif
	# if __has_cpp_attribute(nodiscard) >= 201907L
		[[nodiscard]]
	# endif
		explicit BasicPerlinNoise(URNG&& urng)
		{
			reseed(std::forward<URNG>(urng));
		}

		void reseed(std::uint32_t seed)
		{
			for (size_t i = 0; i < 256; ++i)
			{
				p[i] = static_cast<std::uint8_t>(i);
			}

			std::shuffle(std::begin(p), std::begin(p) + 256, std::default_random_engine(seed));

			for (size_t i = 0; i < 256; ++i)
			{
				p[256 + i] = p[i];
			}
		}

	# ifdef __cpp_lib_concepts
		template <std::uniform_random_bit_generator URNG>
	# else
		template <class URNG, std::enable_if_t<!std::is_arithmetic_v<URNG>>* = nullptr>
	# endif
		void reseed(URNG&& urng)
		{
			for (size_t i = 0; i < 256; ++i)
			{
				p[i] = static_cast<std::uint8_t>(i);
			}

			std::shuffle(std::begin(p), std::begin(p) + 256, std::forward<URNG>(urng));

			for (size_t i = 0; i < 256; ++i)
			{
				p[256 + i] = p[i];
			}
		}

		///////////////////////////////////////
		//
		//	Noise [-1, 1]
		//

		[[nodiscard]]
		value_type noise2D(value_type x, value_type y) const noexcept
		{
			return noise3D(x, y, 0);
		}

		[[nodiscard]]
		value_type noise3D(value_type x, value_type y, value_type z) const noexcept
		{
			const std::int32_t X = static_cast<std::int32_t>(std::floor(x)) & 255;
			const std::int32_t Y = static_cast<std::int32_t>(std::floor(y)) & 255;
			const std::int32_t Z = static_cast<std::int32_t>(std::floor(z)) & 255;

			x -= std::floor(x);
			y -= std::floor(y);
			z -= std::floor(z);

			const value_type u = Fade(x);
			const value_type v = Fade(y);
			const value_type w = Fade(z);

			const std::int32_t A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z;
			const std::int32_t B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;

			return Lerp(w, Lerp(v, Lerp(u, Grad(p[AA], x, y, z),
				Grad(p[BA], x - 1, y, z)),
				Lerp(u, Grad(p[AB], x, y - 1, z),
				Grad(p[BB], x - 1, y - 1, z))),
				Lerp(v, Lerp(u, Grad(p[AA + 1], x, y, z - 1),
				Grad(p[BA + 1], x - 1, y, z - 1)),
				Lerp(u, Grad(p[AB + 1], x, y - 1, z - 1),
				Grad(p[BB + 1], x - 1, y - 1, z - 1))));
		}

		[[nodiscard]]
		value_type accumulatedOctaveNoise2D(value_type x, value_type y, std::int32_t octaves) const noexcept
		{
			value_type result = 0;
			value_type amp = 1;

			for (std::int32_t i = 0; i < octaves; ++i)
			{
				result += noise2D(x, y) * amp;
				x *= 2;
				y *= 2;
				amp /= 2;
			}

			return result;
		}

		[[nodiscard]]
		value_type accumulatedOctaveNoise2D_0_1(value_type x, value_type y, std::int32_t octaves) const noexcept
		{
			return std::clamp<value_type>(accumulatedOctaveNoise2D(x, y, octaves)
				* value_type(0.5) + value_type(0.5), 0, 1);
		}

	};

	using PerlinNoise = BasicPerlinNoise<double>;
}
