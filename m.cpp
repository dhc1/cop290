#include "data_types.h"
#include "3to2.h"
#include "2to3.h"
#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include<windows.h>
#include<Gl/gl.h>
#include<Gl/glu.h>
//#include <plib/pu.h>
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
double rotate_y=0; 
double rotate_x=0;
double zoom = 1;
double tr_x = 0;
double tr_y = 0;
double tr_z = 0;
double x_dir = 0;
double y_dir = 0;
double z_dir = 0 ;

Structure2D* Image_2d ;
Structure3D* in_3D_rotated ;
Structure3D* in_3D_key;
bool O2or3 ;

Structure3D* Image_3d ;



// ----------------------------------------------------------
// display() Callback function
// ----------------------------------------------------------
void display(){
 
  //  Clear screen and Z-buffer
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
 
  // Reset transformations
  glLoadIdentity();
 
  // Other Transformations
  // glTranslatef( 0.1, 0.0, 0.0 );      // Not included
  // glRotatef( 180, 0.0, 1.0, 0.0 );    // Not included
 
  // Rotate when user changes rotate_x and rotate_y
  glTranslatef(tr_x, tr_y ,tr_z);
  
  
  // Other Transformations
  // glScalef( 2.0, 2.0, 0.0 );          // Not included
 
  //Multi-colored side - FRONT
  if(O2or3){
        //before start , delete old
        in_3D_key = in_3D_rotated -> rotate_new(x_dir , y_dir , z_dir) ;  
       // cout << x_dir << " " << y_dir << " " << z_dir << endl;

    Structure2D* Image_2d = generate2D(in_3D_key) ;
    for(int i =0 ; i< Image_2d->points_num ; i++){
      //std::cout <<i << " iter " << Image_2d -> points[i]->x_val << " "<< Image_2d -> points[i]->y_val << endl ;
      for(int i1 =i+1 ; i1< Image_2d ->points_num ;i1++ ){
        if(Image_2d -> ad ->edge_exists(i,i1)){

          point2D* p1 = Image_2d -> points[i];
          point2D* p2 = Image_2d -> points[i1];
          
          if(p1->visible && p2->visible){
            glBegin(GL_LINES);
            glColor3f(1.0, 0.0, 1.0 );
            glVertex2f((p1 ->x_val)*zoom ,(p1 -> y_val)*zoom    );
            glVertex2f((p2 ->x_val)*zoom ,(p2 -> y_val)*zoom    );
            glEnd();
          }
          else{
            glBegin(GL_LINES);
            glColor3f(0.0, 0.0, 1.0);
            glVertex2f((p1 ->x_val)*zoom ,(p1 -> y_val)*zoom    );
            glVertex2f((p2 ->x_val)*zoom ,(p2 -> y_val)*zoom    );
            glEnd();

          }
        }
      }
    }
  }
  else{
   glRotatef( rotate_x, 1.0, 0.0, 0.0 );
   glRotatef( rotate_y, 0.0, 1.0, 0.0 ); 
   ad_list* ad = Image_3d -> ad ;
    for(int i =0 ; i< Image_3d->points_num ; i++){
      for(int i1 =i+1 ; i1< Image_3d ->points_num ;i1++ ){
        if(ad->edge_exists(i,i1)){
          point3D* p1 = Image_3d -> points[i];
          point3D* p2 = Image_3d -> points[i1];
            glLineWidth(5);
            glBegin(GL_LINES);
            glColor3f(0.0, 0.0, 1.0);
            glVertex3f((p1 ->x_val)*zoom ,(p1 -> y_val)*zoom , (p1 -> z_val)*zoom    );
            glVertex3f((p2 ->x_val)*zoom ,(p2 -> y_val)*zoom ,(p2 -> z_val)*zoom   );
            glEnd();

        }
      }
    }
    int** pom = facedetection(Image_3d);
    for(int i =1;i<pom[0][0] +1;i++){
      glLineWidth(1);
      glColor3f(0.5,0,0);
      glBegin(GL_POLYGON);
      for(int i1 =1 ;i1 < pom[i][0] +1;i1++){
        glVertex3f((Image_3d->points[pom[i][i1]] ->x_val)*zoom ,(Image_3d->points[pom[i][i1]] -> y_val)*zoom , (Image_3d->points[pom[i][i1]] -> z_val)*zoom    );
      }
      glEnd();
    }
    
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
  

  glutPostRedisplay();
}
 
// ----------------------------------------------------------
// specialKeys() Callback Function
// ----------------------------------------------------------
void specialKeys( int key, int x, int y ) {
 
  
  //  Right arrow - increase rotation by 5 degree
  if(!O2or3){
  if (key == GLUT_KEY_RIGHT)
    rotate_y += 5;
 
  //  Left arrow - decrease rotation by 5 degree
  else if (key == GLUT_KEY_LEFT)
    rotate_y -= 5;
 
  else if (key == GLUT_KEY_UP)
    rotate_x += 5;
 
  else if (key == GLUT_KEY_DOWN)
    rotate_x -= 5;
 }
 else{
  if (key == GLUT_KEY_RIGHT)
    y_dir += 5;
 
  //  Left arrow - decrease rotation by 5 degree
  else if (key == GLUT_KEY_LEFT)
    y_dir -= 5;
 
  else if (key == GLUT_KEY_UP)
    x_dir += 5;
 
  else if (key == GLUT_KEY_DOWN)
    x_dir -= 5;
 }
  

 
  //  Request display update
  glutPostRedisplay();
 
}
 //bugs dekhlo yaaro
// ----------------------------------------------------------
// main() function
// ----------------------------------------------------------

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
        
        //in_3D can be deleted//
       
       }
       


       //input for 2 to 3 
       //int point_num2 = 4 ;
        else{
        O2or3 = false ;
        int point_num ;
        file >> point_num ;  
        Structure2D* top ;
        Structure2D* front;
        Structure2D* side;
        top = new Structure2D(point_num);
        front = new Structure2D(point_num);
        side = new Structure2D(point_num);
        //ad_list* ad = new ad_list(point_num);
        /*ad_2 -> set_edge(1,2);
        ad_2 -> set_edge(2,3);
        ad_2 -> set_edge(3,4);
        ad_2 -> set_edge(4,1);*/
        int edge_num ;
        file >> edge_num ;
        ad_list* ad = new ad_list(point_num);
        for(int i =0 ; i<edge_num ; i++){
            int a , b;
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
        Structure3D* D3 = generate3D(top , front , side ) ;
        Image_3d = D3 ;

       }

       
       /*cout << D2->points[0]->x_val << " " << D2->points[0]->y_val <<" "<< D2->points[0] ->visible << endl ;
       cout << D2->points[1]->x_val << " " << D2->points[1]->y_val <<" "<< D2->points[1] ->visible << endl ;
       cout << D2->points[2]->x_val << " " << D2->points[2]->y_val <<" "<< D2->points[2] ->visible << endl ;
       cout << D2->points[3]->x_val << " " << D2->points[3]->y_val <<" "<< D2->points[3] ->visible << endl ;
       cout << D2->points[4]->x_val << " " << D2->points[4]->y_val <<" "<< D2->points[4] ->visible << endl ;
        */

       /*cout << D3->points[0]->x_val << " " << D3->points[0]->y_val <<" "<< D3->points[0] ->z_val << endl ;
       cout << D3->points[1]->x_val << " " << D3->points[1]->y_val <<" "<< D3->points[1] ->z_val << endl ;
       cout << D3->points[2]->x_val << " " << D3->points[2]->y_val <<" "<< D3->points[2] ->z_val << endl ;
       cout << D3->points[3]->x_val << " " << D3->points[3]->y_val <<" "<< D3->points[3] ->z_val << endl ;
       cout << D3->points[4]->x_val << " " << D3->points[4]->y_val <<" "<< D3->points[4] ->z_val << endl ;
       //cout << D3->points[4]->visible << endl ;
       */
       
 
  //  Initialize GLUT and process user parameters
  glutInit(&argc,argv);
 
  //  Request double buffered true color window with Z-buffer
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
 
  // Create window
  glutCreateWindow("View");
 
  //  Enable Z-buffer depth test
  glEnable(GL_DEPTH_TEST);
 
  // Callback functions
  glutDisplayFunc(display);
  glutSpecialFunc(specialKeys);
  glutKeyboardFunc(processKeys);


 
  //  Pass control to GLUT for events
  glutMainLoop();
 
  //  Return to OS
  return 0;
 
}