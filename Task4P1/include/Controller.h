#pragma once
#include "Player.h"
#include "Game.h"
#include <map>
#include <unordered_map>
#include <vector>
namespace mrsd
{
	struct Prediction
	{
		float t, x;
	};
	class Controller
	{
		public:
			Controller():p(0){}
			/**
			 * Control AI based on state of game. "t" time has passed since last update.
			 **/
			void control(const Game& g, float t);
			/*
			 * Construct Player and add to game.
			 */
			void createPlayer(Game& g);
			//MY MODIFICATIONS
			//std::unordered_map<float, Prediction> prediction_time_mapping;
			std::vector<Prediction> prediction_vector; //Should be improved to ordered set
			int *my_dangerZone;
			std::vector<int> counter{std::vector<int>(201,0)};
			//void print_details(const Game& g);
			std::vector<int> get_explosion_area(int w, float x,float explosionSize);
		private:
			Player* p;
			float currentTime;
			/*
			 * Track projectile and determine where and when it will hit ground.
			 */
			Prediction trackProjectile(const Projectile& p, const Game& g);

			/*
			 * Determine safe spots at time t based on state of all observed projectiles.
			 */
			void determineSafeSpots(const Game& g);

			/*
			 * Pick safe spot from determined safe spots.
			 */
			int pickSafeSpot(const Game& g);
	};
}
