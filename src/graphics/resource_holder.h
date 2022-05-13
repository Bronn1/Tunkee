#pragma once

#include <memory>
#include <unordered_map>
#include <string_view>
#include <stdexcept>
#include <cassert>
#include <filesystem>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

namespace textures
{
enum class ID
{
    T34TankBody,
    T34TankTurret,
    T34TankTurretDestroyed,
    T34TankBodyDestroyed,
    Explosion,
    Missile,
    ExplodedIcon,
    GunDestroyedIcon,
    EngineDestroyedIcon,
    CrewShellshockedIcon,
    BurningIcon,
    LoaderKilledIcon,
    HiddenDamageIcon,
    MovementInfoIcon,
    ShootingInfoIcon,
    QuestionMarkIcon,
    TileGrass
};
}

namespace fonts
{
enum class ID
{
    Test
};

}

template<typename Resource>
concept ResourceType = requires(Resource resType, const std::string& filename, const sf::IntRect rect )
{
    {resType.loadFromFile(filename)} ->std::convertible_to<bool>;
};

template<typename Identifier, ResourceType Resource>
class ResourceHolder
{
public:
    //using ResourcePtr = std::unique_ptr<Resource>;
    void load(const Identifier& id, const  std::filesystem::path& filename) {
        std::unique_ptr<Resource> resource(std::make_unique<Resource>());
        if (!(*resource).loadFromFile(filename.string()))
            throw std::runtime_error("ResourceHolder::load - Failed to load " );

        (*resource).setSmooth(true);
        m_holder.insert({ id, std::move(resource) });
    }
    Resource& get(const Identifier& id) const {
        assert(m_holder.contains(id));
        return (*m_holder.at(id));
    }
    
private:
    std::unordered_map<Identifier, std::unique_ptr<Resource>> m_holder;
};

using TextureHolder = ResourceHolder<textures::ID, sf::Texture>;
using FontHolder = ResourceHolder<fonts::ID, sf::Font>;


