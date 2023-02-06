import os, sys
import pandas as pd
from matplotlib import pyplot as plt
import seaborn as sns
from scipy import stats
import numpy as np
from sklearn.metrics import confusion_matrix 


from sklearn.metrics import roc_curve
from sklearn.metrics import roc_auc_score

import uproot

# Describe with text Feynmann diagram for the R-parity violating SUSY pair production of stop quarks

process_ntuple = {
                 'background' : 'pred_background.root',
                 'signal' : 'pred_signal.root'
}

RUNPATH = '/lhome/ific/a/adruji/DarkMachines/particle_transformer/'
PLOTPATH = '/lhome/ific/a/adruji/DarkMachines/plotting/'

def main():

    RUNCODE = '20230127-130746_example_ParticleTransformer_ranger_lr0.001_batch100'

    from optparse import OptionParser
    parser = OptionParser()
    parser.add_option("-r","--runcode", dest="runcode", default="", help="Code with run information")
    (options, sys.argv[1:]) = parser.parse_args(sys.argv[1:])

    if options.runcode != "": RUNCODE=options.runcode 

    INPUT_PATH = '%s/training/DarkMachines/kin/ParT/%s/predict_output/' % (RUNPATH, RUNCODE)

    processes = sorted(process_ntuple)
    scores = {}
    list_true = {}
    for process in processes:
        with uproot.open(INPUT_PATH+process_ntuple[process]+':Events') as tree:
            #print(tree.show())
            #print(tree.typenames())
            scores[process] = np.array(tree['score_label_signal'])
            list_true[process] = np.array(tree['label_signal'])
    
    y_pred = np.concatenate([scores['background'],scores['signal']])
    y_true_bool = np.concatenate([list_true['background'],list_true['signal']])
    y_true = [int(item) for item in y_true_bool]

    # Create output folder if it does not exist
    if not os.path.exists('%s/plots/%s/' % (PLOTPATH,RUNCODE)):
        print("Creating output folder ...")
        os.makedirs('%s/plots/%s/' % (PLOTPATH,RUNCODE))

    # Calculate AUC score 
    auc_score = roc_auc_score(y_true, y_pred) 
    print("AUC Score:", auc_score)

    # calculate roc curves form y_true and y_pred
    fpr, tpr, thresholds = roc_curve(y_true, y_pred)
    plt.figure()
    plt.plot(fpr, tpr)
    lw = 2 
    plt.plot(fpr, tpr, color='darkorange', lw=lw, label='ROC curve (area = %0.2f)' % auc_score) 
    plt.plot([0, 1], [0, 1], color='navy', lw=lw, linestyle='--') 
    plt.xlabel('False Positive Rate') 
    plt.ylabel('True Positive Rate') 
    plt.title('ROC Curve') 
    plt.legend(loc="lower right")   # Add AUC value as legend to the ROC curve  
    plt.savefig('%s/plots/%s/roc.png' % (PLOTPATH, RUNCODE))  

    # Make a normalised confusion matrix using y_pred and y_true
    predictions = [] 
    for i in range(len(y_pred)): 
        if y_pred[i]> 0.5: 
            predictions.append(1) 
        else: 
            predictions.append(0)  
      
    confusion_matrix_result = confusion_matrix(y_true, predictions)

    norm_conf_mat = confusion_matrix_result/np.sum(confusion_matrix_result, axis=1, keepdims=True) 
    print ("Confusion Matrix :") 
    print(norm_conf_mat)

    # Plotting the confusion matrix using seaborn heatmap 
    import seaborn as sns
    plt.figure() 
    ax= plt.subplot() 
    sns.heatmap(norm_conf_mat , annot=True) 
    
    # Setting labels and titles for the plot  
    ax.set_xlabel('Predicted labels')  
    ax.set_ylabel('Actual labels')  
    ax.set_title('Confusion Matrix')  
    
    # Displaying the plot  
    plt.savefig('%s/plots/%s/confusion_matrix.png' % (PLOTPATH, RUNCODE))
    

if __name__ == '__main__':
    main()
