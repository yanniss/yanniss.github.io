package maj.ajTest;

import java.io.*;
import org.aspectj.compiler.base.*;
import org.aspectj.compiler.base.ast.*;
import org.aspectj.compiler.base.cst.*;
import org.aspectj.compiler.crosscuts.ast.*;

public class aspecttest {

    public static void main(String myargs[]) {

	// Aspect Definition
	infer aspect1 =
		`[aspect Foo {
			declare precedence: Security, Logging;

			before():get(int Foo.y) {}

			abstract pointcut pc(Object o);

			Object around(): call(int Foo.m(int)) {}

			int around(): call(int Foo.m(int)) {}

			public int x;

			int foo(int x, int y) {
				int z;
				x++;
				return x;
			}
		} ];
	infer pdec1 =
		`[private pointcut pc();];
	infer adv1 =
		`[before():get(int Foo.y) {}];
	infer var1 =  `[int x;];
	infer mods1 = `[public static];
	infer meth1 =
		`[int foo(int x, int y) {
		    //#(Modifier)mods1 int z;
			x++;
			return x;
		}
		];
	infer aspect2 =
		`[aspect Foo {
			#(DecPattern) pdec1
			#(AdviceDec) adv1
			#(PointcutDec) pdec1
			#(VarDec)var1
			#(MethodDec)meth1
			} ];

	System.out.println(aspect1.unparse());
	System.out.println(aspect2.unparse());
    }
}
