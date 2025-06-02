/*
 * dc_control.h
 *
 *  Created on: Jan. 25, 2025
 *      Author: fabiop
 */

#ifndef DC_CONTROL_H_
#define DC_CONTROL_H_

#include <psu_global.h>

psu_state_t dc_control_fsm(psu_state_t new_state);
psu_state_t dc_check_status(void);

#endif /* DC_CONTROL_H_ */
