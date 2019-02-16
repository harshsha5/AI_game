#include "Controller.h"
#include <cmath>
#include <iostream>
#include <math.h>
namespace mrsd
{
	void Controller::control(const mrsd::Game& g, float t)
	{
		//g->tick();
		Player *player_pointer;
		Prediction pred;
		//Projectile p;
		float present_time = g.getGameTime();
		std::list<Explosion> explosion_list;
		std::list<Projectile> projectile_list;
		//map<float, Projectile> projectile_time_mapping;
		projectile_list = g.getProjectiles(); 
		explosion_list = g.getExplosions();
		for(std::list<Projectile>::iterator it = projectile_list.begin();it != projectile_list.end();)
		{
	
			pred = trackProjectile(*it,g);
			prediction_vector.push_back(pred);

		}

		determineSafeSpots(g);



	}

	void Controller::find_explosion_area(Game& g, float x)
	{
		int w = g.getWidth();
		for(int i = std::floor(x - g.explosionSize);i <= std::ceil(x + g.explosionSize);++i)
		{
			if( i >= 0 && i <= w )
				my_dangerZone[i]++;
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
		//pred.t = -2*p.vy/g.getGravity();

		float t1 = -1*p.y + pow(pow(p.vy,2) - (2*g.getGravity()*p.y),0.5);
		if(t1>0)
		{	pred.t = t1;
			pred.x = p.x + (p.vx*pred.t);
			pred.t = pred.t + g.getGameTime();
		}
		else
		{
			pred.t= -1*p.y + pow(pow(p.vy,2) + (2*g.getGravity()*p.y),0.5);
			pred.x = p.x + (p.vx*pred.t);
			pred.t = pred.t + g.getGameTime();
		}
		
		//p.predicted = 1;
		return pred;
	}

	void Controller::determineSafeSpots(const Game& g)
	{	
		int w = g.getWidth;
		my_dangerZone = new int[w+1];
		for(int i = 0; i < w+1; ++i) my_dangerZone[i] = 0;
		for(int i=0;i<prediction_vector.size();i++)
		{
			if(prediction_vector[i].t == g.getGameTime())
				{
					find_explosion_area(g,prediction_vector[i].x);
				}
		}

	}

	int Controller::pickSafeSpot(const Game& g)
	{
		
		return 0;
	}
}
