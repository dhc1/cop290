#include "data_types.h"
#include "3to2.h"
#include "2to3.h"
#include <iostream>
using namespace std;

int main() 
{
       int point_num = 5;
       double x_dir = 0 ;
       double y_dir = 0;
       double z_dir = 0 ;
       
       point3D** set =(point3D**)malloc(point_num * sizeof(point3D*)) ;
         set[0] = new point3D(0,0,0);
         set[1] = new point3D(1,0,0);
         set[2] = new point3D(0,1,0);
         set[3] = new point3D(1,1,0);
         set[4] = new point3D(0.5,0.5,-0.5);


      
       ad_list* ad = new ad_list(point_num);
        ad->set_edge(5, 1 );
        ad->set_edge(5, 2 );
        ad->set_edge(5, 3 );
        ad->set_edge(5, 4 );
        ad->set_edge(1, 2 );
        ad->set_edge(1, 4 );
        ad->set_edge(3, 4 );
        ad->set_edge(3, 2 );

       
       point3D** set_rot = (point3D**)malloc(point_num * sizeof(point3D*));
       for(int i = 0 ; i < point_num ; i++){
          set_rot[i] = rotate(set[i], x_dir , y_dir , z_dir ); 
       }
       delete set;
       Structure2D* ans = generate2D(set_rot,ad,point_num) ; 
       cout << ans->points[0]->visible << endl ;
       cout << ans->points[1]->visible << endl ;
       cout << ans->points[2]->visible << endl ;
       cout << ans->points[3]->visible << endl ;
       cout << ans->points[4]->visible << endl ;

       

       
       
    return 0;
}