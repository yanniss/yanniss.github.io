package maj.ajTest;


import java.io.*;
import org.aspectj.compiler.base.*;
import org.aspectj.compiler.base.ast.*;
import org.aspectj.compiler.base.cst.*;
import org.aspectj.compiler.crosscuts.ast.*;

public class namepatterntest {
	public static void main(String[] myargs)  {
		ASTObject name1 = new AnyName();
		NamePattern name2 = new SimpleName(new Identifier(new Identifier("Foo"), new Identifier("*")));
		ASTObject name3 = new SimpleName(new Identifier(new Identifier("Foo"), new Identifier("*")));
		NamePattern name4 = new SimpleName(new Identifier("foo"));
		ASTObject packStar = new SimpleName(new Identifier(new Identifier(new Identifier("java"), new Identifier("lang")), new Identifier("*")));
		ASTObject selfname1 = packStar;
		System.out.println(name1.unparse());
		System.out.println(name2.unparse());
		System.out.println(name3.unparse());
		System.out.println(name4.unparse());
		System.out.println(packStar.unparse());
		System.out.println(selfname1.unparse());
	}
}
