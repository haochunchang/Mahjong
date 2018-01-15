import numpy as np
import pandas as pd

# 要單個檔案或多個檔案可自行修改
# data1 = pd.read_csv('0115test.txt', delimiter=',')
data = pd.read_csv('0116test.txt', delimiter=',')
# datas = [data1, data2]
# data = pd.concat(datas).reset_index()
data['strategy'] = data['strategy'].apply(str)
data = data[data['strategy'] != 'Not all GreedyAI']

data['remain mycol tiles'] = data['remain mycol tiles'].replace(
    to_replace=[-1, -2], value=0)
amount_of_data = len(data) - 1
print("總局數為 " + str(amount_of_data) + " 局")
strategy_list = list()
for strategy in data['strategy'].unique():
    sub_data = data.loc[data['strategy'] == strategy]
    # remain col 為 0 的比例
    number_count = sub_data['remain mycol tiles'].value_counts()
    amount_of_sub_data = len(sub_data) - 1
    print("\n使用策略 " + str(strategy) + " 的局數共 " + str(amount_of_sub_data) + " 局")
    percentage_of_zero = round(number_count[0] / amount_of_data * 100, 2)
    print("流局 " + str(number_count[0]) +
          " 局，佔 " + str(percentage_of_zero) + "%")
    # 計算中位數
    mjcol_median = round(sub_data['remain mycol tiles'].median())
    print("剩餘牌堆數量中位數為 " + str(mjcol_median))
    # 計算平均
    mjcol_mean = round(sub_data['remain mycol tiles'].mean(), 2)
    print("平均剩餘牌堆數量為 " + str(mjcol_mean))
    # 新增到 dict 裡
    strategy_list.append({'strategy': strategy, 'percentage_of_zero': percentage_of_zero,
                          'mjcol_median': mjcol_median, 'mjcol_mean': mjcol_mean})

strategy_list.sort(key=lambda x: x['percentage_of_zero'], reverse=False)
print("\n目前流局數最少的策略是\t " + str(strategy_list[0]['strategy']))
strategy_list.sort(key=lambda x: x['mjcol_median'], reverse=True)
print("剩餘牌堆中位數最多的策略是\t " + str(strategy_list[0]['strategy']))
strategy_list.sort(key=lambda x: x['mjcol_mean'], reverse=True)
print("剩餘牌堆平均最多的策略是\t " + str(strategy_list[0]['strategy']))
