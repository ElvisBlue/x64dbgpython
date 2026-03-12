from x64dbg import Bookmark, Register

def main():
    currentRIP = Register.Get(Register.RegisterEnum.RIP)
    print("Checking for bookmark at current RIP: 0x%X" % currentRIP)
    
    info = Bookmark.GetInfo(currentRIP)
    if info:
        print("Bookmark found!")
        print("Module: %s" % info.mod)
        print("RVA: 0x%X" % info.rva)
        print("Manual: %s" % info.manual)
    else:
        print("No bookmark at current RIP.")
        
    print("\nRetrieving all bookmarks...")
    bookmarkList = Bookmark.GetList()
    if bookmarkList:
        print("Found %d bookmarks." % len(bookmarkList))
        for i, bm in enumerate(bookmarkList[:5]): # Print up to top 5
            print("  Bookmark %d: Module %s, RVA 0x%X" % (i, bm.mod, bm.rva))
        if len(bookmarkList) > 5:
            print("  ...")
    else:
        print("No bookmarks found in the list.")

if __name__ == "__main__":
    main()
