package maj.ajTest;

import java.io.*;
import org.aspectj.compiler.base.*;
import org.aspectj.compiler.base.ast.*;
import org.aspectj.compiler.base.cst.*;
import org.aspectj.compiler.crosscuts.ast.*;

public class MAJExample1Test {

    public static void main(String myargs[]) {

	infer aspect1 =
	    `[aspect foo {} ];

	infer interface1 = `(TypeD)[InterfaceOne];

	infer interId = `[InterfaceTwo];
	infer interface2 = `(TypeD)[#(TypeD)interId];

	aspect1.addInterface(interface1);
	aspect1.addInterface(interface2);

	infer aspectName = `[OriginalClass_Aspect];

	infer pct1 = `[call(* *(..))];
	infer pct2 = `[!within(OriginalClass_Aspect)]; 
	infer pct3 = `[!cflow(within(OriginalMethod))];
	infer pct4 = `[target(OriginalMethod)];

	infer adv1 = `[Object around() : #(Pointcut)pct1 
		                         && #pct2 
	                                 && #pct3 
	                                 && #pct4 { 
					     try {
						 Method meth = ep.getClass().getMethod(thisJointPoint.getSignature().getName(),
										       (thisJointPoint.getSignature()).getParameterTypes());
						 Object result = meth.invoke(ep, thisJoinPoint.getArgs());
						 return result;
					     } catch (Exception e) {}

					 }];

	infer pct5 = `[target(OriginalMethod) 
		       && (!within(OriginalClass_Aspect)) 
		       && (!cflow(within(OriginalMethod)))];
	infer dec1 = `[private InterfaceName ep;];
	
	Identifier id1 = new Identifier("JndiName");
	infer meth1 = `[OriginalClass_Aspect() {
	    try {
		OriginalMethodHome sh;

		javax.naming.InitialContext initContext = 
		                 new javax.naming.InitialContext();
		String JNDIName = #id1;
		Object obj = initContext.lookup(JNDIName);
	    } catch (Exception e) { } 
	}];

	aspect1.addMember(meth1);
	aspect1.addMember(adv1);

	System.out.println(aspect1.unparse());
    }
}
