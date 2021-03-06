def noParam():
    return True

def numParam(f):
    f = 1
    return f

def decisionFlow(n):
    ret = 'c'
    if n == 1:
        ret = 'b'
    else:
        ret = 'a'
    return ret

def callBehavior():
    noParam()
    return numParam(1)