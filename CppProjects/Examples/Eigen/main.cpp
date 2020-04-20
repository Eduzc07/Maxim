#include <iostream>
#include "symbolicc++.h"
#include <Eigen/Dense>
#include <math.h>

using namespace std;
using namespace Eigen;

int main(void)
{
cout << "Start:\n" << endl;
//----------------------------------------------------------------
//Points
//----------------------------------------------------------------
 MatrixXf waypoints(2,3);
 //double Ti=d0[t_index-1];
 //Ti=3.4644;

 waypoints << 0 ,  0 , 0,
	      3 ,  3 , 3;
//              2 ,  0 , 2,
//              3 ,  -1, 1,
//             4 ,  0 , 0,
//              2 ,  1 , 3;

   cout << "waypoints:\n" << waypoints << endl;

   int n=((int)waypoints.rows()-1);

   MatrixXf d(n,3);
   d= waypoints.block(1,0,n,3)-waypoints.block(0,0,n,3);

   cout << "d:\n" << d << endl;

   VectorXf traj_time(n+1);
   traj_time.setZero(n+1);

   VectorXf d0(n+1);
   d0.setZero(n+1);
   double cusum=0;
   for (int i=0;i<n;i++){
      cout << "x: "<< d(i,0) << ", y: "<< d(i,1) <<", z: "<< d(i,2) <<endl;
      cusum=2*sqrt(pow(d(i,0),2)+pow(d(i,1),2)+pow(d(i,2),2));
      traj_time(i+1)=traj_time(i)+cusum;
      d0(i+1)=cusum;
   }
   cout << "traj_time: " << traj_time.transpose() << endl;
   cout << "d0: " << d0.transpose() << endl;


//   cout << "Points:\n" << n << endl;
//Constant[8]=(ai0,ai1,ai2,ai3,ai4,ai5,ai6,ai7) 
//----------------------------------------------------------------

   MatrixXf sol(n*8,3);
   sol.setZero(n*8,3);

//----------------------------------------------------------------
//Create Matrix B
   MatrixXf B(n*8,3);
   B.setZero(n*8,3);
   B.row(0)=waypoints.row(0);  //First point
   int m=1;
   for (int i=1;i<n;i++){
      B.row(m)=waypoints.row(i);
      B.row(m+1)=waypoints.row(i);
      m=m+2;
   }
  B.row(2*n-1)=waypoints.row(n);

//   cout << "B:\n" << B << endl;
//----------------------------------------------------------------

//----------------------------------------------------------------
//Create Matrix A
//----------------------------------------------------------------
   Symbolic a("a", 8);
   Symbolic t("t");
   Symbolic Ti("Ti");
   Symbolic ft("ft", 7);

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
               //cout << "t:"<< si <<",  Ti:" << Tsi << endl;
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
                //cout << "t:"<< si <<",  Ti:" << Tsi << endl; 
   		
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
                //cout << "t:"<< si <<",  Ti:" << Tsi << endl;  

          }
          A.row(j)=p.transpose(); 
  } 
  //cout << "A:\n" << A << endl;


  for (int i=0;i<3;i++){
  	VectorXf b(n*8);
	b.setZero(n*8);
	VectorXf sol_i(n*8); 
	sol_i.setZero(n*8);

	b=B.col(i);//i <-------------------------------------------------------------------------------
	sol_i = A.colPivHouseholderQr().solve(b);
	sol.col(i)=sol_i;
  }
  double Tif=10.3923;
  double scale =6/Tif; //(t-s0)/Ti
  VectorXf fp(8);
            fp << 1,
                 scale, 
                 scale*scale,
                 scale*scale*scale, 
                 scale*scale*scale*scale,
                 scale*scale*scale*scale*scale, 
                 scale*scale*scale*scale*scale*scale, 
                 scale*scale*scale*scale*scale*scale*scale;

//   cout << "fp:\n" << fp.transpose() << endl;

   int t_index=1;
   cout << "alphax:\n" << sol.col(0).segment(8*n-8*t_index,8) << endl;
   cout << "alphay:\n" << sol.col(1).segment(8*n-8*t_index,8) << endl;
   cout << "alphaz:\n" << sol.col(2).segment(8*n-8*t_index,8) << endl;

   double ptx=fp.transpose()*sol.col(0).segment(8*n-8*t_index,8);
   double pty=fp.transpose()*sol.col(1).segment(8*n-8*t_index,8);
   double ptz=fp.transpose()*sol.col(2).segment(8*n-8*t_index,8);
   cout << "ptx: " << ptx << endl;
   cout << "pty: " << pty << endl;
   cout << "ptz: " << ptz << endl;
  return 0;
}




