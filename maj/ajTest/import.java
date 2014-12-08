package maj.ajTest;

import java.io.*;
import org.aspectj.compiler.base.*;
import org.aspectj.compiler.base.ast.*;
import org.aspectj.compiler.base.cst.*;
import org.aspectj.compiler.crosscuts.ast.*;

public class importtest {

    public static void main(String myargs[]) {

	// Import
	infer imp1 = `[import java.lang.*;];
	Import imp2 = `[import java.lang.*;];
	infer imp3 = `(Import)[import java.lang.*;];
	Import imp4 = `(Import)[import java.lang.*;];
	infer selfimp1 = `(Import)[#imp4];
	infer selfimp2 = `(Import)[#(Import)imp4];

	System.out.println(imp1.unparse());
	System.out.println(imp2.unparse());
	System.out.println(imp3.unparse());
	System.out.println(imp3.unparse());
	System.out.println(selfimp1.unparse());
	System.out.println(selfimp2.unparse());
    }
}
