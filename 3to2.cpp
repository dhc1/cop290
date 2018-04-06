#include "data_types.h"
#include <math.h>
#include <iostream>

double pi = 3.1415926535897;

void sort_by_z(Structure3D* s){
  int i, j;
  bool swap;
   for (i = 0; i < s->points_num-1; i++){
        swap = false ;       
       for (j = 0; j < s->points_num-i-1; j++){ 
           if (s->points[j]->z_val < s->points[j+1]->z_val){
               //std::cout << "true" <<s->points[j]->z_val << std::endl;
               //std::cout << "true" <<s->points[j+1]->z_val << std::endl;
               s ->swap(j, j+ 1);
               swap = true;
               //std::cout << "true";
               
           }
       }
       if(!swap) break;    
    }       
}
Structure2D*  generate2D(Structure3D* s){
    sort_by_z(s);
    ad_list* a = s -> ad ;
    int p_length = s -> points_num ;
    point3D** sorted = s -> points ;
    array* top= new array(p_length) ; /*need not be number of 3d points*/        
    Structure2D* ans= new Structure2D(p_length);
    ans->set_ad(a);
    int t =0 ;
    double z;
	  int i = 0  ;
    
    while(t < p_length){ 
      z = sorted[t]->z_val ;
      ans->addpoint3D(sorted[t],top->visible(sorted[t]));
      if(t-i >2)  i = t ;
      t++;
      while(t < p_length ){
        if(z != sorted[t]->z_val ){
          break ;
        }    
        ans->addpoint3D(sorted[t],top->visible(sorted[t]));
        t++;
      }    
      if(t-i >2) {
      top->add_pl(new tr_plane(sorted ,i, t ));
      /*std::cout << top -> i <<" array_length"<<std::endl;
      std::cout << top -> j[0] -> coe[0]<<std::endl;
      std::cout << top -> j[0] -> coe[1]<<std::endl;
      std::cout << top -> j[0] -> coe[2]<<std::endl;
      std::cout << top -> j[0] -> coe[3]<<std::endl;*/
      


  	  }
    }
    return ans ;
}
