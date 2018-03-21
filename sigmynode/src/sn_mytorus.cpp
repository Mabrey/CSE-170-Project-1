# include "sn_mytorus.h"
#define _USE_MATH_DEFINES 
#include <math.h>

const char* SnMyTorus::class_name = "SnMyTorus"; // static
SN_SHAPE_RENDERER_DEFINITIONS(SnMyTorus);

//===== SnMyTorus =====

SnMyTorus::SnMyTorus () : SnShape ( class_name )
{
	width = 0.5f;
	height = 1.0f;
	R = 0.1f;
	r = 0.04f;
	theta = 0.0f;
	phi = 0.0f;
	res = 6;
	if ( !SnMyTorus::renderer_instantiator ) SnMyTorusRegisterRenderer ();
}

SnMyTorus::~SnMyTorus()
{
}
/*
void SnMyTorus::scaleR(double deltaR, double newR)
{
	R = deltaR + newR;
}

void SnMyTorus::scale_r(double delta_r, double new_r)
{
	r += delta_r + new_r;
}

void SnMyTorus::scaleRes(double deltaRes, double newRes)
{
	res = deltaRes + newRes;
}
*/
GsArray<GsVec> SnMyTorus::buildTorus()
{
	double x1, x2, x3, x4, y1, y2, y3, y4, z1, z2, z3, z4;
	GsArray<GsVec> T(0, 1000);

	for (phi = 0; phi <= (2 * M_PI); phi += M_PI / res)
	{
		for (theta = 0; theta <= (2 * M_PI); theta += M_PI / res)
		{
			
			//point 1
			{
				x1 = xCoord(theta, phi);
				y1 = yCoord(theta, phi);
				z1 = zCoord(theta);
			}

			//point 2
			{
				x2 = xCoord(theta, phi + M_PI / res);
				y2 = yCoord(theta, phi + M_PI / res);
				z2 = zCoord(theta);
			}

			//point 3
			{
				x3 = xCoord(theta + M_PI / res, phi);
				y3 = yCoord(theta + M_PI / res, phi);
				z3 = zCoord(theta + M_PI / res);
			}

			//point 4
			{
				x4 = xCoord(theta + M_PI / res, phi + M_PI / res);
				y4 = yCoord(theta + M_PI / res, phi + M_PI / res);
				z4 = zCoord(theta + M_PI / res);
			}
			
			//Triangle 1
			T.push() = GsVec(x1, y1 ,z1);
			T.push() = GsVec(x2, y2, z2);
			T.push() = GsVec(x3, y3, z3);

			//Triangle 2
			T.push() = GsVec(x2, y2, z2);
			T.push() = GsVec(x3, y3, z3);
			T.push() = GsVec(x4, y4, z4);
		}
	}
	return T;
}

double SnMyTorus::xCoord(double theta, double phi)
{
	double x = (R + (r * cos(theta))) * cos(phi);
	return x;
}

double SnMyTorus::yCoord(double theta, double phi)
{
	double y = (R + (r * cos(theta))) * sin(phi);
	return y;
}

double SnMyTorus::zCoord(double theta)
{
	double z = r * sin(theta);
	return z;
}

void SnMyTorus::get_bounding_box ( GsBox& b ) const
{
	// the bounding box is needed for camera view_all computations
	b.set ( init, init+GsVec(width,height,0) );
}

//===== Renderer Instantiator =====

# include "glr_mynode.h"

static SnShapeRenderer* GlrMyNodeInstantiator ()
{
	return new GlrMyNode;
}

void SnMyTorusRegisterRenderer ()
{
	SnMyTorus::renderer_instantiator = &GlrMyNodeInstantiator;
}
