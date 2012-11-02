/*
Part of GVC Project
by :- Addy , Mau , Chinky , Vidushi , Monica
Copyright Reserved
Nobody outside group should access.
*/

static GLuint sphereList;
// declare all global varables here

static void init(void)
{
	/* nothing to be done here */
	GLfloat mat_specular[] = { 0.0, 0.0, 1.0, 0.15 };
	GLfloat mat_shininess[] = { 100.0 };
	GLfloat position[] = { 10.0, 10, 20.0, 0.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	sphereList = glGenLists(1);
	GLfloat mat_solid[] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat mat_zero[] = { 0.0, 0.0, 0.0, 1.0 };
	glNewList(sphereList, GL_COMPILE);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_zero);
	glMaterialfv(GL_FRONT, GL_DIFFUSE , mat_solid);
	glutSolidSphere (1, 16, 20);
	glEndList();
	glClearColor(0.5,0.5,1.0,0.0);
}

class human
{
	public:
		float lShoulder;
		float rShoulder;
		float lWrist;
		float rWrist;
		float lHip;
		float rHip;
		float lKnee;
		float rKnee;
		human()
		{
			lShoulder = 10;
			rShoulder = 10;
			lWrist = 0;
			rWrist = 0;
			lHip = 0;
			rHip = 0;
			lKnee = 0;
			rKnee = 0;
		}
		void draw()
		{
			glPushMatrix();
				/* draw head */
				glTranslatef(0,5,0);
				glScalef(0.5,1,0.5);
				glCallList (sphereList);
			glPopMatrix();
			glPushMatrix();
				/* draw upper body */
				glTranslatef(0,0.75,0);
				glPushMatrix();
					glScalef(1,0.75,1);
					glCallList (sphereList);
				glPopMatrix();
				glTranslatef(0,1.25,0);
				glPushMatrix();
					glScalef(1.2,0.5,1.2);
					glCallList (sphereList);
				glPopMatrix();
				glTranslatef(0,1.25,0);
				glPushMatrix();
					glScalef(1,0.75,1);
					glCallList (sphereList);
				glPopMatrix();
			glPopMatrix();
			glPushMatrix();
				/* draw hips */
				glScalef(1.5,0.5,1);
				glCallList (sphereList);
			glPopMatrix();
			glPushMatrix();
				/* draw shoulder joint */
				glPushMatrix();
					glTranslatef(-1.25,3.75,0);
					glScalef(0.4,0.4,0.4);
					glCallList (sphereList);
				glPopMatrix();
				/* draw left hand */
				glTranslatef(-1.5,4,0);
				glRotatef(-lShoulder,0,0,1);
				//glRotatef(-lShoulder1,1,0,0);
				glTranslatef(0,-1.5,0);
				glPushMatrix();
					/* draw left arm */
					glScalef(0.25,1.5,0.25);
					glCallList (sphereList);
				glPopMatrix();
				glTranslatef(0,-1.5,0);
				glPushMatrix();
					/* draw left forehand */
					glRotatef(-lWrist,0,0,1);
					glTranslatef(0,-1,0);
					glScalef(0.2,1,0.2);
					glCallList (sphereList);
				glPopMatrix();
			glPopMatrix();
			glPushMatrix();
				/* draw shoulder joint */
				glPushMatrix();
					glTranslatef(1.25,3.75,0);
					glScalef(0.4,0.4,0.4);
					glCallList (sphereList);
				glPopMatrix();
				/* draw right hand */
				glTranslatef(1.5,4,0);
				glRotatef(rShoulder,0,0,1);
				glTranslatef(0,-1.5,0);
				glPushMatrix();
					/* draw right arm */
					glScalef(0.25,1.5,0.25);
					glCallList (sphereList);
				glPopMatrix();
				glTranslatef(0,-1.5,0);
				glPushMatrix();
					/* draw right forehand */
					glRotatef(rWrist,0,0,1);
					glTranslatef(0,-1,0);
					glScalef(0.2,1,0.2);
					glCallList (sphereList);
				glPopMatrix();
			glPopMatrix();
			glPushMatrix();
				/* draw left leg */
				glTranslatef(-1,0,0);
				glRotatef(-lHip,1,0,0);
				glTranslatef(0,-1.5,0);
				glPushMatrix();
					/* draw left thigh */
					glScalef(0.4,1.5,0.4);
					glCallList (sphereList);
				glPopMatrix();
				glTranslatef(0,-1.5,0);
				glPushMatrix();
					/* draw left thigh */
					glRotatef(lKnee,1,0,0);
					glTranslatef(0,-1.5,0);
					glScalef(0.25,1.5,0.25);
					glCallList (sphereList);
				glPopMatrix();
			glPopMatrix();
			glPushMatrix();
				/* draw right leg */
				glTranslatef(1,0,0);
				glRotatef(-rHip,1,0,0);
				glTranslatef(0,-1.5,0);
				glPushMatrix();
					/* draw right thigh */
					glScalef(0.4,1.5,0.4);
					glCallList (sphereList);
				glPopMatrix();
				glTranslatef(0,-1.5,0);
				glPushMatrix();
					/* draw right thigh */
					glRotatef(rKnee,1,0,0);
					glTranslatef(0,-1.5,0);
					glScalef(0.25,1.5,0.25);
					glCallList (sphereList);
				glPopMatrix();
			glPopMatrix();
			//to be completed by ADDY
		}
};
