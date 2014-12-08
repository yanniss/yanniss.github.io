package maj.ajTest;

import java.io.*;
import org.aspectj.compiler.base.*;
import org.aspectj.compiler.base.ast.*;
import org.aspectj.compiler.base.cst.*;
import org.aspectj.compiler.crosscuts.ast.*;

public class methodtest {

    public static void main(String myargs[]) {

	// Method.
	infer mods1 = `[public static];

	infer meth1 =
		`[int foo(int x, int y) throws something, somethingelse {
			#(Modifier[])mods1 int z;
			x++;
			/* doesn't parse 
		    do {
			z++;
			x--;
			} while ( x > 0 ) 
			*/
			return x;
		}
		];

	infer cons1 =
		`[foo(int x, int y) throws something {
			x++;
			return x;
		}
		];


	infer meth1A =
		`[int foo(int x, int y) {
			int z;
			x++;
			return x;
		}
		];

	infer var1 =  `[int x;];
	infer meth1B =
		`[int foo(int x, int y) {
			#(VarDec)var1
			x++;
			return x;
			}];

/* How do we fit Identifier in here???
	infer meth1C =
		`[int foo (int #(Identifier)id1, #(Identifier)id2) {
			int z;
			x++;
			return x;
		}];
*/
	infer stat1 =  `[x++;];
		
	infer meth1D =
		`[int foo(int x, int y) {
			int z;
			#(Statement) stat1
			return x;
		}];

	infer meth2 = `[Foo.Bar baz(int x, int y);];


	    
	System.out.println(meth1.unparse());
	System.out.println(cons1.unparse());
	System.out.println(meth1A.unparse());
	System.out.println(meth1B.unparse());
	System.out.println(meth1D.unparse());
	System.out.println(meth2.unparse());
    }
}
