#include "GameObjectFactory.h"

GameObjectFactory* GameObjectFactory::s_pInstance = NULL;

bool GameObjectFactory::registerType(std::string typeID, BaseCreator* pCreator)
{
    std::map<std::string, BaseCreator*>::iterator it =
        m_creators.find(typeID);

    // if the type is already registered, then do nothing
    if (it != m_creators.end())
    {
        delete pCreator;
        return false;
    }

    m_creators[typeID] = pCreator;
    return true;
}

GameObject* GameObjectFactory::create(std::string typeID)
{
    std::map<std::string, BaseCreator*>::iterator it =
        m_creators.find(typeID);

    if (it == m_creators.end())
    {
        std::cout << "Could not find type: " << typeID << std::endl;
        return NULL;
    }

    BaseCreator* pCreator = (*it).second;
    return pCreator->createGameObject();
}
