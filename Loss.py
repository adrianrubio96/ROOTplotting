import os, sys
import tensorboard as tb
import pandas as pd
from matplotlib import pyplot as plt
import seaborn as sns
from scipy import stats
from packaging import version

RUNPATH = '/lhome/ific/a/adruji/DarkMachines/particle_transformer/'
PLOTPATH = '/lhome/ific/a/adruji/DarkMachines/plotting/'

#def smooth(train, val, step):
#    train_ = [train[i] for i in range(len(train)) if i%step==0]
#    val_ = [val[i] for i in range(len(val)) if i%step==0]
#    return train_, val_

def smooth(train, val, smooth_epochs):
    smoothed_train = []
    smoothed_val = []
    for i in range(len(train) - 1):
        if i >= len(train) - smooth_epochs: continue
        smoothed_train.append((sum(train[i+n] for n in range(smooth_epochs)))/float(smooth_epochs))
        smoothed_val.append((sum(val[i+n] for n in range(smooth_epochs)))/float(smooth_epochs))
    return smoothed_train, smoothed_val

# Function that that takes de Loss curve plot and smooth it

def main():

    RUNCODE = '20230127-130746_example_ParticleTransformer_ranger_lr0.001_batch100'
    TBCODE = 'Jan27_13-07-58_mlwn24.ific.uv.esDarkMachines_kin_ParT'
    TBFILE = 'events.out.tfevents.1674821278.mlwn24.ific.uv.es.9814.0'

    from optparse import OptionParser
    parser = OptionParser()
    parser.add_option("-r","--runcode", dest="runcode", default="", help="Code with run information")
    parser.add_option("-t","--tbcode", dest="tbcode", default="", help="Code from tensorboard")
    parser.add_option("-f","--tbfile", dest="tbfile", default="", help="Output file from tensorboard")
    parser.add_option("-s","--smooth", dest="smooth", type=int, default=5, help="Smoothing for Loss and Acc plots")
    (options, sys.argv[1:]) = parser.parse_args(sys.argv[1:])

    if options.runcode != "": RUNCODE=options.runcode 
    if options.tbcode != "": TBCODE=options.tbcode 
    if options.tbfile != "": TBFILE=options.tbfile 

    INPUT_PATH = RUNPATH+'training/DarkMachines/kin/ParT/'+RUNCODE

    major_ver, minor_ver, _ = version.parse(tb.__version__).release
    assert major_ver >= 2 and minor_ver >= 3, \
        "This notebook requires TensorBoard 2.3 or later."
    print("TensorBoard version: ", tb.__version__)

    # Getting info from tensorboard file
    os.chdir(RUNPATH)
    from tensorboard.backend.event_processing import event_accumulator
    ea = event_accumulator.EventAccumulator('runs/%s/%s' % (TBCODE, TBFILE))
    ea.Reload()
    #print(ea.Tags())
    os.chdir(PLOTPATH)

    # Create output folder if it does not exist
    if not os.path.exists('%s/plots/%s/' % (PLOTPATH,RUNCODE)):
        print("Creating output folder ...")
        os.makedirs('%s/plots/%s/' % (PLOTPATH,RUNCODE))
    
    # Plotting Loss
    Loss_train = [item.value for item in ea.Scalars('Loss/train (epoch)')]
    Loss_eval = [item.value for item in ea.Scalars('Loss/eval (epoch)')]
    Loss_train, Loss_eval = smooth(Loss_train, Loss_eval, options.smooth)
    plt.figure()
    plt.plot(Loss_train, label='Train')
    plt.plot(Loss_eval, label='Val')
    plt.ylabel('Loss')
    plt.xlabel('Epochs')
    plt.legend() 
    plt.ylim(0, 1)
    plt.savefig('%s/plots/%s/Loss.png' % (PLOTPATH, RUNCODE))
    
    # Plotting Accuracy
    Acc_train = [item.value for item in ea.Scalars('Acc/train (epoch)')]
    Acc_eval = [item.value for item in ea.Scalars('Acc/eval (epoch)')]
    Acc_train, Acc_eval = smooth(Acc_train, Acc_eval, options.smooth)
    plt.figure()
    plt.plot(Acc_train, label='Train')
    plt.plot(Acc_eval, label='Val')
    plt.ylabel('Acc')
    plt.xlabel('Epochs')
    plt.legend() 
    plt.ylim(0, 1)
    # Set y axis limits from 0 to max value
    plt.savefig('%s/plots/%s/Acc.png' % (PLOTPATH, RUNCODE))


    


if __name__ == '__main__':
    main()