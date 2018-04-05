class point2D{
public:
	double x_val,y_val ;
	bool visible;
	point2D(double m , double n , bool q);
	
};

class point3D{
public:	
  double x_val,y_val,z_val;
  point3D(double m , double n , double o);
  point3D* duplicate();
};

class ad_list{
public:
  bool** points ;
  int length ;
  ad_list(int m);
  bool edge_exists(int a , int b );
  void set_edge(int a , int b );
  
  
};
class tr_plane {
public:
    double* coe ;//with fixed z coeff sign
    point3D** set ;
    int set_length ;
    static double max(double m , double n);
      
    
    static double min(double m , double n);
      
    
    tr_plane(point3D** in_order, int  start , int end  );
        

    
    bool above(point3D* p);
        
    
    static bool meet(point3D* point ,point3D* p1 ,point3D* p2 );
        
    
    bool visible(point3D* point);
      

};


class array{
public:
  int i ;
  tr_plane** j ;
  int j_length;
  array(int m);
    
  
  void add_pl(tr_plane* m);
      
  
  bool visible(point3D* point);
    
  
};
class Structure2D{
public:	
    point2D** points ;
    int points_num ;
    ad_list* ad ;
    int current ;
    Structure2D(int a);
     
    
    void set_ad(ad_list* a );
    
    void addpoint3D(point3D* m, bool visi);
    
};
class Structure3D{
public: 
    point3D** points ;
    int points_num ;
    ad_list* ad ;
    int current ;
    Structure3D(int a);
    
    void set_ad(ad_list* a );
    
    void addpoint2D(point2D* t, point2D*f ,point2D* s);
    
};