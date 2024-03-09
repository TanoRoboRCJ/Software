import math
import numpy as np
import matplotlib.pyplot as plt
import lidar
from sklearn.preprocessing import StandardScaler

freq = 6
samples = 181  # 90 + 90 + 0

t = np.arange(samples) / samples


def main():
    rawData = np.zeros((len(t), 2))

    # よっ！お待ちかね測定データのヒストグラムだ！
    rawData[:, 1] = lidar.lidar

    # NOTE: 位相推定
    maxCov = -1000.0
    phase = 0
    for i in range(628):
        rawData[:, 0] = dataInit(i / 100)

        cov = calcCov(rawData)
        print(cov)

        if cov > maxCov:
            maxCov = cov
            phase = i

    # 最終プロット
    rawData[:, 0] = dataInit(phase / 100)
    c = np.cov(np.transpose(rawData))

    place = (phase / 100) / (2 * math.pi) * 300
    if place > 150:
        place = place - 300

    print("maxCov: ", maxCov, "phase:", phase / 100)
    print("place:", place)
    plot(rawData)


def plot(data):
    plt.figure()
    plt.title("Data")
    plt.plot(data[1:samples, :])

    plt.figure(figsize=(4, 4))
    plt.title("Phase Diagram")
    plt.scatter(data[1:samples, 0], data[1:samples, 1])
    plt.show()


def calcCov(data):
    c = np.cov(np.transpose(data))
    return c[0, 1]


def dataInit(phase):
    data = np.cos(2 * math.pi * freq * t + phase)
    return data


if __name__ == "__main__":
    main()
