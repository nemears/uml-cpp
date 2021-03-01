def noParam():
    return True

def numParam(f):
    f += 1
    return f

def decisionFlow(b):
    if b:
        print('b is true')
    else:
        print('b is false')

def callBehavior():
    noParam()
    return numParam(1)