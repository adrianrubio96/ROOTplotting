import os,sys
import numpy as np
from sys import platform
from array import *
from scipy.stats import entropy
import numpy as np

import ROOT
from ROOT import TFile, TH2D, TH1D, TObject, TTree, gROOT, TMath, TChain, TLorentzVector, TVector3, Math, TLegend

ROOT.gROOT.LoadMacro("./ATLAS/CommonMethods_PlotHistos.cc")

from ROOT import defineATLASstyle, DefineCanvas, DrawHisto

INPUT_PATH = '/lustre/ific.uv.es/grid/atlas/t3/adruji/DarkMachines/DarkMachines_ntuples/channel1/signal_vs_bkg/'
PLOTPATH = '/lhome/ific/a/adruji/DarkMachines/plotting/'

dataset = 'train'
max_objects = 20

# Function that defines the variables to be plotted
def define_vars_to_plot(variables, max_objects):
    vars_ = {}
    j_obj = {}
    sorted_pT, index_map = sort_by_pT(variables)
    vars_['HT'] = variables['HT'][0]
    vars_['MET'] = variables['MET'][0]
    j_obj['bjet'] = 0
    j_obj['jet'] = 0
    j_obj['lepton'] = 0
    j_obj['photon'] = 0
    list_Energy = [value for value in variables['obj_Energy']]
    #print(list_Energy)
    for i in range(len(variables['obj_Energy'])):
        if variables['isBJet'][index_map[i]] == 1: obj = 'bjet'
        elif variables['isJet'][index_map[i]] == 1: obj = 'jet'
        elif variables['isLepton'][index_map[i]] == 1: obj = 'lepton'
        elif variables['isPhoton'][index_map[i]] == 1: obj = 'photon'
        else: 
            continue

        vars_['%s%d_Energy' % (obj,j_obj[obj])] = variables['obj_Energy'][index_map[i]]
        vars_['%s%d_eta' % (obj,j_obj[obj])] = variables['obj_eta'][index_map[i]]
        vars_['%s%d_phi' % (obj,j_obj[obj])] = variables['obj_phi'][index_map[i]]
        vars_['%s%d_pT' % (obj,j_obj[obj])] = pT(variables)[index_map[i]]
        j_obj[obj] += 1

    return vars_

# Function that calculates pT
def pT(variables):
    pT_ = np.hypot(variables['obj_px'], variables['obj_py'])
    return pT_

# Function that sorts a list of objects by pT and saves the mapping of indices
def sort_by_pT(variables):
    pT_ = pT(variables)
    map_ = {obj: i for i, obj in enumerate(pT_)}
    sorted_pT = sorted(pT_, reverse=True)
    map_sorted = {obj: i for i, obj in enumerate(sorted_pT)}
    index_map = {map_sorted[obj]: map_[obj] for obj in map_}
    return sorted_pT, index_map

def main():

    # Get list of files
    files = os.listdir(INPUT_PATH+'/'+dataset+'/')

    # Define histograms dictionaries
    histos = {}

    # Define variables dictionary
    variables = {}

    # List of objetcs to be plotted
    objs = ['bjet', 'jet', 'lepton', 'photon']

    # Variables to be plotted
    vars_to_plot = ['HT', 'MET']
    for i in range(max_objects):
        for obj in objs:
            vars_to_plot += ['%s%d_Energy' % (obj,i), '%s%d_eta' % (obj,i), '%s%d_phi' % (obj,i), '%s%d_pT' % (obj,i)]

    # Define ranges
    ranges = {}
    ranges['HT'] = [0, 2000]
    ranges['MET'] = [0, 1000]
    for i in range(max_objects):
        for obj in objs:
            ranges['%s%d_Energy' % (obj,i)] = [0, 2000]
            ranges['%s%d_eta' % (obj,i)] = [-5, 5]
            ranges['%s%d_phi' % (obj,i)] = [-3.2, 3.2]
            ranges['%s%d_pT' % (obj,i)] = [0, 2000]

    # Define canvas
    defineATLASstyle()
    canvas = {}

    # Define histograms
    for process in ['background', 'signal']:
        histos[process] = {}
        for v in sorted(vars_to_plot):
            #print(vars_to_plot)
            histos[process][v] = TH1D(process+'_'+v, process+'_'+v, 10, ranges[v][0], ranges[v][1])
            histos[process][v].SetStats(0)
            histos[process][v].Sumw2()

    # For each file, read the tree and save the variables in histograms per process
    print(sorted(files))
    for file in sorted(files):

        print("Reading file: ", file, "...")

        process = 'background' if 'background' in file else 'signal'
        print('Processing %s' % process)

        f = TFile.Open(INPUT_PATH+'/'+dataset+'/'+file, "READ")
        tree = f.Get("tree")

        # Get list of branches
        list_branches = [b.GetName() for b in tree.GetListOfBranches()]

        for v in sorted(list_branches):
            # Get type of variable in branch
            vartype = tree.GetBranch(v).GetListOfLeaves().At(0).GetTypeName()
            if 'VecOps' in vartype:
                if 'int' in vartype:
                    variables[v] = ROOT.std.vector(int,ROOT.Detail.VecOps.RAdoptAllocator(int))()
                    tree.SetBranchAddress(v,variables[v])
                elif 'float' in vartype or 'Float' in vartype:
                    variables[v] = ROOT.std.vector(float,ROOT.Detail.VecOps.RAdoptAllocator(float))()
                    tree.SetBranchAddress(v,variables[v])
            else:
                if 'int' in vartype:
                    variables[v] = array('i',[0])
                    tree.SetBranchAddress(v,variables[v])
                elif 'float' in vartype or 'Float' in vartype:
                    variables[v] = array('f',[0.])
                    tree.SetBranchAddress(v,variables[v])

        # Loop over events
        for i in range(tree.GetEntries()):
            tree.GetEntry(i)
            vars_ = define_vars_to_plot(variables, max_objects)
            # Fill histograms
            for v in sorted(vars_):
                histos[process][v].Fill(vars_[v], 1.)

            if i%10000 == 0: print('Event %d' % i)

        f.Close()

    # Scale histograms
    for process in ['background', 'signal']:
        for v in vars_to_plot:
            integral = histos[process][v].Integral()
            if integral > 0:
                histos[process][v].Scale(1./integral)

    # Create output folder if it does not exist
    if not os.path.exists('%s/plots/input_variables/' % (PLOTPATH)):
        print("Creating output folder ...")
        os.makedirs('%s/plots/input_variables/' % (PLOTPATH))
    
    if not os.path.exists('%s/plots/input_variables/%s' % (PLOTPATH, dataset)):
        print("Creating output folder ...")
        os.makedirs('%s/plots/input_variables/%s' % (PLOTPATH, dataset))

    # Remove existing plots for this dataset
    print("Removing existing plots for dataset %s ..." % dataset)
    os.system("rm %s/plots/input_variables/%s/*_%s.png" % (PLOTPATH, dataset, dataset))

    # Plot histograms
    # Get maximum value
    max_value = 0.
    colors = {}
    ndiv = 10
    for v in sorted(vars_to_plot):

        # Skip empty histograms
        if histos['background'][v].Integral() == 0 and histos['signal'][v].Integral() == 0 : continue

        # Define canvas
        canvas[v] = DefineCanvas(v,v)
        i=1
        for process in ['background', 'signal']:
            
            value = histos[process][v].GetBinContent(histos[process][v].GetMaximumBin())
            if value > max_value: max_value=value
            colors[process] = int(i*2)
            if i==1: 
                histos[process][v].GetXaxis().SetTitle(v)
                histos[process][v].GetYaxis().SetRangeUser(0,max_value*1.20)
                histos[process][v].GetYaxis().SetTitle("Events")
            DrawHisto(histos[process][v], "MIN0" if i==1 else "same MIN0", colors[process], 0, "", ndiv)
            i+=1
    
        # Add legend
        legend = TLegend(0.60,0.89-2*0.05,0.89,0.89)
        legend.SetTextSize(0.035)
        for process in ['background', 'signal']: legend.AddEntry(histos[process][v], process, "l")
        legend.AddEntry(histos['signal'][v], "%s sample" % dataset, "")
        legend.Draw("same")
        
        canvas[v].Print("%s/plots/input_variables/%s/%s_%s.png" % (PLOTPATH, dataset, v, dataset))


if __name__ == "__main__":
    main()