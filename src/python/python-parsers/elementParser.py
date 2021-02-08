import yuml_python

def test():
    return yuml_python.isValidUUID4("i'm not")

if __name__ == "__main__":
    print(test())