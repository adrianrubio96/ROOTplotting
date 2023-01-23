import os,sys
import numpy as np
from sys import platform
from array import *

import ROOT
from ROOT import TFile, TH2D, TH1D, TObject, TTree, gROOT, TMath, TChain, TLorentzVector, TVector3, Math, TLegend

ROOT.gROOT.LoadMacro("CommonMethods_PlotHistos_ADRIAN.cc")

from ROOT import defineATLASstyle, DefineCanvas, DrawHisto

process_ntuple = {
                 'ttbar' : 'ttbar_10fb.root',
                 'Zjets' : 'z_jets_10fb.root',
                 #'ttH'   : 'ttbarHiggs_10fb.root'
                 'wtop' : 'wtop_10fb.root'
}

def main():

    #from optparse import OptionParser
    #parser = OptionParser()
    #parser.add_option("-p","--processes", dest="processes", help="Comma-separated pair-wise list of the processes to compare")
    #(options, sys.argv[1:]) = parser.parse_args(sys.argv[1:])

    INPUT_PATH = '/lhome/ific/a/adruji/GenerativeModels/plotting/predictions/20221114/'
    #pred_ntuples = os.listdir(INPUT_PATH)
    #print(pred_ntuples)
    #processes = [x.split('_')[-1].split('.')[0] for x in pred_ntuples]
    #print(processes)

    processes = sorted(process_ntuple)

    files = {}
    histos = {}
    trees = {}

    defineATLASstyle()
    c = DefineCanvas("c","c")
    for process in processes:
        files[process] = TFile.Open(INPUT_PATH+'pred_%s.root' % process, 'READ')
        trees[process] = files[process].Get('Events')
        histos[process] = TH1D(process,"score",10,0,1)
        histos[process].SetStats(0)
        histos[process].Sumw2()
        score = array('f',[0])
        trees[process].SetBranchAddress('score_label_%s' % process, score)
        for n in range(trees[process].GetEntries()):
            trees[process].GetEntry(n)
            histos[process].Fill(score[0], 1.)
        #trees[process].Draw('score>>histos[%s]' % (process), "(1)","MIN0", 0)
        integral = histos[process].Integral()
        histos[process].Scale(1./integral)

    # Get maximum value
    max_value = 0.
    for process in processes:
        value = histos[process].GetBinContent(histos[process].GetMaximumBin())
        if value > max_value: max_value=value
    
    #Plot histo
    colors = {}
    i=1
    ndiv = 10
    for process in processes:
        colors[process] = int(i*2)
        if i==1: 
            histos[process].GetXaxis().SetTitle("Transformer score")
            histos[process].GetYaxis().SetRangeUser(0,max_value*1.20)
        DrawHisto(histos[process], "MIN0" if i==1 else "same MIN0", colors[process], 0, "", ndiv)
        i+=1

    legend = TLegend(0.70,0.89-2*0.05,0.89,0.89)
    legend.SetTextSize(0.035)
    for process in processes: legend.AddEntry(histos[process], process, "l")
    legend.Draw("same")

    c.Print("./plots/20221114.png")



if __name__ == '__main__':
    main()