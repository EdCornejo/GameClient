#include "Headers.pch"

GPS::GPS(){}

GPS::~GPS() {}

GPSPoint GPS::GetCurrentGPSPoint()
{
  return GPSPoint(0, 0);
}

std::string GPS::GetCurrentAddress()
{
  return "";
}
