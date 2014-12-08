package maj.ajTest;


import java.io.*;
import org.aspectj.compiler.base.*;
import org.aspectj.compiler.base.ast.*;
import org.aspectj.compiler.base.cst.*;
import org.aspectj.compiler.crosscuts.ast.*;

public class importtest {
	public static void main(String[] myargs)  {
		Import imp1 = new Import(new Identifier(new Identifier(new Identifier("java"), new Identifier("lang")), new Identifier("*")));
		Import imp2 = new Import(new Identifier(new Identifier(new Identifier("java"), new Identifier("lang")), new Identifier("*")));
		Import imp3 = new Import(new Identifier(new Identifier(new Identifier("java"), new Identifier("lang")), new Identifier("*")));
		Import imp4 = new Import(new Identifier(new Identifier(new Identifier("java"), new Identifier("lang")), new Identifier("*")));
		Import selfimp1 = imp4;
		Import selfimp2 = imp4;
		System.out.println(imp1.unparse());
		System.out.println(imp2.unparse());
		System.out.println(imp3.unparse());
		System.out.println(imp3.unparse());
		System.out.println(selfimp1.unparse());
		System.out.println(selfimp2.unparse());
	}
}
