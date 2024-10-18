#include "main.h"

#include "tutrc_harurobo_lib/stdio.hpp"
#include "tutrc_harurobo_lib/can.hpp"
#include "tutrc_harurobo_lib/c610.hpp"
#include "tutrc_harurobo_lib/ps3.hpp"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern CAN_HandleTypeDef hcan1;

using namespace tutrc_harurobo_lib;

extern "C" void cppmain(){
	enable_printf(&huart2);

	CAN can1(&hcan1);
	C610 c610(can1);
	PS3 ps3(&huart1);

	float v_target = 0;
	float Kp = 50 ;

	while (true){
		ps3.update();

		if (ps3.get_key_down(PS3::Key::UP)) {
			v_target += 10;
		}
		if(ps3.get_key_down(PS3::Key::DOWN)){
			v_target -= 10;
		}

		float v_actual = c610.get_rps(C610::ID::ID2);
		float e = v_target - v_actual;
		float c = Kp * e ;

		c610.set_current(C610::ID::ID2, c);

		c610.transmit();

		printf("v_target:%f, v_actual:%f\r\n", v_target, v_actual);
		HAL_Delay(10);
	}
}
