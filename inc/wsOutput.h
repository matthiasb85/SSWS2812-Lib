/*
 * wsOutput.h
 *
 *  Created on: 07.10.2014
 *      Author: Matthias Beckert <beckert.matthias@googlemail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef WSOUTPUT_H_
#define WSOUTPUT_H_

extern void wsOutput_Init(void);
extern void wsOutput_Enable();
extern void wsOutput_Disable();
extern uint32_t wsOutput_DetectLeds();

#endif /* WSOUTPUT_H_ */
