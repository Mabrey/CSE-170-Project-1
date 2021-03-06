/*
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!DISCLAIMER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


IT TAKES LIKE 20 SECONDS TO LAUNCH DUE TO NUMBER OF VERTICES; 
IT DOES WORK, JUST GIVE IT SOME TIME


!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!DISCLAIMER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/
# include "my_viewer.h"
# include "bender.h"
#include "bushes.h"

# include <sigogl/ui_button.h>
# include <sigogl/ui_radio_button.h>
# include <sig/sn_primitive.h>
# include <sig/sn_transform.h>
# include <sig/sn_manipulator.h>

# include <sigogl/ws_run.h>

int camera_num = 0, llRotNum = 0, rlRotNum = 0, laRotNum = 0, raRotNum = 0, headRotNum = 0, orientation = 0, shadowOrientation = 0;
SnTransform* transf_head, *transf_torso, *transf_left_arm, *transf_right_arm, *transf_left_leg, *transf_right_leg, *transf_shadow,
*transf_shadow_head, *transf_shadow_torso, *transf_shadow_left_arm, *transf_shadow_right_arm, *transf_shadow_left_leg, *transf_shadow_right_leg;
bool legIncreasing, armIncreasing;
bool cameraMoving = false;

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

void MyViewer::build_scene()
{

	GsModel* bender_head = new GsModel;
	GsModel* bender_torso = new GsModel;
	GsModel* bender_left_arm = new GsModel;
	GsModel* bender_right_arm = new GsModel;
	GsModel* bender_left_leg = new GsModel;
	GsModel* bender_right_leg = new GsModel;

	GsModel* bush1 = new GsModel;
	GsModel* bush2 = new GsModel;

	SnGroup *group_head = new SnGroup;
	SnGroup *group_torso = new SnGroup;
	SnGroup *group_left_arm = new SnGroup;
	SnGroup *group_right_arm = new SnGroup;
	SnGroup *group_left_leg = new SnGroup;
	SnGroup *group_right_leg = new SnGroup;

	SnGroup *group_shadow = new SnGroup;
	SnGroup *group_shadow_head = new SnGroup;
	SnGroup *group_shadow_torso = new SnGroup;
	SnGroup *group_shadow_left_arm = new SnGroup;
	SnGroup *group_shadow_right_arm = new SnGroup;
	SnGroup *group_shadow_left_leg = new SnGroup;
	SnGroup *group_shadow_right_leg = new SnGroup;

	SnGroup *group_bush1 = new SnGroup;
	SnGroup *group_bush2 = new SnGroup;

	SnTransform *transf_bush1, *transf_bush2;

	SnModel*sn = new SnModel;
	GsModel& ground = *sn->model();
	rootg()->add(sn);

	init_bender_models(*bender_head, *bender_torso, *bender_left_arm,
		*bender_right_arm, *bender_left_leg, *bender_right_leg);

	init_bushes(*bush1, *bush2);

	//this is the bender model
	{
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

	group_shadow_head->separator(true);
	group_shadow_head->add(transf_head = new SnTransform);
	group_shadow_head->add(new SnModel(bender_head));
	group_shadow_head->top<SnModel>()->color(GsColor::black);
	transf_head->get().translation(0, 2.7f, 0);

	group_shadow_torso->separator(true);
	group_shadow_torso->add(transf_shadow = new SnTransform);
	group_shadow_torso->add(new SnModel(bender_torso));
	group_shadow_torso->top<SnModel>()->color(GsColor::black);
	transf_shadow->get().translation(0, 8.25f, 0);


	group_shadow_left_arm->separator(true);
	group_shadow_left_arm->add(transf_shadow_left_arm = new SnTransform);
	group_shadow_left_arm->add(new SnModel(bender_left_arm));
	group_shadow_left_arm->top<SnModel>()->color(GsColor::black);
	transf_shadow_left_arm->get().translation(4.5f, 0.8f, 0);


	group_shadow_right_arm->separator(true);
	group_shadow_right_arm->add(transf_shadow_right_arm = new SnTransform);
	group_shadow_right_arm->add(new SnModel(bender_right_arm));
	group_shadow_right_arm->top<SnModel>()->color(GsColor::black);
	transf_shadow_right_arm->get().translation(-4.4f, 0.92f, 0);

	group_shadow_left_leg->separator(true);
	group_shadow_left_leg->add(transf_shadow_left_leg = new SnTransform);
	group_shadow_left_leg->add(new SnModel(bender_left_leg));
	group_shadow_left_leg->top<SnModel>()->color(GsColor::black);
	transf_shadow_left_leg->get().translation(1.2f, -8.32f, 0);

	group_shadow_right_leg->separator(true);
	group_shadow_right_leg->add(transf_shadow_right_leg = new SnTransform);
	group_shadow_right_leg->add(new SnModel(bender_right_leg));
	group_shadow_right_leg->top<SnModel>()->color(GsColor::black);
	transf_shadow_right_leg->get().translation(-1.2f, -8.4f, 0);

}

	//assembles each group onto the torso group to make the full model
	group_torso->add_group(group_head);
	group_torso->add_group(group_left_arm);
	group_torso->add_group(group_right_arm);
	group_torso->add_group(group_left_leg);
	group_torso->add_group(group_right_leg);

	
	group_shadow_torso->add_group(group_shadow_head);
	group_shadow_torso->add_group(group_shadow_left_arm);
	group_shadow_torso->add_group(group_shadow_right_arm);
	group_shadow_torso->add_group(group_shadow_left_leg);
	group_shadow_torso->add_group(group_shadow_right_leg);
	transf_shadow->get().translation(0.0f, 0.1f, -8);
	transf_shadow->get().lcombscale(1, .01f, 1);
	group_shadow_torso->top<SnModel>()->color(GsColor::black);

	GsMat rot;
	GsMat translate;
	GsMat transform = GsMat();
	GsMat currentMat = transf_shadow->get();
	//translate.translation((float)sin(GS_TORAD(orientation)), 0.0f, (float)cos(GS_TORAD(orientation)));
	rot.rotx(GS_TORAD(-90));
	transform.mult(currentMat, rot);
	transf_shadow->set(transform);
	

	//This is the bush models 
	{
		group_bush1->separator(true);
		group_bush1->add(transf_bush1 = new SnTransform);
		group_bush1->add(new SnModel(bush1));
		group_bush1->top<SnModel>()->color(GsColor::green);
		transf_bush1->get().translation(8, 0, 8);
		transf_bush1->get().lcombscale(6, 6, 6);

		group_bush2->separator(true);
		group_bush2->add(transf_bush2 = new SnTransform);
		group_bush2->add(new SnModel(bush2));
		group_bush2->top<SnModel>()->color(GsColor::green);
		transf_bush2->get().translation(-12, 0, 8);
		transf_bush2->get().lcombscale(6, 6, 6);
	}

	//this is the grass model and texture
	{
		ground.V.size(4);
		ground.F.size(2);
		ground.N.size(2);
		ground.V[0] = GsVec(30, 0, 30);
		ground.V[1] = GsVec(30, 0, -30);
		ground.V[2] = GsVec(-30, 0, -30);
		ground.V[3] = GsVec(-30, 0, 30);
		ground.F[0] = GsModel::Face(0, 1, 2);
		ground.F[1] = GsModel::Face(2, 3, 0);
		ground.N[0] = GsVec(0, 1, 0);
		ground.N[1] = GsVec(0, 1, 0);
		ground.culling = false;
	

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
	}

	rootg()->add(group_torso);
	rootg()->add(group_shadow_torso);
	rootg()->add(group_bush1);
	rootg()->add(group_bush2);
	
}

void MyViewer::move_left_leg() {
	
	GsMat rot;
	GsMat translate;
	GsMat transform = GsMat();
	//GsMat headMat = transf_head->get();

	translate.translation(0.0f, -3.3f, 0);
	rot.rotx(GS_TORAD(llRotNum));
	transform.mult(transform, translate);
	transform.mult(transform, rot);
	translate.translation(1.2f, -5.0f, 0);
	transform.mult(transform, translate);


	transf_left_leg->set(transform);
	
}

void MyViewer::move_right_leg() {

	GsMat rot;
	GsMat translate;
	GsMat transform = GsMat();
	//GsMat headMat = transf_head->get();

	translate.translation(0.0f, -3.4f, 0);
	rot.rotx(GS_TORAD(rlRotNum));
	transform.mult(transform, translate);
	transform.mult(transform, rot);
	translate.translation(-1.2f, -5.0f, 0);
	transform.mult(transform, translate);

	

	transf_right_leg->set(transform);
}

void MyViewer::move_left_arm() {


	GsMat rot;
	GsMat translate;
	GsMat transform = GsMat();
	translate.translation(2.0f, 1.0f, 0);
	
	transform.mult(transform, translate);
	rot.rotx(GS_TORAD(laRotNum));
	//rot.rotz(GS_TORAD(-60));
	transform.mult(transform, rot);
	//rot.roty(GS_TORAD(laRotNum));
	rot.rotz(GS_TORAD(-75));
	transform.mult(transform, rot);
	translate.translation(2.7f, 0.0f, 0);
	
	transform.mult(transform, translate);
	transf_left_arm->set(transform);
	transf_shadow_left_arm->set(transform);
}

void MyViewer::move_right_arm() {

	GsMat rot;
	GsMat translate;
	GsMat transform = GsMat();
	translate.translation(-2.0f, 1.0f, 0);

	transform.mult(transform, translate);
	rot.rotx(GS_TORAD(raRotNum));
	//rot.rotz(GS_TORAD(-60));
	transform.mult(transform, rot);
	//rot.roty(GS_TORAD(laRotNum));
	rot.rotz(GS_TORAD(75));
	transform.mult(transform, rot);
	translate.translation(-2.7f, 0.0f, 0);

	transform.mult(transform, translate);
	transf_right_arm->set(transform);
	transf_shadow_right_arm->set(transform);
}

void MyViewer::move_head() {

	GsMat rot;
	GsMat translate;
	GsMat transform = GsMat();
	//GsMat headMat = transf_head->get();

	translate.translation(0.0f, 2.7f, 0);
	rot.roty(GS_TORAD(headRotNum));
	//llegMat.mult(llegMat, translate);
	transform.mult(transform, rot);
	transform.mult(transform, translate);

	//translate.translation(1.2f, -5.0f, 0);
	//llegMat.mult(llegMat, translate);

	transf_head->set(transform);
}

void MyViewer::move_forward() {
	GsMat rot;
	GsMat translate;
	GsMat transform = GsMat();
	GsMat currentMat = transf_torso->get();
	translate.translation(0, 0, 1);
	transform.mult(currentMat, translate);
	transf_torso->set(transform);

	rot = GsMat();
	transform = GsMat();
	currentMat = transf_shadow->get();
	
	translate.translation((float)sin(GS_TORAD(shadowOrientation)), -(float)cos(GS_TORAD(shadowOrientation)), 0);
	//gsout << "Orientation: " << orientation << gsnl;
	transform.mult(currentMat, translate);
	transf_shadow->set(transform);

	//transform.mult(currentMat, rot);
	//transf_shadow->set(transform);

	llRotNum = llRotNum + (legIncreasing ? 4 : -4);
	rlRotNum = rlRotNum + (legIncreasing ? -4 : 4);
	laRotNum = laRotNum + (armIncreasing ? -3 : 3);
	raRotNum = raRotNum + (armIncreasing ? 3 : -3);
	if (llRotNum >= 20 || llRotNum <= -20)
		legIncreasing = !legIncreasing;
	
	if (laRotNum >= 15 || laRotNum <= -15)
		armIncreasing = !armIncreasing;

	move_left_leg();
	move_right_leg();
	move_left_arm();
	move_right_arm();
}

void MyViewer::move_backward() {
	GsMat rot;
	GsMat translate;
	GsMat transform = GsMat();
	GsMat currentMat = transf_torso->get();
	translate.translation(0, 0, -1);
	transform.mult(currentMat, translate);
	transf_torso->set(transform);

	rot = GsMat();
	transform = GsMat();
	currentMat = transf_shadow->get();

	translate.translation(-(float)sin(GS_TORAD(shadowOrientation)), (float)cos(GS_TORAD(shadowOrientation)), 0);
	//gsout << "Orientation: " << orientation << gsnl;
	transform.mult(currentMat, translate);
	transf_shadow->set(transform);

	//transform.mult(currentMat, rot);
	//transf_shadow->set(transform);

	llRotNum = llRotNum + (legIncreasing ? 4 : -4);
	rlRotNum = rlRotNum + (legIncreasing ? -4 : 4);
	laRotNum = laRotNum + (armIncreasing ? -3 : 3);
	raRotNum = raRotNum + (armIncreasing ? 3 : -3);
	if (llRotNum >= 20 || llRotNum <= -20)
		legIncreasing = !legIncreasing;

	if (laRotNum >= 15 || laRotNum <= -15)
		armIncreasing = !armIncreasing;

	move_left_leg();
	move_right_leg();
	move_left_arm();
	move_right_arm();
}

void MyViewer::rotate_torso_left() {
	shadowOrientation += 5;
	GsMat rot;
	GsMat translate;
	GsMat transform = GsMat();
	GsMat currentMat = transf_torso->get();
	rot.roty(GS_TORAD(5));
	transform.mult(currentMat, rot);
	transf_torso->set(transform);


	if (llRotNum >= -4 && llRotNum <= 4 && rlRotNum >= -4 && rlRotNum <= 4 && laRotNum >= -3 && laRotNum <= 3 && raRotNum >= -3 && raRotNum <= 3)
	{
		llRotNum = llRotNum + (legIncreasing ? -4 : 4);
		rlRotNum = rlRotNum + (legIncreasing ? 4 : -4);
		laRotNum = laRotNum + (armIncreasing ? 3 : -3);
		raRotNum = raRotNum + (armIncreasing ? -3 : 3);

		if (llRotNum >= 4 || llRotNum <= -4) legIncreasing = !legIncreasing;
		if (laRotNum >= 3 || laRotNum <= -3) armIncreasing = !armIncreasing;
	}
	else if (llRotNum < -4 && rlRotNum > 4)
	{
		llRotNum += 8;
		rlRotNum -= 8;
	}
	else if (llRotNum > 4 && rlRotNum < -4)
	{
		rlRotNum += 8;
		llRotNum -= 8;
	}
	else if (laRotNum < -3 && raRotNum > 3)
	{
		laRotNum += 6;
		raRotNum -= 6;
	}
	else if (laRotNum > 3 && raRotNum < -3)
	{
		raRotNum += 6;
		laRotNum -= 6;
	}
	
	move_left_leg();
	move_right_leg();
	move_left_arm();
	move_right_arm();
}

void MyViewer::rotate_torso_right() {
	shadowOrientation -= 5;
	GsMat rot;
	GsMat translate;
	GsMat transform = GsMat();
	GsMat currentMat = transf_torso->get();
	rot.roty(GS_TORAD(-5));
	transform.mult(currentMat, rot);
	transf_torso->set(transform);

	if (llRotNum >= -4 && llRotNum <= 4 && rlRotNum >= -4 && rlRotNum <= 4 && laRotNum >= -3 && laRotNum <= 3 && raRotNum >= -3 && raRotNum <= 3)
	{
		llRotNum = llRotNum + (legIncreasing ? 4 : -4);
		rlRotNum = rlRotNum + (legIncreasing ? -4 : 4);
		laRotNum = laRotNum + (armIncreasing ? -3 : 3);
		raRotNum = raRotNum + (armIncreasing ? 3 : -3);

		if (llRotNum >= 4 || llRotNum <= -4) legIncreasing = !legIncreasing;
		if (laRotNum >= 3 || laRotNum <= -3) armIncreasing = !armIncreasing;
	}
	else if (llRotNum < -4 && rlRotNum > 4)
	{
		llRotNum += 8;
		rlRotNum -= 8;
	}
	else if (llRotNum > 4 && rlRotNum < -4)
	{
		rlRotNum += 8;
		llRotNum -= 8;
	}
	else if (laRotNum < -3 && raRotNum > 3)
	{
		laRotNum += 6;
		raRotNum -= 6;
	}
	else if (laRotNum > 3 && raRotNum < -3)
	{
		raRotNum += 6;
		laRotNum -= 6;
	}

	move_left_leg();
	move_right_leg();
	move_left_arm();
	move_right_arm();
}


void MyViewer::move_shadow() {
	//orientation -= 5;
	GsMat rot;
	GsMat translate;
	GsMat transform = GsMat();
	GsMat currentMat = transf_torso->get();
	rot.roty(GS_TORAD(-5));
	transform.mult(currentMat, rot);
	transf_torso->set(transform);

	if (llRotNum >= -4 && llRotNum <= 4 && rlRotNum >= -4 && rlRotNum <= 4 && laRotNum >= -3 && laRotNum <= 3 && raRotNum >= -3 && raRotNum <= 3)
	{
		llRotNum = llRotNum + (legIncreasing ? 4 : -4);
		rlRotNum = rlRotNum + (legIncreasing ? -4 : 4);
		laRotNum = laRotNum + (armIncreasing ? -3 : 3);
		raRotNum = raRotNum + (armIncreasing ? 3 : -3);

		if (llRotNum >= 4 || llRotNum <= -4) legIncreasing = !legIncreasing;
		if (laRotNum >= 3 || laRotNum <= -3) armIncreasing = !armIncreasing;
	}
	else if (llRotNum < -4 && rlRotNum > 4)
	{
		llRotNum += 8;
		rlRotNum -= 8;
	}
	else if (llRotNum > 4 && rlRotNum < -4)
	{
		rlRotNum += 8;
		llRotNum -= 8;
	}
	else if (laRotNum < -3 && raRotNum > 3)
	{
		laRotNum += 6;
		raRotNum -= 6;
	}
	else if (laRotNum > 3 && raRotNum < -3)
	{
		raRotNum += 6;
		laRotNum -= 6;
	}

	move_left_leg();
	move_right_leg();
	move_left_arm();
	move_right_arm();
}

void MyViewer::switch_camera() {
	

	double current, start = gs_time();
	GsMat rotationx, rotationy;
	rotationy.roty(GS_TORAD(90));
	rotationx.rotz(GS_TORAD(45));
	rotationx.mult(rotationx, rotationy);

	camera_num %= 3;
	//gsout << camera_num;
	switch (camera_num)
	{
	case 0:

		//camera().init();
		camera().rotate(rotationx);

		render();
		ws_check();

		break;
	
	case 1:
		rotationx.zero();
		rotationy.zero();

		rotationy.roty(GS_TORAD(-90));
		rotationx.rotx(GS_TORAD(45));
		rotationx.mult(rotationx, rotationy);

		camera().rotate(rotationx);

		render();
		ws_check();

		break;
		

	case 2:
		cameraMoving = true;

		current = gs_time() - start;
		int count = 0;
		do
		{

			current = gs_time() - start;

			if (current < 3.0f) {
				
				camera().eye.y += .1f;
				
			}

			else if (current < 6.0f) {

				camera().eye.x += .2f;
				camera().eye.z -= .15f;
			}
			else if (current < 9.0f) {

				camera().eye.x -= .2f;
				camera().eye.z += .15f;
			}
			else {

				camera().eye.y -= .1f;
			}

			render();
			ws_check();

		} while (current<12.0f);
		cameraMoving = false;
		break;

	}
	
	camera_num++;
}

// Below is an example of how to control the main loop of an animation:
void MyViewer::run_animation ()
{	
	/*
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
	}	while ( m.e24>0 );*/
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

	

	switch ( e.key )
	{	case GsEvent::KeyEsc : gs_exit(); return 1;
		case 'n' : { bool b=!_nbut->value(); _nbut->value(b); show_normals(b); return 1; }
		case GsEvent::KeySpace: {
			if (!cameraMoving)
				switch_camera();
			return 1;
		}
		case 'q': { llRotNum+=2;  move_left_leg(); render(); return 1; }
		case 'a': { llRotNum-=2;  move_left_leg(); render(); return 1; }
		case 'w': { rlRotNum+=2;  move_right_leg(); render(); return 1; }
		case 's': { rlRotNum-=2;  move_right_leg(); render(); return 1; }
		case 'e': { laRotNum+=4;  move_left_arm(); render(); return 1; }
		case 'd': { laRotNum-=4;  move_left_arm(); render(); return 1; }
		case 'r': { raRotNum+=4;  move_right_arm(); render(); return 1; }
		case 'f': { raRotNum-=4;  move_right_arm(); render(); return 1; }
		case 't': { headRotNum+=2;  move_head(); render(); return 1; }
		case 'g': { headRotNum-=2;  move_head(); render(); return 1; }
		case GsEvent::KeyUp: {move_forward(); render(); return 1; }
		case GsEvent::KeyDown: {move_backward(); render(); return 1; }
		case GsEvent::KeyLeft: {rotate_torso_left(); render(); return 1; }
		case GsEvent::KeyRight: {rotate_torso_right(); render(); return 1; }

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
