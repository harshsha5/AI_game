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
		// if(g.getGameTime == g.getTimeStep)
		// {
		// 	std::vector<std::vector<int>> vec(1, std::vector<int> (g.getWidth(), 0));
		// }
		// else
		// {
		// 	std::vector<std::vector<int>> vec = explosion_time_vector; 
		// }
		


		for(std::list<Projectile>::iterator it = projectile_list.begin();it != projectile_list.end();it++)
		{
	
			pred = trackProjectile(*it,g);
			//std::cout<<pred.t<<","<<pred.x<<"\n";
			prediction_vector.push_back(pred);		//PART OF MY INITIAL IMPLEMENTATION	

		}

		determineSafeSpots(g);

		pickSafeSpot(g);

		// explosion_time_vector = vec; 
		// int c;
		// c = std::getchar();
		}

	}

	// void Controller::find_explosion_area(int w, float x,float explosionSize)
	// {
	// 	//int w = g.getWidth();
	// 	for(int i = std::floor(x - explosionSize -2);i <= std::ceil(x + explosionSize + 2);++i)
	// 	{
	// 		if( i >= 0 && i <= w )
	// 			my_dangerZone[i]++;
	// 	}
	// }

	std::vector<int> Controller::get_explosion_area(int w, float x,float explosionSize)
	{	std::vector<int> tempo;
		for(int i = std::floor(x - explosionSize);i <= std::ceil(x + explosionSize);++i)
		{
			if( i >= 0 && i <= w )
				tempo.push_back(i);
		}

		return tempo;
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


		float t1 = (-1*p.vy + pow(pow(p.vy,2) - (2*g.getGravity()*p.y),0.5))/g.getGravity();
		if(t1>0)
		{	pred.t = t1;
			pred.x = p.x + (p.vx*pred.t);
			pred.t = pred.t + g.getGameTime();
		}
		else
		{
			pred.t= (-1*p.vy - pow(pow(p.vy,2) - (2*g.getGravity()*p.y),0.5))/g.getGravity();
			pred.x = p.x + (p.vx*pred.t);
			pred.t = pred.t + g.getGameTime();
		}

		// std::cout<<"Predicted blast location "<<pred.x<<"\n"<<"Predicted time of blast "<<pred.t<<"\n"<<"Velocity_x of projectile"<<p.vx<<"\n";
		// std::cout<<"Velocity_y of projectile "<<p.vy<<"\n"<<"Present game time " << g.getGameTime()<<"\n";
		// std::cout<<"Y location " << p.y<<"\n"<<"X location "<<p.x<<"\n";
		return pred;
	}

	void Controller::determineSafeSpots(const Game& g)
	{	
		int w = g.getWidth();
		float my_time_step = g.getTimeStep();
		float my_explosion_size = g.explosionSize;
		float present_time = g.getGameTime();
		int new_temp_time;
		std::vector<int> temp;

		for(int i=0;i<prediction_vector.size();i++)
		{	
			if(g.getGameTime() > prediction_vector[i].t - 100*my_time_step)
			{
					temp = get_explosion_area(w, prediction_vector[i].x, my_explosion_size);
					for(int j=0;j<temp.size();j++)
					{
						if(counter[j]==0)
							{
								counter[j] = std::ceil(prediction_vector[i].t - present_time + 50*my_time_step);
							}

						else
							{
								new_temp_time = std::ceil(prediction_vector[i].t - present_time + 50*my_time_step);
								counter[j] = std::max(counter[j],new_temp_time);
							}

					}
			}
		}

		for(int i=0;i<counter.size();i++)
		{
			std::cout<<counter[i]<<" , ";
		}


		my_dangerZone = new int[w+1];
		for(int i = 0; i < w+1; ++i) my_dangerZone[i] = 0;

		for(int j=0;j<counter.size();j++)
		{
			if(counter[j]!=0)
				my_dangerZone++;
		}

		// for(int i=0;i<prediction_vector.size();i++)
		// {
		// 	//if(prediction_vector[i].t == g.getGameTime() + 5*my_time_step)
		// 	if(g.getGameTime() > prediction_vector[i].t - 100*my_time_step && g.getGameTime() < prediction_vector[i].t + 100*my_time_step)
		// 		{
		// 			find_explosion_area(w, prediction_vector[i].x,my_explosion_size);
		// 		}
		// }

		// for(int i=0;i<w+1;i++)
		// {
		// 	if(my_dangerZone[i] == 1)
		// 		std::cout<<i<<" , ";
		// }
		// std::cout<<"\n"<<"\n";

		// for(int i=0;i<prediction_vector.size();i++)
		// {
		// 	std::cout<<prediction_vector[i].x<<" , ";
		// }


	}


	int Controller::pickSafeSpot(const Game& g)
	{
		// std::cout<<"\n"<<"In pick safe spot function";
		// for(int i=0;i<g.getWidth() + 1;i++)
		// {
		// 	std::cout<<my_dangerZone[i]<<",";
		// }
		std::cout<<"\n"<<"\n";


		// std::cout<<g.getTimeStep();

		int length = g.getWidth();
		for(int i=5;i<length;i++)		//changed from 0 to 5
		{
			if(my_dangerZone[i]==0)
			{
				p->x = i;
				break;
			}
		}

		std::cout<<"Moving to safe location"<<"\t";
		std::cout<<p->x << "\n";


		//Decrementing counter by 1
		for(int k=0; k<counter.size();k++)
		{
			if(counter[k]!=0)
			counter[k]--;
		}

		return 0;
	}
}
