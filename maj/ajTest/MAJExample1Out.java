aspect foo implements InterfaceOne, InterfaceTwo {
 
   OriginalClass_Aspect() {
    try {
      OriginalMethodHome sh;
      javax.naming.InitialContext initContext = new javax.naming.InitialContext();
      String JNDIName = JndiName.class;
      Object obj = initContext.lookup(JNDIName);
    } catch (Exception e) {
    }
  }
 
  Object around():call(* *(..))
        && !within(OriginalClass_Aspect)
        && !cflow(within(OriginalMethod))
        && target(OriginalMethod){
  try {
    Method meth = ep.getClass().getMethod(thisJointPoint.getSignature().getName(),
        thisJointPoint.getSignature().getParameterTypes());
    Object result = meth.invoke(ep, thisJoinPoint.getArgs());
    return result;
  } catch (Exception e) {
  }
}
}
