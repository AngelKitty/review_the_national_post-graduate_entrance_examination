#
# Example of using Python lists
#
box = [ 'pencil', 'pen', 'ruler', 'rubber' ]
print box
#['pencil', 'pen', 'ruler', 'rubber']
#
for item in box: print item,
print
# pencil pen ruler rubber
#
box.reverse()
for item in box: print item,
print
# rubber ruler pen pencil
#
box.sort()
for item in box: print item,
print
# pen pencil rubber ruler
#
for i in range(0, len(box)): print box[i],
print
# pen pencil rubber ruler
#
for i in range(len(box)-1, -1, -1): print box[i],
print
# ruler rubber pencil pen
#
for i in range(-1, -len(box)-1, -1): print box[i],
print
# ruler rubber pencil pen
#
bag = [ 'data structures', 'calculus', box, 2012012012 ]
print bag
# ['data structures', 'calculus', ['pen', 'pencil', 'rubber', 'ruler'], 2012012012]
for item in bag: print item,
print
# data structures calculus ['pen', 'pencil', 'rubber', 'ruler'] 2012012012
for item in bag[2]: print item,
print
# pen pencil rubber ruler
for item in bag[2][1:3]: print item,
print
# pencil rubber
