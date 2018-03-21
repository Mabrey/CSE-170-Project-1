# pragma once

# include <sig/gs_vec.h>
# include <sig/sn_shape.h>
# include <sig/gs_array.h>

class GsPolygon; // forward declaration
class GsFontStyle; // forward declaration
class UiLabel; // forward declaration

class SnMyTorus : public SnShape
{  public :
	GsVec init;
	float width;
	float height;
	double R;
	double r;
	double theta;
	double phi;
	double res;

   public :
	static const char* class_name; //<! Contains string SnLines2
	SN_SHAPE_RENDERER_DECLARATIONS;

   public :

	/* Default constructor. */
	SnMyTorus ();

	/* Destructor. */
   ~SnMyTorus ();

   void scaleR(double deltaR, double newR);
   void scale_r(double delta_r, double new_r);
   void scaleRes(double deltaRes, double newRes);

   GsArray<GsVec> buildTorus();

   double xCoord(double theta, double phi);
   double yCoord(double theta, double phi);
   double zCoord(double theta);
   

	/*! Returns the bounding box, can be empty. */
	virtual void get_bounding_box ( GsBox &b ) const override;
};

/*	The method below has to be called before drawing SnMyTorus in order to connect SnMyTorus
	to its renderer. In this example it is automatically called the first time SnMyTorus is
	used, with a call from SnMyTorus's constructor. However, if a SnNode is to be used
	independently from its renderer, the connection should be called from another initilization
	function, so that the node does not need to include or be linked with one particular renderer,
	also allowing connections to diferent renderers when/if needed.
	In sig there is a single initializer for all included renderers in the sigogl module. */
void SnMyTorusRegisterRenderer ();
