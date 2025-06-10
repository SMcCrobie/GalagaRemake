#include "Level0.h"

#include "BackgroundManager.h"
#include "Collidable.h"
#include "Fonts.h"
#include "GameState.h"
#include "Loader.h"
#include "RandMacros.h"
#include "GameObject.h"
#include "GameObjectManager.h"


static Collidable meteor;
static sf::Sprite meteorSprite;
static sf::Texture meteorTexture;

static Ship enemyShip;
static sf::Texture enemyShipAnimations;
static RectangleProjectile bossSideKicksProjectile = RectangleProjectile(sf::Vector2f(4.f, 12.f));

extern BackgroundManager backgroundManager;
extern PlayerShip playerShip;

void Level0::initializeLevel()
{
	Loader::LOAD_SAFELY(meteorTexture, R"(Assets\Images\meteor2(1).png)");
	Loader::LOAD_SAFELY(enemyShipAnimations, R"(Assets\Images\bladeShip2(3).png)");

	level_into_text_primary = TempText("Level 0", Fonts::galaxus);
	level_into_text_secondary = TempText("Escaping Starship SaberII", Fonts::play_fair);

	level_outro_text_primary = TempText("Level Complete", Fonts::galaxus);
	level_outro_text_secondary = TempText("Escaped Starship SaberII", Fonts::play_fair);

	meteorSprite.setTexture(meteorTexture);
	meteor.setSprite(meteorSprite, "Meteor", false);
	meteor.animateOnHealth(5, sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(48, 48)));

	meteor.setHealth(5);
	meteor.setVelocity(0.3f, 1.5f);
	meteor.setRotation(1.8f);


	enemyShip.setIsHorizontallyWorldBound(false);
	enemyShip.setTexture(enemyShipAnimations);
	enemyShip.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(104, 106)));
	enemyShip.setPosition(sf::Vector2f(300.f, -50.f));
	enemyShip.setWeaponRechargeTime(20);
	enemyShip.setOrigin(enemyShip.getLocalBounds().width / 2, enemyShip.getLocalBounds().height / 2);
	enemyShip.setDeathSound(SoundType::ShipDeath);
	enemyShip.setScale(sf::Vector2f(.5f, .5f));
}

void Level0::updateLevel()
{
	extern ShipManager enemyShipsManager;
	extern GameObjectManager gameObjectManager;

	if (GameState::gameCycleCounter - GameState::timeOfLastEnemyShip <= GameState::deltaTillNextEnemyShip)
		return;
	GameState::timeOfLastEnemyShip = GameState::gameCycleCounter;

	
	if(gameObjectManager.collidablesCount() < 100)
	{
		const float xCoordinate = RANDOM_FLOAT_WITHIN_RANGE(56.F, 589.F);
		const float yVel = RANDOM_FLOAT_WITHIN_RANGE(.8f, 2.f);
		const float xVel = RANDOM_FLOAT_WITHIN_RANGE(-.5f, .5f);
		const float rotation = RANDOM_FLOAT_WITHIN_RANGE(-2.5f, 2.5f);
		const float scale = RANDOM_FLOAT_WITHIN_RANGE(.6f, 2.5f);
		const float mass = std::pow(scale * 10.6f, 1.55f);

		meteor.setRotation(rotation);
		meteor.setScale(scale, scale);
		meteor.setHealth(mass);
		meteor.setMass(mass);

		bossSideKicksProjectile.setFillColor(sf::Color(0xe97209ff));
		bossSideKicksProjectile.setVelocity(sf::Vector2f(0, -8));

		enemyShip.setProjectile1(bossSideKicksProjectile);
		enemyShip.rotate180();
		enemyShip.setHealth(2);
		
		if(GameState::gameCycleCounter % 2 == 0)
		{
			meteor.setPosition(xCoordinate, -30);
			meteor.setVelocity(xVel, yVel);
			gameObjectManager.createCollidable(meteor);
		}
		if(GameState::gameCycleCounter % 2 != 0 )
		{
			meteor.setPosition(xCoordinate, 1030);
			meteor.setVelocity(-xVel, -yVel);
			gameObjectManager.createCollidable(meteor);
		}

		/*if (GameState::killCounter < 8 && enemyShipsManager.count() < 2) {
			const float xCoord = RANDOM_FLOAT_WITHIN_RANGE(156.F, 489.F);//should make sizing dynamic
			enemyShip.setPosition(sf::Vector2f(xCoord, GameState::world_bounds.top - 50.f));
			enemyShipsManager.createShip(enemyShip);
		}*/

	}
	
}

void Level0::killLevel()
{

}

void Level0::resetLevel()
{
	
}
