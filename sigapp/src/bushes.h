#pragma once
#include "my_viewer.h"
# include <sigogl/ui_button.h>
# include <sigogl/ui_radio_button.h>
# include <sig/sn_primitive.h>
# include <sig/sn_transform.h>
# include <sig/sn_manipulator.h>
# include <sigogl/ws_run.h>

void init_bushes(GsModel& bush1, GsModel& bush2, GsModel& bush3, GsModel& bush4,
	GsModel& bush5, GsModel& bush6, GsModel& bush7, GsModel& bush8, GsModel& bush9) {

	bush1.load_obj("../../objects/LS13_01.obj");
	bush2.load_obj("../../objects/LS13_02.obj");
	bush3.load_obj("../../objects/LS13_03.obj");
	bush4.load_obj("../../objects/LS13_04.obj");
	bush5.load_obj("../../objects/LS13_05.obj");
	bush6.load_obj("../../objects/LS13_06.obj");
	bush7.load_obj("../../objects/LS13_07.obj");
	bush8.load_obj("../../objects/LS13_08.obj");
	bush9.load_obj("../../objects/LS13_09.obj");
}
