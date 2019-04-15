//Author: Addison Stotler
//airGauge.cpp

#include "airGauge.h"

AirGauge::AirGauge()
{
    airTotal = 0;

}

//TODO: Add lines of code that call setAirRecharge(int airUsed)
float AirGauge::airTapX(bool isLeftArrow)
{
    if(isLeftArrow == true)
    {
	airTotal += 25;
	return 9.6;
    }
    airTotal += 25;
    return -9.6;
}

float AirGauge::airTapY(bool isDownArrow)
{
    if(isDownArrow == true)
    {
	airTotal += 25;
	return -35.6;
    }
    airTotal += 25;
    return 35.6;
}

float AirGauge::airHoldX(bool isLeftArrow)
{
    if(isLeftArrow == true)
    {
	airTotal += 2;
	return 4;
    }
    airTotal += 2;
    return -4;
}

float AirGauge::airHoldY(bool isDownArrow, float gravity)
{
    if(isDownArrow == true)
    {
	airTotal += 2;
	return -5;
    }
    airTotal += 2;
    return 5;
}

int AirGauge::getAirTotal()
{
    return airTotal;
}

void AirGauge::resetTotal()
{
    airTotal = 0;
}

