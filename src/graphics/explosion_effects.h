#pragma once

#include <SFML/System/NonCopyable.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>
#include <cassert>


namespace sf
{
	class RenderTarget;
	class RenderTexture;
	class Shader;
}

namespace graphics {
	class PostEffect : sf::NonCopyable
	{
	public:
		virtual	~PostEffect();
		virtual void apply(const sf::RenderTexture& input, sf::RenderTarget& output) = 0;

		static bool	isSupported();


	protected:
		static void	applyShader(const sf::Shader& shader, sf::RenderTarget& output);
	};


	class Animation : public sf::Drawable, public sf::Transformable
	{
	public:
		Animation();
		explicit Animation(const sf::Texture& texture);

		void setTexture(const sf::Texture& texture) { m_sprite.setTexture(texture); }
		const sf::Texture* getTexture() const { return m_sprite.getTexture(); }

		void setFrameSize(sf::Vector2i frameSize) { m_frameSize = frameSize; }
		sf::Vector2i getFrameSize() const { return m_frameSize; }

		void setNumFrames(std::size_t numFrames) { m_numFrames = numFrames; }
		std::size_t getNumFrames() const { return m_numFrames; }

		void setDuration(sf::Time duration) { assert(duration != sf::seconds(0)); m_duration = duration; }
		sf::Time getDuration() const { return m_duration; }

		void setRepeating(bool flag) { m_repeat = flag; }
		bool isRepeating() const { return m_repeat; }

		void restart() { m_currentFrame = 0; }
		bool isFinished() const { return m_currentFrame >= m_numFrames; }

		void centerOrigin();

		sf::FloatRect getLocalBounds() const { return sf::FloatRect(getOrigin(), static_cast<sf::Vector2f>(getFrameSize())); }
		sf::FloatRect getGlobalBounds() const { return getTransform().transformRect(getLocalBounds()); }

		void update(sf::Time dt);


	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		sf::Sprite 				m_sprite;
		sf::Vector2i 			m_frameSize;
		std::size_t 			m_numFrames;
		std::size_t 			m_currentFrame;
		sf::Time 				m_duration;
		sf::Time 				m_elapsedTime;
		bool 					m_repeat;
	};
}