//
// MyStyle, based on the ATLAS style file
//
#include "MyStyle.h"

//=====================================================================
// SetMyStyle()
//=====================================================================
void SetMyStyle() {
  gStyle->SetHistLineWidth(0);
  gStyle->SetMarkerStyle(1);
  gStyle->SetStatW(0.3);
  gStyle->SetStatH(0.2);
  gStyle->SetPalette(1);
  gStyle->SetLegendBorderSize(0);   // remove shade from stat box
  gROOT->ForceStyle();
}

//=====================================================================
// SetMyStyleForTopPlots()
//=====================================================================
void SetMyStyleForTopPlots() {
  SetMyStyle();
  gROOT->ForceStyle();
}

//=====================================================================
// SetMyStyleForShowPlots()
//=====================================================================
void SetMyStyleForShowPlots() {
  SetMyStyle();
  gStyle->SetOptStat(111111111);
  gStyle->SetLabelSize(0.03,"z");
  // gStyle->SetPadRightMargin(0.10);
   gROOT->ForceStyle();
}

//=====================================================================
// myCustomText()
//=====================================================================
void myCustomText(Double_t x, Double_t y, std::string text, Float_t size, Color_t color) {
  TLatex l;
  l.SetTextSize(size); 
  l.SetNDC();
  l.SetTextColor(color);
  l.DrawLatex(x,y,text.c_str());
}

//=====================================================================
// myATLASLabel()
//=====================================================================
void myATLASLabel(Double_t x, Double_t y, char* text, Color_t color) {
  TLatex l; //l.SetTextAlign(12); l.SetTextSize(tsize); 
  l.SetNDC();
  l.SetTextFont(72);
  l.SetTextColor(color);

  double delx = 0.115;

  l.DrawLatex(x,y,"ATLAS");
  if (text) {
    TLatex p; 
    p.SetNDC();
    p.SetTextFont(42);
    p.SetTextColor(color);
    p.DrawLatex(x+delx,y,text);
    //    p.DrawLatex(x,y,"#sqrt{s}=900GeV");
  }
}

//=====================================================================
// PrintRightLabel()
//=====================================================================
void PrintRightLabel(std::string name, Float_t tsize, Float_t X1, Float_t X2) {
  TLatex l;
  l.SetTextAlign(12);
  l.SetTextSize(tsize);
  l.SetTextAngle(-90); 
  l.SetNDC();
  l.DrawLatex(X1, X2, name.c_str());
}

//=====================================================================
// PrintLeftLabel()
//=====================================================================
void PrintLeftLabel(std::string name, Float_t tsize, Float_t X1, Float_t X2) {
  TLatex l;
  l.SetTextAlign(12);
  l.SetTextSize(tsize);
  l.SetTextAngle(90); 
  l.SetNDC();
  l.DrawLatex(X1, X2, name.c_str());
}
