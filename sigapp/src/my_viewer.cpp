
# include "my_viewer.h"
# include "bender.h"

# include <sigogl/ui_button.h>
# include <sigogl/ui_radio_button.h>
# include <sig/sn_primitive.h>
# include <sig/sn_transform.h>
# include <sig/sn_manipulator.h>

# include <sigogl/ws_run.h>

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
	
	GsModel* bender_head = new GsModel;
	GsModel* bender_torso = new GsModel;
	GsModel* bender_left_arm = new GsModel;
	GsModel* bender_right_arm = new GsModel;
	GsModel* bender_left_leg = new GsModel;
	GsModel* bender_right_leg = new GsModel;

	SnModel*sn = new SnModel;
	GsModel& ground = *sn->model();
	rootg()->add(sn);

	ground.V.size(4);
	ground.F.size(2);
	ground.N.size(2);
	ground.V[0] = GsVec(1, 0, 1);
	ground.V[1] = GsVec(1, 0, -1);
	ground.V[2] = GsVec(-1, 0, -1);
	ground.V[3] = GsVec(-1, 0, 1);
	ground.F[0] = GsModel::Face(0, 1, 2);
	ground.F[1] = GsModel::Face(2, 3, 0);
	ground.N[0] = GsVec(0, 1, 0);
	ground.N[1] = GsVec(0, 1, 0);
	//ground.N[2] = GsVec(-1, 0, 1);
	//ground.N[3] = GsVec(-1, 0, -1);
	


	init_bender_models(*bender_head, *bender_torso, *bender_left_arm,
		*bender_right_arm, *bender_left_leg, *bender_right_leg);

	SnGroup *group_head = new SnGroup;
	SnGroup *group_torso = new SnGroup;
	SnGroup *group_left_arm = new SnGroup;
	SnGroup *group_right_arm = new SnGroup;
	SnGroup *group_left_leg = new SnGroup;
	SnGroup *group_right_leg = new SnGroup;
	SnTransform* transf_head, *transf_torso, *transf_left_arm, *transf_right_arm, *transf_left_leg, *transf_right_leg;
	SnManipulator* main_manip = new SnManipulator;
	//SnGroup *g7 = new SnGroup;

	group_head->separator(true);
	group_head->add(transf_head = new SnTransform);
	group_head->add(new SnModel(bender_head));
	group_head->top<SnModel>()->color(GsColor::gray);
	transf_head->get().translation(0, 2.7f, 0);

	group_torso->separator(true);
	group_torso->add(transf_torso = new SnTransform);
	group_torso->add(new SnModel(bender_torso));
	group_torso->top<SnModel>()->color(GsColor::gray);
	transf_torso->get().translation(0, 8.25f, 0);


	group_left_arm->separator(true);
	group_left_arm->add(transf_left_arm = new SnTransform);
	group_left_arm->add(new SnModel(bender_left_arm));
	group_left_arm->top<SnModel>()->color(GsColor::gray);
	transf_left_arm->get().translation(4.5f, 0.8f, 0);

	group_right_arm->separator(true);
	group_right_arm->add(transf_right_arm = new SnTransform);
	group_right_arm->add(new SnModel(bender_right_arm));
	group_right_arm->top<SnModel>()->color(GsColor::gray);
	transf_right_arm->get().translation(-4.4f, 0.92f, 0);

	group_left_leg->separator(true);
	group_left_leg->add(transf_left_leg = new SnTransform);
	group_left_leg->add(new SnModel(bender_left_leg));
	group_left_leg->top<SnModel>()->color(GsColor::gray);
	transf_left_leg->get().translation(1.2f, -8.32f, 0);

	group_right_leg->separator(true);
	group_right_leg->add(transf_right_leg = new SnTransform);
	group_right_leg->add(new SnModel(bender_right_leg));
	group_right_leg->top<SnModel>()->color(GsColor::gray);
	transf_right_leg->get().translation(-1.2f, -8.4f, 0);

	//assembles each group onto the torso group to make the full model
	group_torso->add_group(group_head);
	group_torso->add_group(group_left_arm);
	group_torso->add_group(group_right_arm);
	group_torso->add_group(group_left_leg);
	group_torso->add_group(group_right_leg);

	

	//this is the grass texture 
	//some change
	//Changing material seems to do some stuff. PerGroup crashes the program. Flat doesnt work well, but i could get smooth to kind of work.
	
		GsModel::Group& g = *ground.G.push();
		g.fi = 0;
		g.fn = ground.F.size();
		g.dmap = new GsModel::Texture;
		g.dmap->fname.set("../../textures/grass.png");
		ground.M.push().init();

		
		ground.T.size(4);
		ground.T[0].set(1, 1);
		ground.T[1].set(1, 0);
		ground.T[2].set(0, 0);
		ground.T[3].set(0, 1);
		
		ground.set_mode(GsModel::Flat, GsModel::PerGroupMtl);
		ground.textured = true;
	
	
	//rootg()->add(group_head);
	rootg()->add(group_torso);
	//rootg()->add(group_left_arm);
	//rootg()->add(group_right_arm);
	//rootg()->add(group_left_leg);
	//rootg()->add(group_right_leg);
	
	//gsout << "Normals: " << Bender->N.size() << gsnl;
	//gsout << "Vertices: " << Bender->V.size() << gsnl;

}

GsCamera* camera = new GsCamera;

void switch_camera(int camera_num) {
	
	/*
	double current, start = gs_time();
	
	switch (camera_num)
	{
	case 0:

		break;
	
		

	case 1:
		
		current = gs_time() - start;

		do
		{
			current = gs_time() - start;
			camera->eye.x += 0.001f;
			camera->center.x += 0.001f;
			camera->up.x += 0.001f;
		
			

		} while (current<3.0f);

	}
	*/
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
	int ret = WsViewer::handle_keyboard ( e ); // 1st let system check events
	if ( ret ) return ret;

	int camera_num = 0;

	switch ( e.key )
	{	case GsEvent::KeyEsc : gs_exit(); return 1;
		case 'n' : { bool b=!_nbut->value(); _nbut->value(b); show_normals(b); return 1; }
		case ' ': {camera_num = ++camera_num % 2; switch_camera(camera_num); return 1; }
		
		
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
