// Copyright Jaakko Markus Seppala 2010

#ifndef _TRIANGUL_H
#define _TRIANGUL_H

#include "races.h"
#include "globdata.h"

#define TRIANGUL_SPHERE_CHMMR_1_RAD (3060 / SPHERE_RADIUS_INCREMENT * 2)
#define TRIANGUL_SPHERE_CHMMR_2_RAD (3610 / SPHERE_RADIUS_INCREMENT * 2)
#define TRIANGUL_SPHERE_SHOFIXTI_RAD (850 / SPHERE_RADIUS_INCREMENT * 2)

#define TRIANGUL_SPHERE_CHMMR_X		(5742)
#define TRIANGUL_SPHERE_CHMMR_Y		(8628)
#define TRIANGUL_SPHERE_SHOFIXTI_1_X (3950)
#define TRIANGUL_SPHERE_SHOFIXTI_1_Y (5850)
#define TRIANGUL_SPHERE_SHOFIXTI_2_X (6850)
#define TRIANGUL_SPHERE_SHOFIXTI_2_Y (4400)
#define TRIANGUL_SPHERE_SHOFIXTI_3_X (9600)
#define TRIANGUL_SPHERE_SHOFIXTI_3_Y (4000)

#define SPHERE_STATUSES \
    GET_GAME_STATE(TRIANGULATION_SPHERES_CHMMR),\
    GET_GAME_STATE(TRIANGULATION_SPHERES_CHMMR), \
    GET_GAME_STATE(TRIANGULATION_SPHERES_SHOFIXTI), \
    GET_GAME_STATE(TRIANGULATION_SPHERES_SHOFIXTI), \
    GET_GAME_STATE(TRIANGULATION_SPHERES_SHOFIXTI), \

#define SPHERE_COORDS \
  {TRIANGUL_SPHERE_CHMMR_X, TRIANGUL_SPHERE_CHMMR_Y}, \
  {TRIANGUL_SPHERE_CHMMR_X, TRIANGUL_SPHERE_CHMMR_Y}, \
  {TRIANGUL_SPHERE_SHOFIXTI_1_X, TRIANGUL_SPHERE_SHOFIXTI_1_Y}, \
  {TRIANGUL_SPHERE_SHOFIXTI_2_X, TRIANGUL_SPHERE_SHOFIXTI_2_Y}, \
  {TRIANGUL_SPHERE_SHOFIXTI_3_X, TRIANGUL_SPHERE_SHOFIXTI_3_Y}, \

#define SPHERE_RADIUSES \
	TRIANGUL_SPHERE_CHMMR_1_RAD,  	\
    TRIANGUL_SPHERE_CHMMR_2_RAD, 	\
    TRIANGUL_SPHERE_SHOFIXTI_RAD,\
    TRIANGUL_SPHERE_SHOFIXTI_RAD,\
    TRIANGUL_SPHERE_SHOFIXTI_RAD, \

#define SPHERE_STRINGS	\
	"Crash site?",	\
    "", \
    "Patrol1", \
    "Patrol2", \
    "Patrol3", \

void init_triangulation_spheres ();
void drawTriangulationSpheres (COUNT which_space, COUNT orz_space, RECT *pClipRect);

#endif /* _TRIANGUL_H */
