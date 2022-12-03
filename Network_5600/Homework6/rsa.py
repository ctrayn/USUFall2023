import math

MESSAGE = "hello"

e = 4
n = 33
k = math.floor(math.log2(n))
mask = (1 << k) - 1

bits = int(''.join(format(ord(x) - ord('a'), 'b') for x in MESSAGE), 2)

print(bin(bits))

num = math.floor((len(bin(bits)) / k) + 0.5)

print(num)

orig = []

for i in range(num):
    orig.insert(0, bits & mask)
    bits = bits >> k

encrypted = []

for seg in orig:
    encrypted.append(int(math.pow(seg,e) % n))

to_send = ''.join([f"{x:05b}" for x in encrypted])

print(to_send)
