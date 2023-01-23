import os
import numpy as np
from sys import platform
from array import *

import ROOT
from ROOT import TFile, TH2D, TH1D, TObject, TTree, gROOT, TMath, TChain, TLorentzVector, TVector3, Math, TLegend

ROOT.gROOT.LoadMacro("CommonMethods_PlotHistos_ADRIAN.cc")

from ROOT import defineATLASstyle, DefineCanvas, DrawHisto
#ROOT.defineATLASstyle()
#ROOT.DefineCanvas()
#ROOT.DrawHisto()

#import config_ttw as config

def showEvery(evnt, n, total):
    if evnt%n == 0:
        print('Processed %.f/%.f'%(evnt, total))

def Fill_PhiC(tree, h, entries):
    arr_Phi_C = []
    for evnt in range(entries):

        # Retrieve event
        tree.GetEntry(evnt)

        # Selection
        #if not multilepton_channel(tree.MC_Wdecay2_from_t_pdgId, tree.MC_Wdecay1_from_tbar_pdgId): continue
        #if not lep_pt_cuts(tree.MC_Wdecay2_from_t_pt, tree.MC_Wdecay1_from_tbar_pt): continue

        # Show Progress
        showEvery(evnt, 10000, tree.GetEntries())

        # Retrieve particle info
        top_pt = tree.top_pt
        top_eta = tree.top_eta
        top_phi = tree.top_phi
        top_E = tree.top_E

        antitop_pt = tree.antitop_pt
        antitop_eta = tree.antitop_eta
        antitop_phi = tree.antitop_phi
        antitop_E = tree.antitop_E

        higgs_pt = tree.higgs_pt
        higgs_eta = tree.higgs_eta
        higgs_phi = tree.higgs_phi
        higgs_E = tree.higgs_E

        lep_top_pt = tree.lplus_pt
        lep_top_eta = tree.lplus_eta
        lep_top_phi = tree.lplus_phi
        lep_top_E = tree.lplus_E

        lep_antitop_pt = tree.lminus_pt
        lep_antitop_eta = tree.lminus_eta
        lep_antitop_phi = tree.lminus_phi
        lep_antitop_E = tree.lminus_E

        # Define TLorentzVectors
        p_tlv = TLorentzVector()
        antip_tlv = TLorentzVector()
        top_tlv = TLorentzVector()
        antitop_tlv = TLorentzVector()
        higgs_tlv = TLorentzVector()
        lep_top_tlv = TLorentzVector()
        lep_antitop_tlv = TLorentzVector()

        p_tlv.SetPxPyPzE(0,0,13000./2.,13000./2.)
        antip_tlv.SetPxPyPzE(0,0,-13000./2.,13000./2.)
        top_tlv.SetPtEtaPhiE(top_pt, top_eta, top_phi, top_E)
        antitop_tlv.SetPtEtaPhiE(antitop_pt, antitop_eta, antitop_phi, antitop_E)
        higgs_tlv.SetPtEtaPhiE(higgs_pt, higgs_eta, higgs_phi, higgs_E)
        lep_top_tlv.SetPtEtaPhiE(lep_top_pt, lep_top_eta, lep_top_phi, lep_top_E)
        lep_antitop_tlv.SetPtEtaPhiE(lep_antitop_pt, lep_antitop_eta, lep_antitop_phi, lep_antitop_E)

        #Moving to Higgs rest frame
        #print(str(p_tlv.Px())+' '+str(p_tlv.Py())+' '+str(p_tlv.Pz()))
        #print(str(higgs_tlv.Px())+' '+str(higgs_tlv.Py())+' '+str(higgs_tlv.Pz()))
        p_tlv.Boost(-higgs_tlv.BoostVector())
        antip_tlv.Boost(-higgs_tlv.BoostVector())
        top_tlv.Boost(-higgs_tlv.BoostVector())
        antitop_tlv.Boost(-higgs_tlv.BoostVector())
        higgs_tlv.Boost(-higgs_tlv.BoostVector())
        lep_top_tlv.Boost(-higgs_tlv.BoostVector())
        lep_antitop_tlv.Boost(-higgs_tlv.BoostVector())
        #print(str(higgs_tlv.Px())+' , '+str(higgs_tlv.Py())+' , '+str(higgs_tlv.Pz()))

        # Defining 3-momentum vectors
        p_v = p_tlv.Vect()
        antip_v = antip_tlv.Vect()
        top_v = top_tlv.Vect()
        antitop_v = antitop_tlv.Vect()
        higgs_v = higgs_tlv.Vect()
        lep_top_v = lep_top_tlv.Vect()
        lep_antitop_v = lep_antitop_tlv.Vect()

        # Normalise
        p_n = p_v*(1./p_v.Mag())
        antip_n = antip_v*(1./antip_v.Mag())
        top_n = top_v*(1./top_v.Mag())
        antitop_n = antitop_v*(1./antitop_v.Mag())
        lep_top_n = lep_top_v*(1./lep_top_v.Mag())
        lep_antitop_n = lep_antitop_v*(1./lep_antitop_v.Mag())

        # Define Phi_c observable
        num_Phi_C = (p_n.Cross(antip_n)).Dot(top_n.Cross(antitop_n))
        num_Phi_C = TMath.Sqrt(num_Phi_C*num_Phi_C)
        den_Phi_C = (p_n.Cross(antip_n)).Mag()*(top_n.Cross(antitop_n)).Mag()
        if (den_Phi_C!=0):
            Phi_C = num_Phi_C/den_Phi_C
            Phi_C = TMath.ACos(Phi_C)
            #print(Phi_C)
        else:
            print("Zero denominator when computing Phi_C")

        # Fill histo
        h.Fill(Phi_C)
        arr_Phi_C.append(Phi_C)

    asym, err = asymmetry(arr_Phi_C, np.pi/4.)

    return h, asym, err

def asymmetry(array, centre):
    p = 0
    n = 0
    for x in array:
        if (x>=centre): p+=1
        if (x<centre): n+=1

    asymmetry = float(p-n)/float(p+n)
    error = 2./TMath.Power(p+n, 2)*TMath.Sqrt(n*n*p + p*p*n)

    return asymmetry, error

def main():

    #mainPATH = '/eos/atlas/user/m/mamolla/higgs/ttH/STXS/ttH_FullPhaseSpace/ttH_inc_mumu/'
    #rootfile = 'ttH_P8_inc_mumu_myTopOutput.root'

    mainPATH = '/eos/atlas/user/m/mamolla/higgs/ttH/CP/TRUTH1/'
    BSM_ttH = 'ttH_semilep_CPstudy_BSM_alpha90_myTopOutput.root'
    SM_ttH = 'ttH_semilep_CPstudy_SM_alpha0_myTopOutput.root'
    BSM_ttX = 'ttX0_gamgam_alpha90_myTopOutput.root'
    SM_ttX = 'ttX0_gamgam_alpha0_myTopOutput.root'

    file_SM = TFile.Open(mainPATH+SM_ttH, 'READ')
    file_BSM = TFile.Open(mainPATH+BSM_ttH, 'READ')

    tree_SM = file_SM.Get('tree')
    tree_BSM = file_BSM.Get('tree')

    defineATLASstyle()
    c = DefineCanvas("c","c")
    h_SM = TH1D("h_SM", "", 20, 0, 1.5)
    h_SM.SetStats(0)
    h_SM.Sumw2()
    h_BSM = TH1D("h_BSM", "", 20, 0, 1.5)
    h_BSM.SetStats(0)
    h_BSM.Sumw2()

    # Fill variable histogram and compute asymmetry around pi/4
    h_SM, asym_SM, err_asym_SM = Fill_PhiC(tree_SM, h_SM, 100000)#tree_SM.GetEntries())
    h_BSM, asym_BSM, err_asym_BSM = Fill_PhiC(tree_BSM, h_BSM, 100000)#tree_BSM.GetEntries())

    print("CP-even asymmetry = {:.5f} +/- {:.5f}".format(asym_SM, err_asym_SM))
    print("CP-odd asymmetry = {:.5f} +/- {:.5f}".format(asym_BSM, err_asym_BSM))

    h_SM.Scale(1./h_SM.Integral())
    h_BSM.Scale(1./h_BSM.Integral())

    #Plot histo
    color_SM=2
    color_BSM=4
    ndiv = 20
    DrawHisto(h_BSM, "MIN0", color_BSM, 0, "", ndiv);
    DrawHisto(h_SM, "MIN0 same", color_SM, 0, "", ndiv);

    h_BSM.GetXaxis().SetTitle("$\phi_C$")

    legend = TLegend(0.70,0.89-2*0.05,0.89,0.89)
    legend.SetTextSize(0.035);
    legend.AddEntry(h_SM,"CP-even","l")
    legend.AddEntry(h_BSM,"CP-odd","l")
    legend.Draw("same")

    c.Print("./plots/Phi_C_observable_ttH_12-01-2022.png")

if __name__ == '__main__':
    main()
