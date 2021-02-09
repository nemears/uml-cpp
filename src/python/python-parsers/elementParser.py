from yuml_python import *

def test():
    e = Element()
    print(e.getID())
    child = Element()
    e.ownedElements.append(child)
    print(e.ownedElements[0].getID())
    return isValidUUID4(e.getID())

if __name__ == "__main__":
    print(test())
    e = Element()
    try:
        e.setID("not a valid id")
    except InvalidID_Exception:
        print("could not set id")