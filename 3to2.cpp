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
               s ->swap(j, j+ 1);
               swap = true;
               
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
    int** pom = facedetection(s);
    int pom_c = 1;
    array* top= new array(p_length) ; /*need not be number of 3d points*/        
    Structure2D* ans= new Structure2D(p_length);
    ans->set_ad(a);
    int t =0 ;
    double z;
	  int q = 0  ;
    
    while(t < p_length){ 
      z = sorted[t]->z_val ;
      ans->addpoint3D(sorted[t],true);
      q = t ;
      
      
      t++;
      while(t < p_length ){
        if(z != sorted[t]->z_val ){
          break ;
        }    
        ans->addpoint3D(sorted[t],true);
        t++;
              }

      while(pom_c <= pom[0][0] &&  pom[pom_c][1] <= t  ){
        for(int i = 1; i < pom[pom_c][0] + 1 ; i++){
        }  

        top->add_pl(new tr_plane(sorted , pom[pom_c]));
        pom_c++;
        if(pom_c == (pom[0][0]+1) ) break;

      }
      for(int i1 = q ; i1 < t ; i1++ ){
        for(int i2 = 0 ; i2 < t ; i2++){
            if(i1 != i2 && s -> ad -> edge_exists(i1 , i2))  {
               point3D* p = new point3D((sorted[i1]->x_val + sorted[i2]->x_val)/2 ,(sorted[i1]->y_val + sorted[i2]->y_val)/2 ,(sorted[i1]->z_val + sorted[i2]->z_val)/2   );
               if(top -> visible(p)){
                    (ans-> visi)->set_edge(i1+1,i2+1) ;
            }
          }
        }
      }
    }
    return ans ;
}
