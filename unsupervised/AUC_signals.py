### This script plots the AUC of the signals for the different models
## Input: tex table with the AUC of the signals for the different models

import numpy as np
import matplotlib.pyplot as plt
plt.rc('text', usetex=True)
plt.rc('font', family='serif')
import pandas as pd
import os
import sys
import copy

def main():

    # Parse arguments
    import argparse
    parser = argparse.ArgumentParser(description='Plot AUC of the signals for the different models')
    parser.add_argument("-t", "--tex", dest="tex_table", default=None, required=True, help="Tex table with the AUC of the signals for the different models")
    parser.add_argument("-l", "--legend", dest="legend", default=False, help="Legend for the plot")
    parser.add_argument("-f", "--figsize", dest="figsize", default="(6,5)", help="Figure size")
    parser.add_argument("-s", "--size_font", dest="size_label", default=16, help="Size of the labels")
    args = parser.parse_args()

    tex_table = args.tex_table
    figsize = (int(args.figsize.replace('(','').replace(')','').split(',')[0]), int(args.figsize.replace('(','').replace(')','').split(',')[1]))
    fontsize = int(args.size_label)
    legend = args.legend
    
    ## Read data from tex file
    dict_data = {}
    model_metrics ={'DeepSVDD' : {'MLP': {'AUC' : -1, 'eff_sig' : -1 },
                                'ParT': {'AUC' : -1, 'eff_sig' : -1 },
                                'ParT_{int. SM}': {'AUC' : -1, 'eff_sig' : -1 }},
                    'DDD' : {'ParT': {'AUC' : -1, 'eff_sig' : -1 },
                                'ParT_{int. SM}': {'AUC' : -1, 'eff_sig' : -1 }},
                    'DROCC' : {'MLP': {'AUC' : -1, 'eff_sig' : -1 },
                                'ParT': {'AUC' : -1, 'eff_sig' : -1 },
                                'ParT_{int. SM}': {'AUC' : -1, 'eff_sig' : -1 }}}

    models = []
    with open(tex_table, 'r') as f:
        lines = f.readlines()
        for line in lines:
            ## Read AUCs
            if 'AUC' in line and 'multirow' in line:
                line = line.strip().split('&')
                model = line[0].split('{')[-1].split('}')[0]
                models.append(model)
                dict_data[model] = copy.deepcopy(model_metrics)
                #for i in range(2,len(line)):
                #    print(line[i].strip())
                dict_data[model]['DeepSVDD']['MLP']['AUC'] = float(line[2].strip())
                dict_data[model]['DeepSVDD']['ParT']['AUC'] = float(line[3].strip())
                dict_data[model]['DeepSVDD']['ParT_{int. SM}']['AUC'] = float(line[4].strip())
                dict_data[model]['DDD']['ParT']['AUC'] = float(line[5].strip())
                dict_data[model]['DDD']['ParT_{int. SM}']['AUC'] = float(line[6].strip())
                dict_data[model]['DROCC']['MLP']['AUC'] = float(line[7].strip())
                dict_data[model]['DROCC']['ParT']['AUC'] = float(line[8].strip())
                dict_data[model]['DROCC']['ParT_{int. SM}']['AUC'] = float(line[9].strip().split('\\')[0].strip())

            ## Read eff_sig
            if '\epsilon_S' in line:
                line = line.strip().split('&')
                model = models[-1]
                dict_data[model]['DeepSVDD']['MLP']['eff_sig'] = float(line[2].strip())
                dict_data[model]['DeepSVDD']['ParT']['eff_sig'] = float(line[3].strip())
                dict_data[model]['DeepSVDD']['ParT_{int. SM}']['eff_sig'] = float(line[4].strip())
                dict_data[model]['DDD']['ParT']['eff_sig'] = float(line[5].strip())
                dict_data[model]['DDD']['ParT_{int. SM}']['eff_sig'] = float(line[6].strip())
                dict_data[model]['DROCC']['MLP']['eff_sig'] = float(line[7].strip())
                dict_data[model]['DROCC']['ParT']['eff_sig'] = float(line[8].strip())
                dict_data[model]['DROCC']['ParT_{int. SM}']['eff_sig'] = float(line[9].strip().split('\\')[0].strip())
    
    ## Plot data
    # models listed in y axis
    # AUC in x axis

    markers = ['o', 's', '^']
    colors = ['b', 'g', 'r']

    ## AUC chart
    #fig, ax = plt.subplots()
    #fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(4, 2)) # 1 row, 2 columns ## 4tops
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=figsize) # 1 row, 2 columns ## other
    x = np.arange(len(models))  # the label locations
    width = 0.55 

    for i_marker, architecture in enumerate(dict_data[models[0]]['DeepSVDD'].keys()):
        marker = markers[i_marker]

        AUCs = [dict_data[signal]['DeepSVDD'][architecture]['AUC'] for signal in models]
        ax1.scatter(AUCs, x+width/2., marker=marker, color='b', label='DeepSVDD' + ' ' + architecture, edgecolor='black', s=32)

        AUCs = [dict_data[signal]['DROCC'][architecture]['AUC'] for signal in models]
        ax1.scatter( AUCs, x,  marker=marker, color='g', label='DROCC' + ' ' + architecture, edgecolor='black', s=32)

        if architecture!='MLP':
            AUCs = [dict_data[signal]['DDD'][architecture]['AUC'] for signal in models]
            ax1.scatter( AUCs, x-width/2.,  marker=marker, color='r', label='DDD' + ' ' + architecture, edgecolor='black', s=32)

        #ax1.axhline(y=i, color='black', linewidth=0.5)
    
    # Set x range
    ax1.set_xlim([0.0, 1])

    # Add horizontal lines to separate models
    for i in range(len(models)):
        if i==0: continue
        ax1.axhline(y=(i-width), color='black', linewidth=0.5)

    # Convert models to y ticks
    ax1.set_yticks(x)
    ax1.set_yticklabels(models, fontsize=fontsize)

    # Set size of x-ticks markers
    ax1.tick_params(axis='x', which='major', labelsize=fontsize)

    # Set labels
    ax1.set_xlabel('AUC', fontsize=fontsize)
    for i_marker, architecture in enumerate(dict_data[models[0]]['DeepSVDD'].keys()):

        marker=markers[i_marker]

        eff_sig = [dict_data[signal]['DeepSVDD'][architecture]['eff_sig'] for signal in models]
        ax2.scatter( eff_sig, x+width/2., marker=marker, color='b', label='DeepSVDD' + ' ' + architecture, edgecolor='black', s=32)

        eff_sig = [dict_data[signal]['DROCC'][architecture]['eff_sig'] for signal in models]
        ax2.scatter( eff_sig, x,  marker=marker, color='g', label='DROCC' + ' ' + architecture, edgecolor='black', s=32)

        if architecture!='MLP':
            eff_sig = [dict_data[signal]['DDD'][architecture]['eff_sig'] for signal in models]
            ax2.scatter( eff_sig, x-width/2.,  marker=marker, color='r', label='DDD' + ' ' + architecture, edgecolor='black', s=32)


    # Set x range
    ax2.set_xlim([0., 1])
    # Set x log scale
    #ax2.set_xscale('log')

    # Set labels and size
    ax2.set_xlabel('$\epsilon_S(\epsilon_B=0.01)$', fontsize=fontsize)

    # Set size of x-ticks markers
    ax2.tick_params(axis='x', which='major', labelsize=fontsize)

    # Remove y-ticks
    ax2.set_yticks([])

    # Add horizontal lines to separate models
    for i in range(len(models)):
        if i==0: continue
        ax2.axhline(y=(i-width), color='black', linewidth=0.5)

    # Add legend
    if legend:
        ## reagange legend order
        handles, labels = ax2.get_legend_handles_labels()
        order = [ 2, 5, 0, 3, 6, 1, 4, 7]
        ax2.legend([handles[idx] for idx in order], [labels[idx] for idx in order], loc='lower center', bbox_to_anchor=(-0.3, -0.5), ncol=3, fontsize=10)


    # Set margins of plot
    if legend and ("4tops" not in tex_table):
        plt.subplots_adjust(left=0.2, right=0.9, top=0.9, bottom=0.35)
    elif legend and ("4tops" in tex_table):
        plt.subplots_adjust(left=0.2, right=0.9, top=0.8, bottom=0.45)
    elif not legend and ("4tops" in tex_table):
        plt.subplots_adjust(left=0.2, right=0.9, top=0.8, bottom=0.25)
    else:
        plt.subplots_adjust(left=0.2, right=0.9, top=0.9, bottom=0.15)

    # Add title to the figure
    title = tex_table.split('/')[-1].split('.')[0].replace('4tops','4-top channel')
    fig.suptitle('%s' % title, fontsize=fontsize+2)

    # Save plot
    #plt.show()
    #plt.savefig('AUC_signals_test.png')
    plt.savefig('AUC_signals_%s%s.png' % (tex_table.split('/')[-1].split('.')[0], "_legend" if legend else "" ))
    plt.savefig('AUC_signals_%s%s.pdf' % (tex_table.split('/')[-1].split('.')[0], "_legend" if legend else "" ))


if __name__ == '__main__':
    main()