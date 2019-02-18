#include "Controller.h"
#include <cmath>
#include <iostream>
#include <math.h>

namespace mrsd
{
	void Controller::control(const mrsd::Game& g, float t)
	{	if(p!=0)
		{
			
		Prediction pred;
		float present_time = g.getGameTime();
		std::list<Explosion> explosion_list;
		std::list<Projectile> projectile_list;
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

		// int c;
		// c = std::getchar();
		}

	}

	std::vector<int> Controller::get_explosion_area(int w, float x,float explosionSize)
	{	std::vector<int> tempo;
		for(int i = std::floor(x - explosionSize);i <= std::ceil(x + explosionSize);++i)
		{
			if( i >= 0 && i <= w )
				{
					tempo.push_back(i);
				}
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
			if(g.getGameTime() > prediction_vector[i].t - 50*my_time_step)
			{
					temp = get_explosion_area(w, prediction_vector[i].x, my_explosion_size);
					for(int j=0;j<temp.size();j++)
					{
						if(counter[temp[j]]==0)
							{
								counter[temp[j]] = std::ceil(100*(prediction_vector[i].t - present_time + 50*my_time_step));
							}

						else
							{
								new_temp_time = std::ceil(100*(prediction_vector[i].t - present_time + 50*my_time_step));
								counter[temp[j]] = std::max(counter[j],new_temp_time);
							}

					}
			}
		}


		my_dangerZone = new int[w+1];
		for(int i = 0; i < w+1; ++i) my_dangerZone[i] = 0;

		for(int j=0;j<counter.size();j++)
		{
			if(counter[j]!=0)
				my_dangerZone[j]++;
		}


	}


	int Controller::pickSafeSpot(const Game& g)
	{
		// std::cout<<"\n"<<"In pick safe spot function DANGER_ZONE"<<"\n"<<"\n";
		// for(int i=0;i<g.getWidth() + 1;i++)
		// {
		// 	std::cout<<my_dangerZone[i]<<",";
		// }
		// std::cout<<"\n"<<"\n";

		// std::cout<<"\n"<<"COUNTER"<<"\n"<<"\n";
		// for(int i=0;i<counter.size();i++)
		// {
		// 	std::cout<<counter[i]<<",";
		// }
		// std::cout<<"\n"<<"\n";

		float time_step= g.getTimeStep();

		// std::cout<<g.getTimeStep();


		//MOVING THE PLAYER
		if(my_dangerZone[int(p->x)]==0)
			{
				
			}
		else
		{
			int closest_left_spot;
			int closest_right_spot;
			int point_to_move;
			for(int i = int(p->x);i>=0;i--)
				{
					if(my_dangerZone[i]==0)
					{
						closest_left_spot = i;
						break;
					}
				}

			for(int i = int(p->x);i>=g.getWidth();i++)
				{
					if(my_dangerZone[i]==0)
					{
						closest_right_spot = i;
						break;
					}
				}

			point_to_move = std::min(std::abs(p->x - closest_left_spot),std::abs(p->x - closest_right_spot));

			if(std::abs(p->x - closest_left_spot) < std::abs(p->x - closest_right_spot))
			{
				p->x = p->x - g.playerSpeed;
			}

			else
			{
				p->x = p->x + g.playerSpeed;
			}
			


		}
		
		//Decrementing counter by 100*time_step
		for(int k=0; k<counter.size();k++)
		{
			if(counter[k]>0)
				counter[k] = counter[k] - int((100*time_step));
			else
			{
				counter[k] = 0;
			}
			
		}

		return 0;
	}
}
