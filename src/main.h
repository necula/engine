#ifndef _MAIN_H
#define _MAIN_H

#include <stdlib.h>
#include <cstdio>
#include <iostream>
#include "esUtil.h"

#include "Camera\Camera.h"
#include "Vector3D.h"
#include "Resources\ResourceManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Postprocess.h"

#define MOUSE_LEFTBTN 1
#define MOUSE_RIGHTBTN 2

int old_x, old_y;

int s_windowWidth, s_windowHeight;

float *Vector3D::arr;

CCamera *camera;
CResourceManager *rm;
CScene *scene;
ESContext esContext;
CPostprocess *pp;

static int key_w, key_a, key_s, key_d,
			key_i, key_j, key_k, key_l,
			key_space, key_x, key_p;

float movement_speedup = 1.0f;
float totalTime;

#endif