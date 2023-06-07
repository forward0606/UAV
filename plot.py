import matplotlib.pyplot as plt
import re

# check str is number or not
def is_number(num):
  pattern = re.compile(r'^[-+]?[-0-9]\d*\.\d*|[-+]?\.?[0-9]\d*$')
  result = pattern.match(num)
  if result:
    return True
  else:
    return False

# open file
file = open('test', 'r')
lines = file.readlines()
file.close()

# flag 
key = 0
flag = 0
count = 0
record = 0
node_num = 0
vec = []

# read file
for line in lines:
    if "portal" in line:
        flag = 1

    if "corner" in line:
        flag = 2

    if "the origin input" in line:
        key = -1

    if "the scaled input" in line:
        key = 1

    if "list of node: x, y" in line:
        flag = 3
        print("FLAG")

    if "num_of_node, B" in line:
        record = 1


    # draw portal
    if flag == 1:
        num = line.split()
        if len(num) == 2 and is_number(num[0]) and is_number(num[1]):
            count += 1
            x = float(num[0])
            y = float(num[1])
            plt.scatter(x, y, c = 'blue', s = 5)
            if count == 12:
                count = 0
                flag = 0

    if flag == 2:
        num = line.split()
        if len(num) == 3 and is_number(num[1]) and is_number(num[2]):
            count += 1
            x = float(num[1])
            y = float(num[2])
            vec.append((x, y))
            if count == 4:
                print("SQUARE:")
                for (x, y) in vec:
                    print((x, y))
                # 01 02 23 13
                plt.plot((vec[0][0], vec[1][0]), (vec[0][1], vec[1][1]), "gray")
                plt.plot((vec[0][0], vec[2][0]), (vec[0][1], vec[2][1]), "gray")
                plt.plot((vec[1][0], vec[3][0]), (vec[1][1], vec[3][1]), "gray")
                plt.plot((vec[2][0], vec[3][0]), (vec[2][1], vec[3][1]), "gray")
                vec = []
                count = 0
                flag = 0

    if flag == 3:
        num = line.split(' ')
        print(num)
        if len(num) == 2 and is_number(num[0]) and is_number(num[1]):
            count += 1
            x = float(num[0])
            y = float(num[1])
            vec.append((x, y))
            if count == node_num:
                if key == 1:
                    print("SCALED: ")
                    color = "red"
                elif key == -1:
                    print("ORINGINAL: ")
                    color = "black"                
                for (x, y) in vec:
                    print((x, y))
                    plt.scatter(x, y, c = color, s = 30)                
                vec = []
                count = 0
                flag = 0


    if record == 1:
        num = line.split(',')
        if is_number(num[0]):
            node_num = int(num[0])
            print("NODE NUM: ", node_num)
            record = 0

plt.show()
