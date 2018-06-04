#include "MovingObject.h"



void MovingObject::UpdatePosition()
{
	pos += vel;
}