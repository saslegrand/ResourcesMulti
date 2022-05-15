#pragma once

#include "singleton.hpp"

namespace Core::Engine
{
	class EngineMaster final : public Singleton<EngineMaster>
	{
		friend class Singleton<EngineMaster>;

	private:
		EngineMaster();
		~EngineMaster();
		
		bool editMode = false;
		bool lockEdit = false;

	public:
		static void toggleEditMode();
		static void lockEditMode(bool isLock);
		static void update();
		static void init();
	};
}