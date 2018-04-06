#include "data_types.h"
#include <iostream>
#include <math.h>
int** facedetection(Structure3D* Image_3d){
    ad_list* ad = Image_3d->ad;
    int n = Image_3d -> points_num ;
    int** pom = (int**)malloc(40*n * sizeof(int*));
    pom[0]=(int *) malloc(sizeof(int));
    int pom_l = 1;
    int* po = (int *) malloc(2*n*sizeof(int));
    int po_s =n ;
    int po_e = n;
    for(int i =0 ; i < n  ; i++){
      for(int i1 = i+1; i1<n ; i1++){
        if(ad->edge_exists(i,i1)){
          for(int i2 = i1+1; i2 < n ; i2++){
            if(ad->edge_exists(i1,i2) && ad->edge_exists(i,i2)){
              pom[pom_l] = (int *) malloc(4 * sizeof(int));
              pom[pom_l][0] = 3 ;
              pom[pom_l][1] = i;
              pom[pom_l][2] = i1;
              pom[pom_l][3] = i2;
              pom_l ++;

            }
            else if(ad->edge_exists(i1,i2) || ad->edge_exists(i,i2)){
              po_s = n;
              po_e = n ;
                
              if(!ad->edge_exists(i,i2)){
              po[n] = i ;
              po[n+1] = i1 ;
              po[n+2] = i2 ;

              po_e += 3;
              }
              else{
                po[n] = i ;
                po[n+1] = i1;
                po[n-1] = i2;
                po_s -=1;
                po_e +=2;
              }
              
              point3D** pts3 =(point3D**)malloc(3*sizeof(point3D*)); 
              pts3[0] = Image_3d -> points[i];
              pts3[1] = Image_3d -> points[i1];
              pts3[2] = Image_3d -> points[i2];
              tr_plane* pl = new tr_plane(pts3,0,3);
              for(int i3 = i2+1 ; i3<n ; i3 ++){
                if(pl -> on_plane(Image_3d -> points[i3])){
                  if(ad ->edge_exists(po[po_s],i3) && ad ->edge_exists(po[po_e - 1],i3) ){
                    po[po_e] = i3;
                    po_e++;
                    pom[pom_l] = (int *) malloc((po_e - po_s +1)*sizeof(int)); 
                    pom[pom_l][0] = po_e -po_s ;
                    for(int a =po_s; a<po_e ; a++){
                        pom[pom_l][a -po_s + 1] = po[a];
                        //glVertex3f((Image_3d->points[po[a]] ->x_val)*zoom ,(Image_3d->points[po[a]] -> y_val)*zoom , (Image_3d->points[po[a]] -> z_val)*zoom    ); 
                    }
                    pom_l++;

                

                    //cout << "pomark :" << po_mark<<endl;
                  }
                  else if(ad ->edge_exists(po[po_s],i3) ){
                    po[po_s - 1] = i3;
                    po_s --;
                  }
                  else if(ad ->edge_exists(po[po_e - 1],i3) ){
                    po[po_e] = i3 ;
                    po_e ++;
                  }
                }
              }
            }  
          }
        }
      }
    }
    pom[0][0] = pom_l -1;
    return pom ;

}

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
void point3D::rotate( double x , double y , double z  , double ab_x , double ab_y , double ab_z){
    double pi = 3.1415926535897;
    double m =  x_val - ab_x;
    double n =  y_val - ab_y;
    double o =  z_val - ab_z;

    //about x 
    m = m ;
    n = n*cos((pi*x)/180) - o*sin((pi*x)/180);
    o = n*sin((pi*x)/180) + o*cos((pi*x)/180);

    // about y
    m =o*sin((pi*y)/180) + m*cos((pi*y)/180);
    n = n ;
    o = o*cos((pi*y)/180) -m*sin((pi*y)/180);
    //about z
    m = cos((pi*z)/180)*m -n*sin((pi*z)/180);
    n = sin((pi*z)/180)*m +n*cos((pi*z)/180);



     x_val = m + ab_x;
     y_val = n + ab_y;
     z_val = o + ab_z;
}
point3D* point3D::rotate_new(  double x , double y , double z , double ab_x , double ab_y , double ab_z){
    point3D* b = duplicate();
    b->rotate(x,y,z, ab_x , ab_y , ab_z);
    return b ;
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
void ad_list::swap_num(int a , int b){
    bool* temp ;
    temp = points[a];
    points[a] = points[b];
    points[b] = temp ;
    bool temp1 ;
    for(int i = 0 ; i<length ; i++){
        temp1 = points[i][a];
        points[i][a] = points[i][b];
        points[i][b]= temp1;
    }
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
bool tr_plane::on_plane(point3D* p){
    double m = coe[0]*p->x_val+coe[1]*p->y_val +coe[2]*p->z_val+coe[3];
    if(m == 0){
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
   //   std::cout <<" x: " <<set[i]->x_val << " y: " << set[i]->y_val << std::endl;
      if(point->x_val <= max(set[i]->x_val,set[i+1]->x_val) && point->x_val >= min(set[i]->x_val,set[i+1]->x_val) && 
          meet(point , set[i], set[i+1] )) {
        meets ++ ;  
     //   std::cout <<" x: " <<set[i]->x_val << " y: " << set[i]->y_val << "met here" <<std::endl;
       // std::cout <<" x: " <<set[i+1]->x_val << " y: " << set[i+1]->y_val << std::endl;
      } 
  }
  
  if(point->x_val <= max(set[0]->x_val,set[set_length-1]->x_val) && point->x_val >= min(set[0]->x_val,set[set_length-1]->x_val) && 
          meet(point , set[0], set[set_length -1] ))         meets ++ ;  
  if(meets == 1){a = false; 
    //std::cout << "point is inside the polygon x:"<< point->x_val <<" y:"<< point ->y_val << std::endl;
     
  }
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
  point3D* a = new point3D(t->x_val , f->x_val , s -> x_val);
  //for topview x, y are x y but for f->x_val gives y  
  points[current] = a ;
  current++ ; 
}
void Structure3D::swap(int a , int b ){
  ad -> swap_num(a , b);
  point3D* temp ;
  temp = points[a];
  points[a] = points[b];
  points[b] = temp ;
}
Structure3D* Structure3D::rotate_new(double x , double y , double z ){
  Structure3D* ans = new Structure3D(points_num);
  double x_centroid=0;
  double y_centroid=0;
  double z_centroid=0;
  for(int i =0 ; i< points_num;i++){
    x_centroid += points[i]->x_val;
    y_centroid += points[i]->y_val;
    z_centroid += points[i]->z_val;
  }
  x_centroid = x_centroid/points_num;
  y_centroid = y_centroid/points_num;
  z_centroid = z_centroid/points_num;

  point3D** ne = (point3D**) malloc(points_num * sizeof(point3D*));
  for(int i =0 ; i< points_num ; i++){
      ne[i] = points[i]->rotate_new(x,y,z,x_centroid,y_centroid,z_centroid);
  }
  ad_list* a_n = new ad_list(points_num);
  for(int i =0 ; i<points_num ; i++){
    for(int i1 =0 ; i1 <points_num ; i1++){
      a_n -> points[i][i1] = ad -> points[i][i1];
    }
  }
  (ans->points) = ne ;
  (ans->ad) = a_n ;
  return ans ;
}
