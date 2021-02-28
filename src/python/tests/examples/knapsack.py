import time
import timeit

#An exhaustive search function for the knapsack problem
#Input the amount of weight the knapsack can carry, the weight of each possible
#      item, the value of each possible item
#Output a tuple of the maximum value and a list of the values of each
def eSearch(size, weight, value):
    vM = 0
    mRock = []
    for i in range(len(weight)):
        if weight[i] <= size and len(weight) > 1:
            if value[i] > vM:
                vM = value[i]
                mRock = [value[i]]
            nWeight = weight[:]
            nWeight.remove(weight[i])
            nValue = value[:]
            nValue.remove(value[i])
            val, rock = eSearch(size-weight[i],nWeight,nValue)
            if val+value[i] > vM:
                vM = val + value[i]
                mRock = [value[i]] + rock
    return vM,mRock

#A dynamic search function for the knapsack problem
#Input the amount of weight the knapsack can carry, the weight of each possible
#      item, the value of each possible item
#Output a tuple of the maximum value and a list of the values of each
def dSearch(size, weight, value):
    if len(weight) < 1 or size == 0:
        return 0, []    
    else:
        rWeight = weight[:]
        rValue = value[:]
        rWeight.remove(weight[0])
        rValue.remove(value[0])
        if weight[0] > size:
            return dSearch(size,rWeight,rValue) 
        else:
            inclusive = dSearch(size-weight[0],rWeight,rValue)
            exclusive = dSearch(size,rWeight,rValue)
            if  value[0] + inclusive[0] > exclusive[0]:
                return inclusive[0]+value[0], inclusive[1]+[value[0]]
            else:
                return exclusive

#A helper function for the quicksort function that swaps two values in a list
#Input The list of ratios, the left index of the quicksort, and the right
#      of the quicksort
#Output N/A
def swap(vals, l, r):
    lP = vals[l]
    rP = vals[r]
    vals[l] = rP
    vals[r] = lP

#A quicksort function for the ratios of value to weight
#Input a list of tuples of the value and weight of each item
#Output a sorted list of the tuples based on their ratios
def sort(vals):
    l = 0
    r = len(vals)-1
    if r < 1:
        return vals
    m = (int)(0.5*r)+1
    middle = vals.pop(m)
    pivot = middle[0]/middle[1]
    r = r-1
    while r > l:
        while vals[l][0]/vals[l][1] >= pivot and r > l:
            l += 1
        while vals[r][0]/vals[r][1] < pivot and r > l:
            r -= 1
        if r > l:
            swap(vals, l, r)
    if l == len(vals)-1 and vals[l][0]/vals[l][1] > pivot:
        lList = vals[:]
        rList = []
    else:
        lList = vals[:l]
        rList = vals[l:]
    s = sort(lList)
    g = sort(rList)
    pivotA = [middle]
    vals = s + pivotA + g
    return vals

# A greedy search function for the knapsack problem
#Input the amount of weight the knapsack can carry, the weight of each possible
#      item, the value of each possible item
#Output a tuple of the maximum value and a list of the values of each
def gSearch(size,weight,value):
    vM = 0
    ratios = []
    sRatios = []
    sRem = size
    bestRocks = []
    for i in range(len(weight)):
        ratios.append((value[i],weight[i]))
    sRatios = sort(ratios[:])
    for r in sRatios:
        if weight[ratios.index(r)] <= sRem:
            sRem -= weight[ratios.index(r)]
            vM += value[ratios.index(r)]
            bestRocks.append(value[ratios.index(r)])
    return vM, bestRocks

#Times each knapsack search function
#Input the function, then the size, then the weight list, then the value list
#Output the time in seconds that it took the function to run
def timeFunc1(f,size,weight,value):
    s = time.time()
    f(size,weight,value)
    e = time.time()
    return abs(e-s)

def timeFunc2(f,size,weight,value):
    s = timeit.timeit()
    f(size,weight,value)
    e = timeit.timeit()
    return abs(e-s)

with open('test2.txt','r') as f:
    size = int(f.readline())
    weight = f.readline().split(',')
    value = f.readline().split(',')
    ratio = []
    for i in range(len(weight)):
        weight[i] = int(weight[i])
        value[i] = int(value[i])
        ratio.append((value[i],weight[i]))
    print('Greedy Result: ',gSearch(size,weight,value))
    print('Time: ',timeFunc2(gSearch,size,weight,value))
    print('Dynamic Result: ',dSearch(size, weight,value))
    print('Time: ', timeFunc1(dSearch,size,weight,value))
    print('Exauhstive Result: ',eSearch(size,weight,value))
    print('Time: ',timeFunc1(eSearch,size,weight,value))    
