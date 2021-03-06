def noParam():
    return True

def numParam(f):
    f = 1
    return f

def decisionFlow(b):
    ret = 'c'
    if b == 1:
        ret = 'b'
    elif b == 2:
        ret = 'a'
    return ret

def callBehavior():
    noParam()
    return numParam(1)