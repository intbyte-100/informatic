def input_list(vec):
    print(f"Enter {len(vec)} integers: ")
    return [int(input()) for _ in range(len(vec))]

def count_negative(vec):
    return sum(1 for number in vec if number < 0)
    
def compare_and_return(a, b):
    return a if count_negative(a) > count_negative(b) else b
    

    

a = input_list([0] * 5)
b = input_list([0] * 5)


print(" ".join(map(str, compare_and_return(a, b))))

