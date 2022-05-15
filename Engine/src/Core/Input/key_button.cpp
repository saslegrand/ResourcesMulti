#include "key_button.hpp"

namespace Core::Input
{
	KeyButton::KeyButton(int keyID)
		: m_keyID(keyID)
	{

	}

	void KeyButton::compute(GLFWwindow* window)
	{
		m_wasDown = m_isDown;
		m_isDown = glfwGetKey(window, m_keyID);

		// Set button parameters
		m_isPressed = !m_wasDown && m_isDown;
		m_isReleased = m_wasDown && !m_isDown;
	}

	bool KeyButton::isButtonPressed() const
	{
		return m_isPressed;
	}

	bool KeyButton::isButtonReleased() const
	{
		return m_isReleased;
	}

	bool KeyButton::isButtonDown() const
	{
		return m_isDown;
	}
}