#pragma once

#include <string>

#include <irrklang/irrklang.h>

#include "singleton.hpp"
#include "maths.hpp"

namespace Core::Engine
{
	class SoundManager final : public Singleton<SoundManager>
	{
		friend class Singleton<SoundManager>;

	private:
		SoundManager();
		~SoundManager();

		irrklang::ISoundEngine* m_soundEngine = nullptr;

	public:
		static void init();
		static void play2D(const std::string& path, bool loop = false);
		static bool isPlaying(const std::string& path);
	};
}