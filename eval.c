#include <stdio.h>
#include <math.h>

int main()
{
	FILE *flabel,*fobs;
	flabel = fopen("labels.dat","r");
	fobs = fopen("obs.dat","r");
	int i;

	for(i=0;i<49;i++) {
		int lu,lv,lw,lx,ou,ov,ow,ox,d2,ad;
		float d;
		fscanf(flabel,"%d%d%d%d",&lu,&lv,&lw,&lx);
		fscanf(fobs,"%d%d%d%d",&ou,&ov,&ow,&ox);
		d2 = (lu-ou)*(lu-ou) + (lv-ov)*(lv-ov) + (lw-ow)*(lw-ow)
			 + (lx-ox)*(lx-ox);
		d = sqrt(d2/4);
		ad = (lu+lv+lw+lx-ou-ov-ox-ow)/4;
		printf("%2d %.f %d\n",i,d,ad);
	}

	return 0;
}
