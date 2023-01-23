#ifndef  __MYSTYLE_H
#define __MYSTYLE_H

#include "TROOT.h"
#include "TStyle.h"
#include "TLatex.h"

#include <iostream>
#include <string>

void SetMyStyle();
void SetMyStyleForTopPlots();
void SetMyStyleForShowPlots();

void myCustomText(Double_t, Double_t, std::string, Float_t, Color_t);
void myATLASLabel(Double_t, Double_t, char*, Color_t);
void PrintRightLabel(std::string, Float_t X1=0.97, Float_t X2=0.55);
void PrintLeftLabel(std::string name, Float_t X1=0.03, Float_t X2=0.50);

#endif // __MYSTYLE_H
