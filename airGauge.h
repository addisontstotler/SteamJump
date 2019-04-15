//Author: Addison Stotler
//airGauge.h

#ifndef AIR_GAUGE
#define AIR_GAUGE

#include <math.h>

class AirGauge
{
  public:
  AirGauge();

    float airTapX(bool isLeftArrow);
    float airTapY(bool isDownArrow);
    float airHoldX(bool isLeftArrow);
    float airHoldY(bool isDownArrow, float gravity);

    int getAirTotal();
    void resetTotal();
    
  private:
    int airTotal;

  
};

#endif
