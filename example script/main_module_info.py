from x64dbg import Module

def PrintModuleInfo(moduleInfo):
    print("Base: 0x%X" % moduleInfo.base)
    print("Entry: 0x%X" % moduleInfo.entry)
    print("Name: %s" % moduleInfo.name)
    print("Path: %s" % moduleInfo.path)
    print("Section count: %d" % moduleInfo.sectionCount)
    print("Module size: 0x%X" % moduleInfo.size)
    
def PrintSectionInfo(sectionInfo):
    print("Addr: 0x%X" % sectionInfo.addr)
    print("Name: %s" % sectionInfo.name)
    print("Size: 0x%X" % sectionInfo.size)

def PrintImportInfo(importInfo):
    print("IAT RVA: 0x%X" % importInfo.iatRva)
    print("IAT VA: 0x%X" % importInfo.iatVa)
    print("Ordinal: 0x%X" % importInfo.ordinal)
    print("Name: %s" % importInfo.name)
    print("undecoratedName: %s" % importInfo.undecoratedName)

def main():
    print("Main module information")
    print("-------------------------------------")
    mainModule = Module.GetMainModuleInfo()
    PrintModuleInfo(mainModule)
    print("Main module section information")
    print("-------------------------------------")
    sectionList = Module.SectionListFromAddr(mainModule.base)
    for i in range(0, len(sectionList)):
        print("Section #%d" % (i + 1))
        PrintSectionInfo(sectionList[i])
        print("-------------------------------------")
    
    print("Import information")
    
    
    importList = Module.GetImports(mainModule)
    for item in importList:
        PrintImportInfo(item)
        
    print("-------------------------------------")
    
if __name__ == "__main__":
    main()