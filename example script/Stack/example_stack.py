from x64dbg import Stack, Register

def main():
    currentCSP = Register.Get(Register.RegisterEnum.CSP)
    print("Testing Stack API...")
    print("Current Stack Pointer (CSP): 0x%X" % currentCSP)
    
    # Peek top of the stack
    print("\nPeek at top of stack...")
    val1 = Stack.Peek(0)
    print("[CSP + 0] = 0x%X" % val1)
    
    ptr_size = Register.Size()
    val2 = Stack.Peek(ptr_size)
    print("[CSP + %d] = 0x%X" % (ptr_size, val2))
    
    # Let's not actually push/pop in this example since it modifies runtime execution 
    # and might crash if we don't clean it up just right.
    # But here is how you would use it:
    # Stack.Push(0x1337)
    # top = Stack.Pop()

if __name__ == "__main__":
    main()
