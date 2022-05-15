#include "mouse_button.hpp"

namespace Core::Input
{
	MouseButton::MouseButton(int keyID)
		: m_keyID(keyID)
	{

	}

	void MouseButton::compute(GLFWwindow* window)
	{
		m_wasDown = m_isDown;
		m_isDown = glfwGetMouseButton(window, m_keyID);

		// Set button parameters
		m_isPressed = !m_wasDown && m_isDown;
		m_isReleased = m_wasDown && !m_isDown;
	}

	bool MouseButton::isButtonPressed() const
	{
		return m_isPressed;
	}

	bool MouseButton::isButtonReleased() const
	{
		return m_isReleased;
	}

	bool MouseButton::isButtonDown() const
	{
		return m_isDown;
	}
}