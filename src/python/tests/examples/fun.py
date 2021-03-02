def noParam():
    return True

def numParam(f):
    f = 1
    return f

def decisionFlow(b):
    if b == True:
        return noParam()
    else:
        return False

def callBehavior():
    noParam()
    return numParam(1)