#include "PauseState.h"

#include "TextureManager.h"
#include "Game.h"
#include "MenuButton.h"
#include "InputHandler.h"
#include "MainMenuState.h"
#include "StateParser.h"

const std::string PauseState::s_pauseID = "PAUSE";

void PauseState::update()
{
    for (int i = 0; i < m_gameObjects.size(); i++)
    {
        m_gameObjects[i]->update();
    }
}

void PauseState::render()
{
    for (int i = 0; i < m_gameObjects.size(); i++)
    {
        m_gameObjects[i]->draw();
    }
}

bool PauseState::onEnter()
{
    // Parse the states file
    StateParser stateParser;
    stateParser.parseState("assets/states-config.xml", s_pauseID, &m_gameObjects, &m_textureIDList);

    m_callbacks.push_back(NULL);
    m_callbacks.push_back(s_pauseToMain);
    m_callbacks.push_back(s_resumePlay);

    setCallbacks(m_callbacks);

    std::cout << "Entering PauseState" << std::endl;
    return true;
}

bool PauseState::onExit()
{
    for (GameObject *pGameObject : m_gameObjects)
    {
        pGameObject->clean();
    }
    m_gameObjects.clear();

    // Clear the texture manager
    for (std::string textureID : m_textureIDList)
    {
        TextureManager::Instance()->clearFromTextureMap(textureID);
    }

    // reset the mouse button states to false
    InputHandler::Instance()->reset();

    std::cout << "Exiting PauseState" << std::endl;
    return true;
}

void PauseState::setCallbacks(const std::vector<Callback>& callbacks)
{
    // Go through the game objects
    for (int i = 0; i < m_gameObjects.size(); i++)
    {
        // If they are of type MenuButton, then assign a callback
        // based on the id passed in from the file
        if (dynamic_cast<MenuButton*>(m_gameObjects[i]))
        {
            MenuButton *pButton =
                dynamic_cast<MenuButton*>(m_gameObjects[i]);
            pButton->setCallback(callbacks[pButton->getCallBackID()]);
        }
    }
}

void PauseState::s_pauseToMain()
{
    Game::Instance()->getStateMachine()->changeState(new MainMenuState());
}

void PauseState::s_resumePlay()
{
    Game::Instance()->getStateMachine()->popState();
}
