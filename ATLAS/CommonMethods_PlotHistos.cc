
// #ifndef _COMMONMETHODS_PLOTHISTOS_H_
// #define _COMMONMETHODS_PLOTHISTOS_H_

#include <sstream>
#include <iostream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <cmath>
#include <Riostream.h>

#include <algorithm>
#include <stdio.h>


#include "Rtypes.h"

#include "AtlasUtils.C"
#include "AtlasStyle.C"
#include "MyStyle.C"

#include "TApplication.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TMath.h"
#include "TProfile.h"
#include "TTree.h"
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
#include "TString.h"

using namespace std;
using std::vector;

void defineATLASstyle(){

    std::cout << "\nApplying ATLAS style settings...\n" << std::endl ;

    TStyle *atlasStyle = new TStyle("ATLAS","Atlas style");

    // use plain black on white colors
    Int_t icol=0; // WHITE
    atlasStyle->SetFrameBorderMode(icol);
    atlasStyle->SetFrameFillColor(icol);
    atlasStyle->SetCanvasBorderMode(icol);
    atlasStyle->SetCanvasColor(icol);
    atlasStyle->SetPadBorderMode(icol);
    atlasStyle->SetPadColor(icol);
    atlasStyle->SetStatColor(icol);
    //atlasStyle->SetFillColor(icol); // don't use: white fill color floa *all* objects

    // set the paper & margin sizes
    atlasStyle->SetPaperSize(20,26);
    atlasStyle->SetPadTopMargin(0.05);
    atlasStyle->SetPadRightMargin(0.05);
    atlasStyle->SetPadBottomMargin(0.16);
    atlasStyle->SetPadLeftMargin(0.16);

    atlasStyle->SetTitleYOffset(1.5);

    // use large fonts
    //Int_t font=72; // Helvetica italics
    Int_t font=42; // Helvetica
    Double_t tsize=0.05;
    atlasStyle->SetTextFont(font);

    atlasStyle->SetTextSize(tsize);
    atlasStyle->SetLabelFont(font,"x");
    atlasStyle->SetTitleFont(font,"x");
    atlasStyle->SetTitleOffset(1.2,"x");
    atlasStyle->SetLabelOffset(0.01,"x");
    atlasStyle->SetLabelFont(font,"y");
    atlasStyle->SetTitleFont(font,"y");
    atlasStyle->SetLabelFont(font,"z");
    atlasStyle->SetTitleFont(font,"z");

    atlasStyle->SetLabelSize(tsize,"x");
    atlasStyle->SetTitleSize(tsize,"x");
    atlasStyle->SetLabelSize(tsize,"y");
    atlasStyle->SetTitleSize(tsize,"y");
    atlasStyle->SetLabelSize(tsize,"z");
    atlasStyle->SetTitleSize(tsize,"z");

    // use bold lines and Lines
    atlasStyle->SetLineStyle(6);//20
    atlasStyle->SetMarkerSize(1.0);
    atlasStyle->SetHistLineWidth(2.); // default is 2.
    atlasStyle->SetLineStyleString(2,"[12 12]"); // postscr dashes

    // get rid of X error bars and error bar caps
    atlasStyle->SetErrorX(0.5);

    // do not display any of the standard histogram decorations
    atlasStyle->SetOptTitle(0);
    atlasStyle->SetOptStat(0); //(1111)
    atlasStyle->SetOptFit(0); //(1111)

    // put tick marks on top and RHS of plots
    atlasStyle->SetPadTickX(1);
    atlasStyle->SetPadTickY(1);

    atlasStyle->SetPalette(1);
    atlasStyle->SetNumberContours(99);
}

//=====================================================================
// PrintLabels()
//=====================================================================
//void PrintLabels(string channel) { PrintLabels(channel, 0.0, -0.12); }
void PrintLabels(string channel, Float_t myOffsetX, Float_t myOffsetY) {

  Int_t fEnergy = 7; // (TeV)
  Double_t fIntegratedLumi = 4.66; // (fb)

  Float_t offsetX = 0.20;
  Float_t offsetY = 0.88;

  Float_t fontSize = 0.045;

  // ATLAS label (default: top-left corner)
  //myCustomText(offsetX, offsetY, "#bf{#it{ATLAS}} internal", fontSize, kBlack);

  // lumi label
  //myCustomText(offsetX+myOffsetX, offsetY+myOffsetY, Form("#int #it{L} dt = %2.2f fb^{-1}", fIntegratedLumi), fontSize, kBlack);
  // energy label
  //myCustomText(offsetX+myOffsetX, offsetY+myOffsetY-0.07, Form("#sqrt{s} = %d TeV", fEnergy), fontSize, kBlack);

  if (channel != "") {
    char* myLabel = new char[10*channel.size()+1];
    sprintf(myLabel, Form("%s", channel.c_str()));
    myLabel[10*channel.size()] = '\0';
    myText(offsetX, offsetY-0.04, kBlack, myLabel);
    //myText(0.20, 0.82, kBlack, myLabel);
  }

}

//=====================================================================
// DrawHisto()
//=====================================================================
void DrawHisto(TH1* histo, TString option, Int_t color, Int_t marker_style, string region, Int_t ndiv)
{
  //histo->SetMinimum(0.);
  histo->SetLineColor(color);
  histo->SetMarkerStyle(marker_style);
  histo->SetMarkerColor(color);
  histo->Draw(option);
  if (ndiv != 0) histo->GetXaxis()->SetNdivisions(ndiv);

  PrintLabels(region, 0.0, -0.12);
  return;
}


//================================================
// DefineCanvas()
//================================================
TCanvas* DefineCanvas(const char name[], const char title[]) {

  TCanvas* fCanvas = new TCanvas( Form("%s", name), Form("%s", title), 800, 600);
  fCanvas->SetFillColor(0); fCanvas->SetLineColor(0); fCanvas->SetLineWidth(0);
  return fCanvas;

}


//================================================
// DefineCanvas_2pads()
//================================================
TCanvas* DefineCanvas_2pads(const char name[], const char title[]) {

  TCanvas* fCanvas = new TCanvas( Form("%s", name), Form("%s", title), 700, 660); // 800, 600);
  fCanvas->SetFillColor(0); fCanvas->SetLineColor(0); fCanvas->SetLineWidth(0);

  // preaparing the TPads
  TPad *fPad1 = new TPad("fPad1", "fPad1", 0.0, 0.30, 1.0, 1.0, 0);
  TPad *fPad2 = new TPad("fPad2", "fPad2", 0.0, 0.0, 1.0, 0.30, 0);
//   fPad1->SetTopMargin(0.06);
//   fPad1->SetBottomMargin(0.018);
//   fPad1->SetRightMargin(0.05);
//   fPad1->SetLeftMargin(0.14);

//   fPad2->SetTopMargin(0.04);
//   fPad2->SetBottomMargin(0.35);
//   fPad2->SetRightMargin(0.05);
//   fPad2->SetLeftMargin(0.14);
  fPad1->SetTopMargin(0.05);
  fPad1->SetBottomMargin(0.020);
  fPad1->SetRightMargin(0.01);
  fPad1->SetLeftMargin(0.15);

  fPad2->SetTopMargin(0.04);
  fPad2->SetBottomMargin(0.40);
  fPad2->SetRightMargin(0.01);
  fPad2->SetLeftMargin(0.15);

  fPad1->Draw(); fPad2->Draw();
  return fCanvas;

}


//================================================
// ScaleHistos()
//================================================
void ScaleHistos(TH1F* histo, Double_t SF) {
  histo->Scale(SF);
}


//================================================
// SetUnderFlow()
//================================================
void SetUnderFlow(TH1F* histo) {
  double underflow = histo->GetBinContent(0);
  double binone = histo->GetBinContent(1);
  histo->SetBinContent(1, underflow + binone);
  histo->SetBinContent(0, 0);
  double UFerror = histo->GetBinError(0);
  double bin1error = histo->GetBinError(1);
  //errors are very conservative
  histo->SetBinError(1, UFerror + bin1error);
  histo->SetBinError(0, 0);
}

//================================================
// SetOverFlow()
//================================================
void SetOverFlow(TH1F* histo) {
  int lastbin = histo->GetNbinsX();
  double overflow = histo->GetBinContent(lastbin + 1);
  double last = histo->GetBinContent(lastbin);
  histo->SetBinContent(lastbin, overflow + last);
  histo->SetBinContent(lastbin + 1, 0);
  double OFerror = histo->GetBinError(lastbin + 1);
  double lasterror = histo->GetBinError(lastbin);
  histo->SetBinError(lastbin, OFerror + lasterror);
  histo->SetBinError(lastbin + 1, 0);
}

//================================================
// ShowRatio()
//================================================
void ShowRatio(TH1F* hData, TH1F* hExp, Float_t lowerLimit, Float_t upperLimit) {

  TH1F* hRatio(0);
  TH1F* hLineInOne(0);
  hRatio = (TH1F*)hData->Clone();
  hRatio->Divide(hExp);

  // for (int ibin=0; ibin<hRatio->GetNbinsX(); ibin++)
  //cout<<"hRatio: "<<hRatio->GetBinContent(ibin+1)<<" +- "<<hRatio->GetBinError(ibin+1)<<endl;

  hLineInOne = (TH1F*)hData->Clone();
  hLineInOne->Divide(hData);
  // for (int i=0; i<hData->GetNbinsX(); i++) {
  //     //ratio: data/exp. with hData unc.
  //     hRatio->SetBinContent(i+1,0.);   hRatio->SetBinError(i+1, 0.);
  //     if (hExp->GetBinContent(i+1)!=0.) {
  //       hRatio->SetBinContent(i+1, hData->GetBinContent(i+1)/hExp->GetBinContent(i+1));
  //       hRatio->SetBinError(i+1, float(hData->GetBinError(i+1)/hExp->GetBinContent(i+1))); //take into account only hData unc.
  //     }
  //     else { hRatio->SetBinContent(i+1, -1000.); }
  //     //line in 1 with hPrediction unc.
  //     hLineInOne->SetBinContent(i+1,0.);      hLineInOne->SetBinError(i+1,0.);
  //     hLineInOne->SetBinContent(i+1, 1.);
  //     if (hExp->GetBinContent(i+1)!=0.)
  //       hLineInOne->SetBinError(i+1, float(hExp->GetBinError(i+1)*hData->GetBinContent(i+1)/TMath::Power(hExp->GetBinContent(i+1),2)));  //take into account only hPrediction unc.
  //     else hLineInOne->SetBinContent(i+1, -1000.);
  //   }

  Float_t myFontSize = 0.10;
  hRatio->SetMarkerSize(0.8);  hRatio->SetMarkerStyle(8);
  hRatio->SetMarkerSize(0.8);  hRatio->SetMarkerStyle(1);    hRatio->SetMarkerColor(1);
  hRatio->GetYaxis()->SetTitle("");
  hRatio->GetYaxis()->SetLabelSize(myFontSize);
  hRatio->GetXaxis()->SetLabelSize(myFontSize);
  hRatio->GetXaxis()->SetTitleSize(myFontSize);
  //hRatio->GetYaxis()->SetRangeUser(lowerLimit+0.001, upperLimit-0.001);
  hRatio->GetYaxis()->SetNdivisions(804);
  hRatio->GetYaxis()->SetTickLength(0.05);
  hRatio->GetXaxis()->SetTickLength(0.07);
  //hRatio->SetLineColor(1);
  hRatio->SetLineColor(hData->GetLineColor());
  hRatio->SetLineStyle(hData->GetLineStyle());
  hRatio->SetAxisRange(lowerLimit, upperLimit, "Y");
  //hRatio->SetAxisRange(lowerLimit+0.01, upperLimit-0.01, "Y");
  hRatio->Draw("P E SAME");
  // hLineInOne->SetMarkerSize(0.);
  // hLineInOne->SetMarkerStyle(0);
  // hLineInOne->SetFillColor(0);
  // hLineInOne->SetLineColor(2);
  // hLineInOne->SetFillStyle(0);//3256);
  // hLineInOne->Draw("L SAME"); //"L E2 SAME");

  TLine* myLine = new TLine();
  myLine->SetLineStyle(1);
  myLine->DrawLine(hRatio->GetXaxis()->GetXmin(), 1., hRatio->GetXaxis()->GetXmax(), 1.);
  //PrintLeftLabel("Data/Predict.", myFontSize, 0.04, 0.40);
  //PrintLeftLabel("fullSim/AF2", myFontSize, 0.04, 0.40);
  //PrintLeftLabel("TRF/DT", myFontSize, 0.04, 0.40);
  //PrintLeftLabel("RW/Nominal", myFontSize, 0.04, 0.40);
  //PrintLeftLabel("w.PS/wo.PS", myFontSize, 0.04, 0.40);
  PrintLeftLabel("Colour/Black", myFontSize, 0.04, 0.40);

}


//================================================
// ShowRatio()
//================================================
void ShowRatio_wLabel(TH1F* hData, TH1F* hExp, Float_t lowerLimit, Float_t upperLimit, string labelratio, Int_t marker_color, Int_t ndiv) {

  TH1F* hRatio(0);
  TH1F* hLineInOne(0);
  hRatio = (TH1F*)hData->Clone();
  hRatio->Divide(hExp);

  // for (int ibin=0; ibin<hRatio->GetNbinsX(); ibin++)
  //cout<<"hRatio: "<<hRatio->GetBinContent(ibin+1)<<" +- "<<hRatio->GetBinError(ibin+1)<<endl;

  hLineInOne = (TH1F*)hData->Clone();
  hLineInOne->Divide(hData);
  // for (int i=0; i<hData->GetNbinsX(); i++) {
  //     //ratio: data/exp. with hData unc.
  //     hRatio->SetBinContent(i+1,0.);   hRatio->SetBinError(i+1, 0.);
  //     if (hExp->GetBinContent(i+1)!=0.) {
  //       hRatio->SetBinContent(i+1, hData->GetBinContent(i+1)/hExp->GetBinContent(i+1));
  //       hRatio->SetBinError(i+1, float(hData->GetBinError(i+1)/hExp->GetBinContent(i+1))); //take into account only hData unc.
  //     }
  //     else { hRatio->SetBinContent(i+1, -1000.); }
  //     //line in 1 with hPrediction unc.
  //     hLineInOne->SetBinContent(i+1,0.);      hLineInOne->SetBinError(i+1,0.);
  //     hLineInOne->SetBinContent(i+1, 1.);
  //     if (hExp->GetBinContent(i+1)!=0.)
  //       hLineInOne->SetBinError(i+1, float(hExp->GetBinError(i+1)*hData->GetBinContent(i+1)/TMath::Power(hExp->GetBinContent(i+1),2)));  //take into account only hPrediction unc.
  //     else hLineInOne->SetBinContent(i+1, -1000.);
  //   }

  Float_t myFontSize = 0.11;
  // hRatio->SetMarkerSize(0.8);  hRatio->SetMarkerStyle(8);
  hRatio->SetMarkerSize(0.2);  hRatio->SetMarkerStyle(20);    hRatio->SetMarkerColor(marker_color);
  hRatio->GetYaxis()->SetTitle(Form("%s", labelratio.c_str()));
  hRatio->GetYaxis()->SetLabelSize(0.095);//(myFontSize);
  hRatio->GetYaxis()->SetTitleSize(myFontSize);
  hRatio->GetYaxis()->SetTitleOffset(0.55);
  hRatio->GetYaxis()->CenterTitle();
  hRatio->GetYaxis()->SetNdivisions(804);
  hRatio->GetYaxis()->SetTickLength(0.05);
  //hRatio->GetYaxis()->SetRangeUser(lowerLimit+0.001, upperLimit-0.001);

  hRatio->GetXaxis()->SetLabelSize(0.095);//myFontSize);
  hRatio->GetXaxis()->SetTitleSize(myFontSize);
  hRatio->GetXaxis()->SetTitleOffset(1.1);
  hRatio->GetXaxis()->SetTickLength(0.07);
  //hRatio->SetLineColor(1);
  hRatio->SetLineColor(hData->GetLineColor());
  hRatio->SetLineStyle(hData->GetLineStyle());
  hRatio->SetAxisRange(lowerLimit, upperLimit, "Y");
  if (ndiv != 0) hRatio->GetXaxis()->SetNdivisions(ndiv);
  //hRatio->SetAxisRange(lowerLimit+0.01, upperLimit-0.01, "Y");
  hRatio->Draw("P E SAME");

  TLine* myLine = new TLine();
  myLine->SetLineStyle(1);
  myLine->DrawLine(hRatio->GetXaxis()->GetXmin(), 1., hRatio->GetXaxis()->GetXmax(), 1.); //MARIA (for ttH and ttbar comp. at 8 and 13 TeV)
  // PrintLeftLabel(Form("%s", labelratio.c_str()), myFontSize, 0.04, 0.50);

}

//================================================
// ShowRatio()
//================================================
void ShowRatioWOunc_wLabel(TH1F* hData, TH1F* hExp, Float_t lowerLimit, Float_t upperLimit, string labelratio) {

  TH1F* hRatio(0);
  TH1F* hLineInOne(0);
  hRatio = (TH1F*)hData->Clone();
  hRatio->Divide(hExp);

  for (int ibin=0; ibin<hRatio->GetNbinsX(); ibin++) {
    cout<<"hHisto: "<<hData->GetBinContent(ibin+1)<<" +- "<<hData->GetBinError(ibin+1)<<" (before change)"<<endl;
    cout<<"hRatio: "<<hRatio->GetBinContent(ibin+1)<<" +- "<<hRatio->GetBinError(ibin+1)<<" (before change)"<<endl;
    if (hData->GetBinContent(ibin+1)<0.1) hRatio->SetBinContent(ibin+1, 1.);
    hRatio->SetBinError(ibin+1, 0.);
  }
  hLineInOne = (TH1F*)hData->Clone();
  hLineInOne->Divide(hData);
  // for (int i=0; i<hData->GetNbinsX(); i++) {
  //     //ratio: data/exp. with hData unc.
  //     hRatio->SetBinContent(i+1,0.);   hRatio->SetBinError(i+1, 0.);
  //     if (hExp->GetBinContent(i+1)!=0.) {
  //       hRatio->SetBinContent(i+1, hData->GetBinContent(i+1)/hExp->GetBinContent(i+1));
  //       hRatio->SetBinError(i+1, float(hData->GetBinError(i+1)/hExp->GetBinContent(i+1))); //take into account only hData unc.
  //     }
  //     else { hRatio->SetBinContent(i+1, -1000.); }
  //     //line in 1 with hPrediction unc.
  //     hLineInOne->SetBinContent(i+1,0.);      hLineInOne->SetBinError(i+1,0.);
  //     hLineInOne->SetBinContent(i+1, 1.);
  //     if (hExp->GetBinContent(i+1)!=0.)
  //       hLineInOne->SetBinError(i+1, float(hExp->GetBinError(i+1)*hData->GetBinContent(i+1)/TMath::Power(hExp->GetBinContent(i+1),2)));  //take into account only hPrediction unc.
  //     else hLineInOne->SetBinContent(i+1, -1000.);
  //   }

  Float_t myFontSize = 0.10;
  hRatio->SetMarkerSize(0.8);  hRatio->SetMarkerStyle(8);
  hRatio->SetMarkerSize(0.8);  hRatio->SetMarkerStyle(1);    hRatio->SetMarkerColor(1);
  hRatio->GetYaxis()->SetTitle("");
  hRatio->GetYaxis()->SetLabelSize(0.095);//(myFontSize);
  hRatio->GetXaxis()->SetLabelSize(0.095);//myFontSize);
  hRatio->GetXaxis()->SetTitleSize(myFontSize);
  //hRatio->GetYaxis()->SetRangeUser(lowerLimit+0.001, upperLimit-0.001);
  hRatio->GetYaxis()->SetNdivisions(804);
  hRatio->GetYaxis()->SetTickLength(0.05);
  hRatio->GetXaxis()->SetTickLength(0.07);
  //hRatio->SetLineColor(1);
  hRatio->SetLineColor(hData->GetLineColor());
  hRatio->SetLineStyle(hData->GetLineStyle());
  hRatio->SetAxisRange(lowerLimit, upperLimit, "Y");
  //hRatio->SetAxisRange(lowerLimit+0.01, upperLimit-0.01, "Y");
  hRatio->Draw("SAME");

  TLine* myLine = new TLine();
  myLine->SetLineStyle(1);
  myLine->DrawLine(hRatio->GetXaxis()->GetXmin(), 1., hRatio->GetXaxis()->GetXmax(), 1.); //MARIA (for ttH and ttbar comp. at 8 and 13 TeV)
  PrintLeftLabel(Form("%s", labelratio.c_str()), myFontSize, 0.04, 0.50);

}


//================================================
// calcSeparation()
//================================================
float calcSeparation(TH1F* sig, TH1F* bkg) {

  float Separation = 0;
  for(int i=1; i<= sig->GetNbinsX(); i++){

    float y_sig = sig->GetBinContent(i);
    float y_bkg = bkg->GetBinContent(i);

    if((y_sig + y_bkg) != 0)
      Separation += (y_sig - y_bkg)*(y_sig - y_bkg)/(y_sig + y_bkg);

  }

  Separation = 0.5*Separation;
  return Separation;

}
