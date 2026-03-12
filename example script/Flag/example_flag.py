from x64dbg import Flag

def main():
    print("Testing Flag API...")
    
    zf = Flag.GetZF()
    cf = Flag.GetCF()
    
    print("Current Zero Flag (ZF): %s" % str(zf))
    print("Current Carry Flag (CF): %s" % str(cf))
    
    print("Toggling Zero Flag...")
    Flag.SetZF(not zf)
    print("New Zero Flag (ZF): %s" % str(Flag.GetZF()))
    
    # Restore original
    Flag.SetZF(zf)
    
    print("Getting Flag by enum...")
    sf = Flag.Get(Flag.FlagEnum.SF)
    print("Current Sign Flag (SF): %s" % str(sf))

if __name__ == "__main__":
    main()
