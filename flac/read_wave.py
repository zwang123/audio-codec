import wave

with wave.open('xxxx.wav', 'rb') as f:
    f.setpos(4096 * 4 + 3626)
    print(f.readframes(2))
    # expect b'\x00\x00\x00\x00\xff\xff\x00\x00'
    f.setpos(4096 * 5 - 2)
    print(f.readframes(2))
    # expect b'\x01\x00\x00\x00\x00\x00\x00\x00'
