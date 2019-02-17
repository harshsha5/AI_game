#include "Controller.h"
#include <cmath>
#include <iostream>
#include <math.h>

namespace mrsd
{
	void Controller::control(const mrsd::Game& g, float t)
	{	if(p!=0)
		{
		//g->tick();
		//Player *player_pointer;
		Prediction pred;
		//Projectile p;
		float present_time = g.getGameTime();
		std::list<Explosion> explosion_list;
		std::list<Projectile> projectile_list;
		//map<float, Projectile> projectile_time_mapping;
		projectile_list = g.getProjectiles(); 
		explosion_list = g.getExplosions();
		prediction_vector.clear();
		for(std::list<Projectile>::iterator it = projectile_list.begin();it != projectile_list.end();it++)
		{
	
			pred = trackProjectile(*it,g);
			//std::cout<<pred.t<<","<<pred.x<<"\n";
			prediction_vector.push_back(pred);

		}

		determineSafeSpots(g);

		pickSafeSpot(g);

		// int ch;
		// std::cin>>ch;
		}

	}

	void Controller::find_explosion_area(int w, float x,float explosionSize)
	{
		//int w = g.getWidth();
		for(int i = std::floor(x - explosionSize);i <= std::ceil(x + explosionSize);++i)
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
		// pred.t = -2*p.vy/g.getGravity();
		// pred.x = p.x + (p.vx*pred.t);
		// pred.t = pred.t + g.getGameTime();


		float t1 = (-1*p.y + pow(pow(p.vy,2) - (2*g.getGravity()*p.y),0.5))/g.getGravity();
		if(t1>0)
		{	pred.t = t1;
			pred.x = p.x + (p.vx*pred.t);
			pred.t = pred.t + g.getGameTime();
		}
		else
		{
			pred.t= (-1*p.y - pow(pow(p.vy,2) - (2*g.getGravity()*p.y),0.5))/g.getGravity();
			pred.x = p.x + (p.vx*pred.t);
			pred.t = pred.t + g.getGameTime();
		}
		
		return pred;
	}

	void Controller::determineSafeSpots(const Game& g)
	{	
		int w = g.getWidth();
		float my_time_step = g.getTimeStep();
		float my_explosion_size = g.explosionSize;
		my_dangerZone = new int[w+1];
		for(int i = 0; i < w+1; ++i) my_dangerZone[i] = 0;
		for(int i=0;i<prediction_vector.size();i++)
		{
			//if(prediction_vector[i].t == g.getGameTime() + 5*my_time_step)
			if(g.getGameTime() > prediction_vector[i].t - 100*my_time_step && g.getGameTime() < prediction_vector[i].t + 100*my_time_step)
				{
					find_explosion_area(w, prediction_vector[i].x,my_explosion_size);
				}
		}

		for(int i=0;i<w+1;i++)
		{
			if(my_dangerZone[i] == 1)
				std::cout<<i<<" , ";
		}
		std::cout<<"\n";

		for(int i=0;i<prediction_vector.size();i++)
		{
			std::cout<<prediction_vector[i].x<<" , ";
		}


	}


	int Controller::pickSafeSpot(const Game& g)
	{
		std::cout<<"In pick safe spot function";
		for(int i=0;i<g.getWidth() + 1;i++)
		{
			std::cout<<my_dangerZone[i]<<",";
		}
		std::cout<<"\n";


		// std::cout<<g.getTimeStep();

		int length = g.getWidth();
		for(int i=5;i<length;i++)		//changed from 0 to 5
		{
			if(my_dangerZone[i]==0 && my_dangerZone[i-1]==0 && my_dangerZone[i+1]==0)
			{
				p->x = i;
				break;
			}
		}

		std::cout<<"Moving to safe location"<<"\t";
		std::cout<<p->x;


		return 0;
	}
}
