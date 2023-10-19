import math
import numpy as np
import matplotlib.pyplot as plt
from sklearn.preprocessing import StandardScaler

phaseDiff = 1
freq = 3
samples = 300

minDiff = 1.00
phase = 0.01

# 時間配列を用意。1秒分。
t = np.arange(samples) / samples
rawData = np.zeros((len(t), 2))
rawData[:,0] = np.sin(2 * math.pi * freq * t)
rawData[:,1] = np.sin(2 * math.pi * freq * t + phaseDiff)

# ノイズデータ生成
noiseRange = 0.05
noise = np.random.normal(size=(samples,1)) * noiseRange
rawData[:,1] = rawData[:,1] + noise[:,0]

# 標準化
scaler = StandardScaler()
scaler.fit(rawData)
data = scaler.transform(rawData)

data = rawData

c = np.cov(np.transpose(data))
print('共分散: ', c[0,1])

plt.figure()
plt.title('Data')
plt.plot(data[1:samples,:])

plt.figure(figsize=(4,4))
plt.title('Phase Diagram')
plt.scatter(data[1:samples,0],data[1:samples,1])
plt.show()
