#pragma once
#include "my_viewer.h"
# include <sigogl/ui_button.h>
# include <sigogl/ui_radio_button.h>
# include <sig/sn_primitive.h>
# include <sig/sn_transform.h>
# include <sig/sn_manipulator.h>
# include <sigogl/ws_run.h>

void init_bushes(GsModel& bush1, GsModel& bush2) {

	bush1.load_obj("../../objects/LS13_01.obj");
	bush2.load_obj("../../objects/LS13_02.obj");
}
