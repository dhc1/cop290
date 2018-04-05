#include "data_types.h"

point3D** sort_by_z(point3D** b){
return b;
  //create new set sort and return 
}
Structure2D*  generate2D(point3D** p , ad_list* a , int p_length){
    point3D** sorted = sort_by_z(p) ;
    array* top= new array(p_length) ; /*need not be number of 3d points*/        
    Structure2D* ans= new Structure2D(p_length);
    ans->set_ad(a);
    int t =0 ;
    double z;
	int i  ;
    
    while(t < p_length){ 
      z = p[t]->z_val ;
      ans->addpoint3D(sorted[t],top->visible(sorted[t]));
      i = t ;
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
  	  }
    }
    return ans ;
}
point3D* rotate(point3D* a , double x , double y , double z ){
    point3D* m = a->duplicate();
    /*m.x_val = ....;
    m.y_val = ....;
    m.z_val = ....; points roations needed */
    return m ;
}