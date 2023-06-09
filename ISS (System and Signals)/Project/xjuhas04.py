import numpy as np
import matplotlib.pyplot as plt
import soundfile as sf
import IPython
from scipy.signal import spectrogram, lfilter, freqz, tf2zpk
s, fs = sf.read('xjuhas04.wav')

#4.1
print("vzorkovacia frek.",fs)
print("pocet vzorkov signalu",s.size)
print("dlzka signalu v sekundach",(s.size)/fs,"[s]")
tempm = s[0]
tempM = s[0]
for val in s:
    if tempm > val:
        tempm = val
    elif tempM < val:
        tempM = val
print("min value",tempm)
print("max value",tempM)

t = np.arange(s.size) / fs
plt.figure(figsize=(6,3))
plt.plot(t, s)

plt.gca().set_xlabel('$t[s]$')
plt.gca().set_title('Zvukový signál')

plt.tight_layout()
plt.show()

#4.2
s -= np.mean(s)
Nframe = (s.size / 512)-1
frames = []
timeFrames = []

for l in range(int(Nframe)):
    frame = []
    time = []
    for x in range(1024):
        frame.append(s[x + (l*512)])
        time.append(t[x + (l*512)])
    frames.append([])
    timeFrames.append([])
    frames[l] = frame
    timeFrames[l] = time

plt.figure(figsize=(10, 4))
plt.title("Znely ramec")
plt.plot( timeFrames[9],frames[9])
plt.ylabel('Amplitude')
plt.xlabel('Seconds [s]')
plt.show()


def dft_matrix(signal):
    N = signal.shape[0]
    w = np.exp((-2 * np.pi * 1j) / N) 
    r = np.arange(N)
    w_matrix = np.vander(w ** r, increasing=True)  
    return w_matrix
vector = np.array(frames[9])
matrix = dft_matrix(vector)
result = matrix.dot(vector)
for i in range(512):
    result[i+512] = 0
x = np.arange(0,fs/2,fs/2/1024)
plt.title("DFT")
plt.grid(True)
plt.plot(x,abs(result))
plt.show()
plt.title("FFT")
plt.grid(True)
fft = np.fft.fft(np.array(frames[9]))
for i in range(512):
    fft[i+512] = 0

plt.plot(x,abs(fft))
plt.show()
f,t,sgr = spectrogram(s, fs, nperseg=1024,noverlap=512)
sgr_log = 10 * np.log10(sgr+1e-20)
plt.figure("4.4")
plt.pcolormesh(t,f,sgr_log)
plt.gca().set_xlabel('cas')
plt.gca().set_ylabel('Hz')
cbar = plt.colorbar()
cbar.set_label('spektralna hustota vykonu',rotation = 270,labelpad=15)
plt.tight_layout()
plt.show()
f1 = 610
f2 = 1220
f3 = 1830
f4 = 2440

times = []
for k in range(s.size):
    times.append(k/fs)
output_cos1 = np.cos(2 * np.pi * f1 * np.array(times))
output_cos2 = np.cos(2 * np.pi * f2 * np.array(times))
output_cos3 = np.cos(2 * np.pi * f3 * np.array(times))
output_cos4 = np.cos(2 * np.pi * f4 * np.array(times))
output_cos = output_cos1 + output_cos2 + output_cos3 + output_cos4
sf.write('4cos.wav',output_cos,fs)
f,t,sgr = spectrogram( output_cos,fs, nperseg=1024,noverlap=512)
sgr_log = 10 * np.log10(sgr+1e-20)
plt.figure("4.4")
plt.pcolormesh(t,f,sgr_log)
plt.gca().set_xlabel('cas')
plt.gca().set_ylabel('Hz')
cbar = plt.colorbar()
cbar.set_label('spektralna hustota vykonu',rotation = 270,labelpad=15)
plt.tight_layout()
plt.show()

