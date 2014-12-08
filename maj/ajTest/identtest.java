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
		Identifier pack1 = new Identifier(new Identifier("foo"), new Identifier("bar"));
		Identifier pack2 = new Identifier(new Identifier(new Identifier("java"), new Identifier("lang")), pack1);
		Identifier selfid1 = pack1;
		Identifier selfid2 = pack1;
		Identifier selfid3 = pack1;
		//		Identifier crazyid = x + y;
		System.out.println(id1.unparse());
		System.out.println(id2.unparse());
		System.out.println(id3.unparse());
		System.out.println(id4.unparse());
		System.out.println(pack1.unparse());
		System.out.println(pack2.unparse());
		System.out.println(selfid1.unparse());
		System.out.println(selfid2.unparse());
		System.out.println(selfid3.unparse());
		//System.out.println(crazyid.unparse());
	}
}
