
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
void DrawHisto(TH1* histo, TString option, Int_t color, Int_t marker_style, string region)
{ 
  //histo->SetMinimum(0.);
  histo->SetLineColor(color);
  //histo->SetMarkerStyle(marker_style); 
  histo->SetLineStyle(marker_style); 
  histo->SetMarkerColor(color);
  histo->Draw(option);

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
void ShowRatio_wLabel(TH1F* hData, TH1F* hExp, Float_t lowerLimit, Float_t upperLimit, string labelratio) {
 
  TH1F* hRatio(0); 
  TH1F* hLineInOne(0);
  hRatio = (TH1F*)hData->Clone();
  hRatio->Divide(hExp);

  for (int ibin=0; ibin<hRatio->GetNbinsX(); ibin++)
    cout<<"hRatio - "<<hRatio->GetName()<<" --> rel.wrt.nominal: "<<(hRatio->GetBinContent(ibin+1)-1.)*100.<<", with unc.: "<<(hRatio->GetBinError(ibin+1)*100.)<<endl;
  //cout<<"hRatio - "<<hRatio->GetName()<<": "<<hRatio->GetBinContent(ibin+1)<<" +- "<<hRatio->GetBinError(ibin+1)<<endl;
   
  hLineInOne = (TH1F*)hData->Clone();
  hLineInOne->Divide(hData);
  //for (int i=0; i<hData->GetNbinsX(); i++) {
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
  hRatio->SetMarkerSize(0.8);  hRatio->SetMarkerStyle(1);    hRatio->SetMarkerColor(hData->GetMarkerColor()); 
  hRatio->GetYaxis()->SetTitle("");
  hRatio->GetYaxis()->SetLabelSize(0.095);//(myFontSize);
  hRatio->GetXaxis()->SetLabelSize(0.095);//myFontSize);
  hRatio->GetXaxis()->SetTitleSize(myFontSize);
  //hRatio->GetYaxis()->SetRangeUser(lowerLimit+0.001, upperLimit-0.001);
  hRatio->GetYaxis()->SetNdivisions(405);// hRatio->GetYaxis()->SetNdivisions(510,true);  //Best: 405!!
  hRatio->GetYaxis()->SetTickLength(0.05);
  hRatio->GetXaxis()->SetTickLength(0.07);
  //hRatio->SetLineColor(1); 
  hRatio->SetLineColor(hData->GetLineColor());
  //hRatio->SetFillColor(hData->GetLineColor()); //NEW MARIA
  hRatio->SetFillStyle(3018);
  //hRatio->SetLineStyle(hData->GetLineStyle());
  hRatio->SetLineStyle(1);
  hRatio->SetAxisRange(lowerLimit, upperLimit, "Y"); 
  //hRatio->SetAxisRange(lowerLimit+0.01, upperLimit-0.01, "Y");
  hRatio->Draw("HIST E SAME"); //usual one
 
  TLine* myLine = new TLine(); 
  myLine->SetLineStyle(1); 
  myLine->DrawLine(hRatio->GetXaxis()->GetXmin(), 1., hRatio->GetXaxis()->GetXmax(), 1.); //MARIA (for ttH and ttbar comp. at 8 and 13 TeV)
  PrintLeftLabel(Form("%s", labelratio.c_str()), myFontSize, 0.04, 0.50);

}


//================================================
// ShowRatio()
//================================================
void ShowRatioWFillUncBand_wLabel(TH1F* hData, TH1F* hExp, Float_t lowerLimit, Float_t upperLimit, string labelratio) {
 
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
  hRatio->SetMarkerSize(0.8);  hRatio->SetMarkerStyle(1);    hRatio->SetMarkerColor(hData->GetMarkerColor()); 
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
  hRatio->SetFillColor(hData->GetLineColor()); //NEW MARIA
  hRatio->SetFillStyle(3018);
  hRatio->SetFillColorAlpha(hData->GetLineColor(), 0.7);
  hRatio->SetLineStyle(hData->GetLineStyle());
  hRatio->SetLineWidth(2);
  hRatio->SetLineStyle(1);
  hRatio->SetAxisRange(lowerLimit, upperLimit, "Y"); 
  //hRatio->SetAxisRange(lowerLimit+0.01, upperLimit-0.01, "Y");
  //hRatio->Draw("HIST E SAME"); //usual one
  hRatio->Draw("E2 SAME"); 
  

  TLine* myLine = new TLine(); 
  myLine->SetLineStyle(1); 
  myLine->DrawLine(hRatio->GetXaxis()->GetXmin(), 1., hRatio->GetXaxis()->GetXmax(), 1.); //MARIA (for ttH and ttbar comp. at 8 and 13 TeV)
  PrintLeftLabel(Form("%s", labelratio.c_str()), myFontSize, 0.04, 0.50);

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


//=====================================================================
// CalculateAsymmetryFromHisto
//=====================================================================
double CalculateAsymmetryFromHisto(TH1D* histo, int par) {

  // A = (N+ - N-)/(N+ + N-)
  // error_A = sqrt[ (der_A/der_N+)^2*(sigma_N+)^2 + (der_A/der_N-)^2*(sigma_N-)^2 + 2*(der_A/der_N+)*(der_A/der_N-)*sigma_N+N-];
  // CovarianceMatrix = [(sigma_N+)^2 sigma_N+N-; sigma_N+N- (sigma_N-)^2]

  //get binning
  Int_t nbins = histo->GetNbinsX();
  const int NBINS = nbins;

  //retrieve N+, N- and its errors
  Double_t nNp=0.;         Double_t nNm=0.;
  Double_t nNp_error=0.;   Double_t nNm_error=0.;
  Double_t A_FB=0.;        Double_t errorA_FB=0.;  

  //fill "number of events" counters for AFBN
  for (int i=0; i<nbins; ++i) {
    if (i<(nbins/2)) { //N-
      nNm += histo->GetBinContent(i+1); 
      nNm_error += TMath::Power(histo->GetBinError(i+1), 2); //sum_sigma(i)^2
    }
    else  if (i>=(nbins/2)) { //N+
      nNp += histo->GetBinContent(i+1); 
      nNp_error += TMath::Power(histo->GetBinError(i+1), 2); //sum_sigma(i)^2
    }
  }  
  nNm_error = TMath::Sqrt(nNm_error);
  nNp_error = TMath::Sqrt(nNp_error);
  //cout<<"N- = "<<nNm<<" +- "<<nNm_error<<endl;
  //cout<<"N+ = "<<nNp<<" +- "<<nNp_error<<endl;
  
  
  // Compute asymmetry and its error
  //++++++++++++++++++++++++++++++++++
  A_FB =(nNp - nNm)/(nNp + nNm);
  errorA_FB = 2/TMath::Power((nNp+nNm),2)*TMath::Sqrt(TMath::Power(nNp*nNm_error,2)+TMath::Power(nNm*nNp_error,2));
    
  if (par==0) return(A_FB); 
  else if (par==1) return(errorA_FB); 
  else return 0;
  
}


//=====================================================================
// CalculateAsymmetryFromHisto
//=====================================================================
double CalculateAsymmetryFromHisto(TH1* histo, int par) {
  
  Double_t myresult =0.;

  TH1D* tmp = (TH1D*)histo->Clone();
  if (par==0 || par==1) myresult = CalculateAsymmetryFromHisto(tmp, par);
 
  return myresult; 
    
}
