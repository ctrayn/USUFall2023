INPUT=0b10010010100000110110011011111110110110010101000001111001110111111101010101110

TARGET_STRING = "Hello World"

target_binary = int(''.join(format(ord(x), 'b') for x in TARGET_STRING), 2)

result_binary = INPUT ^ target_binary

print(bin(result_binary))

