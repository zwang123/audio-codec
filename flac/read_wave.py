import wave

with wave.open('xxxx.wav', 'rb') as f:
    f.setpos(4096 * 4 - 1)
    print(f.readframes(2))
