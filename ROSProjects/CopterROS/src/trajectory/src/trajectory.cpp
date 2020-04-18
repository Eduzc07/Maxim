#include "ros/ros.h"
#include "copter/Position.h"
#include <iostream>
#include "symbolicc++.h"
#include <Eigen/Dense>

using namespace Eigen;

void solve_alpha(MatrixXf&,MatrixXf,VectorXf);
	
void solve_alpha(MatrixXf &sol,MatrixXf waypoints,VectorXf d0){
 
   //double Ti=d0(1);
   int n=((int)waypoints.rows()-1);
   ROS_INFO("Points:%d, Ti:%0.4fs",n,d0(n));
   sol.setZero(n*8,3);

//----------------------------------------------------------------
//Create Matrix B
//----------------------------------------------------------------
   MatrixXf B(n*8,3);
   B.setZero(n*8,3);
   B.row(0)=waypoints.row(0);  //First point
   int m=1;
   for (int i=1;i<n;i++){
      B.row(m)=waypoints.row(i);
      B.row(m+1)=waypoints.row(i);
      m=m+2;
   }
   B.row(2*n-1)=waypoints.row(n); //Last point
//----------------------------------------------------------------

//----------------------------------------------------------------
//Create Matrix A
//----------------------------------------------------------------
   Symbolic a("a",8);
   Symbolic t("t");
   Symbolic Ti("Ti");
   Symbolic ft("ft",7);

//----------------------------------------------------------
   ft(0)=a(0)+
	 a(1)*(t/Ti)+
         a(2)*(t/Ti)*(t/Ti)+
	 a(3)*(t/Ti)*(t/Ti)*(t/Ti)+
         a(4)*(t/Ti)*(t/Ti)*(t/Ti)*(t/Ti)+ 
         a(5)*(t/Ti)*(t/Ti)*(t/Ti)*(t/Ti)*(t/Ti)+
         a(6)*(t/Ti)*(t/Ti)*(t/Ti)*(t/Ti)*(t/Ti)*(t/Ti)+
         a(7)*(t/Ti)*(t/Ti)*(t/Ti)*(t/Ti)*(t/Ti)*(t/Ti)*(t/Ti);
   for (int i=1;i<7;i++){
       ft(i)=df(ft(i-1), t);
   }

//Solution
  MatrixXf A(n*8,n*8);
  A.setZero(n*8,n*8);
  VectorXf p(n*8);

  int init;
  double si;
  double Tsi;
  int ki=n; //pi
  int df=0;  //dp,ddp,d3p=0;
  int di=1;  //dp[i](si)=dp[i+1](si);
  int vdi=n;
  int q=0;
  for (int j=0;j<n*8;j++){
	  p.setZero(n*8);
          //pi  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	  if(j<2*n){
                if(j%2 == 0){
	               //pi(si-1)
	               si=0;
                       ki=ki-1;  
		}else{
		       //pi(si) 
	               si=d0(n-ki);
		} 
                init=(8*ki);
                Tsi=d0(n-ki);
	  	for(int i=0;i<8;i++){
		     p(init+i)=(double)ft(0).coeff(a(i))[t==si,Ti==Tsi];
		}
	  }
	
          // dp(0)=dp(sn)=0 / ddp(0)=ddp(sn)=0 / d3p(0)=dp3(sn)=0
          if((j>=2*n) && (j<(2*n+6))){
		if(j%2 == 0){
	               //p1
	               si=0;
		       Tsi=d0(1);
                       ki=ki-1;  
		       init=(8*(n-1));
                       df++;
		}else{
		       //pn 
	               si=d0(n);
                       Tsi=d0(n); 
                       init=0;
		}
	  	for(int i=0;i<8;i++){
		        p(init+i)=(double)ft(df).coeff(a(i))[t==si,Ti==Tsi];
         	}
	  }

         //----------------------------------------------------------
         //Velocity p[i](k)(si)=p[i+1](k)(si)
         //Aceleration
	 //Jerk
	 //Snap .....
         //----------------------------------------------------------  
          if(j>=2*n+6){
                q++;
                vdi--;
                si=d0(q);
                Tsi=d0(q); 
                for(int i=0;i<8;i++){
 		   p(8*vdi+i)=(double)ft(di).coeff(a(i))[t==si,Ti==Tsi];
                }
                for(int i=0;i<8;i++){
		   p(8*(vdi-1)+i)=-(double)ft(di).coeff(a(i))[t==0,Ti==Tsi];
                }
 	        if(q==n-1){
                   	di++;
                   	q=0;
		   	vdi=n;  
                }
          }
         A.row(j)=p.transpose(); 
  } 
 
  for (int i=0;i<3;i++){
  	VectorXf b(n*8);
	b.setZero(n*8);
	VectorXf sol_i(n*8); 
	sol_i.setZero(n*8);
	b=B.col(i);//i <---------------------------
	sol_i = A.colPivHouseholderQr().solve(b);
	sol.col(i)=sol_i;
  }
}



int main(int argc, char **argv)
{
  ros::init(argc, argv, "points");
 // if (argc != 4)
 // {
 //   ROS_INFO("usage: X Y Z");
 //   return 1;
 // }

  ros::NodeHandle n;
  ros::Publisher position_pub = n.advertise<copter::Position>("/Position",1);

  ros::Rate loop_rate(50);

  copter::Position pos;

//  pos.pos.x = atof(argv[1]);
//  pos.pos.y = atof(argv[2]);
//  pos.pos.z = atof(argv[3]);

//----------------------------------------------------------------
//Points
//----------------------------------------------------------------

  MatrixXf waypoints(7,3);
  waypoints << 0 ,  0 , 0,
	      0 ,  0 , 2,
              2 ,  2 , 2,
              2 ,  -2, 2,
              -2 ,  2 , 2,
	      -2 , -2, 2,
	      0 , 0, 2;

//----------------------------------------------------------------
 
  int way_rows=((int)waypoints.rows()-1);

  MatrixXf d(way_rows,3);
  d= waypoints.block(1,0,way_rows,3)-waypoints.block(0,0,way_rows,3);

  VectorXf traj_time(way_rows+1);
  traj_time.setZero(way_rows+1);

  VectorXf d0(way_rows+1);
  d0.setZero(way_rows+1);
  double cusum=0;
  for (int i=0;i<way_rows;i++){
      cusum=6*sqrt(pow(d(i,0),2)+pow(d(i,1),2)+pow(d(i,2),2));
      traj_time(i+1)=traj_time(i)+cusum;
      d0(i+1)=cusum;
  }

  int t_index=0;
  double scale;

  double Ti=0;

  MatrixXf sol(way_rows*8,3);
  solve_alpha(sol,waypoints,d0);

  VectorXf fp(8);
  VectorXf dfp(8);
  VectorXf ddfp(8);

  double begin =ros::Time::now().toSec();
  double time; 

  while (ros::ok())
  //while (count<5)
  {
  ros::spinOnce();

  //Get the time and store it in the time variable.
  double now =ros::Time::now().toSec();
  time=now-begin; 

  if(time>traj_time(way_rows)){
      time =(double)traj_time(way_rows);
  }

  if (time>traj_time(t_index+1)){
     t_index++;
  }

  //t-s0
  time=time-(double)traj_time(t_index);
  

  ROS_INFO("time:%0.2fs (%0.2fs),t_index:%d,Ti:%0.03fs",now-begin,time,t_index,d0(t_index+1));
  if(time==0){
    pos.pos.x = 0;
    pos.pos.y = 0;
    pos.pos.z = 0;
  }else{
       //Generates Linear desire_state
       Ti=d0(t_index+1);
       scale = time/Ti; //(t-s0)/Ti
       for (int i=0;i<3;i++){
          //Position
          fp << 1,
                 scale, 
                 scale*scale,
                 scale*scale*scale, 
                 scale*scale*scale*scale,
                 scale*scale*scale*scale*scale, 
                 scale*scale*scale*scale*scale*scale, 
                 scale*scale*scale*scale*scale*scale*scale;
           double pt=fp.transpose()*sol.col(i).segment(8*(way_rows-1)-8*t_index,8);
          //Velocity
          dfp <<  0,
                 1*1/Ti, 
                 2*scale*1/Ti,
                 3*scale*scale*1/Ti, 
                 4*scale*scale*scale*1/Ti,
                 5*scale*scale*scale*scale*1/Ti, 
                 6*scale*scale*scale*scale*scale*1/Ti, 
                 7*scale*scale*scale*scale*scale*scale*1/Ti;
           double dp=dfp.transpose()*sol.col(i).segment(8*(way_rows-1)-8*t_index,8);
          //Aceleration
          ddfp << 0,
                  0, 
                 2*1/Ti,
                 6*scale*1/Ti, 
                 12*scale*scale*1/Ti,
                 20*scale*scale*scale*1/Ti, 
                 30*scale*scale*scale*scale*1/Ti, 
                 42*scale*scale*scale*scale*scale*1/Ti;
           double ap=ddfp.transpose()*sol.col(i).segment(8*(way_rows-1)-8*t_index,8);

	switch (i)
	{
	    case 0:
	     pos.pos.x = pt;
	     pos.vel.x = dp;
	     pos.acc.x = ap;
             break;
	    case 1:
             pos.pos.y = pt;
             pos.vel.y = dp;
             pos.acc.y = ap;
             break;
	    case 2:
             pos.pos.z = pt;
             pos.vel.y = dp;
             pos.acc.y = ap;
             break;
        }   
 
     }
  }

  position_pub.publish(pos);
  loop_rate.sleep();
 }


  return 0;
}
