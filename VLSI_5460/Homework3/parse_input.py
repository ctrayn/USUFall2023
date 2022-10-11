from cmath import sqrt

with open("input_file.txt", 'r') as r:
    for line in r.readlines():
        parts = line.split('\t')
        area = float(parts[1]).real
        ratio = float(parts[2]).real
        print(f"{parts[0]} height {sqrt(area / ratio)} width {sqrt(area * ratio)}")