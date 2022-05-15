#pragma once

#include "input.hpp"

namespace Core::Input
{
	class KeyAxis : public Input
	{
	private:
		float m_value = 0.f;

	public:
		int m_negativeKeyID = -1;
		int m_positiveKeyID = 1;

		KeyAxis(int negativeKeyID, int positiveKeyID);

		void compute(GLFWwindow* window) override;

		float getValue() const;
	};
}