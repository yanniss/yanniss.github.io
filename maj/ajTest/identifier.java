package maj.ajTest;

import java.io.*;
import org.aspectj.compiler.base.*;
import org.aspectj.compiler.base.ast.*;
import org.aspectj.compiler.base.cst.*;
import org.aspectj.compiler.crosscuts.ast.*;

public class identtest {

    public static void main(String myargs[]) {
	// Identifier

	infer id1 = `[foo];
	Identifier id2 = `[foo];
	infer id3 = `(Identifier)[foo];
	Identifier id4 = `(Identifier)[foo];
	infer pack1 = `[foo.bar];
	infer pack2 = `[java.lang.#pack1];

	infer selfid1 = `(Identifier)[#pack1];
	infer selfid2 = `(Identifier)[#(Identifier)pack1];
	infer selfid3 = `[#(Identifier)pack1];
	infer crazyid = `[#(Identifier)[x+y]];

	System.out.println(id1.unparse());
	System.out.println(id2.unparse());
	System.out.println(id3.unparse());
	System.out.println(id4.unparse());

	System.out.println(pack1.unparse());
	System.out.println(pack2.unparse());

	System.out.println(selfid1.unparse());
	System.out.println(selfid2.unparse());
	System.out.println(selfid3.unparse());
	System.out.println(crazyid.unparse());
    }
}
