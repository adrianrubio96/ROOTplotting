import os,sys
import numpy as np
from sys import platform
from array import *

import ROOT
from ROOT import TFile, TH2D, TH1D, TObject, TTree, gROOT, TMath, TChain, TLorentzVector, TVector3, Math, TLegend

ROOT.gROOT.LoadMacro("./ATLAS/CommonMethods_PlotHistos.cc")

from ROOT import defineATLASstyle, DefineCanvas, DrawHisto

process_ntuple = {
                 'background' : 'pred_background.root',
                 'signal' : 'pred_signal.root'
}

def main():

    #from optparse import OptionParser
    #parser = OptionParser()
    #parser.add_option("-p","--processes", dest="processes", help="Comma-separated pair-wise list of the processes to compare")
    #(options, sys.argv[1:]) = parser.parse_args(sys.argv[1:])

    INPUT_PATH = '/lhome/ific/a/adruji/DarkMachines/particle_transformer/training/DarkMachines/kin/ParT/20230118-191825_example_ParticleTransformer_ranger_lr0.001_batch20/predict_output/'
    output_folder = INPUT_PATH.split('/')[-3]

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
            histos[process].GetXaxis().SetTitle("Score")
            histos[process].GetYaxis().SetRangeUser(0,max_value*1.20)
        DrawHisto(histos[process], "MIN0" if i==1 else "same MIN0", colors[process], 0, "", ndiv)
        i+=1

    legend = TLegend(0.70,0.89-2*0.05,0.89,0.89)
    legend.SetTextSize(0.035)
    for process in processes: legend.AddEntry(histos[process], process, "l")
    legend.Draw("same")

    # Create output folder if it does not exist
    if not os.path.exists('./plots/%s/' % output_folder):
        print("Creating output folder ...")
        os.makedirs('./plots/%s/' % output_folder)

    c.Print("./plots/%s/score.png" % output_folder)



if __name__ == '__main__':
    main()