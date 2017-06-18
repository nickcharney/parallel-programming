typedef float4 point;
typedef float4 vector;
typedef float4 color;
typedef float4 sphere;

/*	SphereList = glGenLists( 1 );
	glNewList( SphereList, GL_COMPILE );
		glColor3f(.560784f, .560784f, .737255f);
		glPushMatrix( );
			glTranslatef(-100., -800., 0.);
			glutWireSphere( 600., 100, 100 );
		glPopMatrix( );
		//2nd sphere
		glColor3f(0.137255f, 0.556863f, 0.419608f);
		glPushMatrix();
			glTranslatef(-100., -800., 0.);
			glutWireSphere(1600., 100, 100);
		glPopMatrix();
	glEndList( );
*/





vector
Bounce( vector in, vector n )
{
	vector out = in - n*(vector)( 2.*dot(in.xyz, n.xyz) );
	out.w = 0.;
	return out;
}

vector
BounceSphere( point p, vector v, sphere s )
{
	vector n;
	n.xyz = fast_normalize( p.xyz - s.xyz );
	n.w = 0.;
	return Bounce( v, n );
}

bool
IsInsideSphere( point p, sphere s )
{
	float r = fast_length( p.xyz - s.xyz );
	return  ( r < s.w );
}


kernel
void
Particle( global point *dPobj, global vector *dVel, global color *dCobj )
{
	const float4 G       = (float4) ( 0., -9.8, 0., 0. );
	const float  DT      = 0.1;
	const sphere Sphere1 = (sphere)( -100., -800., 0,  600. );
	const sphere Sphere2 = (sphere)( -100., -800., 0,  1000. );
	int gid = get_global_id( 0 );

	point  p = dPobj[gid];
	vector v = dVel[gid];

	point  pp = p + v*DT + G*(point)( .5*DT*DT );
	vector vp = v + G*DT;
	pp.w = 1.;
	vp.w = 0.;

	if( IsInsideSphere( pp, Sphere1 ) )
	{
		vp = BounceSphere( p, v, Sphere2 );
		pp = p + vp*DT + G*(point)( .5*DT*DT );
		dCobj[gid] = (vector)(0.83,0.55,0.93,1); //changing color if it bounces on sphere 2
	}
	if(!IsInsideSphere( pp, Sphere2 ) )
	{

		vp = BounceSphere( p, v, Sphere2 ) / 2 ;//making the velocity slower if it hits sphere2
		pp = p + vp*DT + G*(point)( .1*DT*DT );
		dCobj[gid] = (vector)(0.46,0.85,0.92,1);//changing color if it bounces on sphere2
	}


	dPobj[gid] = pp;
	dVel[gid]  = vp;

	//dCobj[gid] = vp;

}
