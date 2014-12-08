package maj.ajTest;

import java.io.*;
import org.aspectj.compiler.base.*;
import org.aspectj.compiler.base.ast.*;
import org.aspectj.compiler.base.cst.*;
import org.aspectj.compiler.crosscuts.ast.*;

public class nptest {

    public static void main(String myargs[]) {
	// Name Pattern
	infer name1 = `[*];
	NamePattern name2 = `[Foo.*];
	infer name3 = `(NamePattern)[Foo.*];
	NamePattern name4 = `(NamePattern)[foo];
	infer packStar = `[java.lang.*];

	infer selfname1 = `(NamePattern)[#packStar];
//	infer selfname2 = `(NamePattern)[#(NamePattern)pack1];

	System.out.println(name1.unparse());
	System.out.println(name2.unparse());
	System.out.println(name3.unparse());
	System.out.println(name4.unparse());
	System.out.println(packStar.unparse());
	System.out.println(selfname1.unparse());
    }
}
