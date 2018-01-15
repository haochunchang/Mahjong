import numpy as np
import pandas as pd

data1 = pd.read_csv('0115test.txt', delimiter=',')
data2 = pd.read_csv('0116test.txt', delimiter=',')

datas = [data1, data2]
data = pd.concat(datas)
cols = ['seed', 'strategy', 'remain mycol tiles']


data['remain mycol tiles'] = data['remain mycol tiles'].replace(
    to_replace=[-1, -2], value=0)
amount_of_data = len(data) - 1
print("總局數為 " + str(amount_of_data) + " 局")
strategy_list = list()
for strategy in data['strategy'].unique():
    sub_data = data.loc[data['strategy'] == strategy]
    number_count = sub_data['remain mycol tiles'].value_counts()
    amount_of_sub_data = len(sub_data) - 1
    print("\n使用策略 " + str(strategy) + " 的局數共 " + str(amount_of_sub_data) + " 局")
    percentage_of_zero = round(number_count[0] / amount_of_data * 100, 2)
    print("流局 " + str(number_count[0]) +
          " 局，佔 " + str(percentage_of_zero) + "%")
    remain_mycol_tiles = round(sub_data['remain mycol tiles'].mean(), 2)
    print("平均剩餘牌堆數量為 " + str(remain_mycol_tiles))
    strategy_list.append({'strategy': strategy, 'percentage_of_zero': percentage_of_zero,
                          'remain_mycol_tiles': remain_mycol_tiles})

strategy_list.sort(key=lambda x: x['percentage_of_zero'], reverse=False)
print("\n目前流局數最少的策略是 " + str(strategy_list[0]['strategy']))
strategy_list.sort(key=lambda x: x['remain_mycol_tiles'], reverse=True)
print("剩餘牌堆數最多的策略是 " + str(strategy_list[0]['strategy']))
