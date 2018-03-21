
# include "my_viewer.h"

# include <sigogl/ui_button.h>
# include <sigogl/ui_radio_button.h>
# include <sig/sn_primitive.h>
# include <sig/sn_transform.h>
# include <sig/sn_manipulator.h>

# include <sigogl/ws_run.h>

double bigR, littleR;
int resolution;

void make_torus(GsModel*, double, double, int);
SnLines* computeSegments(bool, GsModel*);

MyViewer::MyViewer ( int x, int y, int w, int h, const char* l ) : WsViewer(x,y,w,h,l)
{
	_nbut=0;
	_animating=false;
	build_ui ();
	build_scene ();
}

void MyViewer::build_ui ()
{
	UiPanel *p, *sp;
	UiManager* uim = WsWindow::uim();
	p = uim->add_panel ( "", UiPanel::HorizLeft );
	p->add ( new UiButton ( "View", sp=new UiPanel() ) );
	{	UiPanel* p=sp;
		p->add ( _nbut=new UiCheckButton ( "Normals", EvNormals ) ); 
	}
	p->add ( new UiButton ( "Animate", EvAnimate ) );
	p->add ( new UiButton ( "Exit", EvExit ) ); p->top()->separate();
}

void MyViewer::add_model ( SnShape* s, GsVec p )
{
	SnManipulator* manip = new SnManipulator;
	GsMat m;
	m.translation ( p );
	manip->initial_mat ( m );

	SnGroup* g = new SnGroup;
	SnLines* l = new SnLines;
	l->color(GsColor::orange);
	g->add(s);
	g->add(l);
	manip->child(g);

	rootg()->add(manip);
}

void MyViewer::build_scene ()
{
	///*
	SnModel* sn = new SnModel;	// create your SnModel 
	GsModel& m = *sn->model();	// access the GsModel
	rootg()->add(sn);

	make_torus(&m, .3, .05, 20);


	GsModel::Group& g = *m.G.push();
	g.fi = 0;
	g.fn = m.F.size();
	g.dmap = new GsModel::Texture;
	g.dmap->fname.set("C:/Users/matth/Desktop/CSE/CSE170/PA5/boobies.png");

	m.M.push().init();

	int nv = m.V.size();
	m.T.size(nv);
	for (int i = 0; i < nv; i++)
	{
		m.T[i].set()
	}

	//*/

	GsPnt center;
	center.set(0, 0, 0);

	GsModel* torus = new GsModel;
	make_torus(torus, .3, 0.05, 20);
	GsVec normal;
	int i = 0;

	SnLines* lines = new SnLines;
	lines = computeSegments(true, torus);

	gsout << "Points size: " << torus->V.size() << gsnl;
	gsout << "Faces size: " << torus->F.size() << gsnl;
	gsout << "Normal size: " << torus->N.size() << gsnl;


	//torus->

	torus->set_mode(GsModel::Smooth, GsModel::NoMtl);
	SnGroup *g1 = new SnGroup;
	SnGroup *g2 = new SnGroup;
	g1 -> separator(true);
	g1 -> add(new SnModel(torus));
	g1->top<SnModel>()->color(GsColor::red);

	g2->separator(true);
	g2->add(lines);
	//g2->top<SnModel>()->color(GsColor::green);
	rootg()->add(g1);
	//rootg()->add(g2);
}

// Below is an example of how to control the main loop of an animation:
void MyViewer::run_animation ()
{
	if ( _animating ) return; // avoid recursive calls
	_animating = true;
	
	int ind = gs_random ( 0, rootg()->size()-1 ); // pick one child
	SnManipulator* manip = rootg()->get<SnManipulator>(ind); // access one of the manipulators
	GsMat m = manip->mat();

	double frdt = 1.0/30.0; // delta time to reach given number of frames per second
	double v = 4; // target velocity is 1 unit per second
	double t=0, lt=0, t0=gs_time();
	do // run for a while:
	{	while ( t-lt<frdt ) { ws_check(); t=gs_time()-t0; } // wait until it is time for next frame
		double yinc = (t-lt)*v;
		if ( t>2 ) yinc=-yinc; // after 2 secs: go down
		lt = t;
		m.e24 += (float)yinc;
		if ( m.e24<0 ) m.e24=0; // make sure it does not go below 0
		manip->initial_mat ( m );
		render(); // notify it needs redraw
		ws_check(); // redraw now
	}	while ( m.e24>0 );
	_animating = false;
}

void computeNormals(GsModel* m)
{
	double phi = 0;
	double tx, ty, tz = 0; //center of small torus circle
	for (int i = 0; i < m->V.size(); i++)
	{
		if (i % resolution == 0 && i != 0)
			phi += 2 * M_PI / resolution;

		tx = bigR * cos(phi);
		ty = bigR * sin(phi);
		GsPnt t = GsPnt(tx, ty, tz);//torus center
		GsPnt s = m->V[i];
		GsVec v = GsVec(s.x - t.x, s.y - t.y, s.z - t.z);
		GsVec s2 = GsPnt(s.x + v.x, s.y + v.y, s.z + v.z);
		m->N.push(v);
		//gsout << "v: " << v.x << ", " << v.y << ", " << v.z << "\n";


	}
}


SnLines* computeSegments(bool flat, GsModel* m)
{
	SnLines* l = new SnLines;
	l->init();
	l->color(GsColor::green);
	for (int i = 0; i < m->V.size(); i++)
	{
		l->push(m->V[i], m->N[i]);
	}
	/*
	if (flat)
	{
		for (int i = 0; i < torus->F.size(); i++)
		{
			const GsVec& a = torus -> V[torus -> F[i].a];
			const GsVec& b = torus -> V[torus -> F[i].b];
			const GsVec& c = torus -> V[torus -> F[i].c];
			GsVec faceCenter = (a + b + c) / 3.0f;
			l->push(faceCenter, torus->N[i] * .01f);
		}
	}
	*/
	return l;

}

double xCoord(double theta, double phi)
{
	double x = (bigR + (littleR * cos(theta))) * cos(phi);
	return x;
}

double yCoord(double theta, double phi)
{
	double y = (bigR + (littleR * cos(theta))) * sin(phi);
	return y;
}

double zCoord(double theta)
{
	double z = littleR * sin(theta);
	return z;
}

void make_torus(GsModel* m, double R, double r, int res)
{
	bigR = R;
	littleR = r;
	resolution = res;
	int count = 0;
	GsModel::Face face;
	double x, y, z;

	for (double phi = 0; phi < (2 * M_PI); phi += 2 * M_PI / resolution)
	{
		for (double theta = 0; theta < (2 * M_PI); theta += 2 * M_PI / resolution)
		{
			x = xCoord(theta, phi);
			y = yCoord(theta, phi);
			z = zCoord(theta);
			m->V.push() = GsPnt(x, y, z);
		}
	}

	for (int i = 0; i < m->V.size(); i++)
	{
		int p1 = i;
		int p2;

		if ((i + 1) % resolution == 0)
		{
			p2 = i - (resolution - 1);
		}
		else p2 = i + 1;

		int p3 = ((i + resolution) % (resolution * resolution));
		face.set(p1, p3, p2);
		m->F.push(face);

		int p4 = ((p2 + resolution) % (resolution * resolution));
		
		face.set(p2, p3, p4);
		m->F.push(face);
		//gsout << "p1: " << p1 << " p2: " << p2 << " p3: " << p3 << " p4: " << p4 << "\n";
	}
	computeNormals(m);

}

/*
void make_torus(GsModel* m, double R, double r, int res)
{
	bigR = R;
	littleR = r;
	resolution = res;
	int count = 0;
	GsModel::Face face;

	//gsout << "Faces size: " << m->F.size() << gsnl;
	//gsout << "Normal size: " << m->N.size() << gsnl;

	double x1, x2, x3, x4, y1, y2, y3, y4, z1, z2, z3, z4;

	for (double phi = 0; phi <= (4 * M_PI); phi += M_PI / resolution)
	{
		for (double theta = 0; theta <= (2 * M_PI); theta += M_PI / resolution)
		{

			//point 1
			{
				x1 = xCoord(theta, phi);
				y1 = yCoord(theta, phi);
				z1 = zCoord(theta);
			}

			//point 2
			{
				x2 = xCoord(theta, phi + M_PI / resolution);
				y2 = yCoord(theta, phi + M_PI / resolution);
				z2 = zCoord(theta);
			}

			//point 3
			{
				x3 = xCoord(theta + M_PI / resolution, phi);
				y3 = yCoord(theta + M_PI / resolution, phi);
				z3 = zCoord(theta + M_PI / resolution);
			}

			//point 4
			{
				x4 = xCoord(theta + M_PI / resolution, phi + M_PI / resolution);
				y4 = yCoord(theta + M_PI / resolution, phi + M_PI / resolution);
				z4 = zCoord(theta + M_PI / resolution);
			}

			m -> V.push() = GsPnt(x1, y1, z1);
			m -> V.push() = GsPnt(x2, y2, z2);
			m -> V.push() = GsPnt(x3, y3, z3);
			m -> V.push() = GsPnt(x4, y4, z4);
			
			face.set(count, count + 1, count + 2);

			m -> F.push(face);
			//m->N.push(count);
			//gsout << "Faces size: " << m->F.size() << gsnl;
			//gsout << "Normal size: " << m->N.size() << gsnl;
			count++;

			face.set(count + 2, count + 1, count);
			m -> F.push(face);
			//m->N.push(count);
			//gsout << "Faces size: " << m->F.size() << gsnl;
			//gsout << "Normal size: " << m->N.size() << gsnl;
			count++;
		}
	}
}
*/


void MyViewer::show_normals ( bool b )
{
	// Note that primitives are only converted to meshes in GsModel
	// at the first draw call.
	GsArray<GsVec> fn;
	SnGroup* r = (SnGroup*)root();
	for ( int k=0; k<r->size(); k++ )
	{	SnManipulator* manip = r->get<SnManipulator>(k);
		SnShape* s = manip->child<SnGroup>()->get<SnShape>(0);
		SnLines* l = manip->child<SnGroup>()->get<SnLines>(1);
		if ( !b ) { l->visible(false); continue; }
		l->visible ( true );
		if ( !l->empty() ) continue; // build only once
		l->init();
		if ( s->instance_name()==SnPrimitive::class_name )
		{	GsModel& m = *((SnModel*)s)->model();
			m.get_normals_per_face ( fn );
			const GsVec* n = fn.pt();
			float f = 0.33f;
			for ( int i=0; i<m.F.size(); i++ )
			{	const GsVec& a=m.V[m.F[i].a]; l->push ( a, a+(*n++)*f );
				const GsVec& b=m.V[m.F[i].b]; l->push ( b, b+(*n++)*f );
				const GsVec& c=m.V[m.F[i].c]; l->push ( c, c+(*n++)*f );
			}
		}  
	}
}

int MyViewer::handle_keyboard ( const GsEvent &e )
{
	GsModel* m = new GsModel;
	SnGroup *g1 = new SnGroup;
	
	int ret = WsViewer::handle_keyboard ( e ); // 1st let system check events
	if ( ret ) return ret;

	switch ( e.key )
	{	case GsEvent::KeyEsc : gs_exit(); return 1;
		case 'n' : { bool b=!_nbut->value(); _nbut->value(b); show_normals(b); return 1; }

		case GsEvent::KeyCodes(113)://q
			resolution += 1;
			gsout << "Resolution: " << resolution << "\n";
			//c->scaleRes(1, res);
			//c->color(GsColor(rd, g, b));
			rootg()->remove();
			//GsModel* m = new GsModel;
			make_torus(m, bigR, littleR, resolution);
			m->set_mode(GsModel::Smooth, GsModel::NoMtl);
			//SnGroup *g1 = new SnGroup;
			g1->separator(true);
			g1->add(new SnModel(m));
			g1->top<SnModel>()->color(GsColor::red);
			rootg()->add(g1);
			WsViewer::render();
			return 1;

		case GsEvent::KeyCodes(97)://a
			resolution -= 1;
			gsout << "Resolution: " << resolution << "\n";
			//c->scaleRes(-1, res);
			rootg()->remove();
			//GsModel* m = new GsModel;
			make_torus(m, bigR, littleR, resolution);
			m->set_mode(GsModel::Smooth, GsModel::NoMtl);
			//SnGroup *g1 = new SnGroup;
			g1->separator(true);
			g1->add(new SnModel(m));
			g1->top<SnModel>()->color(GsColor::red);
			rootg()->add(g1);
			WsViewer::render();
			return 1;

		case GsEvent::KeyCodes(119)://w
			littleR += 0.01;
			rootg()->remove();
			//GsModel* m;
			make_torus(m, bigR, littleR, resolution);
			m->set_mode(GsModel::Smooth, GsModel::NoMtl);
			//SnGroup *g1 = new SnGroup;
			g1->separator(true);
			g1->add(new SnModel(m));
			g1->top<SnModel>()->color(GsColor::red);
			rootg()->add(g1);
			WsViewer::render();
			return 1;

		case GsEvent::KeyCodes(115)://s
			littleR -= 0.01;
			//c->scale_r(-0.01, r);
			rootg()->remove();
			//GsModel* m;
			make_torus(m, bigR, littleR, resolution);
			m->set_mode(GsModel::Smooth, GsModel::NoMtl);
			//SnGroup *g1 = new SnGroup;
			g1->separator(true);
			g1->add(new SnModel(m));
			g1->top<SnModel>()->color(GsColor::red);
			rootg()->add(g1);
			WsViewer::render();
			return 1;

		case GsEvent::KeyCodes(101)://e
			bigR += 0.05;
			//c->scaleR(0.05, R);
			rootg()->remove();
			//GsModel* m;
			make_torus(m, bigR, littleR, resolution);
			m->set_mode(GsModel::Smooth, GsModel::NoMtl);
			//SnGroup *g1 = new SnGroup;
			g1->separator(true);
			g1->add(new SnModel(m));
			g1->top<SnModel>()->color(GsColor::red);
			rootg()->add(g1);
			WsViewer::render();
			return 1;

		case GsEvent::KeyCodes(100)://d
			bigR += -0.05;
			//c->scaleR(-0.05, R);
			rootg()->remove();
			//GsModel* m;
			make_torus(m, bigR, littleR, resolution);
			m->set_mode(GsModel::Smooth, GsModel::NoMtl);
			//SnGroup *g1 = new SnGroup;
			g1->separator(true);
			g1->add(new SnModel(m));
			g1->top<SnModel>()->color(GsColor::red);
			rootg()->add(g1);
			WsViewer::render();
			return 1;

		default: gsout<<"Key pressed: "<<e.key<<gsnl;
	}

	return 0;
}

int MyViewer::uievent ( int e )
{
	

	switch ( e )
	{	case EvNormals: show_normals(_nbut->value()); return 1;
		case EvAnimate: run_animation(); return 1;
		case EvExit: gs_exit();
	}
	return WsViewer::uievent(e);
}
