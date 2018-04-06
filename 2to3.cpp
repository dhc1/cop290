#include"data_types.h"

Structure3D* generate3D(Structure2D* t ,  Structure2D* f ,Structure2D* s   ){
	Structure3D* ans = new Structure3D(t->points_num);
	ans->set_ad(t->ad); 
	for(int i =0 ; i<t->points_num ; i++){
		ans->addpoint2D(t->points[i],f->points[i],s->points[i]);
	}
	return ans;
}