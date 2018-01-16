import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

hu = {'Player 0': [], 'Player 1': [], 'Player 2': [], 'Player 3': []}
gun = {'Player 0': [], 'Player 1': [], 'Player 2': [], 'Player 3': []}

# Parse result
with open('0116result.txt', 'r') as f:
    for line in f:
        if line.split(' ')[0] == 'Player':
            hu_count = int(line.split(',')[1].split(':')[-1][1:])
            gun_count = int(line.split(',')[2].split(':')[-1][1:])
            hu[line.split(':')[0]].append(hu_count)
            gun[line.split(':')[0]].append(gun_count)
huc = pd.DataFrame()
for k, v in hu.items():
    huc[k] = v
gunc = pd.DataFrame()
for k, v in gun.items():
    gunc[k] = v

data = huc - gunc
data = (data - data.min(axis=0)) / (data.max(axis=0) - data.min(axis=0))
print(data)
for i in range(data.shape[1]):
    if data.columns[i] == "Player 3":
        l = "CustomAI"
    else:
        l = "GreedyAI "+data.columns[i]
    sns.distplot(data.iloc[:,i], label=l)
plt.legend(loc='best')
plt.xlabel('# of hu - # of put-gun(After min-max normalization)')
plt.title('Results of 1000 games, 10 rounds per game')
plt.show()
plt.close()
