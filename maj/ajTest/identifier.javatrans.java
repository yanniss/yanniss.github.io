package maj.ajTest;


import java.io.*;
import org.aspectj.compiler.base.*;
import org.aspectj.compiler.base.ast.*;
import org.aspectj.compiler.base.cst.*;
import org.aspectj.compiler.crosscuts.ast.*;

public class identtest {
	public static void main(String[] myargs)  {
		Identifier id1 = new Identifier("foo");
		Identifier id2 = new Identifier("foo");
		Identifier id3 = new Identifier("foo");
		Identifier id4 = new Identifier("foo");
		SimpleName id5 = new SimpleName(new Identifier(new Identifier("foo"), new Identifier("*")));
		Identifier pack1 = new Identifier(new Identifier("foo"), new Identifier("bar"));
		Identifier pack2 = new Identifier(new Identifier(new Identifier("java"), new Identifier("lang")), pack1);
		ASTObject selfid1 = pack1;
		ASTObject selfid2 = pack1;
		System.out.println(id1.unparse());
		System.out.println(id2.unparse());
		System.out.println(id3.unparse());
		System.out.println(id4.unparse());
		System.out.println(id5.unparse());
		System.out.println(pack1.unparse());
		System.out.println(pack2.unparse());
		System.out.println(selfid1.unparse());
		System.out.println(selfid2.unparse());
	}
}
