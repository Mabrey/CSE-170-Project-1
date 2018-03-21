# include "my_viewer.h"
# include <sigogl/ui_button.h>
# include <sigogl/ui_radio_button.h>
# include <sig/sn_primitive.h>
# include <sig/sn_transform.h>
# include <sig/sn_manipulator.h>

# include <sigogl/ws_run.h>


void init_bender_models(GsModel& bender_head, GsModel& bender_torso,
	GsModel& bender_left_arm, GsModel& bender_right_arm, GsModel& bender_left_leg,
	GsModel& bender_right_leg) {

	bender_head.load_obj("../../objects/bender_head.obj");
	bender_torso.load_obj("../../objects/bender_torso.obj");
	bender_left_arm.load_obj("../../objects/bender_left_arm.obj");
	bender_right_arm.load_obj("../../objects/bender_right_arm.obj");
	bender_left_leg.load_obj("../../objects/bender_left_leg.obj");
	bender_right_leg.load_obj("../../objects/bender_right_leg.obj");

}