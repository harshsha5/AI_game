#include "Controller.h"
#include <cmath>
#include <iostream>
namespace mrsd
{
	void Controller::control(const mrsd::Game& g, float t)
	{
		//g->tick();
		Player *player_pointer;
		Projectile p;
		std::list<Explosion> explosion_list;
		std::list<Projectile> projectile_list;
		map<float, int> exlosion_time_mapping;
		projectile_list = g.getProjectiles(); 
		explosion_list = g.getExplosions();
		for(std::list<Projectile>::iterator it = projectile_list.begin();it != projectile_list.end();)
		{
			
		}



	}

	void Controller::createPlayer(Game& g)
	{
		if(p == 0)
		{
			p = new Player();
			p->dead = true;
		}
		if(p->dead)
		{
			p->dead = false;
			p->x = g.getWidth()/2;
			g.newPlayer(p);
		}
	}

	Prediction Controller::trackProjectile(const Projectile& p, const Game& g)
	{
		Prediction pred;
		pred.t = -2*p.vy/g.getGravity();
		pred.x = p.x + (p.vx*pred.t);
		pred.t = pred.t + g.getGameTime();
		return pred;
	}

	void Controller::determineSafeSpots(const Game& g)
	{
	}

	int Controller::pickSafeSpot(const Game& g)
	{
		return 0;
	}
}
