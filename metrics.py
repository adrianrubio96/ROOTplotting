import os, sys
import tensorboard as tb
import pandas as pd
from matplotlib import pyplot as plt
import seaborn as sns
from scipy import stats
from packaging import version

RUNCODE = '20230118-191825_example_ParticleTransformer_ranger_lr0.001_batch20/'
RUNPATH = '/lhome/ific/a/adruji/DarkMachines/particle_transformer/'
PLOTPATH = '/lhome/ific/a/adruji/DarkMachines/plotting/'
INPUT_PATH = RUNPATH+'training/DarkMachines/kin/ParT/'+RUNCODE

def main():
    major_ver, minor_ver, _ = version.parse(tb.__version__).release
    assert major_ver >= 2 and minor_ver >= 3, \
        "This notebook requires TensorBoard 2.3 or later."
    print("TensorBoard version: ", tb.__version__)

    # Getting info from tensorboard file
    os.chdir(RUNPATH)
    from tensorboard.backend.event_processing import event_accumulator
    ea = event_accumulator.EventAccumulator('runs/Jan18_19-18-26_mlwn24.ific.uv.esDarkMachines_kin_ParT/events.out.tfevents.1674065906.mlwn24.ific.uv.es.30067.0')
    ea.Reload()
    print(ea.Tags())
    os.chdir(PLOTPATH)
    
    # Plotting Loss
    Loss_train = [item.value for item in ea.Scalars('Loss/train (epoch)')]
    Loss_eval = [item.value for item in ea.Scalars('Loss/eval (epoch)')]
    plt.figure()
    plt.yscale('log')
    plt.plot(Loss_train)
    plt.plot(Loss_eval)
    plt.ylabel('Loss')
    plt.savefig('%s/plots/%s/Loss.png' % (PLOTPATH, RUNCODE))
    
    # Plotting Accuracy
    Acc_train = [item.value for item in ea.Scalars('Acc/train (epoch)')]
    Acc_eval = [item.value for item in ea.Scalars('Acc/eval (epoch)')]
    plt.figure()
    plt.plot(Acc_train)
    plt.plot(Acc_eval)
    plt.ylabel('Acc')
    plt.savefig('%s/plots/%s/Acc.png' % (PLOTPATH, RUNCODE))

    


if __name__ == '__main__':
    main()