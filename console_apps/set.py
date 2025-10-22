def input_set():
    return set(map(int, input("Enter integers: ").split()))

def get_union(a, b):
    return set(i for i in a.union(b) if abs(i) <= 10)

a = input_set()
b = input_set()


for i in  get_union(a, b):
   print(i, end=' ')
    