package maj.ajTest;

import java.io.*;
import org.aspectj.compiler.base.*;
import org.aspectj.compiler.base.ast.*;
import org.aspectj.compiler.base.cst.*;
import org.aspectj.compiler.crosscuts.ast.*;

public class vardectest {

    public static void main(String myargs[]) {
	// Var. Dec.
	infer var1 =  `[private int x;];
	infer var2 =  `[int[] x;];
	infer var3 =  `[int x[];];
	infer var4 =  `[Foo x;];
	infer var5 =  `[Foo[] x;];
	infer var6 =  `[Foo x[];];
	infer var7 =  `[Foo.Bar x;];
	infer var8 =  `[Foo.Bar[] x;];
	infer var9 =  `[Foo.Bar x[];];
	infer var10 =  `[int x=3;];
	infer var11 =  `[int x[]={1, 2, 3};];
	infer var12 =  `(VarDec)[int x[]={1, 2, 3};];
	//	infer var13 = `(VarDec)[#var12];
	// infer var13 = `[#(VarDec) var12];
	//	infer var12 = `[int x, y];
	
	System.out.println(var1.unparse());
	System.out.println(var2.unparse());
	System.out.println(var3.unparse());
	System.out.println(var4.unparse());
	System.out.println(var5.unparse());
	System.out.println(var6.unparse());
	System.out.println(var7.unparse());
	System.out.println(var8.unparse());
	System.out.println(var9.unparse());
	System.out.println(var10.unparse());
	System.out.println(var11.unparse());
	System.out.println(var12.unparse());
	//	System.out.println(var13.unparse());

	/*&
	try {
	    File outputFile = new File("/home/shuang/research/maj/ajTest/expr_trans.java");
	    FileWriter out = new FileWriter(outputFile);
	    
	    out.write(((maj.ExtendedAST)myAST).toSource());
	    out.close();
	} catch (Exception e) {
	    e.printStackTrace();
	    }*/
    }
}
