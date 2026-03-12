from x64dbg import Comment, Register

def main():
    currentRIP = Register.Get(Register.RegisterEnum.RIP)
    print("Checking for comment at current RIP: 0x%X" % currentRIP)
    
    text = Comment.Get(currentRIP)
    if text:
        print("Comment text: %s" % text)
    else:
        print("No comment text at current RIP.")
        
    info = Comment.GetInfo(currentRIP)
    if info:
        print("\nComment Info found!")
        print("Module: %s" % info.mod)
        print("RVA: 0x%X" % info.rva)
        print("Text: %s" % info.text)
        print("Manual: %s" % info.manual)
        
    print("\nRetrieving all comments...")
    commentList = Comment.GetList()
    if commentList:
        print("Found %d comments." % len(commentList))
        for i, c in enumerate(commentList[:5]): # Print up to top 5
            print("  Comment %d: '%s' at RVA 0x%X (%s)" % (i, c.text, c.rva, c.mod))
        if len(commentList) > 5:
            print("  ...")
    else:
        print("No comments found in the list.")

if __name__ == "__main__":
    main()
