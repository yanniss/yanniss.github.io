package maj.ajTest;

import java.io.*;
import org.aspectj.compiler.base.*;
import org.aspectj.compiler.base.ast.*;
import org.aspectj.compiler.base.cst.*;
import org.aspectj.compiler.crosscuts.ast.*;

public class typespectest {

    public static void main(String myargs[]) {
	// Type Spec
	infer type1 = `[int];
	infer type2 = `[int[]];
	infer type3 = `[int[][]];
	TypeD type4 = `[int];
	infer type5 = `(TypeD)[int];
	TypeD type6 = `(TypeD)[int[]];
	TypeD type7 = `(TypeD)[Foo.Bar];
	TypeD type8 = `(TypeD)[#type1];
	TypeD type9 = `(TypeD)[#(Identifier)type1];
	TypeD type10 = `(TypeD)[#type1[]];
	infer selftyp1 = `(TypeD)[#type4];
	infer selftyp2 = `(TypeD)[#(TypeD)type4];

	System.out.println(type1.unparse());
	System.out.println(type2.unparse());
	System.out.println(type3.unparse());
	System.out.println(type4.unparse());
	System.out.println(type5.unparse());
	System.out.println(type6.unparse());
	System.out.println(type7.unparse());
	System.out.println(type8.unparse());
	System.out.println(type9.unparse());
	System.out.println(selftyp1.unparse());
	System.out.println(selftyp2.unparse());

    }
}
