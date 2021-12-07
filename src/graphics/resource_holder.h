#pragma once

#include <memory>
#include <map>
#include <string_view>
#include <stdexcept>
#include <cassert>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

namespace textures
{
	enum class ID
	{
		T34TankBody,
		T34TankTurret
	};

}

namespace fonts
{
	enum class ID
	{
		Test
	};

}

template<typename Identifier, typename Resource>
class ResourceHolder
{
public:
	//using ResourcePtr = std::unique_ptr<Resource>;
	void load(const Identifier& id,const  std::string& filename) {
		std::unique_ptr<Resource> resource(std::make_unique<Resource>());

		if (!(*resource).loadFromFile(filename.data()))
			std::runtime_error("ResourceHolder::load - Failed to load " + filename);

		(*resource).setSmooth(true);
		m_holder.insert({ id, std::move(resource) });
	}
	Resource& get(const Identifier& id) {
		assert(m_holder.contains(id));
		return (*m_holder[id]);
	}
	
private:
	// TODO unordered would be better here
	std::map<Identifier, std::unique_ptr<Resource>> m_holder;
};

using TextureHolder = ResourceHolder<textures::ID, sf::Texture>;
using FontHolder = ResourceHolder<fonts::ID, sf::Font>;

