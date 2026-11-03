#include "pch.h"
#include "RenderSystem.h"

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
        return *m_windowPtr;
    }

    void RenderSystem::Render(const sf::Drawable& drawable)
    {
        m_windowPtr->draw(drawable);
    }

}