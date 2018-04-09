#include "data_types.h"
#include "3to2.h"
#include "2to3.h"
#include <iostream>
#include <cctype>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include<windows.h>
#include<Gl/gl.h>
#include<Gl/glu.h>
#include<fstream>
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
using namespace std;
 
// ----------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------
void display();
void specialKeys();
void processKeys();
 
// ----------------------------------------------------------
// Global Variables
// ----------------------------------------------------------
double zoom = 0.5;
double tr_x = 0;
double tr_y = 0;
double tr_z = 0;
double x_dir = 0;
double y_dir = 0;
double z_dir = 0 ;

Structure3D* in_3D_rotated ;
bool cas = true ;
bool O2or3 ;
int bol = 0 ;


Structure2D* top ;
Structure2D* front;
Structure2D* side;


// ----------------------------------------------------------
// display() Callback function
// ----------------------------------------------------------
void print2dStruc(Structure3D* input){
	Structure3D* in_3D_key = input -> rotate_new(x_dir , y_dir , z_dir) ;  
       // cout << x_dir << " " << y_dir << " " << z_dir << endl;

    Structure2D* Image_2d = generate2D(in_3D_key) ;
    for(int i =0 ; i< Image_2d->points_num ; i++){
      //std::cout <<i << " iter " << Image_2d -> points[i]->x_val << " "<< Image_2d -> points[i]->y_val << endl ;
      for(int i1 =i+1 ; i1< Image_2d ->points_num ;i1++ ){
        if(Image_2d -> ad ->edge_exists(i,i1)){

          point2D* p1 = Image_2d -> points[i];
          point2D* p2 = Image_2d -> points[i1];
          
          if((Image_2d -> visi)->edge_exists(i,i1)){
            glBegin(GL_LINES);
            glColor3f(1.0, 0.0, 1.0 );
            glVertex3f((p1 ->x_val)*zoom ,(p1 -> y_val)*zoom  ,0  );
            glVertex3f((p2 ->x_val)*zoom ,(p2 -> y_val)*zoom  ,0  );
            glEnd();
          }
          else{
            glBegin(GL_LINES);
            glColor3f(0.3, 0.3, 0.3);
            glVertex3f((p1 ->x_val)*zoom ,(p1 -> y_val)*zoom  ,0  );
            glVertex3f((p2 ->x_val)*zoom ,(p2 -> y_val)*zoom  ,0  );
            glEnd();

          }
        }
      }
    }
}
void print3dStruc(Structure3D* input){
   Structure3D* Image_3d = input -> rotate_new(x_dir , y_dir , z_dir) ;  
   sort_by_z(Image_3d);	
   
   ad_list* ad = Image_3d -> ad ;
    for(int i =0 ; i< Image_3d->points_num ; i++){
      for(int i1 =i+1 ; i1< Image_3d ->points_num ;i1++ ){
        if(ad->edge_exists(i,i1)){
          point3D* p1 = Image_3d -> points[i];
          point3D* p2 = Image_3d -> points[i1];
            glLineWidth(5*zoom);
            glBegin(GL_LINES);
            glColor3f(0.0, 0.0, 1.0);
            glVertex3f((p1 ->x_val)*zoom ,(p1 -> y_val)*zoom , -(p1 -> z_val)*zoom    );
            glVertex3f((p2 ->x_val)*zoom ,(p2 -> y_val)*zoom ,-(p2 -> z_val)*zoom   );
            glEnd();

        }
      }
    }
    int** pom = facedetection(Image_3d);
    for(int i =1;i<pom[0][0] +1;i++){
      glLineWidth(1*zoom);
      glColor3f(0.255,0.140,0);
      glBegin(GL_POLYGON);
      for(int i1 =1 ;i1 < pom[i][0] +1;i1++){
        glVertex3f((Image_3d->points[pom[i][i1]] ->x_val)*zoom ,(Image_3d->points[pom[i][i1]] -> y_val)*zoom , -(Image_3d->points[pom[i][i1]] -> z_val)*zoom    );
      }
      glEnd();
    }
}
void display(){
 
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
 
  glLoadIdentity();
  glTranslatef(tr_x, tr_y ,tr_z);

  point3D* l1 = new point3D(500000,0,0);
  l1->rotate(x_dir,y_dir,z_dir,0,0,0); 
  glBegin(GL_LINES);
  glLineWidth(5);
  glColor3f(1, 0.0, 0.0 );
  glVertex3f(0 ,0  ,0  );
  glVertex3f(l1 -> x_val,l1 -> y_val  ,l1 -> z_val  );
  glEnd();

  point3D* l2 = new point3D(0,500000,0);
  l2->rotate(x_dir,y_dir,z_dir,0,0,0);
  glBegin(GL_LINES);
  glLineWidth(5);
  glColor3f(0, 1.0, 0.0 );
  glVertex3f(0 ,0  ,0  );
  glVertex3f(l2 -> x_val,l2 -> y_val  ,l2 -> z_val  );
  glEnd();

  point3D* l3 = new point3D(0,0,500000);
  l3->rotate(x_dir,y_dir,z_dir,0,0,0);
  glBegin(GL_LINES);
  glLineWidth(5);
  glColor3f(0, 0, 1.0 );
  glVertex3f(0 ,0  ,0  );
  glVertex3f(l3 -> x_val,l3 -> y_val  ,l3 -> z_val  );
  glEnd();
  
  if(O2or3){
  	if(cas )
    print2dStruc(in_3D_rotated);
	  else 
	  print3dStruc(in_3D_rotated);	

  }
  else{
      Structure3D* Image_3d = generate3D(top , front , side ) ;
      print3dStruc(Image_3d);
   }
    
  glFlush();
  glutSwapBuffers();
 
}
void processKeys(unsigned char key , int x , int y){
  if(key == 'm')
    zoom = zoom*1.6 ;
  else if (key == 'n')
    zoom = zoom * (0.625) ;
  
  else if (key == 'd')
    tr_x = tr_x + 0.2 ;
  else if (key == 'a')
    tr_x = tr_x - 0.2;
  else if (key == 'w')
    tr_y = tr_y + 0.2;
  else if (key == 's')
    tr_y = tr_y - 0.2;
  
  if(!O2or3){
  	if (key == 'x')
    z_dir += 5;
 
    else if (key == 'z')
    z_dir -= 5;

  }
  else{
  	if (key == 'x')
    z_dir += 5;
 
    else if (key == 'z')
    z_dir -= 5;	
	
	  else if (key == 'g'){
    cas = !cas;
    }
	
  }  
  glutPostRedisplay();
    
}
 
// ----------------------------------------------------------
// specialKeys() Callback Function
// ----------------------------------------------------------
void specialKeys( int key, int x, int y ) {
 
  
  if (key == GLUT_KEY_RIGHT)
    y_dir += 5;
 
  else if (key == GLUT_KEY_LEFT)
    y_dir -= 5;
 
  else if (key == GLUT_KEY_UP)
    x_dir += 5;
 
  else if (key == GLUT_KEY_DOWN)
    x_dir -= 5;
  

 
  glutPostRedisplay();
 
}

int main(int argc, char* argv[]){

        std::ifstream file;
        file.open(argv[1]);
        int n;
        file>>n;
        

        if(n==1){
        O2or3 = true;
        
        double x_given ;
        double y_given ;
        double z_given ;
        file >> x_given ;
        file >> y_given ;
        file >> z_given ;
        
        int point_num;
        file >>point_num;
        
        point3D** set =(point3D**)malloc(point_num * sizeof(point3D*)) ;
        for(int i =0 ; i< point_num;i++){
            double m,n,o ;
            file >> m;
            file >> n;
            file >> o;
            set[i] = new point3D(m,n,o);
        }
        int edge_num ;
        file >> edge_num ;
        ad_list* ad = new ad_list(point_num);
        for(int i =0 ; i<edge_num ; i++){
            int a , b;
            file >> a ;
            file >>b ;
            ad-> set_edge(a,b);
        }

        Structure3D* in_3D = new Structure3D(point_num);
        in_3D -> set_ad(ad);
        in_3D -> points = set ;


        in_3D_rotated = in_3D->rotate_new(x_given , y_given , z_given );
        
       
       }
       


        else{
        O2or3 = false ;
        int point_num ;
        file >> point_num ;  
        
        top = new Structure2D(point_num);
        front = new Structure2D(point_num);
        side = new Structure2D(point_num);
        int edge_num ;
        file >> edge_num ;
        ad_list* ad = new ad_list(point_num);
        for(int i =0 ; i<edge_num ; i++){
            int a , b ;
            file >> a ;
            file >>b ;
            ad-> set_edge(a,b);
        }

        top->set_ad(ad);
        front->set_ad(ad);
        side->set_ad(ad);
        point2D** set_top = (point2D**) malloc(point_num * sizeof(point2D*));
        for(int i =0 ; i< point_num;i++){
            double m,n ;
            file >> m;
            file >> n;
            set_top[i] = new point2D(m,n,false);
        }
        point2D** set_f = (point2D**) malloc(point_num * sizeof(point2D*));
        for(int i =0 ; i< point_num;i++){
            double m,n ;
            file >> m;
            file >> n;
            set_f[i] = new point2D(m,n,false);
        }
        point2D** set_s = (point2D**) malloc(point_num * sizeof(point2D*));
        for(int i =0 ; i< point_num;i++){
            double m,n ;
            file >> m;
            file >> n;
            set_s[i] = new point2D(m,n,false);
        }
        top->points = set_top ;
        front->points = set_f ;
        side->points = set_s ;
        

       }

       
  glutInit(&argc,argv);
 
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
 
  glutCreateWindow("View");
 
  glEnable(GL_DEPTH_TEST);
 
  glutDisplayFunc(display);
  glutSpecialFunc(specialKeys);
  glutKeyboardFunc(processKeys);


 
  glutMainLoop();
 
  return 0;
 
}