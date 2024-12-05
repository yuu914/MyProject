import numpy as np
import matplotlib.pyplot as plt
from sklearn.metrics import roc_curve, auc

# 模擬參數
num_nodes = 100  # 節點數量
initial_energy = 100  # 每個節點的初始能量（單位：焦耳）
leach_energy_consumption = np.random.uniform(0.5, 2.0, num_nodes)  # 每輪的能量消耗

# 模擬YOLO檢測數據
true_positives = np.random.randint(50, 100, 1)[0]
false_positives = np.random.randint(5, 20, 1)[0]
false_negatives = np.random.randint(5, 20, 1)[0]
true_negatives = 100 - (true_positives + false_positives + false_negatives)

# 精確率、召回率和F1分數計算
precision = true_positives / (true_positives + false_positives)
recall = true_positives / (true_positives + false_negatives)
f1_score = 2 * (precision * recall) / (precision + recall)

# 模擬ROC曲線數據
y_true = np.array([1] * true_positives + [0] * false_negatives + [1] * false_positives + [0] * true_negatives)
y_scores = np.random.uniform(0.2, 0.9, len(y_true))
fpr, tpr, _ = roc_curve(y_true, y_scores)
roc_auc = auc(fpr, tpr)

# 能量消耗圖表
plt.figure(figsize=(10, 6))
plt.bar(range(num_nodes), initial_energy - leach_energy_consumption, color='skyblue')
plt.title('Energy Consumption under LEACH Protocol', fontsize=14)
plt.xlabel('Node ID', fontsize=12)
plt.ylabel('Remaining Energy (J)', fontsize=12)
plt.show()

# 檢測性能
plt.figure(figsize=(8, 5))
metrics = ['Precision', 'Recall', 'F1 Score']
values = [precision, recall, f1_score]
plt.bar(metrics, values, color='lightgreen')
plt.ylim(0, 1)
plt.title('Detection Performance Analysis', fontsize=14)
plt.ylabel('Score', fontsize=12)
plt.show()

# ROC曲線
plt.figure(figsize=(8, 6))
plt.plot(fpr, tpr, label=f'ROC Curve (AUC = {roc_auc:.2f})')
plt.plot([0, 1], [0, 1], 'r--')
plt.xlabel('False Positive Rate', fontsize=12)
plt.ylabel('True Positive Rate', fontsize=12)
plt.title('ROC Curve', fontsize=14)
plt.legend(loc="lower right")
plt.show()

'''
差異分析

模擬數據與理想數據之間的差異分析：
1. 數據隨機化的影響：模擬中的隨機生成檢測數據可能與模型的實際行為有所不同。
2. 簡化的能量消耗模型：LEACH協議的能量消耗假設未考慮網絡拓撲變化或節點的異常能量消耗。
3. 檢測性能偏差：模擬可能未涵蓋YOLO和ResNet的真實算法邏輯，影響性能指標。
4. 環境變量影響：傳感器佈局、背景干擾、數據噪聲等未完全反映在模擬中。
'''