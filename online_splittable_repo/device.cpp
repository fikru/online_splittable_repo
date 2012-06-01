#include "device.h"
#include "global.h"
#include <string>

#include <iostream>
#include <vector>

using namespace std;

device::device(int ID, string name, double capacity, double power, int state, int nr_device)
{
	this->ID = ID;
	this->name = name;
	this->capacity = capacity;
	this->power = power;
	this->state = state;
	this->nr_device = nr_device;
}

device::device(int ID, string name, double capacity, double power, int state)
{
	this->ID = ID;
	this->name = name;
	this->capacity = capacity;
	this->power = power;
	this->state = state;
}

int device::getID()
{
	return this->ID;
}

string device::getName()
{
	return this->name;
}

double device::getCapacity()
{
	return this->capacity;
}

double device::getPower()
{
	return this->power;
}

bool device::getState()
{
	return this->state;
}

int device::getNr_device()
{
	return this->nr_device;
}

void device::setCapacity(double capacity)
{
	this->capacity = capacity;
}

void device::setPower(double power)
{
	this->power = power;
}

void device::setState(bool state)
{
	this->state = state;
}
