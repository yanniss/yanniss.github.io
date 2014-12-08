package maj.ajTest;

import java.io.*;
import org.aspectj.compiler.base.*;
import org.aspectj.compiler.base.ast.*;
import org.aspectj.compiler.base.cst.*;
import org.aspectj.compiler.crosscuts.ast.*;

public class intertypetest {

    public static void main(String myargs[]) {
	// (intertype declarations)
	infer var12 =  `[Foo Bar.x;];
	infer var13 =  `[Foo x,Bar.y;];

	infer id1 = `[foo];
	infer var1A =  `(VarDec)[int x;];
	infer var1B =  `[int #id1;];
	infer var1C =  `[int #(Identifier)id1;];
	infer var2A =  `(VarDec)[int[] x;];
	infer var2B =  `[int[] #id1;];
	infer var2C =  `[int[] #(Identifier)id1;];

	infer var3B =  `[int #id1[];];
	infer var3C =  `[int #(Identifier)id1[];];

	infer var4A =  `(VarDec)[Foo x;];
	infer var4B =  `[Foo #id1;];
	infer var4C =  `[Foo #(Identifier)id1;];

	infer var5A =  `(VarDec)[Foo[] x;];
	infer var5B =  `[Foo[] #id1;];
	infer var5C =  `[Foo[] #(Identifier)id1;];

	infer var6A =  `(VarDec)[Foo x[];];
	infer var6B =  `[Foo #id1[];];
	infer var6C =  `[Foo #(Identifier)id1[];];

	infer var7A =  `(VarDec)[Foo.Bar x;];
	infer var7B =  `[Foo.Bar #id1;];
	infer var7C =  `[Foo.Bar #(Identifier)id1;];

	infer var8A =  `(VarDec)[Foo.Bar[] x;];
	infer var8B =  `[Foo.Bar[] #id1;];
	infer var8C =  `[Foo.Bar[] #(Identifier)id1;];

	infer var9A =  `(VarDec)[Foo.Bar x[];];
	infer var9B =  `[Foo.Bar #id1[];];
	infer var9C =  `[Foo.Bar #(Identifier)id1[];];

	System.out.println(var12.unparse());
	System.out.println(var13.unparse());
	System.out.println(id1.unparse());
	System.out.println(var1A.unparse());
	System.out.println(var1B.unparse());
	System.out.println(var1C.unparse());

	System.out.println(var2A.unparse());
	System.out.println(var2B.unparse());
	System.out.println(var2C.unparse());

	System.out.println(var3B.unparse());
	System.out.println(var3C.unparse());

	System.out.println(var4A.unparse());
	System.out.println(var4B.unparse());
	System.out.println(var4C.unparse());
	System.out.println(var5A.unparse());
	System.out.println(var5B.unparse());
	System.out.println(var5C.unparse());
	System.out.println(var6A.unparse());
	System.out.println(var6B.unparse());
	System.out.println(var6C.unparse());
	System.out.println(var7A.unparse());
	System.out.println(var7B.unparse());
	System.out.println(var7C.unparse());

	System.out.println(var8A.unparse());
	System.out.println(var8B.unparse());
	System.out.println(var8C.unparse());

	System.out.println(var9A.unparse());
	System.out.println(var9B.unparse());
	System.out.println(var9C.unparse());
    }
}
