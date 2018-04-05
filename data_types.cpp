#include "data_types.h"
#include <iostream>

point2D::point2D(double m , double n , bool q){
		x_val = m;
		y_val = n;
		visible = q ;
	}



point3D::point3D(double m , double n , double o){
    x_val = m ;
    y_val = n ;
    z_val = o ;
}

point3D* point3D::duplicate(){
    point3D* dup = new point3D(x_val,y_val,z_val);
    return dup ; 
}



ad_list::ad_list(int m){
  	length = m; 
	points = (bool **)malloc(m * sizeof(bool *));
    for (int i=0; i<m; i++)
         points[i] = (bool *)malloc(m * sizeof(bool));


    for(int i = 0 ; i< m ; i++){
      for(int i1 =0 ; i1< m ; i1++){
        points[i][i1] = false ;
      }
    }
}
bool ad_list::edge_exists(int a , int b ){
    return points[a][b];
}
void ad_list::set_edge(int a , int b ){
    points[a-1][b-1]= true ;
    points[b-1][a-1]= true ;
}



//tr_plane class  
double tr_plane::max(double m , double n){
  if(m >n){
    return m ;
  }
  else{
    return n ;
  }
}
double tr_plane::min(double m , double n){
  if(m <n){
    return m ;
  }
  else{
    return n ;
  }
}
tr_plane::tr_plane(point3D** in_order, int  start , int end  ){
    coe = (double *) malloc(4 * sizeof(double));
    point3D* p1 = in_order[0];
    point3D* p2 = in_order[1];
    point3D* p3 = in_order[2];
    coe[0] = ((p2->y_val - p1->y_val)*(p3->z_val -p1->z_val)) - ((p3->y_val - p1->y_val)*(p2->z_val -p1->z_val)) ;
       coe[1] = - ((p2->x_val - p1->x_val)*(p3->z_val -p1->z_val)) + ((p3->x_val - p1->x_val)*(p2->z_val -p1->z_val)) ;
       coe[2] = ((p2->x_val - p1->x_val)*(p3->y_val -p1->y_val)) - ((p3->x_val - p1->x_val)*(p2->y_val -p1->y_val)) ;
       coe[3] = -(p1->x_val*coe[0]) -(p1->y_val*coe[1]) -(p1->z_val*coe[2]) ;

       if (coe[2]<0){
        for(int i=0 ; i< 4;i++){
          coe[i] = -coe[i];
        }
       }
    set =(point3D **)malloc((end - start) * sizeof(point3D *));
    //commented
    for(int i =start ; i <end ;i++) {
      set[i - start] = in_order[i] ;    
    }
    set_length = end-start ;
    
    

}
bool tr_plane::above(point3D* p){
    double m = coe[0]*p->x_val+coe[1]*p->y_val +coe[2]*p->z_val+coe[3];
    if(m > 0){
      return true ;
    }
    return false ;

}
bool tr_plane::meet(point3D* point ,point3D* p1 ,point3D* p2 ){
    double  m = (p2->y_val -p1->y_val)/(p2->x_val - p1->x_val); 
    m = (point->x_val -p2->x_val)*m ;
    m = m + p2->y_val ;
    if(m <= max(p1->y_val,p2->y_val) && m >= min(p1->y_val,p2->y_val) && m > point->y_val ){
      return true ;
    }
    return false ;

}
bool tr_plane::visible(point3D* point){
  bool a = true; 
  int meets = 0 ;
  for(int i =0 ; i< set_length -1 ; i++ ){
      if(point->x_val <= max(set[i]->x_val,set[i+1]->x_val) && point->x_val >= min(set[i]->x_val,set[i+1]->x_val) && 
          meet(point , set[i], set[i+1] )) {
        meets ++ ;  
      } 
  }
  if(meets == 1){a = false;}
  return a||above(point);
} 



//array class
array::array(int m){
	i=0 ;
	j =(tr_plane **)malloc(m * sizeof(tr_plane *));
	j_length = m ;
//commented
}
void array::add_pl(tr_plane* m){
	j[i] = m ;
	i++ ;
	if(j_length  == i){
	  tr_plane** ne = (tr_plane **)malloc(2*i* sizeof(tr_plane *));
	  for(int k =0; k< i ; k++ ){
	    ne[k] = j[k];
	  }
	  delete j ;
	  j = ne ;
	  j_length = 2*i ;

	}  
}
bool array::visible(point3D* point){
	for(int k =0 ; k< i ;k++){
	    if(!j[k]->visible(point)){
	      return false ;
	    }
	}
	return true ;
}


//structure2D class
Structure2D::Structure2D(int a){
  points = (point2D**) malloc (a * sizeof(point2D *) );
  //ad= new ad_list(a);
  points_num = a;
  current = 0 ;
}
void Structure2D::set_ad(ad_list* a ){
  ad = a ;
}
void Structure2D::addpoint3D(point3D* m, bool visi){
  point2D* a = new point2D(m->x_val , m->y_val , visi); 
  points[current] = a ;
  current++ ; 
}


//Structure3D class
Structure3D::Structure3D(int a){
  points = (point3D**) malloc (a * sizeof(point3D *) );
  points_num = a;
  current = 0 ;
}
void Structure3D::set_ad(ad_list* a ){
  ad = a ;
}
void Structure3D::addpoint2D(point2D* t, point2D*f ,point2D* s){
  point3D* a = new point3D(t->x_val , f->x_val , s -> y_val);
  //for topview x, y are x y but for f->x_val gives y  
  points[current] = a ;
  current++ ; 
}
