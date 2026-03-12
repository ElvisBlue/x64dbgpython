from x64dbg import Label, Register

def main():
    currentRIP = Register.Get(Register.RegisterEnum.RIP)
    print("Testing Label API at current RIP: 0x%X" % currentRIP)
    
    label_text = Label.Get(currentRIP)
    if label_text:
        print("Label at current RIP: %s" % label_text)
    else:
        print("No label at current RIP.")
        
    print("\nSetting a temporary label at current RIP...")
    Label.Set(currentRIP, "MyTempLabel", True)
    
    print("New label text: %s" % Label.Get(currentRIP))
    
    print("Resolving 'MyTempLabel' address...")
    addr = Label.FromString("MyTempLabel")
    print("Resolved to: 0x%X" % addr)
    
    print("Deleting 'MyTempLabel'...")
    Label.Delete(currentRIP)
    
    print("\nRetrieving all labels...")
    labelList = Label.GetList()
    if labelList:
        print("Found %d labels." % len(labelList))
        for i, lbl in enumerate(labelList[:5]): # Print up to top 5
            print("  Label %d: '%s' at RVA 0x%X (%s)" % (i, lbl.text, lbl.rva, lbl.mod))
        if len(labelList) > 5:
            print("  ...")
    else:
        print("No labels found in the list.")

if __name__ == "__main__":
    main()
