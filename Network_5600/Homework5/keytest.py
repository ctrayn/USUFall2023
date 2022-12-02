TEXT = 0x11223344
KEY  = 0x11111111

transmission = TEXT ^ KEY
answer = transmission ^ KEY

print(f'{answer:X}')