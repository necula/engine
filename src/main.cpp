#ifdef _WIN32

#include "main.h"

int Init ( ESContext *esContext )
{
	Vector3D::arr = new float[3];

	s_windowWidth = esContext->width;
	s_windowHeight = esContext->height;
	
	camera = new CCamera();
	camera->moveBackward(4.0f);

	rm = new CResourceManager();
	rm->loadResources("..\\data\\rm.txt");
	
	scene = new CScene("..\\data\\scene.txt", rm);

	pp = new CPostprocess();
	
	totalTime = 0;

	return TRUE;
}


void Update ( ESContext *esContext, float deltaTime )
{
	totalTime += deltaTime;
	if(!key_p) // pause?
	{
		// Camera update
		if(key_w)
			camera->moveForward(deltaTime * movement_speedup * 15.0f);  
		if(key_a)
			camera->moveLeft(deltaTime * movement_speedup * 15.0f);
		if(key_s)
			camera->moveBackward(deltaTime * movement_speedup * 15.0f);
		if(key_d)
			camera->moveRight(deltaTime * movement_speedup * 15.0f);
		if(key_space)
			camera->moveAbsoluteUp(deltaTime * movement_speedup * 10.0f);
		if(key_x)
			camera->moveAbsoluteDown(deltaTime * movement_speedup * 10.0f);

		if(key_i)
			camera->rotateX(deltaTime * -100.0f);
		if(key_k)
			camera->rotateX(deltaTime * 100.0f);
		if(key_j)
			camera->rotateY(deltaTime * -100.0f);
		if(key_l)
			camera->rotateY(deltaTime * 100.0f);
		

		scene->update(deltaTime);
	}
}

void Draw ( ESContext *esContext )
{
	if(!key_p) // pause?
	{
		if(pp->isActive())
			pp->renderToTexture();

		scene->draw();
		
		if(pp->isActive())
			pp->renderFilter();

		eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
	}
}


void ShutDown ( ESContext *esContext )
{
   
}

void Key ( ESContext *esContext, unsigned char key, int x, int y)
{
	//printf("%i %c\n", key, key);

	switch ( key )
	{
		case 'w':
		  key_w = 1; 
		  break;

		case 's':
		  key_s = 1;
		  break;

		case 'a':
		  key_a = 1;
		  break;

		case 'd':
		  key_d = 1;
		  break;

		case 'i':
			key_i = 1;
		  break;

		case 'k':
			key_k = 1;
		  break;

		case 'j':
		  key_j = 1;
		  break;

		case 'l':
		  key_l = 1;
		  break;

		case 32:
		  key_space = 1;
		  break;

		case 'x':
		  key_x = 1;
		  break;

		case 'p':
			key_p = 1;
		break;

		case '1':
			{
				pp->activateFilter(1);	
			}
		break;

		case '2':
		{
			pp->activateFilter(2);
		}
		break;

		case '3':
		{
			pp->activateFilter(3);
		}
		break;

		case 33: // ! (shift+1)
		{
			delete(scene);
			scene = new CScene("..\\data\\scene.txt", rm);
			camera->reset();
			camera->moveBackward(4.0f);
		}
		break;

		case 64: // @ (shift+2)
		{
			delete(scene);
			scene = new CScene("..\\data\\scene2.txt", rm);
			camera->reset();
			camera->moveBackward(4.0f);
		}
		break;

		case 9: // tab
		{
			movement_speedup = 4.0f;
		}
		break;

		case 033: 
		   ShutDown( esContext );
		   exit( 0 );
		   break;
		}
}

void KeyUp ( ESContext *esContext, unsigned char key, int x, int y)
{	

	switch ( key )
	{
		case 'w': case 'W':
		  key_w = 0; 
		  break;

		case 's': case 'S':
		  key_s = 0;
		  break;

		case 'a': case 'A':
		  key_a = 0;
		  break;

		case 'd': case 'D':
		  key_d = 0;
		  break;

		case 'i': case 'I':
			key_i = 0;
		  break;

		case 'k': case 'K':
			key_k = 0;
		  break;

		case 'j': case 'J':
		  key_j = 0;
		  break;

		case 'l': case 'L':
		  key_l = 0;
		  break;

		case 32:
		  key_space = 0;
		  break;

		case 'x': case 'X':
		  key_x = 0;
		  break;

		case 'p': case 'P':
			key_p = 0;
		break;

		case 9: // tab
		{
			movement_speedup = 1.0f;
		}
		break;
	}
}

void MouseMove ( ESContext *esContext, unsigned char key, int x, int y)
{

	if(key == MOUSE_LEFTBTN)
	{
		camera->rotateX((y - old_y) * 0.5f);
		camera->rotateY((x - old_x) * 0.5f);
	}
	old_x = x;
	old_y = y;
}


int main ( int argc, char *argv[] )
{
   

   esInitContext ( &esContext );
   esCreateWindow ( &esContext, "Engine", 1600, 900, ES_WINDOW_RGB | ES_WINDOW_DEPTH);
   
   if ( !Init ( &esContext ) )
      return 0;

   esRegisterDrawFunc ( &esContext, Draw );
   esRegisterUpdateFunc ( &esContext, Update );
   esRegisterKeyFunc(&esContext, Key);
   esRegisterKeyFuncUp(&esContext, KeyUp);

   old_x = 0, old_y = 0;
   esRegisterMouseMoveFunc(&esContext, MouseMove);
   
   esMainLoop ( &esContext );

   ShutDown ( &esContext );
}

#endif