#include "pch.h"
#include "RenderSystem.h"

#include <stdexcept>

namespace Engine
{

    RenderSystem* RenderSystem::Instance()
    {
        static RenderSystem render;
        return &render;
    }

    void RenderSystem::SetMainWindow(sf::RenderWindow* newWindow)
    {
        m_windowPtr = newWindow;
    }

    sf::RenderWindow& RenderSystem::GetMainWindow() const
    {
        if (m_windowPtr == nullptr)
        {
            throw std::runtime_error("Main window is not set");
        }

        return *m_windowPtr;
    }

    void RenderSystem::Render(const sf::Drawable& drawable)
    {
        if (m_windowPtr == nullptr)
        {
            throw std::runtime_error("Render called without a window");
        }

        m_windowPtr->draw(drawable);
    }

}