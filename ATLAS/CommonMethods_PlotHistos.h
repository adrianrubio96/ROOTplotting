#ifndef _COMMONMETHODS_PLOTHISTOS_H_
#define _COMMONMETHODS_PLOTHISTOS_H_

#include <iomanip>
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <cmath>
 
#include <algorithm>
#include <stdio.h>


#include "Rtypes.h"


/* #include "AtlasUtils.h" */
/* #include "AtlasStyle.h" */
/* #include "MyStyle.h" */

#include "TApplication.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TMath.h"
#include "TH1D.h"
#include "TH2D.h"
#include "THStack.h"
#include "TF1.h"
#include "TGraph.h"
#include "TMinuit.h"
#include "TPaveText.h"
#include "TPaveLabel.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TLine.h"

class TString;
const std::string PATH_HISTOS;
const std::string PATH_NTUPLES;

using namespace std;
using std::vector;


void PrintLabels(string comment);

void DrawHisto(TH1D* histo, TString option, Int_t color, Bool_t Normalized, Int_t marker_style, string comment);
void PrintHistoContent(TH1 *h);

vector<string> Ini_SystUpDownList(string lepton, string systFamily);

vector<string> Ini_SystList(string lepton, string systFamily);

vector<string> Ini_SystUpDownNameList(string lepton, string systFamily);

vector<string> Ini_SystNameList(string lepton, string systFamily);





#endif //_COMMONMETHODS_PLOTHISTOS_H_
