class adapter_list:
    adapters = []

    def __init__(self, funcs = None):
        if funcs != None:
            for func in funcs:
                self.adapters.append((0, func))

    def add(self, obj):
        for b in range(0, len(self.adapters)):
            (useageB, adapterB) = self.adapters[b]
            if adapterB == obj:
                useageB += 1

                # Go backwards and insert new B 
                if b > 0:
                    for a in range(b-1, -2, -1):
                        if a == -1 or useageB < self.adapters[a][0] :
                            del self.adapters[b]
                            self.adapters.insert(a+1 , (useageB, adapterB))
                            return

                # Useage isn't greater than any other adapter
                self.adapters[b] = (useageB, adapterB)
                return

        self.adapters.append((1,obj))

    def remove(self, obj):
        for b in range(0, len(self.adapters)):
            (useageB, adapterB) = self.adapters[b]
            if adapterB == obj:
                useageB -= 1

                if useageB < 0:
                    useageB = 0

                # Go forwards and insert new B
                for a in range(b, len(self.adapters)+1, 1):
                    # If useageA is less than useageB or A is after the last element 
                    # Note: useageA = self.adapters[a][0]
                    if a == len(self.adapters) or useageB >= self.adapters[a][0]:
                        # Insert B before A
                        self.adapters.insert(a, (useageB, adapterB))
                        del self.adapters[b]
                        return
                # Useage isn't less than any other adapter
                self.adapters[b] = (useageB, adapterB)
                return

# test adaper_list:
def equals(should, actual):
    if should != actual:
        raise AssertionError(str(should) + " != " + str(actual))

def printL(l):
    print "printing"
    for (use, ada) in l:
        print use, ada

def test():
    alist = adapter_list()
    alist.add("hello")
    alist.add("hello")
    alist.add("world")
    alist.add("world")
    alist.add("hello")
    alist.add("hello")
    alist.add("world")
    alist.add("world")
    equals([(4,'world'),(4,'hello')], alist.adapters)

    alist.add("hej")
    alist.add("hej")
    alist.add("hej")
    alist.add("hej")
    alist.add("hej")
    equals([(5,'hej'),(4,'world'),(4,'hello')], alist.adapters)

    alist.remove("hej")
    equals([(4,'hej'),(4,'world'),(4,'hello')], alist.adapters)
    alist.remove("hej")
    equals([(4,'world'),(4,'hello'),(3,'hej')], alist.adapters)

    alist.remove("hej")
    equals([(4,'world'),(4,'hello'),(2,'hej')], alist.adapters)
    alist.remove("world")
    equals([(4,'hello'),(3,'world'),(2,'hej')], alist.adapters)

    alist.remove("world")
    alist.remove("world")
    alist.remove("world")
    equals([(4,'hello'),(2,'hej'),(0,'world')], alist.adapters)

    alist.remove("hej")
    equals([(4,'hello'),(1,'hej'),(0,'world')], alist.adapters)
    alist.remove("hej")
    equals([(4,'hello'),(0,'hej'),(0,'world')], alist.adapters)
    alist.remove("hej")
    equals([(4,'hello'),(0,'hej'),(0,'world')], alist.adapters)


    alist.remove("hello")
    alist.remove("hello")
    equals([(2,'hello'),(0,'hej'),(0,'world')], alist.adapters)
    alist.remove("hello")
    alist.remove("hello")
    equals([(0,'hello'),(0,'hej'),(0,'world')], alist.adapters)

    # Another test
    alist.adapters = [(8,'world'), (8,'hello'),(5,'hej')]
    alist.add('hej')
    equals([(8,'world'), (8,'hello'),(6,'hej')], alist.adapters)

if __name__ == "__main__":
    test()
    print "test successful!"