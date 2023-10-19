import math
import numpy as np
import matplotlib.pyplot as plt
from sklearn.preprocessing import StandardScaler

phaseDiff = 1.3445
freq = 3
samples = 300

t = np.arange(samples) / samples

def main():
    maxCov = -1.0
    phase = 0

    # 時間配列を用意。1秒分。
    rawData = np.zeros((len(t), 2))

    # ノイズデータ生成（擬似的な生データ）
    noiseRange = 0.05
    noise = np.random.normal(size=(samples,1)) * noiseRange
    rawData[:,1] = np.sin(2 * math.pi * freq * t + phaseDiff) + np.sin(6 * math.pi * freq * t + phaseDiff)
    rawData[:,1] = rawData[:,1] + noise[:,0]

    # 初期状況
    rawData[:,0] = dataInit(0)
    data = standardization(rawData)
    plot(data)

    
    for i in range(628):
        rawData[:,0] = dataInit(i / 100)
        data = standardization(rawData)
    
        cov = calcCov(data)
        print(cov)
    
        if(cov > maxCov):
            maxCov = cov
            phase = i
    
    # 最終プロット
    rawData[:,0] = dataInit(phase / 100)
    data = standardization(rawData)
    c = np.cov(np.transpose(data))
    print('maxCov: ', maxCov, 'phase:', phase / 100)
    plot(data)

def standardization(data):
    scaler = StandardScaler()
    scaler.fit(data)
    data = scaler.transform(data)
    return data

def plot(data) :
    plt.figure()
    plt.title('Data')
    plt.plot(data[1:samples,:])

    plt.figure(figsize=(4,4))
    plt.title('Phase Diagram')
    plt.scatter(data[1:samples,0],data[1:samples,1])
    plt.show()

def calcCov(data):
    c = np.cov(np.transpose(data))
    return c[0,1]

def dataInit(phase):
    data = np.sin(2 * math.pi * freq * t + phase)
    return data

if __name__ == "__main__":
    main()