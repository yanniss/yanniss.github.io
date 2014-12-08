package maj.ajTest;

import java.io.*;
import org.aspectj.compiler.base.*;
import org.aspectj.compiler.base.ast.*;
import org.aspectj.compiler.base.cst.*;
import org.aspectj.compiler.crosscuts.ast.*;

public class pointcuttest {

    public static void main(String myargs[]) {

	// Pointcut
	infer pct1 =
		`[call (public static void Foo.m(int,int,..))];

	infer pct2 =
		`[call (Foo.new(int))];

	infer pct3 =
		`[execution (void Foo.m(int))];

	infer pct4 =
		`[execution (Foo.new(int))];

	infer pct5 =
		`[initialization (Foo.new(int))];

	infer pct6 =
		`[preinitialization (Foo.new(int))];
	infer pct7 =
		`[staticinitialization (Foo.new(int))];

	infer pct8 =
		`[get (Foo x)];
	infer pct9 =
		`[set (Foo x)];

	infer pct10 =
		`[handler (IOException)];

	infer pct11 =
		`[adviceexecution ()];
	infer pct12 =
		`[within (Foo.Bar.*)];

	infer pct13 =
		`[withincode (void Foo.m(int))];
	infer pct14 =
		`[withincode (Foo.new(int))];
	infer pct15 =
		`[withincode (!private void Foo.*(int))];

	infer pct20 =
		`[cflow (call (void Foo.m(int)))];

	infer pct21 =
		`[cflowbelow (call (void Foo.m(int)))];

	infer pct24 =
		`[target (java.io.InputPort)];
	infer pct25 =
		`[args (java.io.InputPort, int)];
	infer pct26 =
		`[args (int, *)];
	infer pct27 =
		`[args (int, .., int)];

	infer pct28 =
		`[this (Point)];

	infer pct29 =
		`[if (x==y)];

	infer pct30 =
		`[!args (int, .., int)];
	infer pct31 =
	    `[args (int, .., int) && withincode (Foo.new(int))];
	infer pct32 =
		`[args (int, .., int) || withincode (Foo.new(int))];
	
	infer pct35 =
		`(Pointcut)[if (x==y)];

	System.out.println(pct1.unparse());
	System.out.println(pct2.unparse());
	System.out.println(pct3.unparse());
	System.out.println(pct4.unparse());
	System.out.println(pct5.unparse());
	System.out.println(pct6.unparse());
	System.out.println(pct7.unparse());
	System.out.println(pct8.unparse());
	System.out.println(pct9.unparse());
	System.out.println(pct10.unparse());
	System.out.println(pct11.unparse());
	System.out.println(pct12.unparse());
	System.out.println(pct13.unparse());
	System.out.println(pct14.unparse());
	System.out.println(pct15.unparse());
	System.out.println(pct20.unparse());
	System.out.println(pct21.unparse());
	System.out.println(pct24.unparse());
	System.out.println(pct25.unparse());
	System.out.println(pct26.unparse());
	System.out.println(pct27.unparse());
	System.out.println(pct28.unparse());
	System.out.println(pct29.unparse());
	System.out.println(pct30.unparse());
	System.out.println(pct31.unparse());
	System.out.println(pct32.unparse());
	System.out.println(pct35.unparse());

    }
}
